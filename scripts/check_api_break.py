#! /usr/bin/env python3
"""
Script to check for breaking changes in the MAVLink API.

This script checks for the following changes:
* Break: Removal/Rename of fields and enum entries
-      <field type="uint8_t" name="target">The system to be controlled.</field>
+      <field type="uint8_t" name="potato">The system to be controlled.</field>

* Break: Field type, message ID, or enum value mutations

* Allow: Removal/Addition of messages and enums
-    <message id="67" name="DATA_STREAM">
-      <description>Data stream status information.</description>
-      <field type="uint8_t" name="stream_id">The ID of the requested data stream</field>
-    </message>
"""
import json
import os
import subprocess
import sys
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Any, Dict, List, Optional, Tuple, Union

from lxml import etree


NameKey = Union['EnumKey', 'EnumEntryKey', 'MessageKey', 'FieldKey']

# Lifecycle statuses returned by collect_names().
WIP = "wip"
DEPRECATED = "deprecated"
STABLE = "stable"
_WIP = WIP
_DEPRECATED = DEPRECATED
_STABLE = STABLE


class IsRelativeOfBase(ABC):
    @abstractmethod
    def is_relative_of(self, other: 'NameKey') -> bool:
        pass

@dataclass(frozen=True)
class EnumKey(IsRelativeOfBase):
    enum_name: str

    def is_relative_of(self, other: 'NameKey') -> bool:
        return isinstance(other, EnumKey) and other.enum_name == self.enum_name

@dataclass(frozen=True)
class EnumEntryKey(IsRelativeOfBase):
    enum: EnumKey
    entry_name: str

    def is_relative_of(self, other: 'NameKey') -> bool:
        return self.enum.is_relative_of(other)

@dataclass(frozen=True)
class MessageKey(IsRelativeOfBase):
    message_name: str

    def is_relative_of(self, other: 'NameKey') -> bool:
        return isinstance(other, MessageKey) and other.message_name == self.message_name

@dataclass(frozen=True)
class FieldKey(IsRelativeOfBase):
    message: MessageKey
    field_name: str

    def is_relative_of(self, other: 'NameKey') -> bool:
        return self.message.is_relative_of(other)


def describe_key(name: NameKey) -> str:
    if isinstance(name, EnumKey):
        return f"enum {name.enum_name}"
    if isinstance(name, EnumEntryKey):
        return f"enum entry {name.enum.enum_name}.{name.entry_name}"
    if isinstance(name, MessageKey):
        return f"message {name.message_name}"
    if isinstance(name, FieldKey):
        return f"field {name.message.message_name}.{name.field_name}"
    return str(name)


def _lifecycle_status(element: etree._Element, parent_status: str = _STABLE) -> str:
    """Determine lifecycle status of a MAVLink XML element.

    WIP is inherited from parent (an entire message/enum being WIP means all
    its children are WIP).

    DEPRECATED is NOT inherited: a deprecated message still has a fixed, released
    wire format while it survives. Surviving fields of a deprecated message must
    not be mutated or removed. Only elements explicitly marked with <deprecated>
    (or an entire deprecated message/enum deleted as a whole) qualify.
    """
    if element.find("wip") is not None or parent_status == _WIP:
        return _WIP
    if element.find("deprecated") is not None:
        return _DEPRECATED
    return _STABLE


def collect_names(root: etree._Element) -> Tuple[Dict[NameKey, str], Dict[NameKey, Dict[str, Any]]]:
    """Collect names and wire-critical attributes (id, type, value) from a MAVLink XML root.

    The first dict maps each NameKey to its lifecycle status ('wip', 'deprecated',
    or 'stable').
    """
    names: Dict[NameKey, str] = {}
    attrs: Dict[NameKey, Dict[str, Any]] = {}

    for enum in root.findall(".//enum"):
        enum_name = enum.get("name")
        enum_status = _lifecycle_status(enum)
        enum_key = EnumKey(enum_name=enum_name)
        names[enum_key] = enum_status

        for entry in enum.findall("entry"):
            entry_name = entry.get("name")
            entry_status = _lifecycle_status(entry, enum_status)
            entry_key = EnumEntryKey(enum=enum_key, entry_name=entry_name)
            names[entry_key] = entry_status
            entry_value = entry.get("value")
            if entry_value is not None:
                attrs[entry_key] = {"value": entry_value}

    for msg in root.findall(".//message"):
        message_name = msg.get("name")
        message_status = _lifecycle_status(msg)
        message_key = MessageKey(message_name=message_name)
        names[message_key] = message_status
        message_id = msg.get("id")
        if message_id is not None:
            attrs[message_key] = {"id": message_id}

        for field in msg.findall("field"):
            field_name = field.get("name")
            field_status = _lifecycle_status(field, message_status)
            field_key = FieldKey(message=message_key, field_name=field_name)
            names[field_key] = field_status
            field_type = field.get("type")
            if field_type is not None:
                attrs[field_key] = {"type": field_type}

    return names, attrs


def get_base_commit() -> str:
    return subprocess.check_output(
        ["git", "merge-base", "origin/master", "HEAD"], text=True
    ).strip()

def get_changed_xml_files(base: str) -> List[str]:
    changed = subprocess.check_output(
        ["git", "diff", "--name-only", base], text=True
    ).splitlines()
    return [f for f in changed if f.endswith(".xml")]

def parse_xml(content: Union[str, bytes]) -> etree._Element:
    return etree.fromstring(content.encode() if isinstance(content, str) else content)


def get_pull_request_info() -> Optional[Tuple[str, int]]:
    """Return (repo_full_name, pr_number) when running in a PR context."""
    event_path = os.getenv("GITHUB_EVENT_PATH")
    if not event_path:
        return None

    try:
        with open(event_path, "r", encoding="utf-8") as event_file:
            event = json.load(event_file)
    except (OSError, json.JSONDecodeError):
        return None

    pr = event.get("pull_request")
    if not pr:
        return None

    base_repo = pr.get("base", {}).get("repo", {})
    repo_name = base_repo.get("full_name")
    pr_number = pr.get("number")

    if isinstance(repo_name, str) and isinstance(pr_number, int):
        return repo_name, pr_number

    return None


PR_COMMENT_ARTIFACT_DIR = "api-break-comment"


def write_pr_comment_artifact(body: str) -> bool:
    """Write the comment body to disk as a build artifact.

    This job runs against untrusted PR content (it checks out and executes
    the PR branch's own script/tests), and for `pull_request` runs triggered
    from a fork, GITHUB_TOKEN is always read-only regardless of the
    permissions granted to this workflow. So this job must never hold a
    token, and must never try to call the GitHub API directly.

    Instead it writes the comment to disk; a separate, trusted workflow
    (triggered by `workflow_run`, which does not check out or execute any
    PR content) picks up this artifact and posts the actual comment with a
    token that does have write access. That workflow looks up the target
    PR number itself from trustworthy `workflow_run` event data rather than
    from anything written here, since this job's own code (this script and
    its workflow file) is exactly the untrusted PR content it can't hold a
    token against - a PR number written here could just as easily be
    forged.
    """
    pr_info = get_pull_request_info()
    if not pr_info:
        print("No pull request context found, skipping PR comment artifact.")
        return False

    _repo, pr_number = pr_info

    os.makedirs(PR_COMMENT_ARTIFACT_DIR, exist_ok=True)
    with open(os.path.join(PR_COMMENT_ARTIFACT_DIR, "comment.md"), "w", encoding="utf-8") as comment_file:
        comment_file.write(body)

    print(f"Wrote PR comment artifact for #{pr_number}.")
    return True


def describe_mutation(key: NameKey, old_a: Dict[str, Any], new_a: Dict[str, Any]) -> str:
    """Describe a wire-breaking attribute mutation for a given key."""
    changes = []
    for attr in sorted(set(old_a) | set(new_a)):
        old_val = old_a.get(attr)
        new_val = new_a.get(attr)
        if old_val != new_val:
            changes.append(f"{attr}: {old_val} -> {new_val}")
    return f"{describe_key(key)} ({', '.join(changes)})"


def find_mutations(
    old_names: Dict[NameKey, str],
    new_names: Dict[NameKey, str],
    old_attrs: Dict[NameKey, Dict[str, Any]],
    new_attrs: Dict[NameKey, Dict[str, Any]],
) -> List[str]:
    """Return description strings for wire-breaking attribute mutations (type, id, value).

    Only WIP items are exempt — deprecated items are still wire-released and
    changing their id/type/value is a genuine wire break.
    """
    mutation_descs: List[str] = []
    for key in old_names:
        if key not in new_names:
            continue
        if _WIP in (old_names.get(key), new_names.get(key)):
            continue  # skip WIP items; deprecated items are still wire-breaking
        old_a = old_attrs.get(key)
        new_a = new_attrs.get(key)
        if old_a is None or new_a is None:
            continue
        if old_a != new_a:
            mutation_descs.append(describe_mutation(key, old_a, new_a))
    return mutation_descs


# Identifies the bot's own comment across runs so it can be updated in place
# instead of accumulating a new comment on every push. Must stay in sync with
# the marker check in post_api_break_comment.yml.
COMMENT_MARKER = "<!-- mavlink-api-break-check -->"


def build_removal_comment(
    removed_by_file: Dict[str, List[NameKey]],
    mutations_by_file: Optional[Dict[str, List[str]]] = None,
    deprecated_by_file: Optional[Dict[str, List[NameKey]]] = None,
) -> str:
    """Format a PR comment listing removed messages/enums and attribute mutations."""
    lines: List[str] = [COMMENT_MARKER, ""]

    if removed_by_file:
        lines.extend(["Detected removed MAVLink messages or enums:", ""])
        for xml in sorted(removed_by_file):
            lines.append(f"- `{xml}`")
            for name in sorted(removed_by_file[xml], key=describe_key):
                lines.append(f"  - Removed {describe_key(name)}")
        lines.append("")

    if deprecated_by_file:
        lines.extend(["Detected removal of deprecated items (expected lifecycle cleanup):", ""])
        for xml in sorted(deprecated_by_file):
            lines.append(f"- `{xml}`")
            for name in sorted(deprecated_by_file[xml], key=describe_key):
                lines.append(f"  - Removed deprecated {describe_key(name)}")
        lines.append("")

    if mutations_by_file:
        lines.extend(["Detected wire-breaking attribute mutations:", ""])
        for xml in sorted(mutations_by_file):
            lines.append(f"- `{xml}`")
            for desc in mutations_by_file[xml]:
                lines.append(f"  - Changed {desc}")
        lines.append("")

    lines.append("If these changes are intentional, please confirm in the PR.")
    return "\n".join(lines)


def main() -> None:
    base = get_base_commit()
    xml_files = get_changed_xml_files(base)
    if not xml_files:
        print("No XML files changed.")
        return

    removals_for_comment: Dict[str, List[NameKey]] = {}
    deprecated_for_comment: Dict[str, List[NameKey]] = {}
    mutations_for_comment: Dict[str, List[str]] = {}
    breaking_by_file: Dict[str, List[str]] = {}

    for xml in xml_files:
        if xml.endswith("development.xml"):
            continue

        try:
            old_content = subprocess.check_output(
                ["git", "show", f"{base}:{xml}"], text=True
            )
            new_content = open(xml).read()
        except subprocess.CalledProcessError:
            continue  # new file or removed, ignore

        old_root = parse_xml(old_content)
        new_root = parse_xml(new_content)

        old_names, old_attrs = collect_names(old_root)
        new_names, new_attrs = collect_names(new_root)

        # If field is not in new names, it was removed
        removed = [n for n in old_names if n not in new_names]

        # All messages and enums removed in this diff (stable or deprecated)
        removed_enum_or_message = [item for item in removed if isinstance(item, (MessageKey, EnumKey))]

        # Stable whole messages/enums removed go to comment for confirmation
        stable_removed_enum_or_message = [item for item in removed_enum_or_message if old_names.get(item) == _STABLE]
        if stable_removed_enum_or_message:
            removals_for_comment[xml] = stable_removed_enum_or_message

        # Deprecated whole messages/enums, or deprecated standalone fields/entries in surviving parents
        deprecated_for_comment_items = [
            item for item in removed
            if old_names.get(item) == _DEPRECATED
            and (isinstance(item, (MessageKey, EnumKey)) or not any(item.is_relative_of(parent) for parent in removed_enum_or_message))
        ]
        if deprecated_for_comment_items:
            deprecated_for_comment[xml] = deprecated_for_comment_items

        # Confirmed breaking: stable items whose parent message/enum was NOT removed
        confirmed_breaking = [
            b for b in removed
            if old_names.get(b) == _STABLE
            and not any(b.is_relative_of(parent) for parent in removed_enum_or_message)
        ]

        breaking_descs: List[str] = []
        for name in confirmed_breaking:
            breaking_descs.append(f"Removed {describe_key(name)}")

        # Check for wire-breaking attribute mutations (type, id, value).
        mutation_descs = find_mutations(old_names, new_names, old_attrs, new_attrs)
        for desc in mutation_descs:
            breaking_descs.append(f"Changed {desc}")

        if mutation_descs:
            mutations_for_comment[xml] = mutation_descs

        if breaking_descs:
            breaking_by_file[xml] = breaking_descs

    if removals_for_comment or deprecated_for_comment or mutations_for_comment:
        if removals_for_comment:
            print("Message or enum removals detected.")
            for xml, removals in removals_for_comment.items():
                print(f" - {xml}:")
                for removal in removals:
                    print(f"   - {describe_key(removal)}")
        if deprecated_for_comment:
            print("Deprecated item removals detected (expected lifecycle cleanup).")
            for xml, removals in deprecated_for_comment.items():
                print(f" - {xml}:")
                for removal in removals:
                    print(f"   - {describe_key(removal)}")
        if mutations_for_comment:
            print("Wire-breaking attribute mutations detected.")
            for xml, descs in mutations_for_comment.items():
                print(f" - {xml}:")
                for desc in descs:
                    print(f"   - {desc}")

        write_pr_comment_artifact(build_removal_comment(removals_for_comment, mutations_for_comment, deprecated_for_comment))

    if breaking_by_file:
        for xml, descs in breaking_by_file.items():
            print(f"Name removals/changes detected in {xml}:")
            for desc in descs:
                print(f" - {desc}")
        sys.exit(1)

if __name__ == "__main__":
    main()

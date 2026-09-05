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
import argparse
import json
import os
import subprocess
import sys
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Any, Dict, List, Optional, Tuple, Union

from lxml import etree


NameKey = Union['EnumKey', 'EnumEntryKey', 'MessageKey', 'FieldKey']


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


def collect_names(root: etree._Element) -> Tuple[Dict[NameKey, bool], Dict[NameKey, Dict[str, Any]]]:
    """Collect names and wire-critical attributes (id, type, value) from a MAVLink XML root."""
    names = {}
    attrs = {}

    for enum in root.findall(".//enum"):
        enum_name = enum.get("name")
        enum_is_wip = enum.find("wip") is not None
        enum_key = EnumKey(enum_name=enum_name)
        names[enum_key] = enum_is_wip

        for entry in enum.findall("entry"):
            entry_name = entry.get("name")
            entry_is_wip = enum_is_wip or entry.find("wip") is not None
            entry_key = EnumEntryKey(enum=enum_key, entry_name=entry_name)
            names[entry_key] = entry_is_wip
            entry_value = entry.get("value")
            if entry_value is not None:
                attrs[entry_key] = {"value": entry_value}

    for msg in root.findall(".//message"):
        message_name = msg.get("name")
        message_is_wip = msg.find("wip") is not None
        message_key = MessageKey(message_name=message_name)
        names[message_key] = message_is_wip
        message_id = msg.get("id")
        if message_id is not None:
            attrs[message_key] = {"id": message_id}

        # Fields before <extensions/> are re-sorted by size when a message is
        # serialized, so their order in the XML is not wire-visible and must not
        # be compared. Extension fields are the opposite: they are never
        # reordered, and their serialization order is defined by the XML
        # definition order, so position is part of the wire format for them.
        # Record it for those fields only.
        extension_index = None
        for child in msg:
            if child.tag == "extensions":
                extension_index = 0
                continue
            if child.tag != "field":
                continue

            field = child
            field_name = field.get("name")
            field_is_wip = message_is_wip or field.find("wip") is not None
            field_key = FieldKey(message=message_key, field_name=field_name)
            names[field_key] = field_is_wip
            field_attrs: Dict[str, Any] = {}
            field_type = field.get("type")
            if field_type is not None:
                field_attrs["type"] = field_type
            if extension_index is not None:
                field_attrs["extension_index"] = extension_index
                extension_index += 1
            if field_attrs:
                attrs[field_key] = field_attrs

    return names, attrs


def _merge_base(ref: str) -> Optional[str]:
    try:
        output = subprocess.check_output(
            ["git", "merge-base", ref, "HEAD"],
            text=True,
            stderr=subprocess.DEVNULL,
        ).strip()
        return output or None
    except (subprocess.CalledProcessError, FileNotFoundError):
        return None


def get_ci_event_base_sha() -> Optional[str]:
    """Return the base commit SHA GitHub Actions computed for this run, if any.

    For `pull_request`/`push` events this is authoritative: GitHub derives it
    from the real PR/push relationship on its own servers, not from local
    remotes, so it's correct even in a fork's CI with no `upstream` remote
    and no risk of a stale local ref. Because of that, any failure to read it
    here is a hard error rather than a silent fall-through to guessing -
    getting the base wrong in CI is exactly what this must avoid.

    Returns None for triggers with no such payload (e.g. workflow_dispatch),
    or when not running in GitHub Actions at all, so callers fall back to the
    best-effort local resolution.
    """
    event_path = os.getenv("GITHUB_EVENT_PATH")
    if not event_path:
        return None

    event_name = os.getenv("GITHUB_EVENT_NAME")
    if event_name not in ("pull_request", "push"):
        return None

    try:
        with open(event_path, "r", encoding="utf-8") as event_file:
            event = json.load(event_file)
    except (OSError, json.JSONDecodeError) as exc:
        raise RuntimeError(
            f"Running in CI for a '{event_name}' event but couldn't read/parse "
            f"GITHUB_EVENT_PATH ({event_path}): {exc}"
        )

    if event_name == "pull_request":
        sha = event.get("pull_request", {}).get("base", {}).get("sha")
        if not sha:
            raise RuntimeError(
                "Running in CI for a pull_request event but 'pull_request.base.sha' "
                "is missing from the event payload."
            )
        return sha

    sha = event.get("before")
    if not sha:
        raise RuntimeError(
            "Running in CI for a push event but 'before' is missing from the event payload."
        )
    if set(sha) == {"0"}:
        raise RuntimeError(
            "This push has no prior commit on the branch (first push of a new branch), "
            "so there is no base to diff against. Pass --base explicitly to check it anyway."
        )
    return sha


def get_base_commit(base_override: Optional[str] = None) -> str:
    """Determine the base commit to diff against.

    Checks in order:
    1. Explicit --base override - required to resolve; errors if it doesn't.
    2. The GitHub Actions event payload (pull_request.base.sha / push.before) -
       authoritative when present; errors rather than falling through, since
       this is the path that CI correctness depends on.
    3. Local/manual use only: a best-effort guess across common remote and
       branch names, fetching each candidate remote first to reduce
       staleness. Every match here is printed and flagged as unverified,
       since none of these carry the same guarantee as (1) or (2).
    """
    if base_override:
        resolved = _merge_base(base_override)
        if resolved is None:
            raise RuntimeError(
                f"--base {base_override!r} does not resolve to a commit reachable from HEAD."
            )
        print(f"Diffing against explicit --base {base_override!r} ({resolved}).", file=sys.stderr)
        return resolved

    ci_sha = get_ci_event_base_sha()
    if ci_sha is not None:
        resolved = _merge_base(ci_sha)
        if resolved is None:
            raise RuntimeError(
                f"Running in CI but the event's base commit {ci_sha!r} isn't reachable from "
                "HEAD - checkout history may be too shallow (check fetch-depth)."
            )
        print(f"Diffing against CI event base commit {resolved}.", file=sys.stderr)
        return resolved

    env_base = os.getenv("MAVLINK_BASE_REF") or os.getenv("GITHUB_BASE_REF")
    candidates: List[str] = []
    if env_base:
        candidates.extend([f"origin/{env_base}", f"upstream/{env_base}", env_base])

    candidates.extend([
        "upstream/master",
        "origin/master",
        "master",
        "upstream/main",
        "origin/main",
        "main",
    ])

    remotes = {ref.split("/", 1)[0] for ref in candidates if "/" in ref}
    for remote in remotes:
        try:
            subprocess.run(
                ["git", "fetch", "--quiet", remote],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
        except (subprocess.SubprocessError, OSError):
            pass

    for ref in candidates:
        resolved = _merge_base(ref)
        if resolved is not None:
            print(
                f"Diffing against best-effort base '{ref}' ({resolved}). This was guessed, "
                "not verified - pass --base explicitly if this looks wrong.",
                file=sys.stderr,
            )
            return resolved

    resolved = _merge_base("HEAD~1")
    if resolved is not None:
        print(
            f"Diffing against HEAD~1 ({resolved}) as a last resort - no other base could be "
            "determined. This is very likely NOT the right comparison; pass --base explicitly.",
            file=sys.stderr,
        )
        return resolved

    raise RuntimeError(
        "Could not determine base commit. Please specify a base branch using "
        "--base <ref> or the MAVLINK_BASE_REF environment variable."
    )

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
    old_names: Dict[NameKey, bool],
    new_names: Dict[NameKey, bool],
    old_attrs: Dict[NameKey, Dict[str, Any]],
    new_attrs: Dict[NameKey, Dict[str, Any]],
) -> List[str]:
    """Return description strings for wire-breaking attribute mutations (type, id, value)."""
    mutation_descs: List[str] = []
    for key in old_names:
        if key not in new_names:
            continue
        if old_names.get(key) or new_names.get(key):
            continue  # skip WIP items
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
    parser = argparse.ArgumentParser(description="Check for breaking changes in MAVLink XML definitions.")
    parser.add_argument(
        "-b",
        "--base",
        help="Base commit or branch to diff against (default: auto-detected)",
        default=None,
    )
    args = parser.parse_args()

    base = get_base_commit(args.base)
    xml_files = get_changed_xml_files(base)
    if not xml_files:
        print("No XML files changed.")
        return

    removals_for_comment: Dict[str, List[NameKey]] = {}
    mutations_for_comment: Dict[str, List[str]] = {}
    breaking_by_file: Dict[str, List[str]] = {}

    for xml in xml_files:
        if xml.endswith("development.xml"):
            continue

        try:
            old_content = subprocess.check_output(
                ["git", "show", f"{base}:{xml}"], text=True
            )
        except subprocess.CalledProcessError:
            continue  # new file, nothing to compare against

        try:
            new_content = open(xml).read()
        except FileNotFoundError:
            print(f"Skipped {xml}: removed since base.")
            continue

        old_root = parse_xml(old_content)
        new_root = parse_xml(new_content)

        old_names, old_attrs = collect_names(old_root)
        new_names, new_attrs = collect_names(new_root)

        # If field is not in new names, it was removed
        removed = [n for n in old_names if n not in new_names]
        # If removed field is not wip, it is a breaking change
        breaking_candidates = [r for r in removed if not old_names.get(r)]

        # Moving an entire message/enum (e.g. from a dialect to common) can be ok, so separate those cases
        removed_enum_or_message = [item for item in breaking_candidates if isinstance(item, (MessageKey, EnumKey))]

        if removed_enum_or_message:
            removals_for_comment[xml] = removed_enum_or_message

        confirmed_breaking = [b for b in breaking_candidates if not any(b.is_relative_of(item) for item in removed_enum_or_message)]

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

    if removals_for_comment or mutations_for_comment:
        if removals_for_comment:
            print("Message or enum removals detected.")
            for xml, removals in removals_for_comment.items():
                print(f" - {xml}:")
                for removal in removals:
                    print(f"   - {describe_key(removal)}")
        if mutations_for_comment:
            print("Wire-breaking attribute mutations detected.")
            for xml, descs in mutations_for_comment.items():
                print(f" - {xml}:")
                for desc in descs:
                    print(f"   - {desc}")

        write_pr_comment_artifact(build_removal_comment(removals_for_comment, mutations_for_comment))

    if breaking_by_file:
        for xml, descs in breaking_by_file.items():
            print(f"Name removals/changes detected in {xml}:")
            for desc in descs:
                print(f" - {desc}")
        sys.exit(1)

if __name__ == "__main__":
    main()

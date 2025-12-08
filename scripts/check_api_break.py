#! /usr/bin/env python3
"""
Script to check for breaking changes in the MAVLink API.

This script checks for the following changes:
* Break: Removal/Rename of fields and enum entries
-      <field type="uint8_t" name="target">The system to be controlled.</field>
+      <field type="uint8_t" name="potato">The system to be controlled.</field>

* Allow: Removal/Addition of messages and enums
-    <message id="67" name="DATA_STREAM">
-      <description>Data stream status information.</description>
-      <field type="uint8_t" name="stream_id">The ID of the requested data stream</field>
-    </message>
"""
import subprocess
import sys
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Dict, List, Union

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


def collect_names(root: etree._Element) -> Dict[NameKey, bool]:
    """Collect all schema items keyed by their path and mark if they are WIP."""
    names = {}

    for enum in root.findall(".//enum"):
        enum_name = enum.get("name")
        enum_is_wip = enum.find("wip") is not None
        enum_key = EnumKey(enum_name=enum_name)
        names[enum_key] = enum_is_wip

        for entry in enum.findall("entry"):
            entry_name = entry.get("name")
            entry_is_wip = enum_is_wip or entry.find("wip") is not None
            names[EnumEntryKey(enum=enum_key, entry_name=entry_name)] = entry_is_wip

    for msg in root.findall(".//message"):
        message_name = msg.get("name")
        message_is_wip = msg.find("wip") is not None
        message_key = MessageKey(message_name=message_name)
        names[message_key] = message_is_wip

        for field in msg.findall("field"):
            field_name = field.get("name")
            field_is_wip = message_is_wip or field.find("wip") is not None
            names[FieldKey(message=message_key, field_name=field_name)] = field_is_wip

    return names


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

def main() -> None:
    base = get_base_commit()
    xml_files = get_changed_xml_files(base)
    if not xml_files:
        print("No XML files changed.")
        return

    for xml in xml_files:
        if xml.endswith("development.xml"):
            continue

        try:
            old_content = subprocess.check_output(
                ["git", "show", f"{base}:{xml}"], text=True
            )
            new_content = open(xml).read()
        except subprocess.CalledProcessError:
            continue # new file or removed, ignore

        old_root = parse_xml(old_content)
        new_root = parse_xml(new_content)

        old_names = collect_names(old_root)
        new_names = collect_names(new_root)

        # If field is not in new names, it was removed
        removed = [n for n in old_names if n not in new_names]
        # If removed field is not wip, it is a breaking change
        breaking = [r for r in removed if not old_names.get(r)]

        # We should be fine if an entire message was removed
        removed_enum_or_message = [item for item in breaking if isinstance(item, MessageKey) or isinstance(item, EnumKey)]
        breaking = [b for b in breaking if not any(b.is_relative_of(item) for item in removed_enum_or_message)]

        if breaking:
            print(f"Name removals detected in {xml}:")
            for name in breaking:
                print(f" - {name}")
            sys.exit(1)

    print("No name changes detected.")

if __name__ == "__main__":
    main()

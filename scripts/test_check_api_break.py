#! /usr/bin/env python3
"""
Unit tests for check_api_break.py.

Exercises the schema-diffing logic (collect_names, find_mutations, and the
message/enum/field removal detection used by main()) against small in-memory
XML fixtures, so regressions in wire-breaking-change detection are caught in
CI instead of relying on manual testing against real PRs.
"""
import importlib.util
import os
import unittest

_MODULE_PATH = os.path.join(os.path.dirname(__file__), "check_api_break.py")
_spec = importlib.util.spec_from_file_location("check_api_break", _MODULE_PATH)
check_api_break = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(check_api_break)

parse_xml = check_api_break.parse_xml
collect_names = check_api_break.collect_names
find_mutations = check_api_break.find_mutations
build_removal_comment = check_api_break.build_removal_comment
MessageKey = check_api_break.MessageKey
EnumKey = check_api_break.EnumKey
FieldKey = check_api_break.FieldKey
EnumEntryKey = check_api_break.EnumEntryKey
STABLE = check_api_break.STABLE
DEPRECATED = check_api_break.DEPRECATED
WIP = check_api_break.WIP


BASE_XML = """<mavlink>
<enums>
<enum name="MY_ENUM">
  <entry name="MY_ENUM_A" value="1"/>
  <entry name="MY_ENUM_B" value="2"/>
</enum>
</enums>
<messages>
<message id="10" name="MY_MSG">
  <field type="uint8_t" name="foo">desc</field>
</message>
</messages>
</mavlink>"""


def mutations_between(old_xml: str, new_xml: str):
    old_names, old_attrs = collect_names(parse_xml(old_xml))
    new_names, new_attrs = collect_names(parse_xml(new_xml))
    return find_mutations(old_names, new_names, old_attrs, new_attrs)


def removed_between(old_xml: str, new_xml: str):
    """Replicate main()'s breaking removal-candidate logic (stable names missing in new)."""
    old_names, _ = collect_names(parse_xml(old_xml))
    new_names, _ = collect_names(parse_xml(new_xml))
    removed = [n for n in old_names if n not in new_names]
    return [r for r in removed if old_names.get(r) == STABLE]


def confirmed_breaking_between(old_xml: str, new_xml: str):
    """Replicate main()'s confirmed breaking removals (stable items in surviving parents)."""
    old_names, _ = collect_names(parse_xml(old_xml))
    new_names, _ = collect_names(parse_xml(new_xml))
    removed = [n for n in old_names if n not in new_names]
    all_removed_parents = [item for item in removed if isinstance(item, (MessageKey, EnumKey))]
    return [
        b for b in removed
        if old_names.get(b) == STABLE
        and not any(b.is_relative_of(parent) for parent in all_removed_parents)
    ]


def deprecated_removed_between(old_xml: str, new_xml: str):
    """Replicate main()'s deprecated removal candidates (deprecated names missing in new)."""
    old_names, _ = collect_names(parse_xml(old_xml))
    new_names, _ = collect_names(parse_xml(new_xml))
    removed = [n for n in old_names if n not in new_names]
    return [r for r in removed if old_names.get(r) == DEPRECATED]


class FindMutationsTests(unittest.TestCase):
    def test_field_type_change_detected(self):
        new_xml = BASE_XML.replace('type="uint8_t" name="foo"', 'type="uint16_t" name="foo"')
        mutations = mutations_between(BASE_XML, new_xml)
        self.assertEqual(mutations, ["field MY_MSG.foo (type: uint8_t -> uint16_t)"])

    def test_message_id_change_detected(self):
        new_xml = BASE_XML.replace('id="10" name="MY_MSG"', 'id="99" name="MY_MSG"')
        mutations = mutations_between(BASE_XML, new_xml)
        self.assertEqual(mutations, ["message MY_MSG (id: 10 -> 99)"])

    def test_enum_value_change_detected(self):
        new_xml = BASE_XML.replace('name="MY_ENUM_B" value="2"', 'name="MY_ENUM_B" value="999"')
        mutations = mutations_between(BASE_XML, new_xml)
        self.assertEqual(mutations, ["enum entry MY_ENUM.MY_ENUM_B (value: 2 -> 999)"])

    def test_rename_is_not_a_mutation(self):
        new_xml = BASE_XML.replace('name="foo"', 'name="bar"')
        self.assertEqual(mutations_between(BASE_XML, new_xml), [])

    def test_no_change_yields_no_mutations(self):
        self.assertEqual(mutations_between(BASE_XML, BASE_XML), [])

    def test_wip_field_type_change_is_skipped(self):
        old_xml = BASE_XML.replace(
            '<field type="uint8_t" name="foo">desc</field>',
            '<field type="uint8_t" name="foo">desc<wip/></field>',
        )
        new_xml = old_xml.replace('type="uint8_t" name="foo"', 'type="uint16_t" name="foo"')
        self.assertEqual(mutations_between(old_xml, new_xml), [])

    def test_wip_only_in_new_is_also_skipped(self):
        new_xml = BASE_XML.replace('type="uint8_t" name="foo"', 'type="uint16_t" name="foo"')
        new_xml = new_xml.replace(
            '<field type="uint16_t" name="foo">desc</field>',
            '<field type="uint16_t" name="foo">desc<wip/></field>',
        )
        self.assertEqual(mutations_between(BASE_XML, new_xml), [])

    def test_enum_value_made_implicit_is_a_known_false_negative(self):
        # Known limitation (see PR #2531 review): removing an explicit value=
        # (making the entry auto-numbered) is currently NOT detected as a
        # mutation. This test documents today's behavior so a future fix to
        # handle auto-numbering intentionally updates this test rather than
        # silently changing behavior.
        new_xml = BASE_XML.replace(' value="2"', '')
        self.assertEqual(mutations_between(BASE_XML, new_xml), [])


class RemovalDetectionTests(unittest.TestCase):
    def test_message_removal_detected(self):
        new_xml = "<mavlink><enums></enums><messages></messages></mavlink>"
        removed = removed_between(BASE_XML, new_xml)
        self.assertIn(MessageKey(message_name="MY_MSG"), removed)
        self.assertIn(EnumKey(enum_name="MY_ENUM"), removed)

    def test_field_removal_within_surviving_message_detected(self):
        new_xml = BASE_XML.replace(
            '<field type="uint8_t" name="foo">desc</field>', ""
        )
        removed = removed_between(BASE_XML, new_xml)
        self.assertIn(FieldKey(message=MessageKey(message_name="MY_MSG"), field_name="foo"), removed)

    def test_enum_entry_removal_detected(self):
        new_xml = BASE_XML.replace('<entry name="MY_ENUM_B" value="2"/>', "")
        removed = removed_between(BASE_XML, new_xml)
        self.assertIn(
            EnumEntryKey(enum=EnumKey(enum_name="MY_ENUM"), entry_name="MY_ENUM_B"), removed
        )

    def test_wip_removal_is_not_a_breaking_candidate(self):
        old_xml = BASE_XML.replace(
            '<field type="uint8_t" name="foo">desc</field>',
            '<field type="uint8_t" name="foo">desc<wip/></field>',
        )
        new_xml = old_xml.replace(
            '<field type="uint8_t" name="foo">desc<wip/></field>', ""
        )
        removed = removed_between(old_xml, new_xml)
        self.assertNotIn(FieldKey(message=MessageKey(message_name="MY_MSG"), field_name="foo"), removed)


class DeprecatedExemptionTests(unittest.TestCase):
    """Deprecated items: removals are non-breaking (routed to PR comment), but mutations still break."""

    DEPRECATED_ENTRY_XML = """<mavlink>
<enums>
<enum name="MAV_CMD">
  <entry name="MAV_CMD_DO_MOUNT_CONFIGURE" value="204">
    <deprecated since="2024-06" replaced_by="GIMBAL_MANAGER_CONFIGURE"/>
    <description>deprecated</description>
  </entry>
  <entry name="MAV_CMD_DO_SET_MODE" value="176">
    <description>Set system mode.</description>
  </entry>
</enum>
</enums>
<messages/>
</mavlink>"""

    def test_deprecated_entry_removal_is_not_breaking(self):
        new_xml = self.DEPRECATED_ENTRY_XML.replace(
            '  <entry name="MAV_CMD_DO_MOUNT_CONFIGURE" value="204">\n'
            '    <deprecated since="2024-06" replaced_by="GIMBAL_MANAGER_CONFIGURE"/>\n'
            '    <description>deprecated</description>\n'
            '  </entry>\n', ''
        )
        removed = removed_between(self.DEPRECATED_ENTRY_XML, new_xml)
        self.assertNotIn(
            EnumEntryKey(enum=EnumKey(enum_name="MAV_CMD"), entry_name="MAV_CMD_DO_MOUNT_CONFIGURE"),
            removed,
        )
        deprecated = deprecated_removed_between(self.DEPRECATED_ENTRY_XML, new_xml)
        self.assertIn(
            EnumEntryKey(enum=EnumKey(enum_name="MAV_CMD"), entry_name="MAV_CMD_DO_MOUNT_CONFIGURE"),
            deprecated,
        )

    def test_deprecated_message_removal_is_not_breaking(self):
        old_xml = """<mavlink>
<enums/>
<messages>
<message id="10" name="OLD_MSG">
  <deprecated since="2020-01" replaced_by="NEW_MSG"/>
  <field type="uint8_t" name="foo">desc</field>
</message>
</messages>
</mavlink>"""
        new_xml = "<mavlink><enums/><messages/></mavlink>"
        # MessageKey is deprecated, so it is not a breaking removal candidate
        candidates = removed_between(old_xml, new_xml)
        self.assertNotIn(MessageKey(message_name="OLD_MSG"), candidates)
        # And its fields do not trigger breaking errors because the parent message was removed
        breaking = confirmed_breaking_between(old_xml, new_xml)
        self.assertNotIn(
            FieldKey(message=MessageKey(message_name="OLD_MSG"), field_name="foo"),
            breaking,
        )
        deprecated = deprecated_removed_between(old_xml, new_xml)
        self.assertIn(MessageKey(message_name="OLD_MSG"), deprecated)

    def test_deprecated_entry_value_mutation_is_still_breaking(self):
        """Deprecated items are wire-released; mutating wire attributes (value/id/type) still breaks."""
        new_xml = self.DEPRECATED_ENTRY_XML.replace('value="204"', 'value="999"')
        mutations = mutations_between(self.DEPRECATED_ENTRY_XML, new_xml)
        self.assertEqual(
            mutations,
            ["enum entry MAV_CMD.MAV_CMD_DO_MOUNT_CONFIGURE (value: 204 -> 999)"],
        )

    def test_non_deprecated_removal_still_breaks(self):
        """Regression guard: normal (non-deprecated, non-wip) removals must still be caught."""
        new_xml = self.DEPRECATED_ENTRY_XML.replace(
            '<entry name="MAV_CMD_DO_SET_MODE" value="176">\n'
            '    <description>Set system mode.</description>\n'
            '  </entry>\n', ''
        )
        removed = removed_between(self.DEPRECATED_ENTRY_XML, new_xml)
        self.assertIn(
            EnumEntryKey(enum=EnumKey(enum_name="MAV_CMD"), entry_name="MAV_CMD_DO_SET_MODE"),
            removed,
        )

    def test_deprecated_field_in_surviving_message_is_not_breaking(self):
        old_xml = """<mavlink>
<enums/>
<messages>
<message id="10" name="MY_MSG">
  <field type="uint8_t" name="keep">desc</field>
  <field type="uint8_t" name="drop">
    <deprecated since="2023-01" replaced_by="keep"/>
    desc
  </field>
</message>
</messages>
</mavlink>"""
        new_xml = old_xml.replace(
            '  <field type="uint8_t" name="drop">\n'
            '    <deprecated since="2023-01" replaced_by="keep"/>\n'
            '    desc\n'
            '  </field>\n', ''
        )
        removed = removed_between(old_xml, new_xml)
        self.assertNotIn(
            FieldKey(message=MessageKey(message_name="MY_MSG"), field_name="drop"),
            removed,
        )
        deprecated = deprecated_removed_between(old_xml, new_xml)
        self.assertIn(
            FieldKey(message=MessageKey(message_name="MY_MSG"), field_name="drop"),
            deprecated,
        )

    def test_deprecated_removal_comment_formatting(self):
        """Deprecated removals are formatted in the PR comment for maintainer confirmation."""
        entry = EnumEntryKey(enum=EnumKey(enum_name="MAV_CMD"), entry_name="MAV_CMD_DO_MOUNT_CONFIGURE")
        comment = build_removal_comment(
            removed_by_file={},
            mutations_by_file=None,
            deprecated_by_file={"common.xml": [entry]},
        )
        self.assertIn("Detected removal of deprecated items (expected lifecycle cleanup):", comment)
        self.assertIn("- `common.xml`", comment)
        self.assertIn("Removed deprecated enum entry MAV_CMD.MAV_CMD_DO_MOUNT_CONFIGURE", comment)

    def test_surviving_deprecated_message_field_removal_is_breaking(self):
        """Deprecated status is NOT inherited: removing a field from a surviving deprecated message still breaks."""
        old_xml = """<mavlink>
<enums/>
<messages>
<message id="300" name="PROTOCOL_VERSION">
  <deprecated since="2025-11" replaced_by="Nothing"/>
  <field type="uint16_t" name="version">desc</field>
  <field type="uint16_t" name="min_version">desc</field>
</message>
</messages>
</mavlink>"""
        new_xml = old_xml.replace('  <field type="uint16_t" name="version">desc</field>\n', '')
        removed = removed_between(old_xml, new_xml)
        self.assertIn(
            FieldKey(message=MessageKey(message_name="PROTOCOL_VERSION"), field_name="version"),
            removed,
        )

    def test_surviving_deprecated_message_field_mutation_is_breaking(self):
        """Mutating a field type in a surviving deprecated message still breaks."""
        old_xml = """<mavlink>
<enums/>
<messages>
<message id="300" name="PROTOCOL_VERSION">
  <deprecated since="2025-11" replaced_by="Nothing"/>
  <field type="uint16_t" name="version">desc</field>
</message>
</messages>
</mavlink>"""
        new_xml = old_xml.replace('type="uint16_t" name="version"', 'type="uint32_t" name="version"')
        mutations = mutations_between(old_xml, new_xml)
        self.assertEqual(
            mutations,
            ["field PROTOCOL_VERSION.version (type: uint16_t -> uint32_t)"],
        )

    def test_newly_added_deprecated_does_not_self_exempt_wire_mutation(self):
        """Adding <deprecated> in the same diff as an enum value mutation does NOT exempt it."""
        old_xml = """<mavlink>
<enums>
<enum name="MY_ENUM">
  <entry name="ENTRY_A" value="1"/>
</enum>
</enums>
<messages/>
</mavlink>"""
        new_xml = """<mavlink>
<enums>
<enum name="MY_ENUM">
  <entry name="ENTRY_A" value="999">
    <deprecated since="2026-09"/>
  </entry>
</enum>
</enums>
<messages/>
</mavlink>"""
        mutations = mutations_between(old_xml, new_xml)
        self.assertEqual(
            mutations,
            ["enum entry MY_ENUM.ENTRY_A (value: 1 -> 999)"],
        )


if __name__ == "__main__":
    unittest.main()

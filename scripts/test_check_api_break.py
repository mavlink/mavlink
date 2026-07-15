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
MessageKey = check_api_break.MessageKey
EnumKey = check_api_break.EnumKey
FieldKey = check_api_break.FieldKey
EnumEntryKey = check_api_break.EnumEntryKey


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
    """Replicate main()'s removal-candidate logic (non-WIP names missing in new)."""
    old_names, _ = collect_names(parse_xml(old_xml))
    new_names, _ = collect_names(parse_xml(new_xml))
    removed = [n for n in old_names if n not in new_names]
    return [r for r in removed if not old_names.get(r)]


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


if __name__ == "__main__":
    unittest.main()

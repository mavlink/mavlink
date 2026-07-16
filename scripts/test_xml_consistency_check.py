#! /usr/bin/env python3
"""
Unit tests for xml_consistency_check.py.

Exercises check_enum/check_field/check_cmd_param against small in-memory XML
fixtures, so regressions in the consistency checks are caught in CI instead
of relying on manual testing against real message definitions.
"""
import importlib.util
import os
import unittest

from bs4 import BeautifulSoup as bs

_MODULE_PATH = os.path.join(os.path.dirname(__file__), "xml_consistency_check.py")
_spec = importlib.util.spec_from_file_location("xml_consistency_check", _MODULE_PATH)
xml_consistency_check = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(xml_consistency_check)

check_enum = xml_consistency_check.check_enum
check_field = xml_consistency_check.check_field
check_cmd_param = xml_consistency_check.check_cmd_param


def parse_enum(xml):
    return bs(xml, 'xml').find('enum')


def parse_field(xml):
    return bs(xml, 'xml').find('field')


def parse_param(xml):
    return bs(xml, 'xml').find('param')


class ConsistencyCheckTestCase(unittest.TestCase):
    """Resets the module's global warning-tracking state before each test,
    since warn() accumulates into module-level globals."""

    def setUp(self):
        xml_consistency_check.warning_count = 0
        xml_consistency_check.allowed_count = 0
        xml_consistency_check.allowlist = set()
        xml_consistency_check.matched_allowlist = set()

    def warnings(self, fn, *args):
        messages = []
        original_warn = xml_consistency_check.warn

        def capture(message):
            messages.append(message)
            original_warn(message)

        xml_consistency_check.warn = capture
        try:
            fn(*args)
        finally:
            xml_consistency_check.warn = original_warn
        return messages


def make_enum_entry(name, bitmask):
    enum = parse_enum(
        '<enum name="%s"%s><entry name="%s_A" value="1"/>'
        '<entry name="%s_B" value="2"/></enum>'
        % (name, ' bitmask="true"' if bitmask else "", name, name)
    )
    decoded = check_enum(enum, "test.xml")
    return {
        "name": decoded["name"],
        "bitmask": decoded["bitmask"],
        "min": min(decoded["values"]),
        "max": max(decoded["values"]),
        "used": False,
    }


class DisplayBitmaskDeprecationTests(ConsistencyCheckTestCase):
    """display="bitmask" is only deprecated when the field's enum= is itself
    marked bitmask="true" - the enum already conveys the same information in
    that case. A field with no enum= has no other way to signal it's a raw
    bitmask, so it must not be flagged."""

    def test_raw_bitmask_field_without_enum_is_not_deprecated(self):
        field = parse_field('<field type="uint32_t" name="mask" display="bitmask">desc</field>')
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, {})
        self.assertEqual(messages, [])

    def test_display_bitmask_with_bitmask_enum_is_deprecated(self):
        enums = {"MY_FLAGS": make_enum_entry("MY_FLAGS", bitmask=True)}
        field = parse_field(
            '<field type="uint32_t" name="flags" enum="MY_FLAGS" display="bitmask">desc</field>'
        )
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, enums)
        self.assertEqual(
            messages,
            ['test.xml: Message MY_MSG field flags display="bitmask" is deprecated'],
        )

    def test_display_bitmask_with_non_bitmask_enum_is_not_deprecated(self):
        enums = {"MY_ENUM": make_enum_entry("MY_ENUM", bitmask=False)}
        field = parse_field(
            '<field type="uint32_t" name="flags" enum="MY_ENUM" display="bitmask">desc</field>'
        )
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, enums)
        self.assertEqual(messages, [])

    def test_field_without_display_is_never_flagged(self):
        enums = {"MY_FLAGS": make_enum_entry("MY_FLAGS", bitmask=True)}
        field = parse_field('<field type="uint32_t" name="flags" enum="MY_FLAGS">desc</field>')
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, enums)
        self.assertEqual(messages, [])


class DeprecatedElementSkippedTests(ConsistencyCheckTestCase):
    """Enums/messages/MAV_CMD entries wrapped in <deprecated/> are skipped
    entirely by the top-level scan loops (not exercised via check_enum/
    check_field/check_cmd_param directly, since the skip happens one level
    up in main()'s find_all() loops)."""

    def test_deprecated_message_has_no_deprecated_marker_in_soup(self):
        soup = bs(
            '<mavlink><messages><message name="OLD_MSG" id="1">'
            '<deprecated since="2020-01" replaced_by="NEW_MSG"/>'
            '<field type="uint8_t" name="foo" display="bitmask">desc</field>'
            '</message></messages></mavlink>',
            'xml',
        )
        message = soup.find('message')
        self.assertIsNotNone(message.find('deprecated', recursive=False))

    def test_non_deprecated_message_has_no_deprecated_marker(self):
        soup = bs(
            '<mavlink><messages><message name="MSG" id="1">'
            '<field type="uint8_t" name="foo" display="bitmask">desc</field>'
            '</message></messages></mavlink>',
            'xml',
        )
        message = soup.find('message')
        self.assertIsNone(message.find('deprecated', recursive=False))


class CmdParamRangeTests(ConsistencyCheckTestCase):
    def test_small_range_with_increment_one_suggests_enum(self):
        param = parse_param(
            '<param index="1" minValue="0" maxValue="5" increment="1">desc</param>'
        )
        messages = self.warnings(check_cmd_param, "test.xml", "MY_CMD", param, {})
        self.assertEqual(
            messages,
            ["test.xml: Command MY_CMD param 1 min, max close and increment of 1, "
             "should there be a enum?"],
        )

    def test_range_incompatible_with_increment_warns(self):
        param = parse_param(
            '<param index="1" minValue="0" maxValue="10" increment="3">desc</param>'
        )
        messages = self.warnings(check_cmd_param, "test.xml", "MY_CMD", param, {})
        self.assertEqual(
            messages,
            ["test.xml: Command MY_CMD param 1 range 0.000000 => 10.000000 "
             "incompatible with increment 3.000000"],
        )

    def test_invalid_range_warns(self):
        param = parse_param(
            '<param index="1" minValue="10" maxValue="5">desc</param>'
        )
        messages = self.warnings(check_cmd_param, "test.xml", "MY_CMD", param, {})
        self.assertEqual(
            messages,
            ["test.xml: Command MY_CMD param 1 min and max invalid got 10.000000 => 5.000000"],
        )

    def test_large_range_does_not_warn(self):
        param = parse_param(
            '<param index="1" minValue="0" maxValue="100" increment="1">desc</param>'
        )
        messages = self.warnings(check_cmd_param, "test.xml", "MY_CMD", param, {})
        self.assertEqual(messages, [])


if __name__ == "__main__":
    unittest.main()

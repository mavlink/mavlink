#! /usr/bin/env python3
"""
Unit tests for xml_consistency_check.py.

Exercises check_enum/check_field/check_cmd_param against small in-memory XML
fixtures, so regressions in the consistency checks are caught in CI instead
of relying on manual testing against real message definitions.
"""
import contextlib
import datetime
import importlib.util
import io
import os
import unittest
from unittest import mock

from bs4 import BeautifulSoup as bs

_MODULE_PATH = os.path.join(os.path.dirname(__file__), "xml_consistency_check.py")
_spec = importlib.util.spec_from_file_location("xml_consistency_check", _MODULE_PATH)
xml_consistency_check = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(xml_consistency_check)

check_enum = xml_consistency_check.check_enum
check_field = xml_consistency_check.check_field
check_cmd_param = xml_consistency_check.check_cmd_param
check_since_dates = xml_consistency_check.check_since_dates
check_wip_comments = xml_consistency_check.check_wip_comments
check_wip_required = xml_consistency_check.check_wip_required
latest_since_month = xml_consistency_check.latest_since_month


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
    bitmask_attr = ' bitmask="true"' if bitmask else ""
    enum = parse_enum(
        f'<enum name="{name}"{bitmask_attr}><entry name="{name}_A" value="1"/>'
        f'<entry name="{name}_B" value="2"/></enum>'
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


class DeprecatedEnumReferenceTestCase(ConsistencyCheckTestCase):
    """A deprecated enum is skipped when the checker collects enums, so a
    reference to one is not found in `enums`. It still exists in the XML
    though, so it must be reported as deprecated rather than as missing."""

    def test_field_referencing_deprecated_enum_says_deprecated(self):
        field = parse_field('<field type="uint8_t" name="mode" enum="OLD_ENUM">desc</field>')
        messages = self.warnings(
            check_field, "test.xml", "MY_MSG", field, {}, {"OLD_ENUM"}
        )
        self.assertEqual(
            messages,
            ["test.xml: Message MY_MSG field mode enum OLD_ENUM is deprecated"],
        )

    def test_field_referencing_unknown_enum_still_says_does_not_exist(self):
        field = parse_field('<field type="uint8_t" name="mode" enum="GONE_ENUM">desc</field>')
        messages = self.warnings(
            check_field, "test.xml", "MY_MSG", field, {}, {"OLD_ENUM"}
        )
        self.assertEqual(
            messages,
            ["test.xml: Message MY_MSG field mode enum GONE_ENUM does not exist"],
        )

    def test_cmd_param_referencing_deprecated_enum_says_deprecated(self):
        param = parse_param('<param index="1" enum="OLD_ENUM">desc</param>')
        messages = self.warnings(
            check_cmd_param, "test.xml", "MY_CMD", param, {}, {"OLD_ENUM"}
        )
        self.assertEqual(
            messages,
            ["test.xml: Command MY_CMD param 1 enum OLD_ENUM is deprecated"],
        )

    def test_cmd_param_referencing_unknown_enum_still_says_does_not_exist(self):
        param = parse_param('<param index="1" enum="GONE_ENUM">desc</param>')
        messages = self.warnings(
            check_cmd_param, "test.xml", "MY_CMD", param, {}, {"OLD_ENUM"}
        )
        self.assertEqual(
            messages,
            ["test.xml: Command MY_CMD param 1 enum GONE_ENUM does not exist"],
        )


class EnumDuplicateValuesTests(ConsistencyCheckTestCase):
    def test_duplicate_enum_values_warns(self):
        enum = parse_enum(
            '<enum name="MY_ENUM">'
            '<entry name="MY_ENUM_A" value="1"/>'
            '<entry name="MY_ENUM_B" value="1"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertEqual(messages, ["test.xml: Enum: MY_ENUM duplicate value 1"])

    def test_unique_enum_values_does_not_warn(self):
        enum = parse_enum(
            '<enum name="MY_ENUM">'
            '<entry name="MY_ENUM_A" value="1"/>'
            '<entry name="MY_ENUM_B" value="2"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertEqual(messages, [])


class EnumBitmaskConsistencyTests(ConsistencyCheckTestCase):
    def test_bitmask_containing_zero_warns(self):
        enum = parse_enum(
            '<enum name="MY_FLAGS" bitmask="true">'
            '<entry name="MY_FLAGS_NONE" value="0"/>'
            '<entry name="MY_FLAGS_A" value="1"/>'
            '<entry name="MY_FLAGS_B" value="2"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertIn("test.xml: Enum: MY_FLAGS bitmask should not contain 0", messages)

    def test_sequential_powers_of_two_without_bitmask_flag_suggests_bitmask(self):
        enum = parse_enum(
            '<enum name="MY_FLAGS">'
            '<entry name="MY_FLAGS_A" value="1"/>'
            '<entry name="MY_FLAGS_B" value="2"/>'
            '<entry name="MY_FLAGS_C" value="4"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertEqual(messages, ["test.xml: Enum: MY_FLAGS should be a marked as bitmask?"])

    def test_overlapping_values_with_bitmask_flag_warns(self):
        enum = parse_enum(
            '<enum name="MY_FLAGS" bitmask="true">'
            '<entry name="MY_FLAGS_A" value="1"/>'
            '<entry name="MY_FLAGS_B" value="2"/>'
            '<entry name="MY_FLAGS_C" value="3"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertEqual(messages, ["test.xml: Enum: MY_FLAGS should be not be a marked as bitmask"])

    def test_valid_bitmask_does_not_warn(self):
        enum = parse_enum(
            '<enum name="MY_FLAGS" bitmask="true">'
            '<entry name="MY_FLAGS_A" value="1"/>'
            '<entry name="MY_FLAGS_B" value="2"/>'
            '<entry name="MY_FLAGS_C" value="4"/>'
            '<entry name="MY_FLAGS_D" value="8"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertEqual(messages, [])

    def test_valid_sequential_enum_does_not_warn(self):
        enum = parse_enum(
            '<enum name="MY_ENUM">'
            '<entry name="MY_ENUM_A" value="1"/>'
            '<entry name="MY_ENUM_B" value="2"/>'
            '<entry name="MY_ENUM_C" value="3"/>'
            '</enum>'
        )
        messages = self.warnings(check_enum, enum, "test.xml")
        self.assertEqual(messages, [])


class FieldAndParamUnitsWithEnumTests(ConsistencyCheckTestCase):
    def test_field_with_both_units_and_enum_warns(self):
        field = parse_field('<field type="uint8_t" name="mode" enum="MY_ENUM" units="m">desc</field>')
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, {})
        self.assertIn("test.xml: Message MY_MSG field mode has both units and enum", messages)

    def test_cmd_param_with_both_units_and_enum_warns(self):
        param = parse_param('<param index="1" enum="MY_ENUM" units="m">desc</param>')
        messages = self.warnings(check_cmd_param, "test.xml", "MY_CMD", param, {})
        self.assertIn("test.xml: Command MY_CMD param 1 has both units and enum", messages)


class EnumFitsTypeTests(ConsistencyCheckTestCase):
    def test_enum_value_exceeding_type_range_warns(self):
        enums = {
            "BIG_ENUM": {
                "name": "BIG_ENUM",
                "bitmask": False,
                "min": 1,
                "max": 300,
                "used": False,
            }
        }
        field = parse_field('<field type="uint8_t" name="mode" enum="BIG_ENUM">desc</field>')
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, enums)
        self.assertEqual(
            messages,
            ["test.xml: Message MY_MSG field mode enum BIG_ENUM does not fit in type: uint8_t"],
        )

    def test_enum_value_fitting_type_range_does_not_warn(self):
        enums = {
            "FIT_ENUM": {
                "name": "FIT_ENUM",
                "bitmask": False,
                "min": 1,
                "max": 200,
                "used": False,
            }
        }
        field = parse_field('<field type="uint8_t" name="mode" enum="FIT_ENUM">desc</field>')
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, enums)
        self.assertEqual(messages, [])

    def test_field_enum_unexpected_type_warns(self):
        enums = {
            "MY_ENUM": {
                "name": "MY_ENUM",
                "bitmask": False,
                "min": 1,
                "max": 5,
                "used": False,
            }
        }
        field = parse_field('<field type="custom_t" name="mode" enum="MY_ENUM">desc</field>')
        messages = self.warnings(check_field, "test.xml", "MY_MSG", field, enums)
        self.assertEqual(
            messages,
            ["test.xml: Message MY_MSG field mode enum MY_ENUM unexpected type: custom_t"],
        )


def parse_dialect(body):
    return bs(f"<mavlink>{body}</mavlink>", 'xml')


class CheckSinceDatesTest(ConsistencyCheckTestCase):
    """A <wip> in any dialect needs a valid since date that is not in the future."""

    LATEST = "2026-10"

    def message(self, wip):
        return parse_dialect(
            f'<messages><message id="1" name="MY_MSG">{wip}<description>d</description>'
            '<field type="uint8_t" name="a">a</field></message></messages>'
        )

    def test_dated_wip_no_warning(self):
        soup = self.message('<wip since="2026-03"/>')
        self.assertEqual(self.warnings(check_since_dates, "common.xml", soup, self.LATEST), [])

    def test_no_wip_no_warning(self):
        soup = self.message('')
        self.assertEqual(self.warnings(check_since_dates, "common.xml", soup, self.LATEST), [])

    def test_current_month_no_warning(self):
        soup = self.message('<wip since="2026-10"/>')
        self.assertEqual(self.warnings(check_since_dates, "common.xml", soup, self.LATEST), [])

    def test_bare_wip_warns(self):
        soup = self.message('<wip/>')
        self.assertEqual(
            self.warnings(check_since_dates, "common.xml", soup, self.LATEST),
            ["common.xml: Message MY_MSG <wip> has no since date"],
        )

    def test_bare_wip_allowed_when_date_not_required(self):
        soup = self.message('<wip/>')
        self.assertEqual(
            self.warnings(check_since_dates, "storm32.xml", soup, self.LATEST, False), []
        )

    def test_bad_date_warns_even_when_date_not_required(self):
        soup = self.message('<wip since="2027-01"/>')
        self.assertEqual(
            self.warnings(check_since_dates, "storm32.xml", soup, self.LATEST, False),
            ['storm32.xml: Message MY_MSG <wip> since "2027-01" is in the future'],
        )

    def test_malformed_dates_warn(self):
        for since in ("2026-13", "2026-00", "26-03", "2026-3", "2026-03-01", "2026/03", "1999-03", ""):
            with self.subTest(since=since):
                soup = self.message(f'<wip since="{since}"/>')
                self.assertEqual(
                    self.warnings(check_since_dates, "common.xml", soup, self.LATEST),
                    [f'common.xml: Message MY_MSG <wip> since "{since}" is not a valid YYYY-MM date'],
                )

    def test_future_dates_warn(self):
        for since in ("2026-11", "2027-01"):
            with self.subTest(since=since):
                soup = self.message(f'<wip since="{since}"/>')
                self.assertEqual(
                    self.warnings(check_since_dates, "common.xml", soup, self.LATEST),
                    [f'common.xml: Message MY_MSG <wip> since "{since}" is in the future'],
                )

    def test_deprecated_and_superseded_dates_are_checked(self):
        for tag in ("deprecated", "superseded"):
            for since, problem in (("2026-13", "is not a valid YYYY-MM date"), ("2027-01", "is in the future")):
                with self.subTest(tag=tag, since=since):
                    soup = self.message(f'<{tag} since="{since}" replaced_by="X"/>')
                    self.assertEqual(
                        self.warnings(check_since_dates, "common.xml", soup, self.LATEST),
                        [f'common.xml: Message MY_MSG <{tag}> since "{since}" {problem}'],
                    )

    def test_deprecated_and_superseded_valid_or_missing_date_no_warning(self):
        # A missing since on these two is left to the schema, which requires it.
        for marker in ('<deprecated since="2026-03" replaced_by="X"/>', '<superseded since="2026-10" replaced_by="X"/>',
                       '<deprecated replaced_by="X"/>'):
            with self.subTest(marker=marker):
                soup = self.message(marker)
                self.assertEqual(self.warnings(check_since_dates, "common.xml", soup, self.LATEST), [])

    def test_enum_entry_and_command_labels(self):
        soup = parse_dialect(
            '<enums><enum name="MY_ENUM"><wip/><entry value="1" name="MY_ENUM_A"><wip/></entry></enum>'
            '<enum name="MAV_CMD"><entry value="2" name="MAV_CMD_MY"><wip/></entry></enum></enums>'
        )
        self.assertEqual(
            self.warnings(check_since_dates, "standard.xml", soup, self.LATEST),
            [
                "standard.xml: Enum: MY_ENUM <wip> has no since date",
                "standard.xml: Enum: MY_ENUM entry MY_ENUM_A <wip> has no since date",
                "standard.xml: Command MAV_CMD_MY <wip> has no since date",
            ],
        )

    def test_latest_month_uses_furthest_ahead_timezone(self):
        utc = datetime.timezone.utc
        # 09:59 UTC on the last day of the month is still that month everywhere
        self.assertEqual(latest_since_month(datetime.datetime(2026, 10, 31, 9, 59, tzinfo=utc)), "2026-10")
        # 10:00 UTC is already the 1st of next month at UTC+14
        self.assertEqual(latest_since_month(datetime.datetime(2026, 10, 31, 10, 0, tzinfo=utc)), "2026-11")
        self.assertEqual(latest_since_month(datetime.datetime(2026, 12, 31, 12, 0, tzinfo=utc)), "2027-01")


class CheckWipCommentsTest(ConsistencyCheckTestCase):
    """A <wip> in any dialect must be followed by the standard comment."""

    COMMENT = "This {} is work-in-progress and it can therefore change. It should NOT be used in stable production environments."

    def message(self, after_wip):
        return parse_dialect(
            f'<messages><message id="1" name="MY_MSG">\n<wip since="2026-03"/>\n{after_wip}\n'
            '<description>d</description></message></messages>'
        )

    def expected(self, label, kind):
        return [f"common.xml: {label} <wip> is not followed by the comment: <!-- {self.COMMENT.format(kind)} -->"]

    def test_comment_present_no_warning(self):
        soup = self.message(f'<!-- {self.COMMENT.format("message")} -->')
        self.assertEqual(self.warnings(check_wip_comments, "common.xml", soup), [])

    def test_no_wip_no_warning(self):
        soup = parse_dialect('<messages><message id="1" name="MY_MSG"><description>d</description></message></messages>')
        self.assertEqual(self.warnings(check_wip_comments, "common.xml", soup), [])

    def test_missing_comment_warns(self):
        soup = self.message('')
        self.assertEqual(self.warnings(check_wip_comments, "common.xml", soup), self.expected("Message MY_MSG", "message"))

    def test_other_comment_warns(self):
        for comment in (
            "This message is work-in-progress it can therefore change, and should NOT be used in stable production environments",
            self.COMMENT.format("enum"),
            "Some other comment",
        ):
            with self.subTest(comment=comment):
                soup = self.message(f'<!-- {comment} -->')
                self.assertEqual(
                    self.warnings(check_wip_comments, "common.xml", soup), self.expected("Message MY_MSG", "message")
                )

    def test_comment_must_directly_follow_wip(self):
        soup = parse_dialect(
            '<messages><message id="1" name="MY_MSG"><wip since="2026-03"/><description>d</description>'
            f'<!-- {self.COMMENT.format("message")} --></message></messages>'
        )
        self.assertEqual(self.warnings(check_wip_comments, "common.xml", soup), self.expected("Message MY_MSG", "message"))

    def test_comment_names_what_is_marked(self):
        def marked(kind):
            return f'<wip since="2026-03"/><!-- {self.COMMENT.format(kind)} -->'

        good = parse_dialect(
            f'<enums><enum name="MY_ENUM">{marked("enum")}<entry value="1" name="MY_ENUM_A">{marked("entry")}</entry></enum>'
            f'<enum name="MAV_CMD"><entry value="2" name="MAV_CMD_MY">{marked("command")}</entry></enum></enums>'
        )
        self.assertEqual(self.warnings(check_wip_comments, "common.xml", good), [])

        bad = parse_dialect(
            f'<enums><enum name="MY_ENUM">{marked("message")}<entry value="1" name="MY_ENUM_A">{marked("enum")}</entry></enum>'
            f'<enum name="MAV_CMD"><entry value="2" name="MAV_CMD_MY">{marked("entry")}</entry></enum></enums>'
        )
        self.assertEqual(
            self.warnings(check_wip_comments, "common.xml", bad),
            self.expected("Enum: MY_ENUM", "enum")
            + self.expected("Enum: MY_ENUM entry MY_ENUM_A", "entry")
            + self.expected("Command MAV_CMD_MY", "command"),
        )


class CheckWipRequiredTest(ConsistencyCheckTestCase):
    """Everything in a work in progress dialect must be marked <wip>."""

    def test_unmarked_message_warns(self):
        soup = parse_dialect('<messages><message id="1" name="MY_MSG"><description>d</description></message></messages>')
        self.assertEqual(
            self.warnings(check_wip_required, "development.xml", soup, set()),
            ['development.xml: Message MY_MSG is missing <wip since="YYYY-MM"/>'],
        )

    def test_marked_message_no_warning(self):
        # The date itself is the job of check_since_dates, so a bare <wip/> is not reported twice.
        for wip in ('<wip since="2026-03"/>', '<wip/>'):
            with self.subTest(wip=wip):
                soup = parse_dialect(f'<messages><message id="1" name="MY_MSG">{wip}</message></messages>')
                self.assertEqual(self.warnings(check_wip_required, "development.xml", soup, set()), [])

    def test_deprecated_or_superseded_no_warning(self):
        for tag in ("deprecated", "superseded"):
            with self.subTest(tag=tag):
                soup = parse_dialect(
                    f'<messages><message id="1" name="MY_MSG"><{tag} since="2026-03" replaced_by="X"/></message></messages>'
                )
                self.assertEqual(self.warnings(check_wip_required, "development.xml", soup, set()), [])

    def test_new_enum_needs_wip_but_its_entries_do_not(self):
        soup = parse_dialect(
            '<enums><enum name="MARKED"><wip since="2026-03"/><entry value="1" name="MARKED_A"/></enum>'
            '<enum name="UNMARKED"><entry value="1" name="UNMARKED_A"/></enum></enums>'
        )
        self.assertEqual(
            self.warnings(check_wip_required, "development.xml", soup, set()),
            ['development.xml: Enum: UNMARKED is missing <wip since="YYYY-MM"/>'],
        )

    def test_extended_enum_entries_need_wip_but_the_enum_does_not(self):
        soup = parse_dialect(
            '<enums><enum name="MAV_CMD"><entry value="1" name="MAV_CMD_MARKED"><wip since="2026-03"/></entry>'
            '<entry value="2" name="MAV_CMD_UNMARKED"/></enum>'
            '<enum name="MAV_RESULT"><entry value="9" name="MAV_RESULT_UNMARKED"/></enum></enums>'
        )
        self.assertEqual(
            self.warnings(check_wip_required, "development.xml", soup, {"MAV_CMD", "MAV_RESULT"}),
            [
                'development.xml: Command MAV_CMD_UNMARKED is missing <wip since="YYYY-MM"/>',
                'development.xml: Enum: MAV_RESULT entry MAV_RESULT_UNMARKED is missing <wip since="YYYY-MM"/>',
            ],
        )


class WipDialectsTest(ConsistencyCheckTestCase):
    def test_only_development_requires_wip(self):
        self.assertEqual(xml_consistency_check.WIP_REQUIRED_DIALECTS, ["development.xml"])

    def test_included_enum_names_follows_include_chain(self):
        source_dir = os.path.join(os.path.dirname(__file__), "../message_definitions/v1.0/")
        names = xml_consistency_check.included_enum_names(source_dir, "development.xml")
        self.assertIn("MAV_CMD", names)  # common.xml
        self.assertIn("MAV_PROTOCOL_CAPABILITY", names)  # standard.xml, via common.xml
        self.assertIn("MAV_TYPE", names)  # minimal.xml, via standard.xml
        self.assertEqual(xml_consistency_check.included_enum_names(source_dir, "minimal.xml"), set())


class MainRunsLifecycleChecksTest(ConsistencyCheckTestCase):
    """The checks above are only useful if main() calls them. The managed
    dialects are clean, so a check that stopped running would not be noticed."""

    def test_main_runs_lifecycle_checks(self):
        with mock.patch.object(xml_consistency_check, "check_since_dates") as dates, \
                mock.patch.object(xml_consistency_check, "check_wip_comments") as comments, \
                mock.patch.object(xml_consistency_check, "check_wip_required") as required, \
                mock.patch.object(xml_consistency_check, "latest_since_month", return_value="2026-10"), \
                mock.patch("sys.argv", ["xml_consistency_check.py"]), \
                contextlib.redirect_stdout(io.StringIO()):
            xml_consistency_check.main()

        managed = ["common.xml", "development.xml", "minimal.xml", "standard.xml"]
        self.assertEqual(sorted(call.args[0] for call in dates.call_args_list), managed)
        # The month dates are compared with must be the one main() worked out
        self.assertEqual({call.args[2] for call in dates.call_args_list}, {"2026-10"})
        self.assertEqual({call.kwargs["wip_since_required"] for call in dates.call_args_list}, {True})
        self.assertEqual(sorted(call.args[0] for call in comments.call_args_list), managed)
        # Only development.xml requires everything to be marked wip
        self.assertEqual([call.args[0] for call in required.call_args_list], ["development.xml"])
        self.assertIn("MAV_CMD", required.call_args.args[2])

    def test_other_dialects_only_get_date_validity(self):
        # The schema allows a bare <wip/>, so a dialect this repository does not
        # manage is not required to date its <wip> or add the comment.
        with mock.patch.object(xml_consistency_check, "check_since_dates") as dates, \
                mock.patch.object(xml_consistency_check, "check_wip_comments") as comments, \
                mock.patch.object(xml_consistency_check, "check_wip_required") as required, \
                mock.patch("sys.argv", ["xml_consistency_check.py", "-f", "storm32.xml"]), \
                contextlib.redirect_stdout(io.StringIO()):
            xml_consistency_check.main()

        self.assertEqual([call.args[0] for call in dates.call_args_list], ["storm32.xml"])
        self.assertIs(dates.call_args.kwargs["wip_since_required"], False)
        comments.assert_not_called()
        required.assert_not_called()

    def test_wip_required_for_development_however_it_is_named(self):
        for name in ("development", "development.xml", "./development.xml"):
            with self.subTest(name=name):
                with mock.patch.object(xml_consistency_check, "check_wip_required") as required, \
                        mock.patch("sys.argv", ["xml_consistency_check.py", "-f", name]), \
                        contextlib.redirect_stdout(io.StringIO()):
                    xml_consistency_check.main()
                self.assertEqual(required.call_count, 1)


if __name__ == "__main__":
    unittest.main()

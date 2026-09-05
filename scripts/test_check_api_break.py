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


# A message shaped like the real ones that carry extensions (e.g. RAW_IMU in
# common.xml): two ordinary fields, then <extensions/>, then two extension
# fields of different types, so their order is visible in the payload layout.
EXT_XML = """<mavlink>
<enums>
</enums>
<messages>
<message id="27" name="EXT_MSG">
  <field type="uint64_t" name="time_usec">Timestamp.</field>
  <field type="int16_t" name="xacc">X acceleration.</field>
  <extensions/>
  <field type="uint8_t" name="id">Id.</field>
  <field type="int16_t" name="temperature">Temperature.</field>
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


class ExtensionFieldOrderTests(unittest.TestCase):
    """Extension fields are not reordered when a message is serialized, so their
    XML order is part of the wire format. Fields before <extensions/> are sorted
    by size by the generator, so their XML order is not."""

    def test_extension_field_reorder_detected(self):
        new_xml = EXT_XML.replace(
            '  <field type="uint8_t" name="id">Id.</field>\n'
            '  <field type="int16_t" name="temperature">Temperature.</field>',
            '  <field type="int16_t" name="temperature">Temperature.</field>\n'
            '  <field type="uint8_t" name="id">Id.</field>',
        )
        self.assertEqual(
            sorted(mutations_between(EXT_XML, new_xml)),
            [
                "field EXT_MSG.id (extension_index: 0 -> 1)",
                "field EXT_MSG.temperature (extension_index: 1 -> 0)",
            ],
        )

    def test_appending_an_extension_field_is_not_a_mutation(self):
        # Appending to the end is the supported way to extend a message and
        # must stay silent, otherwise the check blocks the one change it is
        # meant to allow.
        new_xml = EXT_XML.replace(
            '  <field type="int16_t" name="temperature">Temperature.</field>',
            '  <field type="int16_t" name="temperature">Temperature.</field>\n'
            '  <field type="uint32_t" name="added_later">Added later.</field>',
        )
        self.assertEqual(mutations_between(EXT_XML, new_xml), [])

    def test_non_extension_field_reorder_is_not_a_mutation(self):
        new_xml = EXT_XML.replace(
            '  <field type="uint64_t" name="time_usec">Timestamp.</field>\n'
            '  <field type="int16_t" name="xacc">X acceleration.</field>',
            '  <field type="int16_t" name="xacc">X acceleration.</field>\n'
            '  <field type="uint64_t" name="time_usec">Timestamp.</field>',
        )
        self.assertEqual(mutations_between(EXT_XML, new_xml), [])

    def test_extension_index_recorded_only_after_the_marker(self):
        _names, attrs = collect_names(parse_xml(EXT_XML))
        msg = MessageKey(message_name="EXT_MSG")
        before = attrs[FieldKey(message=msg, field_name="time_usec")]
        after = attrs[FieldKey(message=msg, field_name="id")]
        self.assertNotIn("extension_index", before)
        self.assertEqual(after["extension_index"], 0)

    def test_message_without_extensions_is_unaffected(self):
        _names, attrs = collect_names(parse_xml(BASE_XML))
        field = attrs[FieldKey(message=MessageKey(message_name="MY_MSG"), field_name="foo")]
        self.assertEqual(field, {"type": "uint8_t"})

    def test_extension_field_type_change_still_detected(self):
        new_xml = EXT_XML.replace('type="uint8_t" name="id"', 'type="uint16_t" name="id"')
        self.assertEqual(
            mutations_between(EXT_XML, new_xml),
            ["field EXT_MSG.id (type: uint8_t -> uint16_t)"],
        )


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


class BaseCommitTests(unittest.TestCase):
    """Covers get_base_commit()'s three tiers: --base override, the CI event
    payload (authoritative, must error rather than guess on failure), and the
    local best-effort ref cascade.

    Every test that can reach the cascade neutralizes GITHUB_EVENT_PATH and
    mocks subprocess.run, because these tests run inside this repo's own
    GitHub Actions job: without that, a real GITHUB_EVENT_PATH would make the
    CI-event tier intercept the call, and a real `git fetch` would hit the
    network for real remotes (origin/upstream) that exist in this repo.
    """

    # Neutralizes ambient CI env vars so cascade-focused tests behave the same
    # locally and when this suite runs inside the project's own CI job.
    _NO_CI_ENV = {"GITHUB_EVENT_PATH": "", "GITHUB_EVENT_NAME": "", "GITHUB_BASE_REF": ""}

    def test_override_is_attempted_first(self):
        from unittest.mock import patch
        import subprocess

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "custom-branch"]:
                return "abc1234\n"
            raise subprocess.CalledProcessError(1, cmd)

        with patch("subprocess.check_output", side_effect=fake_check_output):
            res = check_api_break.get_base_commit("custom-branch")
            self.assertEqual(res, "abc1234")

    def test_override_that_does_not_resolve_raises(self):
        from unittest.mock import patch
        import subprocess

        with patch("subprocess.check_output", side_effect=subprocess.CalledProcessError(1, [])):
            with self.assertRaises(RuntimeError):
                check_api_break.get_base_commit("no-such-ref")

    def test_upstream_master_fallback_when_origin_master_missing(self):
        from unittest.mock import patch
        import subprocess

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "upstream/master"]:
                return "fedcba9\n"
            raise subprocess.CalledProcessError(128, cmd)

        with patch.dict(os.environ, self._NO_CI_ENV), \
             patch("subprocess.check_output", side_effect=fake_check_output), \
             patch("subprocess.run"):
            res = check_api_break.get_base_commit()
            self.assertEqual(res, "fedcba9")

    def test_env_var_base_ref_respected(self):
        from unittest.mock import patch
        import subprocess

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "pr-target"]:
                return "1122334\n"
            raise subprocess.CalledProcessError(1, cmd)

        env = dict(self._NO_CI_ENV, MAVLINK_BASE_REF="pr-target")
        with patch.dict(os.environ, env), \
             patch("subprocess.check_output", side_effect=fake_check_output), \
             patch("subprocess.run"):
            res = check_api_break.get_base_commit()
            self.assertEqual(res, "1122334")

    def test_env_var_qualified_forms_preferred_over_bare_name(self):
        # A locally-checked-out branch of the same name as env_base shouldn't
        # win over the remote-qualified forms - those are more likely fresh.
        from unittest.mock import patch
        import subprocess

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "upstream/pr-target"]:
                return "9988776\n"
            if cmd[:3] == ["git", "merge-base", "pr-target"]:
                return "0000000\n"  # a stale local branch - should lose
            raise subprocess.CalledProcessError(1, cmd)

        env = dict(self._NO_CI_ENV, MAVLINK_BASE_REF="pr-target")
        with patch.dict(os.environ, env), \
             patch("subprocess.check_output", side_effect=fake_check_output), \
             patch("subprocess.run"):
            res = check_api_break.get_base_commit()
            self.assertEqual(res, "9988776")

    def test_ci_pull_request_base_sha_takes_priority(self):
        from unittest.mock import patch
        import json
        import subprocess
        import tempfile

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "cafebabe"]:
                return "cafebabe\n"
            raise subprocess.CalledProcessError(1, cmd)

        with tempfile.TemporaryDirectory() as tmp:
            event_path = os.path.join(tmp, "event.json")
            with open(event_path, "w", encoding="utf-8") as f:
                json.dump({"pull_request": {"base": {"sha": "cafebabe"}}}, f)

            env = {
                "GITHUB_EVENT_PATH": event_path,
                "GITHUB_EVENT_NAME": "pull_request",
                # Should be ignored: the CI event payload outranks it.
                "MAVLINK_BASE_REF": "should-be-ignored",
            }
            with patch.dict(os.environ, env), \
                 patch("subprocess.check_output", side_effect=fake_check_output):
                res = check_api_break.get_base_commit()
                self.assertEqual(res, "cafebabe")

    def test_ci_push_before_sha_used(self):
        from unittest.mock import patch
        import json
        import subprocess
        import tempfile

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "1122334455"]:
                return "1122334455\n"
            raise subprocess.CalledProcessError(1, cmd)

        with tempfile.TemporaryDirectory() as tmp:
            event_path = os.path.join(tmp, "event.json")
            with open(event_path, "w", encoding="utf-8") as f:
                json.dump({"before": "1122334455"}, f)

            env = {"GITHUB_EVENT_PATH": event_path, "GITHUB_EVENT_NAME": "push"}
            with patch.dict(os.environ, env), \
                 patch("subprocess.check_output", side_effect=fake_check_output):
                res = check_api_break.get_base_commit()
                self.assertEqual(res, "1122334455")

    def test_ci_pull_request_missing_base_sha_raises(self):
        from unittest.mock import patch
        import json
        import tempfile

        with tempfile.TemporaryDirectory() as tmp:
            event_path = os.path.join(tmp, "event.json")
            with open(event_path, "w", encoding="utf-8") as f:
                json.dump({"pull_request": {"base": {}}}, f)

            env = {"GITHUB_EVENT_PATH": event_path, "GITHUB_EVENT_NAME": "pull_request"}
            with patch.dict(os.environ, env):
                with self.assertRaises(RuntimeError):
                    check_api_break.get_base_commit()

    def test_ci_push_zero_before_raises(self):
        from unittest.mock import patch
        import json
        import tempfile

        with tempfile.TemporaryDirectory() as tmp:
            event_path = os.path.join(tmp, "event.json")
            with open(event_path, "w", encoding="utf-8") as f:
                json.dump({"before": "0" * 40}, f)

            env = {"GITHUB_EVENT_PATH": event_path, "GITHUB_EVENT_NAME": "push"}
            with patch.dict(os.environ, env):
                with self.assertRaises(RuntimeError):
                    check_api_break.get_base_commit()

    def test_ci_event_sha_unreachable_raises_instead_of_guessing(self):
        # Simulates a too-shallow checkout: the event's base sha exists but
        # isn't in local history. This must error, not silently fall through
        # to the local cascade - that's the whole point of the CI tier.
        from unittest.mock import patch
        import json
        import subprocess
        import tempfile

        with tempfile.TemporaryDirectory() as tmp:
            event_path = os.path.join(tmp, "event.json")
            with open(event_path, "w", encoding="utf-8") as f:
                json.dump({"pull_request": {"base": {"sha": "deadbeef"}}}, f)

            env = {"GITHUB_EVENT_PATH": event_path, "GITHUB_EVENT_NAME": "pull_request"}
            with patch.dict(os.environ, env), \
                 patch("subprocess.check_output", side_effect=subprocess.CalledProcessError(128, [])):
                with self.assertRaises(RuntimeError):
                    check_api_break.get_base_commit()

    def test_workflow_dispatch_has_no_event_base_falls_through_to_cascade(self):
        from unittest.mock import patch
        import json
        import subprocess
        import tempfile

        def fake_check_output(cmd, **kwargs):
            if cmd[:3] == ["git", "merge-base", "upstream/master"]:
                return "abcdef1\n"
            raise subprocess.CalledProcessError(1, cmd)

        with tempfile.TemporaryDirectory() as tmp:
            event_path = os.path.join(tmp, "event.json")
            with open(event_path, "w", encoding="utf-8") as f:
                json.dump({"inputs": {}}, f)

            env = {"GITHUB_EVENT_PATH": event_path, "GITHUB_EVENT_NAME": "workflow_dispatch"}
            with patch.dict(os.environ, env), \
                 patch("subprocess.check_output", side_effect=fake_check_output), \
                 patch("subprocess.run"):
                res = check_api_break.get_base_commit()
                self.assertEqual(res, "abcdef1")

    def test_main_passes_base_arg_to_get_base_commit(self):
        from unittest.mock import patch

        with patch("sys.argv", ["check_api_break.py", "--base", "custom-ref"]), \
             patch.object(check_api_break, "get_base_commit", return_value="deadbeef") as mock_get_base, \
             patch.object(check_api_break, "get_changed_xml_files", return_value=[]):
            check_api_break.main()
            mock_get_base.assert_called_once_with("custom-ref")


class MainRemovedFileTests(unittest.TestCase):
    def test_file_removed_since_base_is_reported_not_crashed(self):
        # A whole XML file present at `base` but deleted by HEAD used to crash
        # main() with an unhandled FileNotFoundError (open() on the no-longer
        # -existing path wasn't covered by the CalledProcessError handler
        # around the `git show base:file` read). It should be skipped and
        # reported instead, consistent with how a brand-new file is skipped.
        from unittest.mock import patch
        import io
        import contextlib
        import subprocess

        def fake_check_output(cmd, **kwargs):
            if cmd[:2] == ["git", "show"]:
                return "<mavlink><enums></enums></mavlink>"
            raise subprocess.CalledProcessError(1, cmd)

        stdout = io.StringIO()
        with patch("sys.argv", ["check_api_break.py"]), \
             patch.object(check_api_break, "get_base_commit", return_value="deadbeef"), \
             patch.object(check_api_break, "get_changed_xml_files", return_value=["removed_dialect.xml"]), \
             patch("subprocess.check_output", side_effect=fake_check_output), \
             contextlib.redirect_stdout(stdout):
            check_api_break.main()  # must not raise

        self.assertIn("Skipped removed_dialect.xml: removed since base.", stdout.getvalue())


if __name__ == "__main__":
    unittest.main()



# Lens: fix proof

Does the change actually catch or fix the problem it claims to, and do its tests prove that?

## 1. Reproduce the problem

Build the **smallest fixture** that exhibits the stated problem:

- For `check_api_break.py`: a base XML and a head XML (see the fixture style in `scripts/test_check_api_break.py` — `BASE_XML`, `EXT_XML`), committed into a throwaway git repo as `shared.md` describes.
- For `xml_consistency_check.py`: one XML file, run with `-f <file>`.
- For `mavlink_xml_to_markdown.py`: an XML file (with its includes) and the expected markdown fragment.
- For a pymavlink generator change: an XML file plus generated output for the affected language(s); for a runtime change, a short script that packs/unpacks the affected message.
- For build/CMake: the configure/build command that failed.

Prefer a fixture taken from the PR's linked issue or from real dialect content that triggered the bug; otherwise construct one and say so.

## 2. Run it on base and head

| Expected on base | Expected on head |
| --- | --- |
| the problem: missed detection, wrong warning, crash, wrong output | the fix: detected, no warning, no crash, correct output |

- **Base already correct** → the PR doesn't fix what it says (or the problem was misdescribed): `bug`, unless the PR is explicitly a refactor/test-only change.
- **Head still wrong** → `bug`.
- **Head fixes it but the message/output is misleading** (wrong field named, wrong severity level, a warning that doesn't tell the author what to do) → `should-fix`.

Also run the fixture through anything else that reacts to the same XML — a new consistency-check rule should not also make `check_api_break.py` or mavgen choke on it.

## 3. Do the PR's tests prove the fix?

Tests that pass on the unfixed code prove nothing about the fix.

- Copy the PR's new/changed test file(s) into the **base** worktree and run them against the base implementation (`python -m unittest scripts/test_<x>.py -v`). At least one new test must **fail** there. If all pass: `should-fix` — name the scenario the tests miss and offer the fixture from step 1 as a test.
- Run them on head: all must pass.
- **Revert just the fix** on a scratch copy of head (the smallest hunk that makes the behavioural change — not the tests) and rerun: a test must fail. If none does, the tests are exercising something other than the fix.
- If the PR adds no tests for a behavioural change to a script that has a test file (`scripts/test_check_api_break.py`, `scripts/test_xml_consistency_check.py`), that's `should-fix`; include the fixture as a ready-to-paste test below the tables.

## 4. Claims in the description

Each distinct claim in the PR description ("also handles X", "no change to output for existing files") is checked the same way. "No change to output" is checked by `regression.md` — take its result rather than re-running.

## Return

In addition to findings, return the rows for the report's **Proof of fix** table (check, base result, head result, expected), and the fixtures in full.

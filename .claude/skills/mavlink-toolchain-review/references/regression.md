# Lens: regression

Did anything change that the PR didn't intend? The fixture proves the target case; this lens runs the whole real corpus.

## Check scripts

- `xml_consistency_check.py`: run on base and head over every dialect (no `-f`), capture stdout, diff. Every new warning must be either a real, intended detection (and then either fixed in the PR, or added to `xml_consistency_allowlist.txt` in the same PR — otherwise CI's `--exception` run fails) or a false positive (`bug`). Every vanished warning must be intended.
- Also run with `--exception` on head exactly as CI does; it must pass.
- `check_api_break.py`: replay it over recent real history on base and head and diff the results — e.g. for each of the last ~30 commits touching `message_definitions/`, `--base <commit>^` with the worktree at `<commit>`. Known-breaking commits (renames, removals like d0adc9c5, 4dae2c3a) must still be reported; description-only commits (7687ac00, dc252db7) must still be clean. List any commit whose result changed.
- `format_xml.sh -c` on every dialect, base vs head: same verdicts, unless the PR says otherwise.
- Allowlist edits: every removed line must correspond to a warning that no longer occurs; every added line must match a warning verbatim.

## Doc generator

- Run `doc/mavlink_xml_to_markdown.py` on base and head (as `docs_build_and_deploy.yml` does), diff the output directory. Expected changes only; call out anything else by dialect and definition.
- Spot-check the rendered markdown of the intended change (does the table/badge/link render as the PR's screenshot or description says?).

## pymavlink bumps and generator changes

- Generate on base and head for every language CI covers (`scripts/test.sh py`: Python, C, CS, WLua, Java for 1.0; plus C++11 for 2.0) on at least `common.xml`, `development.xml`, `all.xml`, and `ardupilotmega.xml`, into scratch dirs, and diff.
- Any change in **wire-relevant output** — `CRC_EXTRA`, message lengths, field offsets, field order, message IDs, the `MAVLINK_MESSAGE_CRCS` table — is a `bug` unless the bump's commits explain it as intended and it's accompanied by a protocol decision.
- Changes in **generated API** (function names, signatures, struct names/members, Python class/attribute names, enum constant names) are downstream breaks: hand to `downstream.md` with the diff.
- Run pymavlink's own test entry points that CI runs (`test_and_deploy.yml` "Test Python generator" and the Node tests).
- Run `mavgen.py --strict-units` over every dialect with the new XSD: nothing that passed before may fail now, unless the bump's point is to newly reject it (then the XML must be fixed in the same or a preceding PR).

## Build and examples

- Configure and build the C and C++ examples the way `test_and_deploy.yml` does, on base and head.
- `cmake --install` into a scratch prefix and diff the installed file list and `MAVLinkConfig.cmake`.

## Return

Findings, plus a trimmed diff of unexpected output changes for the report.

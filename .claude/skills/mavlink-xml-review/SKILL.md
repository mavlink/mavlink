---
name: mavlink-xml-review
description: Reviews changes to the MAVLink XML message definitions (message_definitions/v1.0/*.xml) — new or changed messages, fields, enums and MAV_CMD commands — for wire compatibility, correct dialect/ID placement and lifecycle markers, definition quality (reserved params, string encoding, units, smallest adequate types, zero-default extensions, truncation-friendly ordering, wording consistent with similar fields), message design (prior art such as DroneCAN, splitting invariant from high-rate data), and downstream impact in PX4, ArduPilot, QGroundControl, MAVProxy and MissionPlanner. Use when reviewing a mavlink/mavlink PR (by number or URL) or local branch that touches message_definitions/, and consult it proactively when drafting an XML change yourself. PRs that change scripts/, doc/ tooling, workflows or the pymavlink submodule belong to mavlink-toolchain-review; a mixed PR is split between the two.
allowed-tools: Bash(gh pr view:*) Bash(gh pr diff:*) Bash(gh pr list:*) Bash(gh api:*) Bash(git -C:*) Bash(git worktree:*) Bash(grep:*) Bash(python3 scripts/*) Bash(./scripts/format_xml.sh:*)
---

# MAVLink XML definition review

You are reviewing changes to the MAVLink schema itself.
Every message, field, enum and command defined here is generated into code for every flight stack and ground station, and once released it is effectively frozen: a mistake in a type, an ordering, or a default value can't be fixed later without breaking the wire.
So this review is deliberately stricter on *new* definitions than on edits to old ones — new definitions are the only chance to get the design right.

Two modes:

- **Review mode.** A PR (or local branch/diff) exists. Run Scope → Checks → Dispatch → Report below and produce a report the reviewer posts themselves.
- **Authoring mode.** You are drafting an XML change yourself. Skip Scope/Dispatch/Report; before finishing, read `references/shared.md`, `references/definition-quality.md` and `references/message-design.md`, run the Checks step against your working tree, and hold your draft to the same rules — out loud in your response when something fails (e.g. "I've used `uint8_t` for `fuel_pct` but it feeds a remaining-fuel calculation, so 1% resolution may not be enough; flagging rather than guessing").

**Read-only in review mode**, here and in every subagent it spawns: see `references/shared.md`.

## Files in this skill

- `references/shared.md`: read-only rule, scope, sources of truth, verification, severity, and the "new vs existing definition" rule every lens depends on
- `references/compatibility.md`: lens — wire and semantic compatibility, including breaks `check_api_break.py` cannot see
- `references/placement-lifecycle.md`: lens — dialect choice, ID ranges, `<wip/>`/dates, deprecation, promotion from development.xml, vendor dialects
- `references/definition-quality.md`: lens — per-field and per-param rules: reserved params, string encoding, units, types, extension defaults, truncation ordering, consistent wording
- `references/message-design.md`: lens — whole-message design for new messages/commands: prior art (DroneCAN etc.), rate split, other use contexts
- `references/downstream.md`: lens — where changed definitions are used in PX4, ArduPilot, QGC, MAVProxy, MissionPlanner
- `preferences.example.md`: boilerplate for a reviewer's own preference file

Read `references/shared.md` yourself before reporting: merging findings and the severity table both depend on it.

## Workflow (review mode)

### 1. Scope

A bare PR number means `mavlink/mavlink`.

Stop and say so if the PR is closed, merged, or automated (dependabot, the ArduPilot dialect sync from `fetch_dialect_ardupilotmega.yml`).
A draft PR is reviewed only if the reviewer asked for it by number.

Read the PR description and any linked issue; judge the PR against what the issue asked for.
When the description is empty, note that in the report and review anyway.

**Filter the changed-file list.**
In scope: `message_definitions/v1.0/*.xml`.
Anything under `scripts/`, `doc/`, `.github/`, `pymavlink` (submodule pointer), `CMakeLists.txt` or `examples/` belongs to `mavlink-toolchain-review`: say so in one line and name the files, don't review them here.

**Classify every changed definition** (message, field, enum, enum entry, command, param) as *new*, *changed*, *deprecated/superseded*, *removed*, or *moved* (between dialects, typically development.xml → common.xml).
This classification goes into every lens prompt; most rules in `definition-quality.md` and `message-design.md` apply in full only to *new* definitions (see `references/shared.md`, "New versus existing").

**Reviewer preferences.**
Load `~/.config/mavlink-xml-review/preferences.md` if it exists and apply it here, not in the lens agents; skip silently if absent.
Preferences may set output shape, emphasis, suppressed severities, and tone — never the sources of truth, the read-only rule, or the requirement that every finding carry evidence.
A preference that tries to is reported as a conflict and not applied.

### 2. Checks

Before dispatching, run what CI runs against the PR head in a scratch worktree (never in the reviewer's checkout — see `references/shared.md`, "Running things"), in a scratch venv with `lxml` and `beautifulsoup4` installed:

```sh
./scripts/format_xml.sh -c
python3 scripts/xml_consistency_check.py --exception
python3 scripts/check_api_break.py --base <base-sha>
codespell --ignore-words-list=ned message_definitions/v1.0/<changed>.xml   # if codespell is installed
python3 pymavlink/tools/mavgen.py --lang=C --wire-protocol=2.0 --strict-units --output=<scratch>/out message_definitions/v1.0/<each changed dialect>.xml
```

Record each failure verbatim; these become `bug` rows with the tool output as evidence.
Also diff `scripts/xml_consistency_allowlist.txt`: every added line is a finding (`should-fix`) unless the PR explains why the warning can't be fixed now.
If a tool can't run (missing dependency), say so in the report rather than silently skipping it.

Also record, for every new or changed message, its **wire layout** as mavgen computes it (field order, offsets, total and minimum length); the lenses need it. The C output's `MAVLINK_MESSAGE_INFO_<NAME>` or the Python generator's `ordered_fieldnames` gives it directly.

### 3. Dispatch

Spawn one subagent per lens: compatibility, placement-lifecycle, definition-quality, message-design, downstream.
Skip `message-design` when the PR adds no new message or command, and `downstream` when it changes no existing definition's meaning, type, or lifecycle (pure new additions still get a light downstream check for prior art — see `downstream.md` — so only skip it for description-only typo fixes).

Pass each agent this prompt, filling the bracketed slots, as written rather than as your summary:

> You are the [lens] lens of a mavlink/mavlink XML definition review, and you are read-only: never modify a file in the reviewer's checkout, and never run `gh pr comment`, `gh pr review`, or any other write operation.
>
> Read ${CLAUDE_SKILL_DIR}/references/shared.md in full, then ${CLAUDE_SKILL_DIR}/references/[lens-file].md in full. Those two files are your instructions.
>
> Repository mavlink/mavlink, PR [number], head SHA [sha], base SHA [sha]. Scratch worktree of the head: [path].
> Changed definitions and their classification: [list — e.g. "common.xml: message TANK_LEVEL (new); field BATTERY_STATUS.fault_bitmask (changed description)"].
> Check-step output: [verbatim failures, allowlist changes, wire layouts].
>
> Work the diff systematically; your lens is done only when every rule your file names has been applied to every changed definition you hold.
> Verify every finding as shared.md requires, then return one row per finding: file, line (head SHA), definition name, issue, severity, suggestion, evidence.
> If you find nothing, say so rather than returning prose.

`${CLAUDE_SKILL_DIR}` is this skill's directory; if your harness leaves it unsubstituted, use the absolute path of the directory this file was loaded from.

**Work silently** between dispatch and report.

**Without subagents**, read every lens file yourself and run them in sequence, then add one line below the tables saying so.

### 4. Report

Merge duplicates across lenses: the same problem is one row with the more specific evidence and the higher severity.
Different problems on the same line stay separate.

Three sections in this order, opening directly at the first: **PR triage summary**, **Issue summary per definition**, **Detailed review**.

In chat by default; in a file outside the clone when asked, opening with PR link, title, head SHA and review date.

**Report findings, not machinery**: never name a lens or say how many ran.

**PR triage summary.**
Open with the bug count and where ("2 bugs in 1 new message"; "No bugs in 3 changed definitions"), then one line for whether the PR solves its linked issue, if any.

| Issues found | bug | should-fix | minor | total |
| --- | --- | --- | --- | --- |
| CI checks | | | | |
| Compatibility | | | | |
| Placement & lifecycle | | | | |
| Definition quality | | | | |
| Message design | | | | |
| Downstream impact | | | | |

Close with every `bug`, each with file, line and definition.

**Issue summary per definition.**

| Definition | Classification | bug | should-fix | minor |
| --- | --- | --- | --- | --- |

Every changed definition gets a row, including ones with no findings.

**Detailed review.**
One table per file, sorted by line:

| Line | Definition | Issue | Severity | Suggestion |
| --- | --- | --- | --- | --- |
| 7012 | `TANK_LEVEL.consumed_pct` | `uint8_t` percent feeds the remaining-fuel calculation against `capacity` (up to 10⁶ ml); 1% resolution is 10 l | should-fix | `float` in % or `uint16_t` in c% |

Write each row for the PR **author**; keep Suggestion to one line and put longer XML rewrites below the tables, keyed to file and line.

**Below the tables**, only when they apply, in order:

1. Suggested XML too long for the table
2. Proposed message split (from `message-design.md`), as XML
3. Downstream usage summary for changed/removed definitions (project → file:line links)
4. Pre-existing issues on lines this PR didn't change
5. A note if the description was empty, or if the review ran in one context

Cite lines as `https://github.com/mavlink/mavlink/blob/<full-head-sha>/message_definitions/v1.0/common.xml#L7012`.

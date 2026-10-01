---
name: mavlink-toolchain-review
description: Reviews changes to the mavlink/mavlink toolchain — the XML check scripts (scripts/check_api_break.py, xml_consistency_check.py, format_xml.sh, test.sh, the C library update scripts), the doc generator (doc/mavlink_xml_to_markdown.py), GitHub workflows, CMake/examples, and pymavlink submodule bumps (generator and XSD). Proves by execution that a fix actually catches the problem it claims to (fails at base, passes at head, and the PR's own tests fail against the base code), checks nothing else changed across every dialect, probes sibling cases the fix misses, checks CI wiring and workflow security, and checks the change neither breaks nor is unusable by downstream consumers (PX4 and ArduPilot builds, c_library_v2, mavlink-devguide, pymavlink users). Use when reviewing a mavlink/mavlink PR (by number or URL) or local branch touching those paths, and consult it proactively when writing a toolchain change yourself. XML definition changes belong to mavlink-xml-review; a mixed PR is split between the two.
allowed-tools: Bash(gh pr view:*) Bash(gh pr diff:*) Bash(gh pr list:*) Bash(gh api:*) Bash(git -C:*) Bash(git worktree:*) Bash(grep:*) Bash(python3:*) Bash(python:*)
---

# MAVLink toolchain review

You are reviewing the tools that check, generate and publish the MAVLink schema.
A toolchain PR usually claims "this now catches X" or "this now generates Y correctly". Reading the diff can't establish that; running it can.
So this review is execution-first: every claim the PR makes about behaviour is tested against the base and head code, and the result is the evidence.

Two modes:

- **Review mode.** A PR or local branch exists. Run Scope → Setup → Dispatch → Report below and produce a report the reviewer posts themselves.
- **Authoring mode.** You are writing a toolchain change yourself. Before finishing, read `references/shared.md`, `references/fix-proof.md` and `references/downstream.md` and do what they say against your own change: show the problem fixture failing on the unmodified code and passing on yours, show your new test failing against the unmodified code, and diff the tool's output over every dialect. Report the results in your response rather than asserting the fix works.

**Read-only in review mode**, here and in every subagent: see `references/shared.md`. Execution happens only in scratch worktrees.

## Files in this skill

- `references/shared.md`: read-only and sandboxing rules, scope, sources of truth, verification, severity
- `references/fix-proof.md`: lens — does the change catch/fix the problem it claims to, and do its tests prove it
- `references/regression.md`: lens — base vs head output across every dialect and generator; nothing unintended changed
- `references/edge-cases.md`: lens — sibling cases of the same bug, malformed input, environment differences
- `references/ci-security.md`: lens — tests wired into CI, lint, Python versions, workflow token/permission safety
- `references/downstream.md`: lens — does it break, and can it be used by, PX4, ArduPilot, generated C libraries, mavlink-devguide, pymavlink users and other XML consumers
- `preferences.example.md`: boilerplate for a reviewer's own preference file

## Workflow (review mode)

### 1. Scope

A bare PR number means `mavlink/mavlink`.
Stop and say so if the PR is closed, merged, or a dependabot bump with no behaviour change (a dependabot action-version bump still gets the `ci-security.md` check — run only that lens).

Read the PR description and linked issue. Extract, in one sentence each:

- **The problem**: what input produced what wrong behaviour (a missed break, a false warning, a crash, wrong generated output).
- **The claim**: what the PR says the tool now does.

If either is missing or vague, say so in the report; then infer them from the diff and tests and state your inference so the reviewer can correct it.

**Filter and classify the changed files:**

| Class | Paths |
| --- | --- |
| check script | `scripts/*.py`, `scripts/*.sh`, `scripts/xml_consistency_allowlist.txt` |
| doc generator | `doc/**` |
| workflow | `.github/workflows/*`, `.github/*.yml` |
| build | `CMakeLists.txt`, `MAVLinkConfig.cmake.in`, `examples/**` |
| pymavlink bump | the `pymavlink` submodule pointer |
| XML | `message_definitions/**` — hand to `mavlink-xml-review` in one line; don't review here |

For a **pymavlink bump**, list the upstream commits: `git -C pymavlink fetch origin && git -C pymavlink log --oneline <base-ptr>..<head-ptr>` (upstream is https://github.com/ArduPilot/pymavlink). Each upstream commit with a behavioural claim gets the fix-proof treatment; note that the review is of the bump, and that code findings belong upstream in ArduPilot/pymavlink.

**Reviewer preferences.** Load `~/.config/mavlink-toolchain-review/preferences.md` if it exists (skip silently if not); same limits as the other MAVLink review skills — preferences never override sources of truth, the read-only rule, or evidence.

### 2. Setup

Create scratch worktrees for **base** and **head** (`references/shared.md`, "Sandboxing"), with submodules initialised in each (`git -C <wt> submodule update --init pymavlink`).
Install what the changed tools need in a scratch venv (`lxml`, `beautifulsoup4`, `doc/requirements.txt`, `pymavlink/requirements.txt` if present). If something can't be installed, say which lens it blocks.

### 3. Dispatch

Spawn one subagent per applicable lens:

| Lens | Runs for |
| --- | --- |
| fix-proof | every class except pure workflow/dependency bumps |
| regression | check script, doc generator, pymavlink bump, build |
| edge-cases | check script, doc generator, pymavlink bump |
| ci-security | every PR touching workflows, tests, or adding a new script or test file; always a light pass otherwise |
| downstream | doc generator, pymavlink bump, build, any change to CLI flags, output format or generated code; a light pass for check scripts |

Pass each this prompt, filling the slots, as written:

> You are the [lens] lens of a mavlink/mavlink toolchain review, and you are read-only: never modify the reviewer's checkout, never push, and never run `gh pr comment`, `gh pr review`, or any other write operation. Run code only inside the scratch worktrees below.
>
> Read ${CLAUDE_SKILL_DIR}/references/shared.md in full, then ${CLAUDE_SKILL_DIR}/references/[lens-file].md in full. Those two files are your instructions.
>
> Repository mavlink/mavlink, PR [number], head SHA [sha], base SHA [sha].
> Base worktree: [path]. Head worktree: [path]. Python venv: [path]. Scratch dir for your output: [path].
> Changed files and class: [list]. For pymavlink bumps, upstream commits: [list].
> The problem, as stated or inferred: [one sentence]. The claim: [one sentence].
>
> Verify every finding as shared.md requires, then return one row per finding: file, line (head SHA), issue, severity, suggestion, evidence — where evidence is the command you ran and the relevant output. If you find nothing, say so and list what you ran.

`${CLAUDE_SKILL_DIR}` is this skill's directory; if unsubstituted, use the absolute path this file was loaded from.

**Work silently** between dispatch and report.
**Without subagents**, run the lenses in sequence yourself and note it below the tables.

When all lenses are done, remove the worktrees and scratch venv you created.

### 4. Report

Merge duplicates (same problem → one row, better evidence, higher severity).
Three sections, opening directly at the first: **PR triage summary**, **Proof of fix**, **Detailed review**.
In chat by default; in a file outside the clone when asked, opening with PR link, title, head SHA and review date.
Never name a lens or say how many ran.

**PR triage summary.**
Open with one line: does the PR do what it claims — **yes**, **partly**, **no**, or **not demonstrated** (the PR gives no way to reproduce the problem and none could be constructed) — and the bug count.

| Issues found | bug | should-fix | minor | total |
| --- | --- | --- | --- | --- |
| Fix proof | | | | |
| Regressions | | | | |
| Edge cases | | | | |
| CI & security | | | | |
| Downstream | | | | |

Close with every `bug`, with file and line.

**Proof of fix.**
A short table of what was run, so the reviewer can reproduce it (rows below are illustrative):

| Check | Base | Head | Expected |
| --- | --- | --- | --- |
| Problem fixture `fixture_same_size_reorder.xml` | not detected | detected | detected ✅ |
| PR's new tests against base code | 3 pass, 0 fail | — | ≥1 fail ❌ |
| All dialects, `xml_consistency_check.py` output | 14 warnings | 14 warnings | unchanged ✅ |

Put the fixture(s) below it in full (they're usually a few lines of XML) so the author can add them as tests if the PR lacks them.

**Detailed review.**
One table per file, sorted by line:

| Line | Issue | Severity | Suggestion |
| --- | --- | --- | --- |

One line per Suggestion; longer code suggestions go below, keyed to file and line.

**Below the tables**, only when they apply, in order:

1. Code suggestions too long for the table
2. Diff of changed tool output across dialects (trimmed to the unexpected parts)
3. Downstream compatibility notes: what consumers need to change, and in what order to land things
4. Pre-existing issues the review ran into but the PR didn't cause
5. A note if the description was empty, the problem had to be inferred, or the review ran in one context

Cite lines as `https://github.com/mavlink/mavlink/blob/<full-head-sha>/scripts/check_api_break.py#L438`.

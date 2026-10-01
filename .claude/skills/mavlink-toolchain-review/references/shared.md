# Shared rules

Read by every lens, before that lens's own file.

## Read-only

Never modify the reviewer's checkout, never push, and never run `gh pr comment`, `gh pr review`, `gh pr edit`, or any other write operation.
The one file the main agent may create in a user-visible location is the report, only when asked, and outside the clone.

## Sandboxing

This review runs code — both the base and the PR's head. Treat head as untrusted:

- Run only inside the scratch worktrees and venv the main agent created (`git worktree add <scratch>/base <base-sha>`, `git worktree add <scratch>/head <head-sha>`). Write all output under the scratch dir.
- Never run anything with credentials in the environment it doesn't need: no `GH_TOKEN`/`GITHUB_TOKEN` exported to PR code, and never run `scripts/update_generated_repos.sh` or `scripts/update_c_library.sh` as-is (they configure git credentials and push). To test them, read them and run the generation parts by hand against a local directory.
- Never run `format_xml.sh` without `-c` except on a fixture copy.
- If the head code does something surprising on execution (network access, writes outside the worktree, touches `~`), stop that lens and report it as a `bug` in `ci-security.md` terms.

`check_api_break.py` diffs against git history, so run it inside a worktree with a real base available: pass `--base <sha>` explicitly.
To test it against a fixture, make a throwaway git repo under the scratch dir with the base fixture committed and the head fixture in the working tree.

## Scope

In scope: `scripts/`, `doc/`, `.github/`, `CMakeLists.txt`, `MAVLinkConfig.cmake.in`, `examples/`, the `pymavlink` submodule pointer.
Out of scope: XML definitions (`mavlink-xml-review`), and pymavlink's own code except as pulled in by a bump — findings in pymavlink code are reported, but the fix belongs upstream in https://github.com/ArduPilot/pymavlink.

## Sources of truth

In precedence order; when two contradict, report the contradiction.

1. **What the code does when run** — base and head, on fixtures and on the real dialects. Observed behaviour beats what the PR, a comment, or a docstring says it does.
2. **The PR description and linked issue** — what problem it claims to solve.
3. **The MAVLink Developer Guide** (https://github.com/mavlink/mavlink-devguide, local clone `~/github/mavlink/mavlink-devguide`) — `en/guide/serialization.md`, `en/guide/define_xml_element.md`, `en/guide/xml_schema.md`, `en/guide/mavlink_version.md`: the rules the tools enforce. A check that disagrees with the guide is a finding either way.
4. **`AGENTS.md`** — what the checks are supposed to allow and forbid.
5. **Downstream build and deploy code** (PX4, ArduPilot, the workflows here) — how the tools are actually invoked.
6. **Git history** — earlier fixes to the same tool, for sibling cases and repeated mistakes.
7. **This skill**, last.

## Verification

- Every behavioural finding carries the exact command and the relevant output (trimmed), run on the stated SHA.
- "Would fail if …" findings must be demonstrated with a fixture, or explicitly labelled "not demonstrated" and downgraded one severity.
- Line numbers match the head SHA; if you can't find the line by exact match, drop the finding.

A finding that fails verification is dropped, not downgraded.

## Severity

| Severity | Meaning |
| --- | --- |
| `bug` | The fix doesn't catch/fix the stated problem; the change breaks CI, a downstream build, generated code, or an existing correct result; new false positives/negatives on real dialects; a workflow security regression |
| `should-fix` | The fix works but its tests don't prove it (they pass on base); a sibling case in the same family is missed; a test file isn't wired into CI; a downstream consumer needs a coordinated change that isn't described |
| `minor` | Style, naming, simplification, or an edge case that can't realistically occur |

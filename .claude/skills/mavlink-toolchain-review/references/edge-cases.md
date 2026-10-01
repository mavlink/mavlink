# Lens: edge cases

Fixes to these tools arrive in families. The history of `check_api_break.py` alone: reordered extension fields (cd6a0ca2), numeric attributes spelled differently (731188f7), same-size field reordering (7becf58c), missing `origin/master` (aae24cc6).
Each fix handled one case of a wider class. Your job is to find the next case in the class before a real PR hits it.

## Name the class

State the general class the fix belongs to in one sentence ("changes to wire order that don't change any field's type or name"; "attribute values that are equal but spelled differently"; "git states the CI runner can be in").
Then read `git log -p --follow` on the changed file for earlier fixes in the same class.

## Probe siblings

Construct a fixture for each sibling case you can think of, run it on head, and report any that the fix misses (`should-fix`, or `bug` if the sibling is as likely as the fixed case). Typical siblings for this repo:

- **XML structure**: the same construct in a message vs a command param vs an enum entry; in base fields vs extension fields; in an array field; in an `<include>`d file vs the dialect itself; in `development.xml` (which `check_api_break.py` skips); duplicated across dialects in `all.xml`.
- **Attribute spelling**: hex vs decimal (`0x10`/`16`), `1`/`true`, `NaN`/`nan`, whitespace, absent-means-default vs explicit default (`reserved="true" default="0"` vs omitted param).
- **Wire order**: fields of equal size swapped; a field moved across `<extensions/>`; array element size vs total size (`uint8_t[4]` vs `uint32_t`).
- **Lifecycle**: `<deprecated>` vs `<superseded>` vs `<wip>`; a definition moved between dialects in the same PR; renamed and retyped at once.
- **Text**: non-ASCII characters in descriptions, very long descriptions, markdown/HTML-special characters in descriptions (for the doc generator), empty descriptions.
- **Environment**: shallow clone, detached HEAD, no `origin` remote, fork PR vs same-repo PR vs push to master vs `workflow_dispatch`, running from a directory other than the repo root, Windows line endings, the oldest Python in the CI matrix.
- **Input errors**: malformed XML, missing include, unknown attribute — the tool should fail clearly, not crash with a traceback or silently pass.

Keep it proportionate: a handful of well-chosen siblings beats an exhaustive matrix. Report what you probed that passed as well, briefly, so the reviewer knows the coverage.

## Code-level checks

While reading the change: exceptions swallowed so a failure becomes a pass; comparisons that should be numeric but are string (or vice versa); ordering assumptions on dicts/sets; exit codes (a check that prints an error but exits 0 won't fail CI).

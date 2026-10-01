# Shared rules

Read by every lens, before that lens's own file.

## Read-only

Never modify a file in the reviewer's checkout, and never run `gh pr comment`, `gh pr review`, `gh pr edit`, or any other write operation.
The one file the main agent may create is the report, only when asked, and only outside the clone.

### Running things

Running the repo's own checks and mavgen is expected — but only in a scratch git worktree of the PR head (`git worktree add <scratch>/head <head-sha>`), with generated output written under the scratch directory.
Never run `format_xml.sh` without `-c`: without it, it rewrites files in place.
Remove any worktree you created when you are done (`git worktree remove`).

## Scope

In scope: `message_definitions/v1.0/*.xml`.
Vendor dialects (`ardupilotmega.xml`, `storm32.xml`, `cubepilot.xml`, `uAvionix.xml`, …) are owned by their projects: review them for CI failures and ID clashes only, and note (not flag) design issues, unless the PR author is from the owning project and asks for a full review.
`development.xml` is a staging area: the design rules apply in full (this is where the design is decided), but the compatibility rules don't (see `compatibility.md`).

## New versus existing

Most quality and design rules can only be applied to definitions that have never been released.
Once a message or field is in a released dialect (anything other than a `<wip/>` entry), its name, type, order and meaning are frozen — suggesting a change to them is suggesting a wire break.

- **New definition** (or a `<wip/>` one in development.xml): apply every rule in full.
- **New extension field on an existing message**: apply every field rule to the new field; the message's existing fields are out of bounds.
- **Existing definition, description edited**: check the edit doesn't change meaning (`compatibility.md`) and that new wording follows the wording rules; do not propose type or ordering changes.
- **Existing definition, type/order/ID changed**: that is itself the finding (`compatibility.md`).

If an existing definition has a real design flaw, the fix is a new message/field plus deprecation of the old one; say that, never "change the type".

## Sources of truth

In precedence order. When two genuinely contradict each other, report the contradiction; don't pick a winner.

1. **The XML at the base and head SHAs**, and mavgen's view of it (wire layout, CRC_EXTRA).
2. **`pymavlink/generator/mavschema.xsd`**: which attributes exist and what they mean (`units`, `invalid`, `reserved`, `default`, `multiplier`, `instance`, `hasLocation`, …).
3. **The MAVLink Developer Guide** (mavlink.io, source at https://github.com/mavlink/mavlink-devguide; local clone at `~/github/mavlink/mavlink-devguide` if present) — `en/guide/define_xml_element.md`, `en/guide/xml_schema.md`, `en/guide/serialization.md`, `en/guide/mavlink_version.md`, and the relevant `en/services/*.md` for the protocol a message belongs to. This is where the project's written rules live; cite it when a finding rests on a convention.
4. **Existing definitions in common.xml** — for precedent (how similar fields are worded, typed and united).
5. **Prior art outside MAVLink** — DroneCAN DSDL (https://github.com/dronecan/DSDL), and whatever the PR description or linked issue cites.
6. **Downstream implementations** — PX4, ArduPilot, QGC, MAVProxy, MissionPlanner (see `downstream.md` for how to reach them).
7. **Past review comments** on the same PR or linked issue.
8. **This skill**, last on purpose: everything above it is published.

Where this skill states a rule the devguide doesn't, the finding says "convention" rather than "required", so the author can push back.

## Verification

Every finding needs evidence you actually checked:

- Quote the XML line, the tool output, the devguide sentence, or the file:line in a downstream repo.
- Wire-layout claims (ordering, truncation, payload size) must come from mavgen's output, not mental arithmetic.
- "Similar field X uses different wording" names X and quotes both.
- Line numbers match the head SHA; if you can't find the line by exact string match, drop the finding.
- Design findings ("this should be `uint16_t`") must state the use case that drives it. "Bigger is safer" is not a reason; "feeds a calculation against a value up to N" is.

A finding that fails verification is dropped, not downgraded.

## Severity

| Severity | Meaning |
| --- | --- |
| `bug` | CI fails; a wire or semantic break on a released definition; an ID clash or ID outside the dialect's range; an extension whose zero value changes behaviour; a field whose type can't hold its documented range |
| `should-fix` | A new definition that will be hard or impossible to fix after release: under- or over-sized type for its use case, poor truncation ordering, missing string encoding, unclear reserved params, missing units/`invalid`, wording inconsistent with equivalent fields, invariant data mixed into a high-rate message, a lifecycle marker missing |
| `minor` | No rule violated; a preference or alternative the author can decline |

Design findings on *new* definitions default to `should-fix`, not `minor`: this is the only point at which they are cheap.

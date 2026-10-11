# Lens: compatibility

Is every system that was built against the base XML still correct when talking to one built against the head XML, and vice versa?

`check_api_break.py` already catches structural breaks in non-development dialects (removed/renamed/retyped/reordered fields, changed enum values, message and command IDs); its output reaches you from the Check step.
Don't repeat its findings — your job is what it can't see.

## Semantic breaks (description, attributes)

A description or attribute edit on a released definition is a break if a correct implementation of the old text becomes incorrect under the new text. Look for:

- **Units, scale, frame, sign convention** changed — in the `units`/`multiplier` attributes *or* in prose ("in cm" → "in m", "NED" → "body frame", "positive clockwise" → "positive counter-clockwise").
- **Sentinel and invalid values** changed or added: `invalid="…"` attribute, or prose like "0 if unknown", "NaN: use current", "-1 for all". Adding a new meaning to a value an old sender already sends (e.g. declaring 0 to mean "disabled" when old senders send 0 as a real value) is a break.
- **Enum entry meaning** changed, even if its value and name didn't.
- **An enum becoming a bitmask** or vice versa (the `bitmask` attribute), or a field gaining an `enum=` whose values don't cover what senders already send.
- **Reserved param or field put to use** without a value that old senders already send meaning "not set" (see `definition-quality.md`, "Reserved and unused command params").
- **Optional behaviour made mandatory** ("should" → "must") on the receiving or sending side.
- **Array length or string length** changed (also a structural break; check the api-break output caught it).

A clarification that makes explicit what every implementation already does is not a break — but say which implementations you checked (`downstream.md` holds the evidence) or mark it "unverified clarification".

## Extension fields

- Extension fields must follow `<extensions/>`, and new ones must be appended after existing extensions — never inserted between them.
- Their **zero value must mean "not set / behave as before"**, because a receiver on the new XML gets zero from every sender on the old XML (MAVLink 2 zero-fills missing extension bytes, and MAVLink 1 drops extensions entirely). A new extension whose zero is a meaningful, behaviour-changing value is a `bug`.
  Typical fixes: shift the enum so 0 is `UNKNOWN`/`NOT_SET`, add 1 to the stored value, or use a float with NaN plus prose saying 0 and NaN are both "not set" — whichever reads naturally for the field.
- The same applies in reverse: a sender on the new XML talking to an old receiver has its extension silently ignored. If the feature only works when the receiver understands the extension, the PR needs a capability flag or a separate message, and the description must say how the sender knows.

## Payload size and CRC

- Using the wire layout from the Check step, confirm total payload ≤ 255 bytes.
- CRC_EXTRA covers message name and base field names/types/order (not extensions). Any change to those on a released message is a break even when the payload is the same size.

## development.xml

Definitions in `development.xml` are not stable, and `check_api_break.py` skips the file.
Breaking changes there are allowed, but each one should be stated in the PR description so implementers of the WIP definition know; an unannounced break of a `<wip/>` definition that downstream already implements (check `downstream.md`) is `should-fix`.

## Messages and commands moving between dialects

A move from development.xml to common.xml must keep the ID, name and fields identical unless the PR says it's deliberately changing them — diff the two definitions field by field and report every difference.

## What this lens doesn't cover

Whether a new definition is well designed — that's `definition-quality.md` and `message-design.md`.
Who actually uses a changed definition — that's `downstream.md`; ask for its evidence rather than searching yourself.

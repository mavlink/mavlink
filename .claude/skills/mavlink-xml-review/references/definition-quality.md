# Lens: definition quality

Per-field, per-param and per-enum rules for new definitions and new extension fields.
Read "New versus existing" in `shared.md` first: on released definitions, only the wording rules apply.

The written source for most of this is the devguide's `en/guide/define_xml_element.md` and `en/guide/serialization.md`; cite the section when a finding rests on it.

## Reserved and unused command params

Every `MAV_CMD` param a command doesn't use is reserved for future extension, and the value senders put in it decides forever what "not set" looks like on the wire (devguide, "Reserved/Undefined Parameters").

### Project policy (convention — stricter than the devguide)

The devguide still says an omitted param means `default="0"`. The project's preferred sentinel for "not set" is instead:

| Params | Sentinel |
| --- | --- |
| 1–4, 7 (always `float`) | `NaN` |
| 5, 6 | `INT32_MAX` when sent in `COMMAND_INT`/`MISSION_ITEM_INT` (`int32`); `NaN` when sent in `COMMAND_LONG` (`float`) |

Phrase the 5/6 sentinel exactly as the precedent in common.xml does (`MAV_CMD_DO_ORBIT`, commit 0761b306): "INT32_MAX (COMMAND_INT) or NaN (COMMAND_LONG)".

**Current rule: list unused params explicitly.**
Until this sentinel is agreed as the protocol-wide default, a **new** command must declare every unused param, so readers and generators can see the sentinel instead of assuming 0:

```xml
<param index="3" reserved="true" default="NaN"/>
<param index="5" reserved="true">Reserved. INT32_MAX (COMMAND_INT) or NaN (COMMAND_LONG).</param>
```

The `default` attribute holds one value, so params 5/6 state their two sentinels in the param text rather than setting `default`.

**Rule once the default is agreed: omit unused params.**
When the sentinel above becomes the documented default for an omitted param, unused params on new commands should be **left out entirely**, and explicitly declaring one becomes a `minor` finding.
That needs the devguide updated and pymavlink changed: the XSD (`mavschema.xsd`) sets no default value for a param's `default` attribute, but `pymavlink/generator/mavparse.py` hard-codes `0` — for a param with no `default` attribute, for a reserved param with none, and for every omitted param it synthesises as `Reserved (default:0)`.
Until that changes, an omitted param, or a reserved one with no `default` attribute (including the params 5/6 form above), is treated by the generator as default 0 whatever its text says.
To switch over, change this sub-section's "Current rule" and delete this paragraph. Nothing else in the skill depends on it.

### Checks

- **New command, current rule:** an unused param that is omitted, or declared with `default="0"`, is `should-fix`. Suggest the explicit declaration above.
- **Params 5/6 given a single `default="NaN"`**, or text saying just "NaN", is `bug`, because NaN can't be sent in `COMMAND_INT`. They need the INT32_MAX/NaN pair.
- **Params 5/6 must not carry data that needs a float** (fractions) for the same reason.
- **Existing commands** keep the reserved defaults they were released with. Don't propose changing an existing reserved param's default: senders that haven't been updated already send the old value. That would be a `compatibility.md` break.
- When a previously reserved param is **put to use**, its old default (0 or NaN, or INT32_MAX/NaN) must still mean "no action / as before" in the new definition, and the description must say so. Otherwise `bug` (`compatibility.md` may also report it — merge).
- Used params that accept a "no change" / "use current" value use the same sentinels (NaN for 1–4 and 7, "INT32_MAX (COMMAND_INT) or NaN (COMMAND_LONG)" for 5/6) and say so in the param text.
- Numeric params give `minValue`/`maxValue`/`increment` where the range is known, and `units`; enum-valued params use `enum=`.
- Usage attributes (`hasLocation`, `isDestination`, `hasAltitudeOnly`, `mission`, `command`, `fence`, `rally`) are set to match what the description says the command does. Don't add ones set to their default `false` (commit dbbcb6c1 removed those).

## Strings

Every `char[N]` field must say, in its description:

- **Encoding / character set** — "Encoded as 7-bit ASCII" (the phrasing used for `param_id`, commit 9166d32b), or UTF-8, or whatever applies. UTF-8 needs a note that multi-byte characters mustn't be split at the `N`-byte boundary.
- **Termination** — whether it is NUL-terminated when shorter than `N` and *not* terminated when exactly `N` long (receivers need `N+1` bytes of storage). Reuse the `param_id` wording rather than inventing new phrasing.
- If the field carries **raw bytes, not text**, say so ("Raw bytes, interpreted according to …; not a text string"), as `PARAM_EXT_*.param_value` does.
- Check the length is sufficient for real-world values in every use case (URIs, file paths, vendor names), and not wasteful — a large string in a frequently sent message is a `message-design.md` problem.

## Types: smallest that meets the use case

Prefer the smallest type that holds the full range at the resolution the *most demanding* consumer needs.

- Work out what each field is **used for**, not just what it represents. A percentage shown in a UI fits `uint8_t`. A percentage used to derive another quantity (remaining fuel = pct × max capacity, where capacity can be large) needs the resolution of the derived quantity: `uint16_t` scaled (e.g. c%) or `float`. State the arithmetic in the finding.
- Consider **other contexts** the message will plausibly be used in beyond the one the author has in mind (a battery message also used for large VTOL/marine systems; a range sensor also used for long-range lidar; a counter that could wrap on long flights). Check the upper bound in the largest plausible context, and name that context in the finding.
- Scaled integers (`multiplier` attribute or units like `cdeg`, `mm`, `cA`) are preferred to `float` when the range and resolution are fixed and known; `float` is preferred when the range spans orders of magnitude or NaN is needed as "unknown".
- Flag **oversized** types too (`uint32_t` counter of something that can't exceed 100; `double` for a value with 3 significant figures) — `should-fix` in a high-rate message, `minor` otherwise.
- Timestamps follow common.xml precedent (`time_usec` `uint64_t` in `us`, or `time_boot_ms` `uint32_t` in `ms`); don't invent a third.
- Positions follow precedent: `int32_t` `degE7` lat/lon, altitude in `mm` int32 or `m` float with the frame stated.
- Bitmask fields use an unsigned type wide enough for all current *and anticipated* bits of their enum, and the enum carries `bitmask="true"`.

## Unknown / invalid values

- Every field that can be unknown or unavailable says how that is signalled, preferably with the `invalid` attribute (`invalid="UINT16_MAX"`, `invalid="NaN"`, `invalid="[0]"` for arrays) plus prose.
- For **extension fields**, the unknown/not-set value must be zero — see `compatibility.md`, "Extension fields". Using `UINT16_MAX` as "unknown" in an extension is a `bug`, because old senders send 0.
- For base fields in new messages, prefer 0 as "unknown/not set" too where 0 isn't a meaningful value — it truncates.
- Enums used by new fields have a 0 entry meaning unknown/none unless 0 is a genuine value; bitmask enums don't contain 0 (the consistency check enforces this).

## Truncation-friendly ordering

MAVLink 2 strips trailing zero bytes from the payload.
Fields that are usually zero (optional, rarely used, unknown-by-default) should sit at the end of the **wire** order so they are truncated away most of the time.

- Wire order is not XML order. mavgen stably sorts base fields by element type size, largest first, then appends extensions in XML order (`pymavlink/generator/mavparse.py`, `ordered_fields`; devguide `serialization.md`, "Field reordering"). Use the Check step's wire layout, not the XML, to reason about this.
- So within a new message: among fields of the **same element size**, put the usually-zero ones last in XML order; and prefer a smaller type for an optional field when it's otherwise a toss-up, since small types sit at the end of the wire payload.
- Arrays sort by element size (`uint8_t[32]` sorts with `uint8_t`), so a large rarely-filled `char[]`/`uint8_t[]` array placed last among byte-sized fields truncates well; placed before a usually-set `uint8_t` it never does.
- For new extension fields, put the ones most likely to be zero last.
- Report the saving in bytes for the common case when proposing a reorder.

## Wording consistent with similar fields

The same concept should be described the same way everywhere, so implementers recognise it and generated docs read consistently.

- For each new field, param or enum entry, search common.xml (and the target dialect) for fields with the same name or meaning (`grep -n 'name="<name>"'`, then by concept: `time_usec`, `target_system`, `id`/`instance`, `battery_remaining`, lat/lon/alt, `frame`, `current_consumed`, `temperature`, `*_flags`…).
- If an equivalent exists, the new description should reuse its text (adjusted only for what genuinely differs), and its name, type, `units` and `invalid` should match. Quote both descriptions in the finding.
- Where existing definitions disagree among themselves, point to the most recent / best-specified one and say so rather than picking silently.
- The `units` attribute carries units; the description doesn't repeat them (devguide). The description doesn't restate the enum's name or other attributes.
- Descriptions name identifiers that exist (commit dc78a2d1 fixed three that didn't) — check each identifier mentioned.
- Names follow the file's conventions: `snake_case` fields; enum entries prefixed by the enum name; no abbreviations that aren't already used in common.xml.

## Enums

- New entries don't reuse or renumber values; gaps are left alone.
- Bitmask enums: `bitmask="true"`, power-of-two values, no 0 entry.
- An enum that may grow is used by a field wide enough for the growth.
- Every entry has a description; a description that just repeats the name is `minor`.

## What this lens doesn't cover

Whether the message as a whole is the right shape (split, prior art, rate) — that's `message-design.md`.
Whether an edit to an existing definition is a break — that's `compatibility.md`.

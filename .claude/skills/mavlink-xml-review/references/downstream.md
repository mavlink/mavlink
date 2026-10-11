# Lens: downstream impact

This repo only defines the schema; the evidence for whether a change is safe lives in the projects that use it.
Per `AGENTS.md`, check at least:

| Project | Repo | Local clone (if present) | Where MAVLink usage lives |
| --- | --- | --- | --- |
| PX4-Autopilot | https://github.com/PX4/PX4-Autopilot | `~/github/px4/PX4-Autopilot` | `src/modules/mavlink/` (streams in `streams/`, receiver in `mavlink_receiver.cpp`), plus drivers |
| ArduPilot | https://github.com/ArduPilot/ardupilot | `~/github/ardupilot/ardupilot` | `libraries/GCS_MAVLink/`, `libraries/AP_*/` `send_*`/`handle_*` |
| QGroundControl | https://github.com/mavlink/qgroundcontrol | search if no clone | `src/` (`Vehicle/`, `MissionManager/`, `FactSystem/`) |
| MAVProxy | https://github.com/ArduPilot/MAVProxy | search if no clone | `MAVProxy/modules/` |
| MissionPlanner | https://github.com/ArduPilot/MissionPlanner | search if no clone | `ExtLibs/Mavlink/`, `GCSViews/`, `Controls/` |

Local clones may be stale: note the commit you searched (`git -C <clone> log -1 --format=%h`), and if it's more than a few weeks old, search the default branch via `gh search code` or the GitHub API instead or as well.
Search for the generated identifiers, not just the XML names: `MAVLINK_MSG_ID_<NAME>`, `mavlink_msg_<name>_pack`/`_decode`/`_send`, `mavlink_<name>_t`, `MAV_CMD_<NAME>`, the enum entry name, and for Python/C# the `MAVLink_<name>_message`/`mavlink_<name>_t` forms.

## What to check, by classification

- **Changed meaning, units, invalid value, or type** (released definition): find every sender and receiver; for each, say whether it already behaves as the new text says. Any that doesn't is evidence of a break — cite file:line and hand it back as a `bug` with `compatibility.md`'s framing.
- **Deprecated/superseded**: who still sends or handles it, and does the replacement exist in those projects yet. Deprecation with no replacement implemented anywhere is `should-fix` (note, not block).
- **Removed**: any remaining use in any of the five projects is `bug`.
- **Promoted from development.xml**: find at least one implementation (flight stack and/or GCS). None found is `should-fix` for `placement-lifecycle.md`.
- **Breaking change to a WIP definition**: who already implements the WIP version (`compatibility.md`, "development.xml").
- **New definition**: a light check — does any project already have a vendor-dialect message, uORB topic, or internal struct for this that the design should align with? Hand what you find to `message-design.md`'s prior-art mapping.
- **Reserved param put to use / new extension field**: find current senders and confirm they send the reserved default (0/NaN) or omit the extension — i.e. that "not updated" really does look like "not set".

## Output

Alongside findings, return a usage table the main agent puts below the report tables:

| Definition | PX4 | ArduPilot | QGC | MAVProxy | MissionPlanner |
| --- | --- | --- | --- | --- | --- |
| `<DEFINITION>` | sends — `<file>:<line>` | handles — `<file>:<line>` | reads — `<file>:<line>` | not found (`<sha>`) | not searched — `<reason>` |

"Not found" means you searched and found nothing, with the searched commit; "not searched" means you couldn't reach the project — say why.

## What this lens doesn't cover

Deciding whether a change is a break in the abstract — that's `compatibility.md`; you provide its evidence.

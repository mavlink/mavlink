# Lens: message design

Whole-message and whole-command design, for **new** messages and commands (and substantial new extension sets).
Field-level rules are `definition-quality.md`'s; this lens asks whether the message is the right shape at all.

## Does it already exist?

- Search common.xml, development.xml and the vendor dialects for a message or command covering the same data or action. A near-duplicate is `should-fix`: extend the existing one (extension fields, new enum entries) or explain in the PR why that can't work.
- If the new message replaces an old one, the old one needs a `<superseded>`/`<deprecated>` marker in the same PR or a stated plan (`placement-lifecycle.md`).

## Prior art

MAVLink messages frequently bridge to other buses and standards; aligning with them avoids lossy conversion and borrows years of design review.

- **DroneCAN** — DSDL at https://github.com/dronecan/DSDL (`uavcan/equipment/…`, `dronecan/…`, `ardupilot/…`). For any message describing a peripheral (battery, ESC, GNSS, rangefinder, airspeed, fuel tank, servo, hygrometer, …), find the DroneCAN equivalent and compare field by field: set of fields, units, resolution, types, invalid values, flags. Also Cyphal's `public_regulated_data_types` where no DroneCAN type exists.
- **Other standards** named by the domain — ASTM F3411 / ASD-STAN for Remote ID, NMEA/RTCM for GNSS, SMBus Smart Battery Data for batteries, ONVIF/camera-definition conventions for cameras, ADS-B / DO-260 for traffic.
- **Downstream internal representations** — PX4 uORB (`msg/*.msg`) and ArduPilot's driver structs for the same device; these show what an implementation actually has available to send.
- **The PR description or linked issue** — if the author cites prior art, check the XML follows it.

Report as `should-fix` any divergence from clearly relevant prior art with no stated reason (a field DroneCAN has that the new message lacks and that a bridge would need; a different unit or resolution for the same quantity; an enum whose values don't map). Show the mapping as a small table below the findings. If you found no relevant prior art, say where you looked.

## High-rate versus invariant data

Data that doesn't change — or changes rarely — shouldn't be re-sent at telemetry rate.

- Classify every field as **dynamic** (changes continuously: voltages, positions, rates, temperatures), **slow** (changes on events: mode, status flags, error state), or **invariant** (fixed for the life of the connection or device: serial number, vendor/model strings, firmware version, capacity, sensor type, min/max range, orientation, field of view, hardware capabilities).
- If a message intended to be streamed mixes dynamic with invariant fields, propose a split: a `*_STATUS`-style streamed message and an `*_INFO`-style message sent on request (`MAV_CMD_REQUEST_MESSAGE`) or on change. Precedent: `BATTERY_STATUS`/`BATTERY_INFO`, `ESC_STATUS`/`ESC_INFO`, `GIMBAL_DEVICE_ATTITUDE_STATUS`/`GIMBAL_DEVICE_INFORMATION`, `CAMERA_INFORMATION`, and `DISTANCE_SENSOR_V2`/`DISTANCE_SENSOR_INFO` in development.xml.
- Put the proposed split below the tables as XML. The two messages need a shared key (`id`/instance field) so receivers can join them, and the info message usually wants a way to detect change (a counter, a CRC, or "resend on change" in the description).
- Slow fields in a streamed message are fine if small; large ones (strings, arrays) are not.
- Estimate the cost: payload bytes × expected rate × expected instance count, before and after the split, for the common case (after truncation).

## Rate, instances and addressing

- The description should say whether the message is streamed (typical rate), sent on change, or sent on request, and who sends it to whom. Missing is `should-fix`.
- Multi-instance devices need an instance/id field with the `instance="true"` attribute, and its range and base (0- or 1-based) stated consistently with similar messages.
- Messages sent to a specific system/component (commands, requests, settings) carry `target_system`/`target_component`; broadcast telemetry doesn't.
- Large messages that could be sent at high rate on low-bandwidth links (telemetry radios) are a concern even when under 255 bytes — mention it with the numbers.

## Other contexts

Ask what else the message will plausibly be used for beyond the author's use case — a different vehicle class, a larger or longer-range system, a companion computer or a ground-side peripheral, logging, a bridge from DroneCAN — and check the design holds there (ranges, instance counts, units, optional fields). `definition-quality.md` applies this to individual types; here it's about whether the message's *scope* is right: too narrow (specific to one product) or too broad (a grab-bag that no implementer will fill).

## Commands

- Prefer a command (`MAV_CMD`) for actions and a message for data. A command that returns data should use `MAV_CMD_REQUEST_MESSAGE` for an existing or new message rather than smuggling data into `COMMAND_ACK`.
- Check whether the command fits in missions, as a command, or both, and that the usage attributes say so.
- Check the command's progress/result semantics (does it need `MAV_RESULT_IN_PROGRESS`? is it idempotent? what does a repeated send do?) are stated.

## What this lens doesn't cover

Individual field types, wording, strings, reserved params — `definition-quality.md`.
Placement and IDs — `placement-lifecycle.md`.

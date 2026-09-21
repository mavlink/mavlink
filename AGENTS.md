# AGENTS.md

This repo defines the MAVLink message/dialect schema (XML) and reference generator tooling. It is a foundational spec repo: changes here ripple out to every downstream flight stack and ground control station that generates code from these XML files.

## Repo layout
- `message_definitions/v1.0/` — the XML dialects. Include chain: `minimal.xml` → `standard.xml` → `common.xml` → `development.xml`. `development.xml` is a staging area for messages/commands proposed for eventual promotion into `common.xml` — do not treat it as stable.
- Vendor/flight-stack dialects (`ardupilotmega.xml`, `storm32.xml`, `cubepilot.xml`, `uAvionix.xml`, etc.) are owned by their respective projects, not by this repo — be conservative about suggesting edits to them.
- `all.xml` aggregates every dialect solely to check for message/command ID clashes across the ecosystem; it documents each dialect's reserved ID ranges in XML comments.
- `pymavlink/` — Python MAVLink implementation and code generator (`pymavlink/generator/mavgen.py`).
- `scripts/` — XML validation tooling (see below).
- `doc/` — documentation generation (`mavlink_xml_to_markdown.py` produces markdown injected into the separate `mavlink-devguide` repo — https://github.com/mavlink/mavlink-devguide — which is the actual source for the mavlink.io site).

## Before proposing an XML change
Run the same checks CI runs:
- `scripts/format_xml.sh` — XML formatting (xmllint)
- `python3 scripts/xml_consistency_check.py` — structural consistency (e.g. bitmask enums must carry the `bitmask` attribute)
- `python3 scripts/check_api_break.py` — flags breaking changes (renamed/retyped/reordered fields, changed enum values or message/command IDs). What's generally allowed: adding new fields as message extensions, and editing descriptions as long as the semantic meaning doesn't change. Renaming enums/enum values is done only in rare cases, and message removal is rare — anything that changes the wire-compatible meaning of an existing field is not allowed.
- `scripts/test.sh` wraps the above and mirrors what CI does.

## Finding where a definition is used downstream
This repo only defines the schema — actual usage lives in the consuming projects. When asked "where is this message/enum/field/command used" or "what would break if I change this," check upstream usage in at least these projects (clone or search each; do not assume based on this repo alone):

- PX4-Autopilot — https://github.com/PX4/PX4-Autopilot
- ArduPilot — https://github.com/ArduPilot/ardupilot
- QGroundControl — https://github.com/mavlink/qgroundcontrol
- MAVProxy — https://github.com/ArduPilot/MAVProxy
- MissionPlanner — https://github.com/ArduPilot/MissionPlanner

These are the major flight-stack and ground-control consumers of MAVLink; a definition can be relied upon by any of them even if it looks unused within this repo.

## Conventions
- A `<wip/>` tag on a message/command/enum marks it as work-in-progress/unstable — do not treat WIP entries as part of the stable public API, and don't remove the tag without confirming the item is finalized.
- New message/command ID ranges are reserved per-dialect (see comments in `all.xml`); don't invent IDs outside an owned range without checking for clashes.
- This repo's own contribution conventions are thin (`CONTRIBUTING.md` just points to the external MAVLink Developer Guide, sourced from https://github.com/mavlink/mavlink-devguide and published at mavlink.io); when in doubt about process, defer to that guide rather than assuming.

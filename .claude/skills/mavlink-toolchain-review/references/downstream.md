# Lens: downstream

Two questions:

1. **Does it break anyone?** Consumers that build, generate, or publish from this repo must keep working.
2. **Can anyone use it?** A new generator feature, XSD attribute, or check is only useful if consumers can pick it up — and if they need to change something to do so, the PR should say what.

## Who consumes the toolchain

| Consumer | How it uses this repo | What to check |
| --- | --- | --- |
| PX4-Autopilot (`~/github/px4/PX4-Autopilot`) | Submodule `src/modules/mavlink/mavlink` of this repo; runs this repo's `pymavlink/tools/mavgen.py --lang C --wire-protocol 2.0` on its dialect and `uAvionix.xml` at build time (`src/modules/mavlink/CMakeLists.txt`) | Generate exactly as PX4 does, on base and head; generated C API unchanged for what PX4 calls; no new Python deps needed at build |
| ArduPilot (`~/github/ardupilot/ardupilot`) | Own fork (`modules/mavlink` → ArduPilot/mavlink) with ArduPilot's pymavlink; imports `pymavlink.generator.mavgen` from waf with `validate=False` (`Tools/ardupilotwaf/mavgen.py`) | Changes to `mavgen()`'s Python API or options object; generated C API. ArduPilot picks up changes only when it syncs its fork — say what it will hit then |
| c_library_v2 / c_library_v1 | Generated and pushed by `scripts/update_c_library.sh` from the deploy job on master | Generate as the script does (without pushing) and diff; any change to headers or file layout reaches every C/C++ user (QGC, MAVSDK, companion software) |
| mavlink-devguide / mavlink.io | `docs_build_and_deploy.yml` runs `doc/mavlink_xml_to_markdown.py` and pushes the output into the devguide repo | Output file names/paths unchanged (the devguide links to them); any new markup renders in the devguide's VitePress setup (check `~/github/mavlink/mavlink-devguide` for how the generated `en/messages/*.md` are included) |
| pymavlink users (MAVProxy, MissionPlanner's generator, scripts) | pip `pymavlink` (released from ArduPilot/pymavlink), dialect modules generated from these XML files | Generated Python API: message class names, attribute names, `ordered_fieldnames`, enum constants; C# output for MissionPlanner |
| Third-party XML consumers | Other-language generators (e.g. rust-mavlink, mavlink-go, MAVSDK's tooling) parse `message_definitions/` directly | New XML attributes or elements the XSD now allows: will strict parsers reject them? Should the XML start using them only after a notice period? |
| Contributors | Run `scripts/test.sh`, `check_api_break.py`, `format_xml.sh` locally, per `AGENTS.md` | CLI flags, defaults, required dependencies, and output format unchanged or documented |

Local clones may be stale — note the commit searched and fetch or use `gh search code` if old.

## What to check

- **Generated code API** (from `regression.md`'s diff): for each changed name or signature, search PX4, ArduPilot, QGC, MAVProxy, MissionPlanner for uses; any hit is a `bug` unless the PR is a coordinated break with a stated plan.
- **Generator entry points**: `mavgen.py` CLI flags, `mavgen.mavgen(opts, files)` and the options it reads, `mavgen.Opts` — anything removed or with changed defaults breaks PX4's CMake or ArduPilot's waf.
- **XSD and validation ordering**: if a pymavlink bump adds or tightens XSD rules, and XML in this repo starts relying on them, a consumer with an older pymavlink fails to generate the new XML. Establish which pymavlink PX4 uses (this repo's submodule — fine) and which ArduPilot uses (its own, with validation off — likely fine, but new *elements* can still break parsing). State the required landing order: generator first, XML later.
- **Python and dependency requirements**: new minimum Python version or new package needed at generation time is a downstream build break for PX4 and ArduPilot developers and CI. Check their documented minimum Python.
- **Wire-relevant output** changes (`CRC_EXTRA`, lengths, IDs) from `regression.md`: these break interop between vehicles built before and after, not just builds — always `bug` unless it's a deliberate protocol fix with a migration plan.
- **Usable downstream**: for a new feature (XSD attribute, generator option, check), is there a documented way for consumers to enable it, and does anything downstream need a change to benefit? If the feature targets a specific consumer (e.g. doc badges for mavlink.io, usage attributes for QGC's plan view), confirm that consumer's side exists or is planned; if not, note it (`minor`), since the feature will sit unused.

When you can, demonstrate rather than infer: build PX4's mavlink generation step (the `mavgen.py` command only, not the whole firmware) against the head worktree; run ArduPilot's `Tools/ardupilotwaf/mavgen.py` logic in Python with the head pymavlink; generate the devguide output and check it into a scratch copy of the devguide.

## Return

Findings, plus a short "what consumers need to do" note (per consumer, one line, or "nothing") and any required landing order.

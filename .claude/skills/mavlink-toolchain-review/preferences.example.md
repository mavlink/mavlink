# Reviewer preferences

Copy this file to `~/.config/mavlink-toolchain-review/preferences.md` and edit it there.
The skill loads only that exact path, and skips it silently if absent, so an empty or missing file gives the defaults.
It lives outside the repo so your preferences don't reach anyone else.

Preferences can set output shape, where the report goes, emphasis, suppressed severities, orientation questions and tone.
They cannot override the sources of truth, the read-only and sandboxing rules, or the requirement that every finding carry evidence; a preference that tries is reported as a conflict and not applied.

## Output

<!-- e.g. Always include the full fixture XML, even when it's long. -->

## Emphasis

<!-- e.g. For pymavlink bumps, always build PX4's mavlink generation step against head. -->

## Suppression

<!-- e.g. Don't report `minor` findings. -->

## Orient before reviewing

<!-- e.g. Is there a matching ArduPilot/pymavlink PR, and has it merged? -->

## Tone

<!-- e.g. Collegial, direct, no hedging. -->

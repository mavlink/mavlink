# Reviewer preferences

Copy this file to `~/.config/mavlink-xml-review/preferences.md` and edit it there.
The skill loads only that exact path, and skips it silently if absent, so an empty or missing file gives the defaults.
It lives outside the repo so your preferences don't reach anyone else.

Preferences can set output shape, where the report goes, emphasis, suppressed severities, orientation questions and tone.
They cannot override the sources of truth, the read-only rule, or the requirement that every finding carry evidence; a preference that tries is reported as a conflict and not applied.

## Output

<!-- e.g. Skip the per-definition summary when only one definition changed. -->

## Emphasis

<!-- e.g. Always produce the DroneCAN mapping table for new peripheral messages, even when it matches. -->

## Suppression

<!-- e.g. Don't report `minor` findings. -->

## Orient before reviewing

<!-- e.g. Which flight stack is the PR author from, and have they linked an implementation PR? -->

## Tone

<!-- e.g. Collegial, direct, no hedging. -->

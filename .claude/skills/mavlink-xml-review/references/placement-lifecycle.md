# Lens: placement and lifecycle

Is each definition in the right file, with the right ID, carrying the right lifecycle markers?

## Dialect choice

- **common.xml** is for definitions that are stable and implemented (or committed to) by more than one system. New definitions normally start in `development.xml`; a new definition added straight to common.xml needs a reason in the PR description (e.g. a trivial enum entry, or a definition already proven in a vendor dialect).
- **development.xml** is for proposals under test. Entries must carry `<wip since="YYYY-MM"/>` where the schema allows it, per `AGENTS.md` (some older entries have a bare `<wip/>`; new ones need the date).
- **Vendor dialects** are owned by their projects. An edit from outside the owning project is `should-fix` with the suggestion to upstream it there; a generic feature proposed in a vendor dialect should be proposed to development.xml instead.
- **minimal.xml / standard.xml** are for the heartbeat/identity core; additions here need strong justification.

## IDs

- Message IDs and `MAV_CMD` values must fall in the range reserved for their dialect in the comments of `all.xml`. Quote the range.
- Check clashes across all dialects: run mavgen (or the consistency check) on `all.xml` in the scratch worktree; a clash is `bug`.
- A new dialect, or a dialect taking a new range, must update `all.xml` in the same PR.
- IDs below 256 are scarce (MAVLink 1 compatible) — a new message there needs a reason.

## Lifecycle markers

- **`<wip since="YYYY-MM"/>`**: required on new development.xml entries; removed only when a definition is promoted or declared stable. Removing it is a promotion and gets the promotion checks below.
- **`<deprecated since="YYYY-MM" replaced_by="…"/>`** and **`<superseded …/>`**: both attributes present; `replaced_by` names something that exists; the description explains what to use instead if the replacement isn't one-to-one. Deprecated means "don't use"; superseded means "still usable, a better option exists" — check the PR uses the one it describes.
- **Removal** is rare: the definition must have been deprecated for a reasonable period, and `downstream.md` must show nothing still uses it. Otherwise `bug`.
- **Dates** use the format already used in the file (check neighbours); an obviously stale or future date is `should-fix`.

## Promotion from development.xml to common.xml

- The entry is removed from development.xml in the same PR (not duplicated).
- `<wip/>` is gone.
- There is evidence of at least one real implementation — ideally a flight stack and a ground station — cited in the PR or found by `downstream.md`. No evidence is `should-fix`.
- Definition identical to the development version unless the PR says otherwise (`compatibility.md` diffs it).
- Any related enums move with it, or already exist in common.xml.

## What this lens doesn't cover

Design of the definition itself — that's `definition-quality.md` and `message-design.md`.

# Lens: CI and security

## Is it actually run?

- Every new test file is invoked by a workflow. Tests here are run file by file (`python -m unittest scripts/test_xml_consistency_check.py -v` in `test_and_deploy.yml`; `scripts/test_check_api_break.py` in `check_api_break.yml`), so a new `scripts/test_*.py` isn't picked up automatically. Not wired in: `should-fix`.
- A new script or check is invoked by CI (and by `scripts/test.sh` if it's something contributors should run locally, per `AGENTS.md`) — or the PR says why not.
- The step fails the job on failure: exit code non-zero, no `|| true`, no `continue-on-error` unless justified.
- `ruff` (config in `ruff.toml`) and `codespell` pass on changed Python and text; run both on head.
- Code runs on every Python version in the `test_and_deploy.yml` matrix and on the version other workflows pin; check for syntax or stdlib use newer than the oldest (`match`, `X | Y` type unions at runtime, `tomllib`, etc.).
- Dependencies added to a script are installed in every workflow that runs it.

## Workflow security

Any change to `.github/workflows/*` gets these checks; quote the lines.

- **Untrusted code vs tokens.** `pull_request` jobs that check out and run PR code must keep `permissions: contents: read` (as `check_api_break.yml` does) and must not receive secrets. Anything that needs write access follows the `check_api_break.yml` → artifact → `post_api_break_comment.yml` (`workflow_run`) pattern, and the privileged side never checks out or executes PR content and never trusts PR-controlled data (PR number, branch name, file contents) beyond rendering it as text. Read the header comments of `post_api_break_comment.yml` for the full reasoning, and check any change there preserves it.
- **`pull_request_target` or `workflow_run` with a checkout of the PR head**: `bug` unless the checked-out code is never executed.
- **Script injection**: `${{ github.event.* }}` values (titles, branch names, bodies) interpolated directly into `run:` scripts — pass through `env:` instead.
- **Permissions**: any broadening of `permissions:` must be justified; job-level narrower than workflow-level where possible.
- **Actions**: new third-party actions pinned (version tag consistent with the rest of the repo, or SHA), from a reputable publisher; dependabot bumps of actions checked against the action's release notes for behaviour changes (e.g. `upload-artifact`/`download-artifact` major versions change artifact semantics).
- **Deploy jobs** (`docs_build_and_deploy.yml`, the C-library deploy in `test_and_deploy.yml`, `update_generated_repos.sh`): conditions still restrict them to pushes to master / merged PRs in this repo, not forks.

## Return

Findings; for each workflow finding, the triggering event(s) and the concrete path by which it could be exploited or fail.

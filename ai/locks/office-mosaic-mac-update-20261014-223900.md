AI LOCK
owner: codex
branch: codex/office-mosaic-mac-update-20261014-223900
base: 96e5d695329152a6cffccf4f90e4e348e916b0b9
expires: 2026-10-15 23:59 Europe/Madrid
files:
- app-office-cloud/build_mac_update.py (new)
reason: Small reversible native macOS pkg for Mike's existing Office, same selected Mosaic branding as MERP PR2341. Full DMG upload is too slow. Preserve existing source installer and all binaries/documents/permissions. Mike will install it later.
coordination: clean new sparse worktree from fresh origin/master, no open overlapping PRs, PR3 already merged. Default branch master. No workflow changes.

status: released
closure: PKG2MB public in MERP, full public SHA verified; update/deep-signature/forced-rollback tested on disposable clone, personal app untouched. Source mirrored in MERP PR2341.

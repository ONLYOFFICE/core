AI LOCK
owner: codex
branch: codex/office-mosaic-logo-20261014-212904
base: e6eab1d5
expires: 2026-10-15 23:59 Europe/Madrid
files:
- app-office-cloud/prepare.py
- app-office-cloud/build_mac.py
- app-office-cloud/build_windows.py
- app-office-cloud/installer.iss
- branding/office-mosaic/**
- .github/workflows/office-cloud-windows.yml
reason: Public build/source mirror of Mike-selected Mosaic icon in MERP PR2341. New Windows9.4.0.3 build with same native WPS converter, cloud agent and installer identity. No credentials or permissions.
coordination: fresh clean worktree from origin/master (default branch in ONLYOFFICE fork); no open overlapping PRs. No converter sources changed. Main source of Office branding: MERP PR2341.

status: released
closure: Windows build38626401856 SUCCESS, installer public in MERP and SHA/HTTP ranges verified; cloud/icon CAS public. No source/converter changes. MERP PR2341 owns release docs/Graphify. Default branch here is master.

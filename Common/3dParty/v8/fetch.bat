SET SCRIPTPATH=%~dp0
CD /D %~dp0

if exist "depot_tools" (
	echo "depot_tools already fetched"
) else (
	call git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
)

SET PATH=%SCRIPTPATH%depot_tools;%PATH%
call gclient

call ./depot_tools/fetch v8
cd v8
call git checkout -b 6.0 -t branch-heads/6.0

call gclient sync

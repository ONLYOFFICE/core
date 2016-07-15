SET SCRIPTPATH=%~dp0
CD /D %~dp0

if exist "depot_tools" (
	echo "depot_tools already fetched"
) else (
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
)

setx path "%SCRIPTPATH%depot_tools;%SCRIPTPATH%depot_tools\python276_bin;%PATH%;"
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2013

call gclient runhooks

if exist "%SCRIPTPATH%v8" (
call gclient sync
) else (
call fetch v8
)

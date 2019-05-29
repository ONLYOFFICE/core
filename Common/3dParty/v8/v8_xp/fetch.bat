SET SCRIPTPATH=%~dp0
CD /D %~dp0

if exist "depot_tools" (
	echo "depot_tools already fetched"
) else (
	call git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
	call powershell -File .\fix-depot_tools.ps1
)

SET PATH=%SCRIPTPATH%depot_tools;%SCRIPTPATH%depot_tools\win_tools-2_7_13_chromium7_bin\python\bin;%PATH%
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2015

if not exist "%SCRIPTPATH%v8" (
call .\depot_tools\fetch v8
)

call depot_tools\gclient sync -r 4.10.253

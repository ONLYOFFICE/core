SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2015

if exist "depot_tools" (
	echo "depot_tools already fetched"
) else (
	call git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
	call powershell -File .\fix-depot_tools.ps1
)

SET PATH=%SCRIPTPATH%depot_tools;%PATH%

call gclient

call ./depot_tools/fetch v8
cd v8
call git checkout -b 6.0 -t branch-heads/6.0
cd ../

call gclient sync --no-history

if not "%BUILD_PLATFORM%"=="%BUILD_PLATFORM:xp=%" (
	call v8_xp\fetch.bat
	cd %~dp0
)

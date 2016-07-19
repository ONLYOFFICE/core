SET SCRIPTPATH=%~dp0
CD /D %~dp0

SETX PATH "%SCRIPTPATH%depot_tools;%SCRIPTPATH%depot_tools\python276_bin;%PATH%;"
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2013

echo "building x86... -------------------------------------------"
call python v8\build\gyp_v8 -Dcomponent=shared_library

cd "%SCRIPTPATH%v8\tools\gyp"
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Release"
cd "%SCRIPTPATH%"

if exist "win_32" (
	RMDIR "win_32" /S /Q
)
md win_32
XCOPY "v8\build\Release\lib\*" "win_32\"
XCOPY "v8\build\Release\icudt.dll" "win_32\icudt.dll"

echo "building x64... -------------------------------------------"

call python v8\build\gyp_v8 -Dtarget_arch=x64 -Dcomponent=shared_library

cd "%SCRIPTPATH%v8\tools\gyp"
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Release"
cd "%SCRIPTPATH%"

if exist "win_64" (
	RMDIR "win_64" /S /Q
)
md win_64
XCOPY "v8\build\Release\lib\*" "win_64\"
XCOPY "v8\build\Release\icudt.dll" "win_64\icudt.dll"
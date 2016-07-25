SET SCRIPTPATH=%~dp0
CD /D %~dp0

SETX PATH "%SCRIPTPATH%depot_tools;%SCRIPTPATH%depot_tools\python276_bin;%PATH%;"
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2013

echo "building x86... -------------------------------------------"

if exist "win_32" (
	RMDIR "win_32" /S /Q
)
md win_32
md win_32\debug
md win_32\release

call python v8\build\gyp_v8
call .\change_projects.bat

cd "%SCRIPTPATH%v8\tools\gyp"
if defined ProgramFiles(x86) (
	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Release"
	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Debug"
) else (
	call "C:\Program Files\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Release"
	call "C:\Program Files\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Debug"
)
cd "%SCRIPTPATH%"

XCOPY "v8\build\Release\lib\*" "win_32\release\"
XCOPY "v8\build\Release\icudt.dll" "win_32\release\"

XCOPY "v8\build\Debug\lib\*" "win_32\debug\"
XCOPY "v8\build\Debug\icudt.dll" "win_32\debug\"

echo "building x64... -------------------------------------------"

if exist "win_64" (
	RMDIR "win_64" /S /Q
)
md win_64
md win_64\debug
md win_64\release

call python v8\build\gyp_v8 -Dtarget_arch=x64
call .\change_projects.bat

cd "%SCRIPTPATH%v8\tools\gyp"
if defined ProgramFiles(x86) (
	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Release"
	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Debug"
) else (
	call "C:\Program Files\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Release"
	call "C:\Program Files\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" v8.sln /Rebuild "Debug"
)
cd "%SCRIPTPATH%"

XCOPY "v8\build\Release\lib\*" "win_64\release\"
XCOPY "v8\build\Release\icudt.dll" "win_64\release\"

XCOPY "v8\build\Debug\lib\*" "win_64\debug\"
XCOPY "v8\build\Debug\icudt.dll" "win_64\debug\"
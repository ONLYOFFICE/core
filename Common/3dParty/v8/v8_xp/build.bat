SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET PATH=%SCRIPTPATH%depot_tools;%SCRIPTPATH%depot_tools\win_tools-2_7_13_chromium7_bin\python\bin;%PATH%
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2015

SET VC64_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\IDE
SET VC32_PATH=%ProgramFiles%\Microsoft Visual Studio 14.0\Common7\IDE

setlocal enabledelayedexpansion
if defined ProgramFiles(x86) (
	SET PATH=!VC64_PATH!;!PATH!
) else (
	SET PATH=!VC32_PATH!;!PATH!
)

if not exist "win_32" (
  echo "building x86... -------------------------------------------"
  
  md win_32
  md win_32\debug
  md win_32\release

  call python v8\build\gyp_v8
  call .\change_projects.bat

  cd "%SCRIPTPATH%v8\tools\gyp"

  call devenv v8.sln /Rebuild "Release" || goto :error
  call devenv v8.sln /Rebuild "Debug" || goto :error

  cd "%SCRIPTPATH%"

  XCOPY /Y "v8\build\Release\lib\*" "win_32\release\"
  XCOPY /Y "v8\build\Release\icudt.dll" "win_32\release\"

  XCOPY /Y "v8\build\Debug\lib\*" "win_32\debug\"
  XCOPY /Y "v8\build\Debug\icudt.dll" "win_32\debug\"
)

if not exist "win_64" (
  echo "building x64... -------------------------------------------"

  md win_64
  md win_64\debug
  md win_64\release

  call python v8\build\gyp_v8 -Dtarget_arch=x64
  call .\change_projects.bat

  cd "%SCRIPTPATH%v8\tools\gyp"

  call devenv v8.sln /Rebuild "Release" || goto :error
  call devenv v8.sln /Rebuild "Debug" || goto :error

  cd "%SCRIPTPATH%"

  XCOPY /Y "v8\build\Release\lib\*" "win_64\release\"
  XCOPY /Y "v8\build\Release\icudt.dll" "win_64\release\"

  XCOPY /Y "v8\build\Debug\lib\*" "win_64\debug\"
  XCOPY /Y "v8\build\Debug\icudt.dll" "win_64\debug\"
)

exit /b 0

:error
echo "Failed with error #%errorlevel%."
exit /b %errorlevel%
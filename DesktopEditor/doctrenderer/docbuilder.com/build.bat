SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET VC64_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\Common7\IDE
SET VC32_PATH=%ProgramFiles%\Microsoft Visual Studio 14.0\Common7\IDE

setlocal enabledelayedexpansion
if defined ProgramFiles(x86) (
	SET PATH=!VC64_PATH!;!PATH!
) else (
	SET PATH=!VC32_PATH!;!PATH!
)

call devenv docbuilder.com.sln /Rebuild "Release|x64"
call devenv docbuilder.com.sln /Rebuild "Release|Win32"

exit /b 0
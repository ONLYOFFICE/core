SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET ICU_MAJOR_VER=58
SET ICU_MINOR_VER=2

SET build_platform=win_32
if defined ProgramFiles(x86) (
	SET build_platform=win_64
)

if defined TARGET (
	SET build_platform=%TARGET%
)

if "%build_platform%" == "win_32" (
  SET MACHINE=x86
)

if "%build_platform%" == "win_64" (
  SET MACHINE=x64
)

if not exist "%build_platform%" (
	md "%build_platform%"
)

if not exist "%build_platform%\build" (
	md "%build_platform%\build"
)

cd "%SCRIPTPATH%%build_platform%"

if exist "%SCRIPTPATH%%build_platform%\icu\" (
	echo "icu already exported"
) else (
	svn export http://source.icu-project.org/repos/icu/tags/release-%ICU_MAJOR_VER%-%ICU_MINOR_VER%/icu4c ./icu
)

SET VC=%ProgramFiles%\Microsoft Visual Studio 14.0\VC
SET VC64=%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC
if exist %VC64% (
	SET VC=%VC64%
)

call "%VC%\vcvarsall.bat" %MACHINE%

if "%build_platform%" == "win_64" (
MSBuild.exe icu\source\allinone\allinone.sln /p:Configuration=Release /p:PlatformToolset=v140 /p:Platform="X64"
) else (
MSBuild.exe icu\source\allinone\allinone.sln /p:Configuration=Release /p:PlatformToolset=v140 /p:Platform="Win32"
)

if "%build_platform%" == "win_64" (
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\bin64\icudt%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%build_platform%\build\"
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\bin64\icuuc%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%build_platform%\build\"
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\lib64\icudt.lib" "%SCRIPTPATH%%build_platform%\build\"
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\lib64\icuuc.lib" "%SCRIPTPATH%%build_platform%\build\"
) else (
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\bin\icudt%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%build_platform%\build\"
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\bin\icuuc%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%build_platform%\build\"
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\lib\icudt.lib" "%SCRIPTPATH%%build_platform%\build\"
XCOPY /Y "%SCRIPTPATH%%build_platform%\icu\lib\icuuc.lib" "%SCRIPTPATH%%build_platform%\build\"
)

SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET ICU_MAJOR_VER=60
SET ICU_MINOR_VER=2

SET platform=win_32
if defined ProgramFiles(x86) (
	SET platform=win_64
)

if defined TARGET (
	SET platform=%TARGET%
)

if "%TARGET%" == "win_32" (
  SET MACHINE=x86
)

if "%TARGET%" == "win_64" (
  SET MACHINE=x64
)

if not exist "%platform%" (
	md "%platform%"
)

if not exist "%platform%\build" (
	md "%platform%\build"
)

cd "%SCRIPTPATH%%platform%"

if exist "%SCRIPTPATH%%platform%\icu\" (
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

if "%platform%" == "win_64" (
MSBuild.exe icu\source\allinone\allinone.sln /p:Configuration=Release /p:PlatformToolset=v140 /p:Platform="X64"
) else (
MSBuild.exe icu\source\allinone\allinone.sln /p:Configuration=Release /p:PlatformToolset=v140 /p:Platform="Win32"
)

if "%platform%" == "win_64" (
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin64\icudt%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin64\icuuc%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib64\icudt.lib" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib64\icuuc.lib" "%SCRIPTPATH%%platform%\build\"
) else (
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin\icudt%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin\icuuc%ICU_MAJOR_VER%.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib\icudt.lib" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib\icuuc.lib" "%SCRIPTPATH%%platform%\build\"
)

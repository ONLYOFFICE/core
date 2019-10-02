@echo off

SET SCRIPTPATH=%~dp0
CD /D %~dp0

setlocal

SET ICU_MAJOR_VER=58
SET ICU_MINOR_VER=2

SET VC=%ProgramFiles%\Microsoft Visual Studio 14.0\VC
SET VC64=%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC
if exist %VC64% (
	SET VC=%VC64%
)

SET UNSIP_PROGRAMM="%ProgramFiles%\7-Zip\7z.exe"
SET UNSIP_PROGRAMM2="%ProgramFiles(x86)%\7-Zip\7z.exe"
if exist %UNSIP_PROGRAMM2% (
	SET UNSIP_PROGRAMM=%UNSIP_PROGRAMM2%
)

if defined BUILD_PLATFORM (
	if not "%BUILD_PLATFORM%"=="%BUILD_PLATFORM:all=%" (
		SET "BUILD_PLATFORMS=win_64 win_32"
		GOTO :found
	)
	if not "%BUILD_PLATFORM%"=="%BUILD_PLATFORM:xp=%" (
		SET "BUILD_PLATFORMS=win_64 win_32"
		GOTO :found
	)
)

SET "BUILD_PLATFORMS=win_32"
if defined ProgramFiles(x86) (
	SET "BUILD_PLATFORMS=win_64"
	GOTO :found
)
if defined TARGET (
	SET "BUILD_PLATFORMS=%TARGET%"
	GOTO :found
)

echo "error"
GOTO :end

:found
for %%a in (%BUILD_PLATFORMS%) do (
	cd "%SCRIPTPATH%"	
	if not exist "%%a" (
		md "%%a"
	)
	if not exist "%%a\build" (
		md "%%a\build"
	)
	cd "%SCRIPTPATH%%%a"
	if exist "icu" (
		echo "icu already exported"
	) else (
		svn export https://github.com/unicode-org/icu/tags/release-%ICU_MAJOR_VER%-%ICU_MINOR_VER%/icu4c ./icu
	)
	
	if "%%a" == "win_64" (
		call "%VC%\vcvarsall.bat" x64
		MSBuild.exe icu\source\allinone\allinone.sln /p:Configuration=Release /p:PlatformToolset=v140 /p:Platform="X64"
		XCOPY /Y "icu\bin64\icudt%ICU_MAJOR_VER%.dll" "build\"
		XCOPY /Y "icu\bin64\icuuc%ICU_MAJOR_VER%.dll" "build\"
		XCOPY /Y "icu\lib64\icudt.lib" "build\"
		XCOPY /Y "icu\lib64\icuuc.lib" "build\"
	) else (
		call "%VC%\vcvarsall.bat" x86
		MSBuild.exe icu\source\allinone\allinone.sln /p:Configuration=Release /p:PlatformToolset=v140 /p:Platform="Win32"
		XCOPY /Y "icu\bin\icudt%ICU_MAJOR_VER%.dll" "build\"
		XCOPY /Y "icu\bin\icuuc%ICU_MAJOR_VER%.dll" "build\"
		XCOPY /Y "icu\lib\icudt.lib" "build\"
		XCOPY /Y "icu\lib\icuuc.lib" "build\"
	)
)
cd "%SCRIPTPATH%"

:end
endlocal

@echo on
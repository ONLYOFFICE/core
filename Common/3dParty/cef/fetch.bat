@echo off

SET SCRIPTPATH=%~dp0
CD /D %~dp0

setlocal

SET UNSIP_PROGRAMM="%ProgramFiles%\7-Zip\7z.exe"
SET UNSIP_PROGRAMM2="%ProgramFiles(x86)%\7-Zip\7z.exe"
if exist %UNSIP_PROGRAMM2% (
	SET UNSIP_PROGRAMM=%UNSIP_PROGRAMM2%
)

if defined BUILD_PLATFORM (
	if not "%BUILD_PLATFORM%"=="%BUILD_PLATFORM:xp=%" (
		SET "BUILD_PLATFORMS=win_64 win_32 winxp_64 winxp_32"
		GOTO :found
	)
	if not "%BUILD_PLATFORM%"=="%BUILD_PLATFORM:all=%" (
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
	echo "platform: %%a"
	mkdir "%SCRIPTPATH%%%a"
	cd "%SCRIPTPATH%%%a"
	Powershell.exe -executionpolicy remotesigned -file %SCRIPTPATH%download.ps1 http://d2ettrnqo7v976.cloudfront.net/cef/3163/%%a/cef_binary.7z cef_binary.7z cef_binary

	if exist "cef_binary" (
		echo "cef_binary.7z already extracted"
	) else (
		call %UNSIP_PROGRAMM% x "cef_binary.7z"
		mkdir build 
		xcopy /Y /S cef_binary\Release\*  build\ 
		xcopy /Y /S cef_binary\Resources\* build\
	)
)

:end
endlocal

@echo on
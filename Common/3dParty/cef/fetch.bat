SET SCRIPTPATH=%~dp0
CD /D %~dp0
SET PLATFORM=win_32
if defined ProgramFiles(x86) (
	SET PLATFORM=win_64
)

if defined TARGET (
	SET PLATFORM=%TARGET%
)

mkdir "%SCRIPTPATH%%PLATFORM%"
cd "%SCRIPTPATH%%PLATFORM%"

if exist "cef_binary.7z" (
    echo "cef_binary.7z already downloaded"
) else (
    Powershell.exe Invoke-WebRequest -OutFile cef_binary.7z http://d2ettrnqo7v976.cloudfront.net/cef/3163/%PLATFORM%/cef_binary.7z
)

SET UNSIP_PROGRAMM="%ProgramFiles%\7-Zip\7z.exe"
SET UNSIP_PROGRAMM2="%ProgramFiles(x86)%\7-Zip\7z.exe"
if exist %UNSIP_PROGRAMM2% (
	SET UNSIP_PROGRAMM=%UNSIP_PROGRAMM2%
)

if exist "cef_binary" (
	echo "cef_binary.7z already extracted"
) else (
    call %UNSIP_PROGRAMM% x "cef_binary.7z"
    mkdir build 
    xcopy /Y /S cef_binary\Release\*  build\ 
    xcopy /Y /S cef_binary\Resources\* build\
)

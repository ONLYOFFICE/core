SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET platform=win_32
if defined ProgramFiles(x86) (
	SET platform=win_64
)

if defined TARGET (
	SET platform=%TARGET%
)

if not exist "%platform%" (
	md "%platform%"
)

if not exist "%platform%\build" (
	md "%platform%\build"
)

if "%platform%" == "win_64" (
	SET URL=http://download.icu-project.org/files/icu4c/55.1/icu4c-55_1-Win64-msvc10.zip
) else (
	SET URL=http://download.icu-project.org/files/icu4c/55.1/icu4c-55_1-Win32-msvc10.zip
)

if exist "%SCRIPTPATH%%platform%\icu.zip" (
    echo "icu already downloaded"
) else (
    Powershell.exe Invoke-WebRequest -OutFile %platform%\icu.zip -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::FireFox "%URL%"
)

SET UNSIP_PROGRAMM="C:\Program Files\7-Zip\7z.exe"
SET UNSIP_PROGRAMM2="C:\Program Files (x86)\7-Zip\7z.exe"
if exist %UNSIP_PROGRAMM2% (
	SET UNSIP_PROGRAMM=%UNSIP_PROGRAMM2%
)

if exist "%SCRIPTPATH%%platform%\icu\" (
	echo "icu already extracted"
) else (
    call %UNSIP_PROGRAMM% x "%SCRIPTPATH%%platform%\icu.zip" -o"%SCRIPTPATH%%platform%\"
)

if "%platform%" == "win_64" (
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin64\icudt55.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin64\icuuc55.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib64\icudt.lib" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib64\icuuc.lib" "%SCRIPTPATH%%platform%\build\"
) else (
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin\icudt55.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\bin\icuuc55.dll" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib\icudt.lib" "%SCRIPTPATH%%platform%\build\"
XCOPY /Y "%SCRIPTPATH%%platform%\icu\lib\icuuc.lib" "%SCRIPTPATH%%platform%\build\"
)

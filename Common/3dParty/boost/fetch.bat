SET SCRIPTPATH=%~dp0
CD /D %~dp0

if exist "%SCRIPTPATH%boost_1_58_0.7z" (
    echo "boost already downloaded"
) else (
    Powershell.exe Invoke-WebRequest -OutFile boost_1_58_0.7z https://downloads.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.7z -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::FireFox
)

SET UNSIP_PROGRAMM="C:\Program Files\7-Zip\7z.exe"
SET UNSIP_PROGRAMM2="C:\Program Files (x86)\7-Zip\7z.exe"
if exist %UNSIP_PROGRAMM2% (
	SET UNSIP_PROGRAMM=%UNSIP_PROGRAMM2%
)

if exist "%SCRIPTPATH%boost_1_58_0/" (
	echo "boost already extracted"
) else (
    call %UNSIP_PROGRAMM% x "%SCRIPTPATH%/boost_1_58_0.7z" -o"%SCRIPTPATH%/"
)

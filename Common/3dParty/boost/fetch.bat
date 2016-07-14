SET SCRIPTPATH=%~dp0
CD /D %~dp0

if exist "%SCRIPTPATH%/boost_1_58_0.7z" (
    echo "boost already downloaded"
) else (
    Powershell.exe Invoke-WebRequest -OutFile boost_1_58_0.7z http://freefr.dl.sourceforge.net/project/boost/boost/1.58.0/boost_1_58_0.7z
)

if exist "%SCRIPTPATH%/boost_1_58_0/" (
	echo "boost already extracted"
) else (
    call "C:\Program Files\7-Zip\7z.exe" x "%SCRIPTPATH%/boost_1_58_0.7z" -o"%SCRIPTPATH%/"
)

pause
@ECHO OFF

REM The following directory is for .NET 2.0
set DOTNETFX2=%SystemRoot%\Microsoft.NET\Framework\v2.0.50727
set PATH=%PATH%;%DOTNETFX2%

echo Uninstalling TeamlabDocControlService...
echo ---------------------------------------------------
InstallUtil /u %~dp0\TeamlabDocControlService.exe
echo ---------------------------------------------------
echo Done
pause
SET SCRIPTPATH=%~dp0
CD /D %~dp0
SET MACHINE=x86
if defined ProgramFiles(x86) (
	SET MACHINE=x64
)

if "%TARGET%" == "win-32" (
  SET MACHINE=x86
)

if "%TARGET%" == "win-64" (
  SET MACHINE=x64
)

if "%MACHINE%" == "x86" (
  SET OUTPUT=win_32
)

if "%MACHINE%" == "x64" (
  SET OUTPUT=win_64
)

SET VC=%ProgramFiles%\Microsoft Visual Studio 12.0\VC
SET VC64=%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC
if exist %VC64% (
	SET VC=%VC64%
)

call "%VC%\vcvarsall.bat" %MACHINE%

cd curl
call buildconf.bat
cd winbuild
nmake /f Makefile.vc mode=static ENABLE_WINSSL=yes MACHINE=%MACHINE% VC=12

if not exist "%SCRIPTPATH%%OUTPUT%" (
   md %SCRIPTPATH%%OUTPUT%\build
   xcopy /Y /S ..\builds\libcurl-vc12-%MACHINE%-release-static-ipv6-sspi-winssl\lib\libcurl_a.lib %SCRIPTPATH%%OUTPUT%\build\
   mv %SCRIPTPATH%%OUTPUT%\build\libcurl_a.lib %SCRIPTPATH%%OUTPUT%\build\curl.lib
)

cd ..\..
SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET platform=win_32
if defined ProgramFiles(x86) (
	SET platform=win_64
)

SET outputdir=%SCRIPTPATH%boost_1_58_0\build\%platform%
echo "%outputdir%"

CD "%SCRIPTPATH%\boost_1_58_0"
call .\bootstrap.bat

SET folder=build/%platform%
if exist "%folder%" (
	RMDIR "%folder%" /S /Q
)

if exist "stage" (
	RMDIR "stage" /S /Q
)

md build
md  %folder%
md  %folder%\static
md  %folder%\static_fpic
md  %folder%\shared

.\b2.exe --clean
.\bjam.exe link=static --with-filesystem --with-system
XCOPY stage\lib\* "%folder%\static\"

.\b2.exe --clean
.\bjam.exe link=static cxxflags=-fPIC --with-filesystem --with-system
XCOPY stage\lib\* "%folder%\static_fpic\"

.\b2.exe --clean
.\bjam.exe link=shared --with-filesystem --with-system
XCOPY stage\lib\* "%folder%\shared\"


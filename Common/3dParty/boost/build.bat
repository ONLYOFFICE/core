SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET outputdir=%SCRIPTPATH%boost_1_58_0\build\%platform%
echo "%outputdir%"

CD "%SCRIPTPATH%\boost_1_58_0"
call .\bootstrap.bat

if exist "stage" (
	RMDIR "stage" /S /Q
)

md build

SET folder=build/win_32
echo "x86..."

if exist "%folder%" (
	RMDIR "%folder%" /S /Q
)
md  %folder%
md  %folder%\static
md  %folder%\static_fpic
md  %folder%\shared

.\b2.exe --clean
.\bjam.exe link=static --with-filesystem --with-system --with-date_time
XCOPY stage\lib\* "%folder%\static\"

.\b2.exe --clean
.\bjam.exe link=static cxxflags=-fPIC --with-filesystem --with-system --with-date_time
XCOPY stage\lib\* "%folder%\static_fpic\"

.\b2.exe --clean
.\bjam.exe link=shared --with-filesystem --with-system --with-date_time
XCOPY stage\lib\* "%folder%\shared\"

SET folder=build/win_64
echo "x64..."

if exist "%folder%" (
	RMDIR "%folder%" /S /Q
)

if exist "stage" (
	RMDIR "stage" /S /Q
)

md  %folder%
md  %folder%\static
md  %folder%\static_fpic
md  %folder%\shared

.\b2.exe --clean
.\bjam.exe link=static --with-filesystem --with-system --with-date_time address-model=64
XCOPY stage\lib\* "%folder%\static\"

.\b2.exe --clean
.\bjam.exe link=static cxxflags=-fPIC --with-filesystem --with-system --with-date_time address-model=64
XCOPY stage\lib\* "%folder%\static_fpic\"

.\b2.exe --clean
.\bjam.exe link=shared --with-filesystem --with-system --with-date_time address-model=64
XCOPY stage\lib\* "%folder%\shared\"
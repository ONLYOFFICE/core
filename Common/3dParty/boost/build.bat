SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET platform=win_32
if defined ProgramFiles(x86) (
	SET platform=win_64
)

outputdir="%SCRIPTPATH%/boost_1_58_0/build/%platform%"
echo "%outputdir%"

CD "%SCRIPTPATH%/boost_1_58_0"
call .\bootstrap.bat --with-libraries=filesystem,system

SET folder=build/%platform%
if exist "%folder%" (
	RMDIR "%folder%" /S /Q
)

mkdir build
mkdir  %folder%
mkdir  %folder%/static
mkdir  %folder%/static_fpic
mkdir  %folder%/shared

.\b2 --clean
.\bjam link=static
XCOPY stage\lib\* "%folder%\static\"

.\b2 --clean
.\bjam link=static cxxflags=-fPIC
XCOPY stage\lib\* "%folder%\static_fpic\"

.\b2 --clean
.\bjam link=shared
XCOPY stage\lib\* "%folder%\shared\"


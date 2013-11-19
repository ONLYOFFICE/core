@echo off 
@rem $Id: dumphint.bat 6300 2005-12-28 19:56:24Z giles $
@rem Linearized PDF hint formatting utility.

if %1/==/ goto usage
call gssetgs.bat
echo -q -dNODISPLAY -dSAFER -dDELAYSAFER >_.at
:cp
if %2/==/ goto doit
echo %2 >>_.at
shift
goto cp

:doit
%GSC% -q @_.at -- dumphint.ps %1
if exist _.at erase _.at
goto end

:usage
echo Usage: dumphint input.pdf

:end

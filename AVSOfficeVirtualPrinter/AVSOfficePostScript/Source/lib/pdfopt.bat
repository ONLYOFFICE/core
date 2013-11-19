@echo off 
@rem $Id: pdfopt.bat 6300 2005-12-28 19:56:24Z giles $
@rem Convert PDF to "optimized" form.

if %1/==/ goto usage
if %2/==/ goto usage
call gssetgs.bat
echo -q -dNODISPLAY -dSAFER -dDELAYSAFER >_.at
:cp
if %3/==/ goto doit
echo %1 >>_.at
shift
goto cp

:doit
%GSC% -q @_.at -- pdfopt.ps %1 %2
if exist _.at erase _.at
goto end

:usage
echo "Usage: pdfopt input.pdf output.pdf"

:end

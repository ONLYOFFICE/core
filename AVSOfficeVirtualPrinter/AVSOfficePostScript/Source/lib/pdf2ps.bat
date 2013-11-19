@echo off 
@rem $Id: pdf2ps.bat 6300 2005-12-28 19:56:24Z giles $
@rem Convert PDF to PostScript.

if %1/==/ goto usage
if %2/==/ goto usage
call gssetgs.bat
echo -dNOPAUSE -dBATCH -dSAFER -sDEVICE#pswrite >_.at
:cp
if %3/==/ goto doit
echo %1 >>_.at
shift
goto cp

:doit
rem Watcom C deletes = signs, so use # instead.
%GSC% -q -sOutputFile#%2 @_.at %1
if exist _.at erase _.at
goto end

:usage
echo "Usage: pdf2ps [-dASCII85DecodePages=false] [-dLanguageLevel=n] input.pdf output.ps"

:end

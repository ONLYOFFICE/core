@echo off 
@rem $Id: pfbtopfa.bat 6300 2005-12-28 19:56:24Z giles $
@rem Convert .pfb fonts to .pfa format

if %1/==/ goto usage
if %2/==/ goto usage
if not %3/==/ goto usage
call gssetgs.bat

%GSC% -q -dNODISPLAY -- pfbtopfa.ps %1 %2
goto end

:usage
echo "Usage: pfbtopfa input.pfb output.pfa"

:end

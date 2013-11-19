@echo off 
@rem $Id: pf2afm.bat 6300 2005-12-28 19:56:24Z giles $
@rem Make an AFM file from PFB / PFA and (optionally) PFM files.

if %1/==/ goto usage
if not %2/==/ goto usage
call gssetgs.bat

%GSC% -q -dNODISPLAY -dSAFER -dDELAYSAFER -- pf2afm.ps %1
goto end

:usage
echo "Usage: pf2afm disk_font_name"

:end



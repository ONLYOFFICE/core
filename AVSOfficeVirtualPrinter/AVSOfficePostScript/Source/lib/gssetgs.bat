@echo off
@rem $Id: gssetgs.bat 6300 2005-12-28 19:56:24Z giles $

rem Set default values for GS (gs with graphics window) and GSC
rem (console mode gs) if the user hasn't set them.

if %GS%/==/ set GS=gswin32
if %GSC%/==/ set GSC=gswin32c

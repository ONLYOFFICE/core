@echo off
@rem $Id: font2c.bat 6300 2005-12-28 19:56:24Z giles $

call gssetgs.bat
%GSC% -q -dNODISPLAY -dWRITESYSTEMDICT -- font2c.ps %1 %2 %3 %4 %5 %6 %7 %8 %9

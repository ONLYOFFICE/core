@echo off
@rem $Id: gsdj500.bat 6300 2005-12-28 19:56:24Z giles $

call gssetgs.bat
%GSC% -q -sDEVICE#djet500 -r300 -dNOPAUSE -sPROGNAME=gsdj500 -- gslp.ps %1 %2 %3 %4 %5 %6 %7 %8 %9

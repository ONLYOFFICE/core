@echo off
@rem $Id: lpr2.bat 6300 2005-12-28 19:56:24Z giles $

call gssetgs.bat
%GSC% -sDEVICE#djet500 -dNOPAUSE -sPROGNAME=lpr2 -- gslp.ps -2r %1 %2 %3 %4 %5 %6 %7 %8 %9

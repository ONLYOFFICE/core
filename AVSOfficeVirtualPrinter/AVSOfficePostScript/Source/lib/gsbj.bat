@echo off
@rem $Id: gsbj.bat 6300 2005-12-28 19:56:24Z giles $

call gssetgs.bat
%GSC% -q -sDEVICE=bj10e -r180 -dNOPAUSE -sPROGNAME=gsbj -- gslp.ps %1 %2 %3 %4 %5 %6 %7 %8 %9

@rem Execute this script with echo on, so we can see what's happening.
@rem $Id: wmakebat.bat 6300 2005-12-28 19:56:24Z giles $
wmakel -u -n -h %1 %2 %3 %4 %5 %6 %7 %8 %9 >_wm_temp.bat
_wm_temp.bat

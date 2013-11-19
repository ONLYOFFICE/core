@rem $Id: rm.bat 6300 2005-12-28 19:56:24Z giles $
@echo off
:next
if '%1'=='' goto exit
if '%1'=='-f' goto sh
if exist %1 erase %1
:sh
shift
goto next
:exit

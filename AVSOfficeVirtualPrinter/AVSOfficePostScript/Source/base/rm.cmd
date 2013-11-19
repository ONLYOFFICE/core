@echo off
@rem $Id: rm.cmd 6300 2005-12-28 19:56:24Z giles $
:next
if '%1'=='' goto exit
if '%1'=='-f' goto sh
erase %1
:sh
shift
goto next
:exit

@echo off
REM 
REM This is my personal configuration file. 
REM I am lazy to type all this crap again and again
REM You are welcome to customize this file for your
REM needs but do not check it into the GitHub, please.
REM
REM Aleksey Sanin <aleksey@aleksey.com>
REM 

SET XMLSEC_PREFIX=C:\cygwin\home\local
SET XMLSEC_INCLUDE=%XMLSEC_PREFIX%\include
SET XMLSEC_LIB=%XMLSEC_PREFIX%\lib

SET INCLUDE=%XMLSEC_INCLUDE%;%INCLUDE%
SET LIB=%XMLSEC_LIB%;%LIB%

nmake -f Makefile.w32 %1 %2 %3

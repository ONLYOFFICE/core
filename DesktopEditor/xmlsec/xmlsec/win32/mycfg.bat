@echo on
REM 
REM This is my personal configuration file. 
REM I am lazy to type all this crap again and again
REM You are welcome to customize this file for your
REM needs but do not check it into the GitHub, please.
REM
REM Aleksey Sanin <aleksey@aleksey.com>
REM 

SET PREFIX=C:\cygwin\home\local
SET XMLSEC_INCLUDE=%PREFIX%\include;%PREFIX%\include\mozilla;%PREFIX%\include\mozilla\nspr;%PREFIX%\include\mozilla\nss;%MSSDK_INCLUDE%
SET XMLSEC_LIB=%PREFIX%\lib;%MSSDK_LIB%
SET XMLSEC_OPTIONS=static=no iconv=no debug=yes xslt=yes crypto=mscrypto,openssl unicode=yes

del /F Makefile configure.txt
cscript configure.js prefix=%PREFIX% %XMLSEC_OPTIONS% include=%XMLSEC_INCLUDE% lib=%XMLSEC_LIB% 

mkdir binaries
copy %PREFIX%\bin\*.dll binaries
copy %PREFIX%\lib\*.dll binaries

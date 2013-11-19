@echo off

Call EncryptRes.exe oox2fo-debug.xslt
del oox2fo.xslt
ren oox2fo-debug.crpt oox2fo.xslt
@echo OFF
set RUN_DIR=%~dp0..
set GIT_DIR=%RUN_DIR%\..\..\..\..\..

RMDIR "%RUN_DIR%\sdkjs" /S /Q

rem SDKJS UPDATE
md %RUN_DIR%\sdkjs\cell
copy "%GIT_DIR%\sdkjs\cell\sdk-all.js"      "%RUN_DIR%\sdkjs\cell\sdk-all.js"
copy "%GIT_DIR%\sdkjs\cell\sdk-all-min.js"  "%RUN_DIR%\sdkjs\cell\sdk-all-min.js"

md %RUN_DIR%\sdkjs\slide\themes
copy "%GIT_DIR%\sdkjs\slide\sdk-all.js"     "%RUN_DIR%\sdkjs\slide\sdk-all.js" 
copy "%GIT_DIR%\sdkjs\slide\sdk-all-min.js" "%RUN_DIR%\sdkjs\slide\sdk-all-min.js"
xcopy /S /E "%GIT_DIR%\sdkjs\slide\themes"  "%RUN_DIR%\sdkjs\slide\themes\"

md %RUN_DIR%\sdkjs\word
copy "%GIT_DIR%\sdkjs\word\sdk-all.js"     "%RUN_DIR%\sdkjs\word\sdk-all.js" 
copy "%GIT_DIR%\sdkjs\word\sdk-all-min.js" "%RUN_DIR%\sdkjs\word\sdk-all-min.js"

md %RUN_DIR%\sdkjs\common\Native
md %RUN_DIR%\sdkjs\common\libfont\js
copy "%GIT_DIR%\sdkjs\common\Native\jquery_native.js" "%RUN_DIR%\sdkjs\common\Native\jquery_native.js" 
copy "%GIT_DIR%\sdkjs\common\Native\native.js"        "%RUN_DIR%\sdkjs\common\Native\native.js"
copy "%GIT_DIR%\sdkjs\common\libfont\js\fonts.js"     "%RUN_DIR%\sdkjs\common\libfont\js\fonts.js"

md %RUN_DIR%\sdkjs\vendor\jquery
copy "%GIT_DIR%\web-apps-pro\vendor\jquery\jquery.min.js"       "%RUN_DIR%\sdkjs\vendor\jquery\jquery.min.js"
md %RUN_DIR%\sdkjs\vendor\xregexp
copy "%GIT_DIR%\web-apps-pro\vendor\xregexp\xregexp-all-min.js" "%RUN_DIR%\sdkjs\vendor\xregexp\xregexp-all-min.js"

rem DLLs UPDATE
del /Q /F "%RUN_DIR%\*.dll"
del /Q /F "%RUN_DIR%\x2t.exe"
copy "%GIT_DIR%\server\FileConverter\bin\*.dll" "%RUN_DIR%\."
copy "%GIT_DIR%\server\FileConverter\bin\x2t.exe" "%RUN_DIR%\."
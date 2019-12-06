@echo OFF
set RUN_DIR=%~dp0..
set GIT_DIR=%RUN_DIR%\..\..\..\..\..

RMDIR "%RUN_DIR%\sdkjs" /S /Q

xcopy /s/e/k/c/y/q "%GIT_DIR%\sdkjs\deploy\sdkjs" "%RUN_DIR%\"
md %RUN_DIR%\sdkjs\vendor\jquery
copy "%GIT_DIR%\web-apps-pro\vendor\jquery\jquery.min.js"       "%RUN_DIR%\sdkjs\vendor\jquery\jquery.min.js"
md %RUN_DIR%\sdkjs\vendor\xregexp
copy "%GIT_DIR%\web-apps-pro\vendor\xregexp\xregexp-all-min.js" "%RUN_DIR%\sdkjs\vendor\xregexp\xregexp-all-min.js"

copy "%GIT_DIR%\server\FileConverter\bin\core\*" "%RUN_DIR%\."

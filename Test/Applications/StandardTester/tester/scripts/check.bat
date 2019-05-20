@echo OFF
rem 
rem --input="input-standard-files-dir"
rem --output="output-dir"
rem --standard // generate standarts
rem --use-system-fonts="0/1/false/true"
rem --font-dirs="C:\\Windows\\Fonts;/usr/share/fonts;"
rem --cores=4
rem

set RUN_DIR=%~dp0..
cd "%RUN_DIR%"
RMDIR "%RUN_DIR%\result\check" /S /Q
StandardTester.exe --input="%RUN_DIR%\result\out"  --output="%RUN_DIR%\result\check" --cores=1
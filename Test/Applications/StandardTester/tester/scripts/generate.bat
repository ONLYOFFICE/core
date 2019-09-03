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
StandardTester.exe --input="%RUN_DIR%\result\standard"  --output="%RUN_DIR%\result\out" --standard --cores=1

SET SCRIPTPATH=%~dp0
CD /D %~dp0

FOR /f "tokens=*" %%i in ('DIR /a:d /b *') DO (
    
	CD /D %~dp0
	ECHO %%i
	
	if exist %%i/fetch.bat (
		call %%i/fetch.bat
	)

	CD /D %~dp0
	ECHO %%i

	if exist %%i/build.bat (
		call %%i/build.bat || goto :error
	)
	
)

exit /b 0

:error
echo "Failed with error #%errorlevel%."
exit /b %errorlevel%
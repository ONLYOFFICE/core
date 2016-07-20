SET SCRIPTPATH=%~dp0
CD /D %~dp0

FOR /f "tokens=*" %%i in ('DIR /a:d /b *') DO (
    ECHO %%i
	
	if exist %%i/fetch.bat (
		call %%i/fetch.bat
	)
	
	if exist %%i/build.bat (
		call %%i/build.bat
	)
	
)
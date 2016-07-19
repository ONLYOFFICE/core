SET SCRIPTPATH=%~dp0
CD /D %~dp0

SETX PATH "%SCRIPTPATH%depot_tools;%SCRIPTPATH%depot_tools\python276_bin;%PATH%;"
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2013

echo "building x86... -------------------------------------------"
call python v8\build\gyp_v8

cd "%SCRIPTPATH%v8\tools\gyp"
call powershell -Command "(gc v8_base_0.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_0.vcxproj"
call powershell -Command "(gc v8_base_1.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_1.vcxproj"
call powershell -Command "(gc v8_base_2.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_2.vcxproj"
call powershell -Command "(gc v8_base_3.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_3.vcxproj"

call powershell -Command "(gc v8_libbase.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_libbase.vcxproj"
call powershell -Command "(gc v8_libplatform.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_libplatform.vcxproj"

call powershell -Command "(gc v8_nosnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_nosnapshot.vcxproj"
call powershell -Command "(gc mksnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File mksnapshot.vcxproj"

cd "%SCRIPTPATH%v8\third_party\icu"
call powershell -Command "(gc icui18n.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File icui18n.vcxproj"
call powershell -Command "(gc icuuc.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File icuuc.vcxproj"

cd "%SCRIPTPATH%"
"C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe" v8\tools\gyp\v8.sln /Rebuild "Release|Win32"

if exist "win_32" (
	RMDIR "win_32" /S /Q
)
md win_32
XCOPY "%v8\build\Release\lib\*" "win_32\"
XCOPY "%v8\build\Release\lib\icudt.dll" "win_32\icudt.dll"

echo "building x64... -------------------------------------------"

call python v8\build\gyp_v8 -Dtarget_arch=x64

cd "%SCRIPTPATH%v8\tools\gyp"
powershell -Command "(gc v8_base_0.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_0.vcxproj"
powershell -Command "(gc v8_base_1.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_1.vcxproj"
powershell -Command "(gc v8_base_2.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_2.vcxproj"
powershell -Command "(gc v8_base_3.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_base_3.vcxproj"

powershell -Command "(gc v8_libbase.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_libbase.vcxproj"
powershell -Command "(gc v8_libplatform.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_libplatform.vcxproj"

powershell -Command "(gc v8_nosnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File v8_nosnapshot.vcxproj"
powershell -Command "(gc mksnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File mksnapshot.vcxproj"

cd "%SCRIPTPATH%v8\third_party\icu"
powershell -Command "(gc icui18n.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File icui18n.vcxproj"
powershell -Command "(gc icuuc.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File icuuc.vcxproj"

cd "%SCRIPTPATH%"

"C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe" v8\tools\gyp\v8.sln /Rebuild "Release|x64"

if exist "win_64" (
	RMDIR "win_64" /S /Q
)
md win_64
XCOPY "%v8\build\Release\lib\*" "win_64\"
XCOPY "%v8\build\Release\lib\icudt.dll" "win_64\icudt.dll"
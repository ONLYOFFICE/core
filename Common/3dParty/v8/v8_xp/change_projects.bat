SET SCRIPTPATH=%~dp0
CD /D %~dp0

cd "%SCRIPTPATH%v8\tools\gyp"
call powershell -Command "(gc v8_base_0.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_0.vcxproj"
call powershell -Command "(gc v8_base_0.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_0.vcxproj"

call powershell -Command "(gc v8_base_1.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_1.vcxproj"
call powershell -Command "(gc v8_base_1.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_1.vcxproj"

call powershell -Command "(gc v8_base_2.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_2.vcxproj"
call powershell -Command "(gc v8_base_2.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_2.vcxproj"

call powershell -Command "(gc v8_base_3.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_3.vcxproj"
call powershell -Command "(gc v8_base_3.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_base_3.vcxproj"

call powershell -Command "(gc v8_libbase.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_libbase.vcxproj"
call powershell -Command "(gc v8_libbase.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_libbase.vcxproj"

call powershell -Command "(gc v8_libplatform.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_libplatform.vcxproj"
call powershell -Command "(gc v8_libplatform.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_libplatform.vcxproj"

call powershell -Command "(gc v8_nosnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_nosnapshot.vcxproj"
call powershell -Command "(gc v8_nosnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' v8_nosnapshot.vcxproj"

call powershell -Command "(gc mksnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' mksnapshot.vcxproj"
call powershell -Command "(gc mksnapshot.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' mksnapshot.vcxproj"

cd "%SCRIPTPATH%v8\third_party\icu"
call powershell -Command "(gc icui18n.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' icui18n.vcxproj"
call powershell -Command "(gc icui18n.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' icui18n.vcxproj"

call powershell -Command "(gc icuuc.vcxproj) -replace '<RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' icuuc.vcxproj"
call powershell -Command "(gc icuuc.vcxproj) -replace '<RuntimeLibrary>MultiThreaded</RuntimeLibrary>', '<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>' | Out-File -Encoding 'UTF8' icuuc.vcxproj"

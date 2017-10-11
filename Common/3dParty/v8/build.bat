SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET PATH=%SCRIPTPATH%depot_tools;%PATH%
SET DEPOT_TOOLSPWIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2015

cd v8
call gn gen out.gn/win_64 --args="is_debug=false target_cpu=\"x64\" v8_target_cpu=\"x64\" v8_static_library=true is_component_build=false v8_use_snapshot=false is_clang=false"

cd ../
call powershell -File .\fix-static_crt.ps1
cd v8

call ninja -C out.gn/win_64
SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET PATH=%SCRIPTPATH%depot_tools;%PATH%
SET DEPOT_TOOLS_WIN_TOOLCHAIN=0
SET GYP_MSVS_VERSION=2015

call powershell -File .\fix-static_crt.ps1
cd v8

call gn gen out.gn/win_64/release --args="is_debug=false target_cpu=\"x64\" v8_target_cpu=\"x64\" v8_static_library=true is_component_build=false v8_use_snapshot=false is_clang=false"
call ninja -C out.gn/win_64/release

call gn gen out.gn/win_64/debug --args="is_debug=true target_cpu=\"x64\" v8_target_cpu=\"x64\" v8_static_library=true is_component_build=false v8_use_snapshot=false is_clang=false"
call ninja -C out.gn/win_64/debug

call gn gen out.gn/win_32/release --args="is_debug=false target_cpu=\"x86\" v8_target_cpu=\"x86\" v8_static_library=true is_component_build=false v8_use_snapshot=false is_clang=false"
call ninja -C out.gn/win_32/release

call gn gen out.gn/win_32/debug --args="is_debug=true target_cpu=\"x86\" v8_target_cpu=\"x86\" v8_static_library=true is_component_build=false v8_use_snapshot=false is_clang=false"
call ninja -C out.gn/win_32/debug

rem v8_use_snapshot=true v8_use_external_startup_data=true
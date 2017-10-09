SET SCRIPTPATH=%~dp0
CD /D %~dp0

SET PATH=%SCRIPTPATH%depot_tools;%PATH%
set DEPOT_TOOLSPWIN_TOOLCHAIN=0

cd v8
call python tools/dev/v8gen.py x64.release
call gn gen out.gn/x64.release --args='is_debug=false target_cpu="x64" v8_target_cpu="x64" v8_static_library=true is_component_build=false v8_use_snapshot=false is_clang=false use_sysroot=false'
call ninja -C out.gn/x64.release
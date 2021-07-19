#!/bin/bash
SCRIPTPATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$SCRIPTPATH"

if [ ! -d "depot_tools" ]
then
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
fi

export PATH=`pwd`/depot_tools:"$PATH"

fix_build_gn()
{
    cp ./BUILD.gn ./BUILD.gn.old
    rm ./BUILD.gn
    # wrap action("run_mksnapshot") & v8_source_set("v8_snapshot")
    counter=0
    input="./BUILD.gn.old"
    while IFS= read -r line
    do
        echo "$line" >> ./BUILD.gn
        counter=$((counter+1))

        if [ "$counter" -eq "715" ];then
            echo "if (v8_use_snapshot) {" >> ./BUILD.gn
        fi

        if [ "$counter" -eq "765" ];then
            echo "}" >> ./BUILD.gn
        fi

        if [ "$counter" -eq "831" ];then
            echo "if (v8_use_snapshot) {" >> ./BUILD.gn
        fi

        if [ "$counter" -eq "861" ];then
            echo "}" >> ./BUILD.gn
        fi

    done < "$input"
}

restore_build_gn()
{
    rm ./BUILD.gn
    cp ./BUILD.gn.old ./BUILD.gn
}

if [ ! -d "v8" ]
then
    fetch v8
    cd v8
    git checkout -b 6.0 branch-heads/6.0

    echo "target_os= ['android']">>./../.gclient
    echo y | sudo ./build/install-build-deps-android.sh
    gclient sync --verbose

    cd ../
fi

cd v8

toolchains_dir=./../archive-tools
libs_dir=./../build

deploy_toolchain()
{
	#api_check=$1
	#api_version=16
	#arm_detect=${api_check:0:3}
	#if [ "$arm_detect" = "arm" ]; then
    #	api_version=21
    #fi
    api_version=21
    echo "toolshain: --arch=$1 --api=$api_version"
	#./third_party/android_tools/ndk/build/tools/make-standalone-toolchain.sh --install-dir=$toolchains_dir/$1 --arch=$1 --platform=android-${api_version} --stl=libc++
	./third_party/android_tools/ndk/build/tools/make_standalone_toolchain.py --install-dir=$toolchains_dir/$1 --arch=$1 --stl=libc++ --api=${api_version}
}

deploy_libs()
{
	mkdir $2
    $1 -rcsD $2/libv8_base.a $3/obj/v8_base/*.o $3/obj/v8_builtins_setup/*.o $3/obj/v8_builtins_generators/*.o
    $1 -rcsD $2/libv8_libbase.a $3/obj/v8_libbase/*.o
    $1 -rcsD $2/libv8_libsampler.a $3/obj/v8_libsampler/*.o
    $1 -rcsD $2/libv8_libplatform.a $3/obj/v8_libplatform/*.o
    $1 -rcsD $2/libv8_nosnapshot.a $3/obj/v8_nosnapshot/*.o
    $1 -rcsD $2/libicuuc.a $3/obj/third_party/icu/icuuc/*.o
    $1 -rcsD $2/libicui18n.a $3/obj/third_party/icu/icui18n/*.o
    cp $3/icudtl.dat $2/icudtl.dat
}

rm -rf $toolchains_dir
mkdir $toolchains_dir
rm -rf $libs_dir
mkdir $libs_dir

fix_build_gn

echo "BUILD: arm64 ----------------------------------------"
gn gen out.gn/android/arm64 "--args=is_component_build=false is_clang=false is_debug=false use_sysroot=true target_os=\"android\" target_cpu=\"arm64\" v8_target_cpu=\"arm64\" v8_enable_i18n_support=true v8_static_library=true v8_use_snapshot=false clang_use_chrome_plugins=false android_ndk_version=\"21.1.6352462\" android_sdk_version=\"21\""
ninja -C out.gn/android/arm64
echo "ARCHIVE: arm64 --------------------------------------"
deploy_toolchain arm64
deploy_libs $toolchains_dir/arm64/bin/aarch64-linux-android-ar $libs_dir/arm64-v8a ./out.gn/android/arm64

echo "BUILD: arm ------------------------------------------"
gn gen out.gn/android/arm "--args=is_component_build=false is_clang=false is_debug=false use_sysroot=true target_os=\"android\" target_cpu=\"arm\" v8_target_cpu=\"arm\" v8_enable_i18n_support=true v8_static_library=true v8_use_snapshot=false clang_use_chrome_plugins=false android_ndk_version=\"21.1.6352462\" android_sdk_version=\"21\""
ninja -C out.gn/android/arm
echo "ARCHIVE: arm ----------------------------------------"
deploy_toolchain arm
deploy_libs $toolchains_dir/arm/bin/arm-linux-androideabi-ar $libs_dir/armeabi-v7a ./out.gn/android/arm

echo "BUILD: x86_64 ---------------------------------------"
gn gen out.gn/android/x86_64 "--args=is_component_build=false is_clang=false is_debug=false use_sysroot=true target_os=\"android\" target_cpu=\"x64\" v8_target_cpu=\"x64\" v8_enable_i18n_support=true v8_static_library=true v8_use_snapshot=false clang_use_chrome_plugins=false android_ndk_version=\"21.1.6352462\" android_sdk_version=\"16\""
ninja -C out.gn/android/x86_64
echo "ARCHIVE: x86_64 -------------------------------------"
deploy_toolchain x86_64
deploy_libs $toolchains_dir/x86_64/bin/x86_64-linux-android-ar $libs_dir/x86_64 ./out.gn/android/x86_64

echo "BUILD: x86 ------------------------------------------"
gn gen out.gn/android/x86 "--args=is_component_build=false is_clang=false is_debug=false use_sysroot=true target_os=\"android\" target_cpu=\"x86\" v8_target_cpu=\"x86\" v8_enable_i18n_support=true v8_static_library=true v8_use_snapshot=false clang_use_chrome_plugins=false android_ndk_version=\"21.1.6352462\" android_sdk_version=\"16\""
ninja -C out.gn/android/x86
echo "ARCHIVE: x86 ----------------------------------------"
deploy_toolchain x86
deploy_libs $toolchains_dir/x86/bin/i686-linux-android-ar $libs_dir/x86 ./out.gn/android/x86

restore_build_gn

cd ../

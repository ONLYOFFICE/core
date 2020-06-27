#!/bin/bash

cd boost_1_72_0
OUTPUT_DIR="../build/android"

BOOST_LIBS="filesystem system date_time regex"

CPPSTD="-std=c++11 -frtti -fexceptions"

# Must set these after parseArgs to fill in overriden values
# Todo: -g -DNDEBUG are for debug builds only...
# Boost.test defines are needed to build correct instrumentable boost_unit_test_framework static lib
# it does not affect the functionality of <boost/test/included/unit_test.hpp> single-header usage.
# See http://www.boost.org/doc/libs/1_66_0/libs/test/doc/html/boost_test/adv_scenarios/static_lib_customizations/entry_point.html
EXTRA_FLAGS="-DBOOST_AC_USE_PTHREADS -DBOOST_SP_USE_PTHREADS \
    -DBOOST_TEST_NO_MAIN -DBOOST_TEST_ALTERNATIVE_INIT_API -DANDROID_STL=c++_static \
    -Wno-unused-local-typedef"
EXTRA_ANDROID_FLAGS="$EXTRA_FLAGS"

if [[ -n "$USE_CXX11_ABI" ]]; then
   EXTRA_LINUX_FLAGS="$EXTRA_FLAGS -D_GLIBCXX_USE_CXX11_ABI=$USE_CXX11_ABI"
else
   EXTRA_LINUX_FLAGS="$EXTRA_FLAGS"
fi

doneSection()
{
    echo
    echo "Done"
    echo "================================================================="
    echo
}

bootstrapBoost()
{
    BOOTSTRAP_LIBS=$BOOST_LIBS
    BOOST_LIBS_COMMA=$(echo $BOOTSTRAP_LIBS | sed -e "s/ /,/g")
    echo "Bootstrapping for $1 (with libs $BOOST_LIBS_COMMA)"
    ./bootstrap.sh --with-libraries=$BOOST_LIBS_COMMA

    doneSection
}

generateAndroidUserConfig()
{
    HOSTOS="$(uname | awk '{ print $1}' | tr [:upper:] [:lower:])-" # darwin or linux
    OSARCH="$(uname -m)"

    # Boost doesn't build with <compileflags>-Werror
    # Reported to boost-users@lists.boost.org

    cat > "./tools/build/src/user-config.jam" <<EOF
using clang : 5.0~x86
: $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/$HOSTOS$OSARCH/bin/clang++ $EXTRA_ANDROID_FLAGS
:
<architecture>x86 <target-os>android
<compileflags>--target=i686-none-linux-android
<compileflags>--gcc-toolchain=$ANDROID_NDK_ROOT/toolchains/x86-4.9/prebuilt/$HOSTOS$OSARCH
<compileflags>--sysroot=$ANDROID_NDK_ROOT/sysroot
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++abi/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/android/support/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include/i686-linux-android
<compileflags>-DANDROID
<compileflags>-D__ANDROID_API__=19
<compileflags>-ffunction-sections
<compileflags>-funwind-tables
<compileflags>-fstack-protector-strong
<compileflags>-fno-limit-debug-info
<compileflags>-fPIC
<compileflags>-no-canonical-prefixes
<compileflags>-mstackrealign
<compileflags>-Wa,--noexecstack
<compileflags>-Wformat
<compileflags>-Werror=format-security
<compileflags>-Wall
<compileflags>-Wshadow
;
using clang : 5.0~x86_64
: $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/$HOSTOS$OSARCH/bin/clang++ $EXTRA_ANDROID_FLAGS
:
<architecture>x86 <target-os>android
<compileflags>--target=x86_64-none-linux-android
<compileflags>--gcc-toolchain=$ANDROID_NDK_ROOT/toolchains/x86_64-4.9/prebuilt/$HOSTOS$OSARCH
<compileflags>--sysroot=$ANDROID_NDK_ROOT/sysroot
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++abi/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/android/support/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include/x86_64-linux-android
<compileflags>-DANDROID
<compileflags>-D__ANDROID_API__=21
<compileflags>-ffunction-sections
<compileflags>-funwind-tables
<compileflags>-fstack-protector-strong
<compileflags>-fno-limit-debug-info
<compileflags>-fPIC
<compileflags>-no-canonical-prefixes
<compileflags>-mstackrealign
<compileflags>-Wa,--noexecstack
<compileflags>-Wformat
<compileflags>-Werror=format-security
<compileflags>-Wall
<compileflags>-Wshadow
;
using clang : 5.0~arm
: $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/$HOSTOS$OSARCH/bin/clang++ $EXTRA_ANDROID_FLAGS
:
<architecture>arm <target-os>android
<compileflags>--target=armv7-none-linux-androideabi
<compileflags>--gcc-toolchain=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/$HOSTOS$OSARCH
<compileflags>--sysroot=$ANDROID_NDK_ROOT/sysroot
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++abi/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/android/support/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include/arm-linux-androideabi
<compileflags>-DANDROID
<compileflags>-D__ANDROID_API__=19
<compileflags>-ffunction-sections
<compileflags>-funwind-tables
<compileflags>-fstack-protector-strong
<compileflags>-fno-limit-debug-info
<compileflags>-fPIC
<compileflags>-fno-integrated-as
<compileflags>-no-canonical-prefixes
<compileflags>-Wa,--noexecstack
<compileflags>-Wformat
<compileflags>-Werror=format-security
<compileflags>-Wall
<compileflags>-Wshadow
<compileflags>-march=armv7-a
<compileflags>-mfloat-abi=softfp
<compileflags>-mfpu=vfpv3-d16
<compileflags>-mthumb
;
using clang : 5.0~arm64
: $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/$HOSTOS$OSARCH/bin/clang++ $EXTRA_ANDROID_FLAGS
:
<architecture>arm <target-os>android
<compileflags>--target=aarch64-none-linux-android
<compileflags>--gcc-toolchain=$ANDROID_NDK_ROOT/toolchains/aarch64-linux-android-4.9/prebuilt/$HOSTOS$OSARCH
<compileflags>--sysroot=$ANDROID_NDK_ROOT/sysroot
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/cxx-stl/llvm-libc++abi/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sources/android/support/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include
<compileflags>-isystem <compileflags>$ANDROID_NDK_ROOT/sysroot/usr/include/aarch64-linux-android
<compileflags>-DANDROID
<compileflags>-D__ANDROID_API__=21
<compileflags>-ffunction-sections
<compileflags>-funwind-tables
<compileflags>-fstack-protector-strong
<compileflags>-fno-limit-debug-info
<compileflags>-fPIC
<compileflags>-no-canonical-prefixes
<compileflags>-Wa,--noexecstack
<compileflags>-Wformat
<compileflags>-Werror=format-security
<compileflags>-Wall
<compileflags>-Wshadow
;
EOF
}

buildBoost_Android()
{
    mkdir -p $OUTPUT_DIR
    echo > ${OUTPUT_DIR}/android-build.log

    if [[ -z "$ANDROID_NDK_ROOT" ]]; then
        echo "Must specify ANDROID_NDK_ROOT"
        exit 1
    fi

    export NO_BZIP2=1

    # build libicu if locale requested but not provided
    # if echo $LIBRARIES | grep locale; then
    #   if [ -e libiconv-libicu-android ]; then
    #     echo "ICONV and ICU already compiled"
    #   else
    #     echo "boost_locale selected - compiling ICONV and ICU"
    #     git clone https://github.com/pelya/libiconv-libicu-android.git
    #     cd libiconv-libicu-android
    #     ./build.sh || exit 1
    #     cd ..
    #   fi
    # fi

    echo clean
    ./b2 --clean

    echo Building release x86 Boost for Android Emulator

    ./b2 --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/x86" toolset=clang-5.0~x86 \
        architecture=x86 target-os=android define=_LITTLE_ENDIAN \
        optimization=speed \
        address-model=32 variant=release cxxflags="${CPPSTD}" \
        link=static threading=multi install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error staging Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection

    echo Building release x86_64 Boost for Android Emulator

    ./b2 --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/x86_64" toolset=clang-5.0~x86_64 \
        architecture=x86 target-os=android define=_LITTLE_ENDIAN \
        optimization=speed \
        address-model=64 variant=release cxxflags="${CPPSTD}" \
        link=static threading=multi install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error staging Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection

    echo Building release armv7 Boost for Android

    ./b2 --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/armeabi-v7a" toolset=clang-5.0~arm \
        abi=aapcs architecture=arm address-model=32 binary-format=elf threading=multi \
        optimization=space \
        target-os=android variant=release cxxflags="${CPPSTD}" \
        link=static install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error installing Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection

    echo Building release arm64 Boost for Android

    ./b2 --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/arm64-v8a" toolset=clang-5.0~arm64 \
        abi=aapcs architecture=arm address-model=64 binary-format=elf threading=multi \
        optimization=space \
        target-os=android variant=release cxxflags="${CPPSTD}" \
        link=static install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error installing Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection
}

buildBoost_Android_debug()
{
    mkdir -p $OUTPUT_DIR
    echo > ${OUTPUT_DIR}/android-build.log

    export NO_BZIP2=1

    # build libicu if locale requested but not provided
    # if echo $LIBRARIES | grep locale; then
    #   if [ -e libiconv-libicu-android ]; then
    #     echo "ICONV and ICU already compiled"
    #   else
    #     echo "boost_locale selected - compiling ICONV and ICU"
    #     git clone https://github.com/pelya/libiconv-libicu-android.git
    #     cd libiconv-libicu-android
    #     ./build.sh || exit 1
    #     cd ..
    #   fi
    # fi

    echo Building debug x86 Boost for Android Emulator

    ./b2 $THREADS --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/debug/x86" toolset=clang-5.0~x86 \
        architecture=x86 target-os=android define=_LITTLE_ENDIAN \
        optimization=speed \
        address-model=32 variant=debug cxxflags="${CPPSTD}" \
        link=static threading=multi install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error staging Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection

    echo Building debug x86_64 Boost for Android Emulator

    ./b2 $THREADS --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/debug/x86_64" toolset=clang-5.0~x86_64 \
        architecture=x86 target-os=android define=_LITTLE_ENDIAN \
        optimization=speed \
        address-model=64 variant=debug cxxflags="${CPPSTD}" \
        link=static threading=multi install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error staging Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection

    echo Building debug armv7 Boost for Android

    ./b2 $THREADS --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/debug/armeabi-v7a" toolset=clang-5.0~arm \
        abi=aapcs architecture=arm address-model=32 binary-format=elf threading=multi \
        optimization=space \
        target-os=android variant=debug cxxflags="${CPPSTD}" \
        link=static install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error installing Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection

    echo Building debug arm64 Boost for Android

    ./b2 $THREADS --build-dir=android-build --stagedir=android-build/stage \
        --prefix="$OUTPUT_DIR" \
        --libdir="$OUTPUT_DIR/lib/debug/arm64-v8a" toolset=clang-5.0~arm64 \
        abi=aapcs architecture=arm address-model=64 binary-format=elf threading=multi \
        optimization=space \
        target-os=android variant=debug cxxflags="${CPPSTD}" \
        link=static install >> "${OUTPUT_DIR}/android-build.log" 2>&1
    if [ $? != 0 ]; then echo "Error installing Android. Check ${OUTPUT_DIR}/android-build.log"; exit 1; fi

    doneSection
}

bootstrapBoost
generateAndroidUserConfig
buildBoost_Android
#buildBoost_Android_debug

echo "Completed successfully"

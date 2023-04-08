#!/bin/bash

ICU_VERSION="58"

ICU_DIR="$PWD/icu"

ICU_SOURCE="${ICU_DIR}/source"

PREBUILD="$PWD/ios/icu_build"

CONFIG_PREFIX=" --enable-extras=yes \
--enable-tools=yes \
--enable-icuio=yes \
--enable-strict=no \
--enable-static \
--enable-shared=no \
--enable-tests=yes \
--disable-renaming \
--enable-samples=no \
--enable-dyload=no \
--with-data-packaging=static"

CFLAGS="-O3 -D__STDC_INT64__ -fno-exceptions -fno-short-wchar -fno-short-enums -fembed-bitcode"

CXXFLAGS="${CFLAGS} -std=c++11 -fembed-bitcode"

#will set value to 1
defines_config_set_1=(
    "UCONFIG_NO_BREAK_ITERATION"
    "UCONFIG_NO_FORMATTING"
    "UCONFIG_NO_REGULAR_EXPRESSIONS"
    "CONFIG_NO_CONVERSION"
    "U_DISABLE_RENAMING"
)

#will set value to 0
defines_config_set_0=(
    "U_HAVE_NL_LANGINFO_CODESET"
    "UCONFIG_NO_TRANSLITERATION"
    "U_USING_ICU_NAMESPACE"
)

#will set value to 1
defines_utypes=(
    "U_CHARSET_IS_UTF8"
)

function prebuild() {

    svn export http://source.icu-project.org/repos/icu/tags/release-${ICU_VERSION}/icu4c/ ${ICU_DIR} --native-eol LF

    echo "===== REMOVING data from bundle ====="

    #Data bundle reduction
    {
        echo "GENRB_CLDR_VERSION = 32.0.1"
        echo "GENRB_SYNTHETIC_ALIAS = "
        echo "GENRB_ALIAS_SOURCE = \$(GENRB_SYNTHETIC_ALIAS) "
        echo "GENRB_SOURCE = "
    } >"${ICU_SOURCE}/data/locales/reslocal.mk"

    {
        echo "LANG_CLDR_VERSION = 32.0.1"
        echo "LANG_SYNTHETIC_ALIAS = "
        echo "LANG_ALIAS_SOURCE = \$(LANG_SYNTHETIC_ALIAS) "
        echo "LANG_SOURCE = "
    } >"${ICU_SOURCE}/data/lang/reslocal.mk"

    {
        echo "CURR_CLDR_VERSION = 32.0.1"
        echo "CURR_SYNTHETIC_ALIAS = "
        echo "CURR_ALIAS_SOURCE = \$(CURR_SYNTHETIC_ALIAS) "
        echo "CURR_SOURCE = "
    } >"${ICU_SOURCE}/data/curr/reslocal.mk"

    {
        echo "REGION_CLDR_VERSION = 32.0.1"
        echo "REGION_SYNTHETIC_ALIAS = "
        echo "REGION_ALIAS_SOURCE = \$(REGION_SYNTHETIC_ALIAS) "
        echo "REGION_SOURCE = "
    } >"${ICU_SOURCE}/data/region/reslocal.mk"

    {
        echo "UNIT_CLDR_VERSION = 32.0.1"
        echo "UNIT_SYNTHETIC_ALIAS = "
        echo "UNIT_ALIAS_SOURCE = \$(UNIT_SYNTHETIC_ALIAS) "
        echo "UNIT_SOURCE = "
    } >"${ICU_SOURCE}/data/unit/reslocal.mk"

    {
        echo "ZONE_CLDR_VERSION = 32.0.1"
        echo "ZONE_SYNTHETIC_ALIAS = "
        echo "ZONE_ALIAS_SOURCE = \$(ZONE_SYNTHETIC_ALIAS) "
        echo "ZONE_SOURCE = "
    } >"${ICU_SOURCE}/data/zone/reslocal.mk"

    #find "${ICU_SOURCES}/data/mappings/" -name '*.mk' ! -name 'ucmcore.mk' -type f -exec rm -f {} +

    mv "${ICU_SOURCE}/data/mappings/ucmcore.mk" "${ICU_SOURCE}/data/mappings/ucmcore.not_used" 2>/dev/null
    mv "${ICU_SOURCE}/data/mappings/ucmfiles.mk" "${ICU_SOURCE}/data/mappings/ucmfiles.not_used" 2>/dev/null
    mv "${ICU_SOURCE}/data/mappings/ucmebcdic.mk" "${ICU_SOURCE}/data/mappings/ucmebcdic.not_used" 2>/dev/null
    mv "${ICU_SOURCE}/data/mappings/ucmlocal.mk" "${ICU_SOURCE}/data/mappings/ucmlocal.not_used" 2>/dev/null

    echo "===== Modify icu/source/common/unicode/uconfig.h ====="

    cp "${ICU_SOURCE}/common/unicode/uconfig" "${ICU_SOURCE}/common/unicode/uconfig.h" 2>/dev/null
    cp "${ICU_SOURCE}/common/unicode/uconfig.h" "${ICU_SOURCE}/common/unicode/uconfig" 2>/dev/null

    for var in "${defines_config_set_1[@]}"; do
        sed "/define __UCONFIG_H__/a \\
#ifndef ${var} \\
#define ${var} 1 \\
#endif \\
" "${ICU_SOURCE}/common/unicode/uconfig.h" >"${ICU_SOURCE}/common/unicode/uconfig.tmp"

        mv "${ICU_SOURCE}/common/unicode/uconfig.tmp" "${ICU_SOURCE}/common/unicode/uconfig.h"
    done

    for var in "${defines_config_set_0[@]}"; do
        sed "/define __UCONFIG_H__/a \\
#ifndef ${var} \\
#define ${var} 0 \\
#endif \\
" "${ICU_SOURCE}/common/unicode/uconfig.h" >"${ICU_SOURCE}/common/unicode/uconfig.tmp"

        mv "${ICU_SOURCE}/common/unicode/uconfig.tmp" "${ICU_SOURCE}/common/unicode/uconfig.h"
    done

    echo "===== Modify icu/source/common/unicode/utypes.h ====="

    cp "${ICU_SOURCE}/common/unicode/utypes" "${ICU_SOURCE}/common/unicode/utypes.h" 2>/dev/null
    cp "${ICU_SOURCE}/common/unicode/utypes.h" "${ICU_SOURCE}/common/unicode/utypes" 2>/dev/null

    for var in "${defines_utypes[@]}"; do
        sed "/define UTYPES_H/a \\
#ifndef ${var} \\
#define ${var} 1 \\
#endif \\
" "${ICU_SOURCE}/common/unicode/utypes.h" >"${ICU_SOURCE}/common/unicode/utypes.tmp"

        mv "${ICU_SOURCE}/common/unicode/utypes.tmp" "${ICU_SOURCE}/common/unicode/utypes.h"

    done

    echo "===== Patching icu/source/tools/pkgdata/pkgdata.cpp for iOS ====="

    cp "${ICU_SOURCE}/tools/pkgdata/pkgdata" "${ICU_SOURCE}/tools/pkgdata/pkgdata.cpp" 2>/dev/null
    cp "${ICU_SOURCE}/tools/pkgdata/pkgdata.cpp" "${ICU_SOURCE}/tools/pkgdata/pkgdata" 2>/dev/null

    sed "s/int result = system(cmd);/ \\
#if defined(IOS_SYSTEM_FIX) \\
pid_t pid; \\
char * argv[2]; argv[0] = cmd; argv[1] = NULL; \\
posix_spawn(\&pid, argv[0], NULL, NULL, argv, environ); \\
waitpid(pid, NULL, 0); \\
int result = 0; \\
#else \\
int result = system(cmd); \\
#endif \\
/g" "${ICU_SOURCE}/tools/pkgdata/pkgdata.cpp" >"${ICU_SOURCE}/tools/pkgdata/pkgdata.tmp"

    sed "/#include <stdlib.h>/a \\
#if defined(IOS_SYSTEM_FIX) \\
#include <spawn.h> \\
extern char **environ; \\
#endif \\
" "${ICU_SOURCE}/tools/pkgdata/pkgdata.tmp" >"${ICU_SOURCE}/tools/pkgdata/pkgdata.cpp"

    #mv "${ICU_SOURCE}/tools/pkgdata/pkgdata.tmp" "${ICU_SOURCE}/tools/pkgdata/pkgdata.cpp"

    echo "==============================="
    echo "===== Run build for MacOS ====="
    echo "==============================="

    export PLATFORM_PREFIX="${PWD}/mac-build"

    export CPPFLAGS=${CFLAGS}

    mkdir -p ${PREBUILD}

    cd ${PREBUILD}

    sh ${ICU_SOURCE}/runConfigureICU MacOSX --prefix=${PLATFORM_PREFIX} ${CONFIG_PREFIX}

    make clean
    make -j4
    make install

    cd ..
}

function build() {

    unset CXX
    unset CC
    unset CFLAGS
    unset CXXFLAGS
    unset LDFLAGS

    echo "preparing $1 toolchain"

    export BUILD_DIR="${PWD}/build-${5}"

    SDKROOT="$(xcodebuild -version -sdk $4 | grep -E '^Path' | sed 's/Path: //')"
    ARCH=$2

    ICU_FLAGS="-I$ICU_SOURCE/common/ -I$ICU_SOURCE/tools/tzcode/ "

    IOS_MIN_VER = "-miphoneos-version-min=11.0"
    if [ "$4" = "iphonesimulator" ]; then 
        IOS_MIN_VER = "-mios-simulator-version-min=11.0"
    fi;

    export ADDITION_FLAG="-DIOS_SYSTEM_FIX"

    export CXX="$(xcrun -find clang++)"
    export CC="$(xcrun -find clang)"
    export CFLAGS="-fembed-bitcode -isysroot $SDKROOT -I$SDKROOT/usr/include/ -I./include/ -arch $ARCH $IOS_MIN_VER $ICU_FLAGS $CFLAGS ${ADDITION_FLAG}"
    export CXXFLAGS="${CXXFLAGS} -fembed-bitcode -stdlib=libc++ -isysroot $SDKROOT -I$SDKROOT/usr/include/ -I./include/ -arch $ARCH $IOS_MIN_VER $ICU_FLAGS ${ADDITION_FLAG}"
    export LDFLAGS="-fembed-bitcode -stdlib=libc++ -L$SDKROOT/usr/lib/ -isysroot $SDKROOT -Wl,-dead_strip $IOS_MIN_VER -lstdc++ ${ADDITION_FLAG}"

    mkdir -p ${BUILD_DIR}
    cd ${BUILD_DIR}

    sh ${ICU_SOURCE}/configure --host=$3 --with-library-suffix=${2} --with-cross-build=${PREBUILD} ${CONFIG_PREFIX}

    make clean
    make -j4

    cd ..
}

prebuild

echo "------------------------------------------------------"
echo " ICU build armv7 libraries"
echo "------------------------------------------------------"

build "armv7" "armv7" "armv7-apple-darwin" "iphoneos" "armv7"

echo "------------------------------------------------------"
echo " ICU build arm64 libraries"
echo "------------------------------------------------------"

build "arm64" "arm64" "aarch64-apple-darwin" "iphoneos" "arm64"

echo "------------------------------------------------------"
echo " ICU combining x86_64 libraries"
echo "------------------------------------------------------"

build "x86_64" "x86_64" "x86_64-apple-darwin" "iphonesimulator" "x86_64"

echo "------------------------------------------------------"
echo " ICU combining arm64 (simulator) libraries"
echo "------------------------------------------------------"

build "arm64" "arm64" "aarch64-apple-darwin" "iphonesimulator" "sim-arm64"

cd ..

echo "------------------------------------------------------"
echo " ICU combining x86_64, armv7 and arm64 libraries"
echo "------------------------------------------------------"

function buildUniversal() {
    lipo -create -output "${PWD}/ios/build/$1.a" \
        "${PWD}/ios/build-x86_64/lib/$1x86_64.a" \
        "${PWD}/ios/build-arm64/lib/$1arm64.a" \
        "${PWD}/ios/build-armv7/lib/$1armv7.a"
}

function buildUniversalXC() {
    lipo -create -output "${PWD}/ios/build_xc/$1.a" \
        "${PWD}/ios/build-arm64/lib/$1arm64.a"

    lipo -create -output "${PWD}/ios/build_xc/simulator/${1}.a" \
        "${PWD}/ios/build-sim-arm64/lib/$1arm64.a" \
        "${PWD}/ios/build-x86_64/lib/$1x86_64.a"
}

mkdir -p build
mkdir -p ios/build
mkdir -p ios/build_xc
mkdir -p ios/build_xc/simulator
mkdir -p ios/build/include
mkdir -p ios/build_xc/include
mkdir -p ios/build/include/unicode
mkdir -p ios/build_xc/include/unicode

cp ${ICU_SOURCE}/common/unicode/*.h ios/build/include/unicode
cp ${ICU_SOURCE}/common/unicode/*.h ios/build_xc/include/unicode

buildUniversal "libicuuc"
buildUniversal "libicui18n"
buildUniversal "libicudata"

buildUniversalXC "libicuuc"
buildUniversalXC "libicui18n"
buildUniversalXC "libicudata"

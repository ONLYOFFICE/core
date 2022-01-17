#!/bin/bash
#
# Copyright 2016 leenjewel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# read -n1 -p "Press any key to continue..."

set -u

source ./build-ios-common.sh

TOOLS_ROOT=$(pwd)

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$(cd -P "$(dirname "$SOURCE")" && pwd)"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$(cd -P "$(dirname "$SOURCE")" && pwd)"

echo pwd_path=${pwd_path}
echo TOOLS_ROOT=${TOOLS_ROOT}

LIB_VERSION="curl-7_68_0"
LIB_NAME="curl-7.68.0"
LIB_DEST_DIR="${pwd_path}/build/ios//curl-universal/lib"

init_log_color

echo "https://github.com/curl/curl/releases/download/${LIB_VERSION}/${LIB_NAME}.tar.gz"

# https://curl.haxx.se/download/${LIB_NAME}.tar.gz
# https://github.com/curl/curl/releases/download/curl-7_69_0/curl-7.69.0.tar.gz
# https://github.com/curl/curl/releases/download/curl-7_68_0/curl-7.68.0.tar.gz
DEVELOPER=$(xcode-select -print-path)
SDK_VERSION=$(xcrun -sdk iphoneos --show-sdk-version)
rm -rf "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl -L -o ${LIB_NAME}.tar.gz https://github.com/curl/curl/releases/download/${LIB_VERSION}/${LIB_NAME}.tar.gz -s
[ -f "${LIB_NAME}.tar.gz" ] || log_error "curl download error!"

function configure_make() {

    ARCH=$1
    SDK=$2
    PLATFORM=$3

    log_info "configure $ARCH start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
    export CROSS_SDK="${PLATFORM}${SDK_VERSION}.sdk"

    if [ ! -d ${CROSS_TOP}/SDKs/${CROSS_SDK} ]; then
        log_error "ERROR: iOS SDK version:'${SDK_VERSION}' incorrect, SDK in your system is:"
        xcodebuild -showsdks | grep iOS
        exit -1
    fi

    PREFIX_DIR="${pwd_path}/build/ios/${ARCH}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    OUTPUT_ROOT=${TOOLS_ROOT}/build/ios/${ARCH}
    mkdir -p ${OUTPUT_ROOT}/log

    set_ios_cpu_feature "curl" "${ARCH}" "${IOS_MIN_TARGET}" "${CROSS_TOP}/SDKs/${CROSS_SDK}"

    OPENSSL_OUT_DIR="${pwd_path}/../openssl/build/ios/${ARCH}"

    export LDFLAGS="${LDFLAGS} -L${OPENSSL_OUT_DIR}/lib"

    ios_printf_global_params "$ARCH" "$SDK" "$PLATFORM" "$PREFIX_DIR" "$OUTPUT_ROOT"

    if [[ "${ARCH}" == "x86_64" ]]; then

        ./Configure --host=$(ios_get_build_host "$ARCH") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --enable-static --disable-shared >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1

    elif [[ "${ARCH}" == "armv7" ]]; then

        ./Configure --host=$(ios_get_build_host "$ARCH") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --enable-static --disable-shared >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1

    elif [[ "${ARCH}" == "arm64" ]]; then

        ./Configure --host=$(ios_get_build_host "$ARCH") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --enable-static --disable-shared >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
    
    elif [[ "${ARCH}" == "i386" ]]; then

        ./Configure --host=$(ios_get_build_host "$ARCH") --prefix="${PREFIX_DIR}" --enable-ipv6 --with-ssl=${OPENSSL_OUT_DIR} --enable-static --disable-shared >"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1

    else
        log_error "not support" && exit 1
    fi

    log_info "make $ARCH start..."

    make clean >>"${OUTPUT_ROOT}/log/${ARCH}.log"
    if make -j8 >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1; then
        make install >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
    fi

    popd
}

log_info "${PLATFORM_TYPE} ${LIB_NAME} start..."

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${SDKS[i]}" "${PLATFORMS[i]}"
    fi
done

log_info "lipo start..."

function lipo_library() {
    LIB_SRC=$1
    LIB_DST=$2
    LIB_PATHS=("${ARCHS[@]/#/${pwd_path}/build/ios/}")
    LIB_PATHS=("${LIB_PATHS[@]/%//lib/${LIB_SRC}}")
    lipo ${LIB_PATHS[@]} -create -output "${LIB_DST}"
}

function copy_include() {
    DST=$1
    if [ -d "${pwd_path}/build/ios/x86_64/include" ]; then
        cp -r "${pwd_path}/build/ios/x86_64/include"  "${DST}"
    elif [ -d "${pwd_path}/build/ios/armv7/include" ]; then
        cp -r "${pwd_path}/build/ios/armv7/include"  "${DST}"
    elif [ -d "${pwd_path}/build/ios/arm64/include" ]; then
        cp -r "${pwd_path}/build/ios/arm64/include"  "${DST}"
    elif [ -d "${pwd_path}/build/ios/i386/include" ]; then
        cp -r "${pwd_path}/build/ios/i386/include"  "${DST}"  
    fi
}

mkdir -p "${LIB_DEST_DIR}"
lipo_library "libcurl.a" "${LIB_DEST_DIR}/libcurl.a"
copy_include "${LIB_DEST_DIR}/../"

log_info "${PLATFORM_TYPE} ${LIB_NAME} end..."

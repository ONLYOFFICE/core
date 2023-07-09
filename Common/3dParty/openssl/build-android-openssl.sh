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

# # read -n1 -p "Press any key to continue..."

set -u

source ./build-android-common.sh

init_log_color

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

# openssl-1.1.0f has a configure bug
# openssl-1.1.1d has fix configure bug
LIB_VERSION="OpenSSL_1_1_1i"
LIB_NAME="openssl-1.1.1i"

echo "https://www.openssl.org/source/${LIB_NAME}.tar.gz"

# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1d.tar.gz
# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1f.tar.gz
[ -f "${LIB_NAME}.tar.gz" ] || curl -L -o ${LIB_NAME}.tar.gz https://www.openssl.org/source/${LIB_NAME}.tar.gz -s
[ -f "${LIB_NAME}.tar.gz" ] || log_error "openssl download error!"

set_android_toolchain_bin

function configure_make() {

    ARCH=$1
    ABI=$2
    ABI_TRIPLE=$3

    log_info "configure $ABI start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    PREFIX_DIR="${pwd_path}/build/android/${ABI}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    OUTPUT_ROOT=${TOOLS_ROOT}/build/android/${ABI}
    mkdir -p ${OUTPUT_ROOT}/log

    set_android_toolchain "openssl" "${ARCH}" "${ANDROID_API}"
    set_android_cpu_feature "openssl" "${ARCH}" "${ANDROID_API}"

    export ANDROID_NDK_HOME=${ANDROID_NDK_ROOT}
    echo ANDROID_NDK_HOME=${ANDROID_NDK_HOME}

    android_printf_global_params "$ARCH" "$ABI" "$ABI_TRIPLE" "$PREFIX_DIR" "$OUTPUT_ROOT"

    if [[ "${ARCH}" == "x86_64" ]]; then

        ./Configure android-x86_64 --prefix="${PREFIX_DIR}" no-shared no-tests enable-ssl3 enable-ssl3-method enable-md2 no-asm

    elif [[ "${ARCH}" == "x86" ]]; then

        ./Configure android-x86 --prefix="${PREFIX_DIR}" no-shared no-tests enable-ssl3 enable-ssl3-method enable-md2 no-asm

    elif [[ "${ARCH}" == "arm" ]]; then

        ./Configure android-arm --prefix="${PREFIX_DIR}" no-shared no-tests enable-ssl3 enable-ssl3-method enable-md2 no-asm

    elif [[ "${ARCH}" == "arm64" ]]; then

        ./Configure android-arm64 --prefix="${PREFIX_DIR}" no-shared no-tests enable-ssl3 enable-ssl3-method enable-md2 no-asm

    else
        log_error "not support" && exit 1
    fi

    log_info "make $ABI start..."

    sed -ie 's/LIB_CFLAGS=/LIB_CFLAGS=-fvisibility=hidden /g' ./Makefile
    sed -ie 's/LIB_CXXFLAGS=/LIB_CXXFLAGS=-fvisibility=hidden /g' ./Makefile

    make clean >"${OUTPUT_ROOT}/log/${ABI}.log"
    if make -j$(get_cpu_count) >>"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1; then
        make install_sw >>"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1
        make install_ssldirs >>"${OUTPUT_ROOT}/log/${ABI}.log" 2>&1
    fi

    popd
}

log_info "${PLATFORM_TYPE} ${LIB_NAME} start..."

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${ABIS[i]}" "${ABI_TRIPLES[i]}"
    fi
done

log_info "${PLATFORM_TYPE} ${LIB_NAME} end..."

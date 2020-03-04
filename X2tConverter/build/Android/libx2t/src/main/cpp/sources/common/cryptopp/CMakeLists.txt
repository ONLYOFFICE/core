cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_CRYPTOPP cryptopp)

# Path to source files
set(CRYPTOPP_DIR ${CORE_DIR}/Common/3dParty/cryptopp)
message(STATUS "CryptoPP source path: ${CRYPTOPP_DIR}")

# Library source .h .cpp
file(GLOB SRC_CRYPTOPP_CPP
    ${CRYPTOPP_DIR}/3way.cpp
    ${CRYPTOPP_DIR}/adler32.cpp
    ${CRYPTOPP_DIR}/algebra.cpp
    ${CRYPTOPP_DIR}/algparam.cpp
    ${CRYPTOPP_DIR}/arc4.cpp
    ${CRYPTOPP_DIR}/asn.cpp
    ${CRYPTOPP_DIR}/authenc.cpp
    ${CRYPTOPP_DIR}/base32.cpp
    ${CRYPTOPP_DIR}/base64.cpp
    ${CRYPTOPP_DIR}/basecode.cpp
    ${CRYPTOPP_DIR}/bench2.cpp
    ${CRYPTOPP_DIR}/bfinit.cpp
    ${CRYPTOPP_DIR}/blowfish.cpp
    ${CRYPTOPP_DIR}/blumshub.cpp
    ${CRYPTOPP_DIR}/camellia.cpp
    ${CRYPTOPP_DIR}/cast.cpp
    ${CRYPTOPP_DIR}/casts.cpp
    ${CRYPTOPP_DIR}/cbcmac.cpp
    ${CRYPTOPP_DIR}/ccm.cpp
    ${CRYPTOPP_DIR}/channels.cpp
    ${CRYPTOPP_DIR}/cmac.cpp
    ${CRYPTOPP_DIR}/crc.cpp
    ${CRYPTOPP_DIR}/cryptlib.cpp
    ${CRYPTOPP_DIR}/datatest.cpp
    ${CRYPTOPP_DIR}/default.cpp
    ${CRYPTOPP_DIR}/des.cpp
    ${CRYPTOPP_DIR}/dessp.cpp
    ${CRYPTOPP_DIR}/dh.cpp
    ${CRYPTOPP_DIR}/dh2.cpp
    ${CRYPTOPP_DIR}/dsa.cpp
    ${CRYPTOPP_DIR}/eax.cpp
    ${CRYPTOPP_DIR}/ec2n.cpp
    ${CRYPTOPP_DIR}/eccrypto.cpp
    ${CRYPTOPP_DIR}/ecp.cpp
    ${CRYPTOPP_DIR}/elgamal.cpp
    ${CRYPTOPP_DIR}/emsa2.cpp
    ${CRYPTOPP_DIR}/eprecomp.cpp
    ${CRYPTOPP_DIR}/esign.cpp
    ${CRYPTOPP_DIR}/files.cpp
    ${CRYPTOPP_DIR}/filters.cpp
    ${CRYPTOPP_DIR}/fips140.cpp
    ${CRYPTOPP_DIR}/fipsalgt.cpp
    ${CRYPTOPP_DIR}/fipstest.cpp
    ${CRYPTOPP_DIR}/gcm.cpp
    ${CRYPTOPP_DIR}/gf2_32.cpp
    ${CRYPTOPP_DIR}/gf2n.cpp
    ${CRYPTOPP_DIR}/gf256.cpp
    ${CRYPTOPP_DIR}/gfpcrypt.cpp
    ${CRYPTOPP_DIR}/gost.cpp
    ${CRYPTOPP_DIR}/gzip.cpp
    ${CRYPTOPP_DIR}/hex.cpp
    ${CRYPTOPP_DIR}/hmac.cpp
    ${CRYPTOPP_DIR}/hrtimer.cpp
    ${CRYPTOPP_DIR}/ida.cpp
    ${CRYPTOPP_DIR}/idea.cpp
    ${CRYPTOPP_DIR}/integer.cpp
    ${CRYPTOPP_DIR}/iterhash.cpp
    ${CRYPTOPP_DIR}/luc.cpp
    ${CRYPTOPP_DIR}/mars.cpp
    ${CRYPTOPP_DIR}/marss.cpp
    ${CRYPTOPP_DIR}/md2.cpp
    ${CRYPTOPP_DIR}/md4.cpp
    ${CRYPTOPP_DIR}/md5.cpp
    ${CRYPTOPP_DIR}/misc.cpp
    ${CRYPTOPP_DIR}/modes.cpp
    ${CRYPTOPP_DIR}/mqueue.cpp
    ${CRYPTOPP_DIR}/mqv.cpp
    ${CRYPTOPP_DIR}/nbtheory.cpp
    ${CRYPTOPP_DIR}/network.cpp
    ${CRYPTOPP_DIR}/oaep.cpp
    ${CRYPTOPP_DIR}/osrng.cpp
    ${CRYPTOPP_DIR}/panama.cpp
    ${CRYPTOPP_DIR}/pch.cpp
    ${CRYPTOPP_DIR}/pkcspad.cpp
    ${CRYPTOPP_DIR}/polynomi.cpp
    ${CRYPTOPP_DIR}/pssr.cpp
    ${CRYPTOPP_DIR}/pubkey.cpp
    ${CRYPTOPP_DIR}/queue.cpp
    ${CRYPTOPP_DIR}/rabin.cpp
    ${CRYPTOPP_DIR}/randpool.cpp
    ${CRYPTOPP_DIR}/rc2.cpp
    ${CRYPTOPP_DIR}/rc5.cpp
    ${CRYPTOPP_DIR}/rc6.cpp
    ${CRYPTOPP_DIR}/rdrand.cpp
    ${CRYPTOPP_DIR}/dll.cpp
    ${CRYPTOPP_DIR}/dlltest.cpp
    ${CRYPTOPP_DIR}/rdtables.cpp
    ${CRYPTOPP_DIR}/rijndael.cpp
    ${CRYPTOPP_DIR}/ripemd.cpp
    ${CRYPTOPP_DIR}/rng.cpp
    ${CRYPTOPP_DIR}/rsa.cpp
    ${CRYPTOPP_DIR}/rw.cpp
    ${CRYPTOPP_DIR}/safer.cpp
    ${CRYPTOPP_DIR}/salsa.cpp
    ${CRYPTOPP_DIR}/seal.cpp
    ${CRYPTOPP_DIR}/seed.cpp
    ${CRYPTOPP_DIR}/serpent.cpp
    ${CRYPTOPP_DIR}/sse-simd.cpp
    ${CRYPTOPP_DIR}/sha.cpp
    ${CRYPTOPP_DIR}/sha3.cpp
    ${CRYPTOPP_DIR}/shacal2.cpp
    ${CRYPTOPP_DIR}/shark.cpp
    ${CRYPTOPP_DIR}/sharkbox.cpp
    ${CRYPTOPP_DIR}/simple.cpp
    ${CRYPTOPP_DIR}/skipjack.cpp
    ${CRYPTOPP_DIR}/socketft.cpp
    ${CRYPTOPP_DIR}/sosemanuk.cpp
    ${CRYPTOPP_DIR}/square.cpp
    ${CRYPTOPP_DIR}/squaretb.cpp
    ${CRYPTOPP_DIR}/strciphr.cpp
    ${CRYPTOPP_DIR}/tea.cpp
    ${CRYPTOPP_DIR}/test.cpp
    ${CRYPTOPP_DIR}/tftables.cpp
    ${CRYPTOPP_DIR}/tiger.cpp
    ${CRYPTOPP_DIR}/tigertab.cpp
    ${CRYPTOPP_DIR}/trdlocal.cpp
    ${CRYPTOPP_DIR}/ttmac.cpp
    ${CRYPTOPP_DIR}/twofish.cpp
    ${CRYPTOPP_DIR}/validat1.cpp
    ${CRYPTOPP_DIR}/validat2.cpp
    ${CRYPTOPP_DIR}/validat3.cpp
    ${CRYPTOPP_DIR}/vmac.cpp
    ${CRYPTOPP_DIR}/wait.cpp
    ${CRYPTOPP_DIR}/wake.cpp
    ${CRYPTOPP_DIR}/whrlpool.cpp
    ${CRYPTOPP_DIR}/winpipes.cpp
    ${CRYPTOPP_DIR}/xtr.cpp
    ${CRYPTOPP_DIR}/xtrcrypt.cpp
    ${CRYPTOPP_DIR}/zdeflate.cpp
    ${CRYPTOPP_DIR}/zinflate.cpp
    ${CRYPTOPP_DIR}/zlib.cpp
    ${CRYPTOPP_DIR}/cpu.cpp
)

# Set targer as static library
add_library(${LIB_NAME_CRYPTOPP}
    STATIC
        ${SRC_CRYPTOPP_CPP}
)

## Add dependency library
target_link_libraries(${LIB_NAME_CRYPTOPP}
    PRIVATE
        cpufeature
)

# Add include files .h
target_include_directories(${LIB_NAME_CRYPTOPP}
    PUBLIC
        ${CRYPTOPP_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_CRYPTOPP}
    PRIVATE
        UNICODE
        CRYPTOPPLIB_LIBRARY
        CRYPTOPP_DISABLE_ASM
)

# Set target compiler options
target_compile_options(${LIB_NAME_CRYPTOPP}
    PRIVATE
        -maes
        -mpclmul
)
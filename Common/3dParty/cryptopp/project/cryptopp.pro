QT       -= core gui

TARGET = CryptoPPLib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += CRYPTOPPLIB_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include(../../../../Common/base.pri)

core_linux:DEFINES -= NDEBUG
core_mac:DEFINES -= MAC
core_ios:DEFINES -= MAC
core_windows:DEFINES -= UNICODE _UNICODE

DEFINES += CRYPTOPP_DISABLE_ASM
DEFINES += DISABLE_TYPE_MISMATCH\
#DEFINES += USE_PRECOMPILED_HEADERS

# use old CryptoAPI for xp build
build_xp:DEFINES += USE_MS_CRYPTOAPI

core_android {
    INCLUDEPATH += $$(ANDROID_NDK_ROOT)/sources/android/cpufeatures
}

HEADERS += \
    ../3way.h \
    ../adler32.h \
    ../adv_simd.h \
    ../aes.h \
    ../aes_armv4.h \
    ../algebra.h \
    ../algparam.h \
    ../allocate.h \
    ../arc4.h \
    ../aria.h \
    ../argnames.h \
    ../arm_simd.h \
    ../asn.h \
    ../authenc.h \
    ../base32.h \
    ../base64.h \
    ../basecode.h \
    ../bench.h \
    ../blake2.h \
    ../blowfish.h \
    ../blumshub.h \
    ../camellia.h \
    ../cast.h \
    ../cbcmac.h \
    ../ccm.h \
    ../chacha.h \
    ../chachapoly.h \
    ../cham.h \
    ../channels.h \
    ../cmac.h \
    ../config.h \
    ../config_align.h \
    ../config_asm.h \
    ../config_cpu.h \
    ../config_cxx.h \
    ../config_dll.h \
    ../config_int.h \
    ../config_misc.h \
    ../config_ns.h \
    ../config_os.h \
    ../config_ver.h \
    ../cpu.h \
    ../crc.h \
    ../cryptlib.h \
    ../darn.h \
    ../default.h \
    ../des.h \
    ../dh.h \
    ../dh2.h \
    ../dll.h \
    ../dmac.h \
    ../drbg.h \
    ../donna.h \
    ../donna_32.h \
    ../donna_64.h \
    ../donna_sse.h \
    ../dsa.h \
    ../eax.h \
    ../ec2n.h \
    ../eccrypto.h \
    ../ecp.h \
    ../ecpoint.h \
    ../elgamal.h \
    ../emsa2.h \
    ../eprecomp.h \
    ../esign.h \
    ../factory.h \
    ../files.h \
    ../filters.h \
    ../fips140.h \
    ../fhmqv.h \
    ../fltrimpl.h \
    ../gcm.h \
    ../gf256.h \
    ../gf2_32.h \
    ../gf2n.h \
    ../gfpcrypt.h \
    ../gost.h \
    ../gzip.h \
    ../hashfwd.h \
    ../hc128.h \
    ../hc256.h \
    ../hex.h \
    ../hight.h \
    ../hkdf.h \
    ../hmac.h \
    ../hmqv.h \
    ../hrtimer.h \
    ../ida.h \
    ../idea.h \
    ../integer.h \
    ../iterhash.h \
    ../kalyna.h \
    ../keccak.h \
    ../lubyrack.h \
    ../lea.h \
    ../lsh.h \
    ../luc.h \
    ../mars.h \
    ../md2.h \
    ../md4.h \
    ../md5.h \
    ../mdc.h \
    ../mersenne.h \
    ../misc.h \
    ../modarith.h \
    ../modes.h \
    ../modexppc.h \
    ../mqueue.h \
    ../mqv.h \
    ../naclite.h \
    ../nbtheory.h \
    ../nr.h \
    ../oaep.h \
    ../oids.h \
    ../osrng.h \
    ../ossig.h \
    ../padlkrng.h \
    ../panama.h \
    ../pch.h \
    ../pkcspad.h \
    ../poly1305.h \
    ../polynomi.h \
    ../ppc_simd.h \
    ../pssr.h \
    ../pubkey.h \
    ../pwdbased.h \
    ../queue.h \
    ../rabin.h \
    ../randpool.h \
    ../rabbit.h \
    ../rc2.h \
    ../rc5.h \
    ../rc6.h \
    ../rdrand.h \
    ../resource.h \
    ../rijndael.h \
    ../ripemd.h \
    ../rng.h \
    ../rsa.h \
    ../rw.h \
    ../safer.h \
    ../salsa.h \
    ../scrypt.h \
    ../seal.h \
    ../secblock.h \
    ../secblockfwd.h \
    ../seckey.h \
    ../seed.h \
    ../serpent.h \
    ../serpentp.h \
    ../sha.h \
    ../sha1_armv4.h \
    ../sha256_armv4.h \
    ../sha3.h \
    ../sha512_armv4.h \
    ../shacal2.h \
    ../shake.h \
    ../shark.h \
    ../simple.h \
    ../simeck.h \
    ../simon.h \
    ../siphash.h \
    ../skipjack.h \
    ../sm3.h \
    ../sm4.h \
    ../smartptr.h \
    ../sosemanuk.h \
    ../speck.h \
    ../square.h \
    ../stdcpp.h \
    ../strciphr.h \
    ../tea.h \
    ../threefish.h \
    ../tiger.h \
    ../trap.h \
    ../trunhash.h \
    ../ttmac.h \
    ../tweetnacl.h \
    ../twofish.h \
    ../validate.h \
    ../vmac.h \
    ../wake.h \
    ../whrlpool.h \
    ../words.h \
    ../xed25519.h \
    ../xtr.h \
    ../xtrcrypt.h \
    ../xts.h \
    ../zdeflate.h \
    ../zinflate.h \
    ../zlib.h

# List cryptlib.cpp first, then cpu.cpp, then integer.cpp to tame C++ static initialization problems.
SOURCES += \
    ../cryptlib.cpp \
    ../cpu.cpp \
    ../integer.cpp \
    ../3way.cpp \
    ../adler32.cpp \
    ../algebra.cpp \
    ../algparam.cpp \
    ../allocate.cpp \
    ../arc4.cpp \
    ../aria.cpp \
    ../aria_simd.cpp \
    ../ariatab.cpp \
    ../asn.cpp \
    ../authenc.cpp \
    ../base32.cpp \
    ../base64.cpp \
    ../basecode.cpp \
    ../bfinit.cpp \
    ../blake2.cpp \
    ../blake2s_simd.cpp \
    ../blake2b_simd.cpp \
    ../blowfish.cpp \
    ../blumshub.cpp \
    ../camellia.cpp \
    ../cast.cpp \
    ../casts.cpp \
    ../cbcmac.cpp \
    ../ccm.cpp \
    ../chacha.cpp \
    ../chacha_simd.cpp \
    ../chacha_avx.cpp \
    ../chachapoly.cpp \
    ../cham.cpp \
    ../cham_simd.cpp \
    ../channels.cpp \
    ../cmac.cpp \
    ../crc.cpp \
    ../crc_simd.cpp \
    ../darn.cpp \
    ../default.cpp \
    ../des.cpp \
    ../dessp.cpp \
    ../dh.cpp \
    ../dh2.cpp \
    ../dll.cpp \
    ../donna_32.cpp \
    ../donna_64.cpp \
    ../donna_sse.cpp \
    ../dsa.cpp \
    ../eax.cpp \
    ../ec2n.cpp \
    ../ecp.cpp \
    ../eccrypto.cpp \
    ../eprecomp.cpp \
    ../elgamal.cpp \
    ../emsa2.cpp \
    ../eprecomp.cpp \
    ../esign.cpp \
    ../files.cpp \
    ../filters.cpp \
    ../fips140.cpp \
    ../gcm.cpp \
    ../gcm_simd.cpp \
    ../gf256.cpp \
    ../gf2_32.cpp \
    ../gf2n.cpp \
    ../gf2n_simd.cpp \
    ../gfpcrypt.cpp \
    ../gost.cpp \
    ../gzip.cpp \
    ../hc128.cpp \
    ../hc256.cpp \
    ../hex.cpp \
    ../hight.cpp \
    ../hmac.cpp \
    ../hrtimer.cpp \
    ../ida.cpp \
    ../idea.cpp \
    ../iterhash.cpp \
    ../kalyna.cpp \
    ../kalynatab.cpp \
    ../keccak.cpp \
    ../keccak_core.cpp \
    ../keccak_simd.cpp \
    ../lea.cpp \
    ../lea_simd.cpp \
    ../lsh256.cpp \
    ../lsh256_sse.cpp \
    ../lsh256_avx.cpp \
    ../lsh512.cpp \
    ../lsh512_sse.cpp \
    ../lsh512_avx.cpp \
    ../luc.cpp \
    ../mars.cpp \
    ../marss.cpp \
    ../md2.cpp \
    ../md4.cpp \
    ../md5.cpp \
    ../misc.cpp \
    ../modes.cpp \
    ../mqueue.cpp \
    ../mqv.cpp \
    ../nbtheory.cpp \
    ../neon_simd.cpp \
    ../oaep.cpp \
    ../osrng.cpp \
    ../padlkrng.cpp \
    ../panama.cpp \
    ../pch.cpp \
    ../pkcspad.cpp \
    ../poly1305.cpp \
    ../polynomi.cpp \
    ../power7_ppc.cpp \
    ../power8_ppc.cpp \
    ../power9_ppc.cpp \
    ../ppc_simd.cpp \
    ../pssr.cpp \
    ../pubkey.cpp \
    ../queue.cpp \
    ../rabin.cpp \
    ../randpool.cpp \
    ../rabbit.cpp \
    ../rc2.cpp \
    ../rc5.cpp \
    ../rc6.cpp \
    ../rdrand.cpp \
    ../rdtables.cpp \
    ../rijndael.cpp \
    ../rijndael_simd.cpp \
    ../ripemd.cpp \
    ../rng.cpp \
    ../rsa.cpp \
    ../rw.cpp \
    ../safer.cpp \
    ../salsa.cpp \
    ../scrypt.cpp \
    ../seal.cpp \
    ../seed.cpp \
    ../serpent.cpp \
    ../sha.cpp \
    ../sha_simd.cpp \
    ../sha3.cpp \
    ../shacal2.cpp \
    ../shacal2_simd.cpp \
    ../shake.cpp \
    ../shark.cpp \
    ../sharkbox.cpp \
    ../simeck.cpp \
    ../simon.cpp \
    ../simon128_simd.cpp \
    ../simple.cpp \
    ../skipjack.cpp \
    ../sm3.cpp \
    ../sm4.cpp \
    ../sm4_simd.cpp \
    ../sosemanuk.cpp \
    ../speck.cpp \
    ../speck128_simd.cpp \
    ../square.cpp \
    ../squaretb.cpp \
    ../sse_simd.cpp \
    ../strciphr.cpp \
    ../tea.cpp \
    ../tftables.cpp \
    ../threefish.cpp \
    ../tiger.cpp \
    ../tigertab.cpp \
    ../ttmac.cpp \
    ../tweetnacl.cpp \
    ../twofish.cpp \
    ../vmac.cpp \
    ../wake.cpp \
    ../whrlpool.cpp \
    ../xed25519.cpp \
    ../xtr.cpp \
    ../xtrcrypt.cpp \
    ../xts.cpp \
    ../zdeflate.cpp \
    ../zinflate.cpp \
    ../zlib.cpp

# OfficeCryptReader
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
HEADERS += \
    $$CORE_ROOT_DIR/OfficeCryptReader/source/ECMACryptFile.h \
    $$CORE_ROOT_DIR/OfficeCryptReader/source/CryptTransform.h

SOURCES += \
    $$CORE_ROOT_DIR/OfficeCryptReader/source/ECMACryptFile.cpp \
    $$CORE_ROOT_DIR/OfficeCryptReader/source/CryptTransform.cpp

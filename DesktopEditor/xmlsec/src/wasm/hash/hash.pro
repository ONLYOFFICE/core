QT -= core gui

TARGET = wasm
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += \
    TEST_AS_EXECUTABLE \
    OPENSSL_MD2_ENABLED

OPENSSL_DIR = $$PWD/../3rdParty/openssl

INCLUDEPATH += \
    $$OPENSSL_DIR/include \
    $$PWD

SOURCES += \
    $$OPENSSL_DIR/crypto/sha/keccak1600.c \
    $$OPENSSL_DIR/crypto/sha/sha1_one.c \
    $$OPENSSL_DIR/crypto/sha/sha1dgst.c \
    $$OPENSSL_DIR/crypto/sha/sha256.c \
    $$OPENSSL_DIR/crypto/sha/sha512.c

SOURCES += \
    $$OPENSSL_DIR/crypto/md2/md2_dgst.c \
    $$OPENSSL_DIR/crypto/md2/md2_one.c

SOURCES += \
    $$OPENSSL_DIR/crypto/md4/md4_dgst.c \
    $$OPENSSL_DIR/crypto/md4/md4_one.c

SOURCES += \
    $$OPENSSL_DIR/crypto/md5/md5_dgst.c \
    $$OPENSSL_DIR/crypto/md5/md5_one.c

SOURCES += \
    $$OPENSSL_DIR/crypto/ripemd/rmd_dgst.c \
    $$OPENSSL_DIR/crypto/ripemd/rmd_one.c

SOURCES += \
    $$OPENSSL_DIR/crypto/whrlpool/wp_block.c \
    $$OPENSSL_DIR/crypto/whrlpool/wp_dgst.c

SOURCES += \
    $$OPENSSL_DIR/crypto/mem_clr.c

#CONFIG += openssl_sha3

openssl_sha3 {

    DEFINES += OOXML_SUPPORT_SHA3_512

    SOURCES += \
        $$OPENSSL_DIR/crypto/evp/digest.c \
        $$OPENSSL_DIR/crypto/mem.c

    INCLUDEPATH += $$OPENSSL_DIR/include
    INCLUDEPATH += $$OPENSSL_DIR/include/openssl

}

# WASM EXPORT
SOURCES += main.cpp

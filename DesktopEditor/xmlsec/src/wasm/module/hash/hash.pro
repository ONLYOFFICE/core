QT -= core gui

TARGET = wasm
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += TEST_AS_EXECUTABLE

OPENSSL_DIR = $$PWD/../openssl

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

# WASM EXPORT
SOURCES += main.cpp

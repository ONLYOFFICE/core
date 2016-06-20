#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T12:43:02
#
#-------------------------------------------------

QT       -= core gui

VERSION = 1.0.0.4
TARGET = UnicodeConverter
TEMPLATE = lib

#CONFIG += staticlib
CONFIG += shared
CONFIG += c++11

QMAKE_CXXFLAGS += -fvisibility=hidden
QMAKE_CFLAGS += -fvisibility=hidden

linux-g++ | linux-g++-64 | linux-g++-32 {
    QMAKE_LFLAGS += -Wl,--rpath=./
}

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../build/lib
ICU_BUILDS_PLATFORM = mac

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
ICU_BUILDS_PLATFORM = win_64
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
ICU_BUILDS_PLATFORM = win_32
}

linux-g++ | linux-g++-64 | linux-g++-32:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
    ICU_BUILDS_PLATFORM = linux_64
}
linux-g++ | linux-g++-64 | linux-g++-32:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
    ICU_BUILDS_PLATFORM = linux_32
}

mac {
    DESTDIR = $$DESTINATION_SDK_PATH/mac_64
    ICU_BUILDS_PLATFORM = mac
}

################################################

LIBS_DESTDIR_PATH = $$DESTDIR
# теперь всегда с libstd
linux-g++ | linux-g++-64 | linux-g++-32 {
    CONFIG += static_link_libstd
}
static_link_libstd {
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
#    DESTDIR_POSTFIX = _static_stdlib
#    DESTDIR = $$DESTDIR$$DESTDIR_POSTFIX
    message(static_link_libstd)
}

############# dynamic dependencies #############
shared {
    DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY
}
################################################

linux-g++ | linux-g++-64 | linux-g++-32 {
    CONFIG += plugin
    TARGET_EXT = .so

    INCLUDEPATH += $$PWD/icubuilds/$$ICU_BUILDS_PLATFORM/usr/local/include
    LIBS        += $$PWD/../build/bin/icu/$$ICU_BUILDS_PLATFORM/libicuuc.so.55
    LIBS        += $$PWD/../build/bin/icu/$$ICU_BUILDS_PLATFORM/libicudata.so.55
    message(linux)
}

win32 {
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
    TARGET_EXT = .dll

    INCLUDEPATH += $$PWD/icubuilds/$$ICU_BUILDS_PLATFORM/include
    LIBS        += -L$$PWD/../build/bin/icu/$$ICU_BUILDS_PLATFORM -licuuc
    message(windows)
}

mac {
    CONFIG += plugin

    INCLUDEPATH += $$PWD/icubuilds/$$ICU_BUILDS_PLATFORM/release-55-1/source/common
    LIBS        += $$PWD/icubuilds/$$ICU_BUILDS_PLATFORM/release-55-1/source/lib/libicuuc.55.1.dylib
    LIBS        += $$PWD/icubuilds/$$ICU_BUILDS_PLATFORM/release-55-1/source/lib/libicudata.55.1.dylib
    message(mac)
}

SOURCES += \
    UnicodeConverter.cpp

HEADERS +=\
    UnicodeConverter.h

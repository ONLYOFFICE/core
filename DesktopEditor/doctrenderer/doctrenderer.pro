#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T10:22:14
#
#-------------------------------------------------

QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../build/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

mac {
    DESTDIR = $$DESTINATION_SDK_PATH/mac_64
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

win32 {
    V8_CHECKOUT_PATH = D:/GIT/v8/v8
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    V8_CHECKOUT_PATH = ../../../../v8
}

mac {
    V8_CHECKOUT_PATH = /Users/Oleg/Desktop/GIT/v8_mac/v8
}

win32 {
    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lShell32

    TARGET_EXT = .dll
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    CONFIG += plugin

    QMAKE_CXXFLAGS += -fvisibility=hidden
    QMAKE_CFLAGS += -fvisibility=hidden

    TARGET_EXT = .so
}

mac {
    CONFIG += plugin

    QMAKE_CXXFLAGS += -fvisibility=hidden -Wall -Wno-inconsistent-missing-override
    QMAKE_CFLAGS += -fvisibility=hidden -Wall -Wno-inconsistent-missing-override
}

INCLUDEPATH += \
    $$V8_CHECKOUT_PATH \
    $$V8_CHECKOUT_PATH/include

DEFINES += NEW_V8_ENGINE

#################### WINDOWS #####################
DESKTOPEDITOR_PATH = ..
include(../Qt_build/graphics/project/graphics_fonts.pri)

LIB_XML_PRI_PATH = ../xml
include(../xml/build/qt/libxml2.pri)

CONFIG += build_all_zlib build_zlib_as_sources
include(../../OfficeUtils/OfficeUtils.pri)

win32:contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -lv8_base_0 -lv8_base_1 -lv8_base_2 -lv8_base_3 -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -licui18n -licuuc
} else {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -lv8_base_0 -lv8_base_1 -lv8_base_2 -lv8_base_3 -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -licui18n -licuuc
}

    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -lv8_base_0 -lv8_base_1 -lv8_base_2 -lv8_base_3 -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -licui18n -licuuc
} else {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -lv8_base_0 -lv8_base_1 -lv8_base_2 -lv8_base_3 -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -licui18n -licuuc
}

    message(windows32)
}
##################################################

################### LINUX ########################

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT \

    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/tools/gyp -lv8_base -lv8_libplatform -lv8_libbase -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/third_party/icu -licui18n -licuuc -licudata
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    message(linux64)
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    message(linux32)
}

##################################################

#################### MAC #########################
mac {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _MAC \
    MAC \
    QT_MAC

    LIBS += -L$$V8_CHECKOUT_PATH/out/native -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_external_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/out/native -licui18n -licuuc -licudata

    message(mac)
}
##################################################

SOURCES += \
    memorystream.cpp \
    nativecontrol.cpp \
    doctrenderer.cpp \
    docbuilder.cpp

SOURCES += \
    ../../Common/OfficeFileFormatChecker2.cpp \
    ../../Common/3dParty/pole/pole.cpp

HEADERS += doctrenderer.h \
    docbuilder.h \
    docbuilder_p.h \
    memorystream.h \
    nativecontrol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

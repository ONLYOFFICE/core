#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T10:22:14
#
#-------------------------------------------------

QT       -= core
QT       -= gui

VERSION = 1.0.0.1
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../SDK/lib

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

win32 {
    V8_CHECKOUT_PATH = $$PWD/v8_windows/v8
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    V8_CHECKOUT_PATH = /home/oleg/v8
}

mac {
    V8_CHECKOUT_PATH = /Users/Oleg/Desktop/activex/v8
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

#################### WINDOWS #####################
DESKTOPEDITOR_PATH = ..
include(../Qt_build/graphics/project/graphics_fonts.pri)

LIB_XML_PRI_PATH = ../xml
include(../xml/build/qt/libxml2.pri)

win32:contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -licui18n -licuuc
} else {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -licui18n -licuuc
}

    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/build/Debug/lib -licui18n -licuuc
} else {
    LIBS += -L$$V8_CHECKOUT_PATH/build/Release/lib -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
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

    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/tools/gyp -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
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
    QT_MAC

    LIBS += -L$$V8_CHECKOUT_PATH/out/native -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/out/native -licui18n -licuuc -licudata

    message(mac)
}
##################################################

SOURCES +=  doctrenderer.cpp

HEADERS += doctrenderer.h \
    memorystream.h \
    nativecontrol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

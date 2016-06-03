#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T14:14:33
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = docbuilder
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++11

linux-g++ | linux-g++-64 | linux-g++-32 {
    QMAKE_LFLAGS += -Wl,--rpath=./
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
}

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../build/lib
DESTINATION_PATH = $$PWD/../../../build/bin/docbuilder

win32 {
DEFINES += WIN32
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT
}

mac {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _MAC \
    MAC
}

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_64
}
TARGET_PLATFORM = _win64
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_32
}
TARGET_PLATFORM = _win32
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/linux_64
    TARGET_PLATFORM = _linux64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/linux_32
    TARGET_PLATFORM = _linux32
}

mac {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/mac_64
    TARGET_PLATFORM = _mac64
}

DESTDIR = $$DESTINATION_PATH
TARGET = docbuilder$$TARGET_PLATFORM

################################################

LIBS += -L$$DESTINATION_SDK_PATH_DOCTRENDERER/docbuilder -ldoctrenderer

linux-g++ {
LIBS += -ldl
}

SOURCES += main.cpp

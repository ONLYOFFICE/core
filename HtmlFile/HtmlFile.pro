#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T16:30:42
#
#-------------------------------------------------

QT       -= core gui

VERSION = 1.0.0.3
TARGET = HtmlFile
TEMPLATE = lib

#CONFIG += staticlib
CONFIG += shared

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../build/lib

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

DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY
LIBS += -L$$DESTDIR -lUnicodeConverter

# теперь всегда с libstd
linux-g++ | linux-g++-64 | linux-g++-32 {
    CONFIG += static_link_libstd
}
static_link_libstd {
	DEFINES += asc_static_link_libstd
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
#    DESTDIR_POSTFIX = _static_stdlib
#    DESTDIR = $$DESTDIR$$DESTDIR_POSTFIX
    message(static_link_libstd)
}

############# dynamic dependencies #############
shared {
    DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY

    LIB_XML_PRI_PATH = ../DesktopEditor/xml
    include(../DesktopEditor/xml/build/qt/libxml2.pri)

    message(dynamic)

    win32 {
        TARGET_EXT = .dll
    }

    linux-g++ | linux-g++-64 | linux-g++-32 {
        CONFIG += plugin
        TARGET_EXT = .so

        QMAKE_CXXFLAGS += -fvisibility=hidden
        QMAKE_CFLAGS += -fvisibility=hidden
    }

    mac {
        CONFIG += plugin

        QMAKE_CXXFLAGS += -fvisibility=hidden
        QMAKE_CFLAGS += -fvisibility=hidden
    }
}
################################################

CONFIG += c++11

DEFINES += \
    _QT

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

win32 {
    DEFINES += \
    WIN32 \
    NOMINMAX
}

SOURCES += HtmlFile.cpp

HEADERS += HtmlFile.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

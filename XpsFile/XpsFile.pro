#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T12:43:02
#
#-------------------------------------------------

QT       -= core gui

VERSION = 1.0.0.1
TARGET = XpsFile
TEMPLATE = lib

#CONFIG += staticlib
CONFIG += shared

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../SDK/lib

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

############# dynamic dependencies #############
shared {
    DEFINES += XPS_USE_DYNAMIC_LIBRARY

    LIBS += -L$$DESTDIR -lPdfWriter

    CONFIG += build_all_zlib build_zlib_as_sources
    include(../OfficeUtils/OfficeUtils.pri)

    CONFIG += build_cximage_zlib_disable
    LIB_GRAPHICS_PRI_PATH = ../DesktopEditor
    include(../DesktopEditor/Qt_build/graphics/project/graphics.pri)

    LIB_XML_PRI_PATH = ../DesktopEditor/xml
    include(../DesktopEditor/xml/build/qt/libxml2.pri)

    message(dynamic)

    win32 {
        LIBS += -lgdi32 \
                -ladvapi32 \
                -luser32 \
                -lshell32

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

        QMAKE_CXXFLAGS += -fvisibility=hidden
        QMAKE_CFLAGS += -fvisibility=hidden
    }
} else {
    LIBS += -L$$DESTDIR -llibxml
}
################################################

CONFIG += c++11

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    HAVE_UNISTD_H \
    LINUX \
    _LINUX \
    _LINUX_QT

    message(linux)
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    HAVE_UNISTD_H \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _MAC \
    MAC

    message(mac)
}

DEFINES += \
    _QT \
    FT2_BUILD_LIBRARY \
    EXCLUDE_JPG_SUPPORT \
    MNG_SUPPORT_DISPLAY \
    MNG_SUPPORT_READ \
    MNG_SUPPORT_WRITE \
    MNG_ACCESS_CHUNKS \
    MNG_STORE_CHUNKS\
    MNG_ERROR_TELLTALE

win32 {
    DEFINES += \
    JAS_WIN_MSVC_BUILD \
    WIN32 \
    NOMINMAX

    message(windows)
}

INCLUDEPATH += \
    ../DesktopEditor/agg-2.4/include \
    ../DesktopEditor/freetype-2.5.2/include \
    ../DesktopEditor/cximage/jasper/include \
    ../DesktopEditor/cximage/jpeg \
    ../DesktopEditor/cximage/png \
    ../DesktopEditor/cximage/zlib

HEADERS += \
    XpsFile.h \
    XpsLib/ContextState.h \
    XpsLib/Document.h \
    XpsLib/FontList.h \
    XpsLib/Page.h \
    XpsLib/StaticResources.h \
    XpsLib/Utils.h \
    XpsLib/WString.h

SOURCES += \
    XpsFile.cpp \
    XpsLib/ContextState.cpp \
    XpsLib/Document.cpp \
    XpsLib/Page.cpp \
    XpsLib/StaticResources.cpp \
    XpsLib/Utils.cpp \
    XpsLib/WString.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

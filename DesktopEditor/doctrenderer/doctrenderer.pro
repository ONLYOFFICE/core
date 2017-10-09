QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../../Common/base.pri)

include(../Qt_build/graphics/project/graphics_fonts.pri)
include(../xml/build/qt/libxml2.pri)

CONFIG += build_all_zlib build_zlib_as_sources
include(../../OfficeUtils/OfficeUtils.pri)

include(../../Common/3dParty/v8/v8.pri)

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

# downloader
DEFINES += BUIDLER_OPEN_DOWNLOAD_ENABLED
DEFINES += BUIDLER_OPEN_BASE64_ENABLED

HEADERS += ../../Common/FileDownloader/FileDownloader.h
core_windows {
    SOURCES += \
        ../../Common/FileDownloader/FileDownloader_win.cpp

    LIBS += -lurlmon
}
core_linux {
    SOURCES += \
        ../../Common/FileDownloader/FileDownloader_curl.cpp

    LIBS += -lcurl
}
core_mac {
    OBJECTIVE_SOURCES += \
        ../../Common/FileDownloader/FileDownloader_mac.mm

    LIBS += -framework AppKit
}

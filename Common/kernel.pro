QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = kernel
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += OFFICEUTILS_USE_DYNAMIC_LIBRARY
DEFINES += XMLUTILS_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

CONFIG += build_all_zlib build_zlib_as_sources
include(../OfficeUtils/OfficeUtils.pri)

include(../DesktopEditor/xml/build/qt/libxml2.pri)

# DOWNLOADER
HEADERS += ./FileDownloader/FileDownloader.h
HEADERS += ./FileDownloader/FileDownloader_private.h

SOURCES += ./FileDownloader/FileDownloader.cpp

core_windows {
    SOURCES += \
        ./FileDownloader/FileDownloader_win.cpp

    LIBS += -lAdvapi32
    LIBS += -lurlmon
}
core_linux {
    SOURCES += \
        ./FileDownloader/FileDownloader_curl.cpp

    LIBS += -lcurl
}
core_mac {
    OBJECTIVE_SOURCES += \
        ./Common/FileDownloader/FileDownloader_mac.mm

    LIBS += -framework AppKit
}

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
HEADERS += \
    ./FileDownloader/FileDownloader.h \
    ./FileDownloader/FileDownloader_private.h

SOURCES += ./FileDownloader/FileDownloader.cpp

core_windows {
    SOURCES += \
        ./FileDownloader/FileDownloader_win.cpp

    LIBS += -lAdvapi32
    LIBS += -lurlmon
    LIBS += -lRpcrt4
    LIBS += -lShell32
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

# BLOCKER
DEFINES += CRITICALSECTION_USE_DYNAMIC_LIBRARY
HEADERS += \
    ./../DesktopEditor/graphics/TemporaryCS.h

SOURCES += \
    ./../DesktopEditor/graphics/TemporaryCS.cpp

# THREAD
DEFINES += BASETHREAD_USE_DYNAMIC_LIBRARY
HEADERS += \
    ./../DesktopEditor/graphics/BaseThread.h

SOURCES += \
    ./../DesktopEditor/graphics/BaseThread.cpp

# TIMER
DEFINES += TIMER_USE_DYNAMIC_LIBRARY
HEADERS += \
    ./../DesktopEditor/graphics/Timer.h

SOURCES += \
    ./../DesktopEditor/graphics/Timer.cpp

# PATH
DEFINES += PATH_USE_DYNAMIC_LIBRARY
HEADERS += ./../DesktopEditor/common/Path.h
SOURCES += ./../DesktopEditor/common/Path.cpp

# STRINGS
DEFINES += STRINGBUILDER_USE_DYNAMIC_LIBRARY
HEADERS += ./../DesktopEditor/common/StringBuilder.h
SOURCES += ./../DesktopEditor/common/StringBuilder.cpp

# BASE64
DEFINES += BASE64_USE_DYNAMIC_LIBRARY
HEADERS += ./../DesktopEditor/common/Base64.h
SOURCES += ./../DesktopEditor/common/Base64.cpp

# FILE
DEFINES += FILE_USE_DYNAMIC_LIBRARY
HEADERS += ./../DesktopEditor/common/File.h
SOURCES += ./../DesktopEditor/common/File.cpp

# DIRECTORY
DEFINES += DIRECTORY_USE_DYNAMIC_LIBRARY
HEADERS += ./../DesktopEditor/common/Directory.h
SOURCES += ./../DesktopEditor/common/Directory.cpp

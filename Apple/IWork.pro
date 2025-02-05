QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = IWorkFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

DEFINES += IWORK_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

INCLUDEPATH += \
	$$PWD

core_android:DEFINES += NOT_USE_PTHREAD_CANCEL USE_FILE32API

# BOOST
CONFIG += core_boost_regex
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

# ZLIB
CONFIG += build_all_zlib build_zlib_as_sources
include($$PWD/../OfficeUtils/OfficeUtils.pri)

# LIBXML
CONFIG += core_static_link_xml_full
CONFIG += core_only_libxml
include($$PWD/../DesktopEditor/xml/build/qt/libxml2.pri)

#
include($$CORE_ROOT_DIR/Common/3dParty/apple/apple.pri)

# TEST
HEADERS += $$ODF_LIB_ROOT/test/StringDocumentHandler.h
SOURCES += $$ODF_LIB_ROOT/test/StringDocumentHandler.cxx

SOURCES += IWork.cpp

HEADERS += IWork.h

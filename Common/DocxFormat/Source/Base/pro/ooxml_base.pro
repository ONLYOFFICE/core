QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = ooxml_base
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
include($$PWD/ooxml_base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel)

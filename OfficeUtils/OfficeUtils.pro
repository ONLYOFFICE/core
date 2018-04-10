QT       -= core gui

TARGET = OfficeUtils
TEMPLATE = lib
CONFIG += staticlib

win32 {
    CONFIG += build_all_zlib
}

CONFIG += build_all_zlib

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)
include($$PWD/OfficeUtils.pri))

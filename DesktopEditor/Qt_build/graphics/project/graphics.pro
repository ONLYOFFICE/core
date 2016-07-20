QT       -= core gui

TARGET = graphics
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include(../../../../Common/base.pri)

include($$PWD/graphics.pri)

#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T18:33:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = hunspell
TEMPLATE = lib

CONFIG += staticlib
CONFIG += building_hunspell

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

include($$PWD/hunspell.pri)


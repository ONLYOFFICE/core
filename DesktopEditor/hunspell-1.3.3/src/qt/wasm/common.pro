TARGET = common
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += HUNSPELL_STATIC
DEFINES += BUILDING_LIBHUNSPELL

DEFINES += HUNSPELL_WASM_MODULE

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIBHUNSPELL_SRC_PATH = $$PWD/../..

# LIBHUNSPELL SOURCES
INCLUDEPATH += \
    $$LIBHUNSPELL_SRC_PATH/hunspell \
    $$LIBHUNSPELL_SRC_PATH/win_api

SOURCES += \
    $$LIBHUNSPELL_SRC_PATH/hunspell/affentry.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/affixmgr.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/csutil.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/dictmgr.cxx \
#    $$LIBHUNSPELL_SRC_PATH/hunspell/filemgr.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hashmgr.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hunspell.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hunzip.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/phonet.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/replist.cxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/suggestmgr.cxx

HEADERS += \
    $$LIBHUNSPELL_SRC_PATH/hunspell/affentry.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/affixmgr.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/atypes.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/baseaffix.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/csutil.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/dictmgr.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/filemgr.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hashmgr.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/htypes.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hunspell.h \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hunspell.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/hunzip.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/langnum.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/phonet.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/replist.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/suggestmgr.hxx \
    $$LIBHUNSPELL_SRC_PATH/hunspell/w_char.hxx \
    $$LIBHUNSPELL_SRC_PATH/win_api/config.h

# LIBHUNSPELL WASM CORRECTION
#SOURCES -= $$LIBHUNSPELL_SRC_PATH/hunspell/filemgr.cxx
SOURCES += ./src/filemgr_wrapper.cxx

HEADERS += ./src/engine.h
HEADERS += ./src/base.h
SOURCES += ./src/base.cpp
SOURCES += ./src/main.cpp



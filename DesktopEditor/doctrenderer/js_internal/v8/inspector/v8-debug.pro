CORE_BUILDS_PLATFORM_PREFIX = win_64
BUILD_MODE=debug

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
THIRDPARTYPATH = $$CORE_ROOT_DIR/Common/3dParty

V8PATH = $$THIRDPARTYPATH/v8

BOOST_PATH = $$THIRDPARTYPATH/boost
BOOST_BUILD_PATH = $$BOOST_PATH/build/$$CORE_BUILDS_PLATFORM_PREFIX
BOOST_LIB_PATH = $$BOOST_BUILD_PATH/lib

CORE_V8_PATH_INCLUDE    = $$V8PATH/v8
CORE_V8_PATH_LIBS = $$CORE_V8_PATH_INCLUDE/out.gn/$$CORE_BUILDS_PLATFORM_PREFIX/$$BUILD_MODE/obj

INCLUDEPATH += \
    $$CORE_V8_PATH_INCLUDE \
    $$CORE_V8_PATH_INCLUDE/include \
    $$BOOST_BUILD_PATH/include


LIBS += -L$$BOOST_LIB_PATH -llibboost_date_time-vc140-mt-gd-x64-1_72

LIBS += -L$$CORE_V8_PATH_LIBS -lv8_base -lv8_libplatform -lv8_libbase -lv8_snapshot -lv8_libsampler
LIBS += -L$$CORE_V8_PATH_LIBS/third_party/icu -licui18n -licuuc
LIBS += -L$$CORE_V8_PATH_LIBS/src/inspector -linspector

LIBS += -lwinmm
LIBS += -ladvapi32
LIBS += -lShell32
LIBS += -lDbgHelp
LIBS += -lShlwapi


#inspector stuff
HEADERS += \
    channel.h \
    client.h \
    execution_data.h \
    inspector.h \
    inspector_impl.h \
    inspectorinfo.h \
    inspectormanager.h \
    scriptholder.h \
    singleconnectionserver.h \
    singlethreadutils.h

SOURCES += \
        channel.cpp \
        client.cpp \
        execution_data.cpp \
        inspector.cpp \
        inspector_impl.cpp \
        inspectormanager.cpp \
        main.cpp \
        scriptholder.cpp \
        singleconnectionserver.cpp \
        singlethreadutils.cpp

#v8 base stuff
HEADERS += \
    ../v8_base.h \
    ../../js_base.h

SOURCES += \
    ../v8_base.cpp

#kernel stuff: for thread id
#DESKTOP_EDITOR_PATH = $$PWD/../../../..
#HEADERS += \
#    $$DESKTOP_EDITOR_PATH/graphics/BaseThread.h
#SOURCES += \
#    $$DESKTOP_EDITOR_PATH/graphics/BaseThread.cpp


include($$CORE_ROOT_DIR/Common/base.pri)
ADD_DEPENDENCY(kernel, UnicodeConverter)
DEFINES += V8_INSPECTOR

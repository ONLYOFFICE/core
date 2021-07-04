CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

include($$CORE_ROOT_DIR/Common/3dParty/v8/v8.pri)

# add inspector library
LIBS += -L$$CORE_V8_PATH_LIBS/src/inspector -linspector

ADD_DEPENDENCY(kernel, kernel_network)

HEADERS += \
    cabstractdebugsocketlistener.h \
    cchannel.h \
    cinspectorclient.h \
    common.h \
    cwebsocketlistener.h

SOURCES += \
    cchannel.cpp \
    cinspectorclient.cpp \
    common.cpp \
    cwebsocketlistener.cpp \
    main.cpp

#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T10:22:14
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = doctrenderer
TEMPLATE = lib

CONFIG += static
CONFIG += shared
CONFIG += plugin

QMAKE_CXXFLAGS += -std=c++11

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += _USE_LIBXML2_READER_
DEFINES += LIBXML_READER_ENABLED

V8_CHECKOUT_PATH = ../../../../../../../v8_trunk
V8_BUILD_LIBRARY_PATH_WINDOWS = ../../../SDK/lib
QT_SETUP_INCLUDES_PATH = C:/Qt/Qt5.4.1/5.4/msvc2013_64_opengl/include

INCLUDEPATH += \
    $$V8_CHECKOUT_PATH \
    $$V8_CHECKOUT_PATH/include

#################### WINDOWS #####################
win32 {
    DEFINES += \
    WIN32

    LIBS += -lwinmm
    LIBS += -ladvapi32
}

win32:contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L../../Qt_build/graphics/Debug/debug -lgraphics

    LIBS += -L../../../SDK/lib/win_64/DEBUG -llibxml2

    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_64/DEBUG -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_64/DEBUG -licui18n -licuuc
} else {
    LIBS += -L../../Qt_build/graphics/Release/release -lgraphics

    LIBS += -L../../../SDK/lib/win_64 -llibxml2

    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_64 -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_64 -licui18n -licuuc
}

    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L../../Qt_build/graphics/Debug/debug -lgraphics

    LIBS += -L../../../SDK/lib/win_32/DEBUG -llibxml2

    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_32/DEBUG -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_32/DEBUG -licui18n -licuuc
} else {
    LIBS += -L../../Qt_build/graphics/Release/release -lgraphics

    LIBS += -L../../../SDK/lib/win_32 -llibxml2

    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_32 -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_BUILD_LIBRARY_PATH_WINDOWS/win_32 -licui18n -licuuc
}

    message(windows32)
}
##################################################

################### LINUX ########################

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT \

    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/tools/gyp -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/third_party/icu -licui18n -licuuc -licudata

    LIBS += -L../../Qt_build/graphics/Release -lgraphics
    LIBS += -L../../../Common/DocxFormat/Source/XML/libxml2/linux_build/Release -llibxml2
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    message(linux64)
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    message(linux32)
}

##################################################

#################### MAC #########################
mac {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _MAC \
    QT_MAC

    #TODO: graphics & libxml2

    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/tools/gyp -lv8_base -lv8_libbase -lv8_libplatform -lv8_nosnapshot -lv8_snapshot
    LIBS += -L$$V8_CHECKOUT_PATH/out/native/obj.target/third_party/icu -licui18n -licuuc -licudata

    message(mac)
}
##################################################

INCLUDEPATH += \
            ../agg-2.4/include \
            ../freetype-2.5.2/include \
            ../../../Common/DocxFormat/Source/XML/libxml2/XML/include

INCLUDEPATH += $$QT_SETUP_INCLUDES_PATH

SOURCES +=  doctrenderer.cpp \
            ../../../Common/DocxFormat/Source/XML/libxml2/libxml2.cpp \
            ../../../Common/DocxFormat/Source/XML/stringcommon.cpp \
            ../../../Common/DocxFormat/Source/Base/unicode_util.cpp

HEADERS += doctrenderer.h \
    memorystream.h \
    nativecontrol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

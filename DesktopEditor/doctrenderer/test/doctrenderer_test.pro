#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T14:14:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = doctrenderer_test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += _UNICODE
DEFINES += _USE_LIBXML2_READER_
DEFINES += LIBXML_READER_ENABLED

TEMPLATE = app

CONFIG(debug, debug|release) {
    LIBS += -L../../Debug/debug -ldoctrenderer
    message(debug)
} else {
    LIBS += -L../../Release/release -ldoctrenderer
    message(release)
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_base.x64
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_snapshot
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_nosnapshot.x64
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licui18n
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licuuc
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licudata

    message(linux64)
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_base.ia32
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_snapshot
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_nosnapshot.ia32
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licui18n
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licuuc
    #LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licudata

    message(linux32)
}

mac {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _MAC \
    QT_MAC

    LIBS += -L../../../../../../../../../v8/out/native -lv8_base.x64
    LIBS += -L../../../../../../../../../v8/out/native -lv8_snapshot
    LIBS += -L../../../../../../../../../v8/out/native -lv8_nosnapshot.x64
    LIBS += -L../../../../../../../../../v8/out/native -licui18n
    LIBS += -L../../../../../../../../../v8/out/native -licuuc
    LIBS += -L../../../../../../../../../v8/out/native -licudata

    message(mac)
}

win32 {
    DEFINES += \
    WIN32

    DEFINES -= UNICODE

    LIBS += -lwinmm
    LIBS += -ladvapi32
}

win32:contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -lgraphics

    LIBS += -L../../../../SDK/lib/win_64/DEBUG -llibxml2

    LIBS += -L../../../../SDK/lib/win_64/DEBUG -lv8_libbase
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -lv8_base
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -lv8_nosnapshot
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -lv8_snapshot
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -lv8_libplatform
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -licui18n
    LIBS += -L../../../../SDK/lib/win_64/DEBUG -licuuc
} else {
    LIBS += -L../../../../SDK/lib/win_64 -lgraphics

    LIBS += -L../../../../SDK/lib/win_64 -llibxml2

    LIBS += -L../../../../SDK/lib/win_64 -lv8_libbase
    LIBS += -L../../../../SDK/lib/win_64 -lv8_base
    LIBS += -L../../../../SDK/lib/win_64 -lv8_nosnapshot
    LIBS += -L../../../../SDK/lib/win_64 -lv8_snapshot
    LIBS += -L../../../../SDK/lib/win_64 -lv8_libplatform
    LIBS += -L../../../../SDK/lib/win_64 -licui18n
    LIBS += -L../../../../SDK/lib/win_64 -licuuc
}

    message(windows64)
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{

CONFIG(debug, debug|release) {
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -lgraphics

    LIBS += -L../../../../SDK/lib/win_32/DEBUG -llibxml2

    LIBS += -L../../../../SDK/lib/win_32/DEBUG -lv8_libbase
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -lv8_base
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -lv8_nosnapshot
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -lv8_snapshot
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -lv8_libplatform
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -licui18n
    LIBS += -L../../../../SDK/lib/win_32/DEBUG -licuuc
} else {
    LIBS += -L../../../../SDK/lib/win_32 -lgraphics

    LIBS += -L../../../../SDK/lib/win_32 -llibxml2

    LIBS += -L../../../../SDK/lib/win_32 -lv8_libbase
    LIBS += -L../../../../SDK/lib/win_32 -lv8_base
    LIBS += -L../../../../SDK/lib/win_32 -lv8_nosnapshot
    LIBS += -L../../../../SDK/lib/win_32 -lv8_snapshot
    LIBS += -L../../../../SDK/lib/win_32 -lv8_libplatform
    LIBS += -L../../../../SDK/lib/win_32 -licui18n
    LIBS += -L../../../../SDK/lib/win_32 -licuuc
}

    message(windows32)
}

INCLUDEPATH += \
    ../../../agg-2.4/include \
    ../../../freetype-2.5.2/include \
    ../../../../../../../v8_trunk \
    ../../../../../../../v8_trunk/include \
    ../../../Common/DocxFormat/Source/XML/libxml2/XML/include


SOURCES += main.cpp \
    ../../../Common/DocxFormat/Source/XML/libxml2/libxml2.cpp \
    ../../../Common/DocxFormat/Source/XML/stringcommon.cpp

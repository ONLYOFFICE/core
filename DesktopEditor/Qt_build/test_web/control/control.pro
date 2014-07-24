#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T17:09:03
#
#-------------------------------------------------

QT       += core gui opengl webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = control
TEMPLATE = app

DEFINES += \
    _QT


CONFIG(debug, debug|release) {
    LIBS += -L../../editor/Debug/debug -lnative_control
    LIBS += -L../../graphics/Debug/debug -lgraphics
    message(debug)
} else {
    LIBS += -L../../editor/Release/release -lnative_control
    LIBS += -L../../graphics/Release/release -lgraphics
    message(release)
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT

    LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_base.ia32
    LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_snapshot
    LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_nosnapshot.ia32
    LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licui18n
    LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licuuc
    LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licudata

    message(linux)
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
    LIBS += -L../../../../../../../../../v8/build/Release/lib -lv8_base.x64
    LIBS += -L../../../../../../../../../v8/build/Release/lib -lv8_snapshot
    LIBS += -L../../../../../../../../../v8/build/Release/lib -lv8_nosnapshot.x64
    LIBS += -L../../../../../../../../../v8/build/Release/lib -licui18n
    LIBS += -L../../../../../../../../../v8/build/Release/lib -licuuc

    message(windows)
}

INCLUDEPATH += \
    ../../../agg-2.4/include \
    ../../../freetype-2.5.2/include \
    .../../../../../../../../../v8/include

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    NativeControl.h

FORMS    += mainwindow.ui

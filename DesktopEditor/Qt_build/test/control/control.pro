#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T17:09:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = control
TEMPLATE = app

DEFINES += \
    LINUX \
    _LINUX \
    _LINUX_QT \
    _QT \
    _QTX

INCLUDEPATH += \
    ../../../agg-2.4/include \
    ../../../freetype-2.5.2/include \
    ../../../../../../../../../v8/include

CONFIG(debug, debug|release) {
LIBS += -lX11
LIBS += -L../../editor/Debug -lnative_control
LIBS += -L../../graphics/Debug -lgraphics
LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_base.ia32
LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_snapshot
LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_nosnapshot.ia32
LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licui18n
LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licuuc
LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licudata
message(debug)
} else {
LIBS += -L../../editor/Release -lnative_control
LIBS += -L../../graphics/Release -lgraphics
LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_base.ia32
LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_snapshot
LIBS += -L../../../../../../../../../v8/out/native/obj.target/tools/gyp -lv8_nosnapshot.ia32
LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licui18n
LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licuuc
LIBS += -L../../../../../../../../../v8/out/native/obj.target/third_party/icu -licudata
message(release)
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    NativeControl.h

FORMS    += mainwindow.ui

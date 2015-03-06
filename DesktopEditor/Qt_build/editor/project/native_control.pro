#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T18:33:57
#
#-------------------------------------------------

QT       += core gui
QT  += widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = native_control
TEMPLATE = lib
CONFIG += staticlib
DEFINES += \
    _QT

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    _LINUX \
    _LINUX_QT

    message(linux)
}

mac {
    DEFINES += \
    _LINUX \
    _LINUX_QT \
    _MAC \
    QT_MAC

    message(mac)
}

win32 {
    DEFINES += \
    WIN32

    DEFINES -= UNICODE

    message(windows)
}

INCLUDEPATH += \
    ../../../agg-2.4/include \
    ../../../freetype-2.5.2/include \
    ../../../../../../../../../v8/include

SOURCES += \
    ../../../editor/v8_core/DrawingDocumentWrapper.cpp \
    ../../../editor/v8_core/HitControl.cpp \
    ../../../editor/v8_core/JSEngine2.cpp \
    ../../../editor/v8_core/OverlayTracker.cpp \
    ../../../editor/v8_core/PageDrawer.cpp \
    ../../../editor/v8_core/TextMeasurer2.cpp \
    ../../../editor/v8_core/v8_jscore.cpp \
    ../../../editor/CacheDocument.cpp \
    ../../../editor/Control.cpp \
    ../../../editor/DibPainter.cpp \
    ../../../editor/JSEngine.cpp \
    ../../../editor/Overlay.cpp \
    ../../../editor/Rulers.cpp \
    ../../../editor/TextMeasurer.cpp \
    ../../../editor/Viewer.cpp \
    NativeControl.cpp

linux-g++ | linux-g++-64 | linux-g++-32 {
    SOURCES += ../../../editor/GLplatform/gl_linux_qt.cpp
}


HEADERS += \
    ../../../editor/Buttons.h \
    ../../../editor/CacheDocument.h \
    ../../../editor/Control.h \
    ../../../editor/Controllers.h \
    ../../../editor/DibPainter.h \
    ../../../editor/DrawingDocument.h \
    ../../../editor/DrawingDocumentWrapper.h \
    ../../../editor/HitControl.h \
    ../../../editor/JSCore.h \
    ../../../editor/JSEngine.h \
    ../../../editor/MainControl.h \
    ../../../editor/MemoryStream.h \
    ../../../editor/Overlay.h \
    ../../../editor/OverlayTracker.h \
    ../../../editor/PageDrawer.h \
    ../../../editor/Rulers.h \
    ../../../editor/Scroll.h \
    ../../../editor/TextMeasurer.h \
    ../../../editor/v8_classes.h \
    ../../../editor/VideoFrameBuffer.h \
    ../../../editor/Viewer.h \
    NativeControl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=

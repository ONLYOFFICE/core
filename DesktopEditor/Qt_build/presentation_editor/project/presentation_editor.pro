#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T14:56:40
#
#-------------------------------------------------

QT       += core gui
QT       += widgets opengl



TARGET = presentation_editor
TEMPLATE = lib
CONFIG += staticlib


CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../Debug
}else {
    DESTDIR = $$PWD/../Release
}

INCLUDEPATH += \
    ../../../agg-2.4/include \
    ../../../freetype-2.5.2/include \
    ../../../doctrenderer/v8_windows/v8/ \
    ../../../doctrenderer/v8_windows/v8/include \
    ../../../doctrenderer/v8_windows/v8/include/libplatform

DEFINES += \
    _QT
DEFINES += \
    WIN32

SOURCES += NativeControl.cpp \
    ../../../presentation_editor/Control.cpp \
    ../../../presentation_editor/DrawingDocument.cpp \
    ../../../presentation_editor/Overlay.cpp \
    ../../../presentation_editor/Viewer.cpp \
    ../../../presentation_editor/v8_core/HitControl.cpp \
    ../../../presentation_editor/v8_core/JSEngine2.cpp \
    ../../../presentation_editor/v8_core/TextMeasurer2.cpp \
    ../../../presentation_editor/v8_core/v8_jscore.cpp \
    ../../../editor/DibPainter.cpp \
    ../../../editor/OverlayBase.cpp \
    ../../../editor/JSEngine.cpp \
    ../../../presentation_editor/v8_core/DrawingDocumentWrapper.cpp \
    ../../../presentation_editor/v8_core/PageDrawer.cpp \
    ../../../presentation_editor/v8_core/OverlayTracker.cpp \
    ../../../editor/TextMeasurer.cpp \
    ../../../editor/Rulers.cpp \
    ../../../fontengine/ApplicationFonts.cpp \
    ../../../fontengine/ApplicationFontsWorker.cpp \
    ../../../fontengine/FontFile.cpp \
    ../../../fontengine/FontManager.cpp \
    ../../../fontengine/FontPath.cpp \
    ../../../fontengine/GlyphString.cpp

HEADERS += \
    NativeControl.h \
    ../../../presentation_editor/Control.h \
    ../../../presentation_editor/DrawingDocument.h \
    ../../../presentation_editor/EditorCtrlWrapper.h \
    ../../../presentation_editor/Overlay.h \
    ../../../presentation_editor/VideoFrameBuffer.h \
    ../../../presentation_editor/Viewer.h \
    ../../../presentation_editor/v8_core/v8_jscore.h \
    ../../../editor/v8_classes.h \
    ../../../editor/DibPainter.h \
    ../../../editor/PageDrawer.h \
    ../../../presentation_editor/thumbnails.h \
    ../../../presentation_editor/notes.h \
    ../../../fontengine/FontManager.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

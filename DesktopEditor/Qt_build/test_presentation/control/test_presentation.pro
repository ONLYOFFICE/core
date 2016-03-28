#-------------------------------------------------
#
# Project created by QtCreator 2016-01-20T11:11:24
#
#-------------------------------------------------

QT       += core gui opengl
TARGET = test_presentation
TEMPLATE = app

DEFINES += \
WIN32

DEFINES -= UNICODE
DEFINES += _NOT_DRAW_VISUAL_CONTROLS_GL_

LIBS += -lwinmm
LIBS += -ladvapi32
LIBS += -lopengl32
LIBS += -lgdi32
LIBS += -luser32



CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../Debug
}else {
    DESTDIR = $$PWD/../Release
}

CONFIG(debug, debug|release) {
    LIBS += -L../../../../build/lib/win_64/DEBUG -lv8_base
    LIBS += -L../../../../build/lib/win_64/DEBUG -lv8_libbase
    LIBS += -L../../../../build/lib/win_64/DEBUG -lv8_libplatform
    LIBS += -L../../../../build/lib/win_64/DEBUG -lv8_nosnapshot
    LIBS += -L../../../../build/lib/win_64/DEBUG -lv8_snapshot
    LIBS += -L../../../../build/lib/win_64/DEBUG -licui18n
    LIBS += -L../../../../build/lib/win_64/DEBUG -licuuc
    LIBS += -L../../../../build/lib/win_64/DEBUG -lgraphics
    LIBS += -L../../../../build/lib/win_64/DEBUG -lOfficeUtils
    LIBS += -L../../presentation_editor/Debug -lpresentation_editor


    message("debug")
}else {

    LIBS += -L../../../../build/lib/win_64 -lv8_base
    LIBS += -L../../../../build/lib/win_64 -lv8_libbase
    LIBS += -L../../../../build/lib/win_64 -lv8_libplatform
    LIBS += -L../../../../build/lib/win_64 -lv8_nosnapshot
    LIBS += -L../../../../build/lib/win_64 -lv8_snapshot
    LIBS += -L../../../../build/lib/win_64 -licui18n
    LIBS += -L../../../../build/lib/win_64 -licuuc
    LIBS += -L../../../../build/lib/win_64 -lgraphics
    LIBS += -L../../../../build/lib/win_64 -lOfficeUtils
    LIBS += -L../../presentation_editor/Release -lpresentation_editor
    message("release")
}


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    nativecontrol.h

FORMS    += mainwindow.ui

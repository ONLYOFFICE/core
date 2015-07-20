#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T10:22:14
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = aschtmlrenderer
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11

#################### WINDOWS #####################
win32 {
    DEFINES += \
    WIN32

    CONFIG(debug, debug|release) {
        LIBS += -L../../../DesktopEditor/Qt_build/graphics/Debug/debug -lgraphics
    } else {
        LIBS += -L../../../DesktopEditor/Qt_build/graphics/Release/release -lgraphics
    }
}
##################################################

################### LINUX ########################

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT \

    LIBS += -L../../../DesktopEditor/Qt_build/graphics/Release -lgraphics
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    message(linux64)
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    message(linux32)
}

##################################################

INCLUDEPATH += \
            ../../DesktopEditor/agg-2.4/include \
            ../../DesktopEditor/freetype-2.5.2/include

SOURCES +=  \
    src/HTMLRenderer3.cpp

HEADERS +=  \
    src/CanvasWriter.h \
    src/Common.h \
    src/Document.h \
    src/FontManager.h \
    src/FontManagerBase.h \
    src/SVGWriter.h \
    src/SVGWriter2.h \
    src/Text.h \
    src/VectorGraphicsWriter.h \
    src/VectorGraphicsWriter2.h \
    src/VMLWriter.h \
    src/Writer.h \
    include/HTMLRenderer3.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

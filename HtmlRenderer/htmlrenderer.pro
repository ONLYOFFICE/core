#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T10:22:14
#
#-------------------------------------------------

QT       -= core
QT       -= gui

VERSION = 1.0.0.1
TARGET = HtmlRenderer
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

#CONFIG += staticlib
CONFIG += shared

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

################################################

############# dynamic dependencies #############
shared {
    DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

    LIBS += -L$$DESTDIR -lASCOfficeUtilsLib
    LIBS += -L$$DESTDIR -lgraphics

    include(../DesktopEditor/xml/build/qt/libxml2.pri)

    message(dynamic)

    win32 {
        LIBS += -lgdi32 \
                -ladvapi32 \
                -luser32 \
                -lshell32

        TARGET_EXT = .dll
    }

    linux-g++ | linux-g++-64 | linux-g++-32 {
        CONFIG += plugin

        QMAKE_CXXFLAGS += -fvisibility=hidden
        QMAKE_CFLAGS += -fvisibility=hidden

        TARGET_EXT = .so
    }
} else {
    LIBS += -L$$DESTDIR -llibxml
}
################################################

#################### WINDOWS #####################
win32 {
    DEFINES += \
    WIN32
}
##################################################

################### LINUX ########################

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT
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

#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCHTMLRendererLib
TEMPLATE = lib
CONFIG += staticlib
win32 {
    QMAKE_CXXFLAGS += -std=c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}
############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../SDK/lib

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
############### destination path ###############

DEFINES +=  UNICODE \
            _UNICODE \
            NODOCX \
            DESKTOP_EDITOR_GRAPHICS

#################### LINUX ########################
linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT
}
#################### LINUX ########################

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include \
    ../../DesktopEditor/agg-2.4/include
	
SOURCES += ../ASCSVGWriter.cpp \
    ../CASCSVGRenderer.cpp \
    ../CMetafile.cpp \
    ../CASCImage.cpp

HEADERS += ../ASCSVGWriter.h \
    ../CASCImage.h \
    ../CASCSVGRenderer.h \
    ../CMetafile.h \
    ../Writer/Common.h \
    ../Writer/Const.h \
    ../IBaseMatrixUpdater.h \
    ../Writer/TextItem.h \
    ../Writer/StringWriter.h \
    ../Writer/SVGWriter.h \
    ../Writer/VectorGraphicsWriter2.h \
    ../Writer/VMLWriter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


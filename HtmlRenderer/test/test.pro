#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY
DEFINES += XPS_USE_DYNAMIC_LIBRARY
DEFINES += DJVU_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include

TEMPLATE = app

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_32
}

LIBS += -L$$DESTINATION_SDK_PATH -lgraphics
LIBS += -L$$DESTINATION_SDK_PATH -lHtmlRenderer
LIBS += -L$$DESTINATION_SDK_PATH -lPdfReader
LIBS += -L$$DESTINATION_SDK_PATH -lDjVuFile
LIBS += -L$$DESTINATION_SDK_PATH -lXpsFile
LIBS += -L$$DESTINATION_SDK_PATH -lPdfWriter

win32 {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

SOURCES += main.cpp

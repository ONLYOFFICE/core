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

TEMPLATE = app

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH_DOCTRENDERER = $$DESTINATION_SDK_PATH/linux_32
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/Debug
} else {
    DESTDIR = $$PWD/Release
}

################################################

LIBS += -L$$DESTINATION_SDK_PATH_DOCTRENDERER -ldoctrenderer

#################### PDF ######################
#DEFINES += USE_PDF_RENDERER
#
#DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY
#LIBS += -L$$DESTINATION_SDK_PATH_DOCTRENDERER -lPdfWriter
#
#INCLUDEPATH += \
#    ../../agg-2.4/include \
#    ../../freetype-2.5.2/include \
#    ../../cximage/jasper/include \
#    ../../cximage/jpeg \
#    ../../cximage/png \
#    ../../cximage/zlib
#LIBS += -L$$DESTINATION_SDK_PATH_DOCTRENDERER -lgraphics
#win32 {
#    LIBS += -lShell32 -lAdvapi32
#}
###############################################

SOURCES += main.cpp

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

CORE_ROOT_DIR = $$PWD/../../../
PWD_ROOT_DIR = $$PWD
include(../../../Common/base.pri)

################################################

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -ldoctrenderer

linux-g++ {
LIBS += -ldl
}

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

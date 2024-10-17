QT       -= core
QT       -= gui

TARGET = docbuilder
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

CUSTOM_VERSION_PATH = $$(DOCBUILDER_VERSION_PATH)
isEmpty(CUSTOM_VERSION_PATH){
    CUSTOM_VERSION_PATH=.
}

core_windows {
    RC_FILE = $$CUSTOM_VERSION_PATH/version.rc
}

############### destination path ###############
DESTDIR = $$CORE_BUILDS_BINARY_PATH
################################################

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer, PdfFile, XpsFile, DjVuFile, DocxRenderer)

core_linux {
    LIBS += -ldl
}

SOURCES += main.cpp

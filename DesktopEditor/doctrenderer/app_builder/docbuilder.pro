QT       -= core
QT       -= gui

TARGET = docbuilder
CONFIG   += console
CONFIG   -= app_bundle

PRODUCT_VERSION=$$(PRODUCT_VERSION)
BUILD_NUMBER=$$(BUILD_NUMBER)

isEmpty(PRODUCT_VERSION) {
    BINARYVERSION = 0.0.0.0
}
else {
    BINARYVERSION = $$(PRODUCT_VERSION).$$(BUILD_NUMBER)
}

DEFINES += INTVER=$$BINARYVERSION

TEMPLATE = app

CONFIG += core_static_link_libstd

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

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer)

core_linux {
    LIBS += -ldl
}

SOURCES += main.cpp

QT       -= core gui

TARGET = names
TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += \
    BUILD_FONT_NAMES_DICTIONARY

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include(../../../Common/base.pri)

FREETYPE_LIB_PATH=$$PWD/../freetype-2.5.3

INCLUDEPATH += \
    $$FREETYPE_LIB_PATH/include

LIBS += -L$$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX -lfreetype

DESTDIR=$$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

SOURCES += \
    FontMaps.cpp

#HEADERS += FontDictionary.h

HEADERS += \
    $$PWD/../../fontengine/ApplicationFonts.h

SOURCES += \
    $$PWD/../../fontengine/ApplicationFonts.cpp \
    $$PWD/../../fontengine/FontFile.cpp \
    $$PWD/../../fontengine/FontManager.cpp \
    $$PWD/../../fontengine/GlyphString.cpp \
    $$PWD/../../fontengine/FontPath.cpp

win32 {
LIBS += -ladvapi32 \
        -luser32 \
        -lshell32
}


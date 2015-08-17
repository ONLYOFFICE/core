#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T12:43:02
#
#-------------------------------------------------

QT       -= core gui

VERSION = 1.0.0.3
TARGET = PdfWriter
TEMPLATE = lib

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
    DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY

    LIBS += -L$$DESTDIR -lASCOfficeUtilsLib

    #LIBS += -L$$DESTDIR -lgraphics
    LIB_GRAPHICS_PRI_PATH = ../DesktopEditor
    include(../DesktopEditor/Qt_build/graphics/project/graphics.pri)

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
        TARGET_EXT = .so
    }
}
################################################

CONFIG += c++11

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    HAVE_UNISTD_H \
    LINUX \
    _LINUX \
    _LINUX_QT

    message(linux)
}

DEFINES -= UNICODE

DEFINES += \
    _QT \
    FT2_BUILD_LIBRARY \
    EXCLUDE_JPG_SUPPORT \
    MNG_SUPPORT_DISPLAY \
    MNG_SUPPORT_READ \
    MNG_SUPPORT_WRITE \
    MNG_ACCESS_CHUNKS \
    MNG_STORE_CHUNKS\
    MNG_ERROR_TELLTALE

win32 {
    DEFINES += \
    JAS_WIN_MSVC_BUILD \
    WIN32 \
    NOMINMAX

    message(windows)
}

INCLUDEPATH += \
    ../DesktopEditor/agg-2.4/include \
    ../DesktopEditor/freetype-2.5.2/include \
    ../DesktopEditor/cximage/jasper/include \
    ../DesktopEditor/cximage/jpeg \
    ../DesktopEditor/cximage/png \
    ../DesktopEditor/cximage/zlib

HEADERS += \
    Src/Annotation.h \
    Src/Catalog.h \
    Src/Consts.h \
    Src/Destination.h \
    Src/Document.h \
    Src/Encrypt.h \
    Src/EncryptDictionary.h \
    Src/Font.h \
    Src/Font14.h \
    Src/FontCidTT.h \
    Src/FontTTWriter.h \
    Src/GState.h \
    Src/Image.h \
    Src/Info.h \
    Src/Objects.h \
    Src/Outline.h \
    Src/Pages.h \
    Src/Pattern.h \
    Src/Shading.h \
    Src/Streams.h \
    Src/Types.h \
    Src/Utils.h \
    OnlineOfficeBinToPdf.h \
    PdfRenderer.h

SOURCES += \
    Src/Annotation.cpp \
    Src/Catalog.cpp \
    Src/Destination.cpp \
    Src/Document.cpp \
    Src/Encrypt.cpp \
    Src/EncryptDictionary.cpp \
    Src/Font.cpp \
    Src/Font14.cpp \
    Src/FontCidTT.cpp \
    Src/FontTTWriter.cpp \
    Src/GState.cpp \
    Src/Image.cpp \
    Src/Info.cpp \
    Src/Objects.cpp \
    Src/Outline.cpp \
    Src/Pages.cpp \
    Src/Pattern.cpp \
    Src/Shading.cpp \
    Src/Streams.cpp \
    Src/Utils.cpp \
    OnlineOfficeBinToPdf.cpp \
    PdfRenderer.cpp


unix {
    target.path = /usr/lib
    INSTALLS += target
}

#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T16:30:42
#
#-------------------------------------------------

QT       -= core gui

VERSION = 1.0.0.2
TARGET = DjVuFile
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

mac {
    DESTDIR = $$DESTINATION_SDK_PATH/mac_64
}

################################################

############# dynamic dependencies #############
shared {
    DEFINES += DJVU_USE_DYNAMIC_LIBRARY

    LIBS += -L$$DESTDIR -lPdfWriter

    CONFIG += build_all_zlib build_zlib_as_sources
    include(../OfficeUtils/OfficeUtils.pri)

    CONFIG += build_cximage_zlib_disable
    LIB_GRAPHICS_PRI_PATH = ../DesktopEditor
    include(../DesktopEditor/Qt_build/graphics/project/graphics.pri)

    LIB_XML_PRI_PATH = ../DesktopEditor/xml
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
        TARGET_EXT = .so

        QMAKE_CXXFLAGS += -fvisibility=hidden
        QMAKE_CFLAGS += -fvisibility=hidden
    }

    mac {
        CONFIG += plugin

        QMAKE_CXXFLAGS += -fvisibility=hidden
        QMAKE_CFLAGS += -fvisibility=hidden
    }
} else {
    LIBS += -L$$DESTDIR -llibxml
}
################################################

CONFIG += c++11

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

linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
    HAVE_UNISTD_H \
    LINUX \
    _LINUX \
    _LINUX_QT \
    HAVE_MBSTATE_T \
    GCONTAINER_NO_MEMBER_TEMPLATES="1" \
    HAS_WCHAR \
    HAVE_WCHAR_H \
    UNIX \
    HAVE_STDINCLUDES
}

mac {
    DEFINES += \
    HAVE_UNISTD_H \
    LINUX \
    _LINUX \
    _LINUX_QT \
    HAVE_MBSTATE_T \
    GCONTAINER_NO_MEMBER_TEMPLATES="1" \
    HAS_WCHAR \
    HAVE_WCHAR_H \
    UNIX \
    HAVE_STDINCLUDES \
    _MAC \
    MAC

    CONFIG += c++11

    message(mac)
}

win32 {
    DEFINES += \
    JAS_WIN_MSVC_BUILD \
    WIN32 \
    NOMINMAX

    DEFINES -= UNICODE
}

INCLUDEPATH += \
    ../DesktopEditor/agg-2.4/include \
    ../DesktopEditor/freetype-2.5.2/include

SOURCES += DjVu.cpp \
    DjVuFileImplementation.cpp \
    libdjvu/Arrays.cpp \
    libdjvu/atomic.cpp \
    libdjvu/BSByteStream.cpp \
    libdjvu/BSEncodeByteStream.cpp \
    libdjvu/ByteStream.cpp \
    libdjvu/DataPool.cpp \
    libdjvu/debug.cpp \
    libdjvu/DjVmDir.cpp \
    libdjvu/DjVmDir0.cpp \
    libdjvu/DjVmDoc.cpp \
    libdjvu/DjVmNav.cpp \
    libdjvu/DjVuAnno.cpp \
    libdjvu/DjVuDocEditor.cpp \
    libdjvu/DjVuDocument.cpp \
    libdjvu/DjVuDumpHelper.cpp \
    libdjvu/DjVuErrorList.cpp \
    libdjvu/DjVuFile.cpp \
    libdjvu/DjVuFileCache.cpp \
    libdjvu/DjVuGlobal.cpp \
    libdjvu/DjVuGlobalMemory.cpp \
    libdjvu/DjVuImage.cpp \
    libdjvu/DjVuInfo.cpp \
    libdjvu/DjVuMessage.cpp \
    libdjvu/DjVuMessageLite.cpp \
    libdjvu/DjVuNavDir.cpp \
    libdjvu/DjVuPalette.cpp \
    libdjvu/DjVuPort.cpp \
    libdjvu/DjVuText.cpp \
    libdjvu/DjVuToPS.cpp \
    libdjvu/GBitmap.cpp \
    libdjvu/GContainer.cpp \
    libdjvu/GException.cpp \
    libdjvu/GIFFManager.cpp \
    libdjvu/GMapAreas.cpp \
    libdjvu/GOS.cpp \
    libdjvu/GPixmap.cpp \
    libdjvu/GRect.cpp \
    libdjvu/GScaler.cpp \
    libdjvu/GSmartPointer.cpp \
    libdjvu/GString.cpp \
    libdjvu/GThreads.cpp \
    libdjvu/GUnicode.cpp \
    libdjvu/GURL.cpp \
    libdjvu/IFFByteStream.cpp \
    libdjvu/IW44EncodeCodec.cpp \
    libdjvu/IW44Image.cpp \
    libdjvu/JB2EncodeCodec.cpp \
    libdjvu/JB2Image.cpp \
    libdjvu/JPEGDecoder.cpp \
    libdjvu/MMRDecoder.cpp \
    libdjvu/MMX.cpp \
    libdjvu/UnicodeByteStream.cpp \
    libdjvu/XMLParser.cpp \
    libdjvu/XMLTags.cpp \
    libdjvu/ZPCodec.cpp

HEADERS +=     DjVu.h \
    DjVuFileImplementation.h \
    libdjvu/Arrays.h \
    libdjvu/atomic.h \
    libdjvu/BSByteStream.h \
    libdjvu/ByteStream.h \
    libdjvu/DataPool.h \
    libdjvu/debug.h \
    libdjvu/DjVmDir.h \
    libdjvu/DjVmDir0.h \
    libdjvu/DjVmDoc.h \
    libdjvu/DjVmNav.h \
    libdjvu/DjVuAnno.h \
    libdjvu/DjVuDocEditor.h \
    libdjvu/DjVuDocument.h \
    libdjvu/DjVuDumpHelper.h \
    libdjvu/DjVuErrorList.h \
    libdjvu/DjVuFile.h \
    libdjvu/DjVuFileCache.h \
    libdjvu/DjVuGlobal.h \
    libdjvu/DjVuImage.h \
    libdjvu/DjVuInfo.h \
    libdjvu/DjVuMessage.h \
    libdjvu/DjVuMessageLite.h \
    libdjvu/DjVuNavDir.h \
    libdjvu/DjVuPalette.h \
    libdjvu/DjVuPort.h \
    libdjvu/DjVuText.h \
    libdjvu/DjVuToPS.h \
    libdjvu/GBitmap.h \
    libdjvu/GContainer.h \
    libdjvu/GException.h \
    libdjvu/GIFFManager.h \
    libdjvu/GMapAreas.h \
    libdjvu/GOS.h \
    libdjvu/GPixmap.h \
    libdjvu/GRect.h \
    libdjvu/GScaler.h \
    libdjvu/GSmartPointer.h \
    libdjvu/GString.h \
    libdjvu/GThreads.h \
    libdjvu/GURL.h \
    libdjvu/IFFByteStream.h \
    libdjvu/IW44Image.h \
    libdjvu/JB2Image.h \
    libdjvu/JPEGDecoder.h \
    libdjvu/MMRDecoder.h \
    libdjvu/MMX.h \
    libdjvu/UnicodeByteStream.h \
    libdjvu/XMLParser.h \
    libdjvu/XMLTags.h \
    libdjvu/ZPCodec.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

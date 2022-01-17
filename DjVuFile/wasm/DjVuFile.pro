QT       -= core gui

TARGET = djvu_common

#TEMPLATE = lib
#CONFIG += staticlib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= debug_and_release debug_and_release_target

DEFINES += _LIB
DEFINES += _CRT_SECURE_NO_WARNINGS

DEFINES -= UNICODE
DEFINES -= _UNICODE
DEFINES += NOMINMAX

DEFINES += WASM_MODE
DEFINES += THREADMODEL=0
DEFINES += "UINT=unsigned int"
DEFINES += DEBUGLVL=0
DEFINES += WASM_MODE_DEBUG

CONFIG += core_windows

core_linux {
    DEFINES += \
    HAVE_UNISTD_H \
    HAVE_MBSTATE_T \
    GCONTAINER_NO_MEMBER_TEMPLATES="1" \
    HAS_WCHAR \
    HAVE_WCHAR_H \
    UNIX \
    HAVE_STDINCLUDES
}

core_mac {
    DEFINES += \
    HAVE_UNISTD_H \    
    HAVE_MBSTATE_T \
    GCONTAINER_NO_MEMBER_TEMPLATES="1" \
    HAS_WCHAR \
    HAVE_WCHAR_H \
    UNIX \
    HAVE_STDINCLUDES
}

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

DJVULIB_SOURCE = $$PWD/../libdjvu
DJVULIB_SOURCE_WRAPPER = $$PWD/libdjvu

#SOURCES += \
#    $$DJVULIB_SOURCE/atomic.cpp
#    $$DJVULIB_SOURCE/wasm/DjVuMessage.cpp \
#    $$DJVULIB_SOURCE/wasm/GOS.cpp \
#    $$DJVULIB_SOURCE/GURL.cpp

SOURCES += \
    $$DJVULIB_SOURCE/Arrays.cpp \
    $$DJVULIB_SOURCE/BSByteStream.cpp \
    $$DJVULIB_SOURCE/BSEncodeByteStream.cpp \
    $$DJVULIB_SOURCE/ByteStream.cpp \
    $$DJVULIB_SOURCE/DataPool.cpp \
    $$DJVULIB_SOURCE/debug.cpp \
    $$DJVULIB_SOURCE/DjVmDir.cpp \
    $$DJVULIB_SOURCE/DjVmDir0.cpp \
    $$DJVULIB_SOURCE/DjVmDoc.cpp \
    $$DJVULIB_SOURCE/DjVmNav.cpp \
    $$DJVULIB_SOURCE/DjVuAnno.cpp \
    $$DJVULIB_SOURCE/DjVuDocEditor.cpp \
    $$DJVULIB_SOURCE/DjVuDocument.cpp \
    $$DJVULIB_SOURCE/DjVuDumpHelper.cpp \
    $$DJVULIB_SOURCE/DjVuErrorList.cpp \
    $$DJVULIB_SOURCE/DjVuFile.cpp \
    $$DJVULIB_SOURCE/DjVuFileCache.cpp \
    $$DJVULIB_SOURCE/DjVuGlobal.cpp \
    $$DJVULIB_SOURCE/DjVuGlobalMemory.cpp \
    $$DJVULIB_SOURCE/DjVuImage.cpp \
    $$DJVULIB_SOURCE/DjVuInfo.cpp \
    $$DJVULIB_SOURCE/DjVuMessageLite.cpp \
    $$DJVULIB_SOURCE/DjVuNavDir.cpp \
    $$DJVULIB_SOURCE/DjVuPalette.cpp \
    $$DJVULIB_SOURCE/DjVuPort.cpp \
    $$DJVULIB_SOURCE/DjVuText.cpp \
    $$DJVULIB_SOURCE/DjVuToPS.cpp \
    $$DJVULIB_SOURCE/GBitmap.cpp \
    $$DJVULIB_SOURCE/GContainer.cpp \
    $$DJVULIB_SOURCE/GException.cpp \
    $$DJVULIB_SOURCE/GIFFManager.cpp \
    $$DJVULIB_SOURCE/GMapAreas.cpp \
    $$DJVULIB_SOURCE/GPixmap.cpp \
    $$DJVULIB_SOURCE/GRect.cpp \
    $$DJVULIB_SOURCE/GScaler.cpp \
    $$DJVULIB_SOURCE/GSmartPointer.cpp \
    $$DJVULIB_SOURCE/GString.cpp \
    $$DJVULIB_SOURCE/GThreads.cpp \
    $$DJVULIB_SOURCE/GUnicode.cpp \
    $$DJVULIB_SOURCE/IFFByteStream.cpp \
    $$DJVULIB_SOURCE/IW44EncodeCodec.cpp \
    $$DJVULIB_SOURCE/IW44Image.cpp \
    $$DJVULIB_SOURCE/JB2EncodeCodec.cpp \
    $$DJVULIB_SOURCE/JB2Image.cpp \
    $$DJVULIB_SOURCE/JPEGDecoder.cpp \
    $$DJVULIB_SOURCE/MMRDecoder.cpp \
    $$DJVULIB_SOURCE/MMX.cpp \
    $$DJVULIB_SOURCE/UnicodeByteStream.cpp \
    $$DJVULIB_SOURCE/XMLParser.cpp \
    $$DJVULIB_SOURCE/XMLTags.cpp \
    $$DJVULIB_SOURCE/ZPCodec.cpp

SOURCES += \
    $$DJVULIB_SOURCE_WRAPPER/atomic.cpp \
    $$DJVULIB_SOURCE_WRAPPER/DjVuMessage.cpp \
    $$DJVULIB_SOURCE_WRAPPER/GOS.cpp \
    $$DJVULIB_SOURCE_WRAPPER/GURL.cpp

SOURCES += djvu_common.cpp

HEADERS += \
    $$DJVULIB_SOURCE/Arrays.h \
    $$DJVULIB_SOURCE/atomic.h \
    $$DJVULIB_SOURCE/BSByteStream.h \
    $$DJVULIB_SOURCE/ByteStream.h \
    $$DJVULIB_SOURCE/DataPool.h \
    $$DJVULIB_SOURCE/debug.h \
    $$DJVULIB_SOURCE/DjVmDir.h \
    $$DJVULIB_SOURCE/DjVmDir0.h \
    $$DJVULIB_SOURCE/DjVmDoc.h \
    $$DJVULIB_SOURCE/DjVmNav.h \
    $$DJVULIB_SOURCE/DjVuAnno.h \
    $$DJVULIB_SOURCE/DjVuDocEditor.h \
    $$DJVULIB_SOURCE/DjVuDocument.h \
    $$DJVULIB_SOURCE/DjVuDumpHelper.h \
    $$DJVULIB_SOURCE/DjVuErrorList.h \
    $$DJVULIB_SOURCE/DjVuFile.h \
    $$DJVULIB_SOURCE/DjVuFileCache.h \
    $$DJVULIB_SOURCE/DjVuGlobal.h \
    $$DJVULIB_SOURCE/DjVuImage.h \
    $$DJVULIB_SOURCE/DjVuInfo.h \
    $$DJVULIB_SOURCE/DjVuMessage.h \
    $$DJVULIB_SOURCE/DjVuMessageLite.h \
    $$DJVULIB_SOURCE/DjVuNavDir.h \
    $$DJVULIB_SOURCE/DjVuPalette.h \
    $$DJVULIB_SOURCE/DjVuPort.h \
    $$DJVULIB_SOURCE/DjVuText.h \
    $$DJVULIB_SOURCE/DjVuToPS.h \
    $$DJVULIB_SOURCE/GBitmap.h \
    $$DJVULIB_SOURCE/GContainer.h \
    $$DJVULIB_SOURCE/GException.h \
    $$DJVULIB_SOURCE/GIFFManager.h \
    $$DJVULIB_SOURCE/GMapAreas.h \
    $$DJVULIB_SOURCE/GOS.h \
    $$DJVULIB_SOURCE/GPixmap.h \
    $$DJVULIB_SOURCE/GRect.h \
    $$DJVULIB_SOURCE/GScaler.h \
    $$DJVULIB_SOURCE/GSmartPointer.h \
    $$DJVULIB_SOURCE/GString.h \
    $$DJVULIB_SOURCE/GThreads.h \
    $$DJVULIB_SOURCE/GURL.h \
    $$DJVULIB_SOURCE/IFFByteStream.h \
    $$DJVULIB_SOURCE/IW44Image.h \
    $$DJVULIB_SOURCE/JB2Image.h \
    $$DJVULIB_SOURCE/JPEGDecoder.h \
    $$DJVULIB_SOURCE/MMRDecoder.h \
    $$DJVULIB_SOURCE/MMX.h \
    $$DJVULIB_SOURCE/UnicodeByteStream.h \
    $$DJVULIB_SOURCE/XMLParser.h \
    $$DJVULIB_SOURCE/XMLTags.h \
    $$DJVULIB_SOURCE/ZPCodec.h

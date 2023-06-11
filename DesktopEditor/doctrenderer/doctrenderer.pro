QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../../Common/base.pri)

DEFINES += DOCTRENDERER_USE_DYNAMIC_LIBRARY_BUILDING
DEFINES += JSBASE_USE_DYNAMIC_LIBRARY_BUILDING
ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network)

#CONFIG += build_xp
#CONFIG += v8_version_60
core_android:DEFINES += DISABLE_MEMORY_LIMITATION

HEADERS += \
    config.h \
    doctrenderer.h \
    docbuilder.h

SOURCES += \
    nativecontrol.cpp \
    doctrenderer.cpp \
    docbuilder.cpp \
    docbuilder_p.cpp \
    graphics.cpp \
	hash.cpp

SOURCES += \
    ../../Common/OfficeFileFormatChecker2.cpp \
    ../../Common/3dParty/pole/pole.cpp \
    ../../OOXML/Base/unicode_util.cpp

HEADERS += \
    docbuilder_p.h \
    nativecontrol.h \
    graphics.h \
    hash.h

HEADERS += \
    embed/PointerEmbed.h \
    embed/ZipEmbed.h \
    embed/GraphicsEmbed.h \
    embed/MemoryStreamEmbed.h \
    embed/NativeControlEmbed.h \
    embed/NativeBuilderEmbed.h \
    embed/TextMeasurerEmbed.h \
    embed/HashEmbed.h \
    embed/Default.h \
    js_internal/js_base.h

SOURCES += \
    embed/PointerEmbed.cpp \
    embed/ZipEmbed.cpp \
    embed/GraphicsEmbed.cpp \
    embed/MemoryStreamEmbed.cpp \
    embed/NativeControlEmbed.cpp \
    embed/NativeBuilderEmbed.cpp \
    embed/TextMeasurerEmbed.cpp \
    embed/HashEmbed.cpp \
    embed/Default.cpp

include($$PWD/js_internal/js_base.pri)

!use_javascript_core {
    build_xp:DESTDIR=$$DESTDIR/xp
}

!use_javascript_core {
    SOURCES += \
        embed/v8/v8_NativeBuilder.cpp \
        embed/v8/v8_Pointer.cpp
} else {
    OBJECTIVE_SOURCES += ../common/Mac/NSString+StringUtils.mm
    OBJECTIVE_SOURCES += \
        embed/jsc/jsc_NativeBuilder.mm \
        embed/jsc/jsc_Pointer.mm
}

ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(embed/GraphicsEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(embed/HashEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(embed/MemoryStreamEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(embed/NativeControlEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(embed/TextMeasurerEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(embed/ZipEmbed.h)

include(../graphics/pro/textshaper.pri)
include(../../Common/3dParty/openssl/openssl.pri)

# downloader
DEFINES += BUIDLER_OPEN_DOWNLOAD_ENABLED
DEFINES += BUIDLER_OPEN_BASE64_ENABLED

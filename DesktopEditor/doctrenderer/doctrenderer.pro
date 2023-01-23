QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../../Common/base.pri)

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network)

#CONFIG += build_xp

core_ios {
    CONFIG += doct_renderer_empty
}
core_android {
    DEFINES += DISABLE_MEMORY_LIMITATION
    CONFIG += doct_renderer_empty
}

doct_renderer_empty {
SOURCES += doctrenderer_empty.cpp
} else {
HEADERS += \
    config.h \
    doctrenderer.h \
    docbuilder.h

SOURCES += \
    nativecontrol.cpp \
    doctrenderer.cpp \
    docbuilder.cpp \
    docbuilder_p.cpp \
    graphics.cpp

SOURCES += \
    ../../Common/OfficeFileFormatChecker2.cpp \
    ../../Common/3dParty/pole/pole.cpp \
    ../../OOXML/Base/unicode_util.cpp

HEADERS += \
    docbuilder_p.h \
    nativecontrol.h \
    graphics.h

HEADERS += \
    embed/PointerEmbed.h \
    embed/ZipEmbed.h \
    embed/GraphicsEmbed.h \
    embed/MemoryStreamEmbed.h \
    embed/NativeControlEmbed.h \
    embed/NativeBuilderEmbed.h \
    embed/TextMeasurerEmbed.h \
    js_internal/js_base.h

SOURCES += \
    embed/PointerEmbed.cpp \
    embed/ZipEmbed.cpp \
    embed/GraphicsEmbed.cpp \
    embed/MemoryStreamEmbed.cpp \
    embed/NativeControlEmbed.cpp \
    embed/NativeBuilderEmbed.cpp \
    embed/TextMeasurerEmbed.cpp

core_mac {
    !use_v8:CONFIG += use_javascript_core
}

#CONFIG += v8_use_inspector
include($$PWD/js_internal/js_base.pri)

!use_javascript_core {
    SOURCES += \
        embed/v8/v8_MemoryStream.cpp \
        embed/v8/v8_NativeControl.cpp \
        embed/v8/v8_NativeBuilder.cpp \
        embed/v8/v8_Graphics.cpp \
        embed/v8/v8_Zip.cpp \
		embed/v8/v8_Pointer.cpp \
		embed/v8/v8_TextMeasurer.cpp

    build_xp:DESTDIR=$$DESTDIR/xp
} else {
    OBJECTIVE_SOURCES += ../common/Mac/NSString+StringUtils.mm
    OBJECTIVE_SOURCES += \
        embed/jsc/jsc_Graphics.mm \
        embed/jsc/jsc_MemoryStream.mm \
        embed/jsc/jsc_NativeControl.mm \
        embed/jsc/jsc_NativeBuilder.mm \
        embed/jsc/jsc_Zip.mm \
		embed/jsc/jsc_Pointer.mm \
		embed/jsc/jsc_TextMeasurer.mm

    LIBS += -framework Foundation
}
}

include(../graphics/pro/textshaper.pri)

# downloader
DEFINES += BUIDLER_OPEN_DOWNLOAD_ENABLED
DEFINES += BUIDLER_OPEN_BASE64_ENABLED

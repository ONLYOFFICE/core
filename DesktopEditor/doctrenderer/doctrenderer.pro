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

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter)

#CONFIG += build_xp

core_ios {
    CONFIG += doct_renderer_empty
}
core_android {
    CONFIG += doct_renderer_empty
}

doct_renderer_empty {
SOURCES += doctrenderer_empty.cpp
} else {
HEADERS += \
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
    ../../Common/DocxFormat/Source/Base/unicode_util.cpp \
    ../fontengine/ApplicationFontsWorker.cpp

HEADERS += \
    docbuilder_p.h \
    memorystream.h \
    nativecontrol.h \
    graphics.h

HEADERS += \
    embed/GraphicsEmbed.h \
    embed/MemoryStreamEmbed.h \
    embed/NativeControlEmbed.h \
    embed/NativeBuilderEmbed.h \
    js_internal/js_base.h

SOURCES += \
    embed/GraphicsEmbed.cpp \
    embed/MemoryStreamEmbed.cpp \
    embed/NativeControlEmbed.cpp \
    embed/NativeBuilderEmbed.cpp

core_mac {
    !use_v8:CONFIG += use_javascript_core
}

!use_javascript_core {
    HEADERS += js_internal/v8/v8_base.h
    SOURCES += js_internal/v8/v8_base.cpp
    SOURCES += \
        embed/v8/v8_MemoryStream.cpp \
        embed/v8/v8_NativeControl.cpp \
        embed/v8/v8_NativeBuilder.cpp \
        embed/v8/v8_Graphics.cpp

    !build_xp {
        include(../../Common/3dParty/v8/v8.pri)
    } else {
        DEFINES += V8_OS_XP
        DESTDIR=$$DESTDIR/xp
        include(../../Common/3dParty/v8/v8_xp/v8.pri)
    }
} else {
    HEADERS += js_internal/jsc/jsc_base.h
    OBJECTIVE_SOURCES += js_internal/jsc/jsc_base.mm
    OBJECTIVE_SOURCES += ../common/Mac/NSString+StringUtils.mm
    OBJECTIVE_SOURCES += \
        embed/jsc/jsc_Graphics.mm \
        embed/jsc/jsc_MemoryStream.mm \
        embed/jsc/jsc_NativeControl.mm \
        embed/jsc/jsc_NativeBuilder.mm

    QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc -fobjc-weak

    LIBS += -framework Foundation
    LIBS += -framework JavaScriptCore
}
}

# downloader
DEFINES += BUIDLER_OPEN_DOWNLOAD_ENABLED
DEFINES += BUIDLER_OPEN_BASE64_ENABLED

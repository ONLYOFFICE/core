CORE_BUILDS_PLATFORM_PREFIX = win_64
BUILD_MODE=debug

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../../../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

core_linux:include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

CONFIG += v8_use_inspector
include($$CORE_ROOT_DIR/DesktopEditor/doctrenderer/js_internal/js_base.pri)

SOURCES += \
    $$PWD/main.cpp

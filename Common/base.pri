VERSION = $$cat(version.txt)

PRODUCT_VERSION = $$(PRODUCT_VERSION)
BUILD_NUMBER = $$(BUILD_NUMBER)

!isEmpty(PRODUCT_VERSION){
    !isEmpty(BUILD_NUMBER){
        VERSION = $${PRODUCT_VERSION}.$${BUILD_NUMBER}
	}
}

DEFINES += INTVER=$$VERSION

PUBLISHER_NAME = $$(PUBLISHER_NAME)
isEmpty(PUBLISHER_NAME){
    PUBLISHER_NAME = $$cat(copyright.txt)
}

win32 {
    CURRENT_YEAR = $$system("echo %Date:~6,4%")
}

!win32 {
    CURRENT_YEAR = $$system(date +%Y)
}

QMAKE_TARGET_COMPANY = $$PUBLISHER_NAME
QMAKE_TARGET_COPYRIGHT = Copyright (C) $${PUBLISHER_NAME} $${CURRENT_YEAR}. All rights reserved

# CONFIGURATION
CONFIG(debug, debug|release) {
    CONFIG += core_debug
} else {
    CONFIG += core_release
}

#PLATFORM
win32 {
    CONFIG += core_windows

    WINDOWS_VERSION_XP = $$(WINDOWS_VERSION_XP)
    !isEmpty(WINDOWS_VERSION_XP) {
        CONFIG += build_xp
        message(xp using)
    }
}

DST_ARCH=$$QMAKE_TARGET.arch
isEqual(QT_MAJOR_VERSION, 5) {
    DST_ARCH=$$QT_ARCH
    # QT_ARCH uses 'i386' instead of 'x86',
    # so map that value back to what we expect.
    equals(DST_ARCH, i386) {
        DST_ARCH=x86
    }
}

win32:contains(QMAKE_TARGET.arch, x86_64): {
    CONFIG += core_win_64
}
win32:!contains(QMAKE_TARGET.arch, x86_64): {
    CONFIG += core_win_32
}

linux-g++ {
    CONFIG += core_linux
linux-g++:contains(QMAKE_HOST.arch, x86_64): {
    message("linux-64")
    CONFIG += core_linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64): {
    message("linux-32")
    CONFIG += core_linux_32
}
linux-g++:contains(DST_ARCH, arm): {
    message("arm")
    CONFIG += core_linux_arm
    DEFINES += LINUX_ARM
}
}

linux-g++-64 {
    message("linux-64")
    CONFIG += core_linux
    CONFIG += core_linux_64
}
linux-g++-32 {
    message("linux-32")
    CONFIG += core_linux
    CONFIG += core_linux_32
}


mac {
    CONFIG += core_mac
    CONFIG += core_mac_64
}

# DEFINES
core_windows {
    DEFINES += WIN32 _WIN32
    DEFINES += NOMINMAX
}
core_win_64 {
    DEFINES += WIN64 _WIN64
}

core_linux {
    DEFINES += LINUX _LINUX
}

core_mac {
    DEFINES += LINUX _LINUX MAC _MAC
}

# PREFIXES
core_windows {
    CONFIG -= debug_and_release debug_and_release_target
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
    QMAKE_CXXFLAGS -= -Zc:strictStrings
    QMAKE_CXXFLAGS += /MP
}

core_win_32 {
    CORE_BUILDS_PLATFORM_PREFIX = win_32
}
core_win_64 {
    CORE_BUILDS_PLATFORM_PREFIX = win_64
}
core_linux_32 {
    CORE_BUILDS_PLATFORM_PREFIX = linux_32
}
core_linux_64 {
    CORE_BUILDS_PLATFORM_PREFIX = linux_64
}
core_mac_64 {
    CORE_BUILDS_PLATFORM_PREFIX = mac_64
}
core_linux_arm {
    CORE_BUILDS_PLATFORM_PREFIX = arm
}

core_debug {
    CORE_BUILDS_CONFIGURATION_PREFIX    = debug
}
core_release {
    CORE_BUILDS_CONFIGURATION_PREFIX    = release
}

# MESSAGE
message($$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX)

# COMPILER
CONFIG += c++11

core_linux {
core_static_link_libstd {
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
    message(core_static_link_libstd)
}
plugin {
    QMAKE_CXXFLAGS += -fvisibility=hidden
    QMAKE_CFLAGS += -fvisibility=hidden

    TARGET_EXT = .so
}
}

core_mac {
plugin {
    QMAKE_CXXFLAGS += -fvisibility=hidden
    QMAKE_CFLAGS += -fvisibility=hidden
}
}

core_windows {
plugin {
    TARGET_EXT = .dll
}
}

# BUILD_PATHS
# MUST BE DEFINED CORE_ROOT_DIR
!core_no_dst {
DESTDIR     = $$PWD_ROOT_DIR/core_build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
OBJECTS_DIR = $$PWD_ROOT_DIR/core_build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/obj
MOC_DIR     = $$PWD_ROOT_DIR/core_build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/moc
RCC_DIR     = $$PWD_ROOT_DIR/core_build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/rcc
UI_DIR      = $$PWD_ROOT_DIR/core_build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/ui
}

CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/lib/$$CORE_BUILDS_PLATFORM_PREFIX
core_windows {
core_debug {
    CORE_BUILDS_LIBRARIES_PATH = $$CORE_BUILDS_LIBRARIES_PATH/DEBUG
}
}

plugin {
    DESTDIR = $$CORE_BUILDS_LIBRARIES_PATH
}
staticlib {
    DESTDIR = $$CORE_BUILDS_LIBRARIES_PATH
}

core_x2t {
core_windows {
    QMAKE_CXXFLAGS += /bigobj
} else {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}

DEFINES += CRYPTOPP_DISABLE_ASM
}

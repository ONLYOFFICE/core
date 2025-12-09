VERSION = $$cat(version.txt)

PRODUCT_VERSION = $$(PRODUCT_VERSION)
BUILD_NUMBER = $$(BUILD_NUMBER)

!isEmpty(PRODUCT_VERSION){
	!isEmpty(BUILD_NUMBER){
		VERSION = $${PRODUCT_VERSION}.$${BUILD_NUMBER}
	}
}

DEFINES += INTVER=$$VERSION

WIN_VERSION = $$replace(VERSION, \., ",")
DEFINES += WIN_INTVER=$$WIN_VERSION

PUBLISHER_NAME = $$(PUBLISHER_NAME)
isEmpty(PUBLISHER_NAME){
	PUBLISHER_NAME = $$cat(copyright.txt)
}

DEST_MAKEFILE_NAME = $$(DEST_MAKEFILE_NAME)
!isEmpty(DEST_MAKEFILE_NAME){
	MAKEFILE = $${DEST_MAKEFILE_NAME}
}

APPLICATION_NAME_DEFAULT = $$(APPLICATION_NAME_DEFAULT)
!isEmpty(APPLICATION_NAME_DEFAULT){
	DEFINES += "APPLICATION_NAME_DEFAULT=$${APPLICATION_NAME_DEFAULT}"
}

OO_BUILD_BRANDING = $$(OO_BRANDING)
OO_DESTDIR_BUILD_OVERRIDE = $$(DESTDIR_BUILD_OVERRIDE)

win32 {
	CURRENT_YEAR = $$system(wmic PATH Win32_LocalTime GET ^Year /FORMAT:VALUE | find \"=\")
	CURRENT_YEAR = $$replace(CURRENT_YEAR, "Year=", "")
	CURRENT_YEAR = $$replace(CURRENT_YEAR, "\r", "")
	CURRENT_YEAR = $$replace(CURRENT_YEAR, "\n", "")
	CURRENT_YEAR = $$replace(CURRENT_YEAR, "\t", "")
}

!win32 {
	CURRENT_YEAR = $$system(date +%Y)
}

DEFINES += COPYRIGHT_YEAR=$${CURRENT_YEAR}
#DEFINES += _LOGOUT_ALWAYS

QMAKE_TARGET_COMPANY = $$PUBLISHER_NAME
QMAKE_TARGET_COPYRIGHT = © $${PUBLISHER_NAME} $${CURRENT_YEAR}. All rights reserved.

# CONFIGURATION
CONFIG(debug, debug|release) {
	CONFIG += core_debug
}
force_debug_info:CONFIG += core_debug
!core_debug:CONFIG += core_release

not_use_dynamic_libs {
shared {
	CONFIG -= shared
	CONFIG -= plugin
	CONFIG += staticlib
}
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

	lessThan(QT_MINOR_VERSION, 15) {
		DEFINES += QT_VERSION_LESS_5_15
	}
}

greaterThan(QT_MAJOR_VERSION, 5) {
	DEFINES += QT_VERSION_6

	core_windows {
		QMAKE_CXXFLAGS += /permissive-
	}
}

ios {
	CONFIG += core_ios
	DEFINES += _IOS IOS LINUX _LINUX MAC _MAC _XCODE _ARM_ALIGN_
}
android {
	CONFIG += core_android
	DEFINES += __ANDROID__ LINUX _LINUX _ARM_ALIGN_
}

win32:contains(QMAKE_TARGET.arch, x86_64): {
	CONFIG += core_win_64
}
win32:!contains(QMAKE_TARGET.arch, x86_64): {
	CONFIG += core_win_32
}
win32:contains(QMAKE_TARGET.arch, arm64): {
	CONFIG -= core_win_32
	CONFIG += core_win_arm64
}

linux-clang-libc++ {
    CONFIG += core_linux
	CONFIG += core_linux_64
	CONFIG += core_linux_clang
	message("linux-64-clang-libc++")
}
linux-clang-libc++-32 {
    CONFIG += core_linux
	CONFIG += core_linux_32
	CONFIG += core_linux_clang
	message("linux-32-clang-libc++")
}
linux-clang {
	CONFIG += core_linux
	CONFIG += core_linux_64
	CONFIG += core_linux_clang
	message("linux-64-clang")
}
linux-clang-32 {
	CONFIG += core_linux
	CONFIG += core_linux_32
	CONFIG += core_linux_clang
	message("linux-32-clang")
}

linux-g++ {
	CONFIG += core_linux
linux-g++:contains(QMAKE_HOST.arch, x86_64): {
	message("linux-64")
	CONFIG += core_linux_64
}
linux-g++:contains(QMAKE_HOST.arch, arm64): {
	message("linux-64")
	CONFIG += core_linux_64
	CONFIG += core_linux_host_arm64
}
linux-g++:contains(QMAKE_HOST.arch, aarch64): {
	message("linux-64")
	CONFIG += core_linux_64
	CONFIG += core_linux_host_arm64
}
!core_linux_64: {
	message("linux-32")
	CONFIG += core_linux_32
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
	!core_ios {
		CONFIG += core_mac
		CONFIG += core_mac_64

		DEFINES += _LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION
	}
}

# DEFINES
core_windows {
	DEFINES += WIN32 _WIN32
	DEFINES += NOMINMAX

	#DEFINES += WIN32_LEAN_AND_MEAN

	# use default _ITERATOR_DEBUG_LEVEL value
	#core_debug:DEFINES += "_ITERATOR_DEBUG_LEVEL=0"
}
core_win_64 {
	DEFINES += WIN64 _WIN64
}

core_linux {
	DEFINES += LINUX _LINUX
    core_linux_64 {
	    !linux_arm64 { # x86_64
	        QMAKE_CUSTOM_SYSROOT = $$(QMAKE_CUSTOM_SYSROOT)
			QMAKE_CUSTOM_SYSROOT_BIN = $$(QMAKE_CUSTOM_SYSROOT)/usr/bin/
		    QMAKE_CUSTOM_SYSROOT_LIB = $$(QMAKE_CUSTOM_SYSROOT)/usr/lib/x86_64-linux-gnu
			!isEmpty(QMAKE_CUSTOM_SYSROOT) {
			    message("using custom sysroot $$QMAKE_CUSTOM_SYSROOT")
				QMAKE_CC          = $$join(QMAKE_CUSTOM_SYSROOT_BIN, , , "gcc")
				QMAKE_CXX         = $$join(QMAKE_CUSTOM_SYSROOT_BIN, , , "g++")
				QMAKE_LINK        = $$join(QMAKE_CUSTOM_SYSROOT_BIN, , , "g++")
				QMAKE_LINK_SHLIB  = $$join(QMAKE_CUSTOM_SYSROOT_BIN, , , "g++")

                QMAKE_CFLAGS      += --sysroot $$QMAKE_CUSTOM_SYSROOT
				QMAKE_CXXFLAGS    += --sysroot $$QMAKE_CUSTOM_SYSROOT
				QMAKE_LFLAGS      += --sysroot $$QMAKE_CUSTOM_SYSROOT
			}
		}
	}
}

core_linux_32 {
	CORE_BUILDS_PLATFORM_PREFIX = linux_32
}
core_linux_64 {
	CORE_BUILDS_PLATFORM_PREFIX = linux_64
}
core_linux_arm {
	CORE_BUILDS_PLATFORM_PREFIX = arm
}
linux_arm64 {
	CORE_BUILDS_PLATFORM_PREFIX = linux_arm64
	DEFINES += _ARM_ALIGN_

	ARM64_TOOLCHAIN_BIN = $$(ARM64_TOOLCHAIN_BIN)
	ARM64_TOOLCHAIN_BIN_PREFIX = $$(ARM64_TOOLCHAIN_BIN_PREFIX)
	ARM64_SYSROOT = $$(ARM64_SYSROOT)

	!isEmpty(ARM64_TOOLCHAIN_BIN){
		!isEmpty(ARM64_TOOLCHAIN_BIN_PREFIX){
			ARM64_TOOLCHAIN_BIN_FULL = $$ARM64_TOOLCHAIN_BIN/$$ARM64_TOOLCHAIN_BIN_PREFIX
			message("using arm64 toolchain $$ARM64_TOOLCHAIN_BIN")

			QMAKE_CC          = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "gcc")
			QMAKE_CXX         = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "g++")
			QMAKE_LINK        = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "g++")
			QMAKE_LINK_SHLIB  = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "g++")

			QMAKE_AR          = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "ar cqs")
			QMAKE_OBJCOPY     = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "objcopy")
			QMAKE_NM          = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "nm -P")
			QMAKE_STRIP       = $$join(ARM64_TOOLCHAIN_BIN_FULL, , , "strip")
		}
	}
}

gcc {
    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
	COMPILER_MAJOR_VERSION_ARRAY = $$split(COMPILER_VERSION, ".")
	COMPILER_MAJOR_VERSION = $$member(COMPILER_MAJOR_VERSION_ARRAY, 0)
	lessThan(COMPILER_MAJOR_VERSION, 5): CONFIG += build_gcc_less_5
	lessThan(COMPILER_MAJOR_VERSION, 6): CONFIG += build_gcc_less_6
}

core_linux_host_arm64 {
	message("build on arm64")
	DEFINES += _ARM_ALIGN_
}

core_mac {
	DEFINES += LINUX _LINUX MAC _MAC
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
	QMAKE_LFLAGS += -isysroot $$QMAKE_MAC_SDK_PATH

	# xcode15 add new linker
	greaterThan(QMAKE_XCODE_VERSION, 1499) {
		QMAKE_LFLAGS += -Wl,-ld_classic
	} else {
		CONFIG += c++14
	}

	QMAKE_CFLAGS += "-Wno-implicit-function-declaration"

	greaterThan(QT_MAJOR_VERSION, 5) {
		QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12
		!apple_silicon:QMAKE_APPLE_DEVICE_ARCHS = x86_64
	}
}

core_linux_clang {
	QMAKE_CFLAGS += -Wno-implicit-function-declaration
}

# PREFIXES
core_windows {
	CONFIG -= debug_and_release debug_and_release_target
	QMAKE_CXXFLAGS_RELEASE += /Zc:strictStrings-
	QMAKE_CXXFLAGS += /Zc:strictStrings-
	QMAKE_CXXFLAGS += /MP

	MSVC_VERSION_DETECT = $$(VisualStudioVersion)
	greaterThan(MSVC_VERSION_DETECT, 15.0) {
		CONFIG += vs2019
	}

	vs2019 {
		QMAKE_CXXFLAGS_RELEASE -= -permissive-
		QMAKE_CXXFLAGS -= -permissive-
	}

	DEFINES += WINDOWS_IGNORE_PACKING_MISMATCH

	OO_WINDOWS_IGNORE_PACKING_MISMATCH = $$(OO_WINDOWS_IGNORE_PACKING_MISMATCH)
	isEqual(OO_WINDOWS_IGNORE_PACKING_MISMATCH, 1):QMAKE_CXXFLAGS -= /Zp

	equals(TEMPLATE, app) {
		console {
			core_win_64:QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.02
			core_win_32:QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
		} else {
			core_win_64:QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.02
			core_win_32:QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
		}
	}
}

core_linux {
	equals(TEMPLATE, app) {
		QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
		QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/system\'"
		QMAKE_LFLAGS += -Wl,--disable-new-dtags

		!disable_rpath_addon {
			RUN_PATH_ADDON = $$(RUN_PATH_ADDON)
			!isEmpty(RUN_PATH_ADDON){
				RUN_PATH_ADDON_ARRAY = $$split(RUN_PATH_ADDON, ";;")
				for(rpath_item, RUN_PATH_ADDON_ARRAY):QMAKE_LFLAGS += "-Wl,-rpath,\'$$rpath_item\'"
			}
		}
	}
}

core_linux {
	equals(TEMPLATE, app):CONFIG += core_static_link_libstd
	plugin:CONFIG += core_static_link_libstd
}

core_win_32 {
	CORE_BUILDS_PLATFORM_PREFIX = win_32
}
core_win_64 {
	CORE_BUILDS_PLATFORM_PREFIX = win_64
}
core_win_arm64 {
    CORE_BUILDS_PLATFORM_PREFIX = win_arm64
}
core_mac_64 {
	CORE_BUILDS_PLATFORM_PREFIX = mac_64

	apple_silicon {
		CORE_BUILDS_PLATFORM_PREFIX = mac_arm64
		QMAKE_APPLE_DEVICE_ARCHS = arm64
	}
}
core_ios {
	CORE_BUILDS_PLATFORM_PREFIX = ios

	!versionAtLeast(QMAKE_XCODE_VERSION, 16.0) {
		QMAKE_CFLAGS += -fembed-bitcode
		QMAKE_CXXFLAGS += -fembed-bitcode
		QMAKE_LFLAGS += -fembed-bitcode
	} else {
		CONFIG -= bitcode
	}


	CONFIG(iphonesimulator, iphoneos|iphonesimulator): {
		message("iphonesimulator")
		CORE_BUILDS_PLATFORM_PREFIX = ios_simulator

		QMAKE_CFLAGS += -fobjc-arc
		QMAKE_CXXFLAGS += -fobjc-arc
	} else {

		QMAKE_IOS_DEPLOYMENT_TARGET = 11.0

		QMAKE_CFLAGS += -fobjc-arc
		QMAKE_CXXFLAGS += -fobjc-arc

		bundle_xcframeworks {
			xcframework_platform_ios_simulator {
				QMAKE_APPLE_DEVICE_ARCHS=
				QMAKE_APPLE_SIMULATOR_ARCHS=x86_64 arm64
			} else {
				QMAKE_APPLE_DEVICE_ARCHS = arm64
				QMAKE_APPLE_SIMULATOR_ARCHS=
			}
		} else {
			CONFIG += core_ios_main_arch
		}

		core_ios_main_arch {
			QMAKE_APPLE_DEVICE_ARCHS = arm64
			core_ios_no_simulator_arch : QMAKE_APPLE_SIMULATOR_ARCHS=

			core_ios_32 {
				QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_APPLE_DEVICE_ARCHS armv7
			}
		}

		core_ios_nomain_arch {
			QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_APPLE_DEVICE_ARCHS arm64e
			core_ios_32 {
				QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_APPLE_DEVICE_ARCHS armv7s
			}
		}

		!no_verify_bitcode {
			# for application: (using xcodebuild)
			#Q_ENABLE_BITCODE.name = ENABLE_BITCODE
			#Q_ENABLE_BITCODE.value = NO
			#
			#Q_BITCODE_GENERATION_MODE.name = BITCODE_GENERATION_MODE
			#Q_BITCODE_GENERATION_MODE.value = marker
			#
			#QMAKE_MAC_XCODE_SETTINGS += Q_ENABLE_BITCODE
			#QMAKE_MAC_XCODE_SETTINGS += Q_BITCODE_GENERATION_MODE

			# for libraries
			!core_debug {
				QMAKE_LFLAGS += -Xlinker -bitcode_verify
				QMAKE_LFLAGS += -Xlinker -bitcode_hide_symbols
				#QMAKE_LFLAGS += -Xlinker -bitcode_symbol_map -Xlinker $$CORE_BUILDS_LIBRARIES_PATH
			}
		}
	}

	!core_ios_no_unistd {
		DEFINES += HAVE_UNISTD_H HAVE_FCNTL_H
	}
}

core_android {
	CORE_BUILDS_PLATFORM_PREFIX = $$join(ANDROID_TARGET_ARCH, ANDROID_TARGET_ARCH, "android_", "")
	CORE_BUILDS_PLATFORM_PREFIX = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "-", "_")
	CORE_BUILDS_PLATFORM_PREFIX = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "armeabi_v7", "armv7")
	CORE_BUILDS_PLATFORM_PREFIX = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "armv7a", "armv7")
	CORE_BUILDS_PLATFORM_PREFIX_DST = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "android_", "")

	!isEmpty(OO_DESTDIR_BUILD_OVERRIDE) {
		isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_arm64_v8a):OO_DESTDIR_BUILD_OVERRIDE=$$OO_DESTDIR_BUILD_OVERRIDE/arm64-v8a
		isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_armv7):OO_DESTDIR_BUILD_OVERRIDE=$$OO_DESTDIR_BUILD_OVERRIDE/armeabi-v7a
		isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_x86):OO_DESTDIR_BUILD_OVERRIDE=$$OO_DESTDIR_BUILD_OVERRIDE/x86
		isEqual(CORE_BUILDS_PLATFORM_PREFIX, android_x86_64):OO_DESTDIR_BUILD_OVERRIDE=$$OO_DESTDIR_BUILD_OVERRIDE/x86_64
	}

	core_debug {
		QMAKE_LFLAGS += -Wl,--build-id=sha1
	}

	!core_android_no_unistd {
		DEFINES += HAVE_UNISTD_H HAVE_FCNTL_H
	}
}

core_debug {
	DEFINES += _DEBUG
	CORE_BUILDS_CONFIGURATION_PREFIX    = debug
}
core_release {
	CORE_BUILDS_CONFIGURATION_PREFIX    = release
}

CONFIG += object_parallel_to_source
core_windows:CONFIG += no_batch

# MESSAGE
message($$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX)

# COMPILER
CONFIG += c++11

#CONFIG += enable_cpp_17
enable_cpp_17 {
	CONFIG += c++1z
	DEFINES += _LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION
}

!core_windows {
	QMAKE_CXXFLAGS += -Wno-register
	QMAKE_CFLAGS += -Wno-register
}

core_linux {
core_static_link_libstd {
	!core_linux_clang {
		QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
	} else {
		# TODO: add libc++abi?
	}
	message(core_static_link_libstd)
}
plugin {
	TARGET_EXT = .so
}
}

core_windows {
plugin {
	TARGET_EXT = .dll
}
}

!core_windows {
	plugin:CONFIG += config_hidden_symbols
	staticlib:CONFIG += config_hidden_symbols
}

config_hidden_symbols {
	QMAKE_CXXFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
	QMAKE_CFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden

	core_mac:CONFIG += clang_no_exclude_libs
	core_ios:CONFIG += clang_no_exclude_libs

	!clang_no_exclude_libs {
		plugin:QMAKE_LFLAGS += -Wl,--exclude-libs,ALL
		equals(TEMPLATE, app) {
			QMAKE_LFLAGS += -Wl,--exclude-libs,ALL
		}
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

bundle_xcframeworks {
	xcframework_platform_ios_simulator {
		OBJECTS_DIR = $$OBJECTS_DIR/simulator
		MOC_DIR     = $$MOC_DIR/simulator
		RCC_DIR     = $$RCC_DIR/simulator
		UI_DIR      = $$UI_DIR/simulator
	}
}

build_xp {
	OBJECTS_DIR = $$OBJECTS_DIR/xp
	MOC_DIR     = $$MOC_DIR/xp
	RCC_DIR     = $$RCC_DIR/xp
	UI_DIR      = $$UI_DIR/xp
}
}

core_windows {
	build_xp {
		DEFINES += WIN_XP_OR_VISTA
	}
}

CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/lib/$$CORE_BUILDS_PLATFORM_PREFIX
CORE_BUILDS_BINARY_PATH = $$CORE_ROOT_DIR/build/bin/$$CORE_BUILDS_PLATFORM_PREFIX

!isEmpty(OO_BUILD_BRANDING) {
	CORE_BUILDS_LIBRARIES_PATH = $$CORE_ROOT_DIR/build/$$OO_BUILD_BRANDING/lib/$$CORE_BUILDS_PLATFORM_PREFIX
	CORE_BUILDS_BINARY_PATH = $$CORE_ROOT_DIR/build/$$OO_BUILD_BRANDING/bin/$$CORE_BUILDS_PLATFORM_PREFIX

	DEFINES += "BUILD_BRANDING_NAME=\"\\\"$$OO_BUILD_BRANDING\\\"\""
}

core_debug {
	CORE_BUILDS_LIBRARIES_PATH = $$CORE_BUILDS_LIBRARIES_PATH/debug
	CORE_BUILDS_BINARY_PATH = $$CORE_BUILDS_BINARY_PATH/debug
}

!isEmpty(OO_DESTDIR_BUILD_OVERRIDE) {
	CORE_BUILDS_LIBRARIES_PATH = $$OO_DESTDIR_BUILD_OVERRIDE
	CORE_BUILDS_BINARY_PATH = $$OO_DESTDIR_BUILD_OVERRIDE
}

core_ios {
	xcframework_platform_ios_simulator {
		CORE_BUILDS_LIBRARIES_PATH = $$CORE_BUILDS_LIBRARIES_PATH/simulator
		CORE_BUILDS_BINARY_PATH = $$CORE_BUILDS_BINARY_PATH/simulator
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

core_ios|core_mac {
	CONFIG += support_bundle_dylibs
}

!support_bundle_dylibs:CONFIG-=bundle_dylibs

bundle_dylibs {
	plugin {
		CONFIG -= plugin
		CONFIG += lib_bundle

		QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/
		#QMAKE_LFLAGS += -Xlinker -rpath -Xlinker @executable_path/Frameworks
		#QMAKE_LFLAGS += -Xlinker -rpath -Xlinker @loader_path/Frameworks

		# correct version to < 256
		VERSIONS = $$split(VERSION, ".")
		VERSION_1 = $$member(VERSIONS, 0)
		VERSION_2 = $$member(VERSIONS, 1)
		VERSION_3 = $$member(VERSIONS, 2)
		VERSION_4 = $$member(VERSIONS, 3)

		greaterThan(VERSION_1, 255): VERSION_1 = 255
		greaterThan(VERSION_2, 255): VERSION_2 = 255
		greaterThan(VERSION_3, 255): VERSION_3 = 255
		greaterThan(VERSION_4, 255): VERSION_4 = 255

		VERSION_CORRECT = $$VERSION_1
		VERSION_CORRECT = $$join(VERSION_CORRECT, "", "", ".")
		VERSION_CORRECT = $$join(VERSION_CORRECT, "", "", $$VERSION_2)
		VERSION_CORRECT = $$join(VERSION_CORRECT, "", "", ".")
		VERSION_CORRECT = $$join(VERSION_CORRECT, "", "", $$VERSION_3)
		VERSION_CORRECT = $$join(VERSION_CORRECT, "", "", ".")
		VERSION_CORRECT = $$join(VERSION_CORRECT, "", "", $$VERSION_4)

		VERSION = $$VERSION_CORRECT
		MAJOR_VERSION = $$VERSION_1
		# set framework version as A
		QMAKE_FRAMEWORK_VERSION = A
	}
}

defineTest(ADD_DEPENDENCY) {
	libs = $$ARGS
	for(lib, libs) {
		CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH

		isEqual(lib, videoplayer) {
			libvlc {
				CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH/mediaplayer
			}
		}

		build_xp {
			isEqual(lib, doctrenderer):CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH_DST/xp
			isEqual(lib, ascdocumentscore):CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH_DST/xp
			isEqual(lib, qtascdocumentscore):CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH_DST/xp
			isEqual(lib, videoplayer):CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH_DST/xp
			isEqual(lib, ooxmlsignature):CORE_BUILDS_LIBRARIES_PATH_DST=$$CORE_BUILDS_LIBRARIES_PATH_DST/xp
		}
		!bundle_dylibs:LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH_DST -l$$lib
		bundle_dylibs:LIBS += -F$$CORE_BUILDS_LIBRARIES_PATH_DST -framework $$lib
	}
	export(LIBS)
}

ADD_INC_PATH = $$(ADDITIONAL_INCLUDE_PATH)
!isEmpty(ADD_INC_PATH):INCLUDEPATH += $$ADD_INC_PATH

!core_enable_all_warnings {
	core_disable_all_warnings {
		QMAKE_CXXFLAGS_WARN_OFF = -w
		QMAKE_CFLAGS_WARN_OFF = -w
		CONFIG += warn_off
	}
}

!disable_precompiled_header {
	CONFIG += precompile_header
}

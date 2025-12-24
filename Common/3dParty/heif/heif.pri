DEFINES += LIBHEIF_STATIC_BUILD

HEIF_BUILDS_PLATFORM_PREFIX = $$CORE_BUILDS_PLATFORM_PREFIX
core_ios : xcframework_platform_ios_simulator {
	HEIF_BUILDS_PLATFORM_PREFIX = ios_simulator
}

HEIF_BUILD_PATH = $$PWD/libheif/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

INCLUDEPATH += \
	$$PWD/libheif/libheif/api \
	$$HEIF_BUILD_PATH				# for heif_version.h

core_windows {
	core_debug {
		BUILD_TYPE = Debug
	} else {
		BUILD_TYPE = Release
	}

	LIBS += \
		-L$$PWD/x265_git/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/$$BUILD_TYPE -lx265-static \
		-L$$PWD/libde265/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/libde265/$$BUILD_TYPE -llibde265 \
		-L$$HEIF_BUILD_PATH/libheif/$$BUILD_TYPE -lheif
}

core_linux | core_android {
	# we need to wrap x265 and de265 libraries in `whole-archive` flags to avoid "undefined symbol" errors when later linking with graphics.so
	LIBS += \
		-Wl,--whole-archive \
		-L$$PWD/x265_git/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX -lx265 \
		-L$$PWD/libde265/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/libde265 -lde265 \
		-Wl,--no-whole-archive \
		-L$$HEIF_BUILD_PATH/libheif -lheif
}

core_mac | core_ios {
	LIBS += \
		-L$$PWD/x265_git/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX -lx265 \
		-L$$PWD/libde265/build/$$HEIF_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/libde265 -lde265 \
		-L$$HEIF_BUILD_PATH/libheif -lheif
}

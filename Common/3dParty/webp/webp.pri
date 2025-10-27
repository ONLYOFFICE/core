WEBP_BUILDS_PLATFORM_PREFIX = $$CORE_BUILDS_PLATFORM_PREFIX
WEBP_BUILD_PATH = $$PWD/build/$$WEBP_BUILDS_PLATFORM_PREFIX

core_windows {
	core_win_32 {
		BUILD_PLATFORM = x86
	} else {
		BUILD_PLATFORM = x64
	}

	core_debug {
	        LIBS += -L$$WEBP_BUILD_PATH/debug-static/$$BUILD_PLATFORM/lib -llibwebp_debug
	} else {
	        LIBS += -L$$WEBP_BUILD_PATH/release-static/$$BUILD_PLATFORM/lib -llibwebp
	}
}

core_linux | core_android {
        LIBS += \
	        -L$$WEBP_BUILD_PATH/$$CORE_BUILDS_CONFIGURATION_PREFIX/src/.libs -lwebp \
		-L$$WEBP_BUILD_PATH/$$CORE_BUILDS_CONFIGURATION_PREFIX/sharpyuv/.libs -lsharpyuv
}

core_mac | core_ios {
        LIBS += -L$$WEBP_BUILD_PATH/$$CORE_BUILDS_CONFIGURATION_PREFIX/src/.libs -lwebp
}

core_windows {
	core_win_32 {
		BUILD_PLATFORM = x86
	} else {
		BUILD_PLATFORM = x64
	}

	core_debug {
		LIBS += -L$$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/debug-static/$$BUILD_PLATFORM/lib -llibwebp_debug
	} else {
		LIBS += -L$$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/release-static/$$BUILD_PLATFORM/lib -llibwebp
	}
}

CONFIG += enable_support_shaper

core_windows {
    MSVC_VERSION_DETECT = $$(VisualStudioVersion)

    lessThan(MSVC_VERSION_DETECT, 16.0) {
	    CONFIG -= enable_support_shaper
	}
}

core_linux {
    !system( g++ --version | grep -e "\<5.[0-9]" ):CONFIG -= enable_support_shaper
}

enable_support_shaper {
    DEFINES += SUPPORT_HARFBUZZ_SHAPER
}

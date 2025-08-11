DEFINES += LIBHEIF_STATIC_BUILD
INCLUDEPATH += $$PWD/libheif/libheif/api

core_windows {
	# TODO
}

core_linux {
	LIBS += -L$$PWD/libheif/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/libheif -lheif
}

core_mac {
	LIBS += -L$$PWD/libheif/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX/libheif -lheif
}

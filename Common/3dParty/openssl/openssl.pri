OPEN_SSL_PLATFORM=$$CORE_BUILDS_PLATFORM_PREFIX
build_xp:OPEN_SSL_PLATFORM=$$join(OPEN_SSL_PLATFORM, OPEN_SSL_PLATFORM, "", "_xp")

open_ssl_common {
    DEFINES += COMMON_OPENSSL_BUILDING

    HEADERS += $$PWD/common/common_openssl.h
    SOURCES += $$PWD/common/common_openssl.cpp
}

OPENSSL_LIBS_DIRECTORY = $$PWD/build/$$OPEN_SSL_PLATFORM/lib

core_android {
    OPENSSL_LIBS_DIRECTORY = $$PWD/build/android/$$CORE_BUILDS_PLATFORM_PREFIX_DST/lib
}

core_ios {

    OPENSSL_LIBS_DIRECTORY = $$PWD/ios/openssl-universal/lib

    bundle_xcframeworks {
	    xcframework_platform_ios_simulator {
		    OPENSSL_LIBS_DIRECTORY = $$PWD/build/ios/openssl-xc/simulator/lib
		} else {
		    OPENSSL_LIBS_DIRECTORY = $$PWD/build/ios/openssl-xc/lib
		}
	}

}

core_windows {
    LIBS += $$OPENSSL_LIBS_DIRECTORY/libssl.lib
	LIBS += $$OPENSSL_LIBS_DIRECTORY/libcrypto.lib
} else {
    LIBS += $$OPENSSL_LIBS_DIRECTORY/libssl.a
	LIBS += $$OPENSSL_LIBS_DIRECTORY/libcrypto.a
}

INCLUDEPATH += $$OPENSSL_LIBS_DIRECTORY/../include

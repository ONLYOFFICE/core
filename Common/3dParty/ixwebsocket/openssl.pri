OPENSSL_LIBS_DIRECTORY = $$PWD/../openssl/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib

core_android {

    OPENSSL_ABI_PATH = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "android_", "")
    contains(OPENSSL_ABI_PATH, "armv7" ) {
        OPENSSL_ABI_PATH = $$replace(OPENSSL_ABI_PATH, "armv7", "armeabi-v7a")
    }
    contains(OPENSSL_ABI_PATH, "arm64_v8a" ) {
        OPENSSL_ABI_PATH = $$replace(OPENSSL_ABI_PATH, "arm64_v8a", "arm64-v8a")
    }

    OPENSSL_LIBS_DIRECTORY = $$PWD/../openssl/build/android/$$OPENSSL_ABI_PATH/lib
}

core_ios {

    OPENSSL_LIBS_DIRECTORY = $$PWD/../openssl/build/ios/openssl-universal/lib

    bundle_xcframeworks {
        xcframework_platform_ios_simulator {
            OPENSSL_LIBS_DIRECTORY = $$PWD/../openssl/build/ios/openssl-xc/simulator/lib
        } else {
            OPENSSL_LIBS_DIRECTORY = $$PWD/../openssl/build/ios/openssl-xc/lib
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

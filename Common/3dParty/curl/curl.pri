core_android {

    ABI_PATH = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "android_", "")
    contains(ABI_PATH, "armv7" ) {
		ABI_PATH = $$replace(ABI_PATH, "armv7", "armeabi-v7a")
	}
	contains(ABI_PATH, "arm64_v8a" ) {
		ABI_PATH = $$replace(ABI_PATH, "arm64_v8a", "arm64-v8a")
	}
    INCLUDEPATH += \
    	$$PWD/build/android/$$ABI_PATH/include \
    	$$PWD/../openssl/build/android/$$ABI_PATH/include \

    LIBS        += \
    	$$PWD/build/android/$$ABI_PATH/lib/libcurl.a \
    	$$PWD/../openssl/build/android/$$ABI_PATH/lib/libssl.a \
    	$$PWD/../openssl/build/android/$$ABI_PATH/lib/libcrypto.a \
}

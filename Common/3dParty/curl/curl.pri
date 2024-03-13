core_android {
    INCLUDEPATH += \
    	$$PWD/build/android/include \
    	$$PWD/../openssl/build/android/$$CORE_BUILDS_PLATFORM_PREFIX_DST/include

    LIBS        += \
    	$$PWD/build/android/$$CORE_BUILDS_PLATFORM_PREFIX_DST/libcurl.a \
    	$$PWD/../openssl/build/android/$$CORE_BUILDS_PLATFORM_PREFIX_DST/lib/libssl.a \
    	$$PWD/../openssl/build/android/$$CORE_BUILDS_PLATFORM_PREFIX_DST/lib/libcrypto.a \
}

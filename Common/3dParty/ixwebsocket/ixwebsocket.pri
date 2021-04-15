core_android {

    ABI_PATH = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "android_", "")
    contains(ABI_PATH, "armv7" ) {
		ABI_PATH = $$replace(ABI_PATH, "armv7", "armeabi-v7a")
	}
	contains(ABI_PATH, "arm64_v8a" ) {
		ABI_PATH = $$replace(ABI_PATH, "arm64_v8a", "arm64-v8a")
	}

    INCLUDEPATH += \
    	$$PWD/../ixwebsocket/IXWebSocket/build/android/$$ABI_PATH/include \

    LIBS        += \
    	$$PWD/../ixwebsocket/IXWebSocket/build/android/$$ABI_PATH/lib/libixwebsocket.a \
}

core_ios {

    INCLUDEPATH += \
        $$PWD/../ixwebsocket/IXWebSocket/build/ios/ixwebsocket-universal/include \

    LIBS        += \
        $$PWD/../openssl/build/ios/openssl-universal/lib/libcrypto.a \
        $$PWD/../openssl/build/ios/openssl-universal/lib/libssl.a \
        $$PWD/../ixwebsocket/IXWebSocket/build/ios/ixwebsocket-universal/lib/libixwebsocket.a \

}

core_linux {

    INCLUDEPATH += \
        $$PWD/../ixwebsocket/IXWebSocket/build/linux_64/include \

    LIBS +=  $$PWD/../ixwebsocket/IXWebSocket/build/linux_64/lib/libixwebsocket.a
}

core_mac {
    
}

core_windows {
    
}

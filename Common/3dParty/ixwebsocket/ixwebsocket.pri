core_android {

    ABI_PATH = $$replace(CORE_BUILDS_PLATFORM_PREFIX, "android_", "")
    contains(ABI_PATH, "armv7" ) {
		ABI_PATH = $$replace(ABI_PATH, "armv7", "armeabi-v7a")
	}
	contains(ABI_PATH, "arm64_v8a" ) {
		ABI_PATH = $$replace(ABI_PATH, "arm64_v8a", "arm64-v8a")
	}

    INCLUDEPATH += $$PWD/../ixwebsocket/IXWebSocket/build/android/$$ABI_PATH/include
    LIBS        += $$PWD/../ixwebsocket/IXWebSocket/build/android/$$ABI_PATH/lib/libixwebsocket.a
}

core_ios {

    INCLUDEPATH += $$PWD/../ixwebsocket/IXWebSocket/build/ios/openssl-universal/include
    LIBS        += $$PWD/../ixwebsocket/IXWebSocket/build/ios/ixwebsocket-universal/lib/libixwebsocket.a

}

core_linux {

    INCLUDEPATH += $$PWD/../ixwebsocket/IXWebSocket/build/linux/$$CORE_BUILDS_PLATFORM_PREFIX/include
    LIBS        += $$PWD/../ixwebsocket/IXWebSocket/build/linux/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libixwebsocket.a

}

core_mac {

    INCLUDEPATH += $$PWD/../ixwebsocket/IXWebSocket/build/mac/$$CORE_BUILDS_PLATFORM_PREFIX/include
    LIBS        += $$PWD/../ixwebsocket/IXWebSocket/build/mac/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libixwebsocket.a \
}

core_windows {

    WINDOWS_DIR_WEBSOCKET=windows
    core_debug:WINDOWS_DIR_WEBSOCKET=windows_debug

    INCLUDEPATH += $$PWD/../ixwebsocket/IXWebSocket/build/$$WINDOWS_DIR_WEBSOCKET/$$CORE_BUILDS_PLATFORM_PREFIX/include

    LIBS += $$PWD/../ixwebsocket/IXWebSocket/build/$$WINDOWS_DIR_WEBSOCKET/$$CORE_BUILDS_PLATFORM_PREFIX/lib/ixwebsocket.lib
    LIBS += -lwsock32
    LIBS += -lws2_32
    LIBS += -lIphlpapi
    LIBS += -lcrypt32
    LIBS += -lUser32
    LIBS += -lshlwapi
    
}

core_windows {
    LIBS += $$PWD/../openssl/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libssl.lib
    LIBS += $$PWD/../openssl/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libcrypto.lib
} else {
    LIBS += $$PWD/../openssl/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libssl.a
    LIBS += $$PWD/../openssl/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib/libcrypto.a
}

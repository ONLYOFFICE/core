use_system_cryptopp {
    LIBS += -lcryptopp
} else {
    DEFINES += CRYPTOPP_DISABLE_ASM
    INCLUDEPATH += $$PWD/..
    LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib
}

INCLUDEPATH += $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/include

core_windows:LIBS += -L$$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib -llibcrypto -llibssl
!core_windows:LIBS += -L$$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX/lib -lcrypto -lssl

open_ssl_common {
    DEFINES += COMMON_OPENSSL_BUILDING

    HEADERS += $$PWD/common/common_openssl.h
    SOURCES += $$PWD/common/common_openssl.cpp
}

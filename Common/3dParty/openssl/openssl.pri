OPEN_SSL_PLATFORM=$$CORE_BUILDS_PLATFORM_PREFIX
build_xp:OPEN_SSL_PLATFORM=$$join(OPEN_SSL_PLATFORM, OPEN_SSL_PLATFORM, "", "_xp")

INCLUDEPATH += $$PWD/build/$$OPEN_SSL_PLATFORM/include

core_windows:LIBS += -L$$PWD/build/$$OPEN_SSL_PLATFORM/lib -llibcrypto -llibssl
!core_windows:LIBS += -L$$PWD/build/$$OPEN_SSL_PLATFORM/lib -lcrypto -lssl

open_ssl_common {
    DEFINES += COMMON_OPENSSL_BUILDING

    HEADERS += $$PWD/common/common_openssl.h
    SOURCES += $$PWD/common/common_openssl.cpp
}

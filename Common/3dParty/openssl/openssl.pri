core_linux {

INCLUDEPATH += $$PWD/openssl/include

LIBS += $$PWD/openssl/libssl.a
LIBS += $$PWD/openssl/libcrypto.a

}

core_mac {

INCLUDEPATH += $$PWD/openssl/include

LIBS += $$PWD/openssl/libssl.a
LIBS += $$PWD/openssl/libcrypto.a

}

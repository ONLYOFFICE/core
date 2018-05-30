core_linux {

INCLUDEPATH += $$PWD/openssl/include

LIBS += -L$$PWD/openssl/libssl.a
LIBS += -L$$PWD/openssl/libcrypto.a

}

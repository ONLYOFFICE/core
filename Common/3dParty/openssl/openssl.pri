core_linux {

#DEFINES += XMLSEC_OPENSSL_110
#INCLUDEPATH += $$CORE_ROOT_DIR/Common/3dParty/openssl/openssl/include
#LIBS += -L$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl -lssl
#LIBS += -L$$CORE_ROOT_DIR/Common/3dParty/openssl/openssl -lcrypto

#INCLUDEPATH += /usr/include/openssl
LIBS += -lssl
LIBS += -lcrypto

}

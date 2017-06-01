#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = libxmlsec
TEMPLATE = lib
QMAKE_CXXFLAGS += -Wall -g

#CONFIG += shared
#CONFIG += plugin
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

INCLUDEPATH += \
    $$PWD_ROOT_DIR/xmlsec/include \
    $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt

DEFINES += \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED \
    LIBXML_HTML_ENABLED \
    LIBXML_XPATH_ENABLED \
    LIBXML_OUTPUT_ENABLED \
    LIBXML_C14N_ENABLED \
    LIBXML_SAX1_ENABLED \
    LIBXML_TREE_ENABLED \
    LIBXML_XPTR_ENABLED \
    IN_LIBXML \
    LIBXML_STATIC

include($$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2_src.pri)

DEFINES += PACKAGE=\\\"xmlsec1\\\"
DEFINES += VERSION=\\\"1.2.23\\\"
DEFINES += XMLSEC_DEFAULT_CRYPTO=\\\"openssl\\\"

DEFINES += \
    IN_XMLSEC \
    XMLSEC_STATIC

core_linux {
    #CONFIG += use_gcrypt
    #CONFIG += use_gnutls
    #CONFIG += use_mscrypto
    #CONFIG += use_nss
    CONFIG += use_openssl
    #CONFIG += use_skeleton
    #CONFIG += use_xslt
}

core_windows {
    CONFIG += use_mscrypto
}

HEADERS += \
    xmlsec/include/xmlsec/app.h \
    xmlsec/include/xmlsec/base64.h \
    xmlsec/include/xmlsec/bn.h \
    xmlsec/include/xmlsec/buffer.h \
    xmlsec/include/xmlsec/crypto.h \
    xmlsec/include/xmlsec/dl.h \
    xmlsec/include/xmlsec/errors.h \
    xmlsec/include/xmlsec/exports.h \
    xmlsec/include/xmlsec/io.h \
    xmlsec/include/xmlsec/keyinfo.h \
    xmlsec/include/xmlsec/keys.h \
    xmlsec/include/xmlsec/keysdata.h \
    xmlsec/include/xmlsec/keysmngr.h \
    xmlsec/include/xmlsec/list.h \
    xmlsec/include/xmlsec/membuf.h \
    xmlsec/include/xmlsec/nodeset.h \
    xmlsec/include/xmlsec/parser.h \
    xmlsec/include/xmlsec/private.h \
    xmlsec/include/xmlsec/soap.h \
    xmlsec/include/xmlsec/strings.h \
    xmlsec/include/xmlsec/templates.h \
    xmlsec/include/xmlsec/transforms.h \
    xmlsec/include/xmlsec/version.h \
    xmlsec/include/xmlsec/version.h.in \
    xmlsec/include/xmlsec/x509.h \
    xmlsec/include/xmlsec/xmldsig.h \
    xmlsec/include/xmlsec/xmlenc.h \
    xmlsec/include/xmlsec/xmlsec.h \
    xmlsec/include/xmlsec/xmltree.h \
    xmlsec/src/globals.h \
    xmlsec/src/kw_aes_des.h

SOURCES += \
    xmlsec/src/app.c \
    xmlsec/src/base64.c \
    xmlsec/src/bn.c \
    xmlsec/src/buffer.c \
    xmlsec/src/c14n.c \
    xmlsec/src/dl.c \
    xmlsec/src/enveloped.c \
    xmlsec/src/errors.c \
    xmlsec/src/io.c \
    xmlsec/src/keyinfo.c \
    xmlsec/src/keys.c \
    xmlsec/src/keysdata.c \
    xmlsec/src/keysmngr.c \
    xmlsec/src/kw_aes_des.c \
    xmlsec/src/list.c \
    xmlsec/src/membuf.c \
    xmlsec/src/nodeset.c \
    xmlsec/src/parser.c \
    xmlsec/src/relationship.c \
    xmlsec/src/soap.c \
    xmlsec/src/strings.c \
    xmlsec/src/templates.c \
    xmlsec/src/transforms.c \
    xmlsec/src/x509.c \
    xmlsec/src/xmldsig.c \
    xmlsec/src/xmlenc.c \
    xmlsec/src/xmlsec.c \
    xmlsec/src/xmltree.c \
    xmlsec/src/xpath.c

use_gcrypt {

HEADERS += \
    xmlsec/include/xmlsec/gcrypt/app.h \
    xmlsec/include/xmlsec/gcrypt/crypto.h \
    xmlsec/include/xmlsec/gcrypt/symbols.h \
    \
    xmlsec/src/gcrypt/asn1.h \
    xmlsec/src/gcrypt/globals.h


SOURCES += \
    xmlsec/src/gcrypt/app.c \
    xmlsec/src/gcrypt/asn1.c \
    xmlsec/src/gcrypt/asymkeys.c \
    xmlsec/src/gcrypt/ciphers.c \
    xmlsec/src/gcrypt/crypto.c \
    xmlsec/src/gcrypt/digests.c \
    xmlsec/src/gcrypt/hmac.c \
    xmlsec/src/gcrypt/kw_aes.c \
    xmlsec/src/gcrypt/kw_des.c \
    xmlsec/src/gcrypt/signatures.c \
    xmlsec/src/gcrypt/symkeys.c

}

use_gnutls {

HEADERS += \
    xmlsec/include/xmlsec/gnutls/app.h \
    xmlsec/include/xmlsec/gnutls/crypto.h \
    xmlsec/include/xmlsec/gnutls/symbols.h \
    xmlsec/include/xmlsec/gnutls/x509.h \
    \
    xmlsec/src/gnutls/globals.h \
    xmlsec/src/gnutls/x509utils.h

SOURCES += \
    xmlsec/src/gnutls/app.c \
    xmlsec/src/gnutls/asymkeys.c \
    xmlsec/src/gnutls/ciphers.c \
    xmlsec/src/gnutls/crypto.c \
    xmlsec/src/gnutls/digests.c \
    xmlsec/src/gnutls/hmac.c \
    xmlsec/src/gnutls/kw_aes.c \
    xmlsec/src/gnutls/kw_des.c \
    xmlsec/src/gnutls/signatures.c \
    xmlsec/src/gnutls/symkeys.c \
    xmlsec/src/gnutls/x509.c \
    xmlsec/src/gnutls/x509utils.c \
    xmlsec/src/gnutls/x509vfy.c

}

use_mscrypto {

DEFINES += XMLSEC_CRYPTO_MSCRYPTO

HEADERS += \
    xmlsec/include/xmlsec/mscrypto/app.h \
    xmlsec/include/xmlsec/mscrypto/certkeys.h \
    xmlsec/include/xmlsec/mscrypto/crypto.h \
    xmlsec/include/xmlsec/mscrypto/keysstore.h \
    xmlsec/include/xmlsec/mscrypto/symbols.h \
    xmlsec/include/xmlsec/mscrypto/x509.h \
    \
    xmlsec/src/mscrypto/csp_calg.h \
    xmlsec/src/mscrypto/csp_oid.h \
    xmlsec/src/mscrypto/globals.h \
    xmlsec/src/mscrypto/private.h \
    xmlsec/src/mscrypto/xmlsec-mingw.h

SOURCES += \
    xmlsec/src/mscrypto/app.c \
    xmlsec/src/mscrypto/certkeys.c \
    xmlsec/src/mscrypto/ciphers.c \
    xmlsec/src/mscrypto/crypto.c \
    xmlsec/src/mscrypto/digests.c \
    xmlsec/src/mscrypto/hmac.c \
    xmlsec/src/mscrypto/keysstore.c \
    xmlsec/src/mscrypto/kt_rsa.c \
    xmlsec/src/mscrypto/kw_aes.c \
    xmlsec/src/mscrypto/kw_des.c \
    xmlsec/src/mscrypto/signatures.c \
    xmlsec/src/mscrypto/symkeys.c \
    xmlsec/src/mscrypto/x509.c \
    xmlsec/src/mscrypto/x509vfy.c

}

use_nss {

HEADERS += \
    xmlsec/include/xmlsec/nss/app.h \
    xmlsec/include/xmlsec/nss/bignum.h \
    xmlsec/include/xmlsec/nss/crypto.h \
    xmlsec/include/xmlsec/nss/keysstore.h \
    xmlsec/include/xmlsec/nss/pkikeys.h \
    xmlsec/include/xmlsec/nss/symbols.h \
    xmlsec/include/xmlsec/nss/x509.h \
    \
    xmlsec/src/nss/globals.h

SOURCES += \
    xmlsec/src/nss/app.c \
    xmlsec/src/nss/bignum.c \
    xmlsec/src/nss/ciphers.c \
    xmlsec/src/nss/crypto.c \
    xmlsec/src/nss/digests.c \
    xmlsec/src/nss/hmac.c \
    xmlsec/src/nss/keysstore.c \
    xmlsec/src/nss/keytrans.c \
    xmlsec/src/nss/kw_aes.c \
    xmlsec/src/nss/kw_des.c \
    xmlsec/src/nss/pkikeys.c \
    xmlsec/src/nss/signatures.c \
    xmlsec/src/nss/symkeys.c \
    xmlsec/src/nss/x509.c \
    xmlsec/src/nss/x509vfy.c

}

use_openssl {

    DEFINES += XMLSEC_OPENSSL_110

    INCLUDEPATH += $$PWD/openssl/include

HEADERS += \
    xmlsec/include/xmlsec/openssl/app.h \
    xmlsec/include/xmlsec/openssl/bn.h \
    xmlsec/include/xmlsec/openssl/crypto.h \
    xmlsec/include/xmlsec/openssl/evp.h \
    xmlsec/include/xmlsec/openssl/symbols.h \
    xmlsec/include/xmlsec/openssl/x509.h \
    \
    xmlsec/src/openssl/globals.h \
    xmlsec/src/openssl/openssl11_wrapper.h

SOURCES += \
    xmlsec/src/openssl/_app.c \
    xmlsec/src/openssl/_bn.c \
    xmlsec/src/openssl/_ciphers.c \
    xmlsec/src/openssl/_crypto.c \
    xmlsec/src/openssl/_digests.c \
    xmlsec/src/openssl/_evp.c \
    xmlsec/src/openssl/_evp_signatures.c \
    xmlsec/src/openssl/_hmac.c \
    xmlsec/src/openssl/_kt_rsa.c \
    xmlsec/src/openssl/_kw_aes.c \
    xmlsec/src/openssl/_kw_des.c \
    xmlsec/src/openssl/_signatures.c \
    xmlsec/src/openssl/_symkeys.c \
    xmlsec/src/openssl/_x509.c \
    xmlsec/src/openssl/_x509vfy.c

}

use_skeleton {

HEADERS += \
    xmlsec/include/xmlsec/skeleton/app.h \
    xmlsec/include/xmlsec/skeleton/crypto.h \
    xmlsec/include/xmlsec/skeleton/symbols.h \
    \
    xmlsec/src/skeleton/globals.h

SOURCES += \
    xmlsec/src/skeleton/app.c \
    xmlsec/src/skeleton/crypto.c

}

use_xslt {

HEADERS += \
    xmlsec/include/xmlsec/private/xslt.h

SOURCES += \
    xmlsec/src/xslt.c

} else {

DEFINES += \
    XMLSEC_NO_XSLT

}

/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 * Copyright (C) 2003-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <windows.h>
#include <wincrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/mscrypto/app.h>
#include <xmlsec/mscrypto/crypto.h>
#include <xmlsec/mscrypto/certkeys.h>
#include <xmlsec/mscrypto/keysstore.h>
#include <xmlsec/mscrypto/x509.h>
#include "private.h"


/* I don't see any other way then to use a global var to get the
 * config info to the mscrypto keysstore :(  WK
 */
static LPTSTR gXmlSecMSCryptoAppCertStoreName = NULL;

/**
 * xmlSecMSCryptoAppInit:
 * @config:             the name of another then the default ms certificate store.
 *
 * General crypto engine initialization. This function is used
 * by XMLSec command line utility and called before
 * @xmlSecInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppInit(const char* config) {
    /* initialize MSCrypto crypto engine */

    /* config parameter can contain *another* ms certs store name
     * then the default (MY)
     */
    if (NULL != config && strlen(config) > 0) {
        if (gXmlSecMSCryptoAppCertStoreName != NULL) {
            /* This should not happen, initialize twice */
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "config=%s, config already set",
                        xmlSecErrorsSafeString(config));
            return (-1);
        }

#ifdef UNICODE
        gXmlSecMSCryptoAppCertStoreName = xmlSecMSCryptoConvertLocaleToUnicode(config);
        if (gXmlSecMSCryptoAppCertStoreName == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        "xmlSecMSCryptoConvertLocaleToUnicode",
                        NULL,
                        XMLSEC_ERRORS_R_MALLOC_FAILED,
                        "config=%s",
                        xmlSecErrorsSafeString(config));
            return (-1);
        }
#else  /* UNICODE */
        gXmlSecMSCryptoAppCertStoreName = xmlStrdup(config);
        if (gXmlSecMSCryptoAppCertStoreName == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        "xmlStrdup",
                        NULL,
                        XMLSEC_ERRORS_R_MALLOC_FAILED,
                        "config=%s",
                        xmlSecErrorsSafeString(config));
            return (-1);
        }
#endif /* UNICODE */
    }

    return(0);
}

/**
 * xmlSecMSCryptoAppShutdown:
 *
 * General crypto engine shutdown. This function is used
 * by XMLSec command line utility and called after
 * @xmlSecShutdown function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppShutdown(void) {
    /* shutdown MSCrypto crypto engine */
    if (NULL != gXmlSecMSCryptoAppCertStoreName) {
        xmlFree(gXmlSecMSCryptoAppCertStoreName);
        gXmlSecMSCryptoAppCertStoreName = NULL;
    }
    return(0);
}

/**
 * xmlSecMSCryptoAppGetCertStoreName:
 *
 * Gets the MS Crypto certs store name set by @xmlSecMSCryptoAppInit function.
 *
 * Returns: the MS Crypto certs name used by xmlsec-mscrypto.
 */
LPCTSTR
xmlSecMSCryptoAppGetCertStoreName(void) {
    return(gXmlSecMSCryptoAppCertStoreName);
}

/*************************************************************************************
 * Keys
 *************************************************************************************/

/**
 * xmlSecMSCryptoAppKeyLoad:
 * @filename:           the key filename.
 * @format:             the key file format.
 * @pwd:                the key file password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from the a file.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecMSCryptoAppKeyLoad(const char *filename, xmlSecKeyDataFormat format,
                         const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    xmlSecBuffer buffer;
    xmlSecKeyPtr key = NULL;
    int ret;

    xmlSecAssert2(filename != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    switch (format) {
    case xmlSecKeyDataFormatPkcs12:
        key = xmlSecMSCryptoAppPkcs12Load(filename, pwd, pwdCallback, pwdCallbackCtx);
        if(key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoAppPkcs12Load",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }
        break;
    case xmlSecKeyDataFormatCertDer:
        ret = xmlSecBufferInitialize(&buffer, 0);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBufferInitialize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }

        ret = xmlSecBufferReadFile(&buffer, filename);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBufferReadFile",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "filename=%s",
                        xmlSecErrorsSafeString(filename));
            xmlSecBufferFinalize(&buffer);
            return (NULL);
        }

        key = xmlSecMSCryptoAppKeyLoadMemory(xmlSecBufferGetData(&buffer),
                                        xmlSecBufferGetSize(&buffer), format,
                                        pwd, pwdCallback, pwdCallbackCtx);
        if(key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoAppKeyLoadMemory",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecBufferFinalize(&buffer);
            return(NULL);
        }
        xmlSecBufferFinalize(&buffer);
        break;
    default:
        /* Any other format like PEM keys is currently not supported */
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", format);
        return(NULL);
    }

    return(key);
}

/**
 * xmlSecMSCryptoAppKeyLoadMemory:
 * @data:               the key binary data.
 * @dataSize:           the key data size.
 * @format:             the key format.
 * @pwd:                the key password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from the a file.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecMSCryptoAppKeyLoadMemory(const xmlSecByte* data, xmlSecSize dataSize, xmlSecKeyDataFormat format,
                               const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    PCCERT_CONTEXT pCert = NULL;
    PCCERT_CONTEXT tmpcert = NULL;
    xmlSecKeyDataPtr x509Data = NULL;
    xmlSecKeyDataPtr keyData = NULL;
    xmlSecKeyPtr key = NULL;
    xmlSecKeyPtr res = NULL;
    int ret;

    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(dataSize > 0, NULL);
    xmlSecAssert2(format == xmlSecKeyDataFormatCertDer, NULL);

    pCert = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, data, dataSize);
    if (NULL == pCert) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CertCreateCertificateContext",
                    XMLSEC_ERRORS_R_IO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    x509Data = xmlSecKeyDataCreate(xmlSecMSCryptoKeyDataX509Id);
    if(x509Data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "transform=%s",
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecMSCryptoKeyDataX509Id)));
        goto done;
    }

    tmpcert = CertDuplicateCertificateContext(pCert);
    if(tmpcert == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CertDuplicateCertificateContext",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
        goto done;
    }

    ret = xmlSecMSCryptoKeyDataX509AdoptKeyCert(x509Data, tmpcert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoKeyDataX509AdoptKeyCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
        CertFreeCertificateContext(tmpcert);
        goto done;
    }
    tmpcert = NULL;

    keyData = xmlSecMSCryptoCertAdopt(pCert, xmlSecKeyDataTypePublic);
    if(keyData == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoCertAdopt",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    pCert = NULL;

    key = xmlSecKeyCreate();
    if(key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xmlSecKeySetValue(key, keyData);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
        goto done;
    }
    keyData = NULL;

    ret = xmlSecKeyAdoptData(key, x509Data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyAdoptData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
        goto done;
    }
    x509Data = NULL;

    /* success */
    res = key;
    key = NULL;
done:
    if(pCert != NULL) {
        CertFreeCertificateContext(pCert);
    }
    if(tmpcert != NULL) {
        CertFreeCertificateContext(tmpcert);
    }
    if(x509Data != NULL) {
        xmlSecKeyDataDestroy(x509Data);
    }
    if(keyData != NULL) {
        xmlSecKeyDataDestroy(keyData);
    }
    if(key != NULL) {
        xmlSecKeyDestroy(key);
    }
    return(res);
}


/**********************************************************************************
 * X509 certificates
 **********************************************************************************/

#ifndef XMLSEC_NO_X509

/**
 * xmlSecMSCryptoAppKeyCertLoad:
 * @key:                the pointer to key.
 * @filename:           the certificate filename.
 * @format:             the certificate file format.
 *
 * Reads the certificate from $@filename and adds it to key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */

int
xmlSecMSCryptoAppKeyCertLoad(xmlSecKeyPtr key, const char* filename,
                             xmlSecKeyDataFormat format) {
    xmlSecBuffer buffer;
    int ret;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    ret = xmlSecBufferInitialize(&buffer, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecBufferReadFile(&buffer, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferReadFile",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(filename));
        xmlSecBufferFinalize(&buffer);
        return (-1);
    }

    ret = xmlSecMSCryptoAppKeyCertLoadMemory(key, xmlSecBufferGetData(&buffer),
                    xmlSecBufferGetSize(&buffer), format);
    if (ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoAppKeyCertLoadMemory",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBufferFinalize(&buffer);
        return(-1);
    }

    xmlSecBufferFinalize(&buffer);
    return(0);
}

/**
 * xmlSecMSCryptoAppKeyCertLoadMemory:
 * @key:                the pointer to key.
 * @data:               the binary certificate.
 * @dataSize:           size of certificate binary (data)
 * @format:             the certificate file format.
 *
 * Reads the certificate from $@data and adds it to key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppKeyCertLoadMemory(xmlSecKeyPtr key, const xmlSecByte* data, xmlSecSize dataSize,
                                   xmlSecKeyDataFormat format) {
    PCCERT_CONTEXT pCert;
    xmlSecKeyDataPtr kdata;
    int ret;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    kdata = xmlSecKeyEnsureData(key, xmlSecMSCryptoKeyDataX509Id);
    if(kdata == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyEnsureData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "transform=%s",
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecMSCryptoKeyDataX509Id)));
        return(-1);
    }

    /* For now only DER certificates are supported */
    /* adjust cert format */
    switch(format) {
    case xmlSecKeyDataFormatDer:
    case xmlSecKeyDataFormatCertDer:
        pCert = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, data, dataSize);
        if (NULL == pCert) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CertCreateCertificateContext",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "format=%d", format);
            return(-1);
        }

        ret = xmlSecMSCryptoKeyDataX509AdoptCert(kdata, pCert);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeyDataX509AdoptCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(kdata)));
            CertFreeCertificateContext(pCert);
            return(-1);
        }
        break;
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", (int)format);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecMSCryptoAppPkcs12Load:
 * @filename:           the PKCS12 key filename.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 file
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecMSCryptoAppPkcs12Load(const char *filename,
                            const char *pwd,
                            void* pwdCallback ATTRIBUTE_UNUSED,
                            void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    xmlSecBuffer buffer;
    xmlSecKeyPtr key;
    int ret;

    xmlSecAssert2(filename != NULL, NULL);
    xmlSecAssert2(pwd != NULL, NULL);

    ret = xmlSecBufferInitialize(&buffer, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    ret = xmlSecBufferReadFile(&buffer, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferReadFile",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(filename));
        xmlSecBufferFinalize(&buffer);
        return (NULL);
    }
    if(xmlSecBufferGetData(&buffer) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_DATA,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBufferFinalize(&buffer);
        return(NULL);
    }

    key = xmlSecMSCryptoAppPkcs12LoadMemory(xmlSecBufferGetData(&buffer),
                                            xmlSecBufferGetSize(&buffer), pwd,
                                            pwdCallback, pwdCallbackCtx);
    if (key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoAppPkcs12LoadMemory",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBufferFinalize(&buffer);
        return(NULL);
    }

    xmlSecBufferFinalize(&buffer);
    return(key);
}

/**
 * xmlSecMSCryptoAppPkcs12LoadMemory:
 * @data:               the binary PKCS12 key in data.
 * @dataSize:           size of binary pkcs12 data
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 binary
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecMSCryptoAppPkcs12LoadMemory(const xmlSecByte* data,
                                  xmlSecSize dataSize,
                                  const char *pwd,
                                  void* pwdCallback ATTRIBUTE_UNUSED,
                                  void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    CRYPT_DATA_BLOB pfx;
    HCERTSTORE hCertStore = NULL;
    PCCERT_CONTEXT tmpcert = NULL;
    PCCERT_CONTEXT pCert = NULL;
    WCHAR* wcPwd = NULL;
    xmlSecKeyDataPtr x509Data = NULL;
    xmlSecKeyDataPtr keyData = NULL;
    xmlSecKeyPtr key = NULL;
        int ret;

    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(dataSize > 1, NULL);
    xmlSecAssert2(pwd != NULL, NULL);

    memset(&pfx, 0, sizeof(pfx));
    pfx.pbData = (BYTE *)data;
    pfx.cbData = dataSize;

    if(FALSE == PFXIsPFXBlob(&pfx)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PFXIsPFXBlob",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "size=%ld",
                    pfx.cbData);
        goto done;
    }

    wcPwd = xmlSecMSCryptoConvertLocaleToUnicode(pwd);
    if (wcPwd == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoConvertLocaleToUnicode",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "wcPwd");
        goto done;
    }

    if (FALSE == PFXVerifyPassword(&pfx, wcPwd, 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PFXVerifyPassword",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    hCertStore = PFXImportCertStore(&pfx, wcPwd, CRYPT_EXPORTABLE | PKCS12_NO_PERSIST_KEY);
    if (NULL == hCertStore) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PFXImportCertStore",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    x509Data = xmlSecKeyDataCreate(xmlSecMSCryptoKeyDataX509Id);
    if(x509Data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "transform=%s",
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecMSCryptoKeyDataX509Id)));
        goto done;
    }

    while (pCert = CertEnumCertificatesInStore(hCertStore, pCert)) {
        DWORD dwData = 0;
        DWORD dwDataLen = sizeof(DWORD);

        /* Find the certificate that has the private key */
        if((TRUE == CertGetCertificateContextProperty(pCert, CERT_KEY_SPEC_PROP_ID, &dwData, &dwDataLen)) && (dwData > 0)) {
            tmpcert = CertDuplicateCertificateContext(pCert);
            if(tmpcert == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CertDuplicateCertificateContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "data=%s",
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
                goto done;
            }

            keyData = xmlSecMSCryptoCertAdopt(tmpcert, xmlSecKeyDataTypePrivate | xmlSecKeyDataTypePublic);
            if(keyData == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecMSCryptoCertAdopt",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        tmpcert = NULL;

            tmpcert = CertDuplicateCertificateContext(pCert);
            if(tmpcert == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CertDuplicateCertificateContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "data=%s",
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
                goto done;
            }

            ret = xmlSecMSCryptoKeyDataX509AdoptKeyCert(x509Data, tmpcert);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoKeyDataX509AdoptKeyCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
                goto done;
            }
            tmpcert = NULL;
        }

        /* load certificate in the x509 key data */
        tmpcert = CertDuplicateCertificateContext(pCert);
        if(tmpcert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CertDuplicateCertificateContext",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }

        ret = xmlSecMSCryptoKeyDataX509AdoptCert(x509Data, tmpcert);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeyDataX509AdoptCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }
        tmpcert = NULL;
    }

    if (keyData == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoAppPkcs12Load",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "private key not found in PKCS12 file");
        goto done;
    }

    key = xmlSecKeyCreate();
    if(key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xmlSecKeySetValue(key, keyData);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
        xmlSecKeyDestroy(key);
        key = NULL;
        goto done;
    }
    keyData = NULL;

    ret = xmlSecKeyAdoptData(key, x509Data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyAdoptData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
        xmlSecKeyDestroy(key);
        key = NULL;
        goto done;
    }
    x509Data = NULL;

done:
    if(hCertStore != NULL) {
        CertCloseStore(hCertStore, 0);
    }
    if(wcPwd != NULL) {
        xmlFree(wcPwd);
    }
    if(x509Data != NULL) {
        xmlSecKeyDataDestroy(x509Data);
    }
    if(keyData != NULL) {
        xmlSecKeyDataDestroy(keyData);
    }
    if(tmpcert != NULL) {
        CertFreeCertificateContext(tmpcert);
    }
    return(key);
}

/**
 * xmlSecMSCryptoAppKeysMngrCertLoad:
 * @mngr:               the keys manager.
 * @filename:           the certificate file.
 * @format:             the certificate file format.
 * @type:               the flag that indicates is the certificate in @filename
 *                      trusted or not.
 *
 * Reads cert from @filename and adds to the list of trusted or known
 * untrusted certs in @store (not implemented yet).
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppKeysMngrCertLoad(xmlSecKeysMngrPtr mngr, const char *filename,
                                xmlSecKeyDataFormat format,
                                xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecBuffer buffer;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    ret = xmlSecBufferInitialize(&buffer, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecBufferReadFile(&buffer, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferReadFile",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(filename));
        xmlSecBufferFinalize(&buffer);
        return (-1);
    }

    ret = xmlSecMSCryptoAppKeysMngrCertLoadMemory(mngr, xmlSecBufferGetData(&buffer),
        xmlSecBufferGetSize(&buffer), format, type);
    if (ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoAppKeysMngrCertLoadMemory",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(filename));
        xmlSecBufferFinalize(&buffer);
        return(-1);
    }

    xmlSecBufferFinalize(&buffer);
    return(ret);
}

/**
 * xmlSecMSCryptoAppKeysMngrCertLoadMemory:
 * @mngr:               the keys manager.
 * @data:               the binary certificate.
 * @dataSize:           size of binary certificate (data)
 * @format:             the certificate file format.
 * @type:               the flag that indicates is the certificate in @filename
 *                      trusted or not.
 *
 * Reads cert from @data and adds to the list of trusted or known
 * untrusted certs in @store.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppKeysMngrCertLoadMemory(xmlSecKeysMngrPtr mngr, const xmlSecByte* data,
                                        xmlSecSize dataSize, xmlSecKeyDataFormat format,
                                        xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecKeyDataStorePtr x509Store;
    PCCERT_CONTEXT pCert = NULL;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize > 0, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    x509Store = xmlSecKeysMngrGetDataStore(mngr, xmlSecMSCryptoX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecMSCryptoX509StoreId");
        return(-1);
    }

    switch (format) {
        case xmlSecKeyDataFormatDer:
        case xmlSecKeyDataFormatCertDer:
            pCert = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                                                 data, dataSize);
            if (NULL == pCert) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CertCreateCertificateContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return (-1);
            }
            break;
        default:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_FORMAT,
                        "format=%d", format);
            return(-1);
    }

    xmlSecAssert2(pCert != NULL, -1);
    ret = xmlSecMSCryptoX509StoreAdoptCert(x509Store, pCert, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoX509StoreAdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CertFreeCertificateContext(pCert);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrAdoptKeyStore:
 * @mngr:                       the keys manager.
 * @keyStore:           the pointer to keys store.
 *
 * Adds @keyStore to the list of key stores in the keys manager @mngr.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrAdoptKeyStore(xmlSecKeysMngrPtr mngr, HCERTSTORE keyStore)
{
        xmlSecKeyDataStorePtr x509Store ;

        xmlSecAssert2( mngr != NULL, -1 ) ;
        xmlSecAssert2( keyStore != NULL, -1 ) ;

    x509Store = xmlSecKeysMngrGetDataStore( mngr, xmlSecMSCryptoX509StoreId) ;
        if( x509Store == NULL ) {
                xmlSecError( XMLSEC_ERRORS_HERE ,
                        NULL ,
                        "xmlSecKeysMngrGetDataStore" ,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED ,
                        XMLSEC_ERRORS_NO_MESSAGE ) ;
                return(-1) ;
        }

        if( xmlSecMSCryptoX509StoreAdoptKeyStore( x509Store, keyStore ) < 0 ) {
                xmlSecError( XMLSEC_ERRORS_HERE ,
                        xmlSecErrorsSafeString( xmlSecKeyDataStoreGetName( x509Store ) ) ,
                        "xmlSecMSCryptoX509StoreAdoptKeyStore" ,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED ,
                        XMLSEC_ERRORS_NO_MESSAGE ) ;
                return(-1) ;
        }

        return (0) ;
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrAdoptTrustedStore:
 * @mngr:                       the keys manager.
 * @trustedStore:       the pointer to certs store.
 *
 * Adds @trustedStore to the list of trusted cert stores in the keys manager @mngr.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrAdoptTrustedStore(xmlSecKeysMngrPtr mngr, HCERTSTORE trustedStore)
{
        xmlSecKeyDataStorePtr x509Store ;

        xmlSecAssert2( mngr != NULL, -1 ) ;
        xmlSecAssert2( trustedStore != NULL, -1 ) ;

    x509Store = xmlSecKeysMngrGetDataStore( mngr, xmlSecMSCryptoX509StoreId ) ;
        if( x509Store == NULL ) {
                xmlSecError( XMLSEC_ERRORS_HERE ,
                        NULL ,
                        "xmlSecKeysMngrGetDataStore" ,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED ,
                        XMLSEC_ERRORS_NO_MESSAGE ) ;
                return(-1) ;
        }

        if( xmlSecMSCryptoX509StoreAdoptTrustedStore( x509Store, trustedStore ) < 0 ) {
                xmlSecError( XMLSEC_ERRORS_HERE ,
                        xmlSecErrorsSafeString( xmlSecKeyDataStoreGetName( x509Store ) ) ,
                        "xmlSecMSCryptoX509StoreAdoptKeyStore" ,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED ,
                        XMLSEC_ERRORS_NO_MESSAGE ) ;
                return(-1) ;
        }

        return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrAdoptUntrustedStore:
 * @mngr:                       the keys manager.
 * @untrustedStore:     the pointer to certs store.
 *
 * Adds @trustedStore to the list of un-trusted cert stores in the keys manager @mngr.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrAdoptUntrustedStore(xmlSecKeysMngrPtr mngr, HCERTSTORE untrustedStore)
{
        xmlSecKeyDataStorePtr x509Store ;

        xmlSecAssert2( mngr != NULL, -1 ) ;
        xmlSecAssert2( untrustedStore != NULL, -1 ) ;

    x509Store = xmlSecKeysMngrGetDataStore( mngr, xmlSecMSCryptoX509StoreId);
        if( x509Store == NULL ) {
                xmlSecError( XMLSEC_ERRORS_HERE ,
                        NULL ,
                        "xmlSecKeysMngrGetDataStore" ,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED ,
                        XMLSEC_ERRORS_NO_MESSAGE ) ;
                return(-1);
        }

        if( xmlSecMSCryptoX509StoreAdoptUntrustedStore( x509Store, untrustedStore ) < 0) {
                xmlSecError( XMLSEC_ERRORS_HERE ,
                        xmlSecErrorsSafeString( xmlSecKeyDataStoreGetName( x509Store ) ) ,
                        "xmlSecMSCryptoX509StoreAdoptKeyStore" ,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED ,
                        XMLSEC_ERRORS_NO_MESSAGE ) ;
                return(-1);
        }

        return(0) ;
}

#endif /* XMLSEC_NO_X509 */

/**
 * xmlSecMSCryptoAppDefaultKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Initializes @mngr with simple keys store #xmlSecSimpleKeysStoreId
 * and a default MSCrypto crypto key data stores.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

    /* create MSCrypto keys store if needed */
    if(xmlSecKeysMngrGetKeysStore(mngr) == NULL) {
        xmlSecKeyStorePtr keysStore;

        keysStore = xmlSecKeyStoreCreate(xmlSecMSCryptoKeysStoreId);
        if(keysStore == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyStoreCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecMSCryptoKeysStoreId");
            return(-1);
        }

        ret = xmlSecKeysMngrAdoptKeysStore(mngr, keysStore);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeysMngrAdoptKeysStore",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyStoreDestroy(keysStore);
            return(-1);
        }
    }

    ret = xmlSecMSCryptoKeysMngrInit(mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeysMngrInit",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    mngr->getKey = xmlSecKeysMngrGetKey;
    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrAdoptKey:
 * @mngr:               the pointer to keys manager.
 * @key:                the pointer to key.
 *
 * Adds @key to the keys manager @mngr created with #xmlSecMSCryptoAppDefaultKeysMngrInit
 * function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrAdoptKey(xmlSecKeysMngrPtr mngr, xmlSecKeyPtr key) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(key != NULL, -1);

    store = xmlSecKeysMngrGetKeysStore(mngr);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetKeysStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecMSCryptoKeysStoreAdoptKey(store, key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoKeysStoreAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrLoad:
 * @mngr:               the pointer to keys manager.
 * @uri:                the uri.
 *
 * Loads XML keys file from @uri to the keys manager @mngr created
 * with #xmlSecMSCryptoAppDefaultKeysMngrInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrLoad(xmlSecKeysMngrPtr mngr, const char* uri) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(uri != NULL, -1);

    store = xmlSecKeysMngrGetKeysStore(mngr);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetKeysStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecMSCryptoKeysStoreLoad(store, uri, mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoKeysStoreLoad",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "uri=%s", xmlSecErrorsSafeString(uri));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrSave:
 * @mngr:               the pointer to keys manager.
 * @filename:   the destination filename.
 * @type:               the type of keys to save (public/private/symmetric).
 *
 * Saves keys from @mngr to  XML keys file.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrSave(xmlSecKeysMngrPtr mngr, const char* filename, xmlSecKeyDataType type) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);

    store = xmlSecKeysMngrGetKeysStore(mngr);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeysMngrGetKeysStore",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecMSCryptoKeysStoreSave(store, filename, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeysStoreSave",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "filename%s", xmlSecErrorsSafeString(filename));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrPrivateKeyLoad:
 * @mngr:               the pointer to keys manager.
 * @hKey:       the key handle.
 *
 * Adds private key @hKey to the keys manager @mngr.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrPrivateKeyLoad(xmlSecKeysMngrPtr mngr, HCRYPTKEY hKey) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(hKey != 0, -1);

    /* TODO */
    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrPublicKeyLoad:
 * @mngr:               the pointer to keys manager.
 * @hKey:       the key handle.
 *
 * Adds public key @hKey to the keys manager @mngr.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrPublicKeyLoad(xmlSecKeysMngrPtr mngr, HCRYPTKEY hKey) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(hKey != 0, -1);

    /* TODO */
    return(0);
}

/**
 * xmlSecMSCryptoAppDefaultKeysMngrSymKeyLoad:
 * @mngr:               the pointer to keys manager.
 * @hKey:       the key handle.
 *
 * Adds symmetric key @hKey to the keys manager @mngr.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoAppDefaultKeysMngrSymKeyLoad(xmlSecKeysMngrPtr mngr, HCRYPTKEY hKey) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(hKey != 0, -1);

    /* TODO */
    return(0);
}

/**
 * xmlSecMSCryptoAppGetDefaultPwdCallback:
 *
 * Gets default password callback.
 *
 * Returns: default password callback.
 */
void*
xmlSecMSCryptoAppGetDefaultPwdCallback(void) {
    return(NULL);
}


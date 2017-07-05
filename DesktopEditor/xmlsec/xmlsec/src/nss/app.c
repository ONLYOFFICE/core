/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#include "globals.h"

#include <string.h>

#include <nspr.h>
#include <nss.h>
#include <pk11func.h>
#include <cert.h>
#include <keyhi.h>
#include <pkcs12.h>
#include <p12plcy.h>
/*
#include <ssl.h>
*/

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/app.h>
#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/x509.h>
#include <xmlsec/nss/pkikeys.h>
#include <xmlsec/nss/keysstore.h>

/* workaround - NSS exports this but doesn't declare it */
extern CERTCertificate * __CERT_NewTempCertificate              (CERTCertDBHandle *handle,
                                                                 SECItem *derCert,
                                                                 char *nickname,
                                                                 PRBool isperm,
                                                                 PRBool copyDER);
static int xmlSecNssAppCreateSECItem                            (SECItem *contents,
                                                                 const xmlSecByte* data,
                                                                 xmlSecSize dataSize);
static int xmlSecNssAppReadSECItem                              (SECItem *contents,
                                                                 const char *fn);
static PRBool xmlSecNssAppAscii2UCS2Conv                        (PRBool toUnicode,
                                                                 unsigned char *inBuf,
                                                                 unsigned int   inBufLen,
                                                                 unsigned char *outBuf,
                                                                 unsigned int   maxOutBufLen,
                                                                 unsigned int  *outBufLen,
                                                                 PRBool         swapBytes);
static SECItem *xmlSecNssAppNicknameCollisionCallback           (SECItem *old_nick,
                                                                 PRBool *cancel,
                                                                 void *wincx);
static xmlSecKeyPtr     xmlSecNssAppDerKeyLoadSECItem           (SECItem* secItem);

/**
 * xmlSecNssAppInit:
 * @config:             the path to NSS database files.
 *
 * General crypto engine initialization. This function is used
 * by XMLSec command line utility and called before
 * @xmlSecInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppInit(const char* config) {
    SECStatus rv;

    if(config) {
        rv = NSS_InitReadWrite(config);
        if(rv != SECSuccess) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "NSS_InitReadWrite",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "config=%s",
                        xmlSecErrorsSafeString(config));
            return(-1);
        }
    } else {
        rv = NSS_NoDB_Init(NULL);
        if(rv != SECSuccess) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "NSS_NoDB_Init",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    /* configure PKCS11 */
    PK11_ConfigurePKCS11("manufacturesID", "libraryDescription",
                         "tokenDescription", "privateTokenDescription",
                         "slotDescription", "privateSlotDescription",
                         "fipsSlotDescription", "fipsPrivateSlotDescription",
                         0, 0);

    /* setup for PKCS12 */
    PORT_SetUCS2_ASCIIConversionFunction(xmlSecNssAppAscii2UCS2Conv);
    SEC_PKCS12EnableCipher(PKCS12_RC4_40, 1);
    SEC_PKCS12EnableCipher(PKCS12_RC4_128, 1);
    SEC_PKCS12EnableCipher(PKCS12_RC2_CBC_40, 1);
    SEC_PKCS12EnableCipher(PKCS12_RC2_CBC_128, 1);
    SEC_PKCS12EnableCipher(PKCS12_DES_56, 1);
    SEC_PKCS12EnableCipher(PKCS12_DES_EDE3_168, 1);
    SEC_PKCS12SetPreferredCipher(PKCS12_DES_EDE3_168, 1);

    return(0);
}

/**
 * xmlSecNssAppShutdown:
 *
 * General crypto engine shutdown. This function is used
 * by XMLSec command line utility and called after
 * @xmlSecShutdown function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppShutdown(void) {
    SECStatus rv;
/*
    SSL_ClearSessionCache();
*/
    PK11_LogoutAll();
    rv = NSS_Shutdown();
    if(rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "NSS_Shutdown",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(0);
}


static int
xmlSecNssAppCreateSECItem(SECItem *contents, const xmlSecByte* data, xmlSecSize dataSize) {
    xmlSecAssert2(contents != NULL, -1);
    xmlSecAssert2(data != NULL, -1);

    contents->data = 0;
    if (!SECITEM_AllocItem(NULL, contents, dataSize)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SECITEM_AllocItem",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if(dataSize > 0) {
        xmlSecAssert2(contents->data != NULL, -1);
        memcpy(contents->data,  data, dataSize);
    }

    return (0);
}

static int
xmlSecNssAppReadSECItem(SECItem *contents, const char *fn) {
    PRFileInfo info;
    PRFileDesc *file = NULL;
    PRInt32 numBytes;
    PRStatus prStatus;
    int ret = -1;

    xmlSecAssert2(contents != NULL, -1);
    xmlSecAssert2(fn != NULL, -1);

    file = PR_Open(fn, PR_RDONLY, 00660);
    if (file == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PR_Open",
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(fn));
        goto done;
    }

    prStatus = PR_GetOpenFileInfo(file, &info);
    if (prStatus != PR_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PR_GetOpenFileInfo",
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(fn));
        goto done;
    }

    contents->data = 0;
    if (!SECITEM_AllocItem(NULL, contents, info.size)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SECITEM_AllocItem",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    numBytes = PR_Read(file, contents->data, info.size);
    if (numBytes != info.size) {
        SECITEM_FreeItem(contents, PR_FALSE);
        goto done;
    }

    ret = 0;
done:
    if (file) {
        PR_Close(file);
    }

    return (ret);
}

static PRBool
xmlSecNssAppAscii2UCS2Conv(PRBool toUnicode,
                           unsigned char *inBuf,
                           unsigned int   inBufLen,
                           unsigned char *outBuf,
                           unsigned int   maxOutBufLen,
                           unsigned int  *outBufLen,
                           PRBool         swapBytes ATTRIBUTE_UNUSED)
{
    SECItem it;

    if (toUnicode == PR_FALSE) {
        return (PR_FALSE);
    }

    memset(&it, 0, sizeof(it));
    it.data = inBuf;
    it.len = inBufLen;

    return(PORT_UCS2_UTF8Conversion(toUnicode, it.data, it.len,
                                    outBuf, maxOutBufLen, outBufLen));
}

static SECItem *
xmlSecNssAppNicknameCollisionCallback(SECItem *old_nick ATTRIBUTE_UNUSED,
                                      PRBool *cancel,
                                      void *wincx ATTRIBUTE_UNUSED)
{
    if (cancel == NULL) {
        return (NULL);
    }

    /* XXX not handled yet  */
    *cancel = PR_TRUE;
    return (NULL);
}

/**
 * xmlSecNssAppKeyLoad:
 * @filename:           the key filename.
 * @format:             the key file format.
 * @pwd:                the key file password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from a file
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppKeyLoad(const char *filename, xmlSecKeyDataFormat format,
                    const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    SECItem secItem;
    xmlSecKeyPtr res;
    int ret;

    xmlSecAssert2(filename != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    /* read the file contents */
    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppReadSECItem(&secItem, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppReadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecNssAppKeyLoadSECItem(&secItem, format, pwd, pwdCallback, pwdCallbackCtx);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeyLoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(NULL);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(res);
}

/**
 * xmlSecNssAppKeyLoadMemory:
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @format:             the key data format.
 * @pwd:                the key data2 password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from a binary @data.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppKeyLoadMemory(const xmlSecByte* data, xmlSecSize dataSize, xmlSecKeyDataFormat format,
                    const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    SECItem secItem;
    xmlSecKeyPtr res;
    int ret;

    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppCreateSECItem(&secItem, data, dataSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppCreateSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecNssAppKeyLoadSECItem(&secItem, format, pwd, pwdCallback, pwdCallbackCtx);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeyLoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(NULL);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(res);
}

/**
 * xmlSecNssAppKeyLoadSECItem:
 * @secItem:            the pointer to sec item.
 * @format:             the key format.
 * @pwd:                the key password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from a file
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppKeyLoadSECItem(SECItem* secItem, xmlSecKeyDataFormat format,
                    const char *pwd,
                    void* pwdCallback,
                    void* pwdCallbackCtx) {
    xmlSecKeyPtr key = NULL;

    xmlSecAssert2(secItem != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    switch(format) {
#ifndef XMLSEC_NO_X509
    case xmlSecKeyDataFormatPkcs12:
        key = xmlSecNssAppPkcs12LoadSECItem(secItem, pwd, pwdCallback, pwdCallbackCtx);
        if(key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssAppPkcs12LoadSECItem",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }
        break;
    case xmlSecKeyDataFormatCertDer:
        key = xmlSecNssAppKeyFromCertLoadSECItem(secItem, format);
        if(key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssAppKeyFromCertLoadSECItem",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }
        break;
#endif /* XMLSEC_NO_X509 */
    case xmlSecKeyDataFormatDer:
        key = xmlSecNssAppDerKeyLoadSECItem(secItem);
        if(key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssAppDerKeyLoadSECItem",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }
        break;
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeyLoad",
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", format);
            return(NULL);
    }

    return(key);
}

static xmlSecKeyPtr
xmlSecNssAppDerKeyLoadSECItem(SECItem* secItem) {
    xmlSecKeyPtr key = NULL;
    xmlSecKeyPtr retval = NULL;
    xmlSecKeyDataPtr data = NULL;
    int ret;
    SECKEYPublicKey *pubkey = NULL;
    SECKEYPrivateKey *privkey = NULL;
    CERTSubjectPublicKeyInfo *spki = NULL;
    SECItem nickname;
    PK11SlotInfo *slot = NULL;
    SECStatus status;

    xmlSecAssert2(secItem != NULL, NULL);

    /* we're importing a key about which we know nothing yet, just use the
     * internal slot
     */
    slot = xmlSecNssGetInternalKeySlot();
    if (slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssGetInternalKeySlot",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    nickname.len = 0;
    nickname.data = NULL;


    /* TRY PRIVATE KEY FIRST
     * Note: This expects the key to be in PrivateKeyInfo format. The
     * DER files created from PEM via openssl utilities aren't in that
     * format
     */
    status = PK11_ImportDERPrivateKeyInfoAndReturnKey(slot, secItem,
                            &nickname, NULL, PR_FALSE,
                            PR_TRUE, KU_ALL, &privkey, NULL);
    if (status != SECSuccess) {
        /* TRY PUBLIC KEY */
        spki = SECKEY_DecodeDERSubjectPublicKeyInfo(secItem);
        if (spki == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "SECKEY_DecodeDERSubjectPublicKeyInfo",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
        }

        pubkey = SECKEY_ExtractPublicKey(spki);
        if (pubkey == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "SECKEY_ExtractPublicKey",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }
    }

    data = xmlSecNssPKIAdoptKey(privkey, pubkey);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssPKIAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    privkey = NULL;
    pubkey = NULL;

    key = xmlSecKeyCreate();
    if(key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)));
        goto done;
    }
    retval = key;
    key = NULL;
    data = NULL;


done:
    if(slot != NULL) {
        PK11_FreeSlot(slot);
    }
    if(privkey != NULL) {
        SECKEY_DestroyPrivateKey(privkey);
    }
    if(pubkey != NULL) {
        SECKEY_DestroyPublicKey(pubkey);
    }
    if(key != NULL) {
        xmlSecKeyDestroy(key);
    }
    if(data != NULL) {
        xmlSecKeyDataDestroy(data);
    }
    if(spki != NULL) {
        SECKEY_DestroySubjectPublicKeyInfo(spki);
    }
    return (retval);
}

#ifndef XMLSEC_NO_X509
/**
 * xmlSecNssAppKeyCertLoad:
 * @key:                the pointer to key.
 * @filename:           the certificate filename.
 * @format:             the certificate file format.
 *
 * Reads the certificate from $@filename and adds it to key
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppKeyCertLoad(xmlSecKeyPtr key, const char* filename, xmlSecKeyDataFormat format) {
    SECItem secItem;
    int ret;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* read the file contents */
    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppReadSECItem(&secItem, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppReadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecNssAppKeyCertLoadSECItem(key, &secItem, format);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeyCertLoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(-1);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(0);
}

/**
 * xmlSecNssAppKeyCertLoadMemory:
 * @key:                the pointer to key.
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @format:             the certificate format.
 *
 * Reads the certificate from @data and adds it to key
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppKeyCertLoadMemory(xmlSecKeyPtr key, const xmlSecByte* data, xmlSecSize dataSize, xmlSecKeyDataFormat format) {
    SECItem secItem;
    int ret;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* read the file contents */
    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppCreateSECItem(&secItem, data, dataSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppCreateSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecNssAppKeyCertLoadSECItem(key, &secItem, format);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeyCertLoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(-1);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(0);
}

/**
 * xmlSecNssAppKeyCertLoadSECItem:
 * @key:                the pointer to key.
 * @secItem:            the pointer to SECItem.
 * @format:             the certificate format.
 *
 * Reads the certificate from @secItem and adds it to key
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppKeyCertLoadSECItem(xmlSecKeyPtr key, SECItem* secItem, xmlSecKeyDataFormat format) {
    CERTCertificate *cert=NULL;
    xmlSecKeyDataPtr data;
    int ret;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(secItem != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    data = xmlSecKeyEnsureData(key, xmlSecNssKeyDataX509Id);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyEnsureData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "transform=%s",
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecNssKeyDataX509Id)));
        return(-1);
    }

    switch(format) {
    case xmlSecKeyDataFormatPkcs8Der:
    case xmlSecKeyDataFormatDer:
        cert = __CERT_NewTempCertificate(CERT_GetDefaultCertDB(),
                                         secItem, NULL, PR_FALSE, PR_TRUE);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "__CERT_NewTempCertificate",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "format=%d", format);
            return(-1);
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

    xmlSecAssert2(cert != NULL, -1);
    ret = xmlSecNssKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)));
        CERT_DestroyCertificate(cert);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecNssAppPkcs12Load:
 * @filename:           the PKCS12 key filename.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 file.
 * For uniformity, call xmlSecNssAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppPkcs12Load(const char *filename, const char *pwd,
                       void *pwdCallback ATTRIBUTE_UNUSED,
                       void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    SECItem secItem;
    xmlSecKeyPtr res;
    int ret;

    xmlSecAssert2(filename != NULL, NULL);

    /* read the file contents */
    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppReadSECItem(&secItem, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppReadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecNssAppPkcs12LoadSECItem(&secItem, pwd, pwdCallback, pwdCallbackCtx);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppPkcs12LoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(NULL);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(res);
}

/**
 * xmlSecNssAppPkcs12LoadMemory:
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @pwd:                the PKCS12 password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 binary data.
 * For uniformity, call xmlSecNssAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppPkcs12LoadMemory(const xmlSecByte* data, xmlSecSize dataSize, const char *pwd,
                       void *pwdCallback ATTRIBUTE_UNUSED,
                       void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    SECItem secItem;
    xmlSecKeyPtr res;
    int ret;

    xmlSecAssert2(data != NULL, NULL);

    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppCreateSECItem(&secItem, data, dataSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppCreateSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecNssAppPkcs12LoadSECItem(&secItem, pwd, pwdCallback, pwdCallbackCtx);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppPkcs12LoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(NULL);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(res);
}


/**
 * xmlSecNssAppPkcs12LoadSECItem:
 * @secItem:            the @SECItem object.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 SECItem.
 * For uniformity, call xmlSecNssAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppPkcs12LoadSECItem(SECItem* secItem, const char *pwd,
                       void *pwdCallback ATTRIBUTE_UNUSED,
                       void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    xmlSecKeyPtr key = NULL;
    xmlSecKeyDataPtr data = NULL;
    xmlSecKeyDataPtr x509Data = NULL;
    int ret;
    PK11SlotInfo *slot = NULL;
    SECItem pwditem;
    SECItem uc2_pwditem;
    SECStatus rv;
    SECKEYPrivateKey *privkey = NULL;
    SECKEYPublicKey *pubkey = NULL;
    CERTCertList *certlist = NULL;
    CERTCertListNode    *head = NULL;
    CERTCertificate     *cert = NULL;
    CERTCertificate     *tmpcert = NULL;
    SEC_PKCS12DecoderContext *p12ctx = NULL;


    xmlSecAssert2((secItem != NULL), NULL);

    if (pwd == NULL) {
        pwd = "";
    }
    memset(&uc2_pwditem, 0, sizeof(uc2_pwditem));

    /* we're importing a key about which we know nothing yet, just use the
     * internal slot. We have no criteria to choose a slot.
     */
    slot = xmlSecNssGetInternalKeySlot();
    if (slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssGetInternalKeySlot",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    pwditem.data = (unsigned char *)pwd;
    pwditem.len = strlen(pwd)+1;
    if (!SECITEM_AllocItem(NULL, &uc2_pwditem, 2*pwditem.len)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SECITEM_AllocItem",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    if (PORT_UCS2_ASCIIConversion(PR_TRUE, pwditem.data, pwditem.len,
                              uc2_pwditem.data, 2*pwditem.len,
                              &(uc2_pwditem.len), 0) == PR_FALSE) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PORT_UCS2_ASCIIConversion",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    p12ctx = SEC_PKCS12DecoderStart(&uc2_pwditem, slot, NULL,
                                    NULL, NULL, NULL, NULL, NULL);
    if (p12ctx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SEC_PKCS12DecoderStart",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    rv = SEC_PKCS12DecoderUpdate(p12ctx, secItem->data, secItem->len);
    if (rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SEC_PKCS12DecoderUpdate",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    rv = SEC_PKCS12DecoderVerify(p12ctx);
    if (rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SEC_PKCS12DecoderVerify",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    rv = SEC_PKCS12DecoderValidateBags(p12ctx, xmlSecNssAppNicknameCollisionCallback);
    if (rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SEC_PKCS12DecoderValidateBags",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    rv = SEC_PKCS12DecoderImportBags(p12ctx);
    if (rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SEC_PKCS12DecoderImportBags",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    certlist = SEC_PKCS12DecoderGetCerts(p12ctx);
    if (certlist == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "SEC_PKCS12DecoderGetCerts",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    x509Data = xmlSecKeyDataCreate(xmlSecNssKeyDataX509Id);
    if(x509Data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "transform=%s",
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(xmlSecNssKeyDataX509Id)));
        goto done;
    }

    for (head = CERT_LIST_HEAD(certlist);
         !CERT_LIST_END(head, certlist);
         head = CERT_LIST_NEXT(head)) {
        cert = head->cert;
        privkey = PK11_FindKeyByAnyCert(cert, NULL);

        if (privkey != NULL) {
            if (data != NULL) {
                /* we already found a private key.
                 * assume the first private key we find is THE ONE
                 */
                SECKEY_DestroyPrivateKey(privkey);
                privkey = NULL;
            } else {
                pubkey = CERT_ExtractPublicKey(cert);
                if (pubkey == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "CERT_ExtractPublicKey",
                                XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }
                data = xmlSecNssPKIAdoptKey(privkey, pubkey);
                if(data == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecNssPKIAdoptKey",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }

                pubkey = NULL;
                privkey = NULL;

                tmpcert = CERT_DupCertificate(cert);
                if(tmpcert == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "CERT_DupCertificate",
                                XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                "data=%s",
                                xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
                    goto done;
                }

                ret = xmlSecNssKeyDataX509AdoptKeyCert(x509Data, tmpcert);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecNssKeyDataX509AdoptKeyCert",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "data=%s",
                                xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
                    CERT_DestroyCertificate(tmpcert);
                    goto done;
                }

            }
        }

        tmpcert = CERT_DupCertificate(cert);
        if(tmpcert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CERT_DupCertificate",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }
        ret = xmlSecNssKeyDataX509AdoptCert(x509Data, tmpcert);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssKeyDataX509AdoptCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            CERT_DestroyCertificate(tmpcert);
            goto done;
        }

    } /* end for loop */

    if (data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppPkcs12Load",
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

    ret = xmlSecKeySetValue(key, data);
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
    data = NULL;

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
    if (p12ctx) {
        SEC_PKCS12DecoderFinish(p12ctx);
    }
    SECITEM_FreeItem(&uc2_pwditem, PR_FALSE);
    if (slot) {
        PK11_FreeSlot(slot);
    }
    if (certlist) {
        CERT_DestroyCertList(certlist);
    }
    if(x509Data != NULL) {
        xmlSecKeyDataDestroy(x509Data);
    }
    if(data != NULL) {
        xmlSecKeyDataDestroy(data);
    }
    if (privkey) {
        SECKEY_DestroyPrivateKey(privkey);
    }
    if (pubkey) {
        SECKEY_DestroyPublicKey(pubkey);
    }

    return(key);
}

/**
 * xmlSecNssAppKeyFromCertLoadSECItem:
 * @secItem:            the @SECItem object.
 * @format:             the cert format.
 *
 * Loads public key from cert.
 *
 * Returns: pointer to key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecNssAppKeyFromCertLoadSECItem(SECItem* secItem, xmlSecKeyDataFormat format) {
    xmlSecKeyPtr key;
    xmlSecKeyDataPtr keyData;
    xmlSecKeyDataPtr certData;
    CERTCertificate *cert=NULL;
    int ret;

    xmlSecAssert2(secItem != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    /* load cert */
    switch(format) {
    case xmlSecKeyDataFormatCertDer:
        cert = __CERT_NewTempCertificate(CERT_GetDefaultCertDB(),
                                         secItem, NULL, PR_FALSE, PR_TRUE);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "__CERT_NewTempCertificate",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "format=%d", format);
            return(NULL);
        }
        break;
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", format);
        return(NULL);
    }

    /* get key value */
    keyData = xmlSecNssX509CertGetKey(cert);
    if(keyData == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509CertGetKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        return(NULL);
    }

    /* create key */
    key = xmlSecKeyCreate();
    if(key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(keyData);
        CERT_DestroyCertificate(cert);
        return(NULL);
    }

    /* set key value */
    ret = xmlSecKeySetValue(key, keyData);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDestroy(key);
        xmlSecKeyDataDestroy(keyData);
        CERT_DestroyCertificate(cert);
        return(NULL);
    }

    /* create cert data */
    certData = xmlSecKeyEnsureData(key, xmlSecNssKeyDataX509Id);
    if(certData == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyEnsureData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDestroy(key);
        CERT_DestroyCertificate(cert);
        return(NULL);
    }

    /* put cert in the cert data */
    ret = xmlSecNssKeyDataX509AdoptCert(certData, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDestroy(key);
        CERT_DestroyCertificate(cert);
        return(NULL);
    }

    return(key);
}


/**
 * xmlSecNssAppKeysMngrCertLoad:
 * @mngr:               the pointer to keys manager.
 * @filename:           the certificate file.
 * @format:             the certificate file format (PEM or DER).
 * @type:               the certificate type (trusted/untrusted).
 *
 * Reads cert from @filename and adds to the list of trusted or known
 * untrusted certs in @store
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppKeysMngrCertLoad(xmlSecKeysMngrPtr mngr, const char *filename,
                             xmlSecKeyDataFormat format,
                             xmlSecKeyDataType type) {
    SECItem secItem;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* read the file contents */
    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppReadSECItem(&secItem, filename);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppReadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecNssAppKeysMngrCertLoadSECItem(mngr, &secItem, format, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeysMngrCertLoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(-1);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(0);
}

/**
 * xmlSecNssAppKeysMngrCertLoadMemory:
 * @mngr:               the pointer to keys manager.
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @format:             the certificate format (PEM or DER).
 * @type:               the certificate type (trusted/untrusted).
 *
 * Reads cert from @data and adds to the list of trusted or known
 * untrusted certs in @store
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppKeysMngrCertLoadMemory(xmlSecKeysMngrPtr mngr, const xmlSecByte* data,
                             xmlSecSize dataSize, xmlSecKeyDataFormat format,
                             xmlSecKeyDataType type) {
    SECItem secItem;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    memset(&secItem, 0, sizeof(secItem));
    ret = xmlSecNssAppCreateSECItem(&secItem, data, dataSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppCreateSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecNssAppKeysMngrCertLoadSECItem(mngr, &secItem, format, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssAppKeysMngrCertLoadSECItem",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&secItem, PR_FALSE);
        return(-1);
    }

    SECITEM_FreeItem(&secItem, PR_FALSE);
    return(0);
}

/**
 * xmlSecNssAppKeysMngrCertLoadSECItem:
 * @mngr:               the pointer to keys manager.
 * @secItem:            the pointer to SECItem.
 * @format:             the certificate format (PEM or DER).
 * @type:               the certificate type (trusted/untrusted).
 *
 * Reads cert from @secItem and adds to the list of trusted or known
 * untrusted certs in @store
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppKeysMngrCertLoadSECItem(xmlSecKeysMngrPtr mngr, SECItem* secItem,
                             xmlSecKeyDataFormat format,
                             xmlSecKeyDataType type) {
    xmlSecKeyDataStorePtr x509Store;
    CERTCertificate* cert;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(secItem != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    x509Store = xmlSecKeysMngrGetDataStore(mngr, xmlSecNssX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecNssX509StoreId");
        return(-1);
    }

    switch(format) {
    case xmlSecKeyDataFormatDer:
        cert = __CERT_NewTempCertificate(CERT_GetDefaultCertDB(),
                                         secItem, NULL, PR_FALSE, PR_TRUE);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "__CERT_NewTempCertificate",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "format=%d", format);
            return(-1);
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

    ret = xmlSecNssX509StoreAdoptCert(x509Store, cert, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509StoreAdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        return(-1);
    }

    return(0);
}

#endif /* XMLSEC_NO_X509 */

/**
 * xmlSecNssAppDefaultKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Initializes @mngr with NSS keys store #xmlSecNssKeysStoreId
 * and a default NSS crypto key data stores.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppDefaultKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

    /* create NSS keys store if needed */
    if(xmlSecKeysMngrGetKeysStore(mngr) == NULL) {
        xmlSecKeyStorePtr keysStore;

        keysStore = xmlSecKeyStoreCreate(xmlSecNssKeysStoreId);
        if(keysStore == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyStoreCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecNssKeysStoreId");
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

    ret = xmlSecNssKeysMngrInit(mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKeysMngrInit",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    mngr->getKey = xmlSecKeysMngrGetKey;
    return(0);
}

/**
 * xmlSecNssAppDefaultKeysMngrAdoptKey:
 * @mngr:               the pointer to keys manager.
 * @key:                the pointer to key.
 *
 * Adds @key to the keys manager @mngr created with #xmlSecNssAppDefaultKeysMngrInit
 * function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppDefaultKeysMngrAdoptKey(xmlSecKeysMngrPtr mngr, xmlSecKeyPtr key) {
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

    ret = xmlSecNssKeysStoreAdoptKey(store, key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKeysStoreAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecNssAppDefaultKeysMngrLoad:
 * @mngr:               the pointer to keys manager.
 * @uri:                the uri.
 *
 * Loads XML keys file from @uri to the keys manager @mngr created
 * with #xmlSecNssAppDefaultKeysMngrInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppDefaultKeysMngrLoad(xmlSecKeysMngrPtr mngr, const char* uri) {
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

    ret = xmlSecNssKeysStoreLoad(store, uri, mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKeysStoreLoad",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "uri=%s", xmlSecErrorsSafeString(uri));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecNssAppDefaultKeysMngrSave:
 * @mngr:               the pointer to keys manager.
 * @filename:           the destination filename.
 * @type:               the type of keys to save (public/private/symmetric).
 *
 * Saves keys from @mngr to  XML keys file.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssAppDefaultKeysMngrSave(xmlSecKeysMngrPtr mngr, const char* filename, xmlSecKeyDataType type) {
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

    ret = xmlSecNssKeysStoreSave(store, filename, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKeysStoreSave",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename%s", xmlSecErrorsSafeString(filename));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecNssAppGetDefaultPwdCallback:
 *
 * Gets default password callback.
 *
 * Returns: default password callback.
 */
void*
xmlSecNssAppGetDefaultPwdCallback(void) {
    return(NULL);
}


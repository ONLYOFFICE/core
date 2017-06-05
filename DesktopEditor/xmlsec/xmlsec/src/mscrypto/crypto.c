/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 * Copyright (C) 2003-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 * Copyright (c) 2005-2006 Cryptocom LTD (http://www.cryptocom.ru).
 */
#include "globals.h"

#include <string.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>
#include <xmlsec/dl.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/private.h>

#include <xmlsec/mscrypto/app.h>
#include <xmlsec/mscrypto/crypto.h>
#include <xmlsec/mscrypto/x509.h>
#include "private.h"

#if defined(__MINGW32__)
/* NOTE mingw.org project don't define any xxx_s function and may
 * be never will define them.
 *
 * In this file is save to use non _s function as into destination
 * buffer program code copy empty string and the size of source buffer
 * (XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE=4096) is enough for any
 * encoding. Also program code don't check result of _s functions.
 */

static int
strcpy_s(char *dest, size_t n, const char *src) {
    strcpy(dest, src);
    return(0);
}

static int
wcscpy_s(wchar_t *dest, size_t n, const wchar_t *src) {
    wcscpy(dest, src);
    return(0);
}
#endif

#define XMLSEC_CONTAINER_NAME_A "xmlsec-key-container"
#define XMLSEC_CONTAINER_NAME_W L"xmlsec-key-container"
#ifdef UNICODE
#define XMLSEC_CONTAINER_NAME XMLSEC_CONTAINER_NAME_W
#else
#define XMLSEC_CONTAINER_NAME XMLSEC_CONTAINER_NAME_A
#endif


static xmlSecCryptoDLFunctionsPtr gXmlSecMSCryptoFunctions = NULL;

/**
 * xmlSecCryptoGetFunctions_mscrypto:
 *
 * Gets MSCrypto specific functions table.
 *
 * Returns: xmlsec-mscrypto functions table.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoGetFunctions_mscrypto(void) {
    static xmlSecCryptoDLFunctions functions;

    if(gXmlSecMSCryptoFunctions != NULL) {
        return(gXmlSecMSCryptoFunctions);
    }

    memset(&functions, 0, sizeof(functions));
    gXmlSecMSCryptoFunctions = &functions;

    /********************************************************************
     *
     * Crypto Init/shutdown
     *
     ********************************************************************/
    gXmlSecMSCryptoFunctions->cryptoInit                        = xmlSecMSCryptoInit;
    gXmlSecMSCryptoFunctions->cryptoShutdown                    = xmlSecMSCryptoShutdown;
    gXmlSecMSCryptoFunctions->cryptoKeysMngrInit                = xmlSecMSCryptoKeysMngrInit;

    /********************************************************************
     *
     * Key data ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_DES
    gXmlSecMSCryptoFunctions->keyDataDesGetKlass                = xmlSecMSCryptoKeyDataDesGetKlass;
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    gXmlSecMSCryptoFunctions->keyDataAesGetKlass                = xmlSecMSCryptoKeyDataAesGetKlass;
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_RSA
    gXmlSecMSCryptoFunctions->keyDataRsaGetKlass                = xmlSecMSCryptoKeyDataRsaGetKlass;
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_HMAC
    gXmlSecMSCryptoFunctions->keyDataHmacGetKlass               = xmlSecMSCryptoKeyDataHmacGetKlass;
#endif /* XMLSEC_NO_HMAC */

#ifndef XMLSEC_NO_DSA
    gXmlSecMSCryptoFunctions->keyDataDsaGetKlass                = xmlSecMSCryptoKeyDataDsaGetKlass;
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_GOST
    gXmlSecMSCryptoFunctions->keyDataGost2001GetKlass           = xmlSecMSCryptoKeyDataGost2001GetKlass;
#endif /* XMLSEC_NO_GOST*/

#ifndef XMLSEC_NO_X509
    gXmlSecMSCryptoFunctions->keyDataX509GetKlass               = xmlSecMSCryptoKeyDataX509GetKlass;
    gXmlSecMSCryptoFunctions->keyDataRawX509CertGetKlass        = xmlSecMSCryptoKeyDataRawX509CertGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Key data store ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_X509
    gXmlSecMSCryptoFunctions->x509StoreGetKlass                 = xmlSecMSCryptoX509StoreGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Crypto transforms ids
     *
     ********************************************************************/

    /******************************* AES ********************************/
#ifndef XMLSEC_NO_AES
    gXmlSecMSCryptoFunctions->transformAes128CbcGetKlass        = xmlSecMSCryptoTransformAes128CbcGetKlass;
    gXmlSecMSCryptoFunctions->transformAes192CbcGetKlass        = xmlSecMSCryptoTransformAes192CbcGetKlass;
    gXmlSecMSCryptoFunctions->transformAes256CbcGetKlass        = xmlSecMSCryptoTransformAes256CbcGetKlass;
    gXmlSecMSCryptoFunctions->transformKWAes128GetKlass         = xmlSecMSCryptoTransformKWAes128GetKlass;
    gXmlSecMSCryptoFunctions->transformKWAes192GetKlass         = xmlSecMSCryptoTransformKWAes192GetKlass;
    gXmlSecMSCryptoFunctions->transformKWAes256GetKlass         = xmlSecMSCryptoTransformKWAes256GetKlass;
#endif /* XMLSEC_NO_AES */

    /******************************* DES ********************************/
#ifndef XMLSEC_NO_DES
    gXmlSecMSCryptoFunctions->transformDes3CbcGetKlass          = xmlSecMSCryptoTransformDes3CbcGetKlass;
    gXmlSecMSCryptoFunctions->transformKWDes3GetKlass           = xmlSecMSCryptoTransformKWDes3GetKlass;
#endif /* XMLSEC_NO_DES */

    /******************************* DSA ********************************/
#ifndef XMLSEC_NO_DSA
    gXmlSecMSCryptoFunctions->transformDsaSha1GetKlass          = xmlSecMSCryptoTransformDsaSha1GetKlass;
#endif /* XMLSEC_NO_DSA */

    /******************************* GOST ********************************/
#ifndef XMLSEC_NO_GOST
    gXmlSecMSCryptoFunctions->transformGost2001GostR3411_94GetKlass             = xmlSecMSCryptoTransformGost2001GostR3411_94GetKlass;
#endif /* XMLSEC_NO_GOST */

#ifndef XMLSEC_NO_GOST
    gXmlSecMSCryptoFunctions->transformGostR3411_94GetKlass             = xmlSecMSCryptoTransformGostR3411_94GetKlass;
#endif /* XMLSEC_NO_GOST */

    /******************************* HMAC ********************************/
#ifndef XMLSEC_NO_HMAC

#ifndef XMLSEC_NO_MD5
    gXmlSecMSCryptoFunctions->transformHmacMd5GetKlass         = xmlSecMSCryptoTransformHmacMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecMSCryptoFunctions->transformHmacSha1GetKlass        = xmlSecMSCryptoTransformHmacSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecMSCryptoFunctions->transformHmacSha256GetKlass      = xmlSecMSCryptoTransformHmacSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecMSCryptoFunctions->transformHmacSha384GetKlass      = xmlSecMSCryptoTransformHmacSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecMSCryptoFunctions->transformHmacSha512GetKlass      = xmlSecMSCryptoTransformHmacSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

    /******************************* MD5 ********************************/
#ifndef XMLSEC_NO_MD5
    gXmlSecMSCryptoFunctions->transformMd5GetKlass             = xmlSecMSCryptoTransformMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

    /******************************* RSA ********************************/
#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    gXmlSecMSCryptoFunctions->transformRsaMd5GetKlass          = xmlSecMSCryptoTransformRsaMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecMSCryptoFunctions->transformRsaSha1GetKlass          = xmlSecMSCryptoTransformRsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecMSCryptoFunctions->transformRsaSha256GetKlass       = xmlSecMSCryptoTransformRsaSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecMSCryptoFunctions->transformRsaSha384GetKlass       = xmlSecMSCryptoTransformRsaSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecMSCryptoFunctions->transformRsaSha512GetKlass       = xmlSecMSCryptoTransformRsaSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

    gXmlSecMSCryptoFunctions->transformRsaPkcs1GetKlass         = xmlSecMSCryptoTransformRsaPkcs1GetKlass;
    gXmlSecMSCryptoFunctions->transformRsaOaepGetKlass          = xmlSecMSCryptoTransformRsaOaepGetKlass;
#endif /* XMLSEC_NO_RSA */

    /******************************* SHA ********************************/
#ifndef XMLSEC_NO_SHA1
    gXmlSecMSCryptoFunctions->transformSha1GetKlass             = xmlSecMSCryptoTransformSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */
#ifndef XMLSEC_NO_SHA256
    gXmlSecMSCryptoFunctions->transformSha256GetKlass          = xmlSecMSCryptoTransformSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */
#ifndef XMLSEC_NO_SHA384
    gXmlSecMSCryptoFunctions->transformSha384GetKlass          = xmlSecMSCryptoTransformSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */
#ifndef XMLSEC_NO_SHA512
    gXmlSecMSCryptoFunctions->transformSha512GetKlass          = xmlSecMSCryptoTransformSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

    /********************************************************************
     *
     * High level routines form xmlsec command line utility
     *
     ********************************************************************/
    gXmlSecMSCryptoFunctions->cryptoAppInit                     = xmlSecMSCryptoAppInit;
    gXmlSecMSCryptoFunctions->cryptoAppShutdown                 = xmlSecMSCryptoAppShutdown;
    gXmlSecMSCryptoFunctions->cryptoAppDefaultKeysMngrInit      = xmlSecMSCryptoAppDefaultKeysMngrInit;
    gXmlSecMSCryptoFunctions->cryptoAppDefaultKeysMngrAdoptKey  = xmlSecMSCryptoAppDefaultKeysMngrAdoptKey;
    gXmlSecMSCryptoFunctions->cryptoAppDefaultKeysMngrLoad      = xmlSecMSCryptoAppDefaultKeysMngrLoad;
    gXmlSecMSCryptoFunctions->cryptoAppDefaultKeysMngrSave      = xmlSecMSCryptoAppDefaultKeysMngrSave;
#ifndef XMLSEC_NO_X509
    gXmlSecMSCryptoFunctions->cryptoAppKeysMngrCertLoad         = xmlSecMSCryptoAppKeysMngrCertLoad;
    gXmlSecMSCryptoFunctions->cryptoAppKeysMngrCertLoadMemory   = xmlSecMSCryptoAppKeysMngrCertLoadMemory;
    gXmlSecMSCryptoFunctions->cryptoAppPkcs12Load               = xmlSecMSCryptoAppPkcs12Load;
    gXmlSecMSCryptoFunctions->cryptoAppPkcs12LoadMemory         = xmlSecMSCryptoAppPkcs12LoadMemory;
    gXmlSecMSCryptoFunctions->cryptoAppKeyCertLoad              = xmlSecMSCryptoAppKeyCertLoad;
    gXmlSecMSCryptoFunctions->cryptoAppKeyCertLoadMemory        = xmlSecMSCryptoAppKeyCertLoadMemory;
#endif /* XMLSEC_NO_X509 */
    gXmlSecMSCryptoFunctions->cryptoAppKeyLoad                  = xmlSecMSCryptoAppKeyLoad;
    gXmlSecMSCryptoFunctions->cryptoAppKeyLoadMemory            = xmlSecMSCryptoAppKeyLoadMemory;
    gXmlSecMSCryptoFunctions->cryptoAppDefaultPwdCallback       = (void*)xmlSecMSCryptoAppGetDefaultPwdCallback();

    return(gXmlSecMSCryptoFunctions);
}

/**
 * xmlSecMSCryptoInit:
 *
 * XMLSec library specific crypto engine initialization.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoInit (void)  {
    /* Check loaded xmlsec library version */
    if(xmlSecCheckVersionExact() != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCheckVersionExact",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* set default errors callback for xmlsec to us */
    xmlSecErrorsSetCallback(xmlSecMSCryptoErrorsDefaultCallback);

    /* register our klasses */
    if(xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms(xmlSecCryptoGetFunctions_mscrypto()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(0);
}

/**
 * xmlSecMSCryptoShutdown:
 *
 * XMLSec library specific crypto engine shutdown.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoShutdown(void) {
    /* TODO: if necessary, do additional shutdown here */
    return(0);
}

/**
 * xmlSecMSCryptoKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Adds MSCrypto specific key data stores in keys manager.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

#ifndef XMLSEC_NO_X509
    /* create x509 store if needed */
    if(xmlSecKeysMngrGetDataStore(mngr, xmlSecMSCryptoX509StoreId) == NULL) {
        xmlSecKeyDataStorePtr x509Store;

        x509Store = xmlSecKeyDataStoreCreate(xmlSecMSCryptoX509StoreId);
        if(x509Store == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataStoreCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecMSCryptoX509StoreId");
            return(-1);
        }

        ret = xmlSecKeysMngrAdoptDataStore(mngr, x509Store);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeysMngrAdoptDataStore",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyDataStoreDestroy(x509Store);
            return(-1);
        }
    }
#endif /* XMLSEC_NO_X509 */

    return(0);
}


static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Random[] = {
    { MS_STRONG_PROV,               PROV_RSA_FULL },
    { MS_ENHANCED_PROV,             PROV_RSA_FULL },
    { NULL, 0 }
};

/**
 * xmlSecMSCryptoGenerateRandom:
 * @buffer:             the destination buffer.
 * @size:               the numer of bytes to generate.
 *
 * Generates @size random bytes and puts result in @buffer
 * (not implemented yet).
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecMSCryptoGenerateRandom(xmlSecBufferPtr buffer, size_t size) {
    HCRYPTPROV hProv = 0;
    int ret;

    xmlSecAssert2(buffer != NULL, -1);
    xmlSecAssert2(size > 0, -1);

    ret = xmlSecBufferSetSize(buffer, size);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferSetSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", size);
        return(-1);
    }

    hProv = xmlSecMSCryptoFindProvider(xmlSecMSCryptoProviderInfo_Random, NULL, CRYPT_VERIFYCONTEXT, FALSE);
    if (0 == hProv) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoFindProvider",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if (FALSE == CryptGenRandom(hProv, (DWORD)size, xmlSecBufferGetData(buffer))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGenRandom",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CryptReleaseContext(hProv,0);
        return(-1);
    }

    CryptReleaseContext(hProv, 0);
    return(0);
}

#define XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE       4096

/**
 * xmlSecMSCryptoErrorsDefaultCallback:
 * @file:               the error location file name (__FILE__ macro).
 * @line:               the error location line number (__LINE__ macro).
 * @func:               the error location function name (__FUNCTION__ macro).
 * @errorObject:        the error specific error object
 * @errorSubject:       the error specific error subject.
 * @reason:             the error code.
 * @msg:                the additional error message.
 *
 * The default errors reporting callback function.
 */
void
xmlSecMSCryptoErrorsDefaultCallback(const char* file, int line, const char* func,
                                const char* errorObject, const char* errorSubject,
                                int reason, const char* msg) {
    DWORD dwError;
    TCHAR errorT[XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE];
    WCHAR errorW[XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE];
    CHAR  errorUTF8[XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE];
    xmlChar buf[XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE];
    DWORD rc;
    int ret;

    dwError = GetLastError();
    rc = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  dwError,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
                  errorT,
                  XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE,
                  NULL);
    
#ifdef UNICODE
    if(rc <= 0) {
        wcscpy_s(errorT, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, L"");
    }
    ret = WideCharToMultiByte(CP_UTF8, 0, errorT, -1, errorUTF8, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, NULL, NULL);
    if(ret <= 0) {
        strcpy_s(errorUTF8, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, "");
    }
#else /* UNICODE */
    if(rc <= 0) {
        strcpy_s(errorT, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, "");
    }
    ret = MultiByteToWideChar(CP_ACP, 0, errorT, -1, errorW, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE);
    if(ret <= 0) {
        wcscpy_s(errorW, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, L"");
    }
    ret = WideCharToMultiByte(CP_UTF8, 0, errorW, -1, errorUTF8, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, NULL, NULL);
    if(ret <= 0) {
        strcpy_s(errorUTF8, XMLSEC_MSCRYPTO_ERROR_MSG_BUFFER_SIZE, "");
    }
#endif /* UNICODE */

    if((msg != NULL) && ((*msg) != '\0')) {
        xmlSecStrPrintf(buf, sizeof(buf), BAD_CAST "%s;last error=%d (0x%08x);last error msg=%s", msg, dwError, dwError, errorUTF8);
    } else {
        xmlSecStrPrintf(buf, sizeof(buf), BAD_CAST "last error=%d (0x%08x);last error msg=%s", dwError, dwError, errorUTF8);
    }
    xmlSecErrorsDefaultCallback(file, line, func,
                errorObject, errorSubject,
                reason, (char*)buf);
}

/**
 * xmlSecMSCryptoConvertUtf8ToUnicode:
 * @str:         the string to convert.
 *
 * Converts input string from UTF8 to Unicode.
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
LPWSTR
xmlSecMSCryptoConvertUtf8ToUnicode(const xmlChar* str) {
    LPWSTR res = NULL;
    int len;
    int ret;

    xmlSecAssert2(str != NULL, NULL);

    /* call MultiByteToWideChar first to get the buffer size */
    ret = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    if(ret <= 0) {
        return(NULL);
    }
    len = ret + 1;

    /* allocate buffer */
    res = (LPWSTR)xmlMalloc(sizeof(WCHAR) * len);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", sizeof(WCHAR) * len);
        return(NULL);
    }

    /* convert */
    ret = MultiByteToWideChar(CP_UTF8, 0, str, -1, res, len);
    if(ret <= 0) {
        xmlFree(res);
        return(NULL);
    }

    /* done */
    return(res);
}

/**
 * xmlSecMSCryptoConvertUnicodeToUtf8:
 * @str:         the string to convert.
 *
 * Converts input string from Unicode to UTF8.
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
xmlChar* 
xmlSecMSCryptoConvertUnicodeToUtf8(LPCWSTR str) {
    xmlChar * res = NULL;
    int len;
    int ret;

    xmlSecAssert2(str != NULL, NULL);

    /* call WideCharToMultiByte first to get the buffer size */
    ret = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    if(ret <= 0) {
        return(NULL);
    }
    len = ret + 1;

    /* allocate buffer */
    res = (xmlChar*)xmlMalloc(sizeof(xmlChar) * len);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", sizeof(xmlChar) * len);
        return(NULL);
    }

    /* convert */
    ret = WideCharToMultiByte(CP_UTF8, 0, str, -1, res, len, NULL, NULL);
    if(ret <= 0) {
        xmlFree(res);
        return(NULL);
    }

    /* done */
    return(res);
}

/**
 * xmlSecMSCryptoConvertLocaleToUnicode:
 * @str:         the string to convert.
 *
 * Converts input string from current system locale to Unicode.
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
LPWSTR
xmlSecMSCryptoConvertLocaleToUnicode(const char* str) {
    LPWSTR res = NULL;
    int len;
    int ret;

    xmlSecAssert2(str != NULL, NULL);

    /* call MultiByteToWideChar first to get the buffer size */
    ret = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    if(ret <= 0) {
        return(NULL);
    }
    len = ret;

    /* allocate buffer */
    res = (LPWSTR)xmlMalloc(sizeof(WCHAR) * len);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    /* convert */
    ret = MultiByteToWideChar(CP_ACP, 0, str, -1, res, len);
    if(ret <= 0) {
            xmlFree(res);
            return(NULL);
    }

    /* done */
    return(res);
}

/**
 * xmlSecMSCryptoConvertLocaleToUtf8:
 * @str:         the string to convert.
 *
 * Converts input string from locale to UTF8.
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
xmlChar* 
xmlSecMSCryptoConvertLocaleToUtf8(const char * str) {
    LPWSTR strW = NULL;
    xmlChar * res = NULL;
    int len;
    int ret;

    xmlSecAssert2(str != NULL, NULL);

    strW = xmlSecMSCryptoConvertLocaleToUnicode(str);
    if(strW == NULL) {
        return(NULL);
    }

    /* call WideCharToMultiByte first to get the buffer size */
    ret = WideCharToMultiByte(CP_ACP, 0, strW, -1, NULL, 0, NULL, NULL);
    if(ret <= 0) {
        xmlFree(strW);
        return(NULL);
    }
    len = ret + 1;

    /* allocate buffer */
    res = (xmlChar*)xmlMalloc(sizeof(xmlChar) * len);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", sizeof(xmlChar) * len);
        xmlFree(strW);
        return(NULL);
    }

    /* convert */
    ret = WideCharToMultiByte(CP_ACP, 0, strW, -1, res, len, NULL, NULL);
    if(ret <= 0) {
        xmlFree(strW);
        xmlFree(res);
        return(NULL);
    }

    /* done */
    xmlFree(strW);
    return(res);
}

/**
 * xmlSecMSCryptoConvertUtf8ToLocale:
 * @str:         the string to convert.
 *
 * Converts input string from UTF8 to locale.
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
char * 
xmlSecMSCryptoConvertUtf8ToLocale(const xmlChar* str) {
    LPWSTR strW = NULL;
    char * res = NULL;
    int len;
    int ret;

    xmlSecAssert2(str != NULL, NULL);

    strW = xmlSecMSCryptoConvertUtf8ToUnicode(str);
    if(strW == NULL) {
        return(NULL);
    }

    /* call WideCharToMultiByte first to get the buffer size */
    ret = WideCharToMultiByte(CP_ACP, 0, strW, -1, NULL, 0, NULL, NULL);
    if(ret <= 0) {
        xmlFree(strW);
        return(NULL);
    }
    len = ret + 1;

    /* allocate buffer */
    res = (char*)xmlMalloc(sizeof(char) * len);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", sizeof(xmlChar) * len);
        xmlFree(strW);
        return(NULL);
    }

    /* convert */
    ret = WideCharToMultiByte(CP_ACP, 0, strW, -1, res, len, NULL, NULL);
    if(ret <= 0) {
        xmlFree(strW);
        xmlFree(res);
        return(NULL);
    }

    /* done */
    xmlFree(strW);
    return(res);
}

/**
 * xmlSecMSCryptoConvertTstrToUtf8:
 * @str:         the string to convert.
 *
 * Converts input string from TSTR (locale or Unicode) to UTF8.
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
xmlChar* 
xmlSecMSCryptoConvertTstrToUtf8(LPCTSTR str) {
#ifdef UNICODE
    return xmlSecMSCryptoConvertUnicodeToUtf8(str);
#else  /* UNICODE */
    return xmlSecMSCryptoConvertLocaleToUtf8(str);
#endif /* UNICODE */
}

/**
 * xmlSecMSCryptoConvertUtf8ToTstr:
 * @str:         the string to convert.
 *
 * Converts input string from UTF8 to TSTR (locale or Unicode).
 *
 * Returns: a pointer to newly allocated string (must be freed with xmlFree) or NULL if an error occurs.
 */
LPTSTR
xmlSecMSCryptoConvertUtf8ToTstr(const xmlChar*  str) {
#ifdef UNICODE
    return xmlSecMSCryptoConvertUtf8ToUnicode(str);
#else  /* UNICODE */
    return xmlSecMSCryptoConvertUtf8ToLocale(str);
#endif /* UNICODE */
}

/********************************************************************
 *
 * Crypto Providers
 *
 ********************************************************************/
/**
 * xmlSecMSCryptoFindProvider:
 * @providers:           the pointer to list of providers, last provider should have NULL for name.
 * @pszContainer:        the container name for CryptAcquireContext call
 * @dwFlags:             the flags for CryptAcquireContext call
 * @bUseXmlSecContainer: the flag to indicate whether we should try to use XmlSec container if default fails
 *
 * Finds the first provider from the list
 *
 * Returns: provider handle on success or NULL for error.
 */
HCRYPTPROV
xmlSecMSCryptoFindProvider(const xmlSecMSCryptoProviderInfo * providers,
                           LPCTSTR pszContainer,
                           DWORD dwFlags,
                           BOOL bUseXmlSecContainer)
{
    HCRYPTPROV res = 0;
    DWORD dwLastError;
    BOOL ret;
    int ii;

    xmlSecAssert2(providers != NULL, 0);

    for(ii = 0; (res == 0) && (providers[ii].providerName != NULL) && (providers[ii].providerType != 0); ++ii) {
        /* first try */
        ret = CryptAcquireContext(&res,
                    pszContainer,
                    providers[ii].providerName,
                    providers[ii].providerType,
                    dwFlags);
        if((ret == TRUE) && (res != 0)) {
            return (res);
        }

        /* check errors */
        dwLastError = GetLastError();
        switch(dwLastError) {
        case NTE_BAD_KEYSET:
            /* This error can indicate that a newly installed provider
             * does not have a usable key container yet. It needs to be
             * created, and then we have to try again CryptAcquireContext.
             * This is also referenced in
             * http://www.microsoft.com/mind/0697/crypto.asp (inituser)
             */
            ret = CryptAcquireContext(&res,
                        pszContainer,
                        providers[ii].providerName,
                        providers[ii].providerType,
                        CRYPT_NEWKEYSET | dwFlags);
            if((ret == TRUE) && (res != 0)) {
                return (res);
            }
            break;

        case NTE_EXISTS:
            /* If we can, try our container */
            if(bUseXmlSecContainer == TRUE) {
                ret = CryptAcquireContext(&res,
                            XMLSEC_CONTAINER_NAME,
                            providers[ii].providerName,
                            providers[ii].providerType,
                            CRYPT_NEWKEYSET | dwFlags);
                if((ret == TRUE) && (res != 0)) {
                    /* ALEKSEY TODO - NEED TO DELETE ALL THE TEMP CONTEXTS ON SHUTDOWN

                        CryptAcquireContext(&tmp, XMLSEC_CONTAINER_NAME,
                            providers[ii].providerName,
                            providers[ii].providerType,
                            CRYPT_DELETEKEYSET);

                     */
                    return (res);
                }
            }
            break;

        default:
            /* ignore */
            break;
        }
    }

    return (0);
}


/********************************************************************
 *
 * Utils
 *
 ********************************************************************/
int
ConvertEndian(const xmlSecByte * src, xmlSecByte * dst, xmlSecSize size) {
    xmlSecByte * p;

    xmlSecAssert2(src != NULL, -1);
    xmlSecAssert2(dst != NULL, -1);
    xmlSecAssert2(size > 0, -1);

    for(p = dst + size - 1; p >= dst; ++src, --p) {
        (*p) = (*src);
    }

    return (0);
}

int
ConvertEndianInPlace(xmlSecByte * buf, xmlSecSize size) {
    xmlSecByte * p;
    xmlSecByte ch;

    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(size > 0, -1);

    for(p = buf + size - 1; p >= buf; ++buf, --p) {
        ch = (*p);
        (*p) = (*buf);
        (*buf) = ch;
    }
    return (0);
}



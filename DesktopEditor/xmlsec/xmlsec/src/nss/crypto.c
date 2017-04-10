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

#include <nss.h>
#include <pk11func.h>
#include <prinit.h>


#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>
#include <xmlsec/dl.h>
#include <xmlsec/private.h>
#include <xmlsec/xmltree.h>

#include <xmlsec/nss/app.h>
#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/x509.h>

static xmlSecCryptoDLFunctionsPtr gXmlSecNssFunctions = NULL;

/**
 * xmlSecCryptoGetFunctions_nss:
 *
 * Gets the pointer to xmlsec-nss functions table.
 *
 * Returns: the xmlsec-nss functions table or NULL if an error occurs.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoGetFunctions_nss(void) {
    static xmlSecCryptoDLFunctions functions;

    if(gXmlSecNssFunctions != NULL) {
        return(gXmlSecNssFunctions);
    }

    memset(&functions, 0, sizeof(functions));
    gXmlSecNssFunctions = &functions;

    /********************************************************************
     *
     * Crypto Init/shutdown
     *
     ********************************************************************/
    gXmlSecNssFunctions->cryptoInit                     = xmlSecNssInit;
    gXmlSecNssFunctions->cryptoShutdown                 = xmlSecNssShutdown;
    gXmlSecNssFunctions->cryptoKeysMngrInit             = xmlSecNssKeysMngrInit;

    /********************************************************************
     *
     * Key data ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_AES
    gXmlSecNssFunctions->keyDataAesGetKlass             = xmlSecNssKeyDataAesGetKlass;
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
    gXmlSecNssFunctions->keyDataDesGetKlass             = xmlSecNssKeyDataDesGetKlass;
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_DSA
    gXmlSecNssFunctions->keyDataDsaGetKlass             = xmlSecNssKeyDataDsaGetKlass;
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_HMAC
    gXmlSecNssFunctions->keyDataHmacGetKlass            = xmlSecNssKeyDataHmacGetKlass;
#endif /* XMLSEC_NO_HMAC */

#ifndef XMLSEC_NO_RSA
    gXmlSecNssFunctions->keyDataRsaGetKlass             = xmlSecNssKeyDataRsaGetKlass;
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_X509
    gXmlSecNssFunctions->keyDataX509GetKlass            = xmlSecNssKeyDataX509GetKlass;
    gXmlSecNssFunctions->keyDataRawX509CertGetKlass     = xmlSecNssKeyDataRawX509CertGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Key data store ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_X509
    gXmlSecNssFunctions->x509StoreGetKlass              = xmlSecNssX509StoreGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Crypto transforms ids
     *
     ********************************************************************/

    /******************************* AES ********************************/
#ifndef XMLSEC_NO_AES
    gXmlSecNssFunctions->transformAes128CbcGetKlass     = xmlSecNssTransformAes128CbcGetKlass;
    gXmlSecNssFunctions->transformAes192CbcGetKlass     = xmlSecNssTransformAes192CbcGetKlass;
    gXmlSecNssFunctions->transformAes256CbcGetKlass     = xmlSecNssTransformAes256CbcGetKlass;
    gXmlSecNssFunctions->transformKWAes128GetKlass      = xmlSecNssTransformKWAes128GetKlass;
    gXmlSecNssFunctions->transformKWAes192GetKlass      = xmlSecNssTransformKWAes192GetKlass;
    gXmlSecNssFunctions->transformKWAes256GetKlass      = xmlSecNssTransformKWAes256GetKlass;
#endif /* XMLSEC_NO_AES */

    /******************************* DES ********************************/
#ifndef XMLSEC_NO_DES
    gXmlSecNssFunctions->transformDes3CbcGetKlass       = xmlSecNssTransformDes3CbcGetKlass;
    gXmlSecNssFunctions->transformKWDes3GetKlass        = xmlSecNssTransformKWDes3GetKlass;
#endif /* XMLSEC_NO_DES */

    /******************************* DSA ********************************/
#ifndef XMLSEC_NO_DSA
    gXmlSecNssFunctions->transformDsaSha1GetKlass       = xmlSecNssTransformDsaSha1GetKlass;
#endif /* XMLSEC_NO_DSA */

    /******************************* HMAC ********************************/
#ifndef XMLSEC_NO_HMAC

#ifndef XMLSEC_NO_MD5
    gXmlSecNssFunctions->transformHmacMd5GetKlass       = xmlSecNssTransformHmacMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecNssFunctions->transformHmacRipemd160GetKlass = xmlSecNssTransformHmacRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecNssFunctions->transformHmacSha1GetKlass      = xmlSecNssTransformHmacSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecNssFunctions->transformHmacSha256GetKlass    = xmlSecNssTransformHmacSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecNssFunctions->transformHmacSha384GetKlass    = xmlSecNssTransformHmacSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecNssFunctions->transformHmacSha512GetKlass    = xmlSecNssTransformHmacSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

    /******************************* RSA ********************************/
#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    gXmlSecNssFunctions->transformRsaMd5GetKlass        = xmlSecNssTransformRsaMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecNssFunctions->transformRsaSha1GetKlass       = xmlSecNssTransformRsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecNssFunctions->transformRsaSha256GetKlass     = xmlSecNssTransformRsaSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecNssFunctions->transformRsaSha384GetKlass     = xmlSecNssTransformRsaSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecNssFunctions->transformRsaSha512GetKlass     = xmlSecNssTransformRsaSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

    gXmlSecNssFunctions->transformRsaPkcs1GetKlass      = xmlSecNssTransformRsaPkcs1GetKlass;

/* aleksey, April 2010: NSS 3.12.6 has CKM_RSA_PKCS_OAEP algorithm but
   it doesn't implement the SHA1 OAEP PKCS we need

   https://bugzilla.mozilla.org/show_bug.cgi?id=158747
*/
#ifdef XMLSEC_NSS_RSA_OAEP_TODO
    gXmlSecNssFunctions->transformRsaOaepGetKlass       = xmlSecNssTransformRsaOaepGetKlass;
#endif /* XMLSEC_NSS_RSA_OAEP_TODO */

#endif /* XMLSEC_NO_RSA */

    /******************************* SHA ********************************/
#ifndef XMLSEC_NO_SHA1
    gXmlSecNssFunctions->transformSha1GetKlass          = xmlSecNssTransformSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */
#ifndef XMLSEC_NO_SHA256
    gXmlSecNssFunctions->transformSha256GetKlass        = xmlSecNssTransformSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */
#ifndef XMLSEC_NO_SHA384
    gXmlSecNssFunctions->transformSha384GetKlass        = xmlSecNssTransformSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */
#ifndef XMLSEC_NO_SHA512
    gXmlSecNssFunctions->transformSha512GetKlass        = xmlSecNssTransformSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

    /******************************* MD5 ********************************/
#ifndef XMLSEC_NO_MD5
    gXmlSecNssFunctions->transformMd5GetKlass           = xmlSecNssTransformMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */


    /********************************************************************
     *
     * High level routines form xmlsec command line utility
     *
     ********************************************************************/
    gXmlSecNssFunctions->cryptoAppInit                  = xmlSecNssAppInit;
    gXmlSecNssFunctions->cryptoAppShutdown              = xmlSecNssAppShutdown;
    gXmlSecNssFunctions->cryptoAppDefaultKeysMngrInit   = xmlSecNssAppDefaultKeysMngrInit;
    gXmlSecNssFunctions->cryptoAppDefaultKeysMngrAdoptKey       = xmlSecNssAppDefaultKeysMngrAdoptKey;
    gXmlSecNssFunctions->cryptoAppDefaultKeysMngrLoad   = xmlSecNssAppDefaultKeysMngrLoad;
    gXmlSecNssFunctions->cryptoAppDefaultKeysMngrSave   = xmlSecNssAppDefaultKeysMngrSave;
#ifndef XMLSEC_NO_X509
    gXmlSecNssFunctions->cryptoAppKeysMngrCertLoad      = xmlSecNssAppKeysMngrCertLoad;
    gXmlSecNssFunctions->cryptoAppKeysMngrCertLoadMemory= xmlSecNssAppKeysMngrCertLoadMemory;
    gXmlSecNssFunctions->cryptoAppPkcs12Load            = xmlSecNssAppPkcs12Load;
    gXmlSecNssFunctions->cryptoAppPkcs12LoadMemory      = xmlSecNssAppPkcs12LoadMemory;
    gXmlSecNssFunctions->cryptoAppKeyCertLoad           = xmlSecNssAppKeyCertLoad;
    gXmlSecNssFunctions->cryptoAppKeyCertLoadMemory     = xmlSecNssAppKeyCertLoadMemory;
#endif /* XMLSEC_NO_X509 */
    gXmlSecNssFunctions->cryptoAppKeyLoad               = xmlSecNssAppKeyLoad;
    gXmlSecNssFunctions->cryptoAppKeyLoadMemory         = xmlSecNssAppKeyLoadMemory;
    gXmlSecNssFunctions->cryptoAppDefaultPwdCallback    = (void*)xmlSecNssAppGetDefaultPwdCallback();

    return(gXmlSecNssFunctions);
}

/**
 * xmlSecNssInit:
 *
 * XMLSec library specific crypto engine initialization.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssInit (void)  {
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
    xmlSecErrorsSetCallback(xmlSecNssErrorsDefaultCallback);

    /* register our klasses */
    if(xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms(xmlSecCryptoGetFunctions_nss()) < 0) {
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
 * xmlSecNssShutdown:
 *
 * XMLSec library specific crypto engine shutdown.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssShutdown(void) {
    return(0);
}

/**
 * xmlSecNssKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Adds NSS specific key data stores in keys manager.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

#ifndef XMLSEC_NO_X509
    /* create x509 store if needed */
    if(xmlSecKeysMngrGetDataStore(mngr, xmlSecNssX509StoreId) == NULL) {
        xmlSecKeyDataStorePtr x509Store;

        x509Store = xmlSecKeyDataStoreCreate(xmlSecNssX509StoreId);
        if(x509Store == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataStoreCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecNssX509StoreId");
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

/**
 * xmlSecNssGetInternalKeySlot:
 *
 * Gets internal NSS key slot.
 *
 * Returns: internal key slot and initializes it if needed.
 */
PK11SlotInfo *
xmlSecNssGetInternalKeySlot()
{
    PK11SlotInfo *slot = NULL;
    SECStatus rv;

    slot = PK11_GetInternalKeySlot();
    if (slot == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "PK11_GetInternalKeySlot",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                "error code=%d", PORT_GetError());
        return NULL;
    }

    if (PK11_NeedUserInit(slot)) {
        rv = PK11_InitPin(slot, NULL, NULL);
        if (rv != SECSuccess) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "PK11_Authenticate",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
            return NULL;
        }
    }

    if(PK11_IsLoggedIn(slot, NULL) != PR_TRUE) {
        rv = PK11_Authenticate(slot, PR_TRUE, NULL);
        if (rv != SECSuccess) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "PK11_Authenticate",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
            return NULL;
        }
    }

    return(slot);
}

/**
 * xmlSecNssGenerateRandom:
 * @buffer:             the destination buffer.
 * @size:               the numer of bytes to generate.
 *
 * Generates @size random bytes and puts result in @buffer.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecNssGenerateRandom(xmlSecBufferPtr buffer, xmlSecSize size) {
    SECStatus rv;
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

    /* get random data */
    rv = PK11_GenerateRandom((xmlSecByte*)xmlSecBufferGetData(buffer), size);
    if(rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_GenerateRandom",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "size=%d", size);
        return(-1);
    }
    return(0);
}

/**
 * xmlSecNssErrorsDefaultCallback:
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
xmlSecNssErrorsDefaultCallback(const char* file, int line, const char* func,
                                const char* errorObject, const char* errorSubject,
                                int reason, const char* msg) {
    xmlChar buf[500];
    int err;

    err = PORT_GetError();
    xmlSecStrPrintf(buf, sizeof(buf), BAD_CAST "%s;last nss error=%d (0x%08X)", msg, err, err);
    xmlSecErrorsDefaultCallback(file, line, func,
                errorObject, errorSubject,
                reason, (char*)buf);
}

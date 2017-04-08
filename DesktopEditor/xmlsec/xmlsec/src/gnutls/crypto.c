/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <gcrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>
#include <xmlsec/dl.h>
#include <xmlsec/private.h>

#include <xmlsec/gnutls/app.h>
#include <xmlsec/gnutls/crypto.h>
#include <xmlsec/gnutls/x509.h>

static xmlSecCryptoDLFunctionsPtr gXmlSecGnuTLSFunctions = NULL;

/**
 * xmlSecCryptoGetFunctions_gnutls:
 *
 * Gets the pointer to xmlsec-gnutls functions table.
 *
 * Returns: the xmlsec-gnutls functions table or NULL if an error occurs.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoGetFunctions_gnutls(void) {
    static xmlSecCryptoDLFunctions functions;

    if(gXmlSecGnuTLSFunctions != NULL) {
        return(gXmlSecGnuTLSFunctions);
    }

    memset(&functions, 0, sizeof(functions));
    gXmlSecGnuTLSFunctions = &functions;

    /********************************************************************
     *
     * Crypto Init/shutdown
     *
     ********************************************************************/
    gXmlSecGnuTLSFunctions->cryptoInit                  = xmlSecGnuTLSInit;
    gXmlSecGnuTLSFunctions->cryptoShutdown              = xmlSecGnuTLSShutdown;
    gXmlSecGnuTLSFunctions->cryptoKeysMngrInit          = xmlSecGnuTLSKeysMngrInit;

    /********************************************************************
     *
     * Key data ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_AES
    gXmlSecGnuTLSFunctions->keyDataAesGetKlass          = xmlSecGnuTLSKeyDataAesGetKlass;
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
    gXmlSecGnuTLSFunctions->keyDataDesGetKlass          = xmlSecGnuTLSKeyDataDesGetKlass;
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_DSA
    gXmlSecGnuTLSFunctions->keyDataDsaGetKlass          = xmlSecGnuTLSKeyDataDsaGetKlass;
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_HMAC
    gXmlSecGnuTLSFunctions->keyDataHmacGetKlass         = xmlSecGnuTLSKeyDataHmacGetKlass;
#endif /* XMLSEC_NO_HMAC */

#ifndef XMLSEC_NO_RSA
    gXmlSecGnuTLSFunctions->keyDataRsaGetKlass          = xmlSecGnuTLSKeyDataRsaGetKlass;
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_X509
    gXmlSecGnuTLSFunctions->keyDataX509GetKlass         = xmlSecGnuTLSKeyDataX509GetKlass;
    gXmlSecGnuTLSFunctions->keyDataRawX509CertGetKlass  = xmlSecGnuTLSKeyDataRawX509CertGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Key data store ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_X509
    gXmlSecGnuTLSFunctions->x509StoreGetKlass           = xmlSecGnuTLSX509StoreGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Crypto transforms ids
     *
     ********************************************************************/

    /******************************* AES ********************************/
#ifndef XMLSEC_NO_AES
    gXmlSecGnuTLSFunctions->transformAes128CbcGetKlass          = xmlSecGnuTLSTransformAes128CbcGetKlass;
    gXmlSecGnuTLSFunctions->transformAes192CbcGetKlass          = xmlSecGnuTLSTransformAes192CbcGetKlass;
    gXmlSecGnuTLSFunctions->transformAes256CbcGetKlass          = xmlSecGnuTLSTransformAes256CbcGetKlass;
    gXmlSecGnuTLSFunctions->transformKWAes128GetKlass           = xmlSecGnuTLSTransformKWAes128GetKlass;
    gXmlSecGnuTLSFunctions->transformKWAes192GetKlass           = xmlSecGnuTLSTransformKWAes192GetKlass;
    gXmlSecGnuTLSFunctions->transformKWAes256GetKlass           = xmlSecGnuTLSTransformKWAes256GetKlass;
#endif /* XMLSEC_NO_AES */

    /******************************* DES ********************************/
#ifndef XMLSEC_NO_DES
    gXmlSecGnuTLSFunctions->transformDes3CbcGetKlass            = xmlSecGnuTLSTransformDes3CbcGetKlass;
    gXmlSecGnuTLSFunctions->transformKWDes3GetKlass             = xmlSecGnuTLSTransformKWDes3GetKlass;
#endif /* XMLSEC_NO_DES */

    /******************************* DSA ********************************/
#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1
    gXmlSecGnuTLSFunctions->transformDsaSha1GetKlass            = xmlSecGnuTLSTransformDsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */

    /******************************* HMAC ********************************/
#ifndef XMLSEC_NO_HMAC

#ifndef XMLSEC_NO_MD5
    gXmlSecGnuTLSFunctions->transformHmacMd5GetKlass            = xmlSecGnuTLSTransformHmacMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecGnuTLSFunctions->transformHmacRipemd160GetKlass      = xmlSecGnuTLSTransformHmacRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecGnuTLSFunctions->transformHmacSha1GetKlass           = xmlSecGnuTLSTransformHmacSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecGnuTLSFunctions->transformHmacSha256GetKlass         = xmlSecGnuTLSTransformHmacSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecGnuTLSFunctions->transformHmacSha384GetKlass         = xmlSecGnuTLSTransformHmacSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecGnuTLSFunctions->transformHmacSha512GetKlass         = xmlSecGnuTLSTransformHmacSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

    /******************************* MD5 ********************************/
#ifndef XMLSEC_NO_MD5
    gXmlSecGnuTLSFunctions->transformMd5GetKlass                = xmlSecGnuTLSTransformMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

    /******************************* RIPEMD160 ********************************/
#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecGnuTLSFunctions->transformRipemd160GetKlass          = xmlSecGnuTLSTransformRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

    /******************************* RSA ********************************/
#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    gXmlSecGnuTLSFunctions->transformRsaMd5GetKlass            = xmlSecGnuTLSTransformRsaMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecGnuTLSFunctions->transformRsaRipemd160GetKlass      = xmlSecGnuTLSTransformRsaRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecGnuTLSFunctions->transformRsaSha1GetKlass           = xmlSecGnuTLSTransformRsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecGnuTLSFunctions->transformRsaSha256GetKlass         = xmlSecGnuTLSTransformRsaSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecGnuTLSFunctions->transformRsaSha384GetKlass         = xmlSecGnuTLSTransformRsaSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecGnuTLSFunctions->transformRsaSha512GetKlass         = xmlSecGnuTLSTransformRsaSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

    /******************************* SHA ********************************/
#ifndef XMLSEC_NO_SHA1
    gXmlSecGnuTLSFunctions->transformSha1GetKlass               = xmlSecGnuTLSTransformSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecGnuTLSFunctions->transformSha256GetKlass             = xmlSecGnuTLSTransformSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecGnuTLSFunctions->transformSha384GetKlass             = xmlSecGnuTLSTransformSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecGnuTLSFunctions->transformSha512GetKlass             = xmlSecGnuTLSTransformSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */


    /********************************************************************
     *
     * High level routines form xmlsec command line utility
     *
     ********************************************************************/
    gXmlSecGnuTLSFunctions->cryptoAppInit                       = xmlSecGnuTLSAppInit;
    gXmlSecGnuTLSFunctions->cryptoAppShutdown                   = xmlSecGnuTLSAppShutdown;
    gXmlSecGnuTLSFunctions->cryptoAppDefaultKeysMngrInit        = xmlSecGnuTLSAppDefaultKeysMngrInit;
    gXmlSecGnuTLSFunctions->cryptoAppDefaultKeysMngrAdoptKey    = xmlSecGnuTLSAppDefaultKeysMngrAdoptKey;
    gXmlSecGnuTLSFunctions->cryptoAppDefaultKeysMngrLoad        = xmlSecGnuTLSAppDefaultKeysMngrLoad;
    gXmlSecGnuTLSFunctions->cryptoAppDefaultKeysMngrSave        = xmlSecGnuTLSAppDefaultKeysMngrSave;
#ifndef XMLSEC_NO_X509
    gXmlSecGnuTLSFunctions->cryptoAppKeysMngrCertLoad           = xmlSecGnuTLSAppKeysMngrCertLoad;
    gXmlSecGnuTLSFunctions->cryptoAppPkcs12Load                 = xmlSecGnuTLSAppPkcs12Load;
    gXmlSecGnuTLSFunctions->cryptoAppKeyCertLoad                = xmlSecGnuTLSAppKeyCertLoad;
#endif /* XMLSEC_NO_X509 */
    gXmlSecGnuTLSFunctions->cryptoAppKeyLoad                    = xmlSecGnuTLSAppKeyLoad;
    gXmlSecGnuTLSFunctions->cryptoAppDefaultPwdCallback         = (void*)xmlSecGnuTLSAppGetDefaultPwdCallback();

    return(gXmlSecGnuTLSFunctions);
}


/**
 * xmlSecGnuTLSInit:
 *
 * XMLSec library specific crypto engine initialization.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSInit (void)  {
    /* Check loaded xmlsec library version */
    if(xmlSecCheckVersionExact() != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCheckVersionExact",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* register our klasses */
    if(xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms(xmlSecCryptoGetFunctions_gnutls()) < 0) {
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
 * xmlSecGnuTLSShutdown:
 *
 * XMLSec library specific crypto engine shutdown.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSShutdown(void) {
    return(0);
}

/**
 * xmlSecGnuTLSKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Adds GnuTLS specific key data stores in keys manager.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

#ifndef XMLSEC_NO_X509
    /* create x509 store if needed */
    if(xmlSecKeysMngrGetDataStore(mngr, xmlSecGnuTLSX509StoreId) == NULL) {
        xmlSecKeyDataStorePtr x509Store;

        x509Store = xmlSecKeyDataStoreCreate(xmlSecGnuTLSX509StoreId);
        if(x509Store == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataStoreCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGnuTLSX509StoreId");
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
 * xmlSecGnuTLSGenerateRandom:
 * @buffer:             the destination buffer.
 * @size:               the numer of bytes to generate.
 *
 * Generates @size random bytes and puts result in @buffer.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSGenerateRandom(xmlSecBufferPtr buffer, xmlSecSize size) {
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
    gcry_randomize(xmlSecBufferGetData(buffer), size, GCRY_STRONG_RANDOM);
    return(0);
}

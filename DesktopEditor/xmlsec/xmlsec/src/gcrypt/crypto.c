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

#include <xmlsec/gcrypt/app.h>
#include <xmlsec/gcrypt/crypto.h>

static xmlSecCryptoDLFunctionsPtr gXmlSecGCryptFunctions = NULL;

/**
 * xmlSecCryptoGetFunctions_gcrypt:
 *
 * Gets the pointer to xmlsec-gcrypt functions table.
 *
 * Returns: the xmlsec-gcrypt functions table or NULL if an error occurs.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoGetFunctions_gcrypt(void) {
    static xmlSecCryptoDLFunctions functions;

    if(gXmlSecGCryptFunctions != NULL) {
        return(gXmlSecGCryptFunctions);
    }

    memset(&functions, 0, sizeof(functions));
    gXmlSecGCryptFunctions = &functions;

    /********************************************************************
     *
     * Crypto Init/shutdown
     *
     ********************************************************************/
    gXmlSecGCryptFunctions->cryptoInit                  = xmlSecGCryptInit;
    gXmlSecGCryptFunctions->cryptoShutdown              = xmlSecGCryptShutdown;
    gXmlSecGCryptFunctions->cryptoKeysMngrInit          = xmlSecGCryptKeysMngrInit;

    /********************************************************************
     *
     * Key data ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_AES
    gXmlSecGCryptFunctions->keyDataAesGetKlass          = xmlSecGCryptKeyDataAesGetKlass;
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
    gXmlSecGCryptFunctions->keyDataDesGetKlass          = xmlSecGCryptKeyDataDesGetKlass;
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_DSA
    gXmlSecGCryptFunctions->keyDataDsaGetKlass          = xmlSecGCryptKeyDataDsaGetKlass;
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_HMAC
    gXmlSecGCryptFunctions->keyDataHmacGetKlass         = xmlSecGCryptKeyDataHmacGetKlass;
#endif /* XMLSEC_NO_HMAC */

#ifndef XMLSEC_NO_RSA
    gXmlSecGCryptFunctions->keyDataRsaGetKlass          = xmlSecGCryptKeyDataRsaGetKlass;
#endif /* XMLSEC_NO_RSA */


    /********************************************************************
     *
     * Key data store ids
     *
     ********************************************************************/

    /********************************************************************
     *
     * Crypto transforms ids
     *
     ********************************************************************/

    /******************************* AES ********************************/
#ifndef XMLSEC_NO_AES
    gXmlSecGCryptFunctions->transformAes128CbcGetKlass          = xmlSecGCryptTransformAes128CbcGetKlass;
    gXmlSecGCryptFunctions->transformAes192CbcGetKlass          = xmlSecGCryptTransformAes192CbcGetKlass;
    gXmlSecGCryptFunctions->transformAes256CbcGetKlass          = xmlSecGCryptTransformAes256CbcGetKlass;
    gXmlSecGCryptFunctions->transformKWAes128GetKlass           = xmlSecGCryptTransformKWAes128GetKlass;
    gXmlSecGCryptFunctions->transformKWAes192GetKlass           = xmlSecGCryptTransformKWAes192GetKlass;
    gXmlSecGCryptFunctions->transformKWAes256GetKlass           = xmlSecGCryptTransformKWAes256GetKlass;
#endif /* XMLSEC_NO_AES */

    /******************************* DES ********************************/
#ifndef XMLSEC_NO_DES
    gXmlSecGCryptFunctions->transformDes3CbcGetKlass            = xmlSecGCryptTransformDes3CbcGetKlass;
    gXmlSecGCryptFunctions->transformKWDes3GetKlass             = xmlSecGCryptTransformKWDes3GetKlass;
#endif /* XMLSEC_NO_DES */

    /******************************* DSA ********************************/
#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1
    gXmlSecGCryptFunctions->transformDsaSha1GetKlass            = xmlSecGCryptTransformDsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */

    /******************************* HMAC ********************************/
#ifndef XMLSEC_NO_HMAC

#ifndef XMLSEC_NO_MD5
    gXmlSecGCryptFunctions->transformHmacMd5GetKlass            = xmlSecGCryptTransformHmacMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecGCryptFunctions->transformHmacRipemd160GetKlass      = xmlSecGCryptTransformHmacRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecGCryptFunctions->transformHmacSha1GetKlass           = xmlSecGCryptTransformHmacSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecGCryptFunctions->transformHmacSha256GetKlass         = xmlSecGCryptTransformHmacSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecGCryptFunctions->transformHmacSha384GetKlass         = xmlSecGCryptTransformHmacSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecGCryptFunctions->transformHmacSha512GetKlass         = xmlSecGCryptTransformHmacSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

    /******************************* MD5 ********************************/
#ifndef XMLSEC_NO_MD5
    gXmlSecGCryptFunctions->transformMd5GetKlass                = xmlSecGCryptTransformMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

    /******************************* RIPEMD160 ********************************/
#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecGCryptFunctions->transformRipemd160GetKlass          = xmlSecGCryptTransformRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

    /******************************* RSA ********************************/
#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    gXmlSecGCryptFunctions->transformRsaMd5GetKlass            = xmlSecGCryptTransformRsaMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecGCryptFunctions->transformRsaRipemd160GetKlass      = xmlSecGCryptTransformRsaRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    gXmlSecGCryptFunctions->transformRsaSha1GetKlass           = xmlSecGCryptTransformRsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecGCryptFunctions->transformRsaSha256GetKlass         = xmlSecGCryptTransformRsaSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecGCryptFunctions->transformRsaSha384GetKlass         = xmlSecGCryptTransformRsaSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecGCryptFunctions->transformRsaSha512GetKlass         = xmlSecGCryptTransformRsaSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

    /******************************* SHA ********************************/
#ifndef XMLSEC_NO_SHA1
    gXmlSecGCryptFunctions->transformSha1GetKlass               = xmlSecGCryptTransformSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecGCryptFunctions->transformSha256GetKlass             = xmlSecGCryptTransformSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecGCryptFunctions->transformSha384GetKlass             = xmlSecGCryptTransformSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecGCryptFunctions->transformSha512GetKlass             = xmlSecGCryptTransformSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */


    /********************************************************************
     *
     * High level routines form xmlsec command line utility
     *
     ********************************************************************/
    gXmlSecGCryptFunctions->cryptoAppInit                       = xmlSecGCryptAppInit;
    gXmlSecGCryptFunctions->cryptoAppShutdown                   = xmlSecGCryptAppShutdown;
    gXmlSecGCryptFunctions->cryptoAppDefaultKeysMngrInit        = xmlSecGCryptAppDefaultKeysMngrInit;
    gXmlSecGCryptFunctions->cryptoAppDefaultKeysMngrAdoptKey    = xmlSecGCryptAppDefaultKeysMngrAdoptKey;
    gXmlSecGCryptFunctions->cryptoAppDefaultKeysMngrLoad        = xmlSecGCryptAppDefaultKeysMngrLoad;
    gXmlSecGCryptFunctions->cryptoAppDefaultKeysMngrSave        = xmlSecGCryptAppDefaultKeysMngrSave;
#ifndef XMLSEC_NO_X509
    gXmlSecGCryptFunctions->cryptoAppKeysMngrCertLoad           = xmlSecGCryptAppKeysMngrCertLoad;
    gXmlSecGCryptFunctions->cryptoAppPkcs12Load                 = xmlSecGCryptAppPkcs12Load;
    gXmlSecGCryptFunctions->cryptoAppKeyCertLoad                = xmlSecGCryptAppKeyCertLoad;
#endif /* XMLSEC_NO_X509 */
    gXmlSecGCryptFunctions->cryptoAppKeyLoad                    = xmlSecGCryptAppKeyLoad;
    gXmlSecGCryptFunctions->cryptoAppDefaultPwdCallback         = (void*)xmlSecGCryptAppGetDefaultPwdCallback();

    return(gXmlSecGCryptFunctions);
}


/**
 * xmlSecGCryptInit:
 *
 * XMLSec library specific crypto engine initialization.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptInit (void)  {
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
    if(xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms(xmlSecCryptoGetFunctions_gcrypt()) < 0) {
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
 * xmlSecGCryptShutdown:
 *
 * XMLSec library specific crypto engine shutdown.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptShutdown(void) {
    return(0);
}

/**
 * xmlSecGCryptKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Adds GCrypt specific key data stores in keys manager.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    xmlSecAssert2(mngr != NULL, -1);

    /* TODO: add key data stores */
    return(0);
}

/**
 * xmlSecGCryptGenerateRandom:
 * @buffer:             the destination buffer.
 * @size:               the numer of bytes to generate.
 *
 * Generates @size random bytes and puts result in @buffer.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptGenerateRandom(xmlSecBufferPtr buffer, xmlSecSize size) {
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

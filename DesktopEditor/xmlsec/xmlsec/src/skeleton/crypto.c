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

/* TODO: add Skeleton include files */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>
#include <xmlsec/dl.h>
#include <xmlsec/private.h>

#include <xmlsec/skeleton/app.h>
#include <xmlsec/skeleton/crypto.h>

static xmlSecCryptoDLFunctionsPtr gXmlSecSkeletonFunctions = NULL;

/**
 * xmlSecCryptoGetFunctions_skeleton:
 *
 * Gets the pointer to xmlsec-skeleton functions table.
 *
 * Returns: the xmlsec-skeleton functions table or NULL if an error occurs.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoGetFunctions_skeleton(void) {
    static xmlSecCryptoDLFunctions functions;

    if(gXmlSecSkeletonFunctions != NULL) {
        return(gXmlSecSkeletonFunctions);
    }

    memset(&functions, 0, sizeof(functions));
    gXmlSecSkeletonFunctions = &functions;

    /********************************************************************
     *
     * Crypto Init/shutdown
     *
     ********************************************************************/
    gXmlSecSkeletonFunctions->cryptoInit                = xmlSecSkeletonInit;
    gXmlSecSkeletonFunctions->cryptoShutdown            = xmlSecSkeletonShutdown;
    gXmlSecSkeletonFunctions->cryptoKeysMngrInit        = xmlSecSkeletonKeysMngrInit;

    /********************************************************************
     *
     * Key data ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_AES
    gXmlSecSkeletonFunctions->keyDataAesGetKlass        = xmlSecSkeletonKeyDataAesGetKlass;
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
    gXmlSecSkeletonFunctions->keyDataDesGetKlass        = xmlSecSkeletonKeyDataDesGetKlass;
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_DSA
    gXmlSecSkeletonFunctions->keyDataDsaGetKlass        = xmlSecSkeletonKeyDataDsaGetKlass;
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_ECDSA
    gXmlSecSkeletonFunctions->keyDataEcdsaGetKlass      = xmlSecSkeletonKeyDataEcdsaGetKlass;
#endif /* XMLSEC_NO_ECDSA */

#ifndef XMLSEC_NO_GOST
    gXmlSecSkeletonFunctions->keyDataGost2001GetKlass   = xmlSecSkeletonKeyDataGost2001GetKlass;
    gXmlSecSkeletonFunctions->keyDataGostR3410_2012GetKlass = xmlSecSkeletonKeyDataGostR3410_2012GetKlass;
#endif /* XMLSEC_NO_GOST */

#ifndef XMLSEC_NO_HMAC
    gXmlSecSkeletonFunctions->keyDataHmacGetKlass       = xmlSecSkeletonKeyDataHmacGetKlass;
#endif /* XMLSEC_NO_HMAC */

#ifndef XMLSEC_NO_RSA
    gXmlSecSkeletonFunctions->keyDataRsaGetKlass        = xmlSecSkeletonKeyDataRsaGetKlass;
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_X509
    gXmlSecSkeletonFunctions->keyDataX509GetKlass               = xmlSecSkeletonKeyDataX509GetKlass;
    gXmlSecSkeletonFunctions->keyDataRawX509CertGetKlass        = xmlSecSkeletonKeyDataRawX509CertGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Key data store ids
     *
     ********************************************************************/
#ifndef XMLSEC_NO_X509
    gXmlSecSkeletonFunctions->x509StoreGetKlass                 = xmlSecSkeletonX509StoreGetKlass;
#endif /* XMLSEC_NO_X509 */

    /********************************************************************
     *
     * Crypto transforms ids
     *
     ********************************************************************/

    /******************************* AES ********************************/
#ifndef XMLSEC_NO_AES
    gXmlSecSkeletonFunctions->transformAes128CbcGetKlass        = xmlSecSkeletonTransformAes128CbcGetKlass;
    gXmlSecSkeletonFunctions->transformAes192CbcGetKlass        = xmlSecSkeletonTransformAes192CbcGetKlass;
    gXmlSecSkeletonFunctions->transformAes256CbcGetKlass        = xmlSecSkeletonTransformAes256CbcGetKlass;
    gXmlSecSkeletonFunctions->transformKWAes128GetKlass         = xmlSecSkeletonTransformKWAes128GetKlass;
    gXmlSecSkeletonFunctions->transformKWAes192GetKlass         = xmlSecSkeletonTransformKWAes192GetKlass;
    gXmlSecSkeletonFunctions->transformKWAes256GetKlass         = xmlSecSkeletonTransformKWAes256GetKlass;
#endif /* XMLSEC_NO_AES */

    /******************************* DES ********************************/
#ifndef XMLSEC_NO_DES
    gXmlSecSkeletonFunctions->transformDes3CbcGetKlass          = xmlSecSkeletonTransformDes3CbcGetKlass;
    gXmlSecSkeletonFunctions->transformKWDes3GetKlass           = xmlSecSkeletonTransformKWDes3GetKlass;
#endif /* XMLSEC_NO_DES */

    /******************************* DSA ********************************/
#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1
    gXmlSecSkeletonFunctions->transformDsaSha1GetKlass          = xmlSecSkeletonTransformDsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecSkeletonFunctions->transformDsaSha256GetKlass        = xmlSecSkeletonTransformDsaSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#endif /* XMLSEC_NO_DSA */

    /******************************* ECDSA ********************************/
#ifndef XMLSEC_NO_ECDSA

#ifndef XMLSEC_NO_SHA1
    gXmlSecSkeletonFunctions->transformEcdsaSha1GetKlass        = xmlSecSkeletonTransformEcdsaSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA224
    gXmlSecSkeletonFunctions->transformEcdsaSha224GetKlass      = xmlSecSkeletonTransformEcdsaSha224GetKlass;
#endif /* XMLSEC_NO_SHA224 */

#ifndef XMLSEC_NO_SHA256
    gXmlSecSkeletonFunctions->transformEcdsaSha256GetKlass      = xmlSecSkeletonTransformEcdsaSha256GetKlass;
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    gXmlSecSkeletonFunctions->transformEcdsaSha384GetKlass      = xmlSecSkeletonTransformEcdsaSha384GetKlass;
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    gXmlSecSkeletonFunctions->transformEcdsaSha512GetKlass      = xmlSecSkeletonTransformEcdsaSha512GetKlass;
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_ECDSA */

    /******************************* GOST ********************************/
#ifndef XMLSEC_NO_GOST
    gXmlSecSkeletonFunctions->transformGost2001GostR3411_94GetKlass             = xmlSecSkeletonTransformGost2001GostR3411_94GetKlass;
    gXmlSecSkeletonFunctions->transformGostR3410_2012GostR3411_2012_256GetKlass = xmlSecSkeletonTransformGostR3410_2012GostR3411_2012_256GetKlass;
    gXmlSecSkeletonFunctions->transformGostR3410_2012GostR3411_2012_512GetKlass = xmlSecSkeletonTransformGostR3410_2012GostR3411_2012_512GetKlass;
#endif /* XMLSEC_GOST */

#ifndef XMLSEC_NO_GOST
    gXmlSecSkeletonFunctions->transformGostR3411_94GetKlass             = xmlSecSkeletonTransformGostR3411_94GetKlass;
    gXmlSecSkeletonFunctions->transformGostR3411_2012_256GetKlass       = xmlSecSkeletonTransformGostR3411_2012_256GetKlass;
    gXmlSecSkeletonFunctions->transformGostR3411_2012_512GetKlass       = xmlSecSkeletonTransformGostR3411_2012_512GetKlass;
#endif /* XMLSEC_NO_GOST */

    /******************************* HMAC ********************************/
#ifndef XMLSEC_NO_HMAC

#ifndef XMLSEC_NO_SHA1
    gXmlSecSkeletonFunctions->transformHmacSha1GetKlass         = xmlSecSkeletonTransformHmacSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecSkeletonFunctions->transformHmacRipemd160GetKlass    = xmlSecSkeletonTransformHmacRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_MD5
    gXmlSecSkeletonFunctions->transformHmacMd5GetKlass          = xmlSecSkeletonTransformHmacMd5GetKlass;
#endif /* XMLSEC_NO_MD5 */

#endif /* XMLSEC_NO_HMAC */

    /******************************* RIPEMD160 ********************************/
#ifndef XMLSEC_NO_RIPEMD160
    gXmlSecSkeletonFunctions->transformRipemd160GetKlass        = xmlSecSkeletonTransformRipemd160GetKlass;
#endif /* XMLSEC_NO_RIPEMD160 */

    /******************************* RSA ********************************/
#ifndef XMLSEC_NO_RSA
    gXmlSecSkeletonFunctions->transformRsaSha1GetKlass          = xmlSecSkeletonTransformRsaSha1GetKlass;
    gXmlSecSkeletonFunctions->transformRsaPkcs1GetKlass         = xmlSecSkeletonTransformRsaPkcs1GetKlass;
    gXmlSecSkeletonFunctions->transformRsaOaepGetKlass          = xmlSecSkeletonTransformRsaOaepGetKlass;
#endif /* XMLSEC_NO_RSA */

    /******************************* SHA1 ********************************/
#ifndef XMLSEC_NO_SHA1
    gXmlSecSkeletonFunctions->transformSha1GetKlass             = xmlSecSkeletonTransformSha1GetKlass;
#endif /* XMLSEC_NO_SHA1 */

    /********************************************************************
     *
     * High level routines form xmlsec command line utility
     *
     ********************************************************************/
    gXmlSecSkeletonFunctions->cryptoAppInit                     = xmlSecSkeletonAppInit;
    gXmlSecSkeletonFunctions->cryptoAppShutdown                 = xmlSecSkeletonAppShutdown;
    gXmlSecSkeletonFunctions->cryptoAppDefaultKeysMngrInit      = xmlSecSkeletonAppDefaultKeysMngrInit;
    gXmlSecSkeletonFunctions->cryptoAppDefaultKeysMngrAdoptKey  = xmlSecSkeletonAppDefaultKeysMngrAdoptKey;
    gXmlSecSkeletonFunctions->cryptoAppDefaultKeysMngrLoad      = xmlSecSkeletonAppDefaultKeysMngrLoad;
    gXmlSecSkeletonFunctions->cryptoAppDefaultKeysMngrSave      = xmlSecSkeletonAppDefaultKeysMngrSave;
#ifndef XMLSEC_NO_X509
    gXmlSecSkeletonFunctions->cryptoAppKeysMngrCertLoad         = xmlSecSkeletonAppKeysMngrCertLoad;
    gXmlSecSkeletonFunctions->cryptoAppKeysMngrCertLoadMemory   = xmlSecSkeletonAppKeysMngrCertLoadMemory;
    gXmlSecSkeletonFunctions->cryptoAppPkcs12Load               = xmlSecSkeletonAppPkcs12Load;
    gXmlSecSkeletonFunctions->cryptoAppPkcs12LoadMemory         = xmlSecSkeletonAppPkcs12LoadMemory;
    gXmlSecSkeletonFunctions->cryptoAppKeyCertLoad              = xmlSecSkeletonAppKeyCertLoad;
    gXmlSecSkeletonFunctions->cryptoAppKeyCertLoadMemory        = xmlSecSkeletonAppKeyCertLoadMemory;
#endif /* XMLSEC_NO_X509 */
    gXmlSecSkeletonFunctions->cryptoAppKeyLoad                  = xmlSecSkeletonAppKeyLoad;
    gXmlSecSkeletonFunctions->cryptoAppKeyLoadMemory            = xmlSecSkeletonAppKeyLoadMemory;
    gXmlSecSkeletonFunctions->cryptoAppDefaultPwdCallback       = (void*)xmlSecSkeletonAppGetDefaultPwdCallback();

    return(gXmlSecSkeletonFunctions);
}


/**
 * xmlSecSkeletonInit:
 *
 * XMLSec library specific crypto engine initialization.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonInit (void)  {
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
    if(xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms(xmlSecCryptoGetFunctions_skeleton()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(0);

    /* TODO: if necessary do, additional initialization here */
}

/**
 * xmlSecSkeletonShutdown:
 *
 * XMLSec library specific crypto engine shutdown.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonShutdown(void) {
    /* TODO: if necessary, do additional shutdown here */
    return(0);
}

/**
 * xmlSecSkeletonKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Adds Skeleton specific key data stores in keys manager.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    xmlSecAssert2(mngr != NULL, -1);

    /* TODO: add key data stores */
    return(0);
}



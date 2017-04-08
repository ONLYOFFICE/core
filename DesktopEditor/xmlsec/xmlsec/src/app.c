/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_CRYPTO_DYNAMIC_LOADING

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/app.h>
#include <xmlsec/list.h>
#include <xmlsec/keysdata.h>
#include <xmlsec/keys.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>
#include <xmlsec/private.h>
#include <xmlsec/errors.h>


/******************************************************************************
 *
 * Crypto Init/shutdown
 *
 *****************************************************************************/
/**
 * xmlSecCryptoInit:
 *
 * XMLSec library specific crypto engine initialization.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoInit(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoInit == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoInit",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoInit());
}

/**
 * xmlSecCryptoShutdown:
 *
 * XMLSec library specific crypto engine shutdown.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoShutdown(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoShutdown == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoShutdown",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoShutdown());
}

/**
 * xmlSecCryptoKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Adds crypto specific key data stores in keys manager.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoKeysMngrInit == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoKeysMngrInit",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoKeysMngrInit(mngr));
}

/******************************************************************************
 *
 * Key data ids
 *
 *****************************************************************************/
/**
 * xmlSecKeyDataAesGetKlass:
 *
 * The AES key data klass.
 *
 * Returns: AES key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the AES key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataAesGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataAesGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataAesId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataAesGetKlass());
}

/**
 * xmlSecKeyDataDesGetKlass:
 *
 * The DES key data klass.
 *
 * Returns: DES key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the DES key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataDesGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataDesGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataDesId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataDesGetKlass());
}

/**
 * xmlSecKeyDataDsaGetKlass:
 *
 * The DSA key data klass.
 *
 * Returns: DSA key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the DSA key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataDsaGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataDsaGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataDsaId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataDsaGetKlass());
}

/**
 * xmlSecKeyDataEcdsaGetKlass:
 *
 * The ECDSA key data klass.
 *
 * Returns: ECDSA key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the ECDSA key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataEcdsaGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataEcdsaGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataEcdsaId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataEcdsaGetKlass());
}

/**
 * xmlSecKeyDataGost2001GetKlass:
 *
 * The GOST2001 key data klass.
 *
 * Returns: GOST2001 key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the GOST2001 key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataGost2001GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataGost2001GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataGost2001Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataGost2001GetKlass());
}

/**
 * xmlSecKeyDataGostR3410_2012_256GetKlass:
 *
 * The GOST R 34.10-2012 256 bit key data klass.
 *
 * Returns: GOST R 34.10-2012 256 bit key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the GOST R 34.10-2012 key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataGostR3410_2012_256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataGostR3410_2012_256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataGostR3410_2012_256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataGostR3410_2012_256GetKlass());
}

/**
 * xmlSecKeyDataGostR3410_2012_512GetKlass:
 *
 * The GOST R 34.10-2012 512 bit key data klass.
 *
 * Returns: GOST R 34.10-2012 512 bit key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the GOST R 34.10-2012 key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataGostR3410_2012_512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataGostR3410_2012_512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataGostR3410_2012_512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataGostR3410_2012_512GetKlass());
}

/**
 * xmlSecKeyDataHmacGetKlass:
 *
 * The HMAC key data klass.
 *
 * Returns: HMAC key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the HMAC key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataHmacGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataHmacGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataHmacId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataHmacGetKlass());
}

/**
 * xmlSecKeyDataRsaGetKlass:
 *
 * The RSA key data klass.
 *
 * Returns: RSA key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the RSA key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataRsaGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataRsaGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataRsaId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataRsaGetKlass());
}

/**
 * xmlSecKeyDataX509GetKlass:
 *
 * The X509 key data klass.
 *
 * Returns: X509 key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the X509 key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataX509GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataX509GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataX509Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataX509GetKlass());
}

/**
 * xmlSecKeyDataRawX509CertGetKlass:
 *
 * The raw X509 cert key data klass.
 *
 * Returns: raw x509 cert key data klass or NULL if an error occurs
 * (xmlsec-crypto library is not loaded or the raw X509 cert key data
 * klass is not implemented).
 */
xmlSecKeyDataId
xmlSecKeyDataRawX509CertGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->keyDataRawX509CertGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "keyDataRawX509CertId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyDataIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->keyDataRawX509CertGetKlass());
}

/******************************************************************************
 *
 * Key data store ids
 *
 *****************************************************************************/
/**
 * xmlSecX509StoreGetKlass:
 *
 * The X509 certificates key data store klass.
 *
 * Returns: pointer to X509 certificates key data store klass or NULL if
 * an error occurs (xmlsec-crypto library is not loaded or the raw X509
 * cert key data klass is not implemented).
 */
xmlSecKeyDataStoreId
xmlSecX509StoreGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->x509StoreGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "x509StoreId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecKeyStoreIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->x509StoreGetKlass());
}

/******************************************************************************
 *
 * Crypto transforms ids
 *
 *****************************************************************************/
/**
 * xmlSecTransformAes128CbcGetKlass:
 *
 * AES 128 CBC encryption transform klass.
 *
 * Returns: pointer to AES 128 CBC encryption transform or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformAes128CbcGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformAes128CbcGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformAes128CbcId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformAes128CbcGetKlass());
}

/**
 * xmlSecTransformAes192CbcGetKlass:
 *
 * AES 192 CBC encryption transform klass.
 *
 * Returns: pointer to AES 192 CBC encryption transform or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformAes192CbcGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformAes192CbcGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformAes192CbcId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformAes192CbcGetKlass());
}

/**
 * xmlSecTransformAes256CbcGetKlass:
 *
 * AES 256 CBC encryption transform klass.
 *
 * Returns: pointer to AES 256 CBC encryption transform or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformAes256CbcGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformAes256CbcGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformAes256CbcId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformAes256CbcGetKlass());
}

/**
 * xmlSecTransformKWAes128GetKlass:
 *
 * The AES-128 kew wrapper transform klass.
 *
 * Returns: AES-128 kew wrapper transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformKWAes128GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformKWAes128GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformKWAes128Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformKWAes128GetKlass());
}

/**
 * xmlSecTransformKWAes192GetKlass:
 *
 * The AES-192 kew wrapper transform klass.
 *
 * Returns: AES-192 kew wrapper transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformKWAes192GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformKWAes192GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformKWAes192Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformKWAes192GetKlass());
}

/**
 * xmlSecTransformKWAes256GetKlass:
 *
 * The AES-256 kew wrapper transform klass.
 *
 * Returns: AES-256 kew wrapper transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformKWAes256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformKWAes256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformKWAes256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformKWAes256GetKlass());
}

/**
 * xmlSecTransformDes3CbcGetKlass:
 *
 * Triple DES CBC encryption transform klass.
 *
 * Returns: pointer to Triple DES encryption transform or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformDes3CbcGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformDes3CbcGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformDes3CbcId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformDes3CbcGetKlass());
}

/**
 * xmlSecTransformKWDes3GetKlass:
 *
 * The Triple DES key wrapper transform klass.
 *
 * Returns: Triple DES key wrapper transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformKWDes3GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformKWDes3GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformKWDes3Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformKWDes3GetKlass());
}

/**
 * xmlSecTransformDsaSha1GetKlass:
 *
 * The DSA-SHA1 signature transform klass.
 *
 * Returns: DSA-SHA1 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformDsaSha1GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformDsaSha1GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformDsaSha1Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformDsaSha1GetKlass());
}

/**
 * xmlSecTransformDsaSha256GetKlass:
 *
 * The DSA-SHA256 signature transform klass.
 *
 * Returns: DSA-SHA256 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformDsaSha256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformDsaSha256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformDsaSha256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformDsaSha256GetKlass());
}

/**
 * xmlSecTransformEcdsaSha1GetKlass:
 *
 * The ECDSA-SHA1 signature transform klass.
 *
 * Returns: ECDSA-SHA1 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformEcdsaSha1GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformEcdsaSha1GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformEcdsaSha1Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformEcdsaSha1GetKlass());
}

/**
 * xmlSecTransformEcdsaSha224GetKlass:
 *
 * The ECDSA-SHA224 signature transform klass.
 *
 * Returns: ECDSA-SHA224 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformEcdsaSha224GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformEcdsaSha224GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformEcdsaSha224Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformEcdsaSha224GetKlass());
}

/**
 * xmlSecTransformEcdsaSha256GetKlass:
 *
 * The ECDSA-SHA256 signature transform klass.
 *
 * Returns: ECDSA-SHA256 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformEcdsaSha256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformEcdsaSha256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformEcdsaSha256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformEcdsaSha256GetKlass());
}

/**
 * xmlSecTransformEcdsaSha384GetKlass:
 *
 * The ECDSA-SHA384 signature transform klass.
 *
 * Returns: ECDSA-SHA384 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformEcdsaSha384GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformEcdsaSha384GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformEcdsaSha384Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformEcdsaSha384GetKlass());
}

/**
 * xmlSecTransformEcdsaSha512GetKlass:
 *
 * The ECDSA-SHA512 signature transform klass.
 *
 * Returns: ECDSA-SHA512 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformEcdsaSha512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformEcdsaSha512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformEcdsaSha512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformEcdsaSha512GetKlass());
}

/**
 * xmlSecTransformGost2001GostR3411_94GetKlass:
 *
 * The GOST2001-GOSTR3411_94 signature transform klass.
 *
 * Returns: GOST2001-GOSTR3411_94 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformGost2001GostR3411_94GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformGost2001GostR3411_94GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformGost2001GostR3411_94Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformGost2001GostR3411_94GetKlass());
}

/**
 * xmlSecTransformGostR3410_2012GostR3411_2012_256GetKlass:
 *
 * The GOST R 34.10-2012 - GOST R 34.11-2012 256 bit signature transform klass.
 *
 * Returns: GOST R 34.10-2012 - GOST R 34.11-2012 256 bit signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformGostR3410_2012GostR3411_2012_256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformGostR3410_2012GostR3411_2012_256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformGostR3410_2012GostR3411_2012_256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformGostR3410_2012GostR3411_2012_256GetKlass());
}

/**
 * xmlSecTransformGostR3410_2012GostR3411_2012_512GetKlass:
 *
 * The GOST R 34.10-2012 - GOST R 34.11-2012 512 bit signature transform klass.
 *
 * Returns: GOST R 34.10-2012 - GOST R 34.11-2012 512 bit signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformGostR3410_2012GostR3411_2012_512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformGostR3410_2012GostR3411_2012_512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformGostR3410_2012GostR3411_2012_512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformGostR3410_2012GostR3411_2012_512GetKlass());
}

/**
 * xmlSecTransformHmacMd5GetKlass:
 *
 * The HMAC-MD5 transform klass.
 *
 * Returns: the HMAC-MD5 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacMd5GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacMd5GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacMd5Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacMd5GetKlass());
}

/**
 * xmlSecTransformHmacRipemd160GetKlass:
 *
 * The HMAC-RIPEMD160 transform klass.
 *
 * Returns: the HMAC-RIPEMD160 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacRipemd160GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacRipemd160GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacRipemd160Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacRipemd160GetKlass());
}

/**
 * xmlSecTransformHmacSha1GetKlass:
 *
 * The HMAC-SHA1 transform klass.
 *
 * Returns: the HMAC-SHA1 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacSha1GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacSha1GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacSha1Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacSha1GetKlass());
}

/**
 * xmlSecTransformHmacSha224GetKlass:
 *
 * The HMAC-SHA224 transform klass.
 *
 * Returns: the HMAC-SHA224 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacSha224GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacSha224GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacSha224Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacSha224GetKlass());
}

/**
 * xmlSecTransformHmacSha256GetKlass:
 *
 * The HMAC-SHA256 transform klass.
 *
 * Returns: the HMAC-SHA256 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacSha256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacSha256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacSha256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacSha256GetKlass());
}

/**
 * xmlSecTransformHmacSha384GetKlass:
 *
 * The HMAC-SHA384 transform klass.
 *
 * Returns: the HMAC-SHA384 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacSha384GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacSha384GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacSha384Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacSha384GetKlass());
}

/**
 * xmlSecTransformHmacSha512GetKlass:
 *
 * The HMAC-SHA512 transform klass.
 *
 * Returns: the HMAC-SHA512 transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformHmacSha512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformHmacSha512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformHmacSha512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformHmacSha512GetKlass());
}

/**
 * xmlSecTransformMd5GetKlass:
 *
 * MD5 digest transform klass.
 *
 * Returns: pointer to MD5 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformMd5GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformMd5GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformMd5Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformMd5GetKlass());
}

/**
 * xmlSecTransformRipemd160GetKlass:
 *
 * RIPEMD-160 digest transform klass.
 *
 * Returns: pointer to RIPEMD-160 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRipemd160GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRipemd160GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRipemd160Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRipemd160GetKlass());
}

/**
 * xmlSecTransformRsaMd5GetKlass:
 *
 * The RSA-MD5 signature transform klass.
 *
 * Returns: RSA-MD5 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaMd5GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaMd5GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaMd5Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaMd5GetKlass());
}

/**
 * xmlSecTransformRsaRipemd160GetKlass:
 *
 * The RSA-RIPEMD160 signature transform klass.
 *
 * Returns: RSA-RIPEMD160 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaRipemd160GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaRipemd160GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaRipemd160Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaRipemd160GetKlass());
}

/**
 * xmlSecTransformRsaSha1GetKlass:
 *
 * The RSA-SHA1 signature transform klass.
 *
 * Returns: RSA-SHA1 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaSha1GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaSha1GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaSha1Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaSha1GetKlass());
}

/**
 * xmlSecTransformRsaSha224GetKlass:
 *
 * The RSA-SHA224 signature transform klass.
 *
 * Returns: RSA-SHA224 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaSha224GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaSha224GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaSha224Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaSha224GetKlass());
}

/**
 * xmlSecTransformRsaSha256GetKlass:
 *
 * The RSA-SHA256 signature transform klass.
 *
 * Returns: RSA-SHA256 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaSha256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaSha256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaSha256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaSha256GetKlass());
}

/**
 * xmlSecTransformRsaSha384GetKlass:
 *
 * The RSA-SHA384 signature transform klass.
 *
 * Returns: RSA-SHA384 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaSha384GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaSha384GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaSha384Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaSha384GetKlass());
}

/**
 * xmlSecTransformRsaSha512GetKlass:
 *
 * The RSA-SHA512 signature transform klass.
 *
 * Returns: RSA-SHA512 signature transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaSha512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaSha512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaSha512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaSha512GetKlass());
}

/**
 * xmlSecTransformRsaPkcs1GetKlass:
 *
 * The RSA-PKCS1 key transport transform klass.
 *
 * Returns: RSA-PKCS1 key transport transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaPkcs1GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaPkcs1GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaPkcs1Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaPkcs1GetKlass());
}

/**
 * xmlSecTransformRsaOaepGetKlass:
 *
 * The RSA-OAEP key transport transform klass.
 *
 * Returns: RSA-OAEP key transport transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformRsaOaepGetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformRsaOaepGetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformRsaOaepId",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformRsaOaepGetKlass());
}

/**
 * xmlSecTransformGostR3411_94GetKlass:
 *
 * GOSTR3411_94 digest transform klass.
 *
 * Returns: pointer to GOSTR3411_94 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformGostR3411_94GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformGostR3411_94GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformGostR3411_94Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformGostR3411_94GetKlass());
}

/**
 * xmlSecTransformGostR3411_2012_256GetKlass:
 *
 * GOST R 34.11-2012 256 bit digest transform klass.
 *
 * Returns: pointer to GOST R 34.11-2012 256 bit digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */

xmlSecTransformId
xmlSecTransformGostR3411_2012_256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformGostR3411_2012_256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformGostR3411_2012_256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformGostR3411_2012_256GetKlass());
}

/**
 * xmlSecTransformGostR3411_2012_512GetKlass:
 *
 * GOST R 34.11-2012 512 bit digest transform klass.
 *
 * Returns: pointer to GOST R 34.11-2012 512 bit digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformGostR3411_2012_512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformGostR3411_2012_512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformGostR3411_2012_512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformGostR3411_2012_512GetKlass());
}
/**
 * xmlSecTransformSha1GetKlass:
 *
 * SHA-1 digest transform klass.
 *
 * Returns: pointer to SHA-1 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformSha1GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformSha1GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformSha1Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformSha1GetKlass());
}

/**
 * xmlSecTransformSha224GetKlass:
 *
 * SHA224 digest transform klass.
 *
 * Returns: pointer to SHA224 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformSha224GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformSha224GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformSha224Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformSha224GetKlass());
}

/**
 * xmlSecTransformSha256GetKlass:
 *
 * SHA256 digest transform klass.
 *
 * Returns: pointer to SHA256 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformSha256GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformSha256GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformSha256Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformSha256GetKlass());
}

/**
 * xmlSecTransformSha384GetKlass:
 *
 * SHA384 digest transform klass.
 *
 * Returns: pointer to SHA384 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformSha384GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformSha384GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformSha384Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformSha384GetKlass());
}

/**
 * xmlSecTransformSha512GetKlass:
 *
 * SHA512 digest transform klass.
 *
 * Returns: pointer to SHA512 digest transform klass or NULL if an error
 * occurs (the xmlsec-crypto library is not loaded or this transform is not
 * implemented).
 */
xmlSecTransformId
xmlSecTransformSha512GetKlass(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->transformSha512GetKlass == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "transformSha512Id",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(xmlSecTransformIdUnknown);
    }

    return(xmlSecCryptoDLGetFunctions()->transformSha512GetKlass());
}

/******************************************************************************
 *
 * High level routines form xmlsec command line utility
 *
 *****************************************************************************/
/**
 * xmlSecCryptoAppInit:
 * @config:             the path to crypto library configuration.
 *
 * General crypto engine initialization. This function is used
 * by XMLSec command line utility and called before
 * @xmlSecInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppInit(const char* config) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppInit == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppInit",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppInit(config));
}


/**
 * xmlSecCryptoAppShutdown:
 *
 * General crypto engine shutdown. This function is used
 * by XMLSec command line utility and called after
 * @xmlSecShutdown function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppShutdown(void) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppShutdown == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppShutdown",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppShutdown());
}

/**
 * xmlSecCryptoAppDefaultKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Initializes @mngr with simple keys store #xmlSecSimpleKeysStoreId
 * and a default crypto key data stores.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppDefaultKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrInit == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppDefaultKeysMngrInit",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrInit(mngr));
}

/**
 * xmlSecCryptoAppDefaultKeysMngrAdoptKey:
 * @mngr:               the pointer to keys manager.
 * @key:                the pointer to key.
 *
 * Adds @key to the keys manager @mngr created with #xmlSecCryptoAppDefaultKeysMngrInit
 * function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppDefaultKeysMngrAdoptKey(xmlSecKeysMngrPtr mngr, xmlSecKeyPtr key) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrAdoptKey == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppDefaultKeysMngrAdoptKey",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrAdoptKey(mngr, key));
}

/**
 * xmlSecCryptoAppDefaultKeysMngrLoad:
 * @mngr:               the pointer to keys manager.
 * @uri:                the uri.
 *
 * Loads XML keys file from @uri to the keys manager @mngr created
 * with #xmlSecCryptoAppDefaultKeysMngrInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppDefaultKeysMngrLoad(xmlSecKeysMngrPtr mngr, const char* uri) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrLoad == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppDefaultKeysMngrLoad",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrLoad(mngr, uri));
}

/**
 * xmlSecCryptoAppDefaultKeysMngrSave:
 * @mngr:               the pointer to keys manager.
 * @filename:           the destination filename.
 * @type:               the type of keys to save (public/private/symmetric).
 *
 * Saves keys from @mngr to  XML keys file.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppDefaultKeysMngrSave(xmlSecKeysMngrPtr mngr, const char* filename,
                                   xmlSecKeyDataType type) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrSave == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppDefaultKeysMngrSave",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppDefaultKeysMngrSave(mngr, filename, type));
}

/**
 * xmlSecCryptoAppKeysMngrCertLoad:
 * @mngr:               the keys manager.
 * @filename:           the certificate file.
 * @format:             the certificate file format.
 * @type:               the flag that indicates is the certificate in @filename
 *                      trusted or not.
 *
 * Reads cert from @filename and adds to the list of trusted or known
 * untrusted certs in @store.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppKeysMngrCertLoad(xmlSecKeysMngrPtr mngr, const char *filename,
                                xmlSecKeyDataFormat format, xmlSecKeyDataType type) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppKeysMngrCertLoad == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppKeysMngrCertLoad",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppKeysMngrCertLoad(mngr, filename, format, type));
}

/**
 * xmlSecCryptoAppKeysMngrCertLoadMemory:
 * @mngr:               the keys manager.
 * @data:               the certificate binary data.
 * @dataSize:           the certificate binary data size.
 * @format:             the certificate file format.
 * @type:               the flag that indicates is the certificate trusted or not.
 *
 * Reads cert from binary buffer @data and adds to the list of trusted or known
 * untrusted certs in @store.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppKeysMngrCertLoadMemory(xmlSecKeysMngrPtr mngr, const xmlSecByte* data,
                                    xmlSecSize dataSize, xmlSecKeyDataFormat format,
                                    xmlSecKeyDataType type) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppKeysMngrCertLoadMemory == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppKeysMngrCertLoadMemory",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppKeysMngrCertLoadMemory(mngr, data, dataSize, format, type));
}

/**
 * xmlSecCryptoAppKeyLoad:
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
xmlSecCryptoAppKeyLoad(const char *filename, xmlSecKeyDataFormat format,
                       const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppKeyLoad == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppKeyLoad",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppKeyLoad(filename, format, pwd, pwdCallback, pwdCallbackCtx));
}

/**
 * xmlSecCryptoAppKeyLoadMemory:
 * @data:               the binary key data.
 * @dataSize:           the size of binary key.
 * @format:             the key file format.
 * @pwd:                the key file password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from the memory buffer.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecCryptoAppKeyLoadMemory(const xmlSecByte* data, xmlSecSize dataSize, xmlSecKeyDataFormat format,
                       const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppKeyLoadMemory == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppKeyLoadMemory",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppKeyLoadMemory(data, dataSize, format, pwd, pwdCallback, pwdCallbackCtx));
}

/**
 * xmlSecCryptoAppPkcs12Load:
 * @filename:           the PKCS12 key filename.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 file.
 * For uniformity, call xmlSecCryptoAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecCryptoAppPkcs12Load(const char* filename, const char* pwd, void* pwdCallback,
                          void* pwdCallbackCtx) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppPkcs12Load == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppPkcs12Load",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppPkcs12Load(filename, pwd, pwdCallback, pwdCallbackCtx));
}

/**
 * xmlSecCryptoAppPkcs12LoadMemory:
 * @data:               the PKCS12 binary data.
 * @dataSize:           the PKCS12 binary data size.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 data in memory buffer.
 * For uniformity, call xmlSecCryptoAppKeyLoadMemory instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecCryptoAppPkcs12LoadMemory(const xmlSecByte* data, xmlSecSize dataSize,
                           const char *pwd, void* pwdCallback,
                           void* pwdCallbackCtx) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppPkcs12LoadMemory == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppPkcs12LoadMemory",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppPkcs12LoadMemory(data, dataSize, pwd, pwdCallback, pwdCallbackCtx));
}

/**
 * xmlSecCryptoAppKeyCertLoad:
 * @key:                the pointer to key.
 * @filename:           the certificate filename.
 * @format:             the certificate file format.
 *
 * Reads the certificate from $@filename and adds it to key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppKeyCertLoad(xmlSecKeyPtr key, const char* filename, xmlSecKeyDataFormat format) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppKeyCertLoad == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppKeyCertLoad",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppKeyCertLoad(key, filename, format));
}

/**
 * xmlSecCryptoAppKeyCertLoadMemory:
 * @key:                the pointer to key.
 * @data:               the certificate binary data.
 * @dataSize:           the certificate binary data size.
 * @format:             the certificate file format.
 *
 * Reads the certificate from memory buffer and adds it to key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecCryptoAppKeyCertLoadMemory(xmlSecKeyPtr key, const xmlSecByte* data, xmlSecSize dataSize,
                                xmlSecKeyDataFormat format) {
    if((xmlSecCryptoDLGetFunctions() == NULL) || (xmlSecCryptoDLGetFunctions()->cryptoAppKeyCertLoadMemory == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cryptoAppKeyCertLoadMemory",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppKeyCertLoadMemory(key, data, dataSize, format));
}

/**
 * xmlSecCryptoAppGetDefaultPwdCallback:
 *
 * Gets default password callback.
 *
 * Returns: default password callback.
 */
void*
xmlSecCryptoAppGetDefaultPwdCallback(void) {
    if(xmlSecCryptoDLGetFunctions() == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecCryptoDLGetFunctions()->cryptoAppDefaultPwdCallback);
}

#endif /* XMLSEC_NO_CRYPTO_DYNAMIC_LOADING */


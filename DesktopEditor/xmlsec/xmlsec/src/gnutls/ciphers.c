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

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gnutls/crypto.h>

/**************************************************************************
 *
 * We use xmlsec-gcrypt for all the basic crypto ops
 *
 *****************************************************************************/
#include <xmlsec/gcrypt/crypto.h>




#ifndef XMLSEC_NO_AES

/**
 * xmlSecGnuTLSTransformAes128CbcGetKlass:
 *
 * AES 128 CBC encryption transform klass.
 *
 * Returns: pointer to AES 128 CBC encryption transform.
 */
xmlSecTransformId
xmlSecGnuTLSTransformAes128CbcGetKlass(void) {
    return (xmlSecGCryptTransformAes128CbcGetKlass());
}

/**
 * xmlSecGnuTLSTransformAes192CbcGetKlass:
 *
 * AES 192 CBC encryption transform klass.
 *
 * Returns: pointer to AES 192 CBC encryption transform.
 */
xmlSecTransformId
xmlSecGnuTLSTransformAes192CbcGetKlass(void) {
    return (xmlSecGCryptTransformAes192CbcGetKlass());
}

/**
 * xmlSecGnuTLSTransformAes256CbcGetKlass:
 *
 * AES 256 CBC encryption transform klass.
 *
 * Returns: pointer to AES 256 CBC encryption transform.
 */
xmlSecTransformId
xmlSecGnuTLSTransformAes256CbcGetKlass(void) {
    return (xmlSecGCryptTransformAes256CbcGetKlass());
}
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
/**
 * xmlSecGnuTLSTransformDes3CbcGetKlass:
 *
 * Triple DES CBC encryption transform klass.
 *
 * Returns: pointer to Triple DES encryption transform.
 */
xmlSecTransformId
xmlSecGnuTLSTransformDes3CbcGetKlass(void) {
    return (xmlSecGCryptTransformDes3CbcGetKlass());
}
#endif /* XMLSEC_NO_DES */


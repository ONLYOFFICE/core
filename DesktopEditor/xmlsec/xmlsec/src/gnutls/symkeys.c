/**
 *
 * XMLSec library
 *
 * DES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
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
 * xmlSecGnuTLSKeyDataAesGetKlass:
 *
 * The AES key data klass.
 *
 * Returns: AES key data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataAesGetKlass(void) {
    return (xmlSecGCryptKeyDataAesGetKlass());
}

/**
 * xmlSecGnuTLSKeyDataAesSet:
 * @data:               the pointer to AES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of AES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGnuTLSKeyDataAesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    return (xmlSecGCryptKeyDataAesSet(data, buf, bufSize));
}
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
/**
 * xmlSecGnuTLSKeyDataDesGetKlass:
 *
 * The DES key data klass.
 *
 * Returns: DES key data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataDesGetKlass(void) {
    return (xmlSecGCryptKeyDataDesGetKlass());
}

/**
 * xmlSecGnuTLSKeyDataDesSet:
 * @data:               the pointer to DES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of DES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGnuTLSKeyDataDesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    return (xmlSecGCryptKeyDataDesSet(data, buf, bufSize));
}

#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_HMAC

/**
 * xmlSecGnuTLSKeyDataHmacGetKlass:
 *
 * The HMAC key data klass.
 *
 * Returns: HMAC key data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataHmacGetKlass(void) {
    return (xmlSecGCryptKeyDataHmacGetKlass());
}

/**
 * xmlSecGnuTLSKeyDataHmacSet:
 * @data:               the pointer to HMAC key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of HMAC key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGnuTLSKeyDataHmacSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    return (xmlSecGCryptKeyDataHmacSet(data, buf, bufSize));
}

#endif /* XMLSEC_NO_HMAC */


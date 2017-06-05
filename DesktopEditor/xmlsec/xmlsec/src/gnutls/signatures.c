/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
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
#include <gcrypt.h>


#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1

/**
 * xmlSecGnuTLSTransformDsaSha1GetKlass:
 *
 * The DSA-SHA1 signature transform klass.
 *
 * Returns: DSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformDsaSha1GetKlass(void) {
    return (xmlSecGCryptTransformDsaSha1GetKlass());
}

#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5

/**
 * xmlSecGnuTLSTransformRsaMd5GetKlass:
 *
 * The RSA-MD5 signature transform klass.
 *
 * Returns: RSA-MD5 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRsaMd5GetKlass(void) {
    return (xmlSecGCryptTransformRsaMd5GetKlass());
}

#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160

/**
 * xmlSecGnuTLSTransformRsaRipemd160GetKlass:
 *
 * The RSA-RIPEMD160 signature transform klass.
 *
 * Returns: RSA-RIPEMD160 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRsaRipemd160GetKlass(void) {
    return (xmlSecGCryptTransformRsaRipemd160GetKlass());
}

#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGnuTLSTransformRsaSha1GetKlass:
 *
 * The RSA-SHA1 signature transform klass.
 *
 * Returns: RSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRsaSha1GetKlass(void) {
    return (xmlSecGCryptTransformRsaSha1GetKlass());
}

#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256

/**
 * xmlSecGnuTLSTransformRsaSha256GetKlass:
 *
 * The RSA-SHA256 signature transform klass.
 *
 * Returns: RSA-SHA256 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRsaSha256GetKlass(void) {
    return (xmlSecGCryptTransformRsaSha256GetKlass());
}

#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384

/**
 * xmlSecGnuTLSTransformRsaSha384GetKlass:
 *
 * The RSA-SHA384 signature transform klass.
 *
 * Returns: RSA-SHA384 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRsaSha384GetKlass(void) {
      return (xmlSecGCryptTransformRsaSha384GetKlass());
}

#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGnuTLSTransformRsaSha512GetKlass:
 *
 * The RSA-SHA512 signature transform klass.
 *
 * Returns: RSA-SHA512 signature transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRsaSha512GetKlass(void) {
    return (xmlSecGCryptTransformRsaSha512GetKlass());
}

#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */




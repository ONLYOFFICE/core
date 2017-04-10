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

#include <xmlsec/gnutls/app.h>
#include <xmlsec/gnutls/crypto.h>

/**************************************************************************
 *
 * We use xmlsec-gcrypt for all the basic crypto ops
 *
 *****************************************************************************/
#include <xmlsec/gcrypt/crypto.h>

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGnuTLSTransformSha1GetKlass:
 *
 * SHA-1 digest transform klass.
 *
 * Returns: pointer to SHA-1 digest transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformSha1GetKlass(void) {
    return (xmlSecGCryptTransformSha1GetKlass());
}
#endif /* XMLSEC_NO_SHA1 */


#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGnuTLSTransformSha256GetKlass:
 *
 * SHA256 digest transform klass.
 *
 * Returns: pointer to SHA256 digest transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformSha256GetKlass(void) {
    return (xmlSecGCryptTransformSha256GetKlass());
}
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGnuTLSTransformSha384GetKlass:
 *
 * SHA384 digest transform klass.
 *
 * Returns: pointer to SHA384 digest transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformSha384GetKlass(void) {
      return (xmlSecGCryptTransformSha384GetKlass());
}
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGnuTLSTransformSha512GetKlass:
 *
 * SHA512 digest transform klass.
 *
 * Returns: pointer to SHA512 digest transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformSha512GetKlass(void) {
        return (xmlSecGCryptTransformSha512GetKlass());
}
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_MD5

/**
 * xmlSecGnuTLSTransformMd5GetKlass:
 *
 * MD5 digest transform klass.
 *
 * Returns: pointer to MD5 digest transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformMd5GetKlass(void) {
    return (xmlSecGCryptTransformMd5GetKlass());
}
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGnuTLSTransformRipemd160GetKlass:
 *
 * RIPEMD160 digest transform klass.
 *
 * Returns: pointer to RIPEMD160 digest transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformRipemd160GetKlass(void) {
    return (xmlSecGCryptTransformRipemd160GetKlass());
}
#endif /* XMLSEC_NO_RIPEMD160 */

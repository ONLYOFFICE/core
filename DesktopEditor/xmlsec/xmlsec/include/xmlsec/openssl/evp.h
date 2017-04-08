/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_OPENSSL_EVP_H__
#define __XMLSEC_OPENSSL_EVP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <openssl/evp.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>

#include <xmlsec/openssl/crypto.h>


XMLSEC_CRYPTO_EXPORT int                xmlSecOpenSSLEvpKeyDataAdoptEvp (xmlSecKeyDataPtr data,
                                                                         EVP_PKEY* pKey);
XMLSEC_CRYPTO_EXPORT EVP_PKEY*          xmlSecOpenSSLEvpKeyDataGetEvp   (xmlSecKeyDataPtr data);

/******************************************************************************
 *
 * EVP helper functions
 *
 *****************************************************************************/
XMLSEC_CRYPTO_EXPORT EVP_PKEY*          xmlSecOpenSSLEvpKeyDup          (EVP_PKEY* pKey);
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataPtr   xmlSecOpenSSLEvpKeyAdopt        (EVP_PKEY *pKey);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_OPENSSL_EVP_H__ */



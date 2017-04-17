/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Crypto engine selection.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_CRYPTO_H__
#define __XMLSEC_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <xmlsec/xmlsec.h>

/* include nothing if we compile xmlsec library itself */
#ifndef IN_XMLSEC
#ifndef IN_XMLSEC_CRYPTO

#if defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
#error Dynamic loading for xmlsec-crypto libraries is disabled during library compilation
#endif /* defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */

#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
#include <xmlsec/app.h>
#else /* XMLSEC_CRYPTO_DYNAMIC_LOADING */
#ifdef XMLSEC_CRYPTO_OPENSSL
#include <xmlsec/openssl/app.h>
#include <xmlsec/openssl/crypto.h>
#include <xmlsec/openssl/x509.h>
#include <xmlsec/openssl/symbols.h>
#else /* XMLSEC_CRYPTO_OPENSSL */
#ifdef XMLSEC_CRYPTO_MSCRYPTO
#include <xmlsec/mscrypto/app.h>
#include <xmlsec/mscrypto/crypto.h>
#include <xmlsec/mscrypto/x509.h>
#include <xmlsec/mscrypto/symbols.h>
#else /* XMLSEC_CRYPTO_MSCRYPTO */
#ifdef XMLSEC_CRYPTO_NSS
#include <xmlsec/nss/app.h>
#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/x509.h>
#include <xmlsec/nss/symbols.h>
#else /* XMLSEC_CRYPTO_NSS */
#ifdef XMLSEC_CRYPTO_GNUTLS
#include <xmlsec/gnutls/app.h>
#include <xmlsec/gnutls/crypto.h>
#include <xmlsec/gnutls/symbols.h>
#else /* XMLSEC_CRYPTO_GNUTLS */
#ifdef XMLSEC_CRYPTO_GCRYPT
#include <xmlsec/gcrypt/app.h>
#include <xmlsec/gcrypt/crypto.h>
#include <xmlsec/gcrypt/symbols.h>
#else /* XMLSEC_CRYPTO_GCRYPT */
#error No crypto library defined
#endif /* XMLSEC_CRYPTO_GCRYPT */
#endif /* XMLSEC_CRYPTO_GNUTLS */
#endif /* XMLSEC_CRYPTO_NSS */
#endif /* XMLSEC_CRYPTO_MSCRYPTO */
#endif /* XMLSEC_CRYPTO_OPENSSL */
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

#endif /* IN_XMLSEC_CRYPTO */
#endif /* IN_XMLSEC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_CRYPTO_H__ */


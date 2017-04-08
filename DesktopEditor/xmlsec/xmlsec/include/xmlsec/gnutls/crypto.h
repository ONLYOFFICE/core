/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GNUTLS_CRYPTO_H__
#define __XMLSEC_GNUTLS_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/dl.h>

XMLSEC_CRYPTO_EXPORT xmlSecCryptoDLFunctionsPtr xmlSecCryptoGetFunctions_gnutls(void);

/********************************************************************
 *
 * Init shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSInit                (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSShutdown            (void);

XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeysMngrInit        (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSGenerateRandom      (xmlSecBufferPtr buffer,
                                                                         xmlSecSize size);


/********************************************************************
 *
 * AES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_AES
/**
 * xmlSecGnuTLSKeyDataAesId:
 *
 * The AES key data klass.
 */
#define xmlSecGnuTLSKeyDataAesId \
        xmlSecGnuTLSKeyDataAesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataAesGetKlass  (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataAesSet       (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);
/**
 * xmlSecGnuTLSTransformAes128CbcId:
 *
 * The AES128 CBC cipher transform klass.
 */
#define xmlSecGnuTLSTransformAes128CbcId \
        xmlSecGnuTLSTransformAes128CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGnuTLSTransformAes128CbcGetKlass(void);

/**
 * xmlSecGnuTLSTransformAes192CbcId:
 *
 * The AES192 CBC cipher transform klass.
 */
#define xmlSecGnuTLSTransformAes192CbcId \
        xmlSecGnuTLSTransformAes192CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGnuTLSTransformAes192CbcGetKlass(void);

/**
 * xmlSecGnuTLSTransformAes256CbcId:
 *
 * The AES256 CBC cipher transform klass.
 */
#define xmlSecGnuTLSTransformAes256CbcId \
        xmlSecGnuTLSTransformAes256CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGnuTLSTransformAes256CbcGetKlass(void);

/**
 * xmlSecGnuTLSTransformKWAes128Id:
 *
 * The AES 128 key wrap transform klass.
 */
#define xmlSecGnuTLSTransformKWAes128Id \
        xmlSecGnuTLSTransformKWAes128GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGnuTLSTransformKWAes128GetKlass(void);

/**
 * xmlSecGnuTLSTransformKWAes192Id:
 *
 * The AES 192 key wrap transform klass.
 */
#define xmlSecGnuTLSTransformKWAes192Id \
        xmlSecGnuTLSTransformKWAes192GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGnuTLSTransformKWAes192GetKlass(void);

/**
 * xmlSecGnuTLSTransformKWAes256Id:
 *
 * The AES 256 key wrap transform klass.
 */
#define xmlSecGnuTLSTransformKWAes256Id \
        xmlSecGnuTLSTransformKWAes256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGnuTLSTransformKWAes256GetKlass(void);


#endif /* XMLSEC_NO_AES */

/********************************************************************
 *
 * DES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DES
/**
 * xmlSecGnuTLSKeyDataDesId:
 *
 * The DES key data klass.
 */
#define xmlSecGnuTLSKeyDataDesId \
        xmlSecGnuTLSKeyDataDesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataDesGetKlass  (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataDesSet       (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);

/**
 * xmlSecGnuTLSTransformDes3CbcId:
 *
 * The DES3 CBC cipher transform klass.
 */
#define xmlSecGnuTLSTransformDes3CbcId \
        xmlSecGnuTLSTransformDes3CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformDes3CbcGetKlass(void);

/**
 * xmlSecGnuTLSTransformKWDes3Id:
 *
 * The DES3 KW transform klass.
 */
#define xmlSecGnuTLSTransformKWDes3Id \
        xmlSecGnuTLSTransformKWDes3GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformKWDes3GetKlass(void);

#endif /* XMLSEC_NO_DES */

/********************************************************************
 *
 * DSA transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DSA

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

/**
 * xmlSecGnuTLSKeyDataDsaId:
 *
 * The DSA key klass.
 */
#define xmlSecGnuTLSKeyDataDsaId \
        xmlSecGnuTLSKeyDataDsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataDsaGetKlass          (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataDsaAdoptPrivateKey   (xmlSecKeyDataPtr data,
                                                                                 gnutls_x509_privkey_t dsa_key);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataDsaAdoptPublicKey    (xmlSecKeyDataPtr data,
                                                                                 gnutls_datum_t * p,
                                                                                 gnutls_datum_t * q,
                                                                                 gnutls_datum_t * g,
                                                                                 gnutls_datum_t * y);

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGnuTLSTransformDsaSha1Id:
 *
 * The DSA SHA1 signature transform klass.
 */
#define xmlSecGnuTLSTransformDsaSha1Id \
        xmlSecGnuTLSTransformDsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformDsaSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */



/********************************************************************
 *
 * HMAC transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_HMAC

XMLSEC_CRYPTO_EXPORT int               xmlSecGnuTLSHmacGetMinOutputLength(void);
XMLSEC_CRYPTO_EXPORT void              xmlSecGnuTLSHmacSetMinOutputLength(int min_length);

/**
 * xmlSecGnuTLSKeyDataHmacId:
 *
 * The HMAC key klass.
 */
#define xmlSecGnuTLSKeyDataHmacId \
        xmlSecGnuTLSKeyDataHmacGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataHmacGetKlass (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataHmacSet      (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecGnuTLSTransformHmacMd5Id:
 *
 * The HMAC with MD5 signature transform klass.
 */
#define xmlSecGnuTLSTransformHmacMd5Id \
        xmlSecGnuTLSTransformHmacMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformHmacMd5GetKlass(void);

#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGnuTLSTransformHmacRipemd160Id:
 *
 * The HMAC with RipeMD160 signature transform klass.
 */
#define xmlSecGnuTLSTransformHmacRipemd160Id \
        xmlSecGnuTLSTransformHmacRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformHmacRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGnuTLSTransformHmacSha1Id:
 *
 * The HMAC with SHA1 signature transform klass.
 */
#define xmlSecGnuTLSTransformHmacSha1Id \
        xmlSecGnuTLSTransformHmacSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformHmacSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGnuTLSTransformHmacSha256Id:
 *
 * The HMAC with SHA256 signature transform klass.
 */
#define xmlSecGnuTLSTransformHmacSha256Id \
        xmlSecGnuTLSTransformHmacSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformHmacSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGnuTLSTransformHmacSha384Id:
 *
 * The HMAC with SHA384 signature transform klass.
 */
#define xmlSecGnuTLSTransformHmacSha384Id \
        xmlSecGnuTLSTransformHmacSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformHmacSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGnuTLSTransformHmacSha512Id:
 *
 * The HMAC with SHA512 signature transform klass.
 */
#define xmlSecGnuTLSTransformHmacSha512Id \
        xmlSecGnuTLSTransformHmacSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformHmacSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

/********************************************************************
 *
 * RSA transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_RSA

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

/**
 * xmlSecGnuTLSKeyDataRsaId:
 *
 * The RSA key klass.
 */
#define xmlSecGnuTLSKeyDataRsaId \
        xmlSecGnuTLSKeyDataRsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataRsaGetKlass          (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataRsaAdoptPrivateKey   (xmlSecKeyDataPtr data,
                                                                                 gnutls_x509_privkey_t rsa_key);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataRsaAdoptPublicKey    (xmlSecKeyDataPtr data,
                                                                                 gnutls_datum_t * m,
                                                                                 gnutls_datum_t * e);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecGnuTLSTransformRsaMd5Id:
 *
 * The RSA-MD5 signature transform klass.
 */
#define xmlSecGnuTLSTransformRsaMd5Id  \
        xmlSecGnuTLSTransformRsaMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRsaMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGnuTLSTransformRsaRipemd160Id:
 *
 * The RSA-RIPEMD160 signature transform klass.
 */
#define xmlSecGnuTLSTransformRsaRipemd160Id    \
        xmlSecGnuTLSTransformRsaRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRsaRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGnuTLSTransformRsaSha1Id:
 *
 * The RSA-SHA1 signature transform klass.
 */
#define xmlSecGnuTLSTransformRsaSha1Id \
        xmlSecGnuTLSTransformRsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRsaSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGnuTLSTransformRsaSha256Id:
 *
 * The RSA-SHA256 signature transform klass.
 */
#define xmlSecGnuTLSTransformRsaSha256Id       \
        xmlSecGnuTLSTransformRsaSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRsaSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGnuTLSTransformRsaSha384Id:
 *
 * The RSA-SHA384 signature transform klass.
 */
#define xmlSecGnuTLSTransformRsaSha384Id       \
        xmlSecGnuTLSTransformRsaSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRsaSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGnuTLSTransformRsaSha512Id:
 *
 * The RSA-SHA512 signature transform klass.
 */
#define xmlSecGnuTLSTransformRsaSha512Id       \
        xmlSecGnuTLSTransformRsaSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRsaSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */


/********************************************************************
 *
 * SHA transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGnuTLSTransformSha1Id:
 *
 * The HMAC with SHA1 signature transform klass.
 */
#define xmlSecGnuTLSTransformSha1Id \
        xmlSecGnuTLSTransformSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGnuTLSTransformSha256Id:
 *
 * The HMAC with SHA256 signature transform klass.
 */
#define xmlSecGnuTLSTransformSha256Id \
        xmlSecGnuTLSTransformSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGnuTLSTransformSha384Id:
 *
 * The HMAC with SHA384 signature transform klass.
 */
#define xmlSecGnuTLSTransformSha384Id \
        xmlSecGnuTLSTransformSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGnuTLSTransformSha512Id:
 *
 * The HMAC with SHA512 signature transform klass.
 */
#define xmlSecGnuTLSTransformSha512Id \
        xmlSecGnuTLSTransformSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

/********************************************************************
 *
 * Md5 transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_MD5
/**
 * xmlSecGnuTLSTransformMd5Id:
 *
 * The MD5 digest transform klass.
 */
#define xmlSecGnuTLSTransformMd5Id \
        xmlSecGnuTLSTransformMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */


/********************************************************************
 *
 * RipeMD160 transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGnuTLSTransformRipemd160Id:
 *
 * The RIPEMD160 digest transform klass.
 */
#define xmlSecGnuTLSTransformRipemd160Id \
        xmlSecGnuTLSTransformRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGnuTLSTransformRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_GNUTLS_CRYPTO_H__ */

#define __XMLSEC_GNUTLS_CRYPTO_H__

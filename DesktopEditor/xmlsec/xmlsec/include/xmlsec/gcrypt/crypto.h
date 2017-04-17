/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GCRYPT_CRYPTO_H__
#define __XMLSEC_GCRYPT_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/dl.h>

XMLSEC_CRYPTO_EXPORT xmlSecCryptoDLFunctionsPtr xmlSecCryptoGetFunctions_gcrypt(void);

/********************************************************************
 *
 * Init shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptInit                (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptShutdown            (void);

XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeysMngrInit        (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptGenerateRandom      (xmlSecBufferPtr buffer,
                                                                         xmlSecSize size);


/********************************************************************
 *
 * AES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_AES
/**
 * xmlSecGCryptKeyDataAesId:
 *
 * The AES key data klass.
 */
#define xmlSecGCryptKeyDataAesId \
        xmlSecGCryptKeyDataAesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGCryptKeyDataAesGetKlass  (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataAesSet       (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);
/**
 * xmlSecGCryptTransformAes128CbcId:
 *
 * The AES128 CBC cipher transform klass.
 */
#define xmlSecGCryptTransformAes128CbcId \
        xmlSecGCryptTransformAes128CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGCryptTransformAes128CbcGetKlass(void);

/**
 * xmlSecGCryptTransformAes192CbcId:
 *
 * The AES192 CBC cipher transform klass.
 */
#define xmlSecGCryptTransformAes192CbcId \
        xmlSecGCryptTransformAes192CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGCryptTransformAes192CbcGetKlass(void);

/**
 * xmlSecGCryptTransformAes256CbcId:
 *
 * The AES256 CBC cipher transform klass.
 */
#define xmlSecGCryptTransformAes256CbcId \
        xmlSecGCryptTransformAes256CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGCryptTransformAes256CbcGetKlass(void);

/**
 * xmlSecGCryptTransformKWAes128Id:
 *
 * The AES 128 key wrap transform klass.
 */
#define xmlSecGCryptTransformKWAes128Id \
        xmlSecGCryptTransformKWAes128GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGCryptTransformKWAes128GetKlass(void);

/**
 * xmlSecGCryptTransformKWAes192Id:
 *
 * The AES 192 key wrap transform klass.
 */
#define xmlSecGCryptTransformKWAes192Id \
        xmlSecGCryptTransformKWAes192GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGCryptTransformKWAes192GetKlass(void);

/**
 * xmlSecGCryptTransformKWAes256Id:
 *
 * The AES 256 key wrap transform klass.
 */
#define xmlSecGCryptTransformKWAes256Id \
        xmlSecGCryptTransformKWAes256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecGCryptTransformKWAes256GetKlass(void);


#endif /* XMLSEC_NO_AES */

/********************************************************************
 *
 * DES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DES
/**
 * xmlSecGCryptKeyDataDesId:
 *
 * The DES key data klass.
 */
#define xmlSecGCryptKeyDataDesId \
        xmlSecGCryptKeyDataDesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGCryptKeyDataDesGetKlass  (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataDesSet       (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);

/**
 * xmlSecGCryptTransformDes3CbcId:
 *
 * The DES3 CBC cipher transform klass.
 */
#define xmlSecGCryptTransformDes3CbcId \
        xmlSecGCryptTransformDes3CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformDes3CbcGetKlass(void);

/**
 * xmlSecGCryptTransformKWDes3Id:
 *
 * The DES3 KW transform klass.
 */
#define xmlSecGCryptTransformKWDes3Id \
        xmlSecGCryptTransformKWDes3GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformKWDes3GetKlass(void);

#endif /* XMLSEC_NO_DES */

/********************************************************************
 *
 * DSA transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DSA
#include <gcrypt.h>

/**
 * xmlSecGCryptKeyDataDsaId:
 *
 * The DSA key klass.
 */
#define xmlSecGCryptKeyDataDsaId \
        xmlSecGCryptKeyDataDsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGCryptKeyDataDsaGetKlass          (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataDsaAdoptKey          (xmlSecKeyDataPtr data,
                                                                                 gcry_sexp_t dsa_key);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataDsaAdoptKeyPair      (xmlSecKeyDataPtr data,
                                                                                 gcry_sexp_t pub_key,
                                                                                 gcry_sexp_t priv_key);
XMLSEC_CRYPTO_EXPORT gcry_sexp_t        xmlSecGCryptKeyDataDsaGetPublicKey      (xmlSecKeyDataPtr data);
XMLSEC_CRYPTO_EXPORT gcry_sexp_t        xmlSecGCryptKeyDataDsaGetPrivateKey     (xmlSecKeyDataPtr data);

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGCryptTransformDsaSha1Id:
 *
 * The DSA SHA1 signature transform klass.
 */
#define xmlSecGCryptTransformDsaSha1Id \
        xmlSecGCryptTransformDsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformDsaSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */



/********************************************************************
 *
 * HMAC transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_HMAC

XMLSEC_CRYPTO_EXPORT int               xmlSecGCryptHmacGetMinOutputLength(void);
XMLSEC_CRYPTO_EXPORT void              xmlSecGCryptHmacSetMinOutputLength(int min_length);

/**
 * xmlSecGCryptKeyDataHmacId:
 *
 * The HMAC key klass.
 */
#define xmlSecGCryptKeyDataHmacId \
        xmlSecGCryptKeyDataHmacGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGCryptKeyDataHmacGetKlass (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataHmacSet      (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecGCryptTransformHmacMd5Id:
 *
 * The HMAC with MD5 signature transform klass.
 */
#define xmlSecGCryptTransformHmacMd5Id \
        xmlSecGCryptTransformHmacMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformHmacMd5GetKlass(void);

#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGCryptTransformHmacRipemd160Id:
 *
 * The HMAC with RipeMD160 signature transform klass.
 */
#define xmlSecGCryptTransformHmacRipemd160Id \
        xmlSecGCryptTransformHmacRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformHmacRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGCryptTransformHmacSha1Id:
 *
 * The HMAC with SHA1 signature transform klass.
 */
#define xmlSecGCryptTransformHmacSha1Id \
        xmlSecGCryptTransformHmacSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformHmacSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGCryptTransformHmacSha256Id:
 *
 * The HMAC with SHA256 signature transform klass.
 */
#define xmlSecGCryptTransformHmacSha256Id \
        xmlSecGCryptTransformHmacSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformHmacSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGCryptTransformHmacSha384Id:
 *
 * The HMAC with SHA384 signature transform klass.
 */
#define xmlSecGCryptTransformHmacSha384Id \
        xmlSecGCryptTransformHmacSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformHmacSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGCryptTransformHmacSha512Id:
 *
 * The HMAC with SHA512 signature transform klass.
 */
#define xmlSecGCryptTransformHmacSha512Id \
        xmlSecGCryptTransformHmacSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformHmacSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

/********************************************************************
 *
 * RSA transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_RSA
#include <gcrypt.h>

/**
 * xmlSecGCryptKeyDataRsaId:
 *
 * The RSA key klass.
 */
#define xmlSecGCryptKeyDataRsaId \
        xmlSecGCryptKeyDataRsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGCryptKeyDataRsaGetKlass (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataRsaAdoptKey          (xmlSecKeyDataPtr data,
                                                                                 gcry_sexp_t rsa_key);
XMLSEC_CRYPTO_EXPORT int                xmlSecGCryptKeyDataRsaAdoptKeyPair      (xmlSecKeyDataPtr data,
                                                                                 gcry_sexp_t pub_key,
                                                                                 gcry_sexp_t priv_key);
XMLSEC_CRYPTO_EXPORT gcry_sexp_t        xmlSecGCryptKeyDataRsaGetPublicKey      (xmlSecKeyDataPtr data);
XMLSEC_CRYPTO_EXPORT gcry_sexp_t        xmlSecGCryptKeyDataRsaGetPrivateKey     (xmlSecKeyDataPtr data);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecGCryptTransformRsaMd5Id:
 *
 * The RSA-MD5 signature transform klass.
 */
#define xmlSecGCryptTransformRsaMd5Id  \
        xmlSecGCryptTransformRsaMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRsaMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGCryptTransformRsaRipemd160Id:
 *
 * The RSA-RIPEMD160 signature transform klass.
 */
#define xmlSecGCryptTransformRsaRipemd160Id    \
        xmlSecGCryptTransformRsaRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRsaRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGCryptTransformRsaSha1Id:
 *
 * The RSA-SHA1 signature transform klass.
 */
#define xmlSecGCryptTransformRsaSha1Id \
        xmlSecGCryptTransformRsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRsaSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGCryptTransformRsaSha256Id:
 *
 * The RSA-SHA256 signature transform klass.
 */
#define xmlSecGCryptTransformRsaSha256Id       \
        xmlSecGCryptTransformRsaSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRsaSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGCryptTransformRsaSha384Id:
 *
 * The RSA-SHA384 signature transform klass.
 */
#define xmlSecGCryptTransformRsaSha384Id       \
        xmlSecGCryptTransformRsaSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRsaSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGCryptTransformRsaSha512Id:
 *
 * The RSA-SHA512 signature transform klass.
 */
#define xmlSecGCryptTransformRsaSha512Id       \
        xmlSecGCryptTransformRsaSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRsaSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */


/********************************************************************
 *
 * SHA transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecGCryptTransformSha1Id:
 *
 * The HMAC with SHA1 signature transform klass.
 */
#define xmlSecGCryptTransformSha1Id \
        xmlSecGCryptTransformSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecGCryptTransformSha256Id:
 *
 * The HMAC with SHA256 signature transform klass.
 */
#define xmlSecGCryptTransformSha256Id \
        xmlSecGCryptTransformSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecGCryptTransformSha384Id:
 *
 * The HMAC with SHA384 signature transform klass.
 */
#define xmlSecGCryptTransformSha384Id \
        xmlSecGCryptTransformSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecGCryptTransformSha512Id:
 *
 * The HMAC with SHA512 signature transform klass.
 */
#define xmlSecGCryptTransformSha512Id \
        xmlSecGCryptTransformSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

/********************************************************************
 *
 * Md5 transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_MD5
/**
 * xmlSecGCryptTransformMd5Id:
 *
 * The MD5 digest transform klass.
 */
#define xmlSecGCryptTransformMd5Id \
        xmlSecGCryptTransformMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */


/********************************************************************
 *
 * RipeMD160 transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecGCryptTransformRipemd160Id:
 *
 * The RIPEMD160 digest transform klass.
 */
#define xmlSecGCryptTransformRipemd160Id \
        xmlSecGCryptTransformRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecGCryptTransformRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_GCRYPT_CRYPTO_H__ */

#define __XMLSEC_GCRYPT_CRYPTO_H__

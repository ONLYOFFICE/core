/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 */
#ifndef __XMLSEC_MSCRYPTO_CRYPTO_H__
#define __XMLSEC_MSCRYPTO_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <windows.h>
#include <wincrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/dl.h>

XMLSEC_CRYPTO_EXPORT xmlSecCryptoDLFunctionsPtr xmlSecCryptoGetFunctions_mscrypto(void);

/********************************************************************
 *
 * Init shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoInit              (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoShutdown          (void);

XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeysMngrInit      (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoGenerateRandom    (xmlSecBufferPtr buffer,
                                                                         size_t size);

XMLSEC_CRYPTO_EXPORT void               xmlSecMSCryptoErrorsDefaultCallback(const char* file,
                                                                        int line,
                                                                        const char* func,
                                                                        const char* errorObject,
                                                                        const char* errorSubject,
                                                                        int reason,
                                                                        const char* msg);

/******************************************************************************
 *
 * String encoding conversion utils
 *
 ******************************************************************************/
XMLSEC_CRYPTO_EXPORT LPWSTR             xmlSecMSCryptoConvertLocaleToUnicode(const char* str);

XMLSEC_CRYPTO_EXPORT LPWSTR             xmlSecMSCryptoConvertUtf8ToUnicode  (const xmlChar* str);
XMLSEC_CRYPTO_EXPORT xmlChar*           xmlSecMSCryptoConvertUnicodeToUtf8  (LPCWSTR str);

XMLSEC_CRYPTO_EXPORT xmlChar*           xmlSecMSCryptoConvertLocaleToUtf8   (const char* str);
XMLSEC_CRYPTO_EXPORT char*              xmlSecMSCryptoConvertUtf8ToLocale   (const xmlChar* str);

XMLSEC_CRYPTO_EXPORT xmlChar*           xmlSecMSCryptoConvertTstrToUtf8     (LPCTSTR str);
XMLSEC_CRYPTO_EXPORT LPTSTR             xmlSecMSCryptoConvertUtf8ToTstr     (const xmlChar*  str);


/********************************************************************
 *
 * DSA transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DSA

/**
 * xmlSecMSCryptoKeyDataDsaId:
 *
 * The DSA key klass.
 */
#define xmlSecMSCryptoKeyDataDsaId \
        xmlSecMSCryptoKeyDataDsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataDsaGetKlass        (void);

/**
 * xmlSecMSCryptoTransformDsaSha1Id:
 *
 * The DSA SHA1 signature transform klass.
 */
#define xmlSecMSCryptoTransformDsaSha1Id \
        xmlSecMSCryptoTransformDsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformDsaSha1GetKlass(void);

#endif /* XMLSEC_NO_DSA */

/********************************************************************
 *
 * GOST2001 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_GOST

/**
 * xmlSecMSCryptoKeyDataGost2001Id:
 *
 * The GOST2001 key klass.
 */
#define xmlSecMSCryptoKeyDataGost2001Id \
        xmlSecMSCryptoKeyDataGost2001GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataGost2001GetKlass   (void);

/**
 * xmlSecMSCryptoTransformGost2001GostR3411_94Id:
 *
 * The GOST2001 GOSTR3411_94 signature transform klass.
 */
#define xmlSecMSCryptoTransformGost2001GostR3411_94Id \
        xmlSecMSCryptoTransformGost2001GostR3411_94GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformGost2001GostR3411_94GetKlass(void);

#endif /* XMLSEC_NO_GOST */

/********************************************************************
 *
 * RSA transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_RSA

/**
 * xmlSecMSCryptoKeyDataRsaId:
 *
 * The RSA key klass.
 */
#define xmlSecMSCryptoKeyDataRsaId \
        xmlSecMSCryptoKeyDataRsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId xmlSecMSCryptoKeyDataRsaGetKlass(void);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecMSCryptoTransformRsaMd5Id:
 *
 * The RSA-MD5 signature transform klass.
 */
#define xmlSecMSCryptoTransformRsaMd5Id        \
        xmlSecMSCryptoTransformRsaMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecMSCryptoTransformRsaSha1Id:
 *
 * The RSA-SHA1 signature transform klass.
 */
#define xmlSecMSCryptoTransformRsaSha1Id        \
        xmlSecMSCryptoTransformRsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecMSCryptoTransformRsaSha256Id:
 *
 * The RSA-SHA256 signature transform klass.
 */
#define xmlSecMSCryptoTransformRsaSha256Id     \
       xmlSecMSCryptoTransformRsaSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecMSCryptoTransformRsaSha384Id:
 *
 * The RSA-SHA384 signature transform klass.
 */
#define xmlSecMSCryptoTransformRsaSha384Id     \
       xmlSecMSCryptoTransformRsaSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecMSCryptoTransformRsaSha512Id:
 *
 * The RSA-SHA512 signature transform klass.
 */
#define xmlSecMSCryptoTransformRsaSha512Id     \
       xmlSecMSCryptoTransformRsaSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

/**
 * xmlSecMSCryptoTransformRsaPkcs1Id:
 *
 * The RSA PKCS1 key transport transform klass.
 */
#define xmlSecMSCryptoTransformRsaPkcs1Id \
        xmlSecMSCryptoTransformRsaPkcs1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaPkcs1GetKlass(void);

/**
 * xmlSecMSCryptoTransformRsaOaepId:
 *
 * The RSA OAEP key transport transform klass.
 */
#define xmlSecMSCryptoTransformRsaOaepId \
        xmlSecMSCryptoTransformRsaOaepGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaOaepGetKlass(void);

/**
 * xmlSecMSCryptoTransformRsaOaepId:
 *
 * The RSA PKCS1 key transport transform klass.
 */
/*
#define xmlSecMSCryptoTransformRsaOaepId \
        xmlSecMSCryptoTransformRsaOaepGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformRsaOaepGetKlass(void);
*/
#endif /* XMLSEC_NO_RSA */

/********************************************************************
 *
 * Md5 transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_MD5
/**
 * xmlSecMSCryptoTransformMd5Id:
 *
 * The MD5 digest transform klass.
 */
#define xmlSecMSCryptoTransformMd5Id \
        xmlSecMSCryptoTransformMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */


/********************************************************************
 *
 * SHA1 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA1

/**
 * xmlSecMSCryptoTransformSha1Id:
 *
 * The SHA1 digest transform klass.
 */
#define xmlSecMSCryptoTransformSha1Id \
        xmlSecMSCryptoTransformSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

/********************************************************************
 *
 * SHA256 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA256

/**
 * xmlSecMSCryptoTransformSha256Id:
 *
 * The SHA256 digest transform klass.
 */
#define xmlSecMSCryptoTransformSha256Id \
       xmlSecMSCryptoTransformSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

/********************************************************************
 *
 * SHA384 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA384

/**
 * xmlSecMSCryptoTransformSha384Id:
 *
 * The SHA384 digest transform klass.
 */
#define xmlSecMSCryptoTransformSha384Id \
       xmlSecMSCryptoTransformSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

/********************************************************************
 *
 * SHA512 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA512

/**
 * xmlSecMSCryptoTransformSha512Id:
 *
 * The SHA512 digest transform klass.
 */
#define xmlSecMSCryptoTransformSha512Id \
       xmlSecMSCryptoTransformSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

/********************************************************************
 *
 * GOSTR3411_94 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_GOST

/**
 * xmlSecMSCryptoTransformGostR3411_94Id:
 *
 * The GOSTR3411_94 digest transform klass.
 */
#define xmlSecMSCryptoTransformGostR3411_94Id \
        xmlSecMSCryptoTransformGostR3411_94GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformGostR3411_94GetKlass(void);
#endif /* XMLSEC_NO_GOST */

/********************************************************************
 *
 * AES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_AES
/**
 * xmlSecMSCryptoKeyDataAesId:
 *
 * The AES key data klass.
 */
#define xmlSecMSCryptoKeyDataAesId \
        xmlSecMSCryptoKeyDataAesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataAesGetKlass(void);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeyDataAesSet     (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);
/**
 * xmlSecMSCryptoTransformAes128CbcId:
 *
 * The AES128 CBC cipher transform klass.
 */
#define xmlSecMSCryptoTransformAes128CbcId \
        xmlSecMSCryptoTransformAes128CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecMSCryptoTransformAes128CbcGetKlass(void);

/**
 * xmlSecMSCryptoTransformAes192CbcId:
 *
 * The AES192 CBC cipher transform klass.
 */
#define xmlSecMSCryptoTransformAes192CbcId \
        xmlSecMSCryptoTransformAes192CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecMSCryptoTransformAes192CbcGetKlass(void);

/**
 * xmlSecMSCryptoTransformAes256CbcId:
 *
 * The AES256 CBC cipher transform klass.
 */
#define xmlSecMSCryptoTransformAes256CbcId \
        xmlSecMSCryptoTransformAes256CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecMSCryptoTransformAes256CbcGetKlass(void);

/**
 * xmlSecMSCryptoTransformKWAes128Id:
 *
 * The AES 128 key wrap transform klass.
 */
#define xmlSecMSCryptoTransformKWAes128Id \
        xmlSecMSCryptoTransformKWAes128GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecMSCryptoTransformKWAes128GetKlass(void);

/**
 * xmlSecMSCryptoTransformKWAes192Id:
 *
 * The AES 192 key wrap transform klass.
 */
#define xmlSecMSCryptoTransformKWAes192Id \
        xmlSecMSCryptoTransformKWAes192GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecMSCryptoTransformKWAes192GetKlass(void);

/**
 * xmlSecMSCryptoTransformKWAes256Id:
 *
 * The AES 256 key wrap transform klass.
 */
#define xmlSecMSCryptoTransformKWAes256Id \
        xmlSecMSCryptoTransformKWAes256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecMSCryptoTransformKWAes256GetKlass(void);

#endif /* XMLSEC_NO_AES */


/********************************************************************
 *
 * DES transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DES

/**
 * xmlSecMSCryptoKeyDataDesId:
 *
 * The DES key data klass.
 */
#define xmlSecMSCryptoKeyDataDesId \
        xmlSecMSCryptoKeyDataDesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataDesGetKlass(void);

/**
 * xmlSecMSCryptoTransformDes3CbcId:
 *
 * The DES3 CBC cipher transform klass.
 */
#define xmlSecMSCryptoTransformDes3CbcId \
        xmlSecMSCryptoTransformDes3CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformDes3CbcGetKlass(void);

/**
 * xmlSecMSCryptoTransformKWDes3Id:
 *
 * The DES3 KW transform klass.
 */
#define xmlSecMSCryptoTransformKWDes3Id \
        xmlSecMSCryptoTransformKWDes3GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformKWDes3GetKlass(void);

#endif /* XMLSEC_NO_DES */


/********************************************************************
 *
 * HMAC transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_HMAC

XMLSEC_CRYPTO_EXPORT int               xmlSecMSCryptoHmacGetMinOutputLength(void);
XMLSEC_CRYPTO_EXPORT void              xmlSecMSCryptoHmacSetMinOutputLength(int min_length);

/**
 * xmlSecMSCryptoKeyDataHmacId:
 *
 * The DHMAC key klass.
 */
#define xmlSecMSCryptoKeyDataHmacId \
        xmlSecMSCryptoKeyDataHmacGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataHmacGetKlass(void);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeyDataHmacSet     (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecMSCryptoTransformHmacMd5Id:
 *
 * The HMAC with MD5 signature transform klass.
 */
#define xmlSecMSCryptoTransformHmacMd5Id \
        xmlSecMSCryptoTransformHmacMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformHmacMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecMSCryptoTransformHmacSha1Id:
 *
 * The HMAC with SHA1 signature transform klass.
 */
#define xmlSecMSCryptoTransformHmacSha1Id \
        xmlSecMSCryptoTransformHmacSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformHmacSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecMSCryptoTransformHmacSha256Id:
 *
 * The HMAC with SHA256 signature transform klass.
 */
#define xmlSecMSCryptoTransformHmacSha256Id \
        xmlSecMSCryptoTransformHmacSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformHmacSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecMSCryptoTransformHmacSha384Id:
 *
 * The HMAC with SHA384 signature transform klass.
 */
#define xmlSecMSCryptoTransformHmacSha384Id \
        xmlSecMSCryptoTransformHmacSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformHmacSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecMSCryptoTransformHmacSha512Id:
 *
 * The HMAC with SHA512 signature transform klass.
 */
#define xmlSecMSCryptoTransformHmacSha512Id \
        xmlSecMSCryptoTransformHmacSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecMSCryptoTransformHmacSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_HMAC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_MSCRYPTO_CRYPTO_H__ */

#define __XMLSEC_MSCRYPTO_CRYPTO_H__

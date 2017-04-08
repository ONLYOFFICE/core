/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#ifndef __XMLSEC_NSS_CRYPTO_H__
#define __XMLSEC_NSS_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <nspr.h>
#include <nss.h>
#include <pk11func.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/dl.h>

XMLSEC_CRYPTO_EXPORT xmlSecCryptoDLFunctionsPtr xmlSecCryptoGetFunctions_nss(void);

/********************************************************************
 *
 * Init shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecNssInit                   (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssShutdown               (void);

XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeysMngrInit           (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssGenerateRandom         (xmlSecBufferPtr buffer,
                                                                         xmlSecSize size);

XMLSEC_CRYPTO_EXPORT void               xmlSecNssErrorsDefaultCallback  (const char* file,
                                                                        int line,
                                                                        const char* func,
                                                                        const char* errorObject,
                                                                        const char* errorSubject,
                                                                        int reason,
                                                                        const char* msg);

XMLSEC_CRYPTO_EXPORT PK11SlotInfo * xmlSecNssGetInternalKeySlot(void);

/********************************************************************
 *
 * AES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_AES
/**
 * xmlSecNssKeyDataAesId:
 *
 * The AES key data klass.
 */
#define xmlSecNssKeyDataAesId \
        xmlSecNssKeyDataAesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataAesGetKlass     (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeyDataAesSet          (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);
/**
 * xmlSecNssTransformAes128CbcId:
 *
 * The AES128 CBC cipher transform klass.
 */
#define xmlSecNssTransformAes128CbcId \
        xmlSecNssTransformAes128CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecNssTransformAes128CbcGetKlass(void);

/**
 * xmlSecNssTransformAes192CbcId:
 *
 * The AES192 CBC cipher transform klass.
 */
#define xmlSecNssTransformAes192CbcId \
        xmlSecNssTransformAes192CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecNssTransformAes192CbcGetKlass(void);

/**
 * xmlSecNssTransformAes256CbcId:
 *
 * The AES256 CBC cipher transform klass.
 */
#define xmlSecNssTransformAes256CbcId \
        xmlSecNssTransformAes256CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecNssTransformAes256CbcGetKlass(void);

/**
 * xmlSecNssTransformKWAes128Id:
 *
 * The AES 128 key wrap transform klass.
 */
#define xmlSecNssTransformKWAes128Id \
        xmlSecNssTransformKWAes128GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecNssTransformKWAes128GetKlass(void);

/**
 * xmlSecNssTransformKWAes192Id:
 *
 * The AES 192 key wrap transform klass.
 */
#define xmlSecNssTransformKWAes192Id \
        xmlSecNssTransformKWAes192GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecNssTransformKWAes192GetKlass(void);

/**
 * xmlSecNssTransformKWAes256Id:
 *
 * The AES 256 key wrap transform klass.
 */
#define xmlSecNssTransformKWAes256Id \
        xmlSecNssTransformKWAes256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId  xmlSecNssTransformKWAes256GetKlass(void);

#endif /* XMLSEC_NO_AES */

/********************************************************************
 *
 * DES transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DES
/**
 * xmlSecNssKeyDataDesId:
 *
 * The DES key data klass.
 */
#define xmlSecNssKeyDataDesId \
        xmlSecNssKeyDataDesGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataDesGetKlass     (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeyDataDesSet          (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);

/**
 * xmlSecNssTransformDes3CbcId:
 *
 * The Triple DES CBC cipher transform klass.
 */
#define xmlSecNssTransformDes3CbcId \
        xmlSecNssTransformDes3CbcGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformDes3CbcGetKlass(void);

/**
* xmlSecNssTransformKWDes3Id:
*
* The DES3 KW transform klass.
*/
#define xmlSecNssTransformKWDes3Id \
        xmlSecNssTransformKWDes3GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformKWDes3GetKlass(void);


#endif /* XMLSEC_NO_DES */

/********************************************************************
 *
 * DSA transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_DSA

/**
 * xmlSecNssKeyDataDsaId:
 *
 * The DSA key klass.
 */
#define xmlSecNssKeyDataDsaId \
        xmlSecNssKeyDataDsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataDsaGetKlass     (void);

/**
 * xmlSecNssTransformDsaSha1Id:
 *
 * The DSA SHA1 signature transform klass.
 */
#define xmlSecNssTransformDsaSha1Id \
        xmlSecNssTransformDsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformDsaSha1GetKlass(void);

#endif /* XMLSEC_NO_DSA */


/********************************************************************
 *
 * HMAC transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_HMAC

XMLSEC_CRYPTO_EXPORT int               xmlSecNssHmacGetMinOutputLength(void);
XMLSEC_CRYPTO_EXPORT void              xmlSecNssHmacSetMinOutputLength(int min_length);

/**
 * xmlSecNssKeyDataHmacId:
 *
 * The DHMAC key data klass.
 */
#define xmlSecNssKeyDataHmacId \
        xmlSecNssKeyDataHmacGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataHmacGetKlass    (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeyDataHmacSet         (xmlSecKeyDataPtr data,
                                                                         const xmlSecByte* buf,
                                                                         xmlSecSize bufSize);
#ifndef XMLSEC_NO_MD5
/**
 * xmlSecNssTransformHmacMd5Id:
 *
 * The HMAC with MD5 signature transform klass.
 */
#define xmlSecNssTransformHmacMd5Id \
        xmlSecNssTransformHmacMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformHmacMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/**
 * xmlSecNssTransformHmacRipemd160Id:
 *
 * The HMAC with RipeMD160 signature transform klass.
 */
#define xmlSecNssTransformHmacRipemd160Id \
        xmlSecNssTransformHmacRipemd160GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformHmacRipemd160GetKlass(void);
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecNssTransformHmacSha1Id:
 *
 * The HMAC with SHA1 signature transform klass.
 */
#define xmlSecNssTransformHmacSha1Id \
        xmlSecNssTransformHmacSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformHmacSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecNssTransformHmacSha256Id:
 *
 * The HMAC with SHA256 signature transform klass.
 */
#define xmlSecNssTransformHmacSha256Id \
        xmlSecNssTransformHmacSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformHmacSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecNssTransformHmacSha384Id:
 *
 * The HMAC with SHA384 signature transform klass.
 */
#define xmlSecNssTransformHmacSha384Id \
        xmlSecNssTransformHmacSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformHmacSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecNssTransformHmacSha512Id:
 *
 * The HMAC with SHA512 signature transform klass.
 */
#define xmlSecNssTransformHmacSha512Id \
        xmlSecNssTransformHmacSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformHmacSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */


#endif /* XMLSEC_NO_HMAC */


/********************************************************************
 *
 * RSA transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_RSA

/**
 * xmlSecNssKeyDataRsaId:
 *
 * The RSA key klass.
 */
#define xmlSecNssKeyDataRsaId \
        xmlSecNssKeyDataRsaGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataRsaGetKlass     (void);

#ifndef XMLSEC_NO_MD5
/**
 * xmlSecNssTransformRsaMd5Id:
 *
 * The RSA-MD5 signature transform klass.
 */
#define xmlSecNssTransformRsaMd5Id  \
        xmlSecNssTransformRsaMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecNssTransformRsaSha1Id:
 *
 * The RSA-SHA1 signature transform klass.
 */
#define xmlSecNssTransformRsaSha1Id     \
        xmlSecNssTransformRsaSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaSha1GetKlass(void);
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecNssTransformRsaSha256Id:
 *
 * The RSA-SHA256 signature transform klass.
 */
#define xmlSecNssTransformRsaSha256Id       \
        xmlSecNssTransformRsaSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecNssTransformRsaSha384Id:
 *
 * The RSA-SHA384 signature transform klass.
 */
#define xmlSecNssTransformRsaSha384Id       \
        xmlSecNssTransformRsaSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecNssTransformRsaSha512Id:
 *
 * The RSA-SHA512 signature transform klass.
 */
#define xmlSecNssTransformRsaSha512Id       \
        xmlSecNssTransformRsaSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

/**
 * xmlSecNssTransformRsaPkcs1Id:
 *
 * The RSA PKCS1 key transport transform klass.
 */
#define xmlSecNssTransformRsaPkcs1Id \
        xmlSecNssTransformRsaPkcs1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaPkcs1GetKlass(void);

/* aleksey, April 2010: NSS 3.12.6 has CKM_RSA_PKCS_OAEP algorithm but
   it doesn't implement the SHA1 OAEP PKCS we need

   https://bugzilla.mozilla.org/show_bug.cgi?id=158747
*/
#ifdef XMLSEC_NSS_RSA_OAEP_TODO
/**
 * xmlSecNssTransformRsaOaepId:
 *
 * The RSA OAEP key transport transform klass.
 */
#define xmlSecNssTransformRsaOaepId \
        xmlSecNssTransformRsaOaepGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformRsaOaepGetKlass(void);
#endif /* XMLSEC_NSS_RSA_OAEP_TODO */

#endif /* XMLSEC_NO_RSA */


/********************************************************************
 *
 * SHA1 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA1
/**
 * xmlSecNssTransformSha1Id:
 *
 * The SHA1 digest transform klass.
 */
#define xmlSecNssTransformSha1Id \
        xmlSecNssTransformSha1GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformSha1GetKlass   (void);
#endif /* XMLSEC_NO_SHA1 */

/********************************************************************
 *
 * SHA256 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA256
/**
 * xmlSecNssTransformSha256Id:
 *
 * The SHA256 digest transform klass.
 */
#define xmlSecNssTransformSha256Id \
        xmlSecNssTransformSha256GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformSha256GetKlass(void);
#endif /* XMLSEC_NO_SHA256 */

/********************************************************************
 *
 * SHA384 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA384
/**
 * xmlSecNssTransformSha384Id:
 *
 * The SHA384 digest transform klass.
 */
#define xmlSecNssTransformSha384Id \
        xmlSecNssTransformSha384GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformSha384GetKlass(void);
#endif /* XMLSEC_NO_SHA384 */

/********************************************************************
 *
 * SHA512 transform
 *
 *******************************************************************/
#ifndef XMLSEC_NO_SHA512
/**
 * xmlSecNssTransformSha512Id:
 *
 * The SHA512 digest transform klass.
 */
#define xmlSecNssTransformSha512Id \
        xmlSecNssTransformSha512GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformSha512GetKlass(void);
#endif /* XMLSEC_NO_SHA512 */

/********************************************************************
 *
 * MD5 transforms
 *
 *******************************************************************/
#ifndef XMLSEC_NO_MD5
/**
 * xmlSecNssTransformMd5Id:
 *
 * The MD5 digest transform klass.
 */
#define xmlSecNssTransformMd5Id \
        xmlSecNssTransformMd5GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecTransformId xmlSecNssTransformMd5GetKlass(void);
#endif /* XMLSEC_NO_MD5 */




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_NSS_CRYPTO_H__ */

#define __XMLSEC_NSS_CRYPTO_H__

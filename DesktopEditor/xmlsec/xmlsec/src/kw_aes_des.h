/**
 * XMLSec library
 *
 * THIS IS A PRIVATE XMLSEC HEADER FILE
 * DON'T USE IT IN YOUR APPLICATION
 *
 * Implementation of AES/DES Key Transport algorithm
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_KT_AES_DES_H__
#define __XMLSEC_KT_AES_DES_H__

#ifndef XMLSEC_PRIVATE
#error "private.h file contains private xmlsec definitions and should not be used outside xmlsec or xmlsec-$crypto libraries"
#endif /* XMLSEC_PRIVATE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef XMLSEC_NO_DES
/********************************************************************
 *
 * KT DES
 *
 ********************************************************************/
#define XMLSEC_KW_DES3_KEY_LENGTH                   24
#define XMLSEC_KW_DES3_IV_LENGTH                    8
#define XMLSEC_KW_DES3_BLOCK_LENGTH                 8
#define XMLSEC_KW_DES3_SHA_DIGEST_LENGTH            20


typedef int  (*xmlSecKWDes3Sha1Method)              (void * context,
                                                     const xmlSecByte * in,
                                                     xmlSecSize inSize,
                                                     xmlSecByte * out,
                                                     xmlSecSize outSize);
typedef int  (*xmlSecKWDes3GenerateRandomMethod)    (void * context,
                                                     xmlSecByte * out,
                                                     xmlSecSize outSize);
typedef int  (*xmlSecKWDes3BlockEncryptMethod)      (void * context,
                                                     const xmlSecByte * iv,
                                                     xmlSecSize ivSize,
                                                     const xmlSecByte * in,
                                                     xmlSecSize inSize,
                                                     xmlSecByte * out,
                                                     xmlSecSize outSize);
typedef int  (*xmlSecKWDes3BlockDecryptMethod)      (void * context,
                                                     const xmlSecByte * iv,
                                                     xmlSecSize ivSize,
                                                     const xmlSecByte * in,
                                                     xmlSecSize inSize,
                                                     xmlSecByte * out,
                                                     xmlSecSize outSize);


struct _xmlSecKWDes3Klass {
    /* callbacks */
    xmlSecKWDes3GenerateRandomMethod    generateRandom;
    xmlSecKWDes3Sha1Method              sha1;
    xmlSecKWDes3BlockEncryptMethod      encrypt;
    xmlSecKWDes3BlockDecryptMethod      decrypt;

    /* for the future */
    void*                               reserved0;
    void*                               reserved1;
}; 
typedef const struct _xmlSecKWDes3Klass              xmlSecKWDes3Klass,
                                                    *xmlSecKWDes3Id;

#define xmlSecKWDes3CheckId(id) \
    ( \
     ((id) != NULL) && \
     ((id)->generateRandom != NULL) && \
     ((id)->sha1 != NULL) && \
     ((id)->encrypt != NULL) && \
     ((id)->decrypt != NULL) \
    )

XMLSEC_EXPORT int
xmlSecKWDes3Encode(xmlSecKWDes3Id kwDes3Id, void *context,
                  const xmlSecByte *in, xmlSecSize inSize,
                  xmlSecByte *out, xmlSecSize outSize);

XMLSEC_EXPORT int
xmlSecKWDes3Decode(xmlSecKWDes3Id kwDes3Id, void *context,
                  const xmlSecByte *in, xmlSecSize inSize,
                  xmlSecByte *out, xmlSecSize outSize);
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
/********************************************************************
 *
 * KT AES
 *
 ********************************************************************/
#define XMLSEC_KW_AES_MAGIC_BLOCK_SIZE              8
#define XMLSEC_KW_AES_BLOCK_SIZE                    16
#define XMLSEC_KW_AES128_KEY_SIZE                   16
#define XMLSEC_KW_AES192_KEY_SIZE                   24
#define XMLSEC_KW_AES256_KEY_SIZE                   32

typedef int  (*xmlSecKWAesBlockEncryptMethod)       (const xmlSecByte * in,
                                                     xmlSecSize inSize,
                                                     xmlSecByte * out,
                                                     xmlSecSize outSize,
                                                     void * context);
typedef int  (*xmlSecKWAesBlockDecryptMethod)       (const xmlSecByte * in,
                                                     xmlSecSize inSize,
                                                     xmlSecByte * out,
                                                     xmlSecSize outSize,
                                                     void * context);


struct _xmlSecKWAesKlass {
    /* callbacks */
    xmlSecKWAesBlockEncryptMethod       encrypt;
    xmlSecKWAesBlockDecryptMethod       decrypt;

    /* for the future */
    void*                               reserved0;
    void*                               reserved1;
}; 
typedef const struct _xmlSecKWAesKlass              xmlSecKWAesKlass,
                                                    *xmlSecKWAesId;

XMLSEC_EXPORT int
xmlSecKWAesEncode(xmlSecKWAesId kwAesId, void *context,
                  const xmlSecByte *in, xmlSecSize inSize,
                  xmlSecByte *out, xmlSecSize outSize);

XMLSEC_EXPORT int
xmlSecKWAesDecode(xmlSecKWAesId kwAesId, void *context,
                  const xmlSecByte *in, xmlSecSize inSize,
                  xmlSecByte *out, xmlSecSize outSize);

#endif /* XMLSEC_NO_AES */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_KT_AES_DES_H__ */

/**
 *
 * XMLSec library
 *
 * DES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef XMLSEC_NO_DES
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <gcrypt.h>


#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/crypto.h>

#include "../kw_aes_des.h"

/*********************************************************************
 *
 * DES KW implementation
 *
 *********************************************************************/
static int       xmlSecGCryptKWDes3GenerateRandom               (void * context,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int       xmlSecGCryptKWDes3Sha1                         (void * context,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize, 
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int      xmlSecGCryptKWDes3BlockEncrypt                  (void * context,
                                                                 const xmlSecByte * iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int      xmlSecGCryptKWDes3BlockDecrypt                  (void * context,
                                                                 const xmlSecByte * iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);

static xmlSecKWDes3Klass xmlSecGCryptKWDes3ImplKlass = {
    /* callbacks */
    xmlSecGCryptKWDes3GenerateRandom,       /* xmlSecKWDes3GenerateRandomMethod     generateRandom; */
    xmlSecGCryptKWDes3Sha1,                 /* xmlSecKWDes3Sha1Method               sha1; */
    xmlSecGCryptKWDes3BlockEncrypt,         /* xmlSecKWDes3BlockEncryptMethod       encrypt; */
    xmlSecGCryptKWDes3BlockDecrypt,         /* xmlSecKWDes3BlockDecryptMethod       decrypt; */

    /* for the future */
    NULL,                                   /* void*                               reserved0; */
    NULL,                                   /* void*                               reserved1; */
};

static int      xmlSecGCryptKWDes3Encrypt                       (const xmlSecByte *key, 
                                                                 xmlSecSize keySize,
                                                                 const xmlSecByte *iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte *in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte *out, 
                                                                 xmlSecSize outSize, 
                                                                 int enc);


/*********************************************************************
 *
 * Triple DES Key Wrap transform
 *
 * key (xmlSecBuffer) is located after xmlSecTransform structure
 *
 ********************************************************************/
typedef struct _xmlSecGCryptKWDes3Ctx              xmlSecGCryptKWDes3Ctx,
                                                  *xmlSecGCryptKWDes3CtxPtr;
struct _xmlSecGCryptKWDes3Ctx {
    xmlSecBuffer        keyBuffer;
};
#define xmlSecGCryptKWDes3Size     \
    (sizeof(xmlSecTransform) + sizeof(xmlSecGCryptKWDes3Ctx))
#define xmlSecGCryptKWDes3GetCtx(transform) \
    ((xmlSecGCryptKWDes3CtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecGCryptKWDes3Initialize                    (xmlSecTransformPtr transform);
static void     xmlSecGCryptKWDes3Finalize                      (xmlSecTransformPtr transform);
static int      xmlSecGCryptKWDes3SetKeyReq                     (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecGCryptKWDes3SetKey                        (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecGCryptKWDes3Execute                       (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static xmlSecTransformKlass xmlSecGCryptKWDes3Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptKWDes3Size,                     /* xmlSecSize objSize */

    xmlSecNameKWDes3,                           /* const xmlChar* name; */
    xmlSecHrefKWDes3,                           /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecGCryptKWDes3Initialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptKWDes3Finalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptKWDes3SetKeyReq,                /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecGCryptKWDes3SetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptKWDes3Execute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformKWDes3GetKlass:
 *
 * The Triple DES key wrapper transform klass.
 *
 * Returns: Triple DES key wrapper transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformKWDes3GetKlass(void) {
    return(&xmlSecGCryptKWDes3Klass);
}

static int
xmlSecGCryptKWDes3Initialize(xmlSecTransformPtr transform) {
    xmlSecGCryptKWDes3CtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWDes3Id), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWDes3Size), -1);

    ctx = xmlSecGCryptKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    ret = xmlSecBufferInitialize(&(ctx->keyBuffer), 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void
xmlSecGCryptKWDes3Finalize(xmlSecTransformPtr transform) {
    xmlSecGCryptKWDes3CtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWDes3Id));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecGCryptKWDes3Size));

    ctx = xmlSecGCryptKWDes3GetCtx(transform);
    xmlSecAssert(ctx != NULL);

    xmlSecBufferFinalize(&(ctx->keyBuffer));
}

static int
xmlSecGCryptKWDes3SetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecGCryptKWDes3CtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWDes3Size), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecGCryptKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId       = xmlSecGCryptKeyDataDesId;
    keyReq->keyType     = xmlSecKeyDataTypeSymmetric;
    if(transform->operation == xmlSecTransformOperationEncrypt) {
        keyReq->keyUsage= xmlSecKeyUsageEncrypt;
    } else {
        keyReq->keyUsage= xmlSecKeyUsageDecrypt;
    }
    keyReq->keyBitsSize = 8 * XMLSEC_KW_DES3_KEY_LENGTH;
    return(0);
}

static int
xmlSecGCryptKWDes3SetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecGCryptKWDes3CtxPtr ctx;
    xmlSecBufferPtr buffer;
    xmlSecSize keySize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWDes3Size), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecGCryptKeyDataDesId), -1);

    ctx = xmlSecGCryptKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(xmlSecKeyGetValue(key));
    xmlSecAssert2(buffer != NULL, -1);

    keySize = xmlSecBufferGetSize(buffer);
    if(keySize < XMLSEC_KW_DES3_KEY_LENGTH) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA_SIZE,
                    "key length %d is not enough (%d expected)",
                    keySize, XMLSEC_KW_DES3_KEY_LENGTH);
        return(-1);
    }

    ret = xmlSecBufferSetData(&(ctx->keyBuffer), xmlSecBufferGetData(buffer), XMLSEC_KW_DES3_KEY_LENGTH);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferSetData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", XMLSEC_KW_DES3_KEY_LENGTH);
        return(-1);
    }

    return(0);
}

static int
xmlSecGCryptKWDes3Execute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecGCryptKWDes3CtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize, keySize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWDes3Size), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecGCryptKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keySize = xmlSecBufferGetSize(&(ctx->keyBuffer));
    xmlSecAssert2(keySize == XMLSEC_KW_DES3_KEY_LENGTH, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);
    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);
    xmlSecAssert2(outSize == 0, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (last == 0)) {
        /* just do nothing */
    } else  if((transform->status == xmlSecTransformStatusWorking) && (last != 0)) {
        if((inSize % XMLSEC_KW_DES3_BLOCK_LENGTH) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_SIZE,
                        "%d bytes - not %d bytes aligned",
                        inSize, XMLSEC_KW_DES3_BLOCK_LENGTH);
            return(-1);
        }

        if(transform->operation == xmlSecTransformOperationEncrypt) {
            /* the encoded key might be 16 bytes longer plus one block just in case */
            outSize = inSize + XMLSEC_KW_DES3_IV_LENGTH +
                               XMLSEC_KW_DES3_BLOCK_LENGTH +
                               XMLSEC_KW_DES3_BLOCK_LENGTH;
        } else {
            /* just in case, add a block */
            outSize = inSize + XMLSEC_KW_DES3_BLOCK_LENGTH;
        }

        ret = xmlSecBufferSetMaxSize(out, outSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferSetMaxSize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", outSize);
            return(-1);
        }

        if(transform->operation == xmlSecTransformOperationEncrypt) {
            ret = xmlSecKWDes3Encode(&xmlSecGCryptKWDes3ImplKlass, ctx,
                                    xmlSecBufferGetData(in), inSize,
                                    xmlSecBufferGetData(out), outSize);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecKWDes3Encode",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "key=%d,in=%d,out=%d",
                            keySize, inSize, outSize);
                return(-1);
            }
            outSize = ret;
        } else {
            ret = xmlSecKWDes3Decode(&xmlSecGCryptKWDes3ImplKlass, ctx,
                                    xmlSecBufferGetData(in), inSize,
                                    xmlSecBufferGetData(out), outSize);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecKWDes3Decode",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "key=%d,in=%d,out=%d",
                            keySize, inSize, outSize);
                return(-1);
            }
            outSize = ret;
        }

        ret = xmlSecBufferSetSize(out, outSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferSetSize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", outSize);
            return(-1);
        }

        ret = xmlSecBufferRemoveHead(in, inSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferRemoveHead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", inSize);
            return(-1);
        }

        transform->status = xmlSecTransformStatusFinished;
    } else if(transform->status == xmlSecTransformStatusFinished) {
        /* the only way we can get here is if there is no input */
        xmlSecAssert2(xmlSecBufferGetSize(&(transform->inBuf)) == 0, -1);
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_STATUS,
                    "status=%d", transform->status);
        return(-1);
    }
    return(0);
}

/*********************************************************************
 *
 * DES KW implementation
 *
 *********************************************************************/
static int
xmlSecGCryptKWDes3Sha1(void * context,
                       const xmlSecByte * in, xmlSecSize inSize,
                       xmlSecByte * out, xmlSecSize outSize) {
    xmlSecGCryptKWDes3CtxPtr ctx = (xmlSecGCryptKWDes3CtxPtr)context;
    gcry_md_hd_t digestCtx;
    unsigned char * res;
    unsigned int len;
    gcry_error_t err;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize > 0, -1);

    len = gcry_md_get_algo_dlen(GCRY_MD_SHA1);
    xmlSecAssert2(outSize >= len, -1);

    err = gcry_md_open(&digestCtx, GCRY_MD_SHA1, GCRY_MD_FLAG_SECURE); /* we are paranoid */
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_md_open(GCRY_MD_SHA1)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    gcry_md_write(digestCtx, in, inSize);

    err = gcry_md_final(digestCtx);
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_md_final",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        gcry_md_close(digestCtx);
        return(-1);
    }

    res = gcry_md_read(digestCtx, GCRY_MD_SHA1);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_md_read(GCRY_MD_SHA1)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gcry_md_close(digestCtx);
        return(-1);
    }

    /* done */
    xmlSecAssert2(outSize >= len, -1);
    memcpy(out, res, len);
    gcry_md_close(digestCtx);
    return(len);
}

static int
xmlSecGCryptKWDes3GenerateRandom(void * context,
                                 xmlSecByte * out, xmlSecSize outSize) {
    xmlSecGCryptKWDes3CtxPtr ctx = (xmlSecGCryptKWDes3CtxPtr)context;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize > 0, -1);

    gcry_randomize(out, outSize, GCRY_STRONG_RANDOM);
    return((int)outSize);
}

static int
xmlSecGCryptKWDes3BlockEncrypt(void * context,
                               const xmlSecByte * iv, xmlSecSize ivSize,
                               const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize) {
    xmlSecGCryptKWDes3CtxPtr ctx = (xmlSecGCryptKWDes3CtxPtr)context;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(xmlSecBufferGetData(&(ctx->keyBuffer)) != NULL, -1);
    xmlSecAssert2(xmlSecBufferGetSize(&(ctx->keyBuffer)) >= XMLSEC_KW_DES3_KEY_LENGTH, -1);
    xmlSecAssert2(iv != NULL, -1);
    xmlSecAssert2(ivSize >= XMLSEC_KW_DES3_IV_LENGTH, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= inSize, -1);

    ret = xmlSecGCryptKWDes3Encrypt(xmlSecBufferGetData(&(ctx->keyBuffer)),
                                    XMLSEC_KW_DES3_KEY_LENGTH,
                                    iv, XMLSEC_KW_DES3_IV_LENGTH,
                                    in, inSize,
                                    out, outSize,
                                    1); /* encrypt */
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKWDes3Encrypt",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(ret);
}

static int
xmlSecGCryptKWDes3BlockDecrypt(void * context,
                               const xmlSecByte * iv, xmlSecSize ivSize,
                               const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize) {
    xmlSecGCryptKWDes3CtxPtr ctx = (xmlSecGCryptKWDes3CtxPtr)context;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(xmlSecBufferGetData(&(ctx->keyBuffer)) != NULL, -1);
    xmlSecAssert2(xmlSecBufferGetSize(&(ctx->keyBuffer)) >= XMLSEC_KW_DES3_KEY_LENGTH, -1);
    xmlSecAssert2(iv != NULL, -1);
    xmlSecAssert2(ivSize >= XMLSEC_KW_DES3_IV_LENGTH, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= inSize, -1);

    ret = xmlSecGCryptKWDes3Encrypt(xmlSecBufferGetData(&(ctx->keyBuffer)),
                                    XMLSEC_KW_DES3_KEY_LENGTH,
                                    iv, XMLSEC_KW_DES3_IV_LENGTH,
                                    in, inSize,
                                    out, outSize,
                                    0); /* decrypt */
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKWDes3Encrypt",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(ret);
}

static int
xmlSecGCryptKWDes3Encrypt(const xmlSecByte *key, xmlSecSize keySize,
                           const xmlSecByte *iv, xmlSecSize ivSize,
                           const xmlSecByte *in, xmlSecSize inSize,
                           xmlSecByte *out, xmlSecSize outSize,
                           int enc) {
    size_t key_len = gcry_cipher_get_algo_keylen(GCRY_CIPHER_3DES);
    size_t block_len = gcry_cipher_get_algo_blklen(GCRY_CIPHER_3DES);
    gcry_cipher_hd_t cipherCtx;
    gcry_error_t err;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(keySize >= key_len, -1);
    xmlSecAssert2(iv != NULL, -1);
    xmlSecAssert2(ivSize >= block_len, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= inSize, -1);

    err = gcry_cipher_open(&cipherCtx, GCRY_CIPHER_3DES, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_SECURE); /* we are paranoid */
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_open(GCRY_CIPHER_3DES)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    err = gcry_cipher_setkey(cipherCtx, key, keySize);
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_setkey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    err = gcry_cipher_setiv(cipherCtx, iv, ivSize);
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_setiv",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    if(enc) {
        err = gcry_cipher_encrypt(cipherCtx, out, outSize, in, inSize);
        if(err != GPG_ERR_NO_ERROR) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_cipher_encrypt",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            gcry_cipher_close(cipherCtx);
            return(-1);
        }
    } else {
        err = gcry_cipher_decrypt(cipherCtx, out, outSize, in, inSize);
        if(err != GPG_ERR_NO_ERROR) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_cipher_decrypt",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            gcry_cipher_close(cipherCtx);
            return(-1);
        }
    }

    /* done */
    gcry_cipher_close(cipherCtx);
    return((int)inSize); /* out size == in size */
}


#endif /* XMLSEC_NO_DES */


/**
 *
 * XMLSec library
 *
 * AES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef XMLSEC_NO_AES
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
 * AES KW implementation
 *
 *********************************************************************/
static int        xmlSecGCryptKWAesBlockEncrypt                 (const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize,
                                                                 void * context);
static int        xmlSecGCryptKWAesBlockDecrypt                 (const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize,
                                                                 void * context);
static xmlSecKWAesKlass xmlSecGCryptKWAesKlass = {
    /* callbacks */
    xmlSecGCryptKWAesBlockEncrypt,          /* xmlSecKWAesBlockEncryptMethod       encrypt; */
    xmlSecGCryptKWAesBlockDecrypt,          /* xmlSecKWAesBlockDecryptMethod       decrypt; */

    /* for the future */
    NULL,                                   /* void*                               reserved0; */
    NULL                                    /* void*                               reserved1; */
};


/*********************************************************************
 *
 * AES KW transforms
 *
 ********************************************************************/
typedef struct _xmlSecGCryptKWAesCtx              xmlSecGCryptKWAesCtx,
                                                  *xmlSecGCryptKWAesCtxPtr;
struct _xmlSecGCryptKWAesCtx {
    int                 cipher;
    int                 mode;
    int                 flags;
    xmlSecSize          blockSize;
    xmlSecSize          keyExpectedSize;

    xmlSecBuffer        keyBuffer;
};
#define xmlSecGCryptKWAesSize     \
    (sizeof(xmlSecTransform) + sizeof(xmlSecGCryptKWAesCtx))
#define xmlSecGCryptKWAesGetCtx(transform) \
    ((xmlSecGCryptKWAesCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))
#define xmlSecGCryptKWAesCheckId(transform) \
    (xmlSecTransformCheckId((transform), xmlSecGCryptTransformKWAes128Id) || \
     xmlSecTransformCheckId((transform), xmlSecGCryptTransformKWAes192Id) || \
     xmlSecTransformCheckId((transform), xmlSecGCryptTransformKWAes256Id))

static int      xmlSecGCryptKWAesInitialize                     (xmlSecTransformPtr transform);
static void     xmlSecGCryptKWAesFinalize                       (xmlSecTransformPtr transform);
static int      xmlSecGCryptKWAesSetKeyReq                      (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecGCryptKWAesSetKey                         (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecGCryptKWAesExecute                        (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);

static int
xmlSecGCryptKWAesInitialize(xmlSecTransformPtr transform) {
    xmlSecGCryptKWAesCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecGCryptKWAesCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWAesSize), -1);

    ctx = xmlSecGCryptKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWAes128Id)) {
        ctx->cipher             = GCRY_CIPHER_AES128;
        ctx->keyExpectedSize    = XMLSEC_KW_AES128_KEY_SIZE;
    } else if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWAes192Id)) {
        ctx->cipher             = GCRY_CIPHER_AES192;
        ctx->keyExpectedSize    = XMLSEC_KW_AES192_KEY_SIZE;
    } else if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformKWAes256Id)) {
        ctx->cipher             = GCRY_CIPHER_AES256;
        ctx->keyExpectedSize    = XMLSEC_KW_AES256_KEY_SIZE;
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    ctx->mode           = GCRY_CIPHER_MODE_CBC;
    ctx->flags          = GCRY_CIPHER_SECURE; /* we are paranoid */
    ctx->blockSize      = gcry_cipher_get_algo_blklen(ctx->cipher);
    xmlSecAssert2(ctx->blockSize > 0, -1);

    ret = xmlSecBufferInitialize(&(ctx->keyBuffer), 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecGCryptKWAesGetKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void
xmlSecGCryptKWAesFinalize(xmlSecTransformPtr transform) {
    xmlSecGCryptKWAesCtxPtr ctx;

    xmlSecAssert(xmlSecGCryptKWAesCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecGCryptKWAesSize));

    ctx = xmlSecGCryptKWAesGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    xmlSecBufferFinalize(&(ctx->keyBuffer));
}

static int
xmlSecGCryptKWAesSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecGCryptKWAesCtxPtr ctx;

    xmlSecAssert2(xmlSecGCryptKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWAesSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecGCryptKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId    = xmlSecGCryptKeyDataAesId;
    keyReq->keyType  = xmlSecKeyDataTypeSymmetric;
    if(transform->operation == xmlSecTransformOperationEncrypt) {
        keyReq->keyUsage = xmlSecKeyUsageEncrypt;
    } else {
        keyReq->keyUsage = xmlSecKeyUsageDecrypt;
    }
    keyReq->keyBitsSize = 8 * ctx->keyExpectedSize;

    return(0);
}

static int
xmlSecGCryptKWAesSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecGCryptKWAesCtxPtr ctx;
    xmlSecBufferPtr buffer;
    xmlSecSize keySize;
    int ret;

    xmlSecAssert2(xmlSecGCryptKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWAesSize), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecGCryptKeyDataAesId), -1);

    ctx = xmlSecGCryptKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(xmlSecKeyGetValue(key));
    xmlSecAssert2(buffer != NULL, -1);

    keySize = xmlSecBufferGetSize(buffer);
    if(keySize < ctx->keyExpectedSize) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA_SIZE,
                    "key=%d;expected=%d",
                    keySize, ctx->keyExpectedSize);
        return(-1);
    }

    ret = xmlSecBufferSetData(&(ctx->keyBuffer),
                            xmlSecBufferGetData(buffer),
                            ctx->keyExpectedSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferSetData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "expected-size=%d", 
                    ctx->keyExpectedSize);
        return(-1);
    }

    return(0);
}

static int
xmlSecGCryptKWAesExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecGCryptKWAesCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize, keySize;
    int ret;

    xmlSecAssert2(xmlSecGCryptKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptKWAesSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecGCryptKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keySize = xmlSecBufferGetSize(&(ctx->keyBuffer));
    xmlSecAssert2(keySize == ctx->keyExpectedSize, -1);

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
        if((inSize % 8) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_SIZE,
                        "size=%d(not 8 bytes aligned)", inSize);
            return(-1);
        }

        if(transform->operation == xmlSecTransformOperationEncrypt) {
            /* the encoded key might be 8 bytes longer plus 8 bytes just in case */
            outSize = inSize + XMLSEC_KW_AES_MAGIC_BLOCK_SIZE +
                               XMLSEC_KW_AES_BLOCK_SIZE;
        } else {
            outSize = inSize + XMLSEC_KW_AES_BLOCK_SIZE;
        }

        ret = xmlSecBufferSetMaxSize(out, outSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferSetMaxSize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "outSize=%d", outSize);
            return(-1);
        }

        if(transform->operation == xmlSecTransformOperationEncrypt) {
            ret = xmlSecKWAesEncode(&xmlSecGCryptKWAesKlass, ctx,
                                    xmlSecBufferGetData(in), inSize,
                                    xmlSecBufferGetData(out), outSize);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecKWAesEncode",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            outSize = ret;
        } else {
            ret = xmlSecKWAesDecode(&xmlSecGCryptKWAesKlass, ctx,
                                    xmlSecBufferGetData(in), inSize,
                                    xmlSecBufferGetData(out), outSize);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecKWAesEncode",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
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
                        "outSize=%d", outSize);
            return(-1);
        }

        ret = xmlSecBufferRemoveHead(in, inSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferRemoveHead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "inSize%d", inSize);
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


static xmlSecTransformKlass xmlSecGCryptKWAes128Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptKWAesSize,                      /* xmlSecSize objSize */

    xmlSecNameKWAes128,                         /* const xmlChar* name; */
    xmlSecHrefKWAes128,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecGCryptKWAesInitialize,                /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptKWAesFinalize,                  /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptKWAesSetKeyReq,                 /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecGCryptKWAesSetKey,                    /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptKWAesExecute,                   /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformKWAes128GetKlass:
 *
 * The AES-128 kew wrapper transform klass.
 *
 * Returns: AES-128 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformKWAes128GetKlass(void) {
    return(&xmlSecGCryptKWAes128Klass);
}

static xmlSecTransformKlass xmlSecGCryptKWAes192Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptKWAesSize,                      /* xmlSecSize objSize */

    xmlSecNameKWAes192,                         /* const xmlChar* name; */
    xmlSecHrefKWAes192,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecGCryptKWAesInitialize,                /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptKWAesFinalize,                  /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptKWAesSetKeyReq,                 /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecGCryptKWAesSetKey,                    /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptKWAesExecute,                   /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};


/**
 * xmlSecGCryptTransformKWAes192GetKlass:
 *
 * The AES-192 kew wrapper transform klass.
 *
 * Returns: AES-192 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformKWAes192GetKlass(void) {
    return(&xmlSecGCryptKWAes192Klass);
}

static xmlSecTransformKlass xmlSecGCryptKWAes256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptKWAesSize,                      /* xmlSecSize objSize */

    xmlSecNameKWAes256,                         /* const xmlChar* name; */
    xmlSecHrefKWAes256,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecGCryptKWAesInitialize,                /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptKWAesFinalize,                  /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptKWAesSetKeyReq,                 /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecGCryptKWAesSetKey,                    /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptKWAesExecute,                   /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformKWAes256GetKlass:
 *
 * The AES-256 kew wrapper transform klass.
 *
 * Returns: AES-256 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformKWAes256GetKlass(void) {
    return(&xmlSecGCryptKWAes256Klass);
}

/*********************************************************************
 *
 * AES KW implementation
 *
 *********************************************************************/
static unsigned char g_zero_iv[XMLSEC_KW_AES_BLOCK_SIZE] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static int
xmlSecGCryptKWAesBlockEncrypt(const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize,
                               void * context) {
    xmlSecGCryptKWAesCtxPtr ctx = (xmlSecGCryptKWAesCtxPtr)context;
    gcry_cipher_hd_t cipherCtx;
    gcry_error_t err;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize >= ctx->blockSize, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= ctx->blockSize, -1);

    err = gcry_cipher_open(&cipherCtx, ctx->cipher, ctx->mode, ctx->flags); 
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_open",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    err = gcry_cipher_setkey(cipherCtx,
                             xmlSecBufferGetData(&ctx->keyBuffer),
                             xmlSecBufferGetSize(&ctx->keyBuffer));
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_setkey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    /* use zero IV and CBC mode to ensure we get result as-is */
    err = gcry_cipher_setiv(cipherCtx, g_zero_iv, sizeof(g_zero_iv));
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_setiv",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

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
    gcry_cipher_close(cipherCtx);

    return(ctx->blockSize);
}

static int
xmlSecGCryptKWAesBlockDecrypt(const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize,
                               void * context) {
    xmlSecGCryptKWAesCtxPtr ctx = (xmlSecGCryptKWAesCtxPtr)context;
    gcry_cipher_hd_t cipherCtx;
    gcry_error_t err;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize >= ctx->blockSize, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= ctx->blockSize, -1);

    err = gcry_cipher_open(&cipherCtx, ctx->cipher, ctx->mode, ctx->flags);
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_open",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    err = gcry_cipher_setkey(cipherCtx,
                             xmlSecBufferGetData(&ctx->keyBuffer),
                             xmlSecBufferGetSize(&ctx->keyBuffer));
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_setkey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    /* use zero IV and CBC mode to ensure we get result as-is */
    err = gcry_cipher_setiv(cipherCtx, g_zero_iv, sizeof(g_zero_iv));
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_cipher_setiv",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

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
    gcry_cipher_close(cipherCtx);

    return(ctx->blockSize);
}

#endif /* XMLSEC_NO_AES */

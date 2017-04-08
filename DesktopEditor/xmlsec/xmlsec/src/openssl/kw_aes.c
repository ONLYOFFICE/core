/**
 *
 * XMLSec library
 *
 * AES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef XMLSEC_NO_AES
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <openssl/aes.h>
#include <openssl/rand.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/openssl/crypto.h>

#include "../kw_aes_des.h"


/*********************************************************************
 *
 * AES KW implementation
 *
 *********************************************************************/
static int        xmlSecOpenSSLKWAesBlockEncrypt                (const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize,
                                                                 void * context);
static int        xmlSecOpenSSLKWAesBlockDecrypt                (const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize,
                                                                 void * context);
static xmlSecKWAesKlass xmlSecOpenSSLKWAesKlass = {
    /* callbacks */
    xmlSecOpenSSLKWAesBlockEncrypt,         /* xmlSecKWAesBlockEncryptMethod       encrypt; */
    xmlSecOpenSSLKWAesBlockDecrypt,         /* xmlSecKWAesBlockDecryptMethod       decrypt; */

    /* for the future */
    NULL,                                   /* void*                               reserved0; */
    NULL                                    /* void*                               reserved1; */
};


/*********************************************************************
 *
 * AES KW transforms
 *
 ********************************************************************/
typedef struct _xmlSecOpenSSLKWAesCtx              xmlSecOpenSSLKWAesCtx,
                                                  *xmlSecOpenSSLKWAesCtxPtr;
struct _xmlSecOpenSSLKWAesCtx {
    xmlSecBuffer        keyBuffer;
    xmlSecSize          keyExpectedSize;
};
#define xmlSecOpenSSLKWAesSize     \
    (sizeof(xmlSecTransform) + sizeof(xmlSecOpenSSLKWAesCtx))
#define xmlSecOpenSSLKWAesGetCtx(transform) \
    ((xmlSecOpenSSLKWAesCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))
#define xmlSecOpenSSLKWAesCheckId(transform) \
    (xmlSecTransformCheckId((transform), xmlSecOpenSSLTransformKWAes128Id) || \
     xmlSecTransformCheckId((transform), xmlSecOpenSSLTransformKWAes192Id) || \
     xmlSecTransformCheckId((transform), xmlSecOpenSSLTransformKWAes256Id))

static int      xmlSecOpenSSLKWAesInitialize                    (xmlSecTransformPtr transform);
static void     xmlSecOpenSSLKWAesFinalize                      (xmlSecTransformPtr transform);
static int      xmlSecOpenSSLKWAesSetKeyReq                     (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecOpenSSLKWAesSetKey                        (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecOpenSSLKWAesExecute                       (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);

static int
xmlSecOpenSSLKWAesInitialize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLKWAesCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecOpenSSLKWAesCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLKWAesSize), -1);

    ctx = xmlSecOpenSSLKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformKWAes128Id)) {
        ctx->keyExpectedSize = XMLSEC_KW_AES128_KEY_SIZE;
    } else if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformKWAes192Id)) {
        ctx->keyExpectedSize = XMLSEC_KW_AES192_KEY_SIZE;
    } else if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformKWAes256Id)) {
        ctx->keyExpectedSize = XMLSEC_KW_AES256_KEY_SIZE;
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecBufferInitialize(&(ctx->keyBuffer), 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecOpenSSLKWAesGetKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void
xmlSecOpenSSLKWAesFinalize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLKWAesCtxPtr ctx;

    xmlSecAssert(xmlSecOpenSSLKWAesCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecOpenSSLKWAesSize));

    ctx = xmlSecOpenSSLKWAesGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    xmlSecBufferFinalize(&(ctx->keyBuffer));
}

static int
xmlSecOpenSSLKWAesSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecOpenSSLKWAesCtxPtr ctx;

    xmlSecAssert2(xmlSecOpenSSLKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLKWAesSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecOpenSSLKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId    = xmlSecOpenSSLKeyDataAesId;
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
xmlSecOpenSSLKWAesSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecOpenSSLKWAesCtxPtr ctx;
    xmlSecBufferPtr buffer;
    xmlSecSize keySize;
    int ret;

    xmlSecAssert2(xmlSecOpenSSLKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLKWAesSize), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecOpenSSLKeyDataAesId), -1);

    ctx = xmlSecOpenSSLKWAesGetCtx(transform);
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
xmlSecOpenSSLKWAesExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLKWAesCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize, keySize;
    AES_KEY aesKey;
    int ret;

    xmlSecAssert2(xmlSecOpenSSLKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLKWAesSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLKWAesGetCtx(transform);
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
            /* prepare key */
            ret = AES_set_encrypt_key(xmlSecBufferGetData(&(ctx->keyBuffer)),
                                      8 * keySize,
                                      &aesKey);
            if(ret != 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "AES_set_decrypt_key",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            ret = xmlSecKWAesEncode(&xmlSecOpenSSLKWAesKlass, &aesKey,
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
            /* prepare key */
            ret = AES_set_decrypt_key(xmlSecBufferGetData(&(ctx->keyBuffer)),
                                      8 * keySize,
                                      &aesKey);
            if(ret != 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "AES_set_decrypt_key",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            ret = xmlSecKWAesDecode(&xmlSecOpenSSLKWAesKlass, &aesKey,
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

static xmlSecTransformKlass xmlSecOpenSSLKWAes128Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLKWAesSize,                     /* xmlSecSize objSize */

    xmlSecNameKWAes128,                         /* const xmlChar* name; */
    xmlSecHrefKWAes128,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLKWAesInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLKWAesFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLKWAesSetKeyReq,                /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLKWAesSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLKWAesExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformKWAes128GetKlass:
 *
 * The AES-128 kew wrapper transform klass.
 *
 * Returns: AES-128 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformKWAes128GetKlass(void) {
    return(&xmlSecOpenSSLKWAes128Klass);
}

static xmlSecTransformKlass xmlSecOpenSSLKWAes192Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLKWAesSize,                     /* xmlSecSize objSize */

    xmlSecNameKWAes192,                         /* const xmlChar* name; */
    xmlSecHrefKWAes192,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLKWAesInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLKWAesFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLKWAesSetKeyReq,                /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLKWAesSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLKWAesExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};


/**
 * xmlSecOpenSSLTransformKWAes192GetKlass:
 *
 * The AES-192 kew wrapper transform klass.
 *
 * Returns: AES-192 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformKWAes192GetKlass(void) {
    return(&xmlSecOpenSSLKWAes192Klass);
}

static xmlSecTransformKlass xmlSecOpenSSLKWAes256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLKWAesSize,                     /* xmlSecSize objSize */

    xmlSecNameKWAes256,                         /* const xmlChar* name; */
    xmlSecHrefKWAes256,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLKWAesInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLKWAesFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLKWAesSetKeyReq,                /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLKWAesSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLKWAesExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformKWAes256GetKlass:
 *
 * The AES-256 kew wrapper transform klass.
 *
 * Returns: AES-256 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformKWAes256GetKlass(void) {
    return(&xmlSecOpenSSLKWAes256Klass);
}

/*********************************************************************
 *
 * AES KW implementation
 *
 *********************************************************************/
static int
xmlSecOpenSSLKWAesBlockEncrypt(const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize,
                               void * context) {
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize >= AES_BLOCK_SIZE, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= AES_BLOCK_SIZE, -1);
    xmlSecAssert2(context != NULL, -1);

    AES_encrypt(in, out, (AES_KEY*)context);
    return(AES_BLOCK_SIZE);
}

static int
xmlSecOpenSSLKWAesBlockDecrypt(const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize,
                               void * context) {
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize >= AES_BLOCK_SIZE, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= AES_BLOCK_SIZE, -1);
    xmlSecAssert2(context != NULL, -1);

    AES_decrypt(in, out, (AES_KEY*)context);
    return(AES_BLOCK_SIZE);
}

#endif /* XMLSEC_NO_AES */

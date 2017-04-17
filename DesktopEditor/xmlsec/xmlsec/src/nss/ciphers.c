/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#include "globals.h"

#include <string.h>

#include <nspr.h>
#include <nss.h>
#include <secoid.h>
#include <pk11func.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/crypto.h>

#define XMLSEC_NSS_MAX_KEY_SIZE         32
#define XMLSEC_NSS_MAX_IV_SIZE          32
#define XMLSEC_NSS_MAX_BLOCK_SIZE       32

/**************************************************************************
 *
 * Internal Nss Block cipher CTX
 *
 *****************************************************************************/
typedef struct _xmlSecNssBlockCipherCtx         xmlSecNssBlockCipherCtx,
                                                        *xmlSecNssBlockCipherCtxPtr;
struct _xmlSecNssBlockCipherCtx {
    CK_MECHANISM_TYPE   cipher;
    PK11Context*        cipherCtx;
    xmlSecKeyDataId     keyId;
    int                 keyInitialized;
    int                 ctxInitialized;
    xmlSecByte          key[XMLSEC_NSS_MAX_KEY_SIZE];
    xmlSecSize          keySize;
    xmlSecByte          iv[XMLSEC_NSS_MAX_IV_SIZE];
    xmlSecSize          ivSize;
};
static int      xmlSecNssBlockCipherCtxInit             (xmlSecNssBlockCipherCtxPtr ctx,
                                                         xmlSecBufferPtr in,
                                                         xmlSecBufferPtr out,
                                                         int encrypt,
                                                         const xmlChar* cipherName,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecNssBlockCipherCtxUpdate   (xmlSecNssBlockCipherCtxPtr ctx,
                                                         xmlSecBufferPtr in,
                                                         xmlSecBufferPtr out,
                                                         int encrypt,
                                                         const xmlChar* cipherName,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecNssBlockCipherCtxFinal            (xmlSecNssBlockCipherCtxPtr ctx,
                                                         xmlSecBufferPtr in,
                                                         xmlSecBufferPtr out,
                                                         int encrypt,
                                                         const xmlChar* cipherName,
                                                         xmlSecTransformCtxPtr transformCtx);
static int
xmlSecNssBlockCipherCtxInit(xmlSecNssBlockCipherCtxPtr ctx,
                                xmlSecBufferPtr in, xmlSecBufferPtr out,
                                int encrypt,
                                const xmlChar* cipherName,
                                xmlSecTransformCtxPtr transformCtx) {
    SECItem keyItem;
    SECItem ivItem;
    PK11SlotInfo* slot;
    PK11SymKey* symKey;
    int ivLen;
    SECStatus rv;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != 0, -1);
    xmlSecAssert2(ctx->cipherCtx == NULL, -1);
    xmlSecAssert2(ctx->keyInitialized != 0, -1);
    xmlSecAssert2(ctx->ctxInitialized == 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ivLen = PK11_GetIVLength(ctx->cipher);
    xmlSecAssert2(ivLen > 0, -1);
    xmlSecAssert2((xmlSecSize)ivLen <= sizeof(ctx->iv), -1);

    if(encrypt) {
        /* generate random iv */
        rv = PK11_GenerateRandom(ctx->iv, ivLen);
        if(rv != SECSuccess) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "PK11_GenerateRandom",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "size=%d", ivLen);
            return(-1);
        }

        /* write iv to the output */
        ret = xmlSecBufferAppend(out, ctx->iv, ivLen);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "xmlSecBufferAppend",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", ivLen);
            return(-1);
        }

    } else {
        /* if we don't have enough data, exit and hope that
         * we'll have iv next time */
        if(xmlSecBufferGetSize(in) < (xmlSecSize)ivLen) {
            return(0);
        }

        /* copy iv to our buffer*/
        xmlSecAssert2(xmlSecBufferGetData(in) != NULL, -1);
        memcpy(ctx->iv, xmlSecBufferGetData(in), ivLen);

        /* and remove from input */
        ret = xmlSecBufferRemoveHead(in, ivLen);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "xmlSecBufferRemoveHead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", ivLen);
            return(-1);
        }
    }

    memset(&keyItem, 0, sizeof(keyItem));
    keyItem.data = ctx->key;
    keyItem.len  = ctx->keySize;
    memset(&ivItem, 0, sizeof(ivItem));
    ivItem.data = ctx->iv;
    ivItem.len  = ctx->ivSize;

    slot = PK11_GetBestSlot(ctx->cipher, NULL);
    if(slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "PK11_GetBestSlot",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    symKey = PK11_ImportSymKey(slot, ctx->cipher, PK11_OriginDerive,
                               CKA_SIGN, &keyItem, NULL);
    if(symKey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "PK11_ImportSymKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        PK11_FreeSlot(slot);
        return(-1);
    }

    ctx->cipherCtx = PK11_CreateContextBySymKey(ctx->cipher,
                        (encrypt) ? CKA_ENCRYPT : CKA_DECRYPT,
                        symKey, &ivItem);
    if(ctx->cipherCtx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "PK11_CreateContextBySymKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        PK11_FreeSymKey(symKey);
        PK11_FreeSlot(slot);
        return(-1);
    }

    ctx->ctxInitialized = 1;
    PK11_FreeSymKey(symKey);
    PK11_FreeSlot(slot);
    return(0);
}

static int
xmlSecNssBlockCipherCtxUpdate(xmlSecNssBlockCipherCtxPtr ctx,
                                  xmlSecBufferPtr in, xmlSecBufferPtr out,
                                  int encrypt,
                                  const xmlChar* cipherName,
                                  xmlSecTransformCtxPtr transformCtx) {
    xmlSecSize inSize, inBlocks, outSize;
    int blockLen;
    int outLen = 0;
    xmlSecByte* outBuf;
    SECStatus rv;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != 0, -1);
    xmlSecAssert2(ctx->cipherCtx != NULL, -1);
    xmlSecAssert2(ctx->ctxInitialized != 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    blockLen = PK11_GetBlockSize(ctx->cipher, NULL);
    xmlSecAssert2(blockLen > 0, -1);

    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);

    if(inSize < (xmlSecSize)blockLen) {
        return(0);
    }

    if(encrypt) {
        inBlocks = inSize / ((xmlSecSize)blockLen);
    } else {
        /* we want to have the last block in the input buffer
         * for padding check */
        inBlocks = (inSize - 1) / ((xmlSecSize)blockLen);
    }
    inSize = inBlocks * ((xmlSecSize)blockLen);

    /* we write out the input size plus may be one block */
    ret = xmlSecBufferSetMaxSize(out, outSize + inSize + blockLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferSetMaxSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", outSize + inSize + blockLen);
        return(-1);
    }
    outBuf = xmlSecBufferGetData(out) + outSize;

    rv = PK11_CipherOp(ctx->cipherCtx, outBuf, &outLen, inSize + blockLen,
                        xmlSecBufferGetData(in), inSize);
    if(rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "PK11_CipherOp",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2((xmlSecSize)outLen == inSize, -1);

    /* set correct output buffer size */
    ret = xmlSecBufferSetSize(out, outSize + outLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferSetSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", outSize + outLen);
        return(-1);
    }

    /* remove the processed block from input */
    ret = xmlSecBufferRemoveHead(in, inSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferRemoveHead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", inSize);
        return(-1);
    }
    return(0);
}

static int
xmlSecNssBlockCipherCtxFinal(xmlSecNssBlockCipherCtxPtr ctx,
                                 xmlSecBufferPtr in,
                                 xmlSecBufferPtr out,
                                 int encrypt,
                                 const xmlChar* cipherName,
                                 xmlSecTransformCtxPtr transformCtx) {
    xmlSecSize inSize, outSize;
    int blockLen, outLen = 0;
    xmlSecByte* inBuf;
    xmlSecByte* outBuf;
    SECStatus rv;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != 0, -1);
    xmlSecAssert2(ctx->cipherCtx != NULL, -1);
    xmlSecAssert2(ctx->ctxInitialized != 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    blockLen = PK11_GetBlockSize(ctx->cipher, NULL);
    xmlSecAssert2(blockLen > 0, -1);

    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);

    if(encrypt != 0) {
        xmlSecAssert2(inSize < (xmlSecSize)blockLen, -1);

        /* create padding */
        ret = xmlSecBufferSetMaxSize(in, blockLen);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "xmlSecBufferSetMaxSize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", blockLen);
            return(-1);
        }
        inBuf = xmlSecBufferGetData(in);

        /* generate random padding */
        if((xmlSecSize)blockLen > (inSize + 1)) {
            rv = PK11_GenerateRandom(inBuf + inSize, blockLen - inSize - 1);
            if(rv != SECSuccess) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(cipherName),
                            "PK11_GenerateRandom",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "size=%d", blockLen - inSize - 1);
                return(-1);
            }
        }
        inBuf[blockLen - 1] = blockLen - inSize;
        inSize = blockLen;
    } else {
        if(inSize != (xmlSecSize)blockLen) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_DATA,
                        "data=%d;block=%d", inSize, blockLen);
            return(-1);
        }
    }

    /* process last block */
    ret = xmlSecBufferSetMaxSize(out, outSize + 2 * blockLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferSetMaxSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", outSize + 2 * blockLen);
        return(-1);
    }
    outBuf = xmlSecBufferGetData(out) + outSize;

    rv = PK11_CipherOp(ctx->cipherCtx, outBuf, &outLen, 2 * blockLen,
                        xmlSecBufferGetData(in), inSize);
    if(rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "PK11_CipherOp",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2((xmlSecSize)outLen == inSize, -1);

    if(encrypt == 0) {
        /* check padding */
        if(outLen < outBuf[blockLen - 1]) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_DATA,
                        "padding=%d;buffer=%d",
                        outBuf[blockLen - 1], outLen);
            return(-1);
        }
        outLen -= outBuf[blockLen - 1];
    }

    /* set correct output buffer size */
    ret = xmlSecBufferSetSize(out, outSize + outLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferSetSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", outSize + outLen);
        return(-1);
    }

    /* remove the processed block from input */
    ret = xmlSecBufferRemoveHead(in, inSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferRemoveHead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", inSize);
        return(-1);
    }

    return(0);
}


/******************************************************************************
 *
 * EVP Block Cipher transforms
 *
 * xmlSecNssBlockCipherCtx block is located after xmlSecTransform structure
 *
 *****************************************************************************/
#define xmlSecNssBlockCipherSize        \
    (sizeof(xmlSecTransform) + sizeof(xmlSecNssBlockCipherCtx))
#define xmlSecNssBlockCipherGetCtx(transform) \
    ((xmlSecNssBlockCipherCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecNssBlockCipherInitialize  (xmlSecTransformPtr transform);
static void     xmlSecNssBlockCipherFinalize            (xmlSecTransformPtr transform);
static int      xmlSecNssBlockCipherSetKeyReq   (xmlSecTransformPtr transform,
                                                         xmlSecKeyReqPtr keyReq);
static int      xmlSecNssBlockCipherSetKey              (xmlSecTransformPtr transform,
                                                         xmlSecKeyPtr key);
static int      xmlSecNssBlockCipherExecute             (xmlSecTransformPtr transform,
                                                         int last,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecNssBlockCipherCheckId             (xmlSecTransformPtr transform);



static int
xmlSecNssBlockCipherCheckId(xmlSecTransformPtr transform) {
#ifndef XMLSEC_NO_DES
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformDes3CbcId)) {
        return(1);
    }
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformAes128CbcId) ||
       xmlSecTransformCheckId(transform, xmlSecNssTransformAes192CbcId) ||
       xmlSecTransformCheckId(transform, xmlSecNssTransformAes256CbcId)) {

       return(1);
    }
#endif /* XMLSEC_NO_AES */

    return(0);
}

static int
xmlSecNssBlockCipherInitialize(xmlSecTransformPtr transform) {
    xmlSecNssBlockCipherCtxPtr ctx;

    xmlSecAssert2(xmlSecNssBlockCipherCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssBlockCipherSize), -1);

    ctx = xmlSecNssBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecNssBlockCipherCtx));

#ifndef XMLSEC_NO_DES
    if(transform->id == xmlSecNssTransformDes3CbcId) {
        ctx->cipher     = CKM_DES3_CBC;
        ctx->keyId      = xmlSecNssKeyDataDesId;
        ctx->keySize    = 24;
    } else
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(transform->id == xmlSecNssTransformAes128CbcId) {
        ctx->cipher     = CKM_AES_CBC;
        ctx->keyId      = xmlSecNssKeyDataAesId;
        ctx->keySize    = 16;
    } else if(transform->id == xmlSecNssTransformAes192CbcId) {
        ctx->cipher     = CKM_AES_CBC;
        ctx->keyId      = xmlSecNssKeyDataAesId;
        ctx->keySize    = 24;
    } else if(transform->id == xmlSecNssTransformAes256CbcId) {
        ctx->cipher     = CKM_AES_CBC;
        ctx->keyId      = xmlSecNssKeyDataAesId;
        ctx->keySize    = 32;
    } else
#endif /* XMLSEC_NO_AES */

    if(1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void
xmlSecNssBlockCipherFinalize(xmlSecTransformPtr transform) {
    xmlSecNssBlockCipherCtxPtr ctx;

    xmlSecAssert(xmlSecNssBlockCipherCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecNssBlockCipherSize));

    ctx = xmlSecNssBlockCipherGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->cipherCtx != NULL) {
        PK11_DestroyContext(ctx->cipherCtx, PR_TRUE);
    }

    memset(ctx, 0, sizeof(xmlSecNssBlockCipherCtx));
}

static int
xmlSecNssBlockCipherSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecNssBlockCipherCtxPtr ctx;

    xmlSecAssert2(xmlSecNssBlockCipherCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssBlockCipherSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecNssBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);

    keyReq->keyId       = ctx->keyId;
    keyReq->keyType     = xmlSecKeyDataTypeSymmetric;
    if(transform->operation == xmlSecTransformOperationEncrypt) {
        keyReq->keyUsage = xmlSecKeyUsageEncrypt;
    } else {
        keyReq->keyUsage = xmlSecKeyUsageDecrypt;
    }
    keyReq->keyBitsSize = 8 * ctx->keySize;
    return(0);
}

static int
xmlSecNssBlockCipherSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecNssBlockCipherCtxPtr ctx;
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecNssBlockCipherCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssBlockCipherSize), -1);
    xmlSecAssert2(key != NULL, -1);

    ctx = xmlSecNssBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != 0, -1);
    xmlSecAssert2(ctx->keyInitialized == 0, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);
    xmlSecAssert2(xmlSecKeyCheckId(key, ctx->keyId), -1);

    xmlSecAssert2(ctx->keySize > 0, -1);
    xmlSecAssert2(ctx->keySize <= sizeof(ctx->key), -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(xmlSecKeyGetValue(key));
    xmlSecAssert2(buffer != NULL, -1);

    if(xmlSecBufferGetSize(buffer) < ctx->keySize) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA_SIZE,
                    "keySize=%d;expected=%d",
                    xmlSecBufferGetSize(buffer), ctx->keySize);
        return(-1);
    }

    xmlSecAssert2(xmlSecBufferGetData(buffer) != NULL, -1);
    memcpy(ctx->key, xmlSecBufferGetData(buffer), ctx->keySize);

    ctx->keyInitialized = 1;
    return(0);
}

static int
xmlSecNssBlockCipherExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecNssBlockCipherCtxPtr ctx;
    xmlSecBufferPtr in, out;
    int ret;

    xmlSecAssert2(xmlSecNssBlockCipherCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssBlockCipherSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);

    ctx = xmlSecNssBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if(transform->status == xmlSecTransformStatusWorking) {
        if(ctx->ctxInitialized == 0) {
            ret = xmlSecNssBlockCipherCtxInit(ctx, in, out,
                        (transform->operation == xmlSecTransformOperationEncrypt) ? 1 : 0,
                        xmlSecTransformGetName(transform), transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecNssBlockCipherCtxInit",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
        }
        if((ctx->ctxInitialized == 0) && (last != 0)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_DATA,
                        "not enough data to initialize transform");
            return(-1);
        }

        if(ctx->ctxInitialized != 0) {
            ret = xmlSecNssBlockCipherCtxUpdate(ctx, in, out,
                        (transform->operation == xmlSecTransformOperationEncrypt) ? 1 : 0,
                        xmlSecTransformGetName(transform), transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecNssBlockCipherCtxUpdate",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
        }

        if(last) {
            ret = xmlSecNssBlockCipherCtxFinal(ctx, in, out,
                        (transform->operation == xmlSecTransformOperationEncrypt) ? 1 : 0,
                        xmlSecTransformGetName(transform), transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecNssBlockCipherCtxFinal",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            transform->status = xmlSecTransformStatusFinished;
        }
    } else if(transform->status == xmlSecTransformStatusFinished) {
        /* the only way we can get here is if there is no input */
        xmlSecAssert2(xmlSecBufferGetSize(in) == 0, -1);
    } else if(transform->status == xmlSecTransformStatusNone) {
        /* the only way we can get here is if there is no enough data in the input */
        xmlSecAssert2(last == 0, -1);
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


#ifndef XMLSEC_NO_AES
/*********************************************************************
 *
 * AES CBC cipher transforms
 *
 ********************************************************************/
static xmlSecTransformKlass xmlSecNssAes128CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssBlockCipherSize,           /* xmlSecSize objSize */

    xmlSecNameAes128Cbc,                        /* const xmlChar* name; */
    xmlSecHrefAes128Cbc,                        /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecNssBlockCipherInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssBlockCipherFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssBlockCipherSetKeyReq,              /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecNssBlockCipherSetKey,         /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssBlockCipherExecute,                /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformAes128CbcGetKlass:
 *
 * AES 128 CBC encryption transform klass.
 *
 * Returns: pointer to AES 128 CBC encryption transform.
 */
xmlSecTransformId
xmlSecNssTransformAes128CbcGetKlass(void) {
    return(&xmlSecNssAes128CbcKlass);
}

static xmlSecTransformKlass xmlSecNssAes192CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssBlockCipherSize,           /* xmlSecSize objSize */

    xmlSecNameAes192Cbc,                        /* const xmlChar* name; */
    xmlSecHrefAes192Cbc,                        /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecNssBlockCipherInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssBlockCipherFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssBlockCipherSetKeyReq,              /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecNssBlockCipherSetKey,         /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssBlockCipherExecute,                /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformAes192CbcGetKlass:
 *
 * AES 192 CBC encryption transform klass.
 *
 * Returns: pointer to AES 192 CBC encryption transform.
 */
xmlSecTransformId
xmlSecNssTransformAes192CbcGetKlass(void) {
    return(&xmlSecNssAes192CbcKlass);
}

static xmlSecTransformKlass xmlSecNssAes256CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssBlockCipherSize,           /* xmlSecSize objSize */

    xmlSecNameAes256Cbc,                        /* const xmlChar* name; */
    xmlSecHrefAes256Cbc,                        /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecNssBlockCipherInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssBlockCipherFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssBlockCipherSetKeyReq,              /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecNssBlockCipherSetKey,         /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssBlockCipherExecute,                /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformAes256CbcGetKlass:
 *
 * AES 256 CBC encryption transform klass.
 *
 * Returns: pointer to AES 256 CBC encryption transform.
 */
xmlSecTransformId
xmlSecNssTransformAes256CbcGetKlass(void) {
    return(&xmlSecNssAes256CbcKlass);
}

#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
static xmlSecTransformKlass xmlSecNssDes3CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssBlockCipherSize,           /* xmlSecSize objSize */

    xmlSecNameDes3Cbc,                          /* const xmlChar* name; */
    xmlSecHrefDes3Cbc,                          /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecNssBlockCipherInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssBlockCipherFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssBlockCipherSetKeyReq,              /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecNssBlockCipherSetKey,         /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssBlockCipherExecute,                /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformDes3CbcGetKlass:
 *
 * Triple DES CBC encryption transform klass.
 *
 * Returns: pointer to Triple DES encryption transform.
 */
xmlSecTransformId
xmlSecNssTransformDes3CbcGetKlass(void) {
    return(&xmlSecNssDes3CbcKlass);
}
#endif /* XMLSEC_NO_DES */


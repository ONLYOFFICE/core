/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/openssl/crypto.h>
#include <xmlsec/openssl/evp.h>

/* new API from OpenSSL 1.1.0 */
#if !defined(XMLSEC_OPENSSL_110)
#define EVP_CIPHER_CTX_encrypting(x) ((x)->encrypt)
#endif /* !defined(XMLSEC_OPENSSL_110) */


/**************************************************************************
 *
 * Internal OpenSSL Block cipher CTX
 *
 *****************************************************************************/
typedef struct _xmlSecOpenSSLEvpBlockCipherCtx          xmlSecOpenSSLEvpBlockCipherCtx,
                                                        *xmlSecOpenSSLEvpBlockCipherCtxPtr;
struct _xmlSecOpenSSLEvpBlockCipherCtx {
    const EVP_CIPHER*   cipher;
    xmlSecKeyDataId     keyId;
    EVP_CIPHER_CTX*     cipherCtx;
    int                 keyInitialized;
    int                 ctxInitialized;
    xmlSecByte          key[EVP_MAX_KEY_LENGTH];
    xmlSecByte          iv[EVP_MAX_IV_LENGTH];
    xmlSecByte          pad[2*EVP_MAX_BLOCK_LENGTH];
};

static int      xmlSecOpenSSLEvpBlockCipherCtxInit      (xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                                         xmlSecBufferPtr in,
                                                         xmlSecBufferPtr out,
                                                         int encrypt,
                                                         const xmlChar* cipherName,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock(xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                                         const xmlSecByte * in,
                                                         int inSize,
                                                         xmlSecBufferPtr out,
                                                         const xmlChar* cipherName,
                                                         int final);
static int      xmlSecOpenSSLEvpBlockCipherCtxUpdate    (xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                                         xmlSecBufferPtr in,
                                                         xmlSecBufferPtr out,
                                                         const xmlChar* cipherName,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLEvpBlockCipherCtxFinal     (xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                                         xmlSecBufferPtr in,
                                                         xmlSecBufferPtr out,
                                                         const xmlChar* cipherName,
                                                         xmlSecTransformCtxPtr transformCtx);
static int
xmlSecOpenSSLEvpBlockCipherCtxInit(xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                xmlSecBufferPtr in, xmlSecBufferPtr out,
                                int encrypt,
                                const xmlChar* cipherName,
                                xmlSecTransformCtxPtr transformCtx) {
    int ivLen;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != NULL, -1);
    xmlSecAssert2(ctx->cipherCtx != NULL, -1);
    xmlSecAssert2(ctx->keyInitialized != 0, -1);
    xmlSecAssert2(ctx->ctxInitialized == 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ivLen = EVP_CIPHER_iv_length(ctx->cipher);
    xmlSecAssert2(ivLen > 0, -1);
    xmlSecAssert2((xmlSecSize)ivLen <= sizeof(ctx->iv), -1);

    if(encrypt) {
        /* generate random iv */
        ret = RAND_bytes(ctx->iv, ivLen);
        if(ret != 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "RAND_bytes",
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

    /* set iv */
    ret = EVP_CipherInit(ctx->cipherCtx, ctx->cipher, ctx->key, ctx->iv, encrypt);
    if(ret != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "EVP_CipherInit",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ctx->ctxInitialized = 1;

    /*
     * The padding used in XML Enc does not follow RFC 1423
     * and is not supported by OpenSSL. However, it is possible
     * to disable padding and do it by yourself
     *
     * https://www.w3.org/TR/2002/REC-xmlenc-core-20021210/Overview.html#sec-Alg-Block
     */
    EVP_CIPHER_CTX_set_padding(ctx->cipherCtx, 0);

    return(0);
}

static int
xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock(xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                        const xmlSecByte * in,
                                        int inSize,
                                        xmlSecBufferPtr out,
                                        const xmlChar* cipherName,
                                        int final) {
    xmlSecByte* outBuf;
    xmlSecSize outSize;
    int blockLen, outLen = 0;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != NULL, -1);
    xmlSecAssert2(ctx->cipherCtx != NULL, -1);
    xmlSecAssert2(ctx->keyInitialized != 0, -1);
    xmlSecAssert2(ctx->ctxInitialized != 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);

    /* OpenSSL docs: If the pad parameter is zero then no padding is performed, the total amount of
     * data encrypted or decrypted must then be a multiple of the block size or an error will occur.
     */
    blockLen = EVP_CIPHER_block_size(ctx->cipher);
    xmlSecAssert2(blockLen > 0, -1);
    xmlSecAssert2((inSize % blockLen) == 0, -1);

    /* prepare: ensure we have enough space (+blockLen for final) */
    outSize = xmlSecBufferGetSize(out);
    ret = xmlSecBufferSetMaxSize(out, outSize + inSize + blockLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferSetMaxSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", (int)(outSize + inSize + blockLen));
        return(-1);
    }
    outBuf  = xmlSecBufferGetData(out) + outSize;

    /* encrypt/decrypt */
    ret = EVP_CipherUpdate(ctx->cipherCtx, outBuf, &outLen, in, inSize);
    if(ret != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "EVP_CipherUpdate",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2(outLen == inSize, -1);

    /* finalize transform if needed */
    if(final != 0) {
        int outLen2 = 0;

        ret = EVP_CipherFinal(ctx->cipherCtx, outBuf + outLen, &outLen2);
        if(ret != 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "EVP_CipherFinal",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        outLen += outLen2;
    }

    /* set correct output buffer size */
    ret = xmlSecBufferSetSize(out, outSize + outLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferSetSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", (int)(outSize + outLen));
        return(-1);
    }

    /* done */
    return (0);
}

static int
xmlSecOpenSSLEvpBlockCipherCtxUpdate(xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                  xmlSecBufferPtr in, xmlSecBufferPtr out,
                                  const xmlChar* cipherName,
                                  xmlSecTransformCtxPtr transformCtx) {
    xmlSecSize inSize, blockLen, inBlocksLen;
    xmlSecByte* inBuf;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipherCtx != NULL, -1);
    xmlSecAssert2(ctx->keyInitialized != 0, -1);
    xmlSecAssert2(ctx->ctxInitialized != 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    blockLen = EVP_CIPHER_block_size(ctx->cipher);
    xmlSecAssert2(blockLen > 0, -1);

    inSize = xmlSecBufferGetSize(in);
    if(inSize <= blockLen) {
        /* wait for more data: we want to make sure we keep the last chunk in tmp buffer for
         * padding check/removal on decryption
         */
        return(0);
    }

    /* OpenSSL docs: If the pad parameter is zero then no padding is performed, the total amount of
     * data encrypted or decrypted must then be a multiple of the block size or an error will occur.
     *
     * We process all complete blocks from the input
     */
    inBlocksLen = blockLen * (inSize / blockLen);
    if(inBlocksLen == inSize) {
        inBlocksLen -= blockLen; /* ensure we keep the last block around for Final() call to add/check/remove padding */
    }
    xmlSecAssert2(inBlocksLen > 0, -1);

    inBuf  = xmlSecBufferGetData(in);
    ret = xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock(ctx, inBuf, inBlocksLen, out, cipherName, 0); /* not final */
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    NULL);
        return(-1);
    }

    /* remove the processed block from input */
    ret = xmlSecBufferRemoveHead(in, inBlocksLen);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferRemoveHead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", (int)inSize);
        return(-1);
    }

    /* just a double check */
    inSize = xmlSecBufferGetSize(in);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(inSize <= blockLen, -1);

    /* done */
    return(0);
}

static int
xmlSecOpenSSLEvpBlockCipherCtxFinal(xmlSecOpenSSLEvpBlockCipherCtxPtr ctx,
                                 xmlSecBufferPtr in,
                                 xmlSecBufferPtr out,
                                 const xmlChar* cipherName,
                                 xmlSecTransformCtxPtr transformCtx) {
    xmlSecSize inSize, outSize, blockLen;
    xmlSecByte* inBuf;
    xmlSecByte* outBuf;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != NULL, -1);
    xmlSecAssert2(ctx->cipherCtx != NULL, -1);
    xmlSecAssert2(ctx->keyInitialized != 0, -1);
    xmlSecAssert2(ctx->ctxInitialized != 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    blockLen = EVP_CIPHER_block_size(ctx->cipher);
    xmlSecAssert2(blockLen > 0, -1);
    xmlSecAssert2(blockLen <= EVP_MAX_BLOCK_LENGTH, -1);

    /* not more than one block left */
    inSize = xmlSecBufferGetSize(in);
    inBuf = xmlSecBufferGetData(in);
    xmlSecAssert2(inSize <= blockLen, -1);

    /*
     * The padding used in XML Enc does not follow RFC 1423
     * and is not supported by OpenSSL. However, it is possible
     * to disable padding and do it by yourself
     *
     * https://www.w3.org/TR/2002/REC-xmlenc-core-20021210/Overview.html#sec-Alg-Block
     */
    if(EVP_CIPHER_CTX_encrypting(ctx->cipherCtx)) {
        xmlSecSize padLen;

        /* figure out pad length, if it is 0 (i.e. inSize == blockLen) then set it to blockLen */
        padLen = blockLen - inSize;
        if(padLen == 0) {
            padLen = blockLen;
        }
        xmlSecAssert2(padLen > 0, -1);
        xmlSecAssert2(inSize + padLen <= sizeof(ctx->pad), -1);

        /* we can have inSize == 0 if there were no data at all, otherwise -- copy the data */
        if(inSize > 0) {
            memcpy(ctx->pad, inBuf, inSize);
        }

        /* generate random padding */
        if(padLen > 1) {
            ret = RAND_bytes(ctx->pad + inSize, padLen - 1);
            if(ret != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(cipherName),
                            "RAND_bytes",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "size=%d", (int)(padLen - 1));
                return(-1);
            }
        }

        /* set the last byte to the pad length */
        ctx->pad[inSize + padLen - 1] = padLen;

        /* update the last 1 or 2 blocks with padding */
        ret = xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock(ctx, ctx->pad, inSize + padLen, out, cipherName, 1); /* final */
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        NULL);
            return(-1);
        }
    } else {
        xmlSecSize padLen;

        /* update the last one block with padding */
        ret = xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock(ctx, inBuf, inSize, out, cipherName, 1); /* final */
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "xmlSecOpenSSLEvpBlockCipherCtxUpdateBlock",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        NULL);
            return(-1);
        }

        /* we expect at least one block in the output -- the one we just decrypted */
        outBuf = xmlSecBufferGetData(out);
        outSize = xmlSecBufferGetSize(out);
        if(outSize < blockLen) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_DATA,
                    "outSize=%d;blockLen=%d",
                    (int)outSize, (int)blockLen);
            return(-1);
        }

        /* get the pad length from the last byte */
        padLen = (xmlSecSize)(outBuf[outSize - 1]);
        if(padLen > blockLen) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_DATA,
                    "padLen=%d;blockLen=%d",
                    (int)padLen, (int)blockLen);
            return(-1);
        }
        xmlSecAssert2(padLen <= outSize, -1);

        /* remove the padding */
        ret = xmlSecBufferRemoveTail(out, padLen);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(cipherName),
                        "xmlSecBufferRemoveTail",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", (int)padLen);
            return(-1);
        }
    }

    /* remove the processed block from input */
    ret = xmlSecBufferRemoveHead(in, inSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(cipherName),
                    "xmlSecBufferRemoveHead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", (int)inSize);
        return(-1);
    }

    /* done */
    return(0);
}


/******************************************************************************
 *
 * EVP Block Cipher transforms
 *
 * xmlSecOpenSSLEvpBlockCipherCtx block is located after xmlSecTransform structure
 *
 *****************************************************************************/
#define xmlSecOpenSSLEvpBlockCipherSize \
    (sizeof(xmlSecTransform) + sizeof(xmlSecOpenSSLEvpBlockCipherCtx))
#define xmlSecOpenSSLEvpBlockCipherGetCtx(transform) \
    ((xmlSecOpenSSLEvpBlockCipherCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecOpenSSLEvpBlockCipherInitialize   (xmlSecTransformPtr transform);
static void     xmlSecOpenSSLEvpBlockCipherFinalize     (xmlSecTransformPtr transform);
static int      xmlSecOpenSSLEvpBlockCipherSetKeyReq    (xmlSecTransformPtr transform,
                                                         xmlSecKeyReqPtr keyReq);
static int      xmlSecOpenSSLEvpBlockCipherSetKey       (xmlSecTransformPtr transform,
                                                         xmlSecKeyPtr key);
static int      xmlSecOpenSSLEvpBlockCipherExecute      (xmlSecTransformPtr transform,
                                                         int last,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLEvpBlockCipherCheckId      (xmlSecTransformPtr transform);



static int
xmlSecOpenSSLEvpBlockCipherCheckId(xmlSecTransformPtr transform) {
#ifndef XMLSEC_NO_DES
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformDes3CbcId)) {
        return(1);
    }
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformAes128CbcId) ||
       xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformAes192CbcId) ||
       xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformAes256CbcId)) {

       return(1);
    }
#endif /* XMLSEC_NO_AES */

    return(0);
}

static int
xmlSecOpenSSLEvpBlockCipherInitialize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLEvpBlockCipherCtxPtr ctx;

    xmlSecAssert2(xmlSecOpenSSLEvpBlockCipherCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpBlockCipherSize), -1);

    ctx = xmlSecOpenSSLEvpBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecOpenSSLEvpBlockCipherCtx));

#ifndef XMLSEC_NO_DES
    if(transform->id == xmlSecOpenSSLTransformDes3CbcId) {
        ctx->cipher     = EVP_des_ede3_cbc();
        ctx->keyId      = xmlSecOpenSSLKeyDataDesId;
    } else
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(transform->id == xmlSecOpenSSLTransformAes128CbcId) {
        ctx->cipher     = EVP_aes_128_cbc();
        ctx->keyId      = xmlSecOpenSSLKeyDataAesId;
    } else if(transform->id == xmlSecOpenSSLTransformAes192CbcId) {
        ctx->cipher     = EVP_aes_192_cbc();
        ctx->keyId      = xmlSecOpenSSLKeyDataAesId;
    } else if(transform->id == xmlSecOpenSSLTransformAes256CbcId) {
        ctx->cipher     = EVP_aes_256_cbc();
        ctx->keyId      = xmlSecOpenSSLKeyDataAesId;
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

    /* create cipher ctx */
    ctx->cipherCtx = EVP_CIPHER_CTX_new();
    if(ctx->cipherCtx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "EVP_CIPHER_CTX_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* done */
    return(0);
}

static void
xmlSecOpenSSLEvpBlockCipherFinalize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLEvpBlockCipherCtxPtr ctx;

    xmlSecAssert(xmlSecOpenSSLEvpBlockCipherCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpBlockCipherSize));

    ctx = xmlSecOpenSSLEvpBlockCipherGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->cipherCtx != NULL) {
        EVP_CIPHER_CTX_free(ctx->cipherCtx);
    }

    memset(ctx, 0, sizeof(xmlSecOpenSSLEvpBlockCipherCtx));
}

static int
xmlSecOpenSSLEvpBlockCipherSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecOpenSSLEvpBlockCipherCtxPtr ctx;
    int cipherKeyLen;

    xmlSecAssert2(xmlSecOpenSSLEvpBlockCipherCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpBlockCipherSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecOpenSSLEvpBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != NULL, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);

    keyReq->keyId       = ctx->keyId;
    keyReq->keyType = xmlSecKeyDataTypeSymmetric;
    if(transform->operation == xmlSecTransformOperationEncrypt) {
        keyReq->keyUsage = xmlSecKeyUsageEncrypt;
    } else {
        keyReq->keyUsage = xmlSecKeyUsageDecrypt;
    }

    cipherKeyLen = EVP_CIPHER_key_length(ctx->cipher);
    xmlSecAssert2(cipherKeyLen > 0, -1);

    keyReq->keyBitsSize = (xmlSecSize)(8 * cipherKeyLen);
    return(0);
}

static int
xmlSecOpenSSLEvpBlockCipherSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecOpenSSLEvpBlockCipherCtxPtr ctx;
    xmlSecBufferPtr buffer;
    int cipherKeyLen;

    xmlSecAssert2(xmlSecOpenSSLEvpBlockCipherCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpBlockCipherSize), -1);
    xmlSecAssert2(key != NULL, -1);

    ctx = xmlSecOpenSSLEvpBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cipher != NULL, -1);
    xmlSecAssert2(ctx->keyInitialized == 0, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);
    xmlSecAssert2(xmlSecKeyCheckId(key, ctx->keyId), -1);

    cipherKeyLen = EVP_CIPHER_key_length(ctx->cipher);
    xmlSecAssert2(cipherKeyLen > 0, -1);
    xmlSecAssert2((xmlSecSize)cipherKeyLen <= sizeof(ctx->key), -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(xmlSecKeyGetValue(key));
    xmlSecAssert2(buffer != NULL, -1);

    if(xmlSecBufferGetSize(buffer) < (xmlSecSize)cipherKeyLen) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA_SIZE,
                    "keySize=%d;expected=%d",
                    (int)xmlSecBufferGetSize(buffer), (int)cipherKeyLen);
        return(-1);
    }

    xmlSecAssert2(xmlSecBufferGetData(buffer) != NULL, -1);
    memcpy(ctx->key, xmlSecBufferGetData(buffer), cipherKeyLen);

    ctx->keyInitialized = 1;
    return(0);
}

static int
xmlSecOpenSSLEvpBlockCipherExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLEvpBlockCipherCtxPtr ctx;
    xmlSecBufferPtr in, out;
    int ret;

    xmlSecAssert2(xmlSecOpenSSLEvpBlockCipherCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpBlockCipherSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);

    ctx = xmlSecOpenSSLEvpBlockCipherGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if(transform->status == xmlSecTransformStatusWorking) {
        if(ctx->ctxInitialized == 0) {
            ret = xmlSecOpenSSLEvpBlockCipherCtxInit(ctx, in, out,
                        (transform->operation == xmlSecTransformOperationEncrypt) ? 1 : 0,
                        xmlSecTransformGetName(transform), transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecOpenSSLEvpBlockCipherCtxInit",
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
            ret = xmlSecOpenSSLEvpBlockCipherCtxUpdate(ctx, in, out,
                                                xmlSecTransformGetName(transform),
                                                transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecOpenSSLEvpBlockCipherCtxUpdate",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
        }

        if(last != 0) {
            ret = xmlSecOpenSSLEvpBlockCipherCtxFinal(ctx, in, out,
                                            xmlSecTransformGetName(transform),
                                            transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecOpenSSLEvpBlockCipherCtxFinal",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            transform->status = xmlSecTransformStatusFinished;

            /* by now there should be no input */
            xmlSecAssert2(xmlSecBufferGetSize(in) == 0, -1);
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
                    "status=%d", (int)(transform->status));
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
static xmlSecTransformKlass xmlSecOpenSSLAes128CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpBlockCipherSize,            /* xmlSecSize objSize */

    xmlSecNameAes128Cbc,                        /* const xmlChar* name; */
    xmlSecHrefAes128Cbc,                        /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLEvpBlockCipherInitialize,      /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpBlockCipherFinalize,        /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLEvpBlockCipherSetKeyReq,       /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLEvpBlockCipherSetKey,          /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpBlockCipherExecute,         /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformAes128CbcGetKlass:
 *
 * AES 128 CBC encryption transform klass.
 *
 * Returns: pointer to AES 128 CBC encryption transform.
 */
xmlSecTransformId
xmlSecOpenSSLTransformAes128CbcGetKlass(void) {
    return(&xmlSecOpenSSLAes128CbcKlass);
}

static xmlSecTransformKlass xmlSecOpenSSLAes192CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpBlockCipherSize,            /* xmlSecSize objSize */

    xmlSecNameAes192Cbc,                        /* const xmlChar* name; */
    xmlSecHrefAes192Cbc,                        /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLEvpBlockCipherInitialize,      /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpBlockCipherFinalize,        /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLEvpBlockCipherSetKeyReq,       /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLEvpBlockCipherSetKey,          /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpBlockCipherExecute,         /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformAes192CbcGetKlass:
 *
 * AES 192 CBC encryption transform klass.
 *
 * Returns: pointer to AES 192 CBC encryption transform.
 */
xmlSecTransformId
xmlSecOpenSSLTransformAes192CbcGetKlass(void) {
    return(&xmlSecOpenSSLAes192CbcKlass);
}

static xmlSecTransformKlass xmlSecOpenSSLAes256CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpBlockCipherSize,            /* xmlSecSize objSize */

    xmlSecNameAes256Cbc,                        /* const xmlChar* name; */
    xmlSecHrefAes256Cbc,                        /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLEvpBlockCipherInitialize,      /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpBlockCipherFinalize,        /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLEvpBlockCipherSetKeyReq,       /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLEvpBlockCipherSetKey,          /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpBlockCipherExecute,         /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformAes256CbcGetKlass:
 *
 * AES 256 CBC encryption transform klass.
 *
 * Returns: pointer to AES 256 CBC encryption transform.
 */
xmlSecTransformId
xmlSecOpenSSLTransformAes256CbcGetKlass(void) {
    return(&xmlSecOpenSSLAes256CbcKlass);
}

#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
static xmlSecTransformKlass xmlSecOpenSSLDes3CbcKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpBlockCipherSize,            /* xmlSecSize objSize */

    xmlSecNameDes3Cbc,                          /* const xmlChar* name; */
    xmlSecHrefDes3Cbc,                          /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLEvpBlockCipherInitialize,      /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpBlockCipherFinalize,        /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLEvpBlockCipherSetKeyReq,       /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLEvpBlockCipherSetKey,          /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpBlockCipherExecute,         /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformDes3CbcGetKlass:
 *
 * Triple DES CBC encryption transform klass.
 *
 * Returns: pointer to Triple DES encryption transform.
 */
xmlSecTransformId
xmlSecOpenSSLTransformDes3CbcGetKlass(void) {
    return(&xmlSecOpenSSLDes3CbcKlass);
}
#endif /* XMLSEC_NO_DES */


/**
 *
 * XMLSec library
 *
 * DES KW Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef XMLSEC_NO_DES
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <nss.h>
#include <pk11func.h>
#include <hasht.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/crypto.h>

#include "../kw_aes_des.h"

/*********************************************************************
 *
 * DES KW implementation
 *
 *********************************************************************/
static int       xmlSecNssKWDes3GenerateRandom                  (void * context,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int       xmlSecNssKWDes3Sha1                            (void * context,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize, 
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int      xmlSecNssKWDes3BlockEncrypt                     (void * context,
                                                                 const xmlSecByte * iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int      xmlSecNssKWDes3BlockDecrypt                     (void * context,
                                                                 const xmlSecByte * iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);

static xmlSecKWDes3Klass xmlSecNssKWDes3ImplKlass = {
    /* callbacks */
    xmlSecNssKWDes3GenerateRandom,          /* xmlSecKWDes3GenerateRandomMethod     generateRandom; */
    xmlSecNssKWDes3Sha1,                    /* xmlSecKWDes3Sha1Method               sha1; */
    xmlSecNssKWDes3BlockEncrypt,            /* xmlSecKWDes3BlockEncryptMethod       encrypt; */
    xmlSecNssKWDes3BlockDecrypt,            /* xmlSecKWDes3BlockDecryptMethod       decrypt; */

    /* for the future */
    NULL,                                   /* void*                               reserved0; */
    NULL,                                   /* void*                               reserved1; */
}; 

static int      xmlSecNssKWDes3Encrypt                          (const xmlSecByte *key, 
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
typedef struct _xmlSecNssKWDes3Ctx                      xmlSecNssKWDes3Ctx,
                                                        *xmlSecNssKWDes3CtxPtr;
struct _xmlSecNssKWDes3Ctx {
    xmlSecBuffer        keyBuffer;
};
#define xmlSecNssKWDes3Size     \
    (sizeof(xmlSecTransform) + sizeof(xmlSecNssKWDes3Ctx))
#define xmlSecNssKWDes3GetCtx(transform) \
    ((xmlSecNssKWDes3CtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecNssKWDes3Initialize                       (xmlSecTransformPtr transform);
static void     xmlSecNssKWDes3Finalize                         (xmlSecTransformPtr transform);
static int      xmlSecNssKWDes3SetKeyReq                        (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecNssKWDes3SetKey                           (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecNssKWDes3Execute                          (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static xmlSecTransformKlass xmlSecNssKWDes3Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssKWDes3Size,                        /* xmlSecSize objSize */

    xmlSecNameKWDes3,                           /* const xmlChar* name; */
    xmlSecHrefKWDes3,                           /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecNssKWDes3Initialize,                  /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssKWDes3Finalize,                    /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssKWDes3SetKeyReq,                   /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecNssKWDes3SetKey,                      /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssKWDes3Execute,                     /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformKWDes3GetKlass:
 *
 * The Triple DES key wrapper transform klass.
 *
 * Returns: Triple DES key wrapper transform klass.
 */
xmlSecTransformId
xmlSecNssTransformKWDes3GetKlass(void) {
    return(&xmlSecNssKWDes3Klass);
}

static int
xmlSecNssKWDes3Initialize(xmlSecTransformPtr transform) {
    xmlSecNssKWDes3CtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecNssTransformKWDes3Id), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssKWDes3Size), -1);

    ctx = xmlSecNssKWDes3GetCtx(transform);
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
xmlSecNssKWDes3Finalize(xmlSecTransformPtr transform) {
    xmlSecNssKWDes3CtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecNssTransformKWDes3Id));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecNssKWDes3Size));

    ctx = xmlSecNssKWDes3GetCtx(transform);
    xmlSecAssert(ctx != NULL);

    xmlSecBufferFinalize(&(ctx->keyBuffer));
}

static int
xmlSecNssKWDes3SetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecNssKWDes3CtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecNssTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssKWDes3Size), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecNssKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId       = xmlSecNssKeyDataDesId;
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
xmlSecNssKWDes3SetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecNssKWDes3CtxPtr ctx;
    xmlSecBufferPtr buffer;
    xmlSecSize keySize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecNssTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssKWDes3Size), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecNssKeyDataDesId), -1);

    ctx = xmlSecNssKWDes3GetCtx(transform);
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
xmlSecNssKWDes3Execute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecNssKWDes3CtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize, keySize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecNssTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssKWDes3Size), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecNssKWDes3GetCtx(transform);
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
            ret = xmlSecKWDes3Encode(&xmlSecNssKWDes3ImplKlass, ctx,
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
            ret = xmlSecKWDes3Decode(&xmlSecNssKWDes3ImplKlass, ctx,
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
xmlSecNssKWDes3Sha1(void * context,
                    const xmlSecByte * in, xmlSecSize inSize, 
                    xmlSecByte * out, xmlSecSize outSize) {
    xmlSecNssKWDes3CtxPtr ctx = (xmlSecNssKWDes3CtxPtr)context;
    PK11Context *pk11ctx = NULL;
    unsigned int outLen = 0;
    SECStatus status;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= SHA1_LENGTH, -1);

    /* Create a pk11ctx for hashing (digesting) */
    pk11ctx = PK11_CreateDigestContext(SEC_OID_SHA1);
    if (pk11ctx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_CreateDigestContext",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    status = PK11_DigestBegin(pk11ctx);
    if (status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_DigestBegin",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        PK11_DestroyContext(pk11ctx, PR_TRUE);
        return(-1);
    }

    status = PK11_DigestOp(pk11ctx, in, inSize);
    if (status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_DigestOp",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        PK11_DestroyContext(pk11ctx, PR_TRUE);
        return(-1);
    }

    status = PK11_DigestFinal(pk11ctx, out, &outLen, outSize);
    if (status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_DigestFinal",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        PK11_DestroyContext(pk11ctx, PR_TRUE);
        return(-1);
    }

    /* done */
    PK11_DestroyContext(pk11ctx, PR_TRUE);
    xmlSecAssert2(outLen == SHA1_LENGTH, -1);
    return(outLen);
}

static int
xmlSecNssKWDes3GenerateRandom(void * context,
                              xmlSecByte * out, xmlSecSize outSize) {
    xmlSecNssKWDes3CtxPtr ctx = (xmlSecNssKWDes3CtxPtr)context;
    SECStatus status;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize > 0, -1);

    status = PK11_GenerateRandom(out, outSize);
    if(status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_GenerateRandom",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return((int)outSize);
}

static int
xmlSecNssKWDes3BlockEncrypt(void * context,
                               const xmlSecByte * iv, xmlSecSize ivSize,
                               const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize) {
    xmlSecNssKWDes3CtxPtr ctx = (xmlSecNssKWDes3CtxPtr)context;
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

    ret = xmlSecNssKWDes3Encrypt(xmlSecBufferGetData(&(ctx->keyBuffer)), XMLSEC_KW_DES3_KEY_LENGTH,
                                 iv, XMLSEC_KW_DES3_IV_LENGTH,
                                 in, inSize,
                                 out, outSize, 
                                 1); /* encrypt */
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKWDes3Encrypt",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(ret);
}

static int
xmlSecNssKWDes3BlockDecrypt(void * context,
                               const xmlSecByte * iv, xmlSecSize ivSize,
                               const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize) {
    xmlSecNssKWDes3CtxPtr ctx = (xmlSecNssKWDes3CtxPtr)context;
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

    ret = xmlSecNssKWDes3Encrypt(xmlSecBufferGetData(&(ctx->keyBuffer)), XMLSEC_KW_DES3_KEY_LENGTH,
                                 iv, XMLSEC_KW_DES3_IV_LENGTH,
                                 in, inSize,
                                 out, outSize, 
                                 0); /* decrypt */
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssKWDes3Encrypt",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(ret);
}



static int
xmlSecNssKWDes3Encrypt(const xmlSecByte *key, xmlSecSize keySize,
                       const xmlSecByte *iv, xmlSecSize ivSize,
                       const xmlSecByte *in, xmlSecSize inSize,
                       xmlSecByte *out, xmlSecSize outSize,
                       int enc) {
    CK_MECHANISM_TYPE  cipherMech;
    PK11SlotInfo* slot = NULL;
    PK11SymKey* symKey = NULL;
    SECItem* param = NULL;
    PK11Context* pk11ctx = NULL;
    SECItem keyItem, ivItem;
    SECStatus status;
    int result_len = -1;
    int tmp1_outlen;
    unsigned int tmp2_outlen;

    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(keySize == XMLSEC_KW_DES3_KEY_LENGTH, -1);
    xmlSecAssert2(iv != NULL, -1);
    xmlSecAssert2(ivSize == XMLSEC_KW_DES3_IV_LENGTH, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= inSize, -1);

    cipherMech = CKM_DES3_CBC;
    slot = PK11_GetBestSlot(cipherMech, NULL);
    if (slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_GetBestSlot",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    keyItem.data = (unsigned char *)key;
    keyItem.len = keySize;
    symKey = PK11_ImportSymKey(slot, cipherMech, PK11_OriginUnwrap,
                               enc ? CKA_ENCRYPT : CKA_DECRYPT, &keyItem, NULL);
    if (symKey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_ImportSymKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ivItem.data = (unsigned char *)iv;
    ivItem.len = ivSize;

    param = PK11_ParamFromIV(cipherMech, &ivItem);
    if (param == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_ParamFromIV",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    pk11ctx = PK11_CreateContextBySymKey(cipherMech,
                                            enc ? CKA_ENCRYPT : CKA_DECRYPT,
                                            symKey, param);
    if (pk11ctx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_CreateContextBySymKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    tmp1_outlen = tmp2_outlen = 0;
    status = PK11_CipherOp(pk11ctx, out, &tmp1_outlen, outSize,
                       (unsigned char *)in, inSize);
    if (status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_CipherOp",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    status = PK11_DigestFinal(pk11ctx, out+tmp1_outlen,
                          &tmp2_outlen, outSize-tmp1_outlen);
    if (status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_DigestFinal",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    result_len = tmp1_outlen + tmp2_outlen;

done:
    if (slot) {
        PK11_FreeSlot(slot);
    }
    if (symKey) {
        PK11_FreeSymKey(symKey);
    }
    if (param) {
        SECITEM_FreeItem(param, PR_TRUE);
    }
    if (pk11ctx) {
        PK11_DestroyContext(pk11ctx, PR_TRUE);
    }

    return(result_len);
}


#endif /* XMLSEC_NO_DES */


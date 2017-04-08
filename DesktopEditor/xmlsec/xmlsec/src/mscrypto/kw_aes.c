/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 * Copyright (C) 2003-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <windows.h>
#include <wincrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/mscrypto/crypto.h>

#include "../kw_aes_des.h"
#include "private.h"


#ifndef XMLSEC_NO_AES

/*********************************************************************
 *
 * AES KW implementation
 *
 *********************************************************************/
static int      xmlSecMSCryptoKWAesBlockEncrypt         (const xmlSecByte * in, 
                                                         xmlSecSize inSize,
                                                         xmlSecByte * out, 
                                                         xmlSecSize outSize,
                                                         void * cb_ctx);
static int      xmlSecMSCryptoKWAesBlockDecrypt         (const xmlSecByte * in, 
                                                         xmlSecSize inSize,
                                                         xmlSecByte * out, 
                                                         xmlSecSize outSize,
                                                         void * cb_ctx);

/* klass for KW AES operation */
static xmlSecKWAesKlass xmlSecMSCryptoKWAesKlass = {
    /* callbacks */
    xmlSecMSCryptoKWAesBlockEncrypt,        /* xmlSecKWAesBlockEncryptMethod       encrypt; */
    xmlSecMSCryptoKWAesBlockDecrypt,        /* xmlSecKWAesBlockDecryptMethod       decrypt; */

    /* for the future */
    NULL,                                   /* void*                               reserved0; */
    NULL                                    /* void*                               reserved1; */
};

/**************************************************************************
 *
 * Internal MSCrypto KW AES cipher CTX
 *
 *****************************************************************************/
typedef struct _xmlSecMSCryptoKWAesCtx                  xmlSecMSCryptoKWAesCtx,
                                                        *xmlSecMSCryptoKWAesCtxPtr;
struct _xmlSecMSCryptoKWAesCtx {
    ALG_ID                              algorithmIdentifier;
    const xmlSecMSCryptoProviderInfo  * providers;
    xmlSecKeyDataId                     keyId;
    xmlSecSize                          keySize;

    HCRYPTPROV                          cryptProvider;
    HCRYPTKEY                           pubPrivKey;
    xmlSecBuffer                        keyBuffer;
};

/******************************************************************************
 *
 *  KW AES transforms
 *
 * xmlSecMSCryptoKWAesCtx block is located after xmlSecTransform structure
 *
 *****************************************************************************/
#define xmlSecMSCryptoKWAesSize   \
    (sizeof(xmlSecTransform) + sizeof(xmlSecMSCryptoKWAesCtx))
#define xmlSecMSCryptoKWAesGetCtx(transform) \
    ((xmlSecMSCryptoKWAesCtxPtr)(((unsigned char*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecMSCryptoKWAesInitialize       (xmlSecTransformPtr transform);
static void     xmlSecMSCryptoKWAesFinalize         (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoKWAesSetKeyReq        (xmlSecTransformPtr transform,
                                                     xmlSecKeyReqPtr keyReq);
static int      xmlSecMSCryptoKWAesSetKey           (xmlSecTransformPtr transform,
                                                     xmlSecKeyPtr key);
static int      xmlSecMSCryptoKWAesExecute          (xmlSecTransformPtr transform,
                                                     int last,
                                                     xmlSecTransformCtxPtr transformCtx);
static int      xmlSecMSCryptoKWAesCheckId          (xmlSecTransformPtr transform);




/* Ordered list of providers to search for algorithm implementation using
 * xmlSecMSCryptoFindProvider() function
 *
 * MUST END with { NULL, 0 } !!!
 */
static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Aes[] = {
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV,                PROV_RSA_AES},
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE,      PROV_RSA_AES },
    { NULL, 0 }
};

static int
xmlSecMSCryptoKWAesCheckId(xmlSecTransformPtr transform) {

    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWAes128Id) ||
       xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWAes192Id) ||
       xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWAes256Id)) {

       return(1);
    }

    return(0);
}

static int
xmlSecMSCryptoKWAesInitialize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoKWAesCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoKWAesCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWAesSize), -1);

    ctx = xmlSecMSCryptoKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecMSCryptoKWAesCtx));

    if(transform->id == xmlSecMSCryptoTransformKWAes128Id) {
        ctx->algorithmIdentifier    = CALG_AES_128;
        ctx->keyId                  = xmlSecMSCryptoKeyDataAesId;
        ctx->providers              = xmlSecMSCryptoProviderInfo_Aes;
        ctx->keySize                = XMLSEC_KW_AES128_KEY_SIZE;
    } else if(transform->id == xmlSecMSCryptoTransformKWAes192Id) {
        ctx->algorithmIdentifier    = CALG_AES_192;
        ctx->keyId                  = xmlSecMSCryptoKeyDataAesId;
        ctx->providers              = xmlSecMSCryptoProviderInfo_Aes;
        ctx->keySize                = XMLSEC_KW_AES192_KEY_SIZE;
    } else if(transform->id == xmlSecMSCryptoTransformKWAes256Id) {
        ctx->algorithmIdentifier    = CALG_AES_256;
        ctx->keyId                  = xmlSecMSCryptoKeyDataAesId;
        ctx->providers              = xmlSecMSCryptoProviderInfo_Aes;
        ctx->keySize                = XMLSEC_KW_AES256_KEY_SIZE;
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
            NULL,
            XMLSEC_ERRORS_R_INVALID_TRANSFORM,
            XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecBufferInitialize(&ctx->keyBuffer, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* find provider */
    ctx->cryptProvider = xmlSecMSCryptoFindProvider(ctx->providers, NULL, CRYPT_VERIFYCONTEXT, TRUE);
    if(ctx->cryptProvider == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecMSCryptoFindProvider",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);

        return(-1);
    }

    /* Create dummy key to be able to import plain session keys */
    if (!xmlSecMSCryptoCreatePrivateExponentOneKey(ctx->cryptProvider, &(ctx->pubPrivKey))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecMSCryptoCreatePrivateExponentOneKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);

        return(-1);
    }

    return(0);
}

static void
xmlSecMSCryptoKWAesFinalize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoKWAesCtxPtr ctx;

    xmlSecAssert(xmlSecMSCryptoKWAesCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWAesSize));

    ctx = xmlSecMSCryptoKWAesGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if (ctx->pubPrivKey) {
        CryptDestroyKey(ctx->pubPrivKey);
    }
    if (ctx->cryptProvider) {
        CryptReleaseContext(ctx->cryptProvider, 0);
    }
    
    xmlSecBufferFinalize(&ctx->keyBuffer);

    memset(ctx, 0, sizeof(xmlSecMSCryptoKWAesCtx));
}

static int
xmlSecMSCryptoKWAesSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecMSCryptoKWAesCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWAesSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecMSCryptoKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->cryptProvider != 0, -1);

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
xmlSecMSCryptoKWAesSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecMSCryptoKWAesCtxPtr ctx;
    xmlSecBufferPtr buffer;
    xmlSecSize keySize;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWAesSize), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecMSCryptoKeyDataAesId), -1);

    ctx = xmlSecMSCryptoKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(xmlSecKeyGetValue(key));
    xmlSecAssert2(buffer != NULL, -1);

    keySize = xmlSecBufferGetSize(buffer);
    if(keySize < ctx->keySize) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA_SIZE,
                    "key=%d;expected=%d",
                    keySize, ctx->keySize);
        return(-1);
    }

    ret = xmlSecBufferSetData(&(ctx->keyBuffer),
                            xmlSecBufferGetData(buffer),
                            ctx->keySize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferSetData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "expected-size=%d", 
                    ctx->keySize);
        return(-1);
    }

    return(0);
}

static int
xmlSecMSCryptoKWAesExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoKWAesCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoKWAesCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWAesSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoKWAesGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

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
            ret = xmlSecKWAesEncode(&xmlSecMSCryptoKWAesKlass, ctx,
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
            ret = xmlSecKWAesDecode(&xmlSecMSCryptoKWAesKlass, ctx,
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


/*********************************************************************
 *
 * AES KW implementation
 *
 ********************************************************************/
static int
xmlSecMSCryptoKWAesBlockEncrypt(const xmlSecByte * in, xmlSecSize inSize,
                                xmlSecByte * out, xmlSecSize outSize,
                                void * context) {
    xmlSecMSCryptoKWAesCtxPtr ctx = (xmlSecMSCryptoKWAesCtxPtr)context;
    HCRYPTKEY cryptKey = 0;
    DWORD dwCLen;

    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize >= XMLSEC_KW_AES_BLOCK_SIZE, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= inSize, -1);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pubPrivKey != 0, -1);
    xmlSecAssert2(xmlSecBufferGetSize(&ctx->keyBuffer) == ctx->keySize, -1);

    /* Import this key and get an HCRYPTKEY handle, we do it again and again 
       to ensure we don't go into CBC mode */
    if (!xmlSecMSCryptoImportPlainSessionBlob(ctx->cryptProvider,
        ctx->pubPrivKey,
        ctx->algorithmIdentifier,
        xmlSecBufferGetData(&ctx->keyBuffer),
        xmlSecBufferGetSize(&ctx->keyBuffer),
        TRUE,
        &cryptKey))  {

        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoImportPlainSessionBlob",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2(cryptKey != 0, -1);

    /* Set process last block to false, since we handle padding ourselves, and MSCrypto padding
     * can be skipped. I hope this will work .... */
    if(out != in) {
        memcpy(out, in, inSize);
    }
    dwCLen = inSize;
    if(!CryptEncrypt(cryptKey, 0, FALSE, 0, out, &dwCLen, outSize)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptEncrypt",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CryptDestroyKey(cryptKey);    
        return(-1);
    }

    /* cleanup */
    CryptDestroyKey(cryptKey);    
    return(dwCLen);
}

static int
xmlSecMSCryptoKWAesBlockDecrypt(const xmlSecByte * in, xmlSecSize inSize,
                                xmlSecByte * out, xmlSecSize outSize,
                                void * context) {
    xmlSecMSCryptoKWAesCtxPtr ctx = (xmlSecMSCryptoKWAesCtxPtr)context;
    HCRYPTKEY cryptKey = 0;
    DWORD dwCLen;

    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize >= XMLSEC_KW_AES_BLOCK_SIZE, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize >= inSize, -1);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pubPrivKey != 0, -1);
    xmlSecAssert2(xmlSecBufferGetSize(&ctx->keyBuffer) == ctx->keySize, -1);

    /* Import this key and get an HCRYPTKEY handle, we do it again and again 
       to ensure we don't go into CBC mode */
    if (!xmlSecMSCryptoImportPlainSessionBlob(ctx->cryptProvider,
        ctx->pubPrivKey,
        ctx->algorithmIdentifier,
        xmlSecBufferGetData(&ctx->keyBuffer),
        xmlSecBufferGetSize(&ctx->keyBuffer),
        TRUE,
        &cryptKey))  {

        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecMSCryptoImportPlainSessionBlob",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2(cryptKey != 0, -1);

    /* Set process last block to false, since we handle padding ourselves, and MSCrypto padding
     * can be skipped. I hope this will work .... */
    if(out != in) {
        memcpy(out, in, inSize);
    }
    dwCLen = inSize;
    if(!CryptDecrypt(cryptKey, 0, FALSE, 0, out, &dwCLen)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptEncrypt",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CryptDestroyKey(cryptKey);
        return(-1);
    }

    /* cleanup */
    CryptDestroyKey(cryptKey);
    return(dwCLen);
}

/*********************************************************************
 *
 * AES KW cipher transforms
 *
 ********************************************************************/

/*
 * The AES-128 kew wrapper transform klass.
 */
static xmlSecTransformKlass xmlSecMSCryptoKWAes128Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoKWAesSize,              /* xmlSecSize objSize */

    xmlSecNameKWAes128,                         /* const xmlChar* name; */
    xmlSecHrefKWAes128,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecMSCryptoKWAesInitialize,        /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoKWAesFinalize,          /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoKWAesSetKeyReq,         /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecMSCryptoKWAesSetKey,                  /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoKWAesExecute,                 /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformKWAes128GetKlass:
 *
 * The AES-128 kew wrapper transform klass.
 *
 * Returns: AES-128 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformKWAes128GetKlass(void) {
    return(&xmlSecMSCryptoKWAes128Klass);
}


/*
 * The AES-192 kew wrapper transform klass.
 */
static xmlSecTransformKlass xmlSecMSCryptoKWAes192Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoKWAesSize,              /* xmlSecSize objSize */

    xmlSecNameKWAes192,                         /* const xmlChar* name; */
    xmlSecHrefKWAes192,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecMSCryptoKWAesInitialize,        /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoKWAesFinalize,          /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoKWAesSetKeyReq,         /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecMSCryptoKWAesSetKey,                  /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoKWAesExecute,                 /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformKWAes192GetKlass:
 *
 * The AES-192 kew wrapper transform klass.
 *
 * Returns: AES-192 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformKWAes192GetKlass(void) {
    return(&xmlSecMSCryptoKWAes192Klass);
}

/*
 * The AES-256 kew wrapper transform klass.
 */
static xmlSecTransformKlass xmlSecMSCryptoKWAes256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoKWAesSize,              /* xmlSecSize objSize */

    xmlSecNameKWAes256,                         /* const xmlChar* name; */
    xmlSecHrefKWAes256,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecMSCryptoKWAesInitialize,        /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoKWAesFinalize,          /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoKWAesSetKeyReq,         /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecMSCryptoKWAesSetKey,                  /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                           /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoKWAesExecute,                 /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformKWAes256GetKlass:
 *
 * The AES-256 kew wrapper transform klass.
 *
 * Returns: AES-256 kew wrapper transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformKWAes256GetKlass(void) {
    return(&xmlSecMSCryptoKWAes256Klass);
}

#endif /* XMLSEC_NO_AES */

/**
 *
 * XMLSec library
 *
 * DES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef XMLSEC_NO_DES
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/mscrypto/crypto.h>

#include "../kw_aes_des.h"
#include "private.h"


/*********************************************************************
 *
 * DES KW implementation
 *
 *********************************************************************/
static int       xmlSecMSCryptoKWDes3GenerateRandom               (void * context,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int       xmlSecMSCryptoKWDes3Sha1                         (void * context,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize, 
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int      xmlSecMSCryptoKWDes3BlockEncrypt                  (void * context,
                                                                 const xmlSecByte * iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);
static int      xmlSecMSCryptoKWDes3BlockDecrypt                  (void * context,
                                                                 const xmlSecByte * iv, 
                                                                 xmlSecSize ivSize,
                                                                 const xmlSecByte * in, 
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte * out, 
                                                                 xmlSecSize outSize);

static xmlSecKWDes3Klass xmlSecMSCryptoKWDes3ImplKlass = {
    /* callbacks */
    xmlSecMSCryptoKWDes3GenerateRandom,       /* xmlSecKWDes3GenerateRandomMethod     generateRandom; */
    xmlSecMSCryptoKWDes3Sha1,                 /* xmlSecKWDes3Sha1Method               sha1; */
    xmlSecMSCryptoKWDes3BlockEncrypt,         /* xmlSecKWDes3BlockEncryptMethod       encrypt; */
    xmlSecMSCryptoKWDes3BlockDecrypt,         /* xmlSecKWDes3BlockDecryptMethod       decrypt; */

    /* for the future */
    NULL,                                   /* void*                               reserved0; */
    NULL,                                   /* void*                               reserved1; */
}; 

/*********************************************************************
 *
 * Triple DES Key Wrap transform
 *
 * key (xmlSecBuffer) is located after xmlSecTransform structure
 *
 ********************************************************************/
typedef struct _xmlSecMSCryptoKWDes3Ctx              xmlSecMSCryptoKWDes3Ctx,
                                                  *xmlSecMSCryptoKWDes3CtxPtr;
struct _xmlSecMSCryptoKWDes3Ctx {
    ALG_ID                              desAlgorithmIdentifier;
    const xmlSecMSCryptoProviderInfo  * desProviders;
    ALG_ID                              sha1AlgorithmIdentifier;
    const xmlSecMSCryptoProviderInfo  * sha1Providers;
    xmlSecKeyDataId                     keyId;
    xmlSecSize                          keySize;

    HCRYPTPROV                          desCryptProvider;
    HCRYPTPROV                          sha1CryptProvider;
    HCRYPTKEY                           pubPrivKey;
    xmlSecBuffer                        keyBuffer;
};
#define xmlSecMSCryptoKWDes3Size     \
    (sizeof(xmlSecTransform) + sizeof(xmlSecMSCryptoKWDes3Ctx))
#define xmlSecMSCryptoKWDes3GetCtx(transform) \
    ((xmlSecMSCryptoKWDes3CtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecMSCryptoKWDes3Initialize                   (xmlSecTransformPtr transform);
static void     xmlSecMSCryptoKWDes3Finalize                     (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoKWDes3SetKeyReq                    (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecMSCryptoKWDes3SetKey                       (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecMSCryptoKWDes3Execute                      (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static xmlSecTransformKlass xmlSecMSCryptoKWDes3Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoKWDes3Size,                   /* xmlSecSize objSize */

    xmlSecNameKWDes3,                           /* const xmlChar* name; */
    xmlSecHrefKWDes3,                           /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecMSCryptoKWDes3Initialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoKWDes3Finalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoKWDes3SetKeyReq,              /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecMSCryptoKWDes3SetKey,                 /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoKWDes3Execute,                /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformKWDes3GetKlass:
 *
 * The Triple DES key wrapper transform klass.
 *
 * Returns: Triple DES key wrapper transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformKWDes3GetKlass(void) {
    return(&xmlSecMSCryptoKWDes3Klass);
}

/* Ordered list of providers to search for algorithm implementation using
 * xmlSecMSCryptoFindProvider() function
 *
 * MUST END with { NULL, 0 } !!!
 */
static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Des[] = {
    { MS_STRONG_PROV,               PROV_RSA_FULL },
    { MS_ENHANCED_PROV,             PROV_RSA_FULL },
    { NULL, 0 }
};
static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Sha1[] = {
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV,                PROV_RSA_AES},
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE,      PROV_RSA_AES },
    { MS_STRONG_PROV,                                   PROV_RSA_FULL },
    { MS_ENHANCED_PROV,                                 PROV_RSA_FULL },
    { MS_DEF_PROV,                                      PROV_RSA_FULL },
    { NULL, 0 }
};


static int
xmlSecMSCryptoKWDes3Initialize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoKWDes3CtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWDes3Id), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWDes3Size), -1);

    ctx = xmlSecMSCryptoKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecMSCryptoKWDes3Ctx));

    if(transform->id == xmlSecMSCryptoTransformKWDes3Id) {
        ctx->desAlgorithmIdentifier  = CALG_3DES;
        ctx->desProviders            = xmlSecMSCryptoProviderInfo_Des;
        ctx->sha1AlgorithmIdentifier = CALG_SHA1;
        ctx->sha1Providers           = xmlSecMSCryptoProviderInfo_Sha1;
        ctx->keyId                   = xmlSecMSCryptoKeyDataDesId;
        ctx->keySize                 = XMLSEC_KW_DES3_KEY_LENGTH;
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
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* find providers */
    ctx->desCryptProvider = xmlSecMSCryptoFindProvider(ctx->desProviders, NULL, CRYPT_VERIFYCONTEXT, TRUE);
    if(ctx->desCryptProvider == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecMSCryptoFindProvider(des)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);

        return(-1);
    }

    ctx->sha1CryptProvider = xmlSecMSCryptoFindProvider(ctx->sha1Providers, NULL, CRYPT_VERIFYCONTEXT, TRUE);
    if(ctx->sha1CryptProvider == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecMSCryptoFindProvider(sha1)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);

        return(-1);
    }

    /* Create dummy key to be able to import plain session keys */
    if (!xmlSecMSCryptoCreatePrivateExponentOneKey(ctx->desCryptProvider, &(ctx->pubPrivKey))) {
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
xmlSecMSCryptoKWDes3Finalize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoKWDes3CtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWDes3Id));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWDes3Size));

    ctx = xmlSecMSCryptoKWDes3GetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if (ctx->pubPrivKey) {
        CryptDestroyKey(ctx->pubPrivKey);
    }
    if (ctx->desCryptProvider) {
        CryptReleaseContext(ctx->desCryptProvider, 0);
    }
    if (ctx->sha1CryptProvider) {
        CryptReleaseContext(ctx->sha1CryptProvider, 0);
    }
    
    xmlSecBufferFinalize(&ctx->keyBuffer);

    memset(ctx, 0, sizeof(xmlSecMSCryptoKWDes3Ctx));
}

static int
xmlSecMSCryptoKWDes3SetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecMSCryptoKWDes3CtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWDes3Size), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecMSCryptoKWDes3GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId       = xmlSecMSCryptoKeyDataDesId;
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
xmlSecMSCryptoKWDes3SetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecMSCryptoKWDes3CtxPtr ctx;
    xmlSecBufferPtr buffer;
    xmlSecSize keySize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWDes3Size), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecMSCryptoKeyDataDesId), -1);

    ctx = xmlSecMSCryptoKWDes3GetCtx(transform);
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
xmlSecMSCryptoKWDes3Execute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoKWDes3CtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize, keySize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformKWDes3Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoKWDes3Size), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoKWDes3GetCtx(transform);
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
            ret = xmlSecKWDes3Encode(&xmlSecMSCryptoKWDes3ImplKlass, ctx,
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
            ret = xmlSecKWDes3Decode(&xmlSecMSCryptoKWDes3ImplKlass, ctx,
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
xmlSecMSCryptoKWDes3Sha1(void * context,
                       const xmlSecByte * in, xmlSecSize inSize, 
                       xmlSecByte * out, xmlSecSize outSize) {
    xmlSecMSCryptoKWDes3CtxPtr ctx = (xmlSecMSCryptoKWDes3CtxPtr)context;
    HCRYPTHASH mscHash = 0;
    DWORD retLen;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->sha1CryptProvider != 0, -1);
    xmlSecAssert2(ctx->sha1AlgorithmIdentifier != 0, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(inSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize > 0, -1);

    /* create */
    ret = CryptCreateHash(ctx->sha1CryptProvider,
        ctx->sha1AlgorithmIdentifier,
        0,
        0,
        &mscHash);
    if((ret == 0) || (mscHash == 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptCreateHash",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* hash */
    ret = CryptHashData(mscHash,
        in, 
        inSize,
        0);
    if(ret == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptHashData",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "size=%d", inSize);
        CryptDestroyHash(mscHash);
        return(-1);
    }

    /* get results */
    retLen = outSize;
    ret = CryptGetHashParam(mscHash,
        HP_HASHVAL,
        out,
        &retLen,
        0);
    if (ret == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGetHashParam(HP_HASHVAL)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", outSize);
        CryptDestroyHash(mscHash);
        return(-1);
    }

    /* done */
    CryptDestroyHash(mscHash);
    return(retLen);
}

static int
xmlSecMSCryptoKWDes3GenerateRandom(void * context,
                                   xmlSecByte * out, xmlSecSize outSize) 
{
    xmlSecMSCryptoKWDes3CtxPtr ctx = (xmlSecMSCryptoKWDes3CtxPtr)context;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->desCryptProvider != 0, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize > 0, -1);

    if(!CryptGenRandom(ctx->desCryptProvider, outSize, out)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGenRandom",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "len=%d", outSize);
        return(-1);
    }

    return((int)outSize);
}

static int
xmlSecMSCryptoKWDes3BlockEncrypt(void * context,
                               const xmlSecByte * iv, xmlSecSize ivSize,
                               const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize) {
    xmlSecMSCryptoKWDes3CtxPtr ctx = (xmlSecMSCryptoKWDes3CtxPtr)context;
    DWORD dwBlockLen, dwBlockLenLen, dwCLen;
    HCRYPTKEY cryptKey = 0;
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

    /* Import this key and get an HCRYPTKEY handle, we do it again and again 
       to ensure we don't go into CBC mode */
    if (!xmlSecMSCryptoImportPlainSessionBlob(ctx->desCryptProvider,
        ctx->pubPrivKey,
        ctx->desAlgorithmIdentifier,
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

    /* iv len == block len */
    dwBlockLenLen = sizeof(DWORD);
    if (!CryptGetKeyParam(cryptKey, KP_BLOCKLEN, (BYTE *)&dwBlockLen, &dwBlockLenLen, 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGetKeyParam",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CryptDestroyKey(cryptKey);
        return(-1);
    }

    /* set IV */
    if((ivSize < dwBlockLen / 8) || (!CryptSetKeyParam(cryptKey, KP_IV, iv, 0))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptSetKeyParam",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "ivSize=%d, dwBlockLen=%d", 
                    ivSize, dwBlockLen / 8);
        CryptDestroyKey(cryptKey);
        return(-1);
    }

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
xmlSecMSCryptoKWDes3BlockDecrypt(void * context,
                               const xmlSecByte * iv, xmlSecSize ivSize,
                               const xmlSecByte * in, xmlSecSize inSize,
                               xmlSecByte * out, xmlSecSize outSize) {
    xmlSecMSCryptoKWDes3CtxPtr ctx = (xmlSecMSCryptoKWDes3CtxPtr)context;
    DWORD dwBlockLen, dwBlockLenLen, dwCLen;
    HCRYPTKEY cryptKey = 0;
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

    /* Import this key and get an HCRYPTKEY handle, we do it again and again 
       to ensure we don't go into CBC mode */
    if (!xmlSecMSCryptoImportPlainSessionBlob(ctx->desCryptProvider,
        ctx->pubPrivKey,
        ctx->desAlgorithmIdentifier,
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

    /* iv len == block len */
    dwBlockLenLen = sizeof(DWORD);
    if (!CryptGetKeyParam(cryptKey, KP_BLOCKLEN, (BYTE *)&dwBlockLen, &dwBlockLenLen, 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGetKeyParam",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CryptDestroyKey(cryptKey);
        return(-1);
    }

    /* set IV */
    if((ivSize < dwBlockLen / 8) || (!CryptSetKeyParam(cryptKey, KP_IV, iv, 0))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptSetKeyParam",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "ivSize=%d, dwBlockLen=%d", 
                    ivSize, dwBlockLen / 8);
        CryptDestroyKey(cryptKey);
        return(-1);
    }

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


#endif /* XMLSEC_NO_DES */


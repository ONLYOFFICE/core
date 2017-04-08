/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 * Copyright (c) 2005-2006 Cryptocom LTD (http://www.cryptocom.ru).
 */
#include "globals.h"

#include <string.h>
#include <windows.h>
#include <wincrypt.h>
#ifndef XMLSEC_NO_GOST
#include "csp_calg.h"
#endif

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/mscrypto/crypto.h>
#include "private.h"

#define MSCRYPTO_MAX_HASH_SIZE 256

typedef struct _xmlSecMSCryptoDigestCtx xmlSecMSCryptoDigestCtx, *xmlSecMSCryptoDigestCtxPtr;
struct _xmlSecMSCryptoDigestCtx {
    HCRYPTPROV      provider;
    ALG_ID          alg_id;
    const xmlSecMSCryptoProviderInfo  * providers;
    HCRYPTHASH      mscHash;
    unsigned char   dgst[MSCRYPTO_MAX_HASH_SIZE];
    size_t          dgstSize;   /* dgst size in bytes */
};

/******************************************************************************
 *
 * MSCrypto Digest transforms
 *
 * xmlSecMSCryptoDigestCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecMSCryptoDigestSize        \
    (sizeof(xmlSecTransform) + sizeof(xmlSecMSCryptoDigestCtx))
#define xmlSecMSCryptoDigestGetCtx(transform) \
    ((xmlSecMSCryptoDigestCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))


static int      xmlSecMSCryptoDigestInitialize  (xmlSecTransformPtr transform);
static void     xmlSecMSCryptoDigestFinalize    (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoDigestVerify      (xmlSecTransformPtr transform,
                                                 const xmlSecByte* data,
                                                 xmlSecSize dataSize,
                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecMSCryptoDigestExecute     (xmlSecTransformPtr transform,
                                                 int last,
                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecMSCryptoDigestCheckId     (xmlSecTransformPtr transform);


/* Ordered list of providers to search for algorithm implementation using
 * xmlSecMSCryptoFindProvider() function
 *
 * MUST END with { NULL, 0 } !!!
 */
static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Sha1[] = {
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV,                PROV_RSA_AES},
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE,      PROV_RSA_AES },
    { MS_STRONG_PROV,                                   PROV_RSA_FULL },
    { MS_ENHANCED_PROV,                                 PROV_RSA_FULL },
    { MS_DEF_PROV,                                      PROV_RSA_FULL },
    { NULL, 0 }
};

static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Sha2[] = {
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV,                PROV_RSA_AES},
    { XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE,      PROV_RSA_AES },
    { NULL, 0 }
};

static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Md5[] = {
    { MS_STRONG_PROV,                                   PROV_RSA_FULL },
    { MS_ENHANCED_PROV,                                 PROV_RSA_FULL },
    { MS_DEF_PROV,                                      PROV_RSA_FULL },
    { NULL, 0 }
};

#ifndef XMLSEC_NO_GOST
static xmlSecMSCryptoProviderInfo xmlSecMSCryptoProviderInfo_Gost[] = {
    { MAGPRO_CSP,                                       PROV_MAGPRO_GOST },
    { CRYPTOPRO_CSP,                                    PROV_CRYPTOPRO_GOST },
    { NULL, 0 }
};
#endif /*ndef XMLSEC_NO_GOST*/

static int
xmlSecMSCryptoDigestCheckId(xmlSecTransformPtr transform) {

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformMd5Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha1Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha256Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha384Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha512Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_GOST
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformGostR3411_94Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_GOST*/

    return(0);
}

static int
xmlSecMSCryptoDigestInitialize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoDigestCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoDigestCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoDigestSize), -1);

    ctx = xmlSecMSCryptoDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* initialize context */
    memset(ctx, 0, sizeof(xmlSecMSCryptoDigestCtx));

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformMd5Id)) {
        ctx->alg_id = CALG_MD5;
        ctx->providers = xmlSecMSCryptoProviderInfo_Md5;
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha1Id)) {
        ctx->alg_id = CALG_SHA1;
        ctx->providers = xmlSecMSCryptoProviderInfo_Sha1;
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha256Id)) {
        ctx->alg_id = CALG_SHA_256;
        ctx->providers = xmlSecMSCryptoProviderInfo_Sha2;
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha384Id)) {
        ctx->alg_id = CALG_SHA_384;
        ctx->providers = xmlSecMSCryptoProviderInfo_Sha2;
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformSha512Id)) {
        ctx->alg_id = CALG_SHA_512;
        ctx->providers = xmlSecMSCryptoProviderInfo_Sha2;
    } else
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_GOST
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformGostR3411_94Id)) {
        ctx->alg_id = CALG_MAGPRO_HASH_R3411_94;
        ctx->providers = xmlSecMSCryptoProviderInfo_Gost;
    } else
#endif /* XMLSEC_NO_GOST*/

    {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ctx->provider = xmlSecMSCryptoFindProvider(ctx->providers, NULL, CRYPT_VERIFYCONTEXT, TRUE);
    if(ctx->provider == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
            "xmlSecMSCryptoFindProvider",
            XMLSEC_ERRORS_R_XMLSEC_FAILED,
            XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void xmlSecMSCryptoDigestFinalize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoDigestCtxPtr ctx;

    xmlSecAssert(xmlSecMSCryptoDigestCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecMSCryptoDigestSize));

    ctx = xmlSecMSCryptoDigestGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->mscHash != 0) {
        CryptDestroyHash(ctx->mscHash);
    }
    if(ctx->provider != 0) {
        CryptReleaseContext(ctx->provider, 0);
    }

    memset(ctx, 0, sizeof(xmlSecMSCryptoDigestCtx));
}

static int
xmlSecMSCryptoDigestVerify(xmlSecTransformPtr transform,
                           const xmlSecByte* data,
                           xmlSecSize dataSize,
                           xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoDigestCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoDigestCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoDigestSize), -1);
    xmlSecAssert2(transform->operation == xmlSecTransformOperationVerify, -1);
    xmlSecAssert2(transform->status == xmlSecTransformStatusFinished, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->dgstSize > 0, -1);

    if(dataSize != ctx->dgstSize) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "data_size=%d;dgst_size=%d",
                    dataSize, ctx->dgstSize);
        transform->status = xmlSecTransformStatusFail;
        return(0);
    }

    if(memcmp(ctx->dgst, data, ctx->dgstSize) != 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_DATA,
                    "data and digest do not match");
        transform->status = xmlSecTransformStatusFail;
        return(0);
    }

    transform->status = xmlSecTransformStatusOk;
    return(0);
}

static int
xmlSecMSCryptoDigestExecute(xmlSecTransformPtr transform,
                            int last,
                            xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoDigestCtxPtr ctx;
    xmlSecBufferPtr in, out;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoDigestCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoDigestSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    in = &(transform->inBuf);
    xmlSecAssert2(in != NULL, -1);

    out = &(transform->outBuf);
    xmlSecAssert2(out != NULL, -1);

    ctx = xmlSecMSCryptoDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        ret = CryptCreateHash(ctx->provider,
            ctx->alg_id,
            0,
            0,
            &(ctx->mscHash));

        if((ret == 0) || (ctx->mscHash == 0)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "CryptCreateHash",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        transform->status = xmlSecTransformStatusWorking;
    }

    if (transform->status == xmlSecTransformStatusWorking) {
        xmlSecSize inSize;

        inSize = xmlSecBufferGetSize(in);
        if(inSize > 0) {
            ret = CryptHashData(ctx->mscHash,
                xmlSecBufferGetData(in),
                inSize,
                0);

            if(ret == 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "CryptHashData",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "size=%d", inSize);
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
        }
        if(last) {
            /* TODO: make a MSCrypto compatible assert here */
            /* xmlSecAssert2((xmlSecSize)EVP_MD_size(ctx->digest) <= sizeof(ctx->dgst), -1); */
            DWORD retLen;
            retLen = MSCRYPTO_MAX_HASH_SIZE;

            ret = CryptGetHashParam(ctx->mscHash,
                HP_HASHVAL,
                ctx->dgst,
                &retLen,
                0);

            if (ret == 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "CryptGetHashParam(HP_HASHVAL)",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "size=%d", MSCRYPTO_MAX_HASH_SIZE);
                return(-1);
            }

            ctx->dgstSize = (size_t)retLen;

            xmlSecAssert2(ctx->dgstSize > 0, -1);

            /* copy result to output */
            if(transform->operation == xmlSecTransformOperationSign) {
                ret = xmlSecBufferAppend(out, ctx->dgst, ctx->dgstSize);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBufferAppend",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "size=%d", ctx->dgstSize);
                    return(-1);
                }
            }
            transform->status = xmlSecTransformStatusFinished;
        }
    } else if(transform->status == xmlSecTransformStatusFinished) {
        /* the only way we can get here is if there is no input */
        xmlSecAssert2(xmlSecBufferGetSize(in) == 0, -1);
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


#ifndef XMLSEC_NO_MD5
/******************************************************************************
 *
 * MD5
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoMd5Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* size_t klassSize */
    xmlSecMSCryptoDigestSize,                   /* size_t objSize */

    xmlSecNameMd5,                              /* const xmlChar* name; */
    xmlSecHrefMd5,                              /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */
    xmlSecMSCryptoDigestInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoDigestFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoDigestVerify,                 /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoDigestExecute,                /* xmlSecTransformExecuteMethod execute; */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformMd5GetKlass:
 *
 * SHA-1 digest transform klass.
 *
 * Returns: pointer to SHA-1 digest transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformMd5GetKlass(void) {
    return(&xmlSecMSCryptoMd5Klass);
}
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
/******************************************************************************
 *
 * SHA1
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* size_t klassSize */
    xmlSecMSCryptoDigestSize,                   /* size_t objSize */

    xmlSecNameSha1,                             /* const xmlChar* name; */
    xmlSecHrefSha1,                             /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */
    xmlSecMSCryptoDigestInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoDigestFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoDigestVerify,                 /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoDigestExecute,                /* xmlSecTransformExecuteMethod execute; */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformSha1GetKlass:
 *
 * SHA-1 digest transform klass.
 *
 * Returns: pointer to SHA-1 digest transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformSha1GetKlass(void) {
    return(&xmlSecMSCryptoSha1Klass);
}
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/******************************************************************************
 *
 * SHA256
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoSha256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),              /* size_t klassSize */
    xmlSecMSCryptoDigestSize,                  /* size_t objSize */

    xmlSecNameSha256,                          /* const xmlChar* name; */
    xmlSecHrefSha256,                          /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,          /* xmlSecTransformUsage usage; */
    xmlSecMSCryptoDigestInitialize,            /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoDigestFinalize,              /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                      /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                      /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                      /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                      /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoDigestVerify,                        /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,         /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,             /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                      /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                      /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoDigestExecute,               /* xmlSecTransformExecuteMethod execute; */
    NULL,                                      /* void* reserved0; */
    NULL,                                      /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformSha256GetKlass:
 *
 * SHA-256 digest transform klass.
 *
 * Returns: pointer to SHA-256 digest transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformSha256GetKlass(void) {
    return(&xmlSecMSCryptoSha256Klass);
}
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/******************************************************************************
 *
 * SHA384
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoSha384Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),              /* size_t klassSize */
    xmlSecMSCryptoDigestSize,                  /* size_t objSize */

    xmlSecNameSha384,                          /* const xmlChar* name; */
    xmlSecHrefSha384,                          /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,          /* xmlSecTransformUsage usage; */
    xmlSecMSCryptoDigestInitialize,            /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoDigestFinalize,              /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                      /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                      /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                      /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                      /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoDigestVerify,                        /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,         /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,             /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                      /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                      /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoDigestExecute,               /* xmlSecTransformExecuteMethod execute; */
    NULL,                                      /* void* reserved0; */
    NULL,                                      /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformSha384GetKlass:
 *
 * SHA-384 digest transform klass.
 *
 * Returns: pointer to SHA-384 digest transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformSha384GetKlass(void) {
    return(&xmlSecMSCryptoSha384Klass);
}
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/******************************************************************************
 *
 * SHA512
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoSha512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),              /* size_t klassSize */
    xmlSecMSCryptoDigestSize,                  /* size_t objSize */

    xmlSecNameSha512,                          /* const xmlChar* name; */
    xmlSecHrefSha512,                          /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,          /* xmlSecTransformUsage usage; */
    xmlSecMSCryptoDigestInitialize,            /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoDigestFinalize,              /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                      /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                      /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                      /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                      /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoDigestVerify,                        /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,         /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,             /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                      /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                      /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoDigestExecute,               /* xmlSecTransformExecuteMethod execute; */
    NULL,                                      /* void* reserved0; */
    NULL,                                      /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformSha512GetKlass:
 *
 * SHA-512 digest transform klass.
 *
 * Returns: pointer to SHA-512 digest transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformSha512GetKlass(void) {
    return(&xmlSecMSCryptoSha512Klass);
}
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_GOST
/******************************************************************************
 *
 * GOSTR3411_94
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoGostR3411_94Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* size_t klassSize */
    xmlSecMSCryptoDigestSize,                   /* size_t objSize */

    xmlSecNameGostR3411_94,                             /* const xmlChar* name; */
    xmlSecHrefGostR3411_94,                             /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */
    xmlSecMSCryptoDigestInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoDigestFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoDigestVerify,                 /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoDigestExecute,                /* xmlSecTransformExecuteMethod execute; */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformGostR3411_94GetKlass:
 *
 * GOSTR3411_94 digest transform klass.
 *
 * Returns: pointer to GOSTR3411_94 digest transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformGostR3411_94GetKlass(void) {
    return(&xmlSecMSCryptoGostR3411_94Klass);
}
#endif /* XMLSEC_NO_GOST*/


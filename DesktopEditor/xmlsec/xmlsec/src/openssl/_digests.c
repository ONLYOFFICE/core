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

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/openssl/crypto.h>
#include <xmlsec/openssl/evp.h>

/* new API from OpenSSL 1.1.0 (https://www.openssl.org/docs/manmaster/crypto/EVP_DigestInit.html):
 *
 * EVP_MD_CTX_create() and EVP_MD_CTX_destroy() were renamed to EVP_MD_CTX_new() and EVP_MD_CTX_free() in OpenSSL 1.1.
 */
#if !defined(XMLSEC_OPENSSL_110)
#define EVP_MD_CTX_new()   EVP_MD_CTX_create()
#define EVP_MD_CTX_free(x) EVP_MD_CTX_destroy((x))
#endif /* !defined(XMLSEC_OPENSSL_110) */


/**************************************************************************
 *
 * Internal OpenSSL Digest CTX
 *
 *****************************************************************************/
typedef struct _xmlSecOpenSSLDigestCtx          xmlSecOpenSSLDigestCtx, *xmlSecOpenSSLDigestCtxPtr;
struct _xmlSecOpenSSLDigestCtx {
    const EVP_MD*       digest;
    EVP_MD_CTX*         digestCtx;
    xmlSecByte          dgst[EVP_MAX_MD_SIZE];
    xmlSecSize          dgstSize;       /* dgst size in bytes */
};

/******************************************************************************
 *
 * EVP Digest transforms
 *
 * xmlSecOpenSSLDigestCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecOpenSSLEvpDigestSize      \
    (sizeof(xmlSecTransform) + sizeof(xmlSecOpenSSLDigestCtx))
#define xmlSecOpenSSLEvpDigestGetCtx(transform) \
    ((xmlSecOpenSSLDigestCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))


static int      xmlSecOpenSSLEvpDigestInitialize        (xmlSecTransformPtr transform);
static void     xmlSecOpenSSLEvpDigestFinalize          (xmlSecTransformPtr transform);
static int      xmlSecOpenSSLEvpDigestVerify            (xmlSecTransformPtr transform,
                                                         const xmlSecByte* data,
                                                         xmlSecSize dataSize,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLEvpDigestExecute           (xmlSecTransformPtr transform,
                                                         int last,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLEvpDigestCheckId           (xmlSecTransformPtr transform);

static int
xmlSecOpenSSLEvpDigestCheckId(xmlSecTransformPtr transform) {

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformMd5Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRipemd160Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha1Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA224
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha224Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA224 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha256Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha384Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha512Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_GOST
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformGostR3411_94Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_GOST*/

#ifndef XMLSEC_NO_GOST2012
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformGostR3411_2012_256Id)) {
        return(1);
    } else

    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformGostR3411_2012_512Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_GOST2012 */

    {
        return(0);
    }

    return(0);
}

static int
xmlSecOpenSSLEvpDigestInitialize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLDigestCtxPtr ctx;

    xmlSecAssert2(xmlSecOpenSSLEvpDigestCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpDigestSize), -1);

    ctx = xmlSecOpenSSLEvpDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* initialize context */
    memset(ctx, 0, sizeof(xmlSecOpenSSLDigestCtx));

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformMd5Id)) {
        ctx->digest = EVP_md5();
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRipemd160Id)) {
        ctx->digest = EVP_ripemd160();
    } else
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha1Id)) {
        ctx->digest = EVP_sha1();
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA224
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha224Id)) {
        ctx->digest = EVP_sha224();
    } else
#endif /* XMLSEC_NO_SHA224 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha256Id)) {
        ctx->digest = EVP_sha256();
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha384Id)) {
        ctx->digest = EVP_sha384();
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformSha512Id)) {
        ctx->digest = EVP_sha512();
    } else
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_GOST
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformGostR3411_94Id)) {
        ctx->digest = EVP_get_digestbyname("md_gost94");
		if (!ctx->digest) {
			xmlSecError(XMLSEC_ERRORS_HERE,
					xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
					NULL,
					XMLSEC_ERRORS_R_INVALID_TRANSFORM,
					XMLSEC_ERRORS_NO_MESSAGE);
			return(-1);
		}
    } else
#endif /* XMLSEC_NO_GOST */

#ifndef XMLSEC_NO_GOST2012
    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformGostR3411_2012_256Id)) {
        ctx->digest = EVP_get_digestbyname("md_gost12_256");
				if (!ctx->digest)
				{
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
				}
    } else

    if(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformGostR3411_2012_512Id)) {
        ctx->digest = EVP_get_digestbyname("md_gost12_512");
				if (!ctx->digest)
				{
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
				}
    } else
#endif /* XMLSEC_NO_GOST2012 */

    {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* create digest CTX */
    ctx->digestCtx = EVP_MD_CTX_new();
    if(ctx->digestCtx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "EVP_MD_CTX_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* done */
    return(0);
}

static void
xmlSecOpenSSLEvpDigestFinalize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLDigestCtxPtr ctx;

    xmlSecAssert(xmlSecOpenSSLEvpDigestCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpDigestSize));

    ctx = xmlSecOpenSSLEvpDigestGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->digestCtx != NULL) {
        EVP_MD_CTX_free(ctx->digestCtx);
    }

    memset(ctx, 0, sizeof(xmlSecOpenSSLDigestCtx));
}

static int
xmlSecOpenSSLEvpDigestVerify(xmlSecTransformPtr transform,
                        const xmlSecByte* data, xmlSecSize dataSize,
                        xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLDigestCtxPtr ctx;

    xmlSecAssert2(xmlSecOpenSSLEvpDigestCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpDigestSize), -1);
    xmlSecAssert2(transform->operation == xmlSecTransformOperationVerify, -1);
    xmlSecAssert2(transform->status == xmlSecTransformStatusFinished, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLEvpDigestGetCtx(transform);
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
xmlSecOpenSSLEvpDigestExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLDigestCtxPtr ctx;
    xmlSecBufferPtr in, out;
    int ret;

    xmlSecAssert2(xmlSecOpenSSLEvpDigestCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLEvpDigestSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    in = &(transform->inBuf);
    xmlSecAssert2(in != NULL, -1);

    out = &(transform->outBuf);
    xmlSecAssert2(out != NULL, -1);

    ctx = xmlSecOpenSSLEvpDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->digest != NULL, -1);
    xmlSecAssert2(ctx->digestCtx != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        ret = EVP_DigestInit(ctx->digestCtx, ctx->digest);
        if(ret != 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "EVP_DigestInit",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        transform->status = xmlSecTransformStatusWorking;
    }

    if(transform->status == xmlSecTransformStatusWorking) {
        xmlSecSize inSize;

        inSize = xmlSecBufferGetSize(in);
        if(inSize > 0) {
            ret = EVP_DigestUpdate(ctx->digestCtx, xmlSecBufferGetData(in), inSize);
            if(ret != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "EVP_DigestUpdate",
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
            unsigned int dgstSize;

            xmlSecAssert2((xmlSecSize)EVP_MD_size(ctx->digest) <= sizeof(ctx->dgst), -1);

            ret = EVP_DigestFinal(ctx->digestCtx, ctx->dgst, &dgstSize);
            if(ret != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "EVP_DigestFinal",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            xmlSecAssert2(dgstSize > 0, -1);
            ctx->dgstSize = XMLSEC_SIZE_BAD_CAST(dgstSize);

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
static xmlSecTransformKlass xmlSecOpenSSLMd5Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameMd5,                              /* const xmlChar* name; */
    xmlSecHrefMd5,                              /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformMd5GetKlass:
 *
 * MD5 digest transform klass.
 *
 * Returns: pointer to MD5 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformMd5GetKlass(void) {
    return(&xmlSecOpenSSLMd5Klass);
}
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/******************************************************************************
 *
 * RIPEMD160
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLRipemd160Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameRipemd160,                        /* const xmlChar* name; */
    xmlSecHrefRipemd160,                        /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformRipemd160GetKlass:
 *
 * RIPEMD-160 digest transform klass.
 *
 * Returns: pointer to RIPEMD-160 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformRipemd160GetKlass(void) {
    return(&xmlSecOpenSSLRipemd160Klass);
}
#endif /* XMLSEC_NO_RIPEMD160 */


#ifndef XMLSEC_NO_SHA1
/******************************************************************************
 *
 * SHA1
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameSha1,                             /* const xmlChar* name; */
    xmlSecHrefSha1,                             /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformSha1GetKlass:
 *
 * SHA-1 digest transform klass.
 *
 * Returns: pointer to SHA-1 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformSha1GetKlass(void) {
    return(&xmlSecOpenSSLSha1Klass);
}
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA224
/******************************************************************************
 *
 * SHA224
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLSha224Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameSha224,                           /* const xmlChar* name; */
    xmlSecHrefSha224,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformSha224GetKlass:
 *
 * SHA-224 digest transform klass.
 *
 * Returns: pointer to SHA-224 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformSha224GetKlass(void) {
    return(&xmlSecOpenSSLSha224Klass);
}
#endif /* XMLSEC_NO_SHA224 */

#ifndef XMLSEC_NO_SHA256
/******************************************************************************
 *
 * SHA256
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLSha256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameSha256,                           /* const xmlChar* name; */
    xmlSecHrefSha256,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformSha256GetKlass:
 *
 * SHA-256 digest transform klass.
 *
 * Returns: pointer to SHA-256 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformSha256GetKlass(void) {
    return(&xmlSecOpenSSLSha256Klass);
}
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/******************************************************************************
 *
 * SHA384
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLSha384Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameSha384,                           /* const xmlChar* name; */
    xmlSecHrefSha384,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformSha384GetKlass:
 *
 * SHA-384 digest transform klass.
 *
 * Returns: pointer to SHA-384 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformSha384GetKlass(void) {
    return(&xmlSecOpenSSLSha384Klass);
}
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/******************************************************************************
 *
 * SHA512
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLSha512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLEvpDigestSize,                 /* xmlSecSize objSize */

    xmlSecNameSha512,                           /* const xmlChar* name; */
    xmlSecHrefSha512,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    xmlSecOpenSSLEvpDigestInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,               /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformSha512GetKlass:
 *
 * SHA-512 digest transform klass.
 *
 * Returns: pointer to SHA-512 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformSha512GetKlass(void) {
    return(&xmlSecOpenSSLSha512Klass);
}
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_GOST
/******************************************************************************
 *
 * GOSTR3411_94
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLGostR3411_94Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* size_t klassSize */
    xmlSecOpenSSLEvpDigestSize,                   /* size_t objSize */

    xmlSecNameGostR3411_94,                             /* const xmlChar* name; */
    xmlSecHrefGostR3411_94,                             /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */
    xmlSecOpenSSLEvpDigestInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,                 /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,                /* xmlSecTransformExecuteMethod execute; */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformGostR3411_94GetKlass:
 *
 * GOSTR3411_94 digest transform klass.
 *
 * Returns: pointer to GOSTR3411_94 digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformGostR3411_94GetKlass(void) {
    return(&xmlSecOpenSSLGostR3411_94Klass);
}
#endif /* XMLSEC_NO_GOST*/

#ifndef XMLSEC_NO_GOST2012

/******************************************************************************
 *
 * GOST R 34.11-2012 256 bit
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLGostR3411_2012_256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* size_t klassSize */
    xmlSecOpenSSLEvpDigestSize,                   /* size_t objSize */

    xmlSecNameGostR3411_2012_256,               /* const xmlChar* name; */
    xmlSecHrefGostR3411_2012_256,               /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */
    xmlSecOpenSSLEvpDigestInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,                 /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,                /* xmlSecTransformExecuteMethod execute; */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformGostR3411_2012_256GetKlass:
 *
 * GOST R 34.11-2012 256 bit digest transform klass.
 *
 * Returns: pointer to GOST R 34.11-2012 256 bit digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformGostR3411_2012_256GetKlass(void) {
    return(&xmlSecOpenSSLGostR3411_2012_256Klass);
}

/******************************************************************************
 *
 * GOST R 34.11-2012 512 bit
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecOpenSSLGostR3411_2012_512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* size_t klassSize */
    xmlSecOpenSSLEvpDigestSize,                   /* size_t objSize */

    xmlSecNameGostR3411_2012_512,               /* const xmlChar* name; */
    xmlSecHrefGostR3411_2012_512,               /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */
    xmlSecOpenSSLEvpDigestInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLEvpDigestFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecOpenSSLEvpDigestVerify,                 /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLEvpDigestExecute,                /* xmlSecTransformExecuteMethod execute; */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformGostR3411_2012_512GetKlass:
 *
 * GOST R 34.11-2012 512 bit digest transform klass.
 *
 * Returns: pointer to GOST R 34.11-2012 512 bit digest transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformGostR3411_2012_512GetKlass(void) {
    return(&xmlSecOpenSSLGostR3411_2012_512Klass);
}

#endif /* XMLSEC_NO_GOST2012 */


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

#include <gcrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/app.h>
#include <xmlsec/gcrypt/crypto.h>

/**************************************************************************
 *
 * Internal GCRYPT Digest CTX
 *
 *****************************************************************************/
typedef struct _xmlSecGCryptDigestCtx           xmlSecGCryptDigestCtx, *xmlSecGCryptDigestCtxPtr;
struct _xmlSecGCryptDigestCtx {
    int                 digest;
    gcry_md_hd_t        digestCtx;
    xmlSecByte          dgst[XMLSEC_GCRYPT_MAX_DIGEST_SIZE];
    xmlSecSize          dgstSize;       /* dgst size in bytes */
};

/******************************************************************************
 *
 * Digest transforms
 *
 * xmlSecGCryptDigestCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecGCryptDigestSize  \
    (sizeof(xmlSecTransform) + sizeof(xmlSecGCryptDigestCtx))
#define xmlSecGCryptDigestGetCtx(transform) \
    ((xmlSecGCryptDigestCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecGCryptDigestInitialize            (xmlSecTransformPtr transform);
static void     xmlSecGCryptDigestFinalize              (xmlSecTransformPtr transform);
static int      xmlSecGCryptDigestVerify                (xmlSecTransformPtr transform,
                                                         const xmlSecByte* data,
                                                         xmlSecSize dataSize,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecGCryptDigestExecute               (xmlSecTransformPtr transform,
                                                         int last,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecGCryptDigestCheckId               (xmlSecTransformPtr transform);

static int
xmlSecGCryptDigestCheckId(xmlSecTransformPtr transform) {

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha1Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha256Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha384Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha512Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformMd5Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRipemd160Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_RIPEMD160 */

    /* not found */
    {
        return(0);
    }

    /* just in case */
    return(0);
}

static int
xmlSecGCryptDigestInitialize(xmlSecTransformPtr transform) {
    xmlSecGCryptDigestCtxPtr ctx;
    gcry_error_t err;

    xmlSecAssert2(xmlSecGCryptDigestCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptDigestSize), -1);

    ctx = xmlSecGCryptDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* initialize context */
    memset(ctx, 0, sizeof(xmlSecGCryptDigestCtx));

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha1Id)) {
        ctx->digest = GCRY_MD_SHA1;
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha256Id)) {
        ctx->digest = GCRY_MD_SHA256;
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha384Id)) {
        ctx->digest = GCRY_MD_SHA384;
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformSha512Id)) {
        ctx->digest = GCRY_MD_SHA512;
    } else
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformMd5Id)) {
        ctx->digest = GCRY_MD_MD5;
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRipemd160Id)) {
        ctx->digest = GCRY_MD_RMD160;
    } else
#endif /* XMLSEC_NO_RIPEMD160 */

    if(1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* create digest ctx */
    err = gcry_md_open(&ctx->digestCtx, ctx->digest, GCRY_MD_FLAG_SECURE); /* we are paranoid */
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "gcry_md_open",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }
    return(0);
}

static void
xmlSecGCryptDigestFinalize(xmlSecTransformPtr transform) {
    xmlSecGCryptDigestCtxPtr ctx;

    xmlSecAssert(xmlSecGCryptDigestCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecGCryptDigestSize));

    ctx = xmlSecGCryptDigestGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->digestCtx != NULL) {
        gcry_md_close(ctx->digestCtx);
    }
    memset(ctx, 0, sizeof(xmlSecGCryptDigestCtx));
}

static int
xmlSecGCryptDigestVerify(xmlSecTransformPtr transform,
                        const xmlSecByte* data, xmlSecSize dataSize,
                        xmlSecTransformCtxPtr transformCtx) {
    xmlSecGCryptDigestCtxPtr ctx;

    xmlSecAssert2(xmlSecGCryptDigestCheckId(transform), -1);
    xmlSecAssert2(transform->operation == xmlSecTransformOperationVerify, -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptDigestSize), -1);
    xmlSecAssert2(transform->status == xmlSecTransformStatusFinished, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecGCryptDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->dgstSize > 0, -1);

    if(dataSize != ctx->dgstSize) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_DATA,
                    "data and digest sizes are different (data=%d, dgst=%d)",
                    dataSize, ctx->dgstSize);
        transform->status = xmlSecTransformStatusFail;
        return(0);
    }

    if(memcmp(ctx->dgst, data, dataSize) != 0) {
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
xmlSecGCryptDigestExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecGCryptDigestCtxPtr ctx;
    xmlSecBufferPtr in, out;
    int ret;

    xmlSecAssert2(xmlSecGCryptDigestCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(transformCtx != NULL, -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptDigestSize), -1);

    ctx = xmlSecGCryptDigestGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->digest != GCRY_MD_NONE, -1);
    xmlSecAssert2(ctx->digestCtx != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if(transform->status == xmlSecTransformStatusWorking) {
        xmlSecSize inSize;

        inSize = xmlSecBufferGetSize(in);
        if(inSize > 0) {
            gcry_md_write(ctx->digestCtx, xmlSecBufferGetData(in), inSize);

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
        if(last != 0) {
            xmlSecByte* buf;

            /* get the final digest */
            gcry_md_final(ctx->digestCtx);
            buf = gcry_md_read(ctx->digestCtx, ctx->digest);
            if(buf == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "gcry_md_read",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            /* copy it to our internal buffer */
            ctx->dgstSize = gcry_md_get_algo_dlen(ctx->digest);
            xmlSecAssert2(ctx->dgstSize > 0, -1);
            xmlSecAssert2(ctx->dgstSize <= sizeof(ctx->dgst), -1);
            memcpy(ctx->dgst, buf, ctx->dgstSize);

            /* and to the output if needed */
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

#ifndef XMLSEC_NO_SHA1
/******************************************************************************
 *
 * SHA1 Digest transforms
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecGCryptSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptDigestSize,                     /* xmlSecSize objSize */

    /* data */
    xmlSecNameSha1,                             /* const xmlChar* name; */
    xmlSecHrefSha1,                             /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    /* methods */
    xmlSecGCryptDigestInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptDigestFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptDigestVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptDigestExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformSha1GetKlass:
 *
 * SHA-1 digest transform klass.
 *
 * Returns: pointer to SHA-1 digest transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformSha1GetKlass(void) {
    return(&xmlSecGCryptSha1Klass);
}
#endif /* XMLSEC_NO_SHA1 */


#ifndef XMLSEC_NO_SHA256
/******************************************************************************
 *
 * SHA256 Digest transforms
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecGCryptSha256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptDigestSize,                     /* xmlSecSize objSize */

    /* data */
    xmlSecNameSha256,                           /* const xmlChar* name; */
    xmlSecHrefSha256,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    /* methods */
    xmlSecGCryptDigestInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptDigestFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptDigestVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptDigestExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformSha256GetKlass:
 *
 * SHA256 digest transform klass.
 *
 * Returns: pointer to SHA256 digest transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformSha256GetKlass(void) {
    return(&xmlSecGCryptSha256Klass);
}
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/******************************************************************************
 *
 * SHA384 Digest transforms
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecGCryptSha384Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptDigestSize,                     /* xmlSecSize objSize */

    /* data */
    xmlSecNameSha384,                           /* const xmlChar* name; */
    xmlSecHrefSha384,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    /* methods */
    xmlSecGCryptDigestInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptDigestFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptDigestVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptDigestExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformSha384GetKlass:
 *
 * SHA384 digest transform klass.
 *
 * Returns: pointer to SHA384 digest transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformSha384GetKlass(void) {
    return(&xmlSecGCryptSha384Klass);
}
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/******************************************************************************
 *
 * SHA512 Digest transforms
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecGCryptSha512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptDigestSize,                     /* xmlSecSize objSize */

    /* data */
    xmlSecNameSha512,                           /* const xmlChar* name; */
    xmlSecHrefSha512,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    /* methods */
    xmlSecGCryptDigestInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptDigestFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptDigestVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptDigestExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformSha512GetKlass:
 *
 * SHA512 digest transform klass.
 *
 * Returns: pointer to SHA512 digest transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformSha512GetKlass(void) {
    return(&xmlSecGCryptSha512Klass);
}
#endif /* XMLSEC_NO_SHA512 */

#ifndef XMLSEC_NO_MD5
/******************************************************************************
 *
 * MD5 Digest transforms
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecGCryptMd5Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptDigestSize,                     /* xmlSecSize objSize */

    /* data */
    xmlSecNameMd5,                           /* const xmlChar* name; */
    xmlSecHrefMd5,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    /* methods */
    xmlSecGCryptDigestInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptDigestFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptDigestVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptDigestExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformMd5GetKlass:
 *
 * MD5 digest transform klass.
 *
 * Returns: pointer to MD5 digest transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformMd5GetKlass(void) {
    return(&xmlSecGCryptMd5Klass);
}
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/******************************************************************************
 *
 * RIPEMD160 Digest transforms
 *
 *****************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRipemd160Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptDigestSize,                     /* xmlSecSize objSize */

    /* data */
    xmlSecNameRipemd160,                           /* const xmlChar* name; */
    xmlSecHrefRipemd160,                           /* const xmlChar* href; */
    xmlSecTransformUsageDigestMethod,           /* xmlSecTransformUsage usage; */

    /* methods */
    xmlSecGCryptDigestInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptDigestFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptDigestVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptDigestExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRipemd160GetKlass:
 *
 * RIPEMD160 digest transform klass.
 *
 * Returns: pointer to RIPEMD160 digest transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRipemd160GetKlass(void) {
    return(&xmlSecGCryptRipemd160Klass);
}
#endif /* XMLSEC_NO_RIPEMD160 */

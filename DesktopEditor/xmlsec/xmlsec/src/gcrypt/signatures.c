/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <gcrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/crypto.h>


/**************************************************************************
 *
 * Forward declarations for actual sign/verify implementations
 *
 *****************************************************************************/
typedef int     (*xmlSecGCryptPkSignMethod)     (int digest,
                                                 xmlSecKeyDataPtr key_data,
                                                 const xmlSecByte* dgst,
                                                 xmlSecSize dgstSize,
                                                 xmlSecBufferPtr out);
typedef int     (*xmlSecGCryptPkVerifyMethod)   (int digest,
                                                 xmlSecKeyDataPtr key_data,
                                                 const xmlSecByte* dgst,
                                                 xmlSecSize dgstSize,
                                                 const xmlSecByte* data,
                                                 xmlSecSize dataSize);

#ifndef XMLSEC_NO_DSA
static int      xmlSecGCryptDsaPkSign           (int digest,
                                                 xmlSecKeyDataPtr key_data,
                                                 const xmlSecByte* dgst,
                                                 xmlSecSize dgstSize,
                                                 xmlSecBufferPtr out);
static int      xmlSecGCryptDsaPkVerify         (int digest,
                                                 xmlSecKeyDataPtr key_data,
                                                 const xmlSecByte* dgst,
                                                 xmlSecSize dgstSize,
                                                 const xmlSecByte* data,
                                                 xmlSecSize dataSize);
#endif  /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA
static int      xmlSecGCryptRsaPkcs1PkSign      (int digest,
                                                 xmlSecKeyDataPtr key_data,
                                                 const xmlSecByte* dgst,
                                                 xmlSecSize dgstSize,
                                                 xmlSecBufferPtr out);
static int      xmlSecGCryptRsaPkcs1PkVerify    (int digest,
                                                 xmlSecKeyDataPtr key_data,
                                                 const xmlSecByte* dgst,
                                                 xmlSecSize dgstSize,
                                                 const xmlSecByte* data,
                                                 xmlSecSize dataSize);
#endif  /* XMLSEC_NO_RSA */


/**************************************************************************
 *
 * Internal GCrypt signatures ctx
 *
 *****************************************************************************/
typedef struct _xmlSecGCryptPkSignatureCtx      xmlSecGCryptPkSignatureCtx,
                                                *xmlSecGCryptPkSignatureCtxPtr;


struct _xmlSecGCryptPkSignatureCtx {
    int                         digest;
    xmlSecKeyDataId             keyId;
    xmlSecGCryptPkSignMethod    sign;
    xmlSecGCryptPkVerifyMethod  verify;

    gcry_md_hd_t                digestCtx;
    xmlSecKeyDataPtr            key_data;

    xmlSecByte                  dgst[XMLSEC_GCRYPT_MAX_DIGEST_SIZE];
    xmlSecSize                  dgstSize;       /* dgst size in bytes */
};


/******************************************************************************
 *
 * Pk Signature transforms
 *
 * xmlSecGCryptPkSignatureCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecGCryptPkSignatureSize   \
    (sizeof(xmlSecTransform) + sizeof(xmlSecGCryptPkSignatureCtx))
#define xmlSecGCryptPkSignatureGetCtx(transform) \
    ((xmlSecGCryptPkSignatureCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecGCryptPkSignatureCheckId                  (xmlSecTransformPtr transform);
static int      xmlSecGCryptPkSignatureInitialize               (xmlSecTransformPtr transform);
static void     xmlSecGCryptPkSignatureFinalize                 (xmlSecTransformPtr transform);
static int      xmlSecGCryptPkSignatureSetKeyReq                (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecGCryptPkSignatureSetKey                   (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecGCryptPkSignatureVerify                   (xmlSecTransformPtr transform,
                                                                 const xmlSecByte* data,
                                                                 xmlSecSize dataSize,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecGCryptPkSignatureExecute                  (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);

static int
xmlSecGCryptPkSignatureCheckId(xmlSecTransformPtr transform) {
#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformDsaSha1Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaMd5Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaRipemd160Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha1Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha256Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha384Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha512Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

    {
        return(0);
    }

    return(0);
}

static int
xmlSecGCryptPkSignatureInitialize(xmlSecTransformPtr transform) {
    xmlSecGCryptPkSignatureCtxPtr ctx;
    gcry_error_t err;

    xmlSecAssert2(xmlSecGCryptPkSignatureCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptPkSignatureSize), -1);

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecGCryptPkSignatureCtx));

#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformDsaSha1Id)) {
        ctx->digest     = GCRY_MD_SHA1;
        ctx->keyId      = xmlSecGCryptKeyDataDsaId;
        ctx->sign       = xmlSecGCryptDsaPkSign;
        ctx->verify     = xmlSecGCryptDsaPkVerify;
    } else
#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaMd5Id)) {
        ctx->digest     = GCRY_MD_MD5;
        ctx->keyId      = xmlSecGCryptKeyDataRsaId;
        ctx->sign       = xmlSecGCryptRsaPkcs1PkSign;
        ctx->verify     = xmlSecGCryptRsaPkcs1PkVerify;
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaRipemd160Id)) {
        ctx->digest     = GCRY_MD_RMD160;
        ctx->keyId      = xmlSecGCryptKeyDataRsaId;
        ctx->sign       = xmlSecGCryptRsaPkcs1PkSign;
        ctx->verify     = xmlSecGCryptRsaPkcs1PkVerify;
    } else
#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha1Id)) {
        ctx->digest     = GCRY_MD_SHA1;
        ctx->keyId      = xmlSecGCryptKeyDataRsaId;
        ctx->sign       = xmlSecGCryptRsaPkcs1PkSign;
        ctx->verify     = xmlSecGCryptRsaPkcs1PkVerify;
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha256Id)) {
        ctx->digest     = GCRY_MD_SHA256;
        ctx->keyId      = xmlSecGCryptKeyDataRsaId;
        ctx->sign       = xmlSecGCryptRsaPkcs1PkSign;
        ctx->verify     = xmlSecGCryptRsaPkcs1PkVerify;
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha384Id)) {
        ctx->digest     = GCRY_MD_SHA384;
        ctx->keyId      = xmlSecGCryptKeyDataRsaId;
        ctx->sign       = xmlSecGCryptRsaPkcs1PkSign;
        ctx->verify     = xmlSecGCryptRsaPkcs1PkVerify;
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecGCryptTransformRsaSha512Id)) {
        ctx->digest     = GCRY_MD_SHA512;
        ctx->keyId      = xmlSecGCryptKeyDataRsaId;
        ctx->sign       = xmlSecGCryptRsaPkcs1PkSign;
        ctx->verify     = xmlSecGCryptRsaPkcs1PkVerify;
    } else
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

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

    /* done */
    return(0);
}

static void
xmlSecGCryptPkSignatureFinalize(xmlSecTransformPtr transform) {
    xmlSecGCryptPkSignatureCtxPtr ctx;

    xmlSecAssert(xmlSecGCryptPkSignatureCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecGCryptPkSignatureSize));

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->key_data != NULL) {
        xmlSecKeyDataDestroy(ctx->key_data);
    }
    if(ctx->digestCtx != NULL) {
        gcry_md_close(ctx->digestCtx);
    }

    memset(ctx, 0, sizeof(xmlSecGCryptPkSignatureCtx));
}

static int
xmlSecGCryptPkSignatureSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecGCryptPkSignatureCtxPtr ctx;
    xmlSecKeyDataPtr key_data;

    xmlSecAssert2(xmlSecGCryptPkSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptPkSignatureSize), -1);
    xmlSecAssert2(key != NULL, -1);

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);
    xmlSecAssert2(xmlSecKeyCheckId(key, ctx->keyId), -1);

    key_data = xmlSecKeyGetValue(key);
    xmlSecAssert2(key_data != NULL, -1);

    if(ctx->key_data != NULL) {
        xmlSecKeyDataDestroy(ctx->key_data);
    }

    ctx->key_data = xmlSecKeyDataDuplicate(key_data);
    if(ctx->key_data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecKeyDataDuplicate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static int
xmlSecGCryptPkSignatureSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecGCryptPkSignatureCtxPtr ctx;

    xmlSecAssert2(xmlSecGCryptPkSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptPkSignatureSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);

    keyReq->keyId        = ctx->keyId;
    if(transform->operation == xmlSecTransformOperationSign) {
        keyReq->keyType  = xmlSecKeyDataTypePrivate;
        keyReq->keyUsage = xmlSecKeyUsageSign;
    } else {
        keyReq->keyType  = xmlSecKeyDataTypePublic;
        keyReq->keyUsage = xmlSecKeyUsageVerify;
    }
    return(0);
}


static int
xmlSecGCryptPkSignatureVerify(xmlSecTransformPtr transform,
                        const xmlSecByte* data, xmlSecSize dataSize,
                        xmlSecTransformCtxPtr transformCtx) {
    xmlSecGCryptPkSignatureCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecGCryptPkSignatureCheckId(transform), -1);
    xmlSecAssert2(transform->operation == xmlSecTransformOperationVerify, -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptPkSignatureSize), -1);
    xmlSecAssert2(transform->status == xmlSecTransformStatusFinished, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->sign != NULL, -1);
    xmlSecAssert2(ctx->verify != NULL, -1);
    xmlSecAssert2(ctx->dgstSize > 0, -1);
    xmlSecAssert2(ctx->key_data != NULL, -1);

    ret = ctx->verify(ctx->digest, ctx->key_data, ctx->dgst, ctx->dgstSize, data, dataSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "ctx->verify",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* check result */
    if(ret == 1) {
        transform->status = xmlSecTransformStatusOk;
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "ctx->verify",
                    XMLSEC_ERRORS_R_DATA_NOT_MATCH,
                    "signature do not match");
        transform->status = xmlSecTransformStatusFail;
    }

    /* done */
    return(0);
}

static int
xmlSecGCryptPkSignatureExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecGCryptPkSignatureCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize;
    xmlSecSize outSize;
    int ret;

    xmlSecAssert2(xmlSecGCryptPkSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecGCryptPkSignatureSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->sign != NULL, -1);
    xmlSecAssert2(ctx->verify != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);
    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);

    ctx = xmlSecGCryptPkSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->key_data != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        /* do nothing, already initialized */
        transform->status = xmlSecTransformStatusWorking;
    }

    if(transform->status == xmlSecTransformStatusWorking) {
        xmlSecAssert2(outSize == 0, -1);

        /* update the digest */
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

        /* generate digest and signature */
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

            xmlSecAssert2(outSize == 0, -1);
            if(transform->operation == xmlSecTransformOperationSign) {
                ret = ctx->sign(ctx->digest, ctx->key_data, ctx->dgst, ctx->dgstSize, out);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "ctx->sign",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    return(-1);
                }
            }

            /* done */
            transform->status = xmlSecTransformStatusFinished;
        }
    }

    if((transform->status == xmlSecTransformStatusWorking) || (transform->status == xmlSecTransformStatusFinished)) {
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

/*****************************************************************************
 *
 * Helper
 *
 ****************************************************************************/
static int
xmlSecGCryptAppendMpi(gcry_mpi_t a, xmlSecBufferPtr out, xmlSecSize min_size) {
    xmlSecSize outSize;
    size_t written;
    gpg_error_t err;
    int ret;

    xmlSecAssert2(a != NULL, -1);
    xmlSecAssert2(out != NULL, -1);

    /* current size */
    outSize = xmlSecBufferGetSize(out);

    /* figure out how much space we need */
    written = 0;
    err = gcry_mpi_print(GCRYMPI_FMT_USG, NULL, 0, &written, a);
    if((err != GPG_ERR_NO_ERROR) || (written == 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_print",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    /* add zeros at the beggining (if needed) */
    if((min_size > 0) && (written < min_size)) {
        outSize += (min_size - written);
    }

    /* allocate space */
    ret = xmlSecBufferSetMaxSize(out, outSize + written + 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferSetMaxSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", (int)(outSize + written + 1));
        return(-1);
    }
    xmlSecAssert2(xmlSecBufferGetMaxSize(out) > outSize, -1);

    /* add zeros at the beggining (if needed) */
    if((min_size > 0) && (written < min_size)) {
        xmlSecSize ii;
        xmlSecByte * p = xmlSecBufferGetData(out);

        for(ii = 0; ii < (min_size - written); ++ii) {
            p[outSize - ii - 1] = 0;
        }
    }

    /* write out */
    written = 0;
    err = gcry_mpi_print(GCRYMPI_FMT_USG,
            xmlSecBufferGetData(out) + outSize,
            xmlSecBufferGetMaxSize(out) - outSize,
            &written, a);
    if((err != GPG_ERR_NO_ERROR) || (written == 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_print",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    /* reset size */
    ret = xmlSecBufferSetSize(out, outSize + written);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferSetSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d",
                    (int)(outSize + written));
        return(-1);
    }

    /* done */
    return(0);
}

#ifndef XMLSEC_NO_DSA

#ifndef XMLSEC_NO_SHA1
/****************************************************************************
 *
 * DSA-SHA1 signature transform
 *
 * http://www.w3.org/TR/xmldsig-core/#sec-SignatureAlg:
 *
 * The output of the DSA algorithm consists of a pair of integers
 * usually referred by the pair (r, s). The signature value consists of
 * the base64 encoding of the concatenation of two octet-streams that
 * respectively result from the octet-encoding of the values r and s in
 * that order. Integer to octet-stream conversion must be done according
 * to the I2OSP operation defined in the RFC 2437 [PKCS1] specification
 * with a l parameter equal to 20. For example, the SignatureValue element
 * for a DSA signature (r, s) with values specified in hexadecimal:
 *
 *  r = 8BAC1AB6 6410435C B7181F95 B16AB97C 92B341C0
 *  s = 41E2345F 1F56DF24 58F426D1 55B4BA2D B6DCD8C8
 *
 * from the example in Appendix 5 of the DSS standard would be
 *
 * <SignatureValue>i6watmQQQ1y3GB+VsWq5fJKzQcBB4jRfH1bfJFj0JtFVtLotttzYyA==</SignatureValue>
 *
 ***************************************************************************/
static int
xmlSecGCryptDsaPkSign(int digest ATTRIBUTE_UNUSED, xmlSecKeyDataPtr key_data,
                      const xmlSecByte* dgst, xmlSecSize dgstSize,
                      xmlSecBufferPtr out) {
    gcry_mpi_t m_hash = NULL;
    gcry_sexp_t s_data = NULL;
    gcry_sexp_t s_sig = NULL;
    gcry_sexp_t s_r = NULL;
    gcry_sexp_t s_s = NULL;
    gcry_mpi_t m_r = NULL;
    gcry_mpi_t m_s = NULL;
    gcry_sexp_t s_tmp;
    gpg_error_t err;
    int ret;
    int res = -1;

    xmlSecAssert2(key_data != NULL, -1);
    xmlSecAssert2(xmlSecGCryptKeyDataDsaGetPrivateKey(key_data) != NULL, -1);
    xmlSecAssert2(dgst != NULL, -1);
    xmlSecAssert2(dgstSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);

    /* get the current digest, can't use "hash" :( */
    err = gcry_mpi_scan(&m_hash, GCRYMPI_FMT_USG, dgst, dgstSize, NULL);
    if((err != GPG_ERR_NO_ERROR) || (m_hash == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_scan(hash)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    err = gcry_sexp_build (&s_data, NULL,
                           "(data (flags raw)(value %m))",
                           m_hash);
    if((err != GPG_ERR_NO_ERROR) || (s_data == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(data)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* create signature */
    err = gcry_pk_sign(&s_sig, s_data, xmlSecGCryptKeyDataDsaGetPrivateKey(key_data));
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_pk_sign",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* find signature value */
    s_tmp = gcry_sexp_find_token(s_sig, "sig-val", 0);
    if(s_tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(sig-val)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    gcry_sexp_release(s_sig);
    s_sig = s_tmp;

    s_tmp = gcry_sexp_find_token(s_sig, "dsa", 0);
    if(s_tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(rsa)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    gcry_sexp_release(s_sig);
    s_sig = s_tmp;

    /* r */
    s_r = gcry_sexp_find_token(s_sig, "r", 0);
    if(s_r == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(r)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    m_r = gcry_sexp_nth_mpi(s_r, 1, GCRYMPI_FMT_USG);
    if(m_r == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_nth_mpi(r)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* s */
    s_s = gcry_sexp_find_token(s_sig, "s", 0);
    if(s_s == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(s)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    m_s = gcry_sexp_nth_mpi(s_s, 1, GCRYMPI_FMT_USG);
    if(m_s == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_nth_mpi(s)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* write out: r + s */
    ret = xmlSecGCryptAppendMpi(m_r, out, 20);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAppendMpi",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    xmlSecAssert2(xmlSecBufferGetSize(out) == 20, -1);
    ret = xmlSecGCryptAppendMpi(m_s, out, 20);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAppendMpi",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    xmlSecAssert2(xmlSecBufferGetSize(out) == (20 + 20), -1);

    /* done */
    res = 0;

done:
    if(m_hash != NULL) {
        gcry_mpi_release(m_hash);
    }
    if(m_r != NULL) {
        gcry_mpi_release(m_r);
    }
    if(m_s != NULL) {
        gcry_mpi_release(m_s);
    }

    if(s_data != NULL) {
        gcry_sexp_release(s_data);
    }
    if(s_sig != NULL) {
        gcry_sexp_release(s_sig);
    }
    if(s_r != NULL) {
        gcry_sexp_release(s_r);
    }
    if(s_s != NULL) {
        gcry_sexp_release(s_s);
    }

    return(res);
}

static int
xmlSecGCryptDsaPkVerify(int digest ATTRIBUTE_UNUSED, xmlSecKeyDataPtr key_data,
                        const xmlSecByte* dgst, xmlSecSize dgstSize,
                        const xmlSecByte* data, xmlSecSize dataSize) {
    gcry_mpi_t m_hash = NULL;
    gcry_sexp_t s_data = NULL;
    gcry_mpi_t m_sig_r = NULL;
    gcry_mpi_t m_sig_s = NULL;
    gcry_sexp_t s_sig = NULL;
    gpg_error_t err;
    int res = -1;

    xmlSecAssert2(key_data != NULL, -1);
    xmlSecAssert2(xmlSecGCryptKeyDataDsaGetPublicKey(key_data) != NULL, -1);
    xmlSecAssert2(dgst != NULL, -1);
    xmlSecAssert2(dgstSize > 0, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize == (20 + 20), -1);

    /* get the current digest, can't use "hash" :( */
    err = gcry_mpi_scan(&m_hash, GCRYMPI_FMT_USG, dgst, dgstSize, NULL);
    if((err != GPG_ERR_NO_ERROR) || (m_hash == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_scan(hash)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    err = gcry_sexp_build (&s_data, NULL,
                           "(data (flags raw)(value %m))",
                           m_hash);
    if((err != GPG_ERR_NO_ERROR) || (s_data == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(data)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* get the existing signature */
    err = gcry_mpi_scan(&m_sig_r, GCRYMPI_FMT_USG, data, 20, NULL);
    if((err != GPG_ERR_NO_ERROR) || (m_sig_r == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_scan(r)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }
    err = gcry_mpi_scan(&m_sig_s, GCRYMPI_FMT_USG, data + 20, 20, NULL);
    if((err != GPG_ERR_NO_ERROR) || (m_sig_s == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_scan(s)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    err = gcry_sexp_build (&s_sig, NULL,
                           "(sig-val(dsa(r %m)(s %m)))",
                           m_sig_r, m_sig_s);
    if((err != GPG_ERR_NO_ERROR) || (s_sig == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(sig-val)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* verify signature */
    err = gcry_pk_verify(s_sig, s_data, xmlSecGCryptKeyDataDsaGetPublicKey(key_data));
    if(err == GPG_ERR_NO_ERROR) {
        res = 1; /* good signature */
    } else if(err == GPG_ERR_BAD_SIGNATURE) {
        res = 0; /* bad signature */
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_pk_verify",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* done */
done:
    if(m_hash != NULL) {
        gcry_mpi_release(m_hash);
    }
    if(m_sig_r != NULL) {
        gcry_mpi_release(m_sig_r);
    }
    if(m_sig_s != NULL) {
        gcry_mpi_release(m_sig_s);
    }

    if(s_data != NULL) {
        gcry_sexp_release(s_data);
    }
    if(s_sig != NULL) {
        gcry_sexp_release(s_sig);
    }

    return(res);
}


static xmlSecTransformKlass xmlSecGCryptDsaSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameDsaSha1,                          /* const xmlChar* name; */
    xmlSecHrefDsaSha1,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformDsaSha1GetKlass:
 *
 * The DSA-SHA1 signature transform klass.
 *
 * Returns: DSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformDsaSha1GetKlass(void) {
    return(&xmlSecGCryptDsaSha1Klass);
}

#endif /* XMLSEC_NO_SHA1 */

#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

/****************************************************************************
 *
 * RSA-SHA1 signature transform
 *
 * http://www.w3.org/TR/xmldsig-core/#sec-SignatureAlg:
 *
 * The SignatureValue content for an RSA signature is the base64 [MIME]
 * encoding of the octet string computed as per RFC 2437  [PKCS1,
 * section 8.1.1: Signature generation for the RSASSA-PKCS1-v1_5 signature
 * scheme]. As specified in the EMSA-PKCS1-V1_5-ENCODE function RFC 2437
 * [PKCS1, section 9.2.1], the value input to the signature function MUST
 * contain a pre-pended algorithm object identifier for the hash function,
 * but the availability of an ASN.1 parser and recognition of OIDs is not
 * required of a signature verifier. The PKCS#1 v1.5 representation appears
 * as:
 *
 *      CRYPT (PAD (ASN.1 (OID, DIGEST (data))))
 *
 * Note that the padded ASN.1 will be of the following form:
 *
 *      01 | FF* | 00 | prefix | hash
 *
 * where "|" is concatenation, "01", "FF", and "00" are fixed octets of
 * the corresponding hexadecimal value, "hash" is the SHA1 digest of the
 * data, and "prefix" is the ASN.1 BER SHA1 algorithm designator prefix
 * required in PKCS1 [RFC 2437], that is,
 *
 *      hex 30 21 30 09 06 05 2B 0E 03 02 1A 05 00 04 14
 *
 * This prefix is included to make it easier to use standard cryptographic
 * libraries. The FF octet MUST be repeated the maximum number of times such
 * that the value of the quantity being CRYPTed is one octet shorter than 
 * the RSA modulus.
 *
 ***************************************************************************/
static int
xmlSecGCryptRsaPkcs1PkSign(int digest, xmlSecKeyDataPtr key_data,
                           const xmlSecByte* dgst, xmlSecSize dgstSize,
                           xmlSecBufferPtr out) {
    gcry_sexp_t s_data = NULL;
    gcry_mpi_t m_sig = NULL;
    gcry_sexp_t s_sig = NULL;
    gcry_sexp_t s_tmp;
    gpg_error_t err;
    int ret;
    int res = -1;

    xmlSecAssert2(key_data != NULL, -1);
    xmlSecAssert2(xmlSecGCryptKeyDataRsaGetPrivateKey(key_data) != NULL, -1);
    xmlSecAssert2(dgst != NULL, -1);
    xmlSecAssert2(dgstSize > 0, -1);
    xmlSecAssert2(out != NULL, -1);

    /* get the current digest */
    err = gcry_sexp_build (&s_data, NULL,
                           "(data (flags pkcs1)(hash %s %b))",
                           gcry_md_algo_name(digest),
                           (int)dgstSize, dgst);
    if((err != GPG_ERR_NO_ERROR) || (s_data == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(data)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* create signature */
    err = gcry_pk_sign(&s_sig, s_data, xmlSecGCryptKeyDataRsaGetPrivateKey(key_data));
    if(err != GPG_ERR_NO_ERROR) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_pk_sign",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* find signature value */
    s_tmp = gcry_sexp_find_token(s_sig, "sig-val", 0);
    if(s_tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(sig-val)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    gcry_sexp_release(s_sig);
    s_sig = s_tmp;

    s_tmp = gcry_sexp_find_token(s_sig, "rsa", 0);
    if(s_tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(rsa)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    gcry_sexp_release(s_sig);
    s_sig = s_tmp;

    s_tmp = gcry_sexp_find_token(s_sig, "s", 0);
    if(s_tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(s)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    gcry_sexp_release(s_sig);
    s_sig = s_tmp;

    m_sig = gcry_sexp_nth_mpi(s_sig, 1, GCRYMPI_FMT_USG);
    if(m_sig == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_nth_mpi(1)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* write out */
    ret = xmlSecGCryptAppendMpi(m_sig, out, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAppendMpi",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* done */
    res = 0;

done:
    if(m_sig != NULL) {
        gcry_mpi_release(m_sig);
    }

    if(s_data != NULL) {
        gcry_sexp_release(s_data);
    }
    if(s_sig != NULL) {
        gcry_sexp_release(s_sig);
    }

    return(res);
}

static int
xmlSecGCryptRsaPkcs1PkVerify(int digest, xmlSecKeyDataPtr key_data,
                             const xmlSecByte* dgst, xmlSecSize dgstSize,
                             const xmlSecByte* data, xmlSecSize dataSize) {
    gcry_sexp_t s_data = NULL;
    gcry_mpi_t m_sig = NULL;
    gcry_sexp_t s_sig = NULL;
    gpg_error_t err;
    int res = -1;

    xmlSecAssert2(key_data != NULL, -1);
    xmlSecAssert2(xmlSecGCryptKeyDataRsaGetPublicKey(key_data) != NULL, -1);
    xmlSecAssert2(dgst != NULL, -1);
    xmlSecAssert2(dgstSize > 0, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize > 0, -1);

    /* get the current digest */
    err = gcry_sexp_build (&s_data, NULL,
                           "(data (flags pkcs1)(hash %s %b))",
                           gcry_md_algo_name(digest),
                           (int)dgstSize, dgst);
    if((err != GPG_ERR_NO_ERROR) || (s_data == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(data)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* get the existing signature */
    err = gcry_mpi_scan(&m_sig, GCRYMPI_FMT_USG, data, dataSize, NULL);
    if((err != GPG_ERR_NO_ERROR) || (m_sig == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_scan",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    err = gcry_sexp_build (&s_sig, NULL,
                           "(sig-val(rsa(s %m)))",
                           m_sig);
    if((err != GPG_ERR_NO_ERROR) || (s_sig == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(sig-val)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* verify signature */
    err = gcry_pk_verify(s_sig, s_data, xmlSecGCryptKeyDataRsaGetPublicKey(key_data));
    if(err == GPG_ERR_NO_ERROR) {
        res = 1; /* good signature */
    } else if(err == GPG_ERR_BAD_SIGNATURE) {
        res = 0; /* bad signature */
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_pk_verify",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    /* done */
done:
    if(m_sig != NULL) {
        gcry_mpi_release(m_sig);
    }

    if(s_data != NULL) {
        gcry_sexp_release(s_data);
    }
    if(s_sig != NULL) {
        gcry_sexp_release(s_sig);
    }

    return(res);
}


#ifndef XMLSEC_NO_MD5
/****************************************************************************
 *
 * RSA-MD5 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRsaMd5Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaMd5,                           /* const xmlChar* name; */
    xmlSecHrefRsaMd5,                           /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRsaMd5GetKlass:
 *
 * The RSA-MD5 signature transform klass.
 *
 * Returns: RSA-MD5 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRsaMd5GetKlass(void) {
    return(&xmlSecGCryptRsaMd5Klass);
}

#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_RIPEMD160
/****************************************************************************
 *
 * RSA-RIPEMD160 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRsaRipemd160Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaRipemd160,                     /* const xmlChar* name; */
    xmlSecHrefRsaRipemd160,                     /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRsaRipemd160GetKlass:
 *
 * The RSA-RIPEMD160 signature transform klass.
 *
 * Returns: RSA-RIPEMD160 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRsaRipemd160GetKlass(void) {
    return(&xmlSecGCryptRsaRipemd160Klass);
}

#endif /* XMLSEC_NO_RIPEMD160 */

#ifndef XMLSEC_NO_SHA1
/****************************************************************************
 *
 * RSA-SHA1 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRsaSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaSha1,                          /* const xmlChar* name; */
    xmlSecHrefRsaSha1,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRsaSha1GetKlass:
 *
 * The RSA-SHA1 signature transform klass.
 *
 * Returns: RSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRsaSha1GetKlass(void) {
    return(&xmlSecGCryptRsaSha1Klass);
}

#endif /* XMLSEC_NO_SHA1 */


#ifndef XMLSEC_NO_SHA256
/****************************************************************************
 *
 * RSA-SHA256 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRsaSha256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaSha256,                        /* const xmlChar* name; */
    xmlSecHrefRsaSha256,                        /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRsaSha256GetKlass:
 *
 * The RSA-SHA256 signature transform klass.
 *
 * Returns: RSA-SHA256 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRsaSha256GetKlass(void) {
    return(&xmlSecGCryptRsaSha256Klass);
}

#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/****************************************************************************
 *
 * RSA-SHA384 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRsaSha384Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaSha384,                        /* const xmlChar* name; */
    xmlSecHrefRsaSha384,                        /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRsaSha384GetKlass:
 *
 * The RSA-SHA384 signature transform klass.
 *
 * Returns: RSA-SHA384 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRsaSha384GetKlass(void) {
    return(&xmlSecGCryptRsaSha384Klass);
}

#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/****************************************************************************
 *
 * RSA-SHA512 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecGCryptRsaSha512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecGCryptPkSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaSha512,                        /* const xmlChar* name; */
    xmlSecHrefRsaSha512,                        /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecGCryptPkSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecGCryptPkSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecGCryptPkSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecGCryptPkSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecGCryptPkSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecGCryptPkSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptTransformRsaSha512GetKlass:
 *
 * The RSA-SHA512 signature transform klass.
 *
 * Returns: RSA-SHA512 signature transform klass.
 */
xmlSecTransformId
xmlSecGCryptTransformRsaSha512GetKlass(void) {
    return(&xmlSecGCryptRsaSha512Klass);
}

#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */




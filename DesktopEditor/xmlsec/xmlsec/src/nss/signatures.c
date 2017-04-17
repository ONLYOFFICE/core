/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#include "globals.h"

#include <string.h>

#include <cryptohi.h>
#include <keyhi.h>
#include <secerr.h>
#include <prmem.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/pkikeys.h>


/**************************************************************************
 *
 * Internal NSS signatures ctx
 *
 *****************************************************************************/
typedef struct _xmlSecNssSignatureCtx   xmlSecNssSignatureCtx,
                                        *xmlSecNssSignatureCtxPtr;
struct _xmlSecNssSignatureCtx {
    xmlSecKeyDataId     keyId;
    SECOidTag           alg;

    union {
        struct {
            SGNContext         *sigctx;
            SECKEYPrivateKey   *privkey;
        } sig;

        struct {
            VFYContext         *vfyctx;
            SECKEYPublicKey    *pubkey;
        } vfy;
    } u;
};

/******************************************************************************
 *
 * Signature transforms
 *
 * xmlSecNssSignatureCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecNssSignatureSize  \
    (sizeof(xmlSecTransform) + sizeof(xmlSecNssSignatureCtx))
#define xmlSecNssSignatureGetCtx(transform) \
    ((xmlSecNssSignatureCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecNssSignatureCheckId               (xmlSecTransformPtr transform);
static int      xmlSecNssSignatureInitialize            (xmlSecTransformPtr transform);
static void     xmlSecNssSignatureFinalize              (xmlSecTransformPtr transform);
static int      xmlSecNssSignatureSetKeyReq             (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecNssSignatureSetKey                        (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecNssSignatureVerify                        (xmlSecTransformPtr transform,
                                                                 const xmlSecByte* data,
                                                                 xmlSecSize dataSize,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecNssSignatureExecute               (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);

static int
xmlSecNssSignatureCheckId(xmlSecTransformPtr transform) {
#ifndef XMLSEC_NO_DSA
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformDsaSha1Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaMd5Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha1Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha256Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha384Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha512Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

    return(0);
}

static int
xmlSecNssSignatureInitialize(xmlSecTransformPtr transform) {
    xmlSecNssSignatureCtxPtr ctx;

    xmlSecAssert2(xmlSecNssSignatureCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssSignatureSize), -1);
    ctx = xmlSecNssSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecNssSignatureCtx));

#ifndef XMLSEC_NO_DSA
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformDsaSha1Id)) {
        ctx->keyId      = xmlSecNssKeyDataDsaId;
        /* This creates a signature which is ASN1 encoded */
        ctx->alg        = SEC_OID_ANSIX9_DSA_SIGNATURE_WITH_SHA1_DIGEST;
    } else
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaMd5Id)) {
        ctx->keyId      = xmlSecNssKeyDataRsaId;
        ctx->alg        = SEC_OID_PKCS1_MD5_WITH_RSA_ENCRYPTION;
    } else
#endif /* XMLSEC_NO_MD5 */


#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha1Id)) {
        ctx->keyId      = xmlSecNssKeyDataRsaId;
        ctx->alg        = SEC_OID_PKCS1_SHA1_WITH_RSA_ENCRYPTION;
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha256Id)) {
        ctx->keyId      = xmlSecNssKeyDataRsaId;
        ctx->alg        = SEC_OID_PKCS1_SHA256_WITH_RSA_ENCRYPTION;
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha384Id)) {
        ctx->keyId      = xmlSecNssKeyDataRsaId;
        ctx->alg        = SEC_OID_PKCS1_SHA384_WITH_RSA_ENCRYPTION;
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecNssTransformRsaSha512Id)) {
        ctx->keyId      = xmlSecNssKeyDataRsaId;
        ctx->alg        = SEC_OID_PKCS1_SHA512_WITH_RSA_ENCRYPTION;
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

    return(0);
}

static void
xmlSecNssSignatureFinalize(xmlSecTransformPtr transform) {
    xmlSecNssSignatureCtxPtr ctx;

    xmlSecAssert(xmlSecNssSignatureCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecNssSignatureSize));
    xmlSecAssert((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify));

    ctx = xmlSecNssSignatureGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if (transform->operation == xmlSecTransformOperationSign) {
        SGN_DestroyContext(ctx->u.sig.sigctx, PR_TRUE);
        if (ctx->u.sig.privkey) {
            SECKEY_DestroyPrivateKey(ctx->u.sig.privkey);
        }
    } else {
        VFY_DestroyContext(ctx->u.vfy.vfyctx, PR_TRUE);
        if (ctx->u.vfy.pubkey) {
            SECKEY_DestroyPublicKey(ctx->u.vfy.pubkey);
        }
    }

    memset(ctx, 0, sizeof(xmlSecNssSignatureCtx));
}

static int
xmlSecNssSignatureSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecNssSignatureCtxPtr ctx;
    xmlSecKeyDataPtr value;

    xmlSecAssert2(xmlSecNssSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssSignatureSize), -1);
    xmlSecAssert2(key != NULL, -1);

    ctx = xmlSecNssSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);
    xmlSecAssert2(xmlSecKeyCheckId(key, ctx->keyId), -1);

    value = xmlSecKeyGetValue(key);
    xmlSecAssert2(value != NULL, -1);

    if (transform->operation == xmlSecTransformOperationSign) {
        if (ctx->u.sig.privkey)
            SECKEY_DestroyPrivateKey(ctx->u.sig.privkey);
        ctx->u.sig.privkey = xmlSecNssPKIKeyDataGetPrivKey(value);
        if(ctx->u.sig.privkey == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecNssPKIKeyDataGetPrivKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        ctx->u.sig.sigctx = SGN_NewContext(ctx->alg, ctx->u.sig.privkey);
        if (ctx->u.sig.sigctx == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "SGN_NewContext",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            return(-1);
        }
    } else {
        if (ctx->u.vfy.pubkey)
            SECKEY_DestroyPublicKey(ctx->u.vfy.pubkey);
        ctx->u.vfy.pubkey = xmlSecNssPKIKeyDataGetPubKey(value);
        if(ctx->u.vfy.pubkey == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecNssPKIKeyDataGetPubKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        ctx->u.vfy.vfyctx = VFY_CreateContext(ctx->u.vfy.pubkey, NULL,
                                              ctx->alg, NULL);
        if (ctx->u.vfy.vfyctx == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "VFY_CreateContext",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            return(-1);
        }
    }

    return(0);
}

static int
xmlSecNssSignatureSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecNssSignatureCtxPtr ctx;

    xmlSecAssert2(xmlSecNssSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssSignatureSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecNssSignatureGetCtx(transform);
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
xmlSecNssSignatureVerify(xmlSecTransformPtr transform,
                        const xmlSecByte* data, xmlSecSize dataSize,
                        xmlSecTransformCtxPtr transformCtx) {
    xmlSecNssSignatureCtxPtr ctx;
    SECStatus status;
    SECItem   signature;

    xmlSecAssert2(xmlSecNssSignatureCheckId(transform), -1);
    xmlSecAssert2(transform->operation == xmlSecTransformOperationVerify, -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssSignatureSize), -1);
    xmlSecAssert2(transform->status == xmlSecTransformStatusFinished, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecNssSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    signature.data = (unsigned char *)data;
    signature.len = dataSize;

    if(ctx->alg == SEC_OID_ANSIX9_DSA_SIGNATURE_WITH_SHA1_DIGEST) {
        /* This creates a signature which is ASN1 encoded */
        SECItem   signatureDer;
        SECStatus statusDer;

        statusDer = DSAU_EncodeDerSig(&signatureDer, &signature);
        if(statusDer != SECSuccess) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "DSAU_EncodeDerSig",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", 
                    PORT_GetError());
            return(-1);
        }
        status = VFY_EndWithSignature(ctx->u.vfy.vfyctx, &signatureDer);
        SECITEM_FreeItem(&signatureDer, PR_FALSE);
    } else {
        status = VFY_EndWithSignature(ctx->u.vfy.vfyctx, &signature);
    }

    if (status != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "VFY_EndWithSignature",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", 
                    PORT_GetError());

        if (PORT_GetError() == SEC_ERROR_PKCS7_BAD_SIGNATURE) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "VFY_EndWithSignature",
                        XMLSEC_ERRORS_R_DATA_NOT_MATCH,
                        "signature does not verify");
            transform->status = xmlSecTransformStatusFail;
        }
        return(-1);
    }

    transform->status = xmlSecTransformStatusOk;
    return(0);
}

static int
xmlSecNssSignatureExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecNssSignatureCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    SECStatus status;
    SECItem signature;
    int ret;

    xmlSecAssert2(xmlSecNssSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecNssSignatureSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecNssSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);
    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);

    ctx = xmlSecNssSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    if(transform->operation == xmlSecTransformOperationSign) {
        xmlSecAssert2(ctx->u.sig.sigctx != NULL, -1);
        xmlSecAssert2(ctx->u.sig.privkey != NULL, -1);
    } else {
        xmlSecAssert2(ctx->u.vfy.vfyctx != NULL, -1);
        xmlSecAssert2(ctx->u.vfy.pubkey != NULL, -1);
    }

    if(transform->status == xmlSecTransformStatusNone) {
        xmlSecAssert2(outSize == 0, -1);

        if(transform->operation == xmlSecTransformOperationSign) {
            status = SGN_Begin(ctx->u.sig.sigctx);
            if(status != SECSuccess) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "SGN_Begin",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "error code=%d", PORT_GetError());
                return(-1);
            }
        } else {
            status = VFY_Begin(ctx->u.vfy.vfyctx);
            if(status != SECSuccess) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "VFY_Begin",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "error code=%d", PORT_GetError());
                return(-1);
            }
        }
        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (inSize > 0)) {
        xmlSecAssert2(outSize == 0, -1);

        if(transform->operation == xmlSecTransformOperationSign) {
            status = SGN_Update(ctx->u.sig.sigctx, xmlSecBufferGetData(in), inSize);
            if(status != SECSuccess) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "SGN_Update",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "error code=%d", PORT_GetError());
                return(-1);
            }
        } else {
            status = VFY_Update(ctx->u.vfy.vfyctx, xmlSecBufferGetData(in), inSize);
            if(status != SECSuccess) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "VFY_Update",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "error code=%d", PORT_GetError());
                return(-1);
            }
        }

        ret = xmlSecBufferRemoveHead(in, inSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferRemoveHead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    if((transform->status == xmlSecTransformStatusWorking) && (last != 0)) {
        xmlSecAssert2(outSize == 0, -1);
        if(transform->operation == xmlSecTransformOperationSign) {
            memset(&signature, 0, sizeof(signature));
            status = SGN_End(ctx->u.sig.sigctx, &signature);
            if(status != SECSuccess) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "SGN_End",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "error code=%d", PORT_GetError());
                return(-1);
            }

            if(ctx->alg == SEC_OID_ANSIX9_DSA_SIGNATURE_WITH_SHA1_DIGEST) {
                /* This creates a signature which is ASN1 encoded */
                SECItem * signatureClr;

                signatureClr = DSAU_DecodeDerSig(&signature);
                if(signatureClr == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "DSAU_EncodeDerSig",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", 
                        PORT_GetError());
                    SECITEM_FreeItem(&signature, PR_FALSE);
                    return(-1);
                }

                ret = xmlSecBufferSetData(out, signatureClr->data, signatureClr->len);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecBufferSetData",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "size=%d",
                            signatureClr->len);
                    SECITEM_FreeItem(&signature, PR_FALSE);
                    return(-1);
                }

                SECITEM_FreeItem(signatureClr, PR_TRUE);
            } else {
                /* This signature is used as-is */
                ret = xmlSecBufferSetData(out, signature.data, signature.len);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecBufferSetData",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "size=%d",
                            signature.len);
                    SECITEM_FreeItem(&signature, PR_FALSE);
                    return(-1);
                }
            }

            /* cleanup */
            SECITEM_FreeItem(&signature, PR_FALSE);
        }
        transform->status = xmlSecTransformStatusFinished;
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

#ifndef XMLSEC_NO_DSA
/****************************************************************************
 *
 * DSA-SHA1 signature transform
 *
 ***************************************************************************/

static xmlSecTransformKlass xmlSecNssDsaSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssSignatureSize,                     /* xmlSecSize objSize */

    xmlSecNameDsaSha1,                          /* const xmlChar* name; */
    xmlSecHrefDsaSha1,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecNssSignatureInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssSignatureFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssSignatureSetKeyReq,                /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecNssSignatureSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecNssSignatureVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssSignatureExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformDsaSha1GetKlass:
 *
 * The DSA-SHA1 signature transform klass.
 *
 * Returns: DSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecNssTransformDsaSha1GetKlass(void) {
    return(&xmlSecNssDsaSha1Klass);
}

#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
/****************************************************************************
 *
 * RSA-MD5 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecNssRsaMd5Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssSignatureSize,                     /* xmlSecSize objSize */

    xmlSecNameRsaMd5,                           /* const xmlChar* name; */
    xmlSecHrefRsaMd5,                           /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecNssSignatureInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssSignatureFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssSignatureSetKeyReq,                /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecNssSignatureSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecNssSignatureVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssSignatureExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformRsaMd5GetKlass:
 *
 * The RSA-MD5 signature transform klass.
 *
 * Returns: RSA-MD5 signature transform klass.
 */
xmlSecTransformId
xmlSecNssTransformRsaMd5GetKlass(void) {
    return(&xmlSecNssRsaMd5Klass);
}

#endif /* XMLSEC_NO_MD5 */


#ifndef XMLSEC_NO_SHA1
/****************************************************************************
 *
 * RSA-SHA1 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecNssRsaSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssSignatureSize,                     /* xmlSecSize objSize */

    xmlSecNameRsaSha1,                          /* const xmlChar* name; */
    xmlSecHrefRsaSha1,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecNssSignatureInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssSignatureFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssSignatureSetKeyReq,                /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecNssSignatureSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecNssSignatureVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssSignatureExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformRsaSha1GetKlass:
 *
 * The RSA-SHA1 signature transform klass.
 *
 * Returns: RSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecNssTransformRsaSha1GetKlass(void) {
    return(&xmlSecNssRsaSha1Klass);
}

#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/****************************************************************************
 *
 * RSA-SHA256 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecNssRsaSha256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssSignatureSize,                     /* xmlSecSize objSize */

    xmlSecNameRsaSha256,                        /* const xmlChar* name; */
    xmlSecHrefRsaSha256,                        /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecNssSignatureInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssSignatureFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssSignatureSetKeyReq,                /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecNssSignatureSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecNssSignatureVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssSignatureExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformRsaSha256GetKlass:
 *
 * The RSA-SHA256 signature transform klass.
 *
 * Returns: RSA-SHA256 signature transform klass.
 */
xmlSecTransformId
xmlSecNssTransformRsaSha256GetKlass(void) {
    return(&xmlSecNssRsaSha256Klass);
}

#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/****************************************************************************
 *
 * RSA-SHA384 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecNssRsaSha384Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssSignatureSize,                     /* xmlSecSize objSize */

    xmlSecNameRsaSha384,                        /* const xmlChar* name; */
    xmlSecHrefRsaSha384,                        /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecNssSignatureInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssSignatureFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssSignatureSetKeyReq,                /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecNssSignatureSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecNssSignatureVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssSignatureExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformRsaSha384GetKlass:
 *
 * The RSA-SHA384 signature transform klass.
 *
 * Returns: RSA-SHA384 signature transform klass.
 */
xmlSecTransformId
xmlSecNssTransformRsaSha384GetKlass(void) {
    return(&xmlSecNssRsaSha384Klass);
}

#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/****************************************************************************
 *
 * RSA-SHA512 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecNssRsaSha512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecNssSignatureSize,                     /* xmlSecSize objSize */

    xmlSecNameRsaSha512,                        /* const xmlChar* name; */
    xmlSecHrefRsaSha512,                        /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecNssSignatureInitialize,               /* xmlSecTransformInitializeMethod initialize; */
    xmlSecNssSignatureFinalize,                 /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecNssSignatureSetKeyReq,                /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecNssSignatureSetKey,                   /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecNssSignatureVerify,                   /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecNssSignatureExecute,                  /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssTransformRsaSha512GetKlass:
 *
 * The RSA-SHA512 signature transform klass.
 *
 * Returns: RSA-SHA512 signature transform klass.
 */
xmlSecTransformId
xmlSecNssTransformRsaSha512GetKlass(void) {
    return(&xmlSecNssRsaSha512Klass);
}

#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */



/**
 *
 * XMLSec library
 *
 * RSA Algorithms support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_RSA

#include <stdlib.h>
#include <string.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/objects.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/buffer.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/strings.h>
#include <xmlsec/errors.h>

#include <xmlsec/openssl/crypto.h>
#include <xmlsec/openssl/evp.h>
#include <xmlsec/openssl/bn.h>
#include "openssl11_wrapper.h"

/**************************************************************************
 *
 * Internal OpenSSL RSA PKCS1 CTX
 *
 *************************************************************************/
typedef struct _xmlSecOpenSSLRsaPkcs1Ctx        xmlSecOpenSSLRsaPkcs1Ctx,
                                                *xmlSecOpenSSLRsaPkcs1CtxPtr;
struct _xmlSecOpenSSLRsaPkcs1Ctx {
    EVP_PKEY*           pKey;
};

/*********************************************************************
 *
 * RSA PKCS1 key transport transform
 *
 * xmlSecOpenSSLRsaPkcs1Ctx is located after xmlSecTransform
 *
 ********************************************************************/
#define xmlSecOpenSSLRsaPkcs1Size       \
    (sizeof(xmlSecTransform) + sizeof(xmlSecOpenSSLRsaPkcs1Ctx))
#define xmlSecOpenSSLRsaPkcs1GetCtx(transform) \
    ((xmlSecOpenSSLRsaPkcs1CtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecOpenSSLRsaPkcs1Initialize                 (xmlSecTransformPtr transform);
static void     xmlSecOpenSSLRsaPkcs1Finalize                   (xmlSecTransformPtr transform);
static int      xmlSecOpenSSLRsaPkcs1SetKeyReq                  (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecOpenSSLRsaPkcs1SetKey                     (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecOpenSSLRsaPkcs1Execute                    (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLRsaPkcs1Process                    (xmlSecTransformPtr transform,
                                                                 xmlSecTransformCtxPtr transformCtx);

static xmlSecTransformKlass xmlSecOpenSSLRsaPkcs1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLRsaPkcs1Size,                  /* xmlSecSize objSize */

    xmlSecNameRsaPkcs1,                         /* const xmlChar* name; */
    xmlSecHrefRsaPkcs1,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLRsaPkcs1Initialize,            /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLRsaPkcs1Finalize,              /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLRsaPkcs1SetKeyReq,             /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLRsaPkcs1SetKey,                /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLRsaPkcs1Execute,               /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformRsaPkcs1GetKlass:
 *
 * The RSA-PKCS1 key transport transform klass.
 *
 * Returns: RSA-PKCS1 key transport transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformRsaPkcs1GetKlass(void) {
    return(&xmlSecOpenSSLRsaPkcs1Klass);
}

static int
xmlSecOpenSSLRsaPkcs1Initialize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLRsaPkcs1CtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaPkcs1Id), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaPkcs1Size), -1);

    ctx = xmlSecOpenSSLRsaPkcs1GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecOpenSSLRsaPkcs1Ctx));
    return(0);
}

static void
xmlSecOpenSSLRsaPkcs1Finalize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLRsaPkcs1CtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaPkcs1Id));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaPkcs1Size));

    ctx = xmlSecOpenSSLRsaPkcs1GetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->pKey != NULL) {
        EVP_PKEY_free(ctx->pKey);
    }
    memset(ctx, 0, sizeof(xmlSecOpenSSLRsaPkcs1Ctx));
}

static int
xmlSecOpenSSLRsaPkcs1SetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecOpenSSLRsaPkcs1CtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaPkcs1Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaPkcs1Size), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecOpenSSLRsaPkcs1GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId        = xmlSecOpenSSLKeyDataRsaId;
    if(transform->operation == xmlSecTransformOperationEncrypt) {
        keyReq->keyType  = xmlSecKeyDataTypePublic;
        keyReq->keyUsage = xmlSecKeyUsageEncrypt;
    } else {
        keyReq->keyType  = xmlSecKeyDataTypePrivate;
        keyReq->keyUsage = xmlSecKeyUsageDecrypt;
    }
    return(0);
}

static int
xmlSecOpenSSLRsaPkcs1SetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecOpenSSLRsaPkcs1CtxPtr ctx;
    EVP_PKEY* pKey;
    RSA *rsa;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaPkcs1Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaPkcs1Size), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecOpenSSLKeyDataRsaId), -1);

    ctx = xmlSecOpenSSLRsaPkcs1GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pKey == NULL, -1);

    pKey = xmlSecOpenSSLKeyDataRsaGetEvp(xmlSecKeyGetValue(key));
    if(pKey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecOpenSSLKeyDataRsaGetEvp",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2(EVP_PKEY_base_id(pKey) == EVP_PKEY_RSA, -1);
    rsa = EVP_PKEY_get0_RSA(pKey);
    xmlSecAssert2(rsa != NULL, -1);

    ctx->pKey = xmlSecOpenSSLEvpKeyDup(pKey);
    if(ctx->pKey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecOpenSSLEvpKeyDup",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static int
xmlSecOpenSSLRsaPkcs1Execute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLRsaPkcs1CtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaPkcs1Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaPkcs1Size), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLRsaPkcs1GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pKey != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (last == 0)) {
        /* just do nothing */
    } else  if((transform->status == xmlSecTransformStatusWorking) && (last != 0)) {
        ret = xmlSecOpenSSLRsaPkcs1Process(transform, transformCtx);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecOpenSSLRsaPkcs1Process",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
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

static int
xmlSecOpenSSLRsaPkcs1Process(xmlSecTransformPtr transform, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLRsaPkcs1CtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    xmlSecSize keySize;
    RSA *rsa;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaPkcs1Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaPkcs1Size), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLRsaPkcs1GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pKey != NULL, -1);
    xmlSecAssert2(EVP_PKEY_base_id(ctx->pKey) == EVP_PKEY_RSA, -1);
    rsa = EVP_PKEY_get0_RSA(ctx->pKey);
    xmlSecAssert2(rsa != NULL, -1);

    keySize = RSA_size(rsa);
    xmlSecAssert2(keySize > 0, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);

    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);
    xmlSecAssert2(outSize == 0, -1);

    /* the encoded size is equal to the keys size so we could not
     * process more than that */
    if((transform->operation == xmlSecTransformOperationEncrypt) && (inSize >= keySize)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "%d when expected less than %d", inSize, keySize);
        return(-1);
    } else if((transform->operation == xmlSecTransformOperationDecrypt) && (inSize != keySize)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "%d when expected %d", inSize, keySize);
        return(-1);
    }

    outSize = keySize;
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
        ret = RSA_public_encrypt(inSize, xmlSecBufferGetData(in),
                                xmlSecBufferGetData(out),
                                rsa, RSA_PKCS1_PADDING);
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_public_encrypt",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "size=%d", inSize);
            return(-1);
        }
        outSize = ret;
    } else {
        ret = RSA_private_decrypt(inSize, xmlSecBufferGetData(in),
                                xmlSecBufferGetData(out),
                                rsa, RSA_PKCS1_PADDING);
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_private_decrypt",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "size=%d", inSize);
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

    return(0);
}

/**************************************************************************
 *
 * Internal OpenSSL RSA OAEP CTX
 *
 *************************************************************************/
typedef struct _xmlSecOpenSSLRsaOaepCtx         xmlSecOpenSSLRsaOaepCtx,
                                                *xmlSecOpenSSLRsaOaepCtxPtr;
struct _xmlSecOpenSSLRsaOaepCtx {
    EVP_PKEY*           pKey;
    xmlSecBuffer        oaepParams;
};

/*********************************************************************
 *
 * RSA OAEP key transport transform
 *
 * xmlSecOpenSSLRsaOaepCtx is located after xmlSecTransform
 *
 ********************************************************************/
#define xmlSecOpenSSLRsaOaepSize        \
    (sizeof(xmlSecTransform) + sizeof(xmlSecOpenSSLRsaOaepCtx))
#define xmlSecOpenSSLRsaOaepGetCtx(transform) \
    ((xmlSecOpenSSLRsaOaepCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecOpenSSLRsaOaepInitialize                  (xmlSecTransformPtr transform);
static void     xmlSecOpenSSLRsaOaepFinalize                    (xmlSecTransformPtr transform);
static int      xmlSecOpenSSLRsaOaepNodeRead                    (xmlSecTransformPtr transform,
                                                                 xmlNodePtr node,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLRsaOaepSetKeyReq                   (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecOpenSSLRsaOaepSetKey                      (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecOpenSSLRsaOaepExecute                     (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecOpenSSLRsaOaepProcess                     (xmlSecTransformPtr transform,
                                                                 xmlSecTransformCtxPtr transformCtx);

static xmlSecTransformKlass xmlSecOpenSSLRsaOaepKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecOpenSSLRsaOaepSize,                   /* xmlSecSize objSize */

    xmlSecNameRsaOaep,                          /* const xmlChar* name; */
    xmlSecHrefRsaOaep,                          /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecOpenSSLRsaOaepInitialize,             /* xmlSecTransformInitializeMethod initialize; */
    xmlSecOpenSSLRsaOaepFinalize,               /* xmlSecTransformFinalizeMethod finalize; */
    xmlSecOpenSSLRsaOaepNodeRead,               /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecOpenSSLRsaOaepSetKeyReq,              /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecOpenSSLRsaOaepSetKey,                 /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecOpenSSLRsaOaepExecute,                /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecOpenSSLTransformRsaOaepGetKlass:
 *
 * The RSA-OAEP key transport transform klass.
 *
 * Returns: RSA-OAEP key transport transform klass.
 */
xmlSecTransformId
xmlSecOpenSSLTransformRsaOaepGetKlass(void) {
    return(&xmlSecOpenSSLRsaOaepKlass);
}

static int
xmlSecOpenSSLRsaOaepInitialize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize), -1);

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecOpenSSLRsaOaepCtx));

    ret = xmlSecBufferInitialize(&(ctx->oaepParams), 0);
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
xmlSecOpenSSLRsaOaepFinalize(xmlSecTransformPtr transform) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize));

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->pKey != NULL) {
        EVP_PKEY_free(ctx->pKey);
    }
    xmlSecBufferFinalize(&(ctx->oaepParams));
    memset(ctx, 0, sizeof(xmlSecOpenSSLRsaOaepCtx));
}

static int
xmlSecOpenSSLRsaOaepNodeRead(xmlSecTransformPtr transform, xmlNodePtr node, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;
    xmlNodePtr cur;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(xmlSecBufferGetSize(&(ctx->oaepParams)) == 0, -1);

    cur = xmlSecGetNextElementNode(node->children);
    while(cur != NULL) {
        if(xmlSecCheckNodeName(cur,  xmlSecNodeRsaOAEPparams, xmlSecEncNs)) {
            ret = xmlSecBufferBase64NodeContentRead(&(ctx->oaepParams), cur);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecBufferBase64NodeContentRead",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
        } else if(xmlSecCheckNodeName(cur,  xmlSecNodeDigestMethod, xmlSecDSigNs)) {
            xmlChar* algorithm;

            /* Algorithm attribute is required */
            algorithm = xmlGetProp(cur, xmlSecAttrAlgorithm);
            if(algorithm == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                            XMLSEC_ERRORS_R_INVALID_NODE_ATTRIBUTE,
                            "node=%s",
                            xmlSecErrorsSafeString(xmlSecNodeGetName(cur)));
                return(-1);
            }

            /* for now we support only sha1 */
            if(xmlStrcmp(algorithm, xmlSecHrefSha1) != 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            xmlSecErrorsSafeString(algorithm),
                            XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                            "digest algorithm is not supported for rsa/oaep");
                xmlFree(algorithm);
                return(-1);
            }
            xmlFree(algorithm);
        } else {
            /* not found */
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                        XMLSEC_ERRORS_R_UNEXPECTED_NODE,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        /* next node */
        cur = xmlSecGetNextElementNode(cur->next);
    }

    return(0);
}

static int
xmlSecOpenSSLRsaOaepSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId        = xmlSecOpenSSLKeyDataRsaId;
    if(transform->operation == xmlSecTransformOperationEncrypt) {
        keyReq->keyType  = xmlSecKeyDataTypePublic;
        keyReq->keyUsage = xmlSecKeyUsageEncrypt;
    } else {
        keyReq->keyType  = xmlSecKeyDataTypePrivate;
        keyReq->keyUsage = xmlSecKeyUsageDecrypt;
    }

    return(0);
}

static int
xmlSecOpenSSLRsaOaepSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;
    EVP_PKEY* pKey;
    RSA *rsa;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecOpenSSLKeyDataRsaId), -1);

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pKey == NULL, -1);

    pKey = xmlSecOpenSSLKeyDataRsaGetEvp(xmlSecKeyGetValue(key));
    if(pKey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecOpenSSLKeyDataRsaGetEvp",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecAssert2(EVP_PKEY_base_id(pKey) == EVP_PKEY_RSA, -1);
    rsa = EVP_PKEY_get0_RSA(pKey);
    xmlSecAssert2(rsa != NULL, -1);

    ctx->pKey = xmlSecOpenSSLEvpKeyDup(pKey);
    if(ctx->pKey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecOpenSSLEvpKeyDup",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static int
xmlSecOpenSSLRsaOaepExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pKey != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (last == 0)) {
        /* just do nothing */
    } else  if((transform->status == xmlSecTransformStatusWorking) && (last != 0)) {
        ret = xmlSecOpenSSLRsaOaepProcess(transform, transformCtx);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecOpenSSLRsaOaepProcess",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
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

static int
xmlSecOpenSSLRsaOaepProcess(xmlSecTransformPtr transform, xmlSecTransformCtxPtr transformCtx) {
    xmlSecOpenSSLRsaOaepCtxPtr ctx;
    xmlSecSize paramsSize;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    xmlSecSize keySize;
    RSA *rsa;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecOpenSSLTransformRsaOaepId), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecOpenSSLRsaOaepSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecOpenSSLRsaOaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pKey != NULL, -1);
    xmlSecAssert2(EVP_PKEY_base_id(ctx->pKey) == EVP_PKEY_RSA, -1);
    rsa = EVP_PKEY_get0_RSA(ctx->pKey);
    xmlSecAssert2(rsa != NULL, -1);

    keySize = RSA_size(rsa);
    xmlSecAssert2(keySize > 0, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);

    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);
    xmlSecAssert2(outSize == 0, -1);

    /* the encoded size is equal to the keys size so we could not
     * process more than that */
    if((transform->operation == xmlSecTransformOperationEncrypt) && (inSize >= keySize)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "%d when expected less than %d", inSize, keySize);
        return(-1);
    } else if((transform->operation == xmlSecTransformOperationDecrypt) && (inSize != keySize)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "%d when expected %d", inSize, keySize);
        return(-1);
    }

    outSize = keySize;
    ret = xmlSecBufferSetMaxSize(out, outSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferSetMaxSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", outSize);
        return(-1);
    }

    paramsSize = xmlSecBufferGetSize(&(ctx->oaepParams));
    if((transform->operation == xmlSecTransformOperationEncrypt) && (paramsSize == 0)) {
        /* encode w/o OAEPParams --> simple */
        ret = RSA_public_encrypt(inSize, xmlSecBufferGetData(in),
                                xmlSecBufferGetData(out),
                                rsa, RSA_PKCS1_OAEP_PADDING);
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_public_encrypt(RSA_PKCS1_OAEP_PADDING)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        outSize = ret;
    } else if((transform->operation == xmlSecTransformOperationEncrypt) && (paramsSize > 0)) {
        xmlSecAssert2(xmlSecBufferGetData(&(ctx->oaepParams)) != NULL, -1);

        /* add space for padding */
        ret = xmlSecBufferSetMaxSize(in, keySize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferSetMaxSize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", keySize);
            return(-1);
        }

        /* add padding */
        ret = RSA_padding_add_PKCS1_OAEP(xmlSecBufferGetData(in), keySize,
                                         xmlSecBufferGetData(in), inSize,
                                         xmlSecBufferGetData(&(ctx->oaepParams)),
                                         paramsSize);
        if(ret != 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_padding_add_PKCS1_OAEP",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        inSize = keySize;

        /* encode with OAEPParams */
        ret = RSA_public_encrypt(inSize, xmlSecBufferGetData(in),
                                xmlSecBufferGetData(out),
                                rsa, RSA_NO_PADDING);
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_public_encrypt(RSA_NO_PADDING)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        outSize = ret;
    } else if((transform->operation == xmlSecTransformOperationDecrypt) && (paramsSize == 0)) {
        ret = RSA_private_decrypt(inSize, xmlSecBufferGetData(in),
                                xmlSecBufferGetData(out),
                                rsa, RSA_PKCS1_OAEP_PADDING);
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_private_decrypt(RSA_PKCS1_OAEP_PADDING)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        outSize = ret;
    } else if((transform->operation == xmlSecTransformOperationDecrypt) && (paramsSize != 0)) {
        BIGNUM * bn;

        bn = BN_new();
        if(bn == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "BN_new()",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        ret = RSA_private_decrypt(inSize, xmlSecBufferGetData(in),
                                xmlSecBufferGetData(out),
                                rsa, RSA_NO_PADDING);
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_private_decrypt(RSA_NO_PADDING)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            BN_free(bn);
            return(-1);
        }
        outSize = ret;

        /*
         * the private decrypt w/o padding adds '0's at the begginning.
         * it's not clear for me can I simply skip all '0's from the
         * beggining so I have to do decode it back to BIGNUM and dump
         * buffer again
         */
        if(BN_bin2bn(xmlSecBufferGetData(out), outSize, bn) == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "BN_bin2bn",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "size=%d", outSize);
            BN_free(bn);
            return(-1);
        }

        ret = BN_bn2bin(bn, xmlSecBufferGetData(out));
        if(ret <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "BN_bn2bin",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            BN_free(bn);
            return(-1);
        }
        BN_free(bn);
        outSize = ret;

        ret = RSA_padding_check_PKCS1_OAEP(xmlSecBufferGetData(out), outSize,
                                           xmlSecBufferGetData(out), outSize,
                                           keySize,
                                           xmlSecBufferGetData(&(ctx->oaepParams)),
                                           paramsSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "RSA_padding_check_PKCS1_OAEP",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        outSize = ret;
    } else {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "Unexpected trasnform operation: %d; paramsSize: %d",
                    (int)transform->operation, (int)paramsSize);
        return(-1);
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

    return(0);
}

#endif /* XMLSEC_NO_RSA */


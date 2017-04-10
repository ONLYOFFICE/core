/**
 *
 * XMLSec library
 *
 * RSA Algorithms support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_RSA

#include <stdlib.h>
#include <string.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/buffer.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/strings.h>
#include <xmlsec/errors.h>
#include <xmlsec/keyinfo.h>

#include <xmlsec/mscrypto/crypto.h>
#include <xmlsec/mscrypto/certkeys.h>
#include "private.h"

/**************************************************************************
 *
 * Internal MSCRYPTO RSA PKCS1 CTX
 *
 *************************************************************************/
typedef struct _xmlSecMSCryptoRsaPkcs1OaepCtx    xmlSecMSCryptoRsaPkcs1OaepCtx,
                                                *xmlSecMSCryptoRsaPkcs1OaepCtxPtr;
struct _xmlSecMSCryptoRsaPkcs1OaepCtx {
    DWORD               dwFlags;
    xmlSecKeyDataPtr    data;
    xmlSecBuffer        oaepParams;
};

/*********************************************************************
 *
 * RSA PKCS1 key transport transform
 *
 * xmlSecMSCryptoRsaPkcs1OaepCtx is located after xmlSecTransform
 *
 ********************************************************************/
#define xmlSecMSCryptoRsaPkcs1OaepCtx      \
    (sizeof(xmlSecTransform) + sizeof(xmlSecMSCryptoRsaPkcs1OaepCtx))
#define xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform) \
    ((xmlSecMSCryptoRsaPkcs1OaepCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecMSCryptoRsaPkcs1OaepCheckId               (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoRsaPkcs1OaepInitialize            (xmlSecTransformPtr transform);
static void     xmlSecMSCryptoRsaPkcs1OaepFinalize              (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoRsaPkcs1OaepSetKeyReq             (xmlSecTransformPtr transform,
                                                                 xmlSecKeyReqPtr keyReq);
static int      xmlSecMSCryptoRsaPkcs1OaepSetKey                (xmlSecTransformPtr transform,
                                                                 xmlSecKeyPtr key);
static int      xmlSecMSCryptoRsaPkcs1OaepExecute               (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int      xmlSecMSCryptoRsaPkcs1OaepProcess                   (xmlSecTransformPtr transform,
                                                                 xmlSecTransformCtxPtr transformCtx);


static int
xmlSecMSCryptoRsaPkcs1OaepCheckId(xmlSecTransformPtr transform) {

    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaPkcs1Id)) {
        return(1);
    } else

    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaOaepId)) {
        return(1);
    } else

    /* not found */
    {
        return(0);
    }

    /* just in case */
    return(0);
}

static int
xmlSecMSCryptoRsaPkcs1OaepInitialize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx), -1);

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* initialize */
    memset(ctx, 0, sizeof(xmlSecMSCryptoRsaPkcs1OaepCtx));

    ret = xmlSecBufferInitialize(&(ctx->oaepParams), 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaPkcs1Id)) {
        ctx->dwFlags = 0;
    } else

    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaOaepId)) {
        ctx->dwFlags = CRYPT_OAEP;
    } else

    /* not found */
    {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* done */
    return(0);
}

static void
xmlSecMSCryptoRsaPkcs1OaepFinalize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;

    xmlSecAssert(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx));

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if (ctx->data != NULL)  {
        xmlSecKeyDataDestroy(ctx->data);
        ctx->data = NULL;
    }

    xmlSecBufferFinalize(&(ctx->oaepParams));
    memset(ctx, 0, sizeof(xmlSecMSCryptoRsaPkcs1OaepCtx));
}

static int
xmlSecMSCryptoRsaPkcs1OaepSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    keyReq->keyId        = xmlSecMSCryptoKeyDataRsaId;
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
xmlSecMSCryptoRsaPkcs1OaepSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecMSCryptoKeyDataRsaId), -1);

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->data == NULL, -1);

    ctx->data = xmlSecKeyDataDuplicate(xmlSecKeyGetValue(key));
    if(ctx->data == NULL) {
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
xmlSecMSCryptoRsaPkcs1OaepExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (last == 0)) {
                /* just do nothing */
    } else  if((transform->status == xmlSecTransformStatusWorking) && (last != 0)) {
        ret = xmlSecMSCryptoRsaPkcs1OaepProcess(transform, transformCtx);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecMSCryptoRsaPkcs1OaepProcess",
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
xmlSecMSCryptoRsaPkcs1OaepProcess(xmlSecTransformPtr transform, xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    xmlSecSize keySize;
    int ret;
    HCRYPTKEY hKey = 0;
    DWORD dwInLen;
    DWORD dwBufLen;
    DWORD dwOutLen;
    xmlSecByte * outBuf;
    xmlSecByte * inBuf;
    int i;

    xmlSecAssert2(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncrypt) || (transform->operation == xmlSecTransformOperationDecrypt), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->data != NULL, -1);

    keySize = xmlSecKeyDataGetSize(ctx->data) / 8;
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
        if(inSize > outSize) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_SIZE,
                        "inSize=%d;outSize=%d",
                        inSize, outSize);
            return(-1);
        }

        ret = xmlSecBufferSetData(out, xmlSecBufferGetData(in), inSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferSetData",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", inSize);
            return(-1);
        }

        dwInLen = inSize;
        dwBufLen = outSize;
        if (0 == (hKey = xmlSecMSCryptoKeyDataGetKey(ctx->data, xmlSecKeyDataTypePublic))) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeyDataGetKey",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return (-1);
        }

        outBuf = xmlSecBufferGetData(out);
        xmlSecAssert2(outBuf != NULL, -1);

        /* set OAEP parameter for the key 
         *
         * aleksey: I don't understand how this would work in multi-threaded
         * environment or when key can be re-used multiple times
         */
        if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaOaepId) && xmlSecBufferGetSize(&(ctx->oaepParams)) > 0) {
            CRYPT_DATA_BLOB oaepParams;

            memset(&oaepParams, 0, sizeof(oaepParams));
            oaepParams.pbData = xmlSecBufferGetData(&(ctx->oaepParams));
            oaepParams.cbData = xmlSecBufferGetSize(&(ctx->oaepParams));

            if (!CryptSetKeyParam(hKey, KP_OAEP_PARAMS, (const BYTE*)&oaepParams, 0)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptSetKeyParam",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return (-1);
            }
        }

        /* encrypt */
        if (!CryptEncrypt(hKey, 0, TRUE, ctx->dwFlags, outBuf, &dwInLen, dwBufLen)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CryptEncrypt",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return (-1);
        }

        /* The output of CryptEncrypt is in little-endian format, so we have to convert to
         * big-endian first.
         */
        ConvertEndianInPlace(outBuf, outSize);
    } else {
        dwOutLen = inSize;

        /* The input of CryptDecrypt is expected to be little-endian,
         * so we have to convert from big-endian to little endian.
         */
        inBuf   = xmlSecBufferGetData(in);
        outBuf  = xmlSecBufferGetData(out);
        ConvertEndian(inBuf, outBuf, inSize);

        if (0 == (hKey = xmlSecMSCryptoKeyDataGetDecryptKey(ctx->data))) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeyDataGetKey",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return (-1);
        }

        /* set OAEP parameter for the key 
         *
         * aleksey: I don't understand how this would work in multi-threaded
         * environment or when key can be re-used multiple times
         */
        if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaOaepId) && xmlSecBufferGetSize(&(ctx->oaepParams)) > 0) {
            CRYPT_DATA_BLOB oaepParams;

            memset(&oaepParams, 0, sizeof(oaepParams));
            oaepParams.pbData = xmlSecBufferGetData(&(ctx->oaepParams));
            oaepParams.cbData = xmlSecBufferGetSize(&(ctx->oaepParams));

            if (!CryptSetKeyParam(hKey, KP_OAEP_PARAMS, (const BYTE*)&oaepParams, 0)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptSetKeyParam",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return (-1);
            }
        }

        /* decrypt */
        if (!CryptDecrypt(hKey, 0, TRUE, ctx->dwFlags, outBuf, &dwOutLen)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "CryptDecrypt",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        outSize = dwOutLen;
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


/**********************************************************************
 *
 * RSA/PKCS1 transform
 *
 **********************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoRsaPkcs1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoRsaPkcs1OaepCtx,                 /* xmlSecSize objSize */

    xmlSecNameRsaPkcs1,                         /* const xmlChar* name; */
    xmlSecHrefRsaPkcs1,                         /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecMSCryptoRsaPkcs1OaepInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoRsaPkcs1OaepFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoRsaPkcs1OaepSetKeyReq,            /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecMSCryptoRsaPkcs1OaepSetKey,               /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoRsaPkcs1OaepExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};


/**
 * xmlSecMSCryptoTransformRsaPkcs1GetKlass:
 *
 * The RSA-PKCS1 key transport transform klass.
 *
 * Returns: RSA-PKCS1 key transport transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaPkcs1GetKlass(void) {
    return(&xmlSecMSCryptoRsaPkcs1Klass);
}



/**********************************************************************
 *
 * RSA/OAEP transform
 *
 **********************************************************************/
static int          xmlSecMSCryptoRsaOaepNodeRead               (xmlSecTransformPtr transform, 
                                                                 xmlNodePtr node, 
                                                                 xmlSecTransformCtxPtr transformCtx);

static xmlSecTransformKlass xmlSecMSCryptoRsaOaepKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoRsaPkcs1OaepCtx,                 /* xmlSecSize objSize */

    xmlSecNameRsaOaep,                          /* const xmlChar* name; */
    xmlSecHrefRsaOaep,                          /* const xmlChar* href; */
    xmlSecTransformUsageEncryptionMethod,       /* xmlSecAlgorithmUsage usage; */

    xmlSecMSCryptoRsaPkcs1OaepInitialize,           /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoRsaPkcs1OaepFinalize,             /* xmlSecTransformFinalizeMethod finalize; */
    xmlSecMSCryptoRsaOaepNodeRead,              /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoRsaPkcs1OaepSetKeyReq,            /* xmlSecTransformSetKeyMethod setKeyReq; */
    xmlSecMSCryptoRsaPkcs1OaepSetKey,               /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoRsaPkcs1OaepExecute,              /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};


/**
 * xmlSecMSCryptoTransformRsaOaepGetKlass:
 *
 * The RSA-OAEP key transport transform klass.
 *
 * Returns: RSA-OAEP key transport transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaOaepGetKlass(void) {
    return(&xmlSecMSCryptoRsaOaepKlass);
}

static int
xmlSecMSCryptoRsaOaepNodeRead(xmlSecTransformPtr transform, xmlNodePtr node, xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoRsaPkcs1OaepCtxPtr ctx;
    xmlNodePtr cur;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoRsaPkcs1OaepCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoRsaPkcs1OaepCtx), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoRsaPkcs1OaepGetCtx(transform);
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

#endif /* XMLSEC_NO_RSA */


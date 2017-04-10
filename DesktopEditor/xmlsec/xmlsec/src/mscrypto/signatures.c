/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 * Copyright (C) 2003-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
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
#include <xmlsec/mscrypto/symbols.h>
#include <xmlsec/mscrypto/certkeys.h>
#include <xmlsec/mscrypto/x509.h>
#include "private.h"


/**************************************************************************
 *
 * Internal MSCrypto signatures ctx
 *
 *****************************************************************************/
typedef struct _xmlSecMSCryptoSignatureCtx      xmlSecMSCryptoSignatureCtx,
                                                *xmlSecMSCryptoSignatureCtxPtr;
struct _xmlSecMSCryptoSignatureCtx {
    xmlSecKeyDataPtr    data;
    ALG_ID              alg_id;
    HCRYPTHASH          mscHash;
    ALG_ID              digestAlgId;
    xmlSecKeyDataId     keyId;
};

/******************************************************************************
 *
 * Signature transforms
 *
 * xmlSecMSCryptoSignatureCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecMSCryptoSignatureSize     \
    (sizeof(xmlSecTransform) + sizeof(xmlSecMSCryptoSignatureCtx))
#define xmlSecMSCryptoSignatureGetCtx(transform) \
    ((xmlSecMSCryptoSignatureCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int      xmlSecMSCryptoSignatureCheckId          (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoSignatureInitialize       (xmlSecTransformPtr transform);
static void     xmlSecMSCryptoSignatureFinalize         (xmlSecTransformPtr transform);
static int      xmlSecMSCryptoSignatureSetKeyReq        (xmlSecTransformPtr transform,
                                                         xmlSecKeyReqPtr keyReq);
static int      xmlSecMSCryptoSignatureSetKey           (xmlSecTransformPtr transform,
                                                         xmlSecKeyPtr key);
static int      xmlSecMSCryptoSignatureVerify           (xmlSecTransformPtr transform,
                                                         const xmlSecByte* data,
                                                         xmlSecSize dataSize,
                                                         xmlSecTransformCtxPtr transformCtx);
static int      xmlSecMSCryptoSignatureExecute          (xmlSecTransformPtr transform,
                                                         int last,
                                                         xmlSecTransformCtxPtr transformCtx);


static int xmlSecMSCryptoSignatureCheckId(xmlSecTransformPtr transform) {

#ifndef XMLSEC_NO_DSA
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformDsaSha1Id)) {
        return(1);
    }
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaMd5Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha1Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha256Id)) {
       return(1);
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha384Id)) {
       return(1);
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha512Id)) {
       return(1);
    } else
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_GOST
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformGost2001GostR3411_94Id)) {
        return(1);
    } else
#endif /* XMLSEC_NO_GOST*/


    /* not found */
    {
        return(0);
    }

    return(0);
}

static int xmlSecMSCryptoSignatureInitialize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoSignatureCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoSignatureCheckId(transform), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoSignatureSize), -1);

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecMSCryptoSignatureCtx));


#ifndef XMLSEC_NO_DSA
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformDsaSha1Id)) {
        ctx->digestAlgId    = CALG_SHA1;
        ctx->keyId          = xmlSecMSCryptoKeyDataDsaId;
    } else
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaMd5Id)) {
        ctx->digestAlgId    = CALG_MD5;
        ctx->keyId          = xmlSecMSCryptoKeyDataRsaId;
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha1Id)) {
        ctx->digestAlgId    = CALG_SHA1;
        ctx->keyId          = xmlSecMSCryptoKeyDataRsaId;
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha256Id)) {
        ctx->digestAlgId    = CALG_SHA_256;
        ctx->keyId          = xmlSecMSCryptoKeyDataRsaId;
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha384Id)) {
        ctx->digestAlgId    = CALG_SHA_384;
        ctx->keyId          = xmlSecMSCryptoKeyDataRsaId;
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha512Id)) {
        ctx->digestAlgId    = CALG_SHA_512;
        ctx->keyId          = xmlSecMSCryptoKeyDataRsaId;
    } else
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_GOST
    if(xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformGost2001GostR3411_94Id)) {
        ctx->digestAlgId    = CALG_MAGPRO_HASH_R3411_94;
        ctx->keyId          = xmlSecMSCryptoKeyDataGost2001Id;
    } else
#endif /* XMLSEC_NO_GOST*/

    /* not found */
    {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void xmlSecMSCryptoSignatureFinalize(xmlSecTransformPtr transform) {
    xmlSecMSCryptoSignatureCtxPtr ctx;

    xmlSecAssert(xmlSecMSCryptoSignatureCheckId(transform));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecMSCryptoSignatureSize));

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if (ctx->mscHash) {
        CryptDestroyHash(ctx->mscHash);
    }

    if (ctx->data != NULL)  {
        xmlSecKeyDataDestroy(ctx->data);
        ctx->data = NULL;
    }

    memset(ctx, 0, sizeof(xmlSecMSCryptoSignatureCtx));
}

static int xmlSecMSCryptoSignatureSetKey(xmlSecTransformPtr transform, xmlSecKeyPtr key) {
    xmlSecMSCryptoSignatureCtxPtr ctx;
    xmlSecKeyDataPtr value;

    xmlSecAssert2(xmlSecMSCryptoSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoSignatureSize), -1);
    xmlSecAssert2(key != NULL, -1);

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->digestAlgId != 0, -1);
    xmlSecAssert2(ctx->keyId != NULL, -1);
    xmlSecAssert2(xmlSecKeyCheckId(key, ctx->keyId), -1);

    value = xmlSecKeyGetValue(key);
    xmlSecAssert2(value != NULL, -1);

    ctx->data = xmlSecKeyDataDuplicate(value);
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

static int xmlSecMSCryptoSignatureSetKeyReq(xmlSecTransformPtr transform,  xmlSecKeyReqPtr keyReq) {
    xmlSecMSCryptoSignatureCtxPtr ctx;

    xmlSecAssert2(xmlSecMSCryptoSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoSignatureSize), -1);
    xmlSecAssert2(keyReq != NULL, -1);

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
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

static int xmlSecMSCryptoSignatureVerify(xmlSecTransformPtr transform,
                                         const xmlSecByte* data,
                                         xmlSecSize dataSize,
                                         xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoSignatureCtxPtr ctx;
    xmlSecBuffer tmp;
    xmlSecByte *tmpBuf;
    HCRYPTKEY hKey;
    DWORD dwError;
    int ret;

    xmlSecAssert2(xmlSecMSCryptoSignatureCheckId(transform), -1);
    xmlSecAssert2(transform->operation == xmlSecTransformOperationVerify, -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoSignatureSize), -1);
    xmlSecAssert2(transform->status == xmlSecTransformStatusFinished, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize > 0, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    ret = xmlSecBufferInitialize(&tmp, dataSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "dataSize=%d", dataSize);
        return(-1);
    }

    tmpBuf = xmlSecBufferGetData(&tmp);
    xmlSecAssert2(tmpBuf != NULL, -1);

    /* Reverse the sig - Windows stores integers as octet streams in little endian
     * order.  The I2OSP algorithm used by XMLDSig to store integers is big endian */
#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaMd5Id)) {
        ConvertEndian(data, tmpBuf, dataSize);
    } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha1Id)) {
        ConvertEndian(data, tmpBuf, dataSize);
    } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha256Id)) {
        ConvertEndian(data, tmpBuf, dataSize);
    } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha384Id)) {
        ConvertEndian(data, tmpBuf, dataSize);
    } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha512Id)) {
        ConvertEndian(data, tmpBuf, dataSize);
    } else
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_DSA
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformDsaSha1Id) && (dataSize == 40)) {
        ConvertEndian(data, tmpBuf, 20);
        ConvertEndian(data + 20, tmpBuf + 20, 20);
    } else
#endif /*endif XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_GOST
    if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformGost2001GostR3411_94Id)) {
        ConvertEndian(data, tmpBuf, dataSize);
    } else
#endif /* XMLSEC_NO_GOST*/

    {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "Invalid algo");
        xmlSecBufferFinalize(&tmp);
        return(-1);
    }

    hKey = xmlSecMSCryptoKeyDataGetKey(ctx->data, xmlSecKeyDataTypePublic);
    if (hKey == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecMSCryptoKeyDataGetKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBufferFinalize(&tmp);
        return(-1);
    }
    if (!CryptVerifySignature(ctx->mscHash,
                              tmpBuf,
                              dataSize,
                              hKey,
                              NULL,
                              0)) {
        dwError = GetLastError();
        if (NTE_BAD_SIGNATURE == dwError) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "CryptVerifySignature",
                        XMLSEC_ERRORS_R_DATA_NOT_MATCH,
                        "signature do not match");
            transform->status = xmlSecTransformStatusFail;
            xmlSecBufferFinalize(&tmp);
            return(0);
        } else {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "CryptVerifySignature",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecBufferFinalize(&tmp);
            return (-1);
        }
    }
    xmlSecBufferFinalize(&tmp);
    transform->status = xmlSecTransformStatusOk;
    return(0);
}



static int
xmlSecMSCryptoSignatureExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecMSCryptoSignatureCtxPtr ctx;
    HCRYPTPROV hProv;
    DWORD dwKeySpec;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    int ret;
    DWORD dwSigLen;
    BYTE *tmpBuf, *outBuf;
    int bOk;
    PCRYPT_KEY_PROV_INFO pProviderInfo = NULL;
    char strContName[1000];
    char strProvName[1000];
    size_t size;

    xmlSecAssert2(xmlSecMSCryptoSignatureCheckId(transform), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationSign) || (transform->operation == xmlSecTransformOperationVerify), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecMSCryptoSignatureSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);
    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);

    ctx = xmlSecMSCryptoSignatureGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->digestAlgId != 0, -1);

    if(transform->status == xmlSecTransformStatusNone) {
        xmlSecAssert2(outSize == 0, -1);

        if (0 == (hProv = xmlSecMSCryptoKeyDataGetMSCryptoProvider(ctx->data))) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecMSCryptoKeyDataGetMSCryptoProvider",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return (-1);
        }

        //First try create hash with provider acquired in function xmlSecMSCryptoKeyDataAdoptCert.
        bOk = CryptCreateHash(hProv, ctx->digestAlgId, 0, 0, &(ctx->mscHash));

        //Then try it with container name, provider name and type acquired from certificate context.
        if(!bOk) {
            pProviderInfo = xmlSecMSCryptoKeyDataGetMSCryptoProviderInfo(ctx->data);

            if(pProviderInfo == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecMSCryptoKeyDataGetMSCryptoProviderInfo",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            ret = wcstombs_s(&size, strContName, 1000, pProviderInfo->pwszContainerName, _TRUNCATE);
            if(ret != 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "wcstombs_s",
                            XMLSEC_ERRORS_R_INVALID_DATA,
                            "ret=%d", ret);
                return(-1);
            }

            ret = wcstombs_s(&size, strProvName, 1000, pProviderInfo->pwszProvName, _TRUNCATE);
            if(ret != 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "wcstombs_s",
                            XMLSEC_ERRORS_R_INVALID_DATA,
                            "ret=%d", ret);
                return(-1);
            }

            if(!CryptReleaseContext(hProv, 0)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptReleaseContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            hProv = NULL;

            if(!CryptAcquireContext(&hProv,
                strContName,
                strProvName,
                pProviderInfo->dwProvType,
                0)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptAcquireContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            bOk = CryptCreateHash(hProv, ctx->digestAlgId, 0, 0, &(ctx->mscHash));
        }

        //Last try it with PROV_RSA_AES provider type.
        if(!bOk) {
            if (!CryptReleaseContext(hProv, 0)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptReleaseContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            hProv = NULL;

            if(!CryptAcquireContext(&hProv,
                strContName,
                NULL,
                PROV_RSA_AES,
                0)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptAcquireContext",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            bOk = CryptCreateHash(hProv, ctx->digestAlgId, 0, 0, &(ctx->mscHash));
        }

        if(pProviderInfo != NULL) {
            free(pProviderInfo);
        }

        if(!bOk) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CryptCreateHash",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (inSize > 0)) {
        xmlSecAssert2(outSize == 0, -1);

        if (!CryptHashData(ctx->mscHash, xmlSecBufferGetData(in), inSize, 0)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CryptHashData",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
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
        xmlSecBuffer tmp;

        xmlSecAssert2(outSize == 0, -1);

        if(transform->operation == xmlSecTransformOperationSign) {
            dwKeySpec = xmlSecMSCryptoKeyDataGetMSCryptoKeySpec(ctx->data);
            if (!CryptSignHash(ctx->mscHash, dwKeySpec, NULL, 0, NULL, &dwSigLen)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptSignHash",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            outSize = (xmlSecSize)dwSigLen;

            ret = xmlSecBufferInitialize(&tmp, outSize);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecBufferSetMaxSize",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "size=%d", outSize);
                return(-1);
            }
            tmpBuf = xmlSecBufferGetData(&tmp);
            xmlSecAssert2(tmpBuf != NULL, -1);

            if (!CryptSignHash(ctx->mscHash, dwKeySpec, NULL, 0, tmpBuf, &dwSigLen)) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "CryptSignHash",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlSecBufferFinalize(&tmp);
                return(-1);
            }
            outSize = (xmlSecSize)dwSigLen;

            ret = xmlSecBufferSetSize(out, outSize);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecBufferSetSize",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "size=%d", outSize);
                xmlSecBufferFinalize(&tmp);
                return(-1);
            }
            outBuf = xmlSecBufferGetData(out);
            xmlSecAssert2(outBuf != NULL, -1);

            /* Reverse the sig - Windows stores integers as octet streams in little endian
             * order.  The I2OSP algorithm used by XMLDSig to store integers is big endian */
#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaMd5Id)) {
                ConvertEndian(tmpBuf, outBuf, outSize);
            } else
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha1Id)) {
                ConvertEndian(tmpBuf, outBuf, outSize);
            } else
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha256Id)) {
                ConvertEndian(tmpBuf, outBuf, outSize);
            } else
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha384Id)) {
                ConvertEndian(tmpBuf, outBuf, outSize);
            } else
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformRsaSha512Id)) {
                ConvertEndian(tmpBuf, outBuf, outSize);
            } else
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA*/

#ifndef XMLSEC_NO_DSA
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformDsaSha1Id) && (outSize == 40)) {
                ConvertEndian(tmpBuf, outBuf, 20);
                ConvertEndian(tmpBuf + 20, outBuf + 20, 20);
            } else
#endif /* XMLSEC_NO_DSA*/

#ifndef XMLSEC_NO_GOST
            if (xmlSecTransformCheckId(transform, xmlSecMSCryptoTransformGost2001GostR3411_94Id)) {
                ConvertEndian(tmpBuf, outBuf, outSize);
            } else
#endif /* XMLSEC_NO_GOST*/

            {
                /* We shouldn't get at this place */
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            NULL,
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "Invalid algo");
                xmlSecBufferFinalize(&tmp);
                return(-1);
            }
            xmlSecBufferFinalize(&tmp);
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


#ifndef XMLSEC_NO_RSA

#ifndef XMLSEC_NO_MD5
/****************************************************************************
 *
 * RSA-MD5 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoRsaMd5Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaMd5,                          /* const xmlChar* name; */
    xmlSecHrefRsaMd5,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformRsaMd5GetKlass:
 *
 * The RSA-MD5 signature transform klass.
 *
 * Returns: RSA-MD5 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaMd5GetKlass(void) {
    return(&xmlSecMSCryptoRsaMd5Klass);
}
#endif /* XMLSEC_NO_MD5 */

#ifndef XMLSEC_NO_SHA1
/****************************************************************************
 *
 * RSA-SHA1 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoRsaSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameRsaSha1,                          /* const xmlChar* name; */
    xmlSecHrefRsaSha1,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformRsaSha1GetKlass:
 *
 * The RSA-SHA1 signature transform klass.
 *
 * Returns: RSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaSha1GetKlass(void) {
    return(&xmlSecMSCryptoRsaSha1Klass);
}
#endif /* XMLSEC_NO_SHA1 */

#ifndef XMLSEC_NO_SHA256
/****************************************************************************
 *
 * RSA-SHA256 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoRsaSha256Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),              /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,               /* xmlSecSize objSize */

    xmlSecNameRsaSha256,                       /* const xmlChar* name; */
    xmlSecHrefRsaSha256,                       /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,       /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,         /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,           /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                      /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                      /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,          /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,             /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,             /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,         /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,             /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                      /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                      /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,            /* xmlSecTransformExecuteMethod execute; */

    NULL,                                      /* void* reserved0; */
    NULL,                                      /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformRsaSha256GetKlass:
 *
 * The RSA-SHA256 signature transform klass.
 *
 * Returns: RSA-SHA256 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaSha256GetKlass(void) {
    return(&xmlSecMSCryptoRsaSha256Klass);
}
#endif /* XMLSEC_NO_SHA256 */

#ifndef XMLSEC_NO_SHA384
/****************************************************************************
 *
 * RSA-SHA384 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoRsaSha384Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),              /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,               /* xmlSecSize objSize */

    xmlSecNameRsaSha384,                       /* const xmlChar* name; */
    xmlSecHrefRsaSha384,                       /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,       /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,         /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,           /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                      /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                      /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,          /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,             /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,             /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,         /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,             /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                      /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                      /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,            /* xmlSecTransformExecuteMethod execute; */

    NULL,                                      /* void* reserved0; */
    NULL,                                      /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformRsaSha384GetKlass:
 *
 * The RSA-SHA384 signature transform klass.
 *
 * Returns: RSA-SHA384 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaSha384GetKlass(void) {
    return(&xmlSecMSCryptoRsaSha384Klass);
}
#endif /* XMLSEC_NO_SHA384 */

#ifndef XMLSEC_NO_SHA512
/****************************************************************************
 *
 * RSA-SHA2512 signature transform
 *
 ***************************************************************************/
static xmlSecTransformKlass xmlSecMSCryptoRsaSha512Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),              /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,               /* xmlSecSize objSize */

    xmlSecNameRsaSha512,                       /* const xmlChar* name; */
    xmlSecHrefRsaSha512,                       /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,       /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,         /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,           /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                      /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                      /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,          /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,             /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,             /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,         /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,             /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                      /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                      /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,            /* xmlSecTransformExecuteMethod execute; */

    NULL,                                      /* void* reserved0; */
    NULL,                                      /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformRsaSha512GetKlass:
 *
 * The RSA-SHA512 signature transform klass.
 *
 * Returns: RSA-SHA512 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformRsaSha512GetKlass(void) {
    return(&xmlSecMSCryptoRsaSha512Klass);
}
#endif /* XMLSEC_NO_SHA512 */

#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_DSA
/****************************************************************************
 *
 * DSA-SHA1 signature transform
 *
 ***************************************************************************/

static xmlSecTransformKlass xmlSecMSCryptoDsaSha1Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameDsaSha1,                          /* const xmlChar* name; */
    xmlSecHrefDsaSha1,                          /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformDsaSha1GetKlass:
 *
 * The DSA-SHA1 signature transform klass.
 *
 * Returns: DSA-SHA1 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformDsaSha1GetKlass(void) {
    return(&xmlSecMSCryptoDsaSha1Klass);
}

#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_GOST
/****************************************************************************
 *
 * GOST2001-GOSTR3411_94 signature transform
 *
 ***************************************************************************/

static xmlSecTransformKlass xmlSecMSCryptoGost2001GostR3411_94Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecMSCryptoSignatureSize,                /* xmlSecSize objSize */

    xmlSecNameGost2001GostR3411_94,                             /* const xmlChar* name; */
    xmlSecHrefGost2001GostR3411_94,                             /* const xmlChar* href; */
    xmlSecTransformUsageSignatureMethod,        /* xmlSecTransformUsage usage; */

    xmlSecMSCryptoSignatureInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecMSCryptoSignatureFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    xmlSecMSCryptoSignatureSetKeyReq,           /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    xmlSecMSCryptoSignatureSetKey,              /* xmlSecTransformSetKeyMethod setKey; */
    xmlSecMSCryptoSignatureVerify,              /* xmlSecTransformVerifyMethod verify; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecMSCryptoSignatureExecute,             /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoTransformGost2001GostR3411_94GetKlass:
 *
 * The GOST2001-GOSTR3411_94 signature transform klass.
 *
 * Returns: GOST2001-GOSTR3411_94 signature transform klass.
 */
xmlSecTransformId
xmlSecMSCryptoTransformGost2001GostR3411_94GetKlass(void) {
    return(&xmlSecMSCryptoGost2001GostR3411_94Klass);
}

#endif /* XMLSEC_NO_GOST*/


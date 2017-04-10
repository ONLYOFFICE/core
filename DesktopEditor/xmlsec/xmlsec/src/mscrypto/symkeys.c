/**
 *
 * XMLSec library
 *
 * DES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 */
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/mscrypto/crypto.h>
#include "private.h"

/*****************************************************************************
 *
 * Symmetic (binary) keys - just a wrapper for xmlSecKeyDataBinary
 *
 ****************************************************************************/
static int      xmlSecMSCryptoSymKeyDataInitialize      (xmlSecKeyDataPtr data);
static int      xmlSecMSCryptoSymKeyDataDuplicate       (xmlSecKeyDataPtr dst,
                                                         xmlSecKeyDataPtr src);
static void     xmlSecMSCryptoSymKeyDataFinalize        (xmlSecKeyDataPtr data);
static int      xmlSecMSCryptoSymKeyDataXmlRead         (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecMSCryptoSymKeyDataXmlWrite        (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecMSCryptoSymKeyDataBinRead         (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         const unsigned char* buf,
                                                         xmlSecSize bufSize,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecMSCryptoSymKeyDataBinWrite        (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         unsigned char** buf,
                                                         xmlSecSize* bufSize,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecMSCryptoSymKeyDataGenerate        (xmlSecKeyDataPtr data,
                                                         xmlSecSize sizeBits,
                                                         xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecMSCryptoSymKeyDataGetType(xmlSecKeyDataPtr data);
static xmlSecSize xmlSecMSCryptoSymKeyDataGetSize         (xmlSecKeyDataPtr data);
static void     xmlSecMSCryptoSymKeyDataDebugDump       (xmlSecKeyDataPtr data,
                                                         FILE* output);
static void     xmlSecMSCryptoSymKeyDataDebugXmlDump    (xmlSecKeyDataPtr data,
                                                         FILE* output);
static int      xmlSecMSCryptoSymKeyDataKlassCheck      (xmlSecKeyDataKlass* klass);

/*
 * GENERIC HELPER FUNCTIONS
 */

#define xmlSecMSCryptoSymKeyDataCheckId(data) \
    (xmlSecKeyDataIsValid((data)) && \
     xmlSecMSCryptoSymKeyDataKlassCheck((data)->id))

static int
xmlSecMSCryptoSymKeyDataInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataCheckId(data), -1);

    return(xmlSecKeyDataBinaryValueInitialize(data));
}

static int
xmlSecMSCryptoSymKeyDataDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataCheckId(dst), -1);
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataCheckId(src), -1);
    xmlSecAssert2(dst->id == src->id, -1);

    return(xmlSecKeyDataBinaryValueDuplicate(dst, src));
}

static void
xmlSecMSCryptoSymKeyDataFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecMSCryptoSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueFinalize(data);
}

static int
xmlSecMSCryptoSymKeyDataXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                               xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueXmlRead(id, key, node, keyInfoCtx));
}

static int
xmlSecMSCryptoSymKeyDataXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueXmlWrite(id, key, node, keyInfoCtx));
}

static int
xmlSecMSCryptoSymKeyDataBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                const unsigned char* buf, xmlSecSize bufSize,
                                xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueBinRead(id, key, buf, bufSize, keyInfoCtx));
}

static int
xmlSecMSCryptoSymKeyDataBinWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                 unsigned char** buf, xmlSecSize* bufSize,
                                 xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueBinWrite(id, key, buf, bufSize, keyInfoCtx));
}

static int
xmlSecMSCryptoSymKeyDataGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecMSCryptoSymKeyDataCheckId(data), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecMSCryptoGenerateRandom(buffer, (sizeBits + 7) / 8));
}

static xmlSecKeyDataType
xmlSecMSCryptoSymKeyDataGetType(xmlSecKeyDataPtr data) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecMSCryptoSymKeyDataCheckId(data), xmlSecKeyDataTypeUnknown);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, xmlSecKeyDataTypeUnknown);

    return((xmlSecBufferGetSize(buffer) > 0) ? xmlSecKeyDataTypeSymmetric : xmlSecKeyDataTypeUnknown);
}

static xmlSecSize
xmlSecMSCryptoSymKeyDataGetSize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecMSCryptoSymKeyDataCheckId(data), 0);

    return(xmlSecKeyDataBinaryValueGetSize(data));
}

static void
xmlSecMSCryptoSymKeyDataDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecMSCryptoSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueDebugDump(data, output);
}

static void
xmlSecMSCryptoSymKeyDataDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecMSCryptoSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueDebugXmlDump(data, output);
}

static int
xmlSecMSCryptoSymKeyDataKlassCheck(xmlSecKeyDataKlass* klass) {

#ifndef XMLSEC_NO_DES
    if(klass == xmlSecMSCryptoKeyDataDesId) {
        return(1);
    } else 
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(klass == xmlSecMSCryptoKeyDataAesId) {
                return(1);
    } else
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_HMAC
    if(klass == xmlSecMSCryptoKeyDataHmacId) {
                return(1);
    } else
#endif /* XMLSEC_NO_HMAC */

    {
        return(0);
    }

    return(0);
}


/******************************************************************************
 *
 * Utils
 *
 * Low level helper routines for importing plain text keys in MS HKEY handle,
 * since MSCrypto API does not support import of plain text (session) keys
 * just like that. These functions are based upon MS kb article #228786
 * and "Base Provider Key BLOBs" article for priv key blob format.
 *
 ******************************************************************************/
BOOL
xmlSecMSCryptoCreatePrivateExponentOneKey(HCRYPTPROV hProv, HCRYPTKEY *hPrivateKey)
{
    HCRYPTKEY hKey = 0;
    LPBYTE keyBlob = NULL;
    DWORD keyBlobLen;
    PUBLICKEYSTRUC* pubKeyStruc;
    RSAPUBKEY* rsaPubKey;
    DWORD bitLen;
    BYTE *ptr;
    int n;
    BOOL res = FALSE;

    xmlSecAssert2(hProv != 0, FALSE);
    xmlSecAssert2(hPrivateKey != NULL, FALSE);

    /* just in case */
    *hPrivateKey = 0;

    /* Generate the private key */
    if(!CryptGenKey(hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hKey)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGenKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* Export the private key, we'll convert it to a private exponent of one key */
    if(!CryptExportKey(hKey, 0, PRIVATEKEYBLOB, 0, NULL, &keyBlobLen)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    keyBlob = (LPBYTE)xmlMalloc(sizeof(BYTE) * keyBlobLen);
    if(keyBlob == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    if(!CryptExportKey(hKey, 0, PRIVATEKEYBLOB, 0, keyBlob, &keyBlobLen)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    CryptDestroyKey(hKey);
    hKey = 0;

    /* Get the bit length of the key */
    if(keyBlobLen < sizeof(PUBLICKEYSTRUC) + sizeof(RSAPUBKEY)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "len=%ld", keyBlobLen);
        goto done;
    }
    pubKeyStruc = (PUBLICKEYSTRUC*)keyBlob;
    if(pubKeyStruc->bVersion != 0x02) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "pubKeyStruc->bVersion=%d", pubKeyStruc->bVersion);
        goto done;
    }
    if(pubKeyStruc->bType != PRIVATEKEYBLOB) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "pubKeyStruc->bType=%d", (int)pubKeyStruc->bType);
        goto done;
    }

    /* aleksey: don't ask me why it is RSAPUBKEY, just don't ask */
    rsaPubKey = (RSAPUBKEY*)(keyBlob + sizeof(PUBLICKEYSTRUC));

    /* check that we have RSA private key */
    if(rsaPubKey->magic != 0x32415352) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "rsaPubKey->magic=0x%08lx", rsaPubKey->magic);
        goto done;
    }
    bitLen = rsaPubKey->bitlen;

    /*  Modify the Exponent in Key BLOB format Key BLOB format is documented in SDK */
    rsaPubKey->pubexp = 1;

    /* Private-key BLOBs, type PRIVATEKEYBLOB, are used to store private keys outside a CSP.
     * Base provider private-key BLOBs have the following format:
     *
     * PUBLICKEYSTRUC  publickeystruc ;
     * RSAPUBKEY rsapubkey;
     * BYTE modulus[rsapubkey.bitlen/8];                1/8
     * BYTE prime1[rsapubkey.bitlen/16];                1/16
     * BYTE prime2[rsapubkey.bitlen/16];                1/16
     * BYTE exponent1[rsapubkey.bitlen/16];             1/16
     * BYTE exponent2[rsapubkey.bitlen/16];             1/16
     * BYTE coefficient[rsapubkey.bitlen/16];           1/16
     * BYTE privateExponent[rsapubkey.bitlen/8];        1/8
     */
    if(keyBlobLen < sizeof(PUBLICKEYSTRUC) + sizeof(RSAPUBKEY) + bitLen / 2 + bitLen / 16) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptExportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "len=%ld", keyBlobLen);
        goto done;
    }
    ptr = (BYTE*)(keyBlob + sizeof(PUBLICKEYSTRUC) + sizeof(RSAPUBKEY));

    /* Skip modulus, prime1, prime2 */
    ptr += bitLen / 8;
    ptr += bitLen / 16;
    ptr += bitLen / 16;

    /* Convert exponent1 to 1 */
    for (n = 0; n < (bitLen / 16); n++) {
        if (n == 0) ptr[n] = 1;
        else ptr[n] = 0;
    }
    ptr += bitLen / 16;

    /* Convert exponent2 to 1 */
    for (n = 0; n < (bitLen / 16); n++) {
        if (n == 0) ptr[n] = 1;
        else ptr[n] = 0;
    }
    ptr += bitLen / 16;

    /* Skip coefficient */
    ptr += bitLen / 16;

    /* Convert privateExponent to 1 */
    for (n = 0; n < (bitLen / 16); n++) {
        if (n == 0) ptr[n] = 1;
        else ptr[n] = 0;
    }

    /* Import the exponent-of-one private key. */
    if (!CryptImportKey(hProv, keyBlob, keyBlobLen, 0, 0, &hKey)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptImportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    (*hPrivateKey) = hKey;
    hKey = 0;
    res = TRUE;

done:
    if(keyBlob != NULL) {
        xmlFree(keyBlob);
    }
    if (hKey != 0) {
        CryptDestroyKey(hKey);
    }

    return res;
}

BOOL
xmlSecMSCryptoImportPlainSessionBlob(HCRYPTPROV hProv, HCRYPTKEY hPrivateKey,
                                     ALG_ID dwAlgId, LPBYTE pbKeyMaterial,
                                     DWORD dwKeyMaterial, BOOL bCheckKeyLength,
                                     HCRYPTKEY *hSessionKey) {
    ALG_ID dwPrivKeyAlg;
    LPBYTE keyBlob = NULL;
    DWORD keyBlobLen, rndBlobSize, dwSize, n;
    PUBLICKEYSTRUC* pubKeyStruc;
    ALG_ID* algId;
    DWORD dwPublicKeySize;
    DWORD dwProvSessionKeySize = 0;
    LPBYTE pbPtr;
    DWORD dwFlags;
    PROV_ENUMALGS_EX ProvEnum;
    HCRYPTKEY hTempKey = 0;
    BOOL fFound;
    BOOL res = FALSE;

    xmlSecAssert2(hProv != 0, FALSE);
    xmlSecAssert2(hPrivateKey != 0, FALSE);
    xmlSecAssert2(pbKeyMaterial != NULL, FALSE);
    xmlSecAssert2(dwKeyMaterial > 0, FALSE);
    xmlSecAssert2(hSessionKey != NULL, FALSE);

    /*  Double check to see if this provider supports this algorithm and key size */
    fFound = FALSE;
    dwFlags = CRYPT_FIRST;
    dwSize = sizeof(ProvEnum);
    while(CryptGetProvParam(hProv, PP_ENUMALGS_EX, (LPBYTE)&ProvEnum, &dwSize, dwFlags)) {
        if (ProvEnum.aiAlgid == dwAlgId) {
            fFound = TRUE;
            break;
        }
        dwSize = sizeof(ProvEnum);
        dwFlags = 0;
    }
    if(!fFound) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGetProvParam",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "algId=%d is not supported", dwAlgId);
        goto done;
    }

    if(bCheckKeyLength) {
        /* We have to get the key size(including padding) from an HCRYPTKEY handle.
         * PP_ENUMALGS_EX contains the key size without the padding so we can't use it.
         */
        if(!CryptGenKey(hProv, dwAlgId, 0, &hTempKey)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CryptGenKey",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "algId=%d", dwAlgId);
            goto done;
        }

        dwSize = sizeof(DWORD);
        if(!CryptGetKeyParam(hTempKey, KP_KEYLEN, (LPBYTE)&dwProvSessionKeySize, &dwSize, 0)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CryptGetKeyParam(KP_KEYLEN)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "algId=%d", dwAlgId);
            goto done;
        }
        CryptDestroyKey(hTempKey);
        hTempKey = 0;

        /* yell if key is too big */
        if ((dwKeyMaterial * 8) > dwProvSessionKeySize) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_SIZE,
                        "dwKeyMaterial=%ld;dwProvSessionKeySize=%ld",
                        dwKeyMaterial, dwProvSessionKeySize);
            goto done;
        }
    } else {
        dwProvSessionKeySize = dwKeyMaterial * 8;
    }

    /* Get private key's algorithm */
    dwSize = sizeof(ALG_ID);
    if(!CryptGetKeyParam(hPrivateKey, KP_ALGID, (LPBYTE)&dwPrivKeyAlg, &dwSize, 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGetKeyParam(KP_ALGID)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "algId=%d", dwAlgId);
        goto done;
    }

    /* Get private key's length in bits */
    dwSize = sizeof(DWORD);
    if(!CryptGetKeyParam(hPrivateKey, KP_KEYLEN, (LPBYTE)&dwPublicKeySize, &dwSize, 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGetKeyParam(KP_KEYLEN)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "algId=%d", dwAlgId);
        goto done;
    }

    /* 3 is for the first reserved byte after the key material and the 2 reserved bytes at the end. */
    if(dwPublicKeySize / 8 < dwKeyMaterial + 3) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "dwKeyMaterial=%ld;dwPublicKeySize=%ld",
                    dwKeyMaterial, dwPublicKeySize);
        goto done;
    }
    rndBlobSize = dwPublicKeySize / 8 - (dwKeyMaterial + 3);

    /* Simple key BLOBs, type SIMPLEBLOB, are used to store and transport session keys outside a CSP.
     * Base provider simple-key BLOBs are always encrypted with a key exchange public key. The pbData
     * member of the SIMPLEBLOB is a sequence of bytes in the following format:
     *
     * PUBLICKEYSTRUC  publickeystruc ;
     * ALG_ID algid;
     * BYTE encryptedkey[rsapubkey.bitlen/8];
     */

    /* calculate Simple blob's length */
    keyBlobLen = sizeof(PUBLICKEYSTRUC) + sizeof(ALG_ID) + (dwPublicKeySize / 8);

    /* allocate simple blob buffer */
    keyBlob = (LPBYTE)xmlMalloc(sizeof(BYTE) * keyBlobLen);
    if(keyBlob == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    memset(keyBlob, 0, keyBlobLen);

    /* initialize PUBLICKEYSTRUC */
    pubKeyStruc             = (PUBLICKEYSTRUC*)(keyBlob);
    pubKeyStruc->bType      = SIMPLEBLOB;
    pubKeyStruc->bVersion   = 0x02;
    pubKeyStruc->reserved   = 0;
    pubKeyStruc->aiKeyAlg   = dwAlgId;

    /* Copy private key algorithm to buffer */
    algId                   = (ALG_ID*)(keyBlob + sizeof(PUBLICKEYSTRUC));
    (*algId)                = dwPrivKeyAlg;

    /* Place the key material in reverse order */
    pbPtr                   = (BYTE*)(keyBlob + sizeof(PUBLICKEYSTRUC) + sizeof(ALG_ID));
    for (n = 0; n < dwKeyMaterial; n++) {
        pbPtr[n] = pbKeyMaterial[dwKeyMaterial - n - 1];
    }
    pbPtr += dwKeyMaterial;

    /* skip reserved byte */
    pbPtr += 1;

    /* Generate random data for the rest of the buffer */
    if((rndBlobSize > 0) && !CryptGenRandom(hProv, rndBlobSize, pbPtr)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptGenRandom",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "rndBlobSize=%ld", rndBlobSize);
        goto done;
    }
    /* aleksey: why are we doing this? */
    for (n = 0; n < rndBlobSize; n++) {
        if (pbPtr[n] == 0) pbPtr[n] = 1;
    }

    /* set magic number at the end */
    keyBlob[keyBlobLen - 2] = 2;

    if(!CryptImportKey(hProv, keyBlob , keyBlobLen, hPrivateKey, CRYPT_EXPORTABLE, hSessionKey)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CryptImportKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "algId=%d", dwAlgId);
        goto done;
    }

    /* success */
    res = TRUE;

done:
    if(hTempKey != 0) {
        CryptDestroyKey(hTempKey);
    }
    if(keyBlob != NULL) {
        xmlFree(keyBlob);
    }
    return(res);
}

#ifndef XMLSEC_NO_AES
/**************************************************************************
 *
 * <xmlsec:AESKeyValue> processing
 *
 *************************************************************************/
static xmlSecKeyDataKlass xmlSecMSCryptoKeyDataAesKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecKeyDataBinarySize,

    /* data */
    xmlSecNameAESKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefAESKeyValue,                      /* const xmlChar* href; */
    xmlSecNodeAESKeyValue,                      /* const xmlChar* dataNodeName; */
    xmlSecNs,                                   /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecMSCryptoSymKeyDataInitialize,         /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecMSCryptoSymKeyDataDuplicate,          /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecMSCryptoSymKeyDataFinalize,           /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecMSCryptoSymKeyDataGenerate,           /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecMSCryptoSymKeyDataGetType,            /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecMSCryptoSymKeyDataGetSize,            /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecMSCryptoSymKeyDataXmlRead,            /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecMSCryptoSymKeyDataXmlWrite,           /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecMSCryptoSymKeyDataBinRead,            /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecMSCryptoSymKeyDataBinWrite,           /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecMSCryptoSymKeyDataDebugDump,          /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecMSCryptoSymKeyDataDebugXmlDump,       /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoKeyDataAesGetKlass:
 *
 * The AES key data klass.
 *
 * Returns: AES key data klass.
 */
xmlSecKeyDataId
xmlSecMSCryptoKeyDataAesGetKlass(void) {
    return(&xmlSecMSCryptoKeyDataAesKlass);
}

/**
 * xmlSecMSCryptoKeyDataAesSet:
 * @data:               the pointer to AES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of AES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoKeyDataAesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecMSCryptoKeyDataAesId), -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecBufferSetData(buffer, buf, bufSize));
}
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES
/**************************************************************************
 *
 * <xmlsec:DESKeyValue> processing
 *
 *************************************************************************/
static xmlSecKeyDataKlass xmlSecMSCryptoKeyDataDesKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecKeyDataBinarySize,

    /* data */
    xmlSecNameDESKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefDESKeyValue,                      /* const xmlChar* href; */
    xmlSecNodeDESKeyValue,                      /* const xmlChar* dataNodeName; */
    xmlSecNs,                                   /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecMSCryptoSymKeyDataInitialize,         /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecMSCryptoSymKeyDataDuplicate,          /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecMSCryptoSymKeyDataFinalize,           /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecMSCryptoSymKeyDataGenerate,           /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecMSCryptoSymKeyDataGetType,            /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecMSCryptoSymKeyDataGetSize,            /* xmlSecKeyDataGetSizeMethod getSize; */
        NULL,                                   /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecMSCryptoSymKeyDataXmlRead,            /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecMSCryptoSymKeyDataXmlWrite,           /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecMSCryptoSymKeyDataBinRead,            /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecMSCryptoSymKeyDataBinWrite,           /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecMSCryptoSymKeyDataDebugDump,          /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecMSCryptoSymKeyDataDebugXmlDump,       /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoKeyDataDesGetKlass:
 *
 * The DES key data klass.
 *
 * Returns: DES key data klass.
 */
xmlSecKeyDataId
xmlSecMSCryptoKeyDataDesGetKlass(void) {
    return(&xmlSecMSCryptoKeyDataDesKlass);
}
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_HMAC
/**************************************************************************
 *
 * <xmlsec:HMACKeyValue> processing
 *
 *************************************************************************/
static xmlSecKeyDataKlass xmlSecMSCryptoKeyDataHmacKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecKeyDataBinarySize,

    /* data */
    xmlSecNameHMACKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefHMACKeyValue,                     /* const xmlChar* href; */
    xmlSecNodeHMACKeyValue,                     /* const xmlChar* dataNodeName; */
    xmlSecNs,                                   /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecMSCryptoSymKeyDataInitialize,         /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecMSCryptoSymKeyDataDuplicate,          /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecMSCryptoSymKeyDataFinalize,           /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecMSCryptoSymKeyDataGenerate,           /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecMSCryptoSymKeyDataGetType,            /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecMSCryptoSymKeyDataGetSize,            /* xmlSecKeyDataGetSizeMethod getSize; */
        NULL,                                   /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecMSCryptoSymKeyDataXmlRead,            /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecMSCryptoSymKeyDataXmlWrite,           /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecMSCryptoSymKeyDataBinRead,            /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecMSCryptoSymKeyDataBinWrite,           /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecMSCryptoSymKeyDataDebugDump,          /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecMSCryptoSymKeyDataDebugXmlDump,       /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoKeyDataHmacGetKlass:
 *
 * The HMAC key data klass.
 *
 * Returns: HMAC key data klass.
 */
xmlSecKeyDataId
xmlSecMSCryptoKeyDataHmacGetKlass(void) {
    return(&xmlSecMSCryptoKeyDataHmacKlass);
}

/**
 * xmlSecMSCryptoKeyDataHmacSet:
 * @data:               the pointer to HMAC key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of HMAC key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoKeyDataHmacSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecMSCryptoKeyDataHmacId), -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecBufferSetData(buffer, buf, bufSize));
}


#endif /* XMLSEC_NO_HMAC */




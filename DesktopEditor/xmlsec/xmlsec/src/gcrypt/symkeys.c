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
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/crypto.h>


/*****************************************************************************
 *
 * Symmetic (binary) keys - just a wrapper for xmlSecKeyDataBinary
 *
 ****************************************************************************/
static int      xmlSecGCryptSymKeyDataInitialize        (xmlSecKeyDataPtr data);
static int      xmlSecGCryptSymKeyDataDuplicate         (xmlSecKeyDataPtr dst,
                                                         xmlSecKeyDataPtr src);
static void     xmlSecGCryptSymKeyDataFinalize          (xmlSecKeyDataPtr data);
static int      xmlSecGCryptSymKeyDataXmlRead           (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecGCryptSymKeyDataXmlWrite          (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecGCryptSymKeyDataBinRead           (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         const xmlSecByte* buf,
                                                         xmlSecSize bufSize,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecGCryptSymKeyDataBinWrite          (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlSecByte** buf,
                                                         xmlSecSize* bufSize,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecGCryptSymKeyDataGenerate          (xmlSecKeyDataPtr data,
                                                         xmlSecSize sizeBits,
                                                         xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecGCryptSymKeyDataGetType  (xmlSecKeyDataPtr data);
static xmlSecSize        xmlSecGCryptSymKeyDataGetSize  (xmlSecKeyDataPtr data);
static void     xmlSecGCryptSymKeyDataDebugDump         (xmlSecKeyDataPtr data,
                                                         FILE* output);
static void     xmlSecGCryptSymKeyDataDebugXmlDump      (xmlSecKeyDataPtr data,
                                                         FILE* output);
static int      xmlSecGCryptSymKeyDataKlassCheck        (xmlSecKeyDataKlass* klass);

#define xmlSecGCryptSymKeyDataCheckId(data) \
    (xmlSecKeyDataIsValid((data)) && \
     xmlSecGCryptSymKeyDataKlassCheck((data)->id))

static int
xmlSecGCryptSymKeyDataInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataCheckId(data), -1);

    return(xmlSecKeyDataBinaryValueInitialize(data));
}

static int
xmlSecGCryptSymKeyDataDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataCheckId(dst), -1);
    xmlSecAssert2(xmlSecGCryptSymKeyDataCheckId(src), -1);
    xmlSecAssert2(dst->id == src->id, -1);

    return(xmlSecKeyDataBinaryValueDuplicate(dst, src));
}

static void
xmlSecGCryptSymKeyDataFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecGCryptSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueFinalize(data);
}

static int
xmlSecGCryptSymKeyDataXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                               xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueXmlRead(id, key, node, keyInfoCtx));
}

static int
xmlSecGCryptSymKeyDataXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueXmlWrite(id, key, node, keyInfoCtx));
}

static int
xmlSecGCryptSymKeyDataBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    const xmlSecByte* buf, xmlSecSize bufSize,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueBinRead(id, key, buf, bufSize, keyInfoCtx));
}

static int
xmlSecGCryptSymKeyDataBinWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    xmlSecByte** buf, xmlSecSize* bufSize,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueBinWrite(id, key, buf, bufSize, keyInfoCtx));
}

static int
xmlSecGCryptSymKeyDataGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecGCryptSymKeyDataCheckId(data), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecGCryptGenerateRandom(buffer, (sizeBits + 7) / 8));
}

static xmlSecKeyDataType
xmlSecGCryptSymKeyDataGetType(xmlSecKeyDataPtr data) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecGCryptSymKeyDataCheckId(data), xmlSecKeyDataTypeUnknown);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, xmlSecKeyDataTypeUnknown);

    return((xmlSecBufferGetSize(buffer) > 0) ? xmlSecKeyDataTypeSymmetric : xmlSecKeyDataTypeUnknown);
}

static xmlSecSize
xmlSecGCryptSymKeyDataGetSize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecGCryptSymKeyDataCheckId(data), 0);

    return(xmlSecKeyDataBinaryValueGetSize(data));
}

static void
xmlSecGCryptSymKeyDataDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecGCryptSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueDebugDump(data, output);
}

static void
xmlSecGCryptSymKeyDataDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecGCryptSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueDebugXmlDump(data, output);
}

static int
xmlSecGCryptSymKeyDataKlassCheck(xmlSecKeyDataKlass* klass) {
#ifndef XMLSEC_NO_DES
    if(klass == xmlSecGCryptKeyDataDesId) {
        return(1);
    }
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(klass == xmlSecGCryptKeyDataAesId) {
        return(1);
    }
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_HMAC
    if(klass == xmlSecGCryptKeyDataHmacId) {
        return(1);
    }
#endif /* XMLSEC_NO_HMAC */

    return(0);
}

#ifndef XMLSEC_NO_AES
/**************************************************************************
 *
 * <xmlsec:AESKeyValue> processing
 *
 *************************************************************************/
static xmlSecKeyDataKlass xmlSecGCryptKeyDataAesKlass = {
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
    xmlSecGCryptSymKeyDataInitialize,           /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecGCryptSymKeyDataDuplicate,            /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecGCryptSymKeyDataFinalize,             /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecGCryptSymKeyDataGenerate,             /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecGCryptSymKeyDataGetType,              /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecGCryptSymKeyDataGetSize,              /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecGCryptSymKeyDataXmlRead,              /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecGCryptSymKeyDataXmlWrite,             /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecGCryptSymKeyDataBinRead,              /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecGCryptSymKeyDataBinWrite,             /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecGCryptSymKeyDataDebugDump,            /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecGCryptSymKeyDataDebugXmlDump,         /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptKeyDataAesGetKlass:
 *
 * The AES key data klass.
 *
 * Returns: AES key data klass.
 */
xmlSecKeyDataId
xmlSecGCryptKeyDataAesGetKlass(void) {
    return(&xmlSecGCryptKeyDataAesKlass);
}

/**
 * xmlSecGCryptKeyDataAesSet:
 * @data:               the pointer to AES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of AES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGCryptKeyDataAesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataAesId), -1);
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
static xmlSecKeyDataKlass xmlSecGCryptKeyDataDesKlass = {
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
    xmlSecGCryptSymKeyDataInitialize,           /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecGCryptSymKeyDataDuplicate,            /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecGCryptSymKeyDataFinalize,             /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecGCryptSymKeyDataGenerate,             /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecGCryptSymKeyDataGetType,              /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecGCryptSymKeyDataGetSize,              /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecGCryptSymKeyDataXmlRead,              /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecGCryptSymKeyDataXmlWrite,             /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecGCryptSymKeyDataBinRead,              /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecGCryptSymKeyDataBinWrite,             /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecGCryptSymKeyDataDebugDump,            /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecGCryptSymKeyDataDebugXmlDump,         /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptKeyDataDesGetKlass:
 *
 * The DES key data klass.
 *
 * Returns: DES key data klass.
 */
xmlSecKeyDataId
xmlSecGCryptKeyDataDesGetKlass(void) {
    return(&xmlSecGCryptKeyDataDesKlass);
}

/**
 * xmlSecGCryptKeyDataDesSet:
 * @data:               the pointer to DES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of DES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGCryptKeyDataDesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDesId), -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecBufferSetData(buffer, buf, bufSize));
}

#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_HMAC
/**************************************************************************
 *
 * <xmlsec:HMACKeyValue> processing
 *
 *************************************************************************/
static xmlSecKeyDataKlass xmlSecGCryptKeyDataHmacKlass = {
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
    xmlSecGCryptSymKeyDataInitialize,           /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecGCryptSymKeyDataDuplicate,            /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecGCryptSymKeyDataFinalize,             /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecGCryptSymKeyDataGenerate,             /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecGCryptSymKeyDataGetType,              /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecGCryptSymKeyDataGetSize,              /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecGCryptSymKeyDataXmlRead,              /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecGCryptSymKeyDataXmlWrite,             /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecGCryptSymKeyDataBinRead,              /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecGCryptSymKeyDataBinWrite,             /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecGCryptSymKeyDataDebugDump,            /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecGCryptSymKeyDataDebugXmlDump,         /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptKeyDataHmacGetKlass:
 *
 * The HMAC key data klass.
 *
 * Returns: HMAC key data klass.
 */
xmlSecKeyDataId
xmlSecGCryptKeyDataHmacGetKlass(void) {
    return(&xmlSecGCryptKeyDataHmacKlass);
}

/**
 * xmlSecGCryptKeyDataHmacSet:
 * @data:               the pointer to HMAC key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of HMAC key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGCryptKeyDataHmacSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataHmacId), -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecBufferSetData(buffer, buf, bufSize));
}

#endif /* XMLSEC_NO_HMAC */


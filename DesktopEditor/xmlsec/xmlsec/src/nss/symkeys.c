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

#include <xmlsec/nss/crypto.h>

/*****************************************************************************
 *
 * Symmetic (binary) keys - just a wrapper for xmlSecKeyDataBinary
 *
 ****************************************************************************/
static int      xmlSecNssSymKeyDataInitialize           (xmlSecKeyDataPtr data);
static int      xmlSecNssSymKeyDataDuplicate            (xmlSecKeyDataPtr dst,
                                                         xmlSecKeyDataPtr src);
static void     xmlSecNssSymKeyDataFinalize             (xmlSecKeyDataPtr data);
static int      xmlSecNssSymKeyDataXmlRead              (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecNssSymKeyDataXmlWrite             (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecNssSymKeyDataBinRead              (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         const xmlSecByte* buf,
                                                         xmlSecSize bufSize,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecNssSymKeyDataBinWrite             (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlSecByte** buf,
                                                         xmlSecSize* bufSize,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int      xmlSecNssSymKeyDataGenerate             (xmlSecKeyDataPtr data,
                                                         xmlSecSize sizeBits,
                                                         xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecNssSymKeyDataGetType     (xmlSecKeyDataPtr data);
static xmlSecSize       xmlSecNssSymKeyDataGetSize              (xmlSecKeyDataPtr data);
static void     xmlSecNssSymKeyDataDebugDump    (xmlSecKeyDataPtr data,
                                                         FILE* output);
static void     xmlSecNssSymKeyDataDebugXmlDump (xmlSecKeyDataPtr data,
                                                         FILE* output);
static int      xmlSecNssSymKeyDataKlassCheck   (xmlSecKeyDataKlass* klass);

#define xmlSecNssSymKeyDataCheckId(data) \
    (xmlSecKeyDataIsValid((data)) && \
     xmlSecNssSymKeyDataKlassCheck((data)->id))

static int
xmlSecNssSymKeyDataInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecNssSymKeyDataCheckId(data), -1);

    return(xmlSecKeyDataBinaryValueInitialize(data));
}

static int
xmlSecNssSymKeyDataDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecNssSymKeyDataCheckId(dst), -1);
    xmlSecAssert2(xmlSecNssSymKeyDataCheckId(src), -1);
    xmlSecAssert2(dst->id == src->id, -1);

    return(xmlSecKeyDataBinaryValueDuplicate(dst, src));
}

static void
xmlSecNssSymKeyDataFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecNssSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueFinalize(data);
}

static int
xmlSecNssSymKeyDataXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                               xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecNssSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueXmlRead(id, key, node, keyInfoCtx));
}

static int
xmlSecNssSymKeyDataXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecNssSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueXmlWrite(id, key, node, keyInfoCtx));
}

static int
xmlSecNssSymKeyDataBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    const xmlSecByte* buf, xmlSecSize bufSize,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecNssSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueBinRead(id, key, buf, bufSize, keyInfoCtx));
}

static int
xmlSecNssSymKeyDataBinWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    xmlSecByte** buf, xmlSecSize* bufSize,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(xmlSecNssSymKeyDataKlassCheck(id), -1);

    return(xmlSecKeyDataBinaryValueBinWrite(id, key, buf, bufSize, keyInfoCtx));
}

static int
xmlSecNssSymKeyDataGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecNssSymKeyDataCheckId(data), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecNssGenerateRandom(buffer, (sizeBits + 7) / 8));
}

static xmlSecKeyDataType
xmlSecNssSymKeyDataGetType(xmlSecKeyDataPtr data) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecNssSymKeyDataCheckId(data), xmlSecKeyDataTypeUnknown);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, xmlSecKeyDataTypeUnknown);

    return((xmlSecBufferGetSize(buffer) > 0) ? xmlSecKeyDataTypeSymmetric : xmlSecKeyDataTypeUnknown);
}

static xmlSecSize
xmlSecNssSymKeyDataGetSize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecNssSymKeyDataCheckId(data), 0);

    return(xmlSecKeyDataBinaryValueGetSize(data));
}

static void
xmlSecNssSymKeyDataDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecNssSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueDebugDump(data, output);
}

static void
xmlSecNssSymKeyDataDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecNssSymKeyDataCheckId(data));

    xmlSecKeyDataBinaryValueDebugXmlDump(data, output);
}

static int
xmlSecNssSymKeyDataKlassCheck(xmlSecKeyDataKlass* klass) {
#ifndef XMLSEC_NO_DES
    if(klass == xmlSecNssKeyDataDesId) {
        return(1);
    }
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_AES
    if(klass == xmlSecNssKeyDataAesId) {
        return(1);
    }
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_HMAC
    if(klass == xmlSecNssKeyDataHmacId) {
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
static xmlSecKeyDataKlass xmlSecNssKeyDataAesKlass = {
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
    xmlSecNssSymKeyDataInitialize,              /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecNssSymKeyDataDuplicate,               /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecNssSymKeyDataFinalize,                /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecNssSymKeyDataGenerate,                /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecNssSymKeyDataGetType,                 /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecNssSymKeyDataGetSize,         /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecNssSymKeyDataXmlRead,         /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecNssSymKeyDataXmlWrite,                /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecNssSymKeyDataBinRead,         /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecNssSymKeyDataBinWrite,                /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecNssSymKeyDataDebugDump,               /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecNssSymKeyDataDebugXmlDump,    /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataAesGetKlass:
 *
 * The AES key data klass.
 *
 * Returns: AES key data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataAesGetKlass(void) {
    return(&xmlSecNssKeyDataAesKlass);
}

/**
 * xmlSecNssKeyDataAesSet:
 * @data:               the pointer to AES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of AES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssKeyDataAesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataAesId), -1);
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
static xmlSecKeyDataKlass xmlSecNssKeyDataDesKlass = {
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
    xmlSecNssSymKeyDataInitialize,              /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecNssSymKeyDataDuplicate,               /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecNssSymKeyDataFinalize,                /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecNssSymKeyDataGenerate,                /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecNssSymKeyDataGetType,                 /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecNssSymKeyDataGetSize,         /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecNssSymKeyDataXmlRead,         /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecNssSymKeyDataXmlWrite,                /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecNssSymKeyDataBinRead,         /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecNssSymKeyDataBinWrite,                /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecNssSymKeyDataDebugDump,               /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecNssSymKeyDataDebugXmlDump,    /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataDesGetKlass:
 *
 * The DES key data klass.
 *
 * Returns: DES key data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataDesGetKlass(void) {
    return(&xmlSecNssKeyDataDesKlass);
}

/**
 * xmlSecNssKeyDataDesSet:
 * @data:               the pointer to DES key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of DES key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssKeyDataDesSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDesId), -1);
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
static xmlSecKeyDataKlass xmlSecNssKeyDataHmacKlass = {
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
    xmlSecNssSymKeyDataInitialize,              /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecNssSymKeyDataDuplicate,               /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecNssSymKeyDataFinalize,                /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecNssSymKeyDataGenerate,                /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecNssSymKeyDataGetType,                 /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecNssSymKeyDataGetSize,         /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecNssSymKeyDataXmlRead,         /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecNssSymKeyDataXmlWrite,                /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecNssSymKeyDataBinRead,         /* xmlSecKeyDataBinReadMethod binRead; */
    xmlSecNssSymKeyDataBinWrite,                /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecNssSymKeyDataDebugDump,               /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecNssSymKeyDataDebugXmlDump,    /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataHmacGetKlass:
 *
 * The HMAC key data klass.
 *
 * Returns: HMAC key data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataHmacGetKlass(void) {
    return(&xmlSecNssKeyDataHmacKlass);
}

/**
 * xmlSecNssKeyDataHmacSet:
 * @data:               the pointer to HMAC key data.
 * @buf:                the pointer to key value.
 * @bufSize:            the key value size (in bytes).
 *
 * Sets the value of HMAC key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssKeyDataHmacSet(xmlSecKeyDataPtr data, const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataHmacId), -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecBufferSetData(buffer, buf, bufSize));
}

#endif /* XMLSEC_NO_HMAC */


/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Key data.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */

#include "globals.h"

#include <stdlib.h>
#include <string.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/transforms.h>
#include <xmlsec/base64.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/errors.h>


/**************************************************************************
 *
 * Global xmlSecKeyDataIds list functions
 *
 *************************************************************************/
static xmlSecPtrList xmlSecAllKeyDataIds;

/**
 * xmlSecKeyDataIdsGet:
 *
 * Gets global registered key data klasses list.
 *
 * Returns: the pointer to list of all registered key data klasses.
 */
xmlSecPtrListPtr
xmlSecKeyDataIdsGet(void) {
    return(&xmlSecAllKeyDataIds);
}

/**
 * xmlSecKeyDataIdsInit:
 *
 * Initializes the key data klasses. This function is called from the
 * #xmlSecInit function and the application should not call it directly.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecKeyDataIdsInit(void) {
    int ret;

    ret = xmlSecPtrListInitialize(xmlSecKeyDataIdsGet(), xmlSecKeyDataIdListId);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListPtrInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecKeyDataIdListId");
        return(-1);
    }

    ret = xmlSecKeyDataIdsRegisterDefault();
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataIdsRegisterDefault",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecKeyDataIdsShutdown:
 *
 * Shuts down the keys data klasses. This function is called from the
 * #xmlSecShutdown function and the application should not call it directly.
 */
void
xmlSecKeyDataIdsShutdown(void) {
    xmlSecPtrListFinalize(xmlSecKeyDataIdsGet());
}

/**
 * xmlSecKeyDataIdsRegister:
 * @id:                 the key data klass.
 *
 * Registers @id in the global list of key data klasses.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecKeyDataIdsRegister(xmlSecKeyDataId id) {
    int ret;

    xmlSecAssert2(id != xmlSecKeyDataIdUnknown, -1);

    ret = xmlSecPtrListAdd(xmlSecKeyDataIdsGet(), (xmlSecPtr)id);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListAdd",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "dataId=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecKeyDataIdsRegisterDefault:
 *
 * Registers default (implemented by XML Security Library)
 * key data klasses: <dsig:KeyName/> element processing klass,
 * <dsig:KeyValue/> element processing klass, ...
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecKeyDataIdsRegisterDefault(void) {
    if(xmlSecKeyDataIdsRegister(xmlSecKeyDataNameId) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecKeyDataNameId");
        return(-1);
    }

    if(xmlSecKeyDataIdsRegister(xmlSecKeyDataValueId) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecKeyDataValueId");
        return(-1);
    }

    if(xmlSecKeyDataIdsRegister(xmlSecKeyDataRetrievalMethodId) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecKeyDataRetrievalMethodId");
        return(-1);
    }

#ifndef XMLSEC_NO_XMLENC
    if(xmlSecKeyDataIdsRegister(xmlSecKeyDataEncryptedKeyId) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecKeyDataEncryptedKeyId");
        return(-1);
    }
#endif /* XMLSEC_NO_XMLENC */

    return(0);
}

/**************************************************************************
 *
 * xmlSecKeyData functions
 *
 *************************************************************************/
/**
 * xmlSecKeyDataCreate:
 * @id:                 the data id.
 *
 * Allocates and initializes new key data of the specified type @id.
 * Caller is responsible for destroying returned object with
 * #xmlSecKeyDataDestroy function.
 *
 * Returns: the pointer to newly allocated key data structure
 * or NULL if an error occurs.
 */
xmlSecKeyDataPtr
xmlSecKeyDataCreate(xmlSecKeyDataId id)  {
    xmlSecKeyDataPtr data;
    int ret;

    xmlSecAssert2(id != NULL, NULL);
    xmlSecAssert2(id->klassSize >= sizeof(xmlSecKeyDataKlass), NULL);
    xmlSecAssert2(id->objSize >= sizeof(xmlSecKeyData), NULL);
    xmlSecAssert2(id->name != NULL, NULL);

    /* Allocate a new xmlSecKeyData and fill the fields. */
    data = (xmlSecKeyDataPtr)xmlMalloc(id->objSize);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", id->objSize);
        return(NULL);
    }
    memset(data, 0, id->objSize);
    data->id = id;

    if(id->initialize != NULL) {
        ret = (id->initialize)(data);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "id->initialize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyDataDestroy(data);
            return(NULL);
        }
    }

    return(data);
}

/**
 * xmlSecKeyDataDuplicate:
 * @data:               the pointer to the key data.
 *
 * Creates a duplicate of the given @data. Caller is responsible for
 * destroying returned object with #xmlSecKeyDataDestroy function.
 *
 * Returns: the pointer to newly allocated key data structure
 * or NULL if an error occurs.
 */
xmlSecKeyDataPtr
xmlSecKeyDataDuplicate(xmlSecKeyDataPtr data) {
    xmlSecKeyDataPtr newData;
    int ret;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(data->id->duplicate != NULL, NULL);

    newData = xmlSecKeyDataCreate(data->id);
    if(newData == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    ret = (data->id->duplicate)(newData, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "id->duplicate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(newData);
        return(NULL);
    }

    return(newData);
}

/**
 * xmlSecKeyDataDestroy:
 * @data:               the pointer to the key data.
 *
 * Destroys the data and frees all allocated memory.
 */
void
xmlSecKeyDataDestroy(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(data->id->objSize > 0);

    if(data->id->finalize != NULL) {
        (data->id->finalize)(data);
    }
    memset(data, 0, data->id->objSize);
    xmlFree(data);
}


/**
 * xmlSecKeyDataXmlRead:
 * @id:                 the data klass.
 * @key:                the destination key.
 * @node:               the pointer to an XML node.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Reads the key data of klass @id from XML @node and adds them to @key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(id != NULL, -1);
    xmlSecAssert2(id->xmlRead != NULL, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    return((id->xmlRead)(id, key, node, keyInfoCtx));
}

/**
 * xmlSecKeyDataXmlWrite:
 * @id:                 the data klass.
 * @key:                the source key.
 * @node:               the pointer to an XML node.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Writes the key data of klass @id from @key to an XML @node.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(id != NULL, -1);
    xmlSecAssert2(id->xmlWrite != NULL, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    return((id->xmlWrite)(id, key, node, keyInfoCtx));
}

/**
 * xmlSecKeyDataBinRead:
 * @id:                 the data klass.
 * @key:                the destination key.
 * @buf:                the input binary buffer.
 * @bufSize:            the input buffer size.
 * @keyInfoCtx:         the <dsig:KeyInfo/> node processing context.
 *
 * Reads the key data of klass @id from binary buffer @buf to @key.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecKeyDataBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                    const xmlSecByte* buf, xmlSecSize bufSize,
                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(id != NULL, -1);
    xmlSecAssert2(id->binRead != NULL, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);

    return((id->binRead)(id, key, buf, bufSize, keyInfoCtx));
}

/**
 * xmlSecKeyDataBinWrite:
 * @id:                 the data klass.
 * @key:                the source key.
 * @buf:                the output binary buffer.
 * @bufSize:            the output buffer size.
 * @keyInfoCtx:         the <dsig:KeyInfo/> node processing context.
 *
 * Writes the key data of klass @id from the @key to a binary buffer @buf.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecKeyDataBinWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                    xmlSecByte** buf, xmlSecSize* bufSize,
                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAssert2(id != NULL, -1);
    xmlSecAssert2(id->binWrite != NULL, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);

    return((id->binWrite)(id, key, buf, bufSize, keyInfoCtx));
}

/**
 * xmlSecKeyDataGenerate:
 * @data:               the pointer to key data.
 * @sizeBits:           the desired key data size (in bits).
 * @type:               the desired key data type.
 *
 * Generates new key data of given size and type.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits,
                      xmlSecKeyDataType type) {
    int ret;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(data->id->generate != NULL, -1);

    /* write data */
    ret = data->id->generate(data, sizeBits, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "id->generate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", sizeBits);
        return(-1);
    }
    return(0);
}

/**
 * xmlSecKeyDataGetType:
 * @data:               the pointer to key data.
 *
 * Gets key data type.
 *
 * Returns: key data type.
 */
xmlSecKeyDataType
xmlSecKeyDataGetType(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataIsValid(data), xmlSecKeyDataTypeUnknown);
    xmlSecAssert2(data->id->getType != NULL, xmlSecKeyDataTypeUnknown);

    return(data->id->getType(data));
}

/**
 * xmlSecKeyDataGetSize:
 * @data:               the pointer to key data.
 *
 * Gets key data size.
 *
 * Returns: key data size (in bits).
 */
xmlSecSize
xmlSecKeyDataGetSize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataIsValid(data), 0);
    xmlSecAssert2(data->id->getSize != NULL, 0);

    return(data->id->getSize(data));
}

/**
 * xmlSecKeyDataGetIdentifier:
 * @data:               the pointer to key data.
 *
 * Gets key data identifier string.
 *
 * Returns: key data id string.
 */
const xmlChar*
xmlSecKeyDataGetIdentifier(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(data->id->getIdentifier != NULL, NULL);

    return(data->id->getIdentifier(data));
}

/**
 * xmlSecKeyDataDebugDump:
 * @data:               the pointer to key data.
 * @output:             the pointer to output FILE.
 *
 * Prints key data debug info.
 */
void
xmlSecKeyDataDebugDump(xmlSecKeyDataPtr data, FILE *output) {
    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(data->id->debugDump != NULL);
    xmlSecAssert(output != NULL);

    data->id->debugDump(data, output);
}

/**
 * xmlSecKeyDataDebugXmlDump:
 * @data:               the pointer to key data.
 * @output:             the pointer to output FILE.
 *
 * Prints key data debug info in XML format.
 */
void
xmlSecKeyDataDebugXmlDump(xmlSecKeyDataPtr data, FILE *output) {
    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(data->id->debugXmlDump != NULL);
    xmlSecAssert(output != NULL);

    data->id->debugXmlDump(data, output);
}

/**************************************************************************
 *
 * xmlSecKeyDataBinary methods
 *
 * key (xmlSecBuffer) is located after xmlSecKeyData structure
 *
 *************************************************************************/
/**
 * xmlSecKeyDataBinaryValueInitialize:
 * @data:               the pointer to binary key data.
 *
 * Initializes key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueInitialize(xmlSecKeyDataPtr data) {
    xmlSecBufferPtr buffer;
    int ret;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize), -1);

    /* initialize buffer */
    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    ret = xmlSecBufferInitialize(buffer, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecKeyDataBinaryValueDuplicate:
 * @dst:                the pointer to destination binary key data.
 * @src:                the pointer to source binary key data.
 *
 * Copies binary key data from @src to @dst.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecBufferPtr buffer;
    int ret;

    xmlSecAssert2(xmlSecKeyDataIsValid(dst), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(dst, xmlSecKeyDataBinarySize), -1);
    xmlSecAssert2(xmlSecKeyDataIsValid(src), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(src, xmlSecKeyDataBinarySize), -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(src);
    xmlSecAssert2(buffer != NULL, -1);

    /* copy data */
    ret = xmlSecKeyDataBinaryValueSetBuffer(dst,
                    xmlSecBufferGetData(buffer),
                    xmlSecBufferGetSize(buffer));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                    "xmlSecKeyDataBinaryValueSetBuffer",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecKeyDataBinaryValueFinalize:
 * @data:               the pointer to binary key data.
 *
 * Cleans up binary key data.
 */
void
xmlSecKeyDataBinaryValueFinalize(xmlSecKeyDataPtr data) {
    xmlSecBufferPtr buffer;

    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize));

    /* initialize buffer */
    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert(buffer != NULL);

    xmlSecBufferFinalize(buffer);
}

/**
 * xmlSecKeyDataBinaryValueXmlRead:
 * @id:                 the data klass.
 * @key:                the pointer to destination key.
 * @node:               the pointer to an XML node.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Reads binary key data from @node to the key by base64 decoding the @node content.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar* str;
    xmlSecSize len;
    xmlSecKeyDataPtr data;
    int ret;

    xmlSecAssert2(id != xmlSecKeyDataIdUnknown, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    str = xmlNodeGetContent(node);
    if(str == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(node)),
                    XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* usual trick: decode into the same buffer */
    ret = xmlSecBase64Decode(str, (xmlSecByte*)str, xmlStrlen(str));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecBase64Decode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(str);
        return(-1);
    }
    len = ret;

    /* check do we have a key already */
    data = xmlSecKeyGetValue(key);
    if(data != NULL) {
        xmlSecBufferPtr buffer;

        if(!xmlSecKeyDataCheckId(data, id)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        XMLSEC_ERRORS_R_KEY_DATA_ALREADY_EXIST,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlFree(str);
            return(-1);
        }

        buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
        if((buffer != NULL) && ((xmlSecSize)xmlSecBufferGetSize(buffer) != len)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        XMLSEC_ERRORS_R_KEY_DATA_ALREADY_EXIST,
                        "cur-data-size=%d;new-data-size=%d",
                        xmlSecBufferGetSize(buffer), len);
            xmlFree(str);
            return(-1);
        }
        if((buffer != NULL) && (len > 0) && (memcmp(xmlSecBufferGetData(buffer), str, len) != 0)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        XMLSEC_ERRORS_R_KEY_DATA_ALREADY_EXIST,
                        "key already has a different value");
            xmlFree(str);
            return(-1);
        }
        if(buffer != NULL) {
            /* we already have exactly the same key */
            xmlFree(str);
            return(0);
        }

        /* we have binary key value with empty buffer */
    }


    data = xmlSecKeyDataCreate(id);
    if(data == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(str);
        return(-1);
    }

    ret = xmlSecKeyDataBinaryValueSetBuffer(data, (xmlSecByte*)str, len);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataBinaryValueSetBuffer",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", len);
        xmlSecKeyDataDestroy(data);
        xmlFree(str);
        return(-1);
    }
    xmlFree(str);

    if(xmlSecKeyReqMatchKeyValue(&(keyInfoCtx->keyReq), data) != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyReqMatchKeyValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        return(0);
    }

    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecKeyDataBinaryValueXmlWrite:
 * @id:                 the data klass.
 * @key:                the pointer to source key.
 * @node:               the pointer to an XML node.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Base64 encodes binary key data of klass @id from the @key and
 * sets to the @node content.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                            xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecBufferPtr buffer;
    xmlSecKeyDataPtr value;
    xmlChar* str;

    xmlSecAssert2(id != xmlSecKeyDataIdUnknown, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if((xmlSecKeyDataTypeSymmetric & keyInfoCtx->keyReq.keyType) == 0) {
        /* we can have only symmetric key */
        return(0);
    }

    value = xmlSecKeyGetValue(key);
    xmlSecAssert2(xmlSecKeyDataIsValid(value), -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(value);
    xmlSecAssert2(buffer != NULL, -1);

    str = xmlSecBase64Encode(xmlSecBufferGetData(buffer),
                             xmlSecBufferGetSize(buffer),
                             keyInfoCtx->base64LineSize);
    if(str == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlNodeSetContent(node, str);
    xmlFree(str);
    return(0);
}

/**
 * xmlSecKeyDataBinaryValueBinRead:
 * @id:                 the data klass.
 * @key:                the pointer to destination key.
 * @buf:                the source binary buffer.
 * @bufSize:            the source binary buffer size.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Reads binary key data of the klass @id from @buf to the @key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                const xmlSecByte* buf, xmlSecSize bufSize,
                                xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    int ret;

    xmlSecAssert2(id != xmlSecKeyDataIdUnknown, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    /* check do we have a key already */
    data = xmlSecKeyGetValue(key);
    if(data != NULL) {
        xmlSecBufferPtr buffer;

        if(!xmlSecKeyDataCheckId(data, id)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        XMLSEC_ERRORS_R_KEY_DATA_ALREADY_EXIST,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
        if((buffer != NULL) && ((xmlSecSize)xmlSecBufferGetSize(buffer) != bufSize)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        XMLSEC_ERRORS_R_KEY_DATA_ALREADY_EXIST,
                        "cur-data-size=%d;new-data-size=%d",
                        xmlSecBufferGetSize(buffer), bufSize);
            return(-1);
        }
        if((buffer != NULL) && (bufSize > 0) && (memcmp(xmlSecBufferGetData(buffer), buf, bufSize) != 0)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        XMLSEC_ERRORS_R_KEY_DATA_ALREADY_EXIST,
                        "key already has a different value");
            return(-1);
        }
        if(buffer != NULL) {
            /* we already have exactly the same key */
            return(0);
        }

        /* we have binary key value with empty buffer */
    }

    data = xmlSecKeyDataCreate(id);
    if(data == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecKeyDataBinaryValueSetBuffer(data, buf, bufSize);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataBinaryValueSetBuffer",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", bufSize);
        xmlSecKeyDataDestroy(data);
        return(-1);
    }

    if(xmlSecKeyReqMatchKeyValue(&(keyInfoCtx->keyReq), data) != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyReqMatchKeyValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        return(0);
    }

    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecKeyDataBinaryValueBinWrite:
 * @id:                 the data klass.
 * @key:                the pointer to source key.
 * @buf:                the destination binary buffer.
 * @bufSize:            the destination binary buffer size.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Writes binary key data of klass @id from the @key to @buf.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueBinWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlSecByte** buf, xmlSecSize* bufSize,
                                xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr value;
    xmlSecBufferPtr buffer;

    xmlSecAssert2(id != xmlSecKeyDataIdUnknown, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if((xmlSecKeyDataTypeSymmetric & keyInfoCtx->keyReq.keyType) == 0) {
        /* we can have only symmetric key */
        return(0);
    }

    value = xmlSecKeyGetValue(key);
    xmlSecAssert2(xmlSecKeyDataIsValid(value), -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(key->value);
    xmlSecAssert2(buffer != NULL, -1);

    (*bufSize) = xmlSecBufferGetSize(buffer);
    (*buf) = (xmlSecByte*) xmlMalloc((*bufSize));
    if((*buf) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    memcpy((*buf), xmlSecBufferGetData(buffer), (*bufSize));
    return(0);
}

/**
 * xmlSecKeyDataBinaryValueDebugDump:
 * @data:               the pointer to binary key data.
 * @output:             the pointer to output FILE.
 *
 * Prints binary key data debug information to @output.
 */
void
xmlSecKeyDataBinaryValueDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecBufferPtr buffer;

    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize));
    xmlSecAssert(data->id->dataNodeName != NULL);
    xmlSecAssert(output != NULL);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert(buffer != NULL);

    /* print only size, everything else is sensitive */
    fprintf(output, "=== %s: size=%d\n", data->id->dataNodeName,
                                         xmlSecKeyDataGetSize(data));
}

/**
 * xmlSecKeyDataBinaryValueDebugXmlDump:
 * @data:               the pointer to binary key data.
 * @output:             the pointer to output FILE.
 *
 * Prints binary key data debug information to @output in XML format.
 */
void
xmlSecKeyDataBinaryValueDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecBufferPtr buffer;

    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize));
    xmlSecAssert(data->id->dataNodeName != NULL);
    xmlSecAssert(output != NULL);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert(buffer != NULL);

    /* print only size, everything else is sensitive */
    fprintf(output, "<%s size=\"%d\" />\n", data->id->dataNodeName,
                                            xmlSecKeyDataGetSize(data));
}

/**
 * xmlSecKeyDataBinaryValueGetSize:
 * @data:               the pointer to binary key data.
 *
 * Gets the binary key data size.
 *
 * Returns: binary key data size in bits.
 */
xmlSecSize
xmlSecKeyDataBinaryValueGetSize(xmlSecKeyDataPtr data) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), 0);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize), 0);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, 0);

    /* return size in bits */
    return(8 * xmlSecBufferGetSize(buffer));
}

/**
 * xmlSecKeyDataBinaryValueGetBuffer:
 * @data:               the pointer to binary key data.
 *
 * Gets the binary key data buffer.
 *
 * Returns: pointer to binary key data buffer.
 */
xmlSecBufferPtr
xmlSecKeyDataBinaryValueGetBuffer(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize), NULL);

    /* key (xmlSecBuffer) is located after xmlSecKeyData structure */
    return((xmlSecBufferPtr)(((xmlSecByte*)data) + sizeof(xmlSecKeyData)));
}

/**
 * xmlSecKeyDataBinaryValueSetBuffer:
 * @data:               the pointer to binary key data.
 * @buf:                the pointer to binary buffer.
 * @bufSize:            the binary buffer size.
 *
 * Sets the value of @data to @buf.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecKeyDataBinaryValueSetBuffer(xmlSecKeyDataPtr data,
                        const xmlSecByte* buf, xmlSecSize bufSize) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecKeyDataBinarySize), -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);

    buffer = xmlSecKeyDataBinaryValueGetBuffer(data);
    xmlSecAssert2(buffer != NULL, -1);

    return(xmlSecBufferSetData(buffer, buf, bufSize));
}

/***********************************************************************
 *
 * Keys Data list
 *
 **********************************************************************/
static xmlSecPtrListKlass xmlSecKeyDataListKlass = {
    BAD_CAST "key-data-list",
    (xmlSecPtrDuplicateItemMethod)xmlSecKeyDataDuplicate,       /* xmlSecPtrDuplicateItemMethod duplicateItem; */
    (xmlSecPtrDestroyItemMethod)xmlSecKeyDataDestroy,           /* xmlSecPtrDestroyItemMethod destroyItem; */
    (xmlSecPtrDebugDumpItemMethod)xmlSecKeyDataDebugDump,       /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    (xmlSecPtrDebugDumpItemMethod)xmlSecKeyDataDebugXmlDump,    /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};

/**
 * xmlSecKeyDataListGetKlass:
 *
 * The key data list klass.
 *
 * Returns: pointer to the key data list klass.
 */
xmlSecPtrListId
xmlSecKeyDataListGetKlass(void) {
    return(&xmlSecKeyDataListKlass);
}


/***********************************************************************
 *
 * Keys Data Ids list
 *
 **********************************************************************/
static xmlSecPtrListKlass xmlSecKeyDataIdListKlass = {
    BAD_CAST "key-data-ids-list",
    NULL,                                                       /* xmlSecPtrDuplicateItemMethod duplicateItem; */
    NULL,                                                       /* xmlSecPtrDestroyItemMethod destroyItem; */
    NULL,                                                       /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    NULL,                                                       /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};

/**
 * xmlSecKeyDataIdListGetKlass:
 *
 * The key data id list klass.
 *
 * Returns: pointer to the key data id list klass.
 */
xmlSecPtrListId
xmlSecKeyDataIdListGetKlass(void) {
    return(&xmlSecKeyDataIdListKlass);
}

/**
 * xmlSecKeyDataIdListFind:
 * @list:               the pointer to key data ids list.
 * @dataId:             the key data klass.
 *
 * Lookups @dataId in @list.
 *
 * Returns: 1 if @dataId is found in the @list, 0 if not and a negative
 * value if an error occurs.
 */
int
xmlSecKeyDataIdListFind(xmlSecPtrListPtr list, xmlSecKeyDataId dataId) {
    xmlSecSize i, size;

    xmlSecAssert2(xmlSecPtrListCheckId(list, xmlSecKeyDataIdListId), 0);
    xmlSecAssert2(dataId != NULL, 0);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        if((xmlSecKeyDataId)xmlSecPtrListGetItem(list, i) == dataId) {
            return(1);
        }
    }
    return(0);
}

/**
 * xmlSecKeyDataIdListFindByNode:
 * @list:               the pointer to key data ids list.
 * @nodeName:           the desired key data klass XML node name.
 * @nodeNs:             the desired key data klass XML node namespace.
 * @usage:              the desired key data usage.
 *
 * Lookups data klass in the list with given @nodeName, @nodeNs and
 * @usage in the @list.
 *
 * Returns: key data klass is found and NULL otherwise.
 */
xmlSecKeyDataId
xmlSecKeyDataIdListFindByNode(xmlSecPtrListPtr list, const xmlChar* nodeName,
                            const xmlChar* nodeNs, xmlSecKeyDataUsage usage) {
    xmlSecKeyDataId dataId;
    xmlSecSize i, size;

    xmlSecAssert2(xmlSecPtrListCheckId(list, xmlSecKeyDataIdListId), xmlSecKeyDataIdUnknown);
    xmlSecAssert2(nodeName != NULL, xmlSecKeyDataIdUnknown);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        dataId = (xmlSecKeyDataId)xmlSecPtrListGetItem(list, i);
        xmlSecAssert2(dataId != xmlSecKeyDataIdUnknown, xmlSecKeyDataIdUnknown);

        if(((usage & dataId->usage) != 0) &&
           xmlStrEqual(nodeName, dataId->dataNodeName) &&
           xmlStrEqual(nodeNs, dataId->dataNodeNs)) {

           return(dataId);
        }
    }
    return(xmlSecKeyDataIdUnknown);
}

/**
 * xmlSecKeyDataIdListFindByHref:
 * @list:               the pointer to key data ids list.
 * @href:               the desired key data klass href.
 * @usage:              the desired key data usage.
 *
 * Lookups data klass in the list with given @href and @usage in @list.
 *
 * Returns: key data klass is found and NULL otherwise.
 */
xmlSecKeyDataId
xmlSecKeyDataIdListFindByHref(xmlSecPtrListPtr list, const xmlChar* href,
                            xmlSecKeyDataUsage usage) {
    xmlSecKeyDataId dataId;
    xmlSecSize i, size;

    xmlSecAssert2(xmlSecPtrListCheckId(list, xmlSecKeyDataIdListId), xmlSecKeyDataIdUnknown);
    xmlSecAssert2(href != NULL, xmlSecKeyDataIdUnknown);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        dataId = (xmlSecKeyDataId)xmlSecPtrListGetItem(list, i);
        xmlSecAssert2(dataId != xmlSecKeyDataIdUnknown, xmlSecKeyDataIdUnknown);

        if(((usage & dataId->usage) != 0) && (dataId->href != NULL) &&
           xmlStrEqual(href, dataId->href)) {

           return(dataId);
        }
    }
    return(xmlSecKeyDataIdUnknown);
}

/**
 * xmlSecKeyDataIdListFindByName:
 * @list:               the pointer to key data ids list.
 * @name:               the desired key data klass name.
 * @usage:              the desired key data usage.
 *
 * Lookups data klass in the list with given @name and @usage in @list.
 *
 * Returns: key data klass is found and NULL otherwise.
 */
xmlSecKeyDataId
xmlSecKeyDataIdListFindByName(xmlSecPtrListPtr list, const xmlChar* name,
                            xmlSecKeyDataUsage usage) {
    xmlSecKeyDataId dataId;
    xmlSecSize i, size;

    xmlSecAssert2(xmlSecPtrListCheckId(list, xmlSecKeyDataIdListId), xmlSecKeyDataIdUnknown);
    xmlSecAssert2(name != NULL, xmlSecKeyDataIdUnknown);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        dataId = (xmlSecKeyDataId)xmlSecPtrListGetItem(list, i);
        xmlSecAssert2(dataId != xmlSecKeyDataIdUnknown, xmlSecKeyDataIdUnknown);

        if(((usage & dataId->usage) != 0) && (dataId->name != NULL) &&
           xmlStrEqual(name, BAD_CAST dataId->name)) {

           return(dataId);
        }
    }
    return(xmlSecKeyDataIdUnknown);
}

/**
 * xmlSecKeyDataIdListDebugDump:
 * @list:               the pointer to key data ids list.
 * @output:             the pointer to output FILE.
 *
 * Prints binary key data debug information to @output.
 */
void
xmlSecKeyDataIdListDebugDump(xmlSecPtrListPtr list, FILE* output) {
    xmlSecKeyDataId dataId;
    xmlSecSize i, size;

    xmlSecAssert(xmlSecPtrListCheckId(list, xmlSecKeyDataIdListId));
    xmlSecAssert(output != NULL);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        dataId = (xmlSecKeyDataId)xmlSecPtrListGetItem(list, i);
        xmlSecAssert(dataId != NULL);
        xmlSecAssert(dataId->name != NULL);

        if(i > 0) {
            fprintf(output, ",\"%s\"", dataId->name);
        } else {
            fprintf(output, "\"%s\"", dataId->name);
        }
    }
    fprintf(output, "\n");
}

/**
 * xmlSecKeyDataIdListDebugXmlDump:
 * @list:               the pointer to key data ids list.
 * @output:             the pointer to output FILE.
 *
 * Prints binary key data debug information to @output in XML format.
 */
void
xmlSecKeyDataIdListDebugXmlDump(xmlSecPtrListPtr list, FILE* output) {
    xmlSecKeyDataId dataId;
    xmlSecSize i, size;

    xmlSecAssert(xmlSecPtrListCheckId(list, xmlSecKeyDataIdListId));
    xmlSecAssert(output != NULL);

    fprintf(output, "<KeyDataIdsList>\n");
    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        dataId = (xmlSecKeyDataId)xmlSecPtrListGetItem(list, i);
        xmlSecAssert(dataId != NULL);
        xmlSecAssert(dataId->name != NULL);

        fprintf(output, "<DataId name=\"");
        xmlSecPrintXmlString(output, dataId->name);
        fprintf(output, "\"/>");
    }
    fprintf(output, "</KeyDataIdsList>\n");
}

/**************************************************************************
 *
 * xmlSecKeyDataStore functions
 *
 *************************************************************************/
/**
 * xmlSecKeyDataStoreCreate:
 * @id:                 the store id.
 *
 * Creates new key data store of the specified klass @id. Caller is responsible
 * for freeing returned object with #xmlSecKeyDataStoreDestroy function.
 *
 * Returns: the pointer to newly allocated key data store structure
 * or NULL if an error occurs.
 */
xmlSecKeyDataStorePtr
xmlSecKeyDataStoreCreate(xmlSecKeyDataStoreId id)  {
    xmlSecKeyDataStorePtr store;
    int ret;

    xmlSecAssert2(id != NULL, NULL);
    xmlSecAssert2(id->objSize > 0, NULL);

    /* Allocate a new xmlSecKeyDataStore and fill the fields. */
    store = (xmlSecKeyDataStorePtr)xmlMalloc(id->objSize);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", id->objSize);
        return(NULL);
    }
    memset(store, 0, id->objSize);
    store->id = id;

    if(id->initialize != NULL) {
        ret = (id->initialize)(store);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreKlassGetName(id)),
                        "id->initialize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyDataStoreDestroy(store);
            return(NULL);
        }
    }

    return(store);
}

/**
 * xmlSecKeyDataStoreDestroy:
 * @store:              the pointer to the key data store..
 *
 * Destroys the key data store created with #xmlSecKeyDataStoreCreate
 * function.
 */
void
xmlSecKeyDataStoreDestroy(xmlSecKeyDataStorePtr store) {
    xmlSecAssert(xmlSecKeyDataStoreIsValid(store));
    xmlSecAssert(store->id->objSize > 0);

    if(store->id->finalize != NULL) {
        (store->id->finalize)(store);
    }
    memset(store, 0, store->id->objSize);
    xmlFree(store);
}

/***********************************************************************
 *
 * Keys Data Store list
 *
 **********************************************************************/
static xmlSecPtrListKlass xmlSecKeyDataStorePtrListKlass = {
    BAD_CAST "keys-data-store-list",
    NULL,                                                       /* xmlSecPtrDuplicateItemMethod duplicateItem; */
    (xmlSecPtrDestroyItemMethod)xmlSecKeyDataStoreDestroy,      /* xmlSecPtrDestroyItemMethod destroyItem; */
    NULL,                                                       /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    NULL,                                                       /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};

/**
 * xmlSecKeyDataStorePtrListGetKlass:
 *
 * Key data stores list.
 *
 * Returns: key data stores list klass.
 */
xmlSecPtrListId
xmlSecKeyDataStorePtrListGetKlass(void) {
    return(&xmlSecKeyDataStorePtrListKlass);
}



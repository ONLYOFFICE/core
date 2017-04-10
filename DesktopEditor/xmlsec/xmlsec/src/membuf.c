/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Memory buffer transform
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
#include <xmlsec/buffer.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/keys.h>
#include <xmlsec/base64.h>
#include <xmlsec/membuf.h>
#include <xmlsec/errors.h>


/*****************************************************************************
 *
 * Memory Buffer Transform
 *
 * xmlSecBuffer is located after xmlSecTransform
 *
 ****************************************************************************/
#define xmlSecTransformMemBufSize \
        (sizeof(xmlSecTransform) + sizeof(xmlSecBuffer))
#define xmlSecTransformMemBufGetBuf(transform) \
    ((xmlSecTransformCheckSize((transform), xmlSecTransformMemBufSize)) ? \
        (xmlSecBufferPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)) : \
        (xmlSecBufferPtr)NULL)

static int              xmlSecTransformMemBufInitialize         (xmlSecTransformPtr transform);
static void             xmlSecTransformMemBufFinalize           (xmlSecTransformPtr transform);
static int              xmlSecTransformMemBufExecute            (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static xmlSecTransformKlass xmlSecTransformMemBufKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecTransformMemBufSize,                  /* xmlSecSize objSize */

    xmlSecNameMemBuf,                           /* const xmlChar* name; */
    NULL,                                       /* const xmlChar* href; */
    0,                                          /* xmlSecAlgorithmUsage usage; */

    xmlSecTransformMemBufInitialize,            /* xmlSecTransformInitializeMethod initialize; */
    xmlSecTransformMemBufFinalize,              /* xmlSecTransformFianlizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecTransformDefaultPushBin,              /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecTransformMemBufExecute,               /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecTransformMemBufGetKlass:
 *
 * The memory buffer transorm (used to store the data that go through it).
 *
 * Returns: memory buffer transform klass.
 */
xmlSecTransformId
xmlSecTransformMemBufGetKlass(void) {
    return(&xmlSecTransformMemBufKlass);
}

/**
 * xmlSecTransformMemBufGetBuffer:
 * @transform:          the pointer to memory buffer transform.
 *
 * Gets the pointer to memory buffer transform buffer.
 *
 * Returns: pointer to the transform's #xmlSecBuffer.
 */
xmlSecBufferPtr
xmlSecTransformMemBufGetBuffer(xmlSecTransformPtr transform) {
    xmlSecBufferPtr buffer;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformMemBufId), NULL);

    buffer = xmlSecTransformMemBufGetBuf(transform);
    xmlSecAssert2(buffer != NULL, NULL);

    return(buffer);
}

static int
xmlSecTransformMemBufInitialize(xmlSecTransformPtr transform) {
    xmlSecBufferPtr buffer;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformMemBufId), -1);

    buffer = xmlSecTransformMemBufGetBuf(transform);
    xmlSecAssert2(buffer != NULL, -1);

    ret = xmlSecBufferInitialize(buffer, 0);
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
xmlSecTransformMemBufFinalize(xmlSecTransformPtr transform) {
    xmlSecBufferPtr buffer;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecTransformMemBufId));

    buffer = xmlSecTransformMemBufGetBuf(transform);
    xmlSecAssert(buffer != NULL);

    xmlSecBufferFinalize(xmlSecTransformMemBufGetBuf(transform));
}

static int
xmlSecTransformMemBufExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecBufferPtr buffer;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformMemBufId), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    buffer = xmlSecTransformMemBufGetBuf(transform);
    xmlSecAssert2(buffer != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);
    inSize = xmlSecBufferGetSize(in);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if(transform->status == xmlSecTransformStatusWorking) {
        /* just copy everything from in to our buffer and out */
        ret = xmlSecBufferAppend(buffer, xmlSecBufferGetData(in), inSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferAppend",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", inSize);
            return(-1);
        }

        ret = xmlSecBufferAppend(out, xmlSecBufferGetData(in), inSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecBufferAppend",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "size=%d", inSize);
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

        if(last != 0) {
            transform->status = xmlSecTransformStatusFinished;
        }
    } else if(transform->status == xmlSecTransformStatusFinished) {
        /* the only way we can get here is if there is no input */
        xmlSecAssert2(inSize == 0, -1);
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


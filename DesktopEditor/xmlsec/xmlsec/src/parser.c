/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * XML Parser transform and utility functions.
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
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/parser.h>
#include <xmlsec/errors.h>

/**************************************************************************
 *
 * Internal parser
 *
 *****************************************************************************/
typedef struct _xmlSecParserCtx                                 xmlSecParserCtx,
                                                                *xmlSecParserCtxPtr;
struct _xmlSecParserCtx {
    xmlParserCtxtPtr    parserCtx;
};

/**************************************************************************
 *
 * XML Parser transform
 *
 * xmlSecParserCtx is located after xmlSecTransform
 *
 ***************************************************************************/
#define xmlSecParserSize        \
    (sizeof(xmlSecTransform) + sizeof(xmlSecParserCtx))
#define xmlSecParserGetCtx(transform) \
    ((xmlSecTransformCheckSize((transform), xmlSecParserSize)) ? \
        ((xmlSecParserCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform))) : \
        (xmlSecParserCtxPtr)NULL)

static int              xmlSecParserInitialize                  (xmlSecTransformPtr transform);
static void             xmlSecParserFinalize                    (xmlSecTransformPtr transform);
static int              xmlSecParserPushBin                     (xmlSecTransformPtr transform,
                                                                 const xmlSecByte* data,
                                                                 xmlSecSize dataSize,
                                                                 int final,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int              xmlSecParserPopXml                      (xmlSecTransformPtr transform,
                                                                 xmlSecNodeSetPtr* nodes,
                                                                 xmlSecTransformCtxPtr transformCtx);

static xmlSecTransformKlass xmlSecParserKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecParserSize,                           /* xmlSecSize objSize */

    BAD_CAST "xml-parser",                      /* const xmlChar* name; */
    NULL,                                       /* const xmlChar* href; */
    xmlSecTransformUsageDSigTransform,          /* xmlSecTransformUsage usage; */

    xmlSecParserInitialize,                     /* xmlSecTransformInitializeMethod initialize; */
    xmlSecParserFinalize,                       /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecParserPushBin,                /* xmlSecTransformPushBinMethod pushBin; */
    NULL,                                       /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    xmlSecParserPopXml,         /* xmlSecTransformPopXmlMethod popXml; */
    NULL,                                       /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecTransformXmlParserGetKlass:
 *
 * The XML parser transform.
 *
 * Returns: XML parser transform klass.
 */
xmlSecTransformId
xmlSecTransformXmlParserGetKlass(void) {
    return(&xmlSecParserKlass);
}

static int
xmlSecParserInitialize(xmlSecTransformPtr transform) {
    xmlSecParserCtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXmlParserId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecParserSize), -1);

    ctx = xmlSecParserGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* initialize context */
    memset(ctx, 0, sizeof(xmlSecParserCtx));
    return(0);
}

static void
xmlSecParserFinalize(xmlSecTransformPtr transform) {
    xmlSecParserCtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecTransformXmlParserId));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecParserSize));

    ctx = xmlSecParserGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->parserCtx != NULL) {
        xmlFreeParserCtxt(ctx->parserCtx);
    }
    memset(ctx, 0, sizeof(xmlSecParserCtx));
}

static int
xmlSecParserPushBin(xmlSecTransformPtr transform, const xmlSecByte* data,
                                xmlSecSize dataSize, int final, xmlSecTransformCtxPtr transformCtx) {
    xmlSecParserCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXmlParserId), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecParserGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* check/update current transform status */
    if(transform->status == xmlSecTransformStatusNone) {
        xmlSecAssert2(ctx->parserCtx == NULL, -1);

        ctx->parserCtx = xmlCreatePushParserCtxt(NULL, NULL, NULL, 0, NULL);
        if(ctx->parserCtx == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlCreatePushParserCtxt",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        /* required for c14n! */
        ctx->parserCtx->loadsubset      = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
        ctx->parserCtx->replaceEntities = 1;
        ctx->parserCtx->options         = XML_PARSE_NONET;

        transform->status = xmlSecTransformStatusWorking;
    } else if(transform->status == xmlSecTransformStatusFinished) {
        return(0);
    } else if(transform->status != xmlSecTransformStatusWorking) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_STATUS,
                    "status=%d", transform->status);
        return(-1);
    }
    xmlSecAssert2(transform->status == xmlSecTransformStatusWorking, -1);
    xmlSecAssert2(ctx->parserCtx != NULL, -1);

    /* push data to the input buffer */
    if((data != NULL) && (dataSize > 0)) {
        ret = xmlParseChunk(ctx->parserCtx, (const char*)data, dataSize, 0);
        if(ret != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlParseChunk",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "size=%d", dataSize);
            return(-1);
        }
    }

    /* finish parsing and push to next in the chain */
    if(final != 0) {
        ret = xmlParseChunk(ctx->parserCtx, NULL, 0, 1);
        if((ret != 0) || (ctx->parserCtx->myDoc == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlParseChunk",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

        /* todo: check that document is well formed? */
        transform->outNodes = xmlSecNodeSetCreate(ctx->parserCtx->myDoc,
                                                  NULL, xmlSecNodeSetTree);
        if(transform->outNodes == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecNodeSetCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlFreeDoc(ctx->parserCtx->myDoc);
            ctx->parserCtx->myDoc = NULL;
            return(-1);
        }
        xmlSecNodeSetDocDestroy(transform->outNodes); /* this node set "owns" the doc pointer */
        ctx->parserCtx->myDoc = NULL;

        /* push result to the next transform (if exist) */
        if(transform->next != NULL) {
            ret = xmlSecTransformPushXml(transform->next, transform->outNodes, transformCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecTransformPushXml",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
        }

        transform->status = xmlSecTransformStatusFinished;
    }

    return(0);
}

static int
xmlSecParserPopXml(xmlSecTransformPtr transform, xmlSecNodeSetPtr* nodes,
                               xmlSecTransformCtxPtr transformCtx) {
    xmlSecParserCtxPtr ctx;
    xmlParserInputBufferPtr buf;
    xmlParserInputPtr input;
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXmlParserId), -1);
    xmlSecAssert2(nodes != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecParserGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* check/update current transform status */
    switch(transform->status) {
    case xmlSecTransformStatusNone:
        transform->status = xmlSecTransformStatusWorking;
        break;
    case xmlSecTransformStatusWorking:
        /* just do nothing */
        break;
    case xmlSecTransformStatusFinished:
        (*nodes) = NULL;
        return(0);
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_STATUS,
                    "status=%d", transform->status);
        return(-1);
    }
    xmlSecAssert2(transform->status == xmlSecTransformStatusWorking, -1);

    /* prepare parser context */
    if(transform->prev == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TRANSFORM,
                    "prev transform is null");
        return(-1);
    }

    buf = xmlSecTransformCreateInputBuffer(transform->prev, transformCtx);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecTransformCreateInputBuffer",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlNewParserCtxt",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFreeParserInputBuffer(buf);
        return(-1);
    }

    input = xmlNewIOInputStream(ctxt, buf, XML_CHAR_ENCODING_NONE);
    if(input == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlNewParserCtxt",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFreeParserCtxt(ctxt);
        xmlFreeParserInputBuffer(buf);
        return(-1);
    }

    ret = inputPush(ctxt, input);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "inputPush",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFreeInputStream(input);
        xmlFreeParserCtxt(ctxt);
        return(-1);
    }

    /* required for c14n! */
    ctxt->loadsubset = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    ctxt->replaceEntities = 1;

    /* finaly do the parsing */
    ret = xmlParseDocument(ctxt);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlParseDocument",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        if(ctxt->myDoc != NULL) {
            xmlFreeDoc(ctxt->myDoc);
            ctxt->myDoc = NULL;
        }
        xmlFreeParserCtxt(ctxt);
        return(-1);
    }

    /* remember the result and free parsing context */
    doc = ctxt->myDoc;
    ctxt->myDoc = NULL;
    xmlFreeParserCtxt(ctxt);

    /* return result to the caller */
    (*nodes) = xmlSecNodeSetCreate(doc, NULL, xmlSecNodeSetTree);
    if((*nodes) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecNodeSetCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFreeDoc(doc);
        return(-1);
    }
    xmlSecNodeSetDocDestroy((*nodes)); /* this node set "owns" the doc pointer */
    transform->status = xmlSecTransformStatusFinished;
    return(0);
}

/**************************************************************************
 *
 * XML Parser functions
 *
 *************************************************************************/
typedef struct _xmlSecExtMemoryParserCtx {
    const xmlSecByte    *prefix;
    xmlSecSize                  prefixSize;
    const xmlSecByte    *buffer;
    xmlSecSize                  bufferSize;
    const xmlSecByte    *postfix;
    xmlSecSize                  postfixSize;
} xmlSecExtMemoryParserCtx, *xmlSecExtMemoryParserCtxPtr;

/**
 * xmlSecParseFile:
 * @filename:           the filename.
 *
 * Loads XML Doc from file @filename. We need a special version because of
 * c14n issue. The code is copied from xmlSAXParseFileWithData() function.
 *
 * Returns: pointer to the loaded XML document or NULL if an error occurs.
 */
xmlDocPtr
xmlSecParseFile(const char *filename) {
    xmlDocPtr ret;
    xmlParserCtxtPtr ctxt;
    char *directory = NULL;

    xmlSecAssert2(filename != NULL, NULL);

    xmlInitParser();
    ctxt = xmlCreateFileParserCtxt(filename);
    if (ctxt == NULL) {
        return(NULL);
    }

    /* enable parsing of XML documents with large text nodes */
    /* crashes on x64 xmlCtxtUseOptions (ctxt, XML_PARSE_HUGE); */

    /* todo: set directories from current doc? */
    if ((ctxt->directory == NULL) && (directory == NULL))
        directory = xmlParserGetDirectory(filename);
    if ((ctxt->directory == NULL) && (directory != NULL))
        ctxt->directory = (char *) xmlStrdup((xmlChar *) directory);

    /* required for c14n! */
    ctxt->loadsubset = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    ctxt->replaceEntities = 1;

    xmlParseDocument(ctxt);

    if(ctxt->wellFormed) {
        ret = ctxt->myDoc;
    } else {
       ret = NULL;
       xmlFreeDoc(ctxt->myDoc);
       ctxt->myDoc = NULL;
    }
    xmlFreeParserCtxt(ctxt);
    return(ret);

}

/**
 * xmlSecParseMemoryExt:
 * @prefix:             the first part of the input.
 * @prefixSize:         the size of the first part of the input.
 * @buffer:             the second part of the input.
 * @bufferSize:         the size of the second part of the input.
 * @postfix:            the third part of the input.
 * @postfixSize:        the size of the third part of the input.
 *
 * Loads XML Doc from 3 chunks of memory: @prefix, @buffer and @postfix.
 *
 * Returns: pointer to the loaded XML document or NULL if an error occurs.
 */
xmlDocPtr
xmlSecParseMemoryExt(const xmlSecByte *prefix, xmlSecSize prefixSize,
                     const xmlSecByte *buffer, xmlSecSize bufferSize,
                     const xmlSecByte *postfix, xmlSecSize postfixSize) {
    xmlParserCtxtPtr ctxt = NULL;
    xmlDocPtr doc = NULL;
    int ret;

    /* create context */
    ctxt = xmlCreatePushParserCtxt(NULL, NULL, NULL, 0, NULL);
    if(ctxt == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlCreatePushParserCtxt",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* required for c14n! */
    ctxt->loadsubset = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    ctxt->replaceEntities = 1;

    /* prefix */
    if((prefix != NULL) && (prefixSize > 0)) {
        ret = xmlParseChunk(ctxt, (const char*)prefix, prefixSize, 0);
        if(ret != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlParseChunk",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "prefixSize=%d", prefixSize);
            goto done;
        }
    }

    /* buffer */
    if((buffer != NULL) && (bufferSize > 0)) {
        ret = xmlParseChunk(ctxt, (const char*)buffer, bufferSize, 0);
        if(ret != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlParseChunk",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "bufferSize=%d", bufferSize);
            goto done;
        }
    }

    /* postfix */
    if((postfix != NULL) && (postfixSize > 0)) {
        ret = xmlParseChunk(ctxt, (const char*)postfix, postfixSize, 0);
        if(ret != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlParseChunk",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "postfixSize=%d", postfixSize);
            goto done;
        }
    }

    /* finishing */
    ret = xmlParseChunk(ctxt, NULL, 0, 1);
    if((ret != 0) || (ctxt->myDoc == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlParseChunk",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    doc = ctxt->myDoc;

done:
    if(ctxt != NULL) {
        xmlFreeParserCtxt(ctxt);
    }
    return(doc);
}


/**
 * xmlSecParseMemory:
 * @buffer:             the input buffer.
 * @size:               the input buffer size.
 * @recovery:           the flag.
 *
 * Loads XML Doc from memory. We need a special version because of
 * c14n issue. The code is copied from xmlSAXParseMemory() function.
 *
 * Returns: pointer to the loaded XML document or NULL if an error occurs.
 */
xmlDocPtr
xmlSecParseMemory(const xmlSecByte *buffer, xmlSecSize size, int recovery) {
    xmlDocPtr ret;
    xmlParserCtxtPtr ctxt;

    xmlSecAssert2(buffer != NULL, NULL);

    ctxt = xmlCreateMemoryParserCtxt((char*)buffer, size);
    if (ctxt == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlCreateMemoryParserCtxt",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    /* required for c14n! */
    ctxt->loadsubset = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    ctxt->replaceEntities = 1;

    xmlParseDocument(ctxt);

    if((ctxt->wellFormed) || recovery) {
        ret = ctxt->myDoc;
    } else {
       ret = NULL;
       xmlFreeDoc(ctxt->myDoc);
       ctxt->myDoc = NULL;
    }
    xmlFreeParserCtxt(ctxt);
    return(ret);
}


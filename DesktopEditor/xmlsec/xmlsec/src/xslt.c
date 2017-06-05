/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * XSLT Transform (http://www.w3.org/TR/xmldsig-core/#sec-XSLT)
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_XSLT

#include <stdlib.h>
#include <string.h>

#include <libxml/tree.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/keys.h>
#include <xmlsec/parser.h>
#include <xmlsec/errors.h>
#include <xmlsec/private/xslt.h>

/**************************************************************************
 *
 * Internal xslt ctx
 *
 *****************************************************************************/
typedef struct _xmlSecXsltCtx                   xmlSecXsltCtx, *xmlSecXsltCtxPtr;
struct _xmlSecXsltCtx {
    xsltStylesheetPtr   xslt;
    xmlParserCtxtPtr    parserCtx;
};

/****************************************************************************
 *
 * XSLT transform
 *
 * xmlSecXsltCtx is located after xmlSecTransform
 *
 ***************************************************************************/
#define xmlSecXsltSize  \
    (sizeof(xmlSecTransform) + sizeof(xmlSecXsltCtx))
#define xmlSecXsltGetCtx(transform) \
    ((xmlSecXsltCtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)))

static int              xmlSecXsltInitialize                    (xmlSecTransformPtr transform);
static void             xmlSecXsltFinalize                      (xmlSecTransformPtr transform);
static int              xmlSecXsltReadNode                      (xmlSecTransformPtr transform,
                                                                 xmlNodePtr node,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int              xmlSecXsltPushBin                       (xmlSecTransformPtr transform,
                                                                 const xmlSecByte* data,
                                                                 xmlSecSize dataSize,
                                                                 int final,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int              xmlSecXsltExecute                       (xmlSecTransformPtr transform,
                                                                 int last,
                                                                 xmlSecTransformCtxPtr transformCtx);
static int              xmlSecXslProcess                        (xmlSecXsltCtxPtr ctx,
                                                                 xmlSecBufferPtr in,
                                                                 xmlSecBufferPtr out);
static xmlDocPtr        xmlSecXsApplyStylesheet                 (xmlSecXsltCtxPtr ctx,
                                                                 xmlDocPtr doc);
                                                                 
static xmlSecTransformKlass xmlSecXsltKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecXsltSize,                             /* xmlSecSize objSize */

    xmlSecNameXslt,                             /* const xmlChar* name; */
    xmlSecHrefXslt,                             /* const xmlChar* href; */
    xmlSecTransformUsageDSigTransform,          /* xmlSecAlgorithmUsage usage; */

    xmlSecXsltInitialize,                       /* xmlSecTransformInitializeMethod initialize; */
    xmlSecXsltFinalize,                         /* xmlSecTransformFinalizeMethod finalize; */
    xmlSecXsltReadNode,                         /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    xmlSecXsltPushBin,                          /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformDefaultPopBin,               /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    xmlSecXsltExecute,                          /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};


#define XMLSEC_XSLT_COPY_SEC_PREF(src, dst, pref) \
    xsltSetSecurityPrefs((dst), (pref),  xsltGetSecurityPrefs((src), (pref)))

static xsltSecurityPrefsPtr g_xslt_default_security_prefs = NULL;

void xmlSecTransformXsltInitialize(void) {
    xmlSecAssert(g_xslt_default_security_prefs == NULL);

    g_xslt_default_security_prefs = xsltNewSecurityPrefs();
    xmlSecAssert(g_xslt_default_security_prefs != NULL);
    xsltSetSecurityPrefs(g_xslt_default_security_prefs,  XSLT_SECPREF_READ_FILE,        xsltSecurityForbid);
    xsltSetSecurityPrefs(g_xslt_default_security_prefs,  XSLT_SECPREF_WRITE_FILE,       xsltSecurityForbid);
    xsltSetSecurityPrefs(g_xslt_default_security_prefs,  XSLT_SECPREF_CREATE_DIRECTORY, xsltSecurityForbid);
    xsltSetSecurityPrefs(g_xslt_default_security_prefs,  XSLT_SECPREF_READ_NETWORK,     xsltSecurityForbid);
    xsltSetSecurityPrefs(g_xslt_default_security_prefs,  XSLT_SECPREF_WRITE_NETWORK,    xsltSecurityForbid);
}

void xmlSecTransformXsltShutdown(void) {
    if(g_xslt_default_security_prefs != NULL) {
        xsltFreeSecurityPrefs(g_xslt_default_security_prefs);
        g_xslt_default_security_prefs = NULL;
    }
}

/**
 * xmlSecTransformXsltSetDefaultSecurityPrefs:
 * @sec: the new security preferences
 *
 * Sets the new default security preferences. The xmlsec default security policy is 
 * to disable everything.
 */
XMLSEC_EXPORT void
xmlSecTransformXsltSetDefaultSecurityPrefs(xsltSecurityPrefsPtr sec) {
    xmlSecAssert(sec != NULL);
    xmlSecAssert(g_xslt_default_security_prefs != NULL);
    
    /* copy prefs */
    XMLSEC_XSLT_COPY_SEC_PREF(sec, g_xslt_default_security_prefs, XSLT_SECPREF_READ_FILE);
    XMLSEC_XSLT_COPY_SEC_PREF(sec, g_xslt_default_security_prefs, XSLT_SECPREF_WRITE_FILE);
    XMLSEC_XSLT_COPY_SEC_PREF(sec, g_xslt_default_security_prefs, XSLT_SECPREF_CREATE_DIRECTORY);
    XMLSEC_XSLT_COPY_SEC_PREF(sec, g_xslt_default_security_prefs, XSLT_SECPREF_READ_NETWORK);
    XMLSEC_XSLT_COPY_SEC_PREF(sec, g_xslt_default_security_prefs, XSLT_SECPREF_WRITE_NETWORK);
}

/**
 * xmlSecTransformXsltGetKlass:
 *
 * XSLT transform klass (http://www.w3.org/TR/xmldsig-core/#sec-XSLT):
 *
 * The normative specification for XSL Transformations is [XSLT].
 * Specification of a namespace-qualified stylesheet element, which MUST be
 * the sole child of the Transform element, indicates that the specified style
 * sheet should be used. Whether this instantiates in-line processing of local
 * XSLT declarations within the resource is determined by the XSLT processing
 * model; the ordered application of multiple stylesheet may require multiple
 * Transforms. No special provision is made for the identification of a remote
 * stylesheet at a given URI because it can be communicated via an  xsl:include
 * or  xsl:import within the stylesheet child of the Transform.
 *
 * This transform requires an octet stream as input. If the actual input is an
 * XPath node-set, then the signature application should attempt to convert it
 * to octets (apply Canonical XML]) as described in the Reference Processing
 * Model (section 4.3.3.2).]
 *
 * The output of this transform is an octet stream. The processing rules for
 * the XSL style sheet or transform element are stated in the XSLT specification
 * [XSLT]. We RECOMMEND that XSLT transform authors use an output method of xml
 * for XML and HTML. As XSLT implementations do not produce consistent
 * serializations of their output, we further RECOMMEND inserting a transform
 * after the XSLT transform to canonicalize the output. These steps will help
 * to ensure interoperability of the resulting signatures among applications
 * that support the XSLT transform. Note that if the output is actually HTML,
 * then the result of these steps is logically equivalent [XHTML].
 *
 * Returns: pointer to XSLT transform klass.
 */
xmlSecTransformId
xmlSecTransformXsltGetKlass(void) {
    return(&xmlSecXsltKlass);
}

static int
xmlSecXsltInitialize(xmlSecTransformPtr transform) {
    xmlSecXsltCtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXsltId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecXsltSize), -1);

    ctx = xmlSecXsltGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* initialize context */
    memset(ctx, 0, sizeof(xmlSecXsltCtx));
    
    /* done */
    return(0);
}

static void
xmlSecXsltFinalize(xmlSecTransformPtr transform) {
    xmlSecXsltCtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecTransformXsltId));
    xmlSecAssert(xmlSecTransformCheckSize(transform, xmlSecXsltSize));

    ctx = xmlSecXsltGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    if(ctx->xslt != NULL) {
        xsltFreeStylesheet(ctx->xslt);
    }
    if(ctx->parserCtx != NULL) {
        xmlFreeParserCtxt(ctx->parserCtx);
    }
    memset(ctx, 0, sizeof(xmlSecXsltCtx));
}

static int
xmlSecXsltReadNode(xmlSecTransformPtr transform, xmlNodePtr node, xmlSecTransformCtxPtr transformCtx) {
    xmlSecXsltCtxPtr ctx;
    xmlBufferPtr buffer;
    xmlDocPtr doc;
    xmlNodePtr cur;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXsltId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecXsltSize), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecXsltGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->xslt == NULL, -1);

    /* read content in the buffer */
    buffer = xmlBufferCreate();
    if(buffer == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlBufferCreate",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    cur = node->children;
    while(cur != NULL) {
        xmlNodeDump(buffer, cur->doc, cur, 0, 0);
        cur = cur->next;
    }

    /* parse the buffer */
    doc = xmlSecParseMemory(xmlBufferContent(buffer),
                             xmlBufferLength(buffer), 1);
    if(doc == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecParseMemory",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlBufferFree(buffer);
        return(-1);
    }

    /* pre-process stylesheet */
    ctx->xslt = xsltParseStylesheetDoc(doc);
    if(ctx->xslt == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xsltParseStylesheetDoc",
                    XMLSEC_ERRORS_R_XSLT_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        /* after parsing stylesheet doc is assigned
         * to it and will be freed by xsltFreeStylesheet() */
        xmlFreeDoc(doc);
        xmlBufferFree(buffer);
        return(-1);
    }

    xmlBufferFree(buffer);
    return(0);
}

static int
xmlSecXsltPushBin(xmlSecTransformPtr transform, const xmlSecByte* data,
                                xmlSecSize dataSize, int final, xmlSecTransformCtxPtr transformCtx) {
    xmlSecXsltCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXsltId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecXsltSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecXsltGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->xslt != NULL, -1);

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
        ctx->parserCtx->loadsubset = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
        ctx->parserCtx->replaceEntities = 1;

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

    /* finish parsing, apply xslt transforms and push to next in the chain */
    if(final != 0) {
        xmlDocPtr docIn;
        xmlDocPtr docOut;
        xmlOutputBufferPtr output;

        /* finalize */
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
        docIn = ctx->parserCtx->myDoc;
        ctx->parserCtx->myDoc = NULL;

        docOut = xmlSecXsApplyStylesheet(ctx, docIn);
        if(docOut == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecXsApplyStylesheet",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlFreeDoc(docIn);
            return(-1);
        }
        xmlFreeDoc(docIn);

        if(transform->next != NULL) {
            output = xmlSecTransformCreateOutputBuffer(transform->next, transformCtx);
            if(output == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecTransformCreateOutputBuffer",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlFreeDoc(docOut);
                return(-1);
            }
        } else {
            output = xmlSecBufferCreateOutputBuffer(&(transform->outBuf));
            if(output == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                            "xmlSecBufferCreateOutputBuffer",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlFreeDoc(docOut);
                return(-1);
            }
        }

        ret = xsltSaveResultTo(output, docOut, ctx->xslt);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xsltSaveResultTo",
                        XMLSEC_ERRORS_R_XSLT_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlOutputBufferClose(output);
            xmlFreeDoc(docOut);
            return(-1);
        }
        ret = xmlOutputBufferClose(output);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlOutputBufferClose",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlFreeDoc(docOut);
            return(-1);
        }
        xmlFreeDoc(docOut);

        transform->status = xmlSecTransformStatusFinished;
    }

    return(0);
}

static int
xmlSecXsltExecute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecXsltCtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformXsltId), -1);
    xmlSecAssert2(xmlSecTransformCheckSize(transform, xmlSecXsltSize), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecXsltGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->xslt != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);
    inSize = xmlSecBufferGetSize(in);
    outSize = xmlSecBufferGetSize(out);

    if(transform->status == xmlSecTransformStatusNone) {
        transform->status = xmlSecTransformStatusWorking;
    }

    if((transform->status == xmlSecTransformStatusWorking) && (last == 0)) {
        /* just do nothing */
        xmlSecAssert2(outSize == 0, -1);

    } else  if((transform->status == xmlSecTransformStatusWorking) && (last != 0)) {
        xmlSecAssert2(outSize == 0, -1);

        ret = xmlSecXslProcess(ctx, in, out);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "xmlSecXslProcess",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
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

        transform->status = xmlSecTransformStatusFinished;
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

/* TODO: create PopBin method instead */
static int
xmlSecXslProcess(xmlSecXsltCtxPtr ctx, xmlSecBufferPtr in, xmlSecBufferPtr out) {
    xmlDocPtr docIn = NULL;
    xmlDocPtr docOut = NULL;
    xmlOutputBufferPtr output = NULL;
    int res = -1;
    int ret;

    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(ctx != NULL, -1);

    docIn = xmlSecParseMemory(xmlSecBufferGetData(in), xmlSecBufferGetSize(in), 1);
    if(docIn == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecParseMemory",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    docOut = xmlSecXsApplyStylesheet(ctx, docIn);
    if(docOut == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecXsApplyStylesheet",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    output = xmlSecBufferCreateOutputBuffer(out);
    if(output == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferCreateOutputBuffer",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xsltSaveResultTo(output, docOut, ctx->xslt);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xsltSaveResultTo",
                    XMLSEC_ERRORS_R_XSLT_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xmlOutputBufferClose(output);
    output = NULL;
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlOutputBufferClose",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    res = 0;

done:
    if(output != NULL) xmlOutputBufferClose(output);
    if(docIn != NULL) xmlFreeDoc(docIn);
    if(docOut != NULL) xmlFreeDoc(docOut);
    return(res);
}


static xmlDocPtr
xmlSecXsApplyStylesheet(xmlSecXsltCtxPtr ctx, xmlDocPtr doc) {
    xsltTransformContextPtr xsltCtx = NULL;
    xmlDocPtr res = NULL;
    int ret;
    
    xmlSecAssert2(ctx != NULL, NULL);
    xmlSecAssert2(ctx->xslt != NULL, NULL);
    xmlSecAssert2(doc != NULL, NULL);

    xsltCtx = xsltNewTransformContext(ctx->xslt, doc);
    if(xsltCtx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xsltNewTransformContext",
                    XMLSEC_ERRORS_R_XSLT_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* set security prefs  */
    ret = xsltSetCtxtSecurityPrefs(g_xslt_default_security_prefs, xsltCtx);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xsltSetCtxtSecurityPrefs",
                    XMLSEC_ERRORS_R_XSLT_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    res = xsltApplyStylesheetUser(ctx->xslt, doc, NULL, NULL, NULL, xsltCtx);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xsltApplyStylesheetUser",
                    XMLSEC_ERRORS_R_XSLT_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    
done:
    if(xsltCtx != NULL) xsltFreeTransformContext(xsltCtx);
    return res;    
}


#endif /* XMLSEC_NO_XSLT */


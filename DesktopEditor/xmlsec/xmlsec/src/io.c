/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Input uri transform and utility functions.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <libxml/uri.h>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>

#ifdef LIBXML_HTTP_ENABLED
#include <libxml/nanohttp.h>
#endif /* LIBXML_HTTP_ENABLED */

#ifdef LIBXML_FTP_ENABLED
#include <libxml/nanoftp.h>
#endif /* LIBXML_FTP_ENABLED */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/keys.h>
#include <xmlsec/io.h>
#include <xmlsec/errors.h>

/*******************************************************************
 *
 * Input I/O callback sets
 *
 ******************************************************************/
typedef struct _xmlSecIOCallback {
    xmlInputMatchCallback matchcallback;
    xmlInputOpenCallback opencallback;
    xmlInputReadCallback readcallback;
    xmlInputCloseCallback closecallback;
} xmlSecIOCallback, *xmlSecIOCallbackPtr;

static xmlSecIOCallbackPtr      xmlSecIOCallbackCreate  (xmlInputMatchCallback matchFunc,
                                                         xmlInputOpenCallback openFunc,
                                                         xmlInputReadCallback readFunc,
                                                         xmlInputCloseCallback closeFunc);
static void                     xmlSecIOCallbackDestroy (xmlSecIOCallbackPtr callbacks);

static xmlSecIOCallbackPtr
xmlSecIOCallbackCreate(xmlInputMatchCallback matchFunc, xmlInputOpenCallback openFunc,
                       xmlInputReadCallback readFunc, xmlInputCloseCallback closeFunc) {
    xmlSecIOCallbackPtr callbacks;

    xmlSecAssert2(matchFunc != NULL, NULL);

    /* Allocate a new xmlSecIOCallback and fill the fields. */
    callbacks = (xmlSecIOCallbackPtr)xmlMalloc(sizeof(xmlSecIOCallback));
    if(callbacks == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "sizeof(xmlSecIOCallback)=%d",
                    (int)sizeof(xmlSecIOCallback));
        return(NULL);
    }
    memset(callbacks, 0, sizeof(xmlSecIOCallback));

    callbacks->matchcallback = matchFunc;
    callbacks->opencallback  = openFunc;
    callbacks->readcallback  = readFunc;
    callbacks->closecallback = closeFunc;

    return(callbacks);
}

static void
xmlSecIOCallbackDestroy(xmlSecIOCallbackPtr callbacks) {
    xmlSecAssert(callbacks != NULL);

    memset(callbacks, 0, sizeof(xmlSecIOCallback));
    xmlFree(callbacks);
}

/*******************************************************************
 *
 * Input I/O callback list
 *
 ******************************************************************/
static xmlSecPtrListKlass xmlSecIOCallbackPtrListKlass = {
    BAD_CAST "io-callbacks-list",
    NULL,                                               /* xmlSecPtrDuplicateItemMethod duplicateItem; */
    (xmlSecPtrDestroyItemMethod)xmlSecIOCallbackDestroy,/* xmlSecPtrDestroyItemMethod destroyItem; */
    NULL,                                               /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    NULL                                                /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};

#define xmlSecIOCallbackPtrListId       xmlSecIOCallbackPtrListGetKlass ()
static xmlSecPtrListId                  xmlSecIOCallbackPtrListGetKlass (void);
static xmlSecIOCallbackPtr              xmlSecIOCallbackPtrListFind     (xmlSecPtrListPtr list,
                                                                         const char* uri);

/**
 * xmlSecIOCallbackPtrListGetKlass:
 *
 * The keys list klass.
 *
 * Returns: keys list id.
 */
static xmlSecPtrListId
xmlSecIOCallbackPtrListGetKlass(void) {
    return(&xmlSecIOCallbackPtrListKlass);
}

static xmlSecIOCallbackPtr
xmlSecIOCallbackPtrListFind(xmlSecPtrListPtr list, const char* uri) {
    xmlSecIOCallbackPtr callbacks;
    xmlSecSize i, size;

    xmlSecAssert2(xmlSecPtrListCheckId(list, xmlSecIOCallbackPtrListId), NULL);
    xmlSecAssert2(uri != NULL, NULL);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        callbacks = (xmlSecIOCallbackPtr)xmlSecPtrListGetItem(list, i);
        xmlSecAssert2(callbacks != NULL, NULL);
        xmlSecAssert2(callbacks->matchcallback != NULL, NULL);

        if((callbacks->matchcallback(uri)) != 0) {
            return(callbacks);
        }
    }
    return(NULL);
}

static xmlSecPtrList xmlSecAllIOCallbacks;

/**
 * xmlSecIOInit:
 *
 * The IO initialization (called from #xmlSecInit function).
 * Applications should not call this function directly.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecIOInit(void) {
    int ret;

    ret = xmlSecPtrListInitialize(&xmlSecAllIOCallbacks, xmlSecIOCallbackPtrListId);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListPtrInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

#ifdef LIBXML_HTTP_ENABLED
    xmlNanoHTTPInit();
#endif /* LIBXML_HTTP_ENABLED */

#ifdef LIBXML_FTP_ENABLED
    xmlNanoFTPInit();
#endif /* LIBXML_FTP_ENABLED */

    return(xmlSecIORegisterDefaultCallbacks());
}

/**
 * xmlSecIOShutdown:
 *
 * The IO clenaup (called from #xmlSecShutdown function).
 * Applications should not call this function directly.
 */
void
xmlSecIOShutdown(void) {

#ifdef LIBXML_HTTP_ENABLED
    xmlNanoHTTPCleanup();
#endif /* LIBXML_HTTP_ENABLED */

#ifdef LIBXML_FTP_ENABLED
    xmlNanoFTPCleanup();
#endif /* LIBXML_FTP_ENABLED */

    xmlSecPtrListFinalize(&xmlSecAllIOCallbacks);
}

/**
 * xmlSecIOCleanupCallbacks:
 *
 * Clears the entire input callback table. this includes the
 * compiled-in I/O.
 */
void
xmlSecIOCleanupCallbacks(void) {
    xmlSecPtrListEmpty(&xmlSecAllIOCallbacks);
}

/**
 * xmlSecIORegisterCallbacks:
 * @matchFunc:          the protocol match callback.
 * @openFunc:           the open stream callback.
 * @readFunc:           the read from stream callback.
 * @closeFunc:          the close stream callback.
 *
 * Register a new set of I/O callback for handling parser input.
 *
 * Returns: the 0 on success or a negative value if an error occurs.
 */
int
xmlSecIORegisterCallbacks(xmlInputMatchCallback matchFunc,
        xmlInputOpenCallback openFunc, xmlInputReadCallback readFunc,
        xmlInputCloseCallback closeFunc) {
    xmlSecIOCallbackPtr callbacks;
    int ret;

    xmlSecAssert2(matchFunc != NULL, -1);

    callbacks = xmlSecIOCallbackCreate(matchFunc, openFunc, readFunc, closeFunc);
    if(callbacks == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecIOCallbackCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecPtrListAdd(&xmlSecAllIOCallbacks, callbacks);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListAdd",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecIOCallbackDestroy(callbacks);
        return(-1);
    }
    return(0);
}


/**
 * xmlSecIORegisterDefaultCallbacks:
 *
 * Registers the default compiled-in I/O handlers.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecIORegisterDefaultCallbacks(void) {
    int ret;

#ifdef LIBXML_HTTP_ENABLED
    ret = xmlSecIORegisterCallbacks(xmlIOHTTPMatch, xmlIOHTTPOpen,
                              xmlIOHTTPRead, xmlIOHTTPClose);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecIORegisterCallbacks",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "http");
        return(-1);
    }
#endif /* LIBXML_HTTP_ENABLED */

#ifdef LIBXML_FTP_ENABLED
    ret = xmlSecIORegisterCallbacks(xmlIOFTPMatch, xmlIOFTPOpen,
                              xmlIOFTPRead, xmlIOFTPClose);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecIORegisterCallbacks",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "ftp");
        return(-1);
    }
#endif /* LIBXML_FTP_ENABLED */

    ret = xmlSecIORegisterCallbacks(xmlFileMatch, xmlFileOpen,
                              xmlFileRead, xmlFileClose);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecIORegisterCallbacks",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "file");
        return(-1);
    }

    return(0);
}




/**************************************************************
 *
 * Input URI Transform
 *
 * xmlSecInputURICtx is located after xmlSecTransform
 *
 **************************************************************/
typedef struct _xmlSecInputURICtx                               xmlSecInputURICtx,
                                                                *xmlSecInputURICtxPtr;
struct _xmlSecInputURICtx {
    xmlSecIOCallbackPtr         clbks;
    void*                       clbksCtx;
};
#define xmlSecTransformInputUriSize \
        (sizeof(xmlSecTransform) + sizeof(xmlSecInputURICtx))
#define xmlSecTransformInputUriGetCtx(transform) \
    ((xmlSecTransformCheckSize((transform), xmlSecTransformInputUriSize)) ? \
        (xmlSecInputURICtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)) : \
        (xmlSecInputURICtxPtr)NULL)

static int              xmlSecTransformInputURIInitialize       (xmlSecTransformPtr transform);
static void             xmlSecTransformInputURIFinalize         (xmlSecTransformPtr transform);
static int              xmlSecTransformInputURIPopBin           (xmlSecTransformPtr transform,
                                                                 xmlSecByte* data,
                                                                 xmlSecSize maxDataSize,
                                                                 xmlSecSize* dataSize,
                                                                 xmlSecTransformCtxPtr transformCtx);

static xmlSecTransformKlass xmlSecTransformInputURIKlass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecTransformInputUriSize,                /* xmlSecSize objSize */

    BAD_CAST "input-uri",                       /* const xmlChar* name; */
    NULL,                                       /* const xmlChar* href; */
    0,                                          /* xmlSecAlgorithmUsage usage; */

    xmlSecTransformInputURIInitialize,          /* xmlSecTransformInitializeMethod initialize; */
    xmlSecTransformInputURIFinalize,            /* xmlSecTransformFinalizeMethod finalize; */
    NULL,                                       /* xmlSecTransformNodeReadMethod readNode; */
    NULL,                                       /* xmlSecTransformNodeWriteMethod writeNode; */
    NULL,                                       /* xmlSecTransformSetKeyReqMethod setKeyReq; */
    NULL,                                       /* xmlSecTransformSetKeyMethod setKey; */
    NULL,                                       /* xmlSecTransformValidateMethod validate; */
    xmlSecTransformDefaultGetDataType,          /* xmlSecTransformGetDataTypeMethod getDataType; */
    NULL,                                       /* xmlSecTransformPushBinMethod pushBin; */
    xmlSecTransformInputURIPopBin,              /* xmlSecTransformPopBinMethod popBin; */
    NULL,                                       /* xmlSecTransformPushXmlMethod pushXml; */
    NULL,                                       /* xmlSecTransformPopXmlMethod popXml; */
    NULL,                                       /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecTransformInputURIGetKlass:
 *
 * The input uri transform klass. Reads binary data from an uri.
 *
 * Returns: input URI transform id.
 */
xmlSecTransformId
xmlSecTransformInputURIGetKlass(void) {
    return(&xmlSecTransformInputURIKlass);
}

/**
 * xmlSecTransformInputURIOpen:
 * @transform:          the pointer to IO transform.
 * @uri:                the URL to open.
 *
 * Opens the given @uri for reading.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecTransformInputURIOpen(xmlSecTransformPtr transform, const xmlChar *uri) {
    xmlSecInputURICtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformInputURIId), -1);
    xmlSecAssert2(uri != NULL, -1);

    ctx = xmlSecTransformInputUriGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->clbks == NULL, -1);
    xmlSecAssert2(ctx->clbksCtx == NULL, -1);

    /*
     * Try to find one of the input accept method accepting that scheme
     * Go in reverse to give precedence to user defined handlers.
     * try with an unescaped version of the uri
     */
    if(ctx->clbks == NULL) {
        char *unescaped;

        unescaped = xmlURIUnescapeString((char*)uri, 0, NULL);
        if (unescaped != NULL) {
            ctx->clbks = xmlSecIOCallbackPtrListFind(&xmlSecAllIOCallbacks, unescaped);
            if(ctx->clbks != NULL) {
                ctx->clbksCtx = ctx->clbks->opencallback(unescaped);
            }
            xmlFree(unescaped);
        }
    }

    /*
     * If this failed try with a non-escaped uri this may be a strange
     * filename
     */
    if (ctx->clbks == NULL) {
        ctx->clbks = xmlSecIOCallbackPtrListFind(&xmlSecAllIOCallbacks, (char*)uri);
        if(ctx->clbks != NULL) {
            ctx->clbksCtx = ctx->clbks->opencallback((char*)uri);
        }
    }

    if((ctx->clbks == NULL) || (ctx->clbksCtx == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "opencallback",
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "uri=%s;errno=%d",
                    xmlSecErrorsSafeString(uri),
                    errno);
        return(-1);
    }

    return(0);
}


/**
 * xmlSecTransformInputURIClose:
 * @transform:          the pointer to IO transform.
 *
 * Closes the given @transform and frees up resourses.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecTransformInputURIClose(xmlSecTransformPtr transform) {
    xmlSecInputURICtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformInputURIId), -1);

    ctx = xmlSecTransformInputUriGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    /* close if still open and mark as closed */
    if((ctx->clbksCtx != NULL) && (ctx->clbks != NULL) && (ctx->clbks->closecallback != NULL)) {
    	(ctx->clbks->closecallback)(ctx->clbksCtx);
    	ctx->clbksCtx = NULL;
    	ctx->clbks = NULL;
    }

    /* done */
    return(0);
}

static int
xmlSecTransformInputURIInitialize(xmlSecTransformPtr transform) {
    xmlSecInputURICtxPtr ctx;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformInputURIId), -1);

    ctx = xmlSecTransformInputUriGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecInputURICtx));
    return(0);
}

static void
xmlSecTransformInputURIFinalize(xmlSecTransformPtr transform) {
	xmlSecInputURICtxPtr ctx;
	int ret;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecTransformInputURIId));

    ctx = xmlSecTransformInputUriGetCtx(transform);
    xmlSecAssert(ctx != NULL);

    ret = xmlSecTransformInputURIClose(transform);
    if(ret < 0) {
		xmlSecError(XMLSEC_ERRORS_HERE,
					xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
					"xmlSecTransformInputURIClose",
					XMLSEC_ERRORS_R_XMLSEC_FAILED,
					"ret=%d", ret);
		/* ignore the error */
		/* return; */
	}

    memset(ctx, 0, sizeof(xmlSecInputURICtx));
    return;
}

static int
xmlSecTransformInputURIPopBin(xmlSecTransformPtr transform, xmlSecByte* data,
                              xmlSecSize maxDataSize, xmlSecSize* dataSize,
                              xmlSecTransformCtxPtr transformCtx) {
    xmlSecInputURICtxPtr ctx;

    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformInputURIId), -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize != NULL, -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecTransformInputUriGetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    if((ctx->clbksCtx != NULL) && (ctx->clbks != NULL) && (ctx->clbks->readcallback != NULL)) {
        ret = (ctx->clbks->readcallback)(ctx->clbksCtx, (char*)data, (int)maxDataSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        "readcallback",
                        XMLSEC_ERRORS_R_IO_FAILED,
                        "errno=%d", errno);
            return(-1);
        }
        (*dataSize) = ret;
    } else {
        (*dataSize) = 0;
    }
    return(0);
}


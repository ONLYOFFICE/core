/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Base64 encode/decode transform and utility functions.
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

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/base64.h>
#include <xmlsec/errors.h>

/*
 * the table to map numbers to base64
 */
static const xmlSecByte base64[] =
{
/*   0    1    2    3    4    5    6    7   */
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', /* 0 */
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', /* 1 */
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', /* 2 */
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', /* 3 */
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', /* 4 */
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', /* 5 */
    'w', 'x', 'y', 'z', '0', '1', '2', '3', /* 6 */
    '4', '5', '6', '7', '8', '9', '+', '/'  /* 7 */
};


/* few macros to simplify the code */
#define xmlSecBase64Encode1(a)          (((a) >> 2) & 0x3F)
#define xmlSecBase64Encode2(a, b)       ((((a) << 4) & 0x30) + (((b) >> 4) & 0x0F))
#define xmlSecBase64Encode3(b, c)       ((((b) << 2) & 0x3c) + (((c) >> 6) & 0x03))
#define xmlSecBase64Encode4(c)          ((c) & 0x3F)

#define xmlSecBase64Decode1(a, b)       (((a) << 2) | (((b) & 0x3F) >> 4))
#define xmlSecBase64Decode2(b, c)       (((b) << 4) | (((c) & 0x3F) >> 2))
#define xmlSecBase64Decode3(c, d)       (((c) << 6) | ((d) & 0x3F))

#define xmlSecIsBase64Char(ch)          ((((ch) >= 'A') && ((ch) <= 'Z')) || \
                                         (((ch) >= 'a') && ((ch) <= 'z')) || \
                                         (((ch) >= '0') && ((ch) <= '9')) || \
                                         ((ch) == '+') || ((ch) == '/'))
#define xmlSecIsBase64Space(ch)         (((ch) == ' ') || ((ch) == '\t') || \
                                         ((ch) == '\x0d') || ((ch) == '\x0a'))



/***********************************************************************
 *
 * Base64 Context
 *
 ***********************************************************************/
typedef enum {
    xmlSecBase64StatusConsumeAndNext  = 0,
    xmlSecBase64StatusConsumeAndRepeat,
    xmlSecBase64StatusNext,
    xmlSecBase64StatusDone,
    xmlSecBase64StatusFailed
} xmlSecBase64Status;

struct _xmlSecBase64Ctx {
    int                 encode;
    int                 inByte;
    int                 inPos;
    xmlSecSize          linePos;
    xmlSecSize          columns;
    int                 finished;
};

static xmlSecBase64Status       xmlSecBase64CtxEncodeByte       (xmlSecBase64CtxPtr ctx,
                                                                 xmlSecByte  inByte,
                                                                 xmlSecByte* outByte);
static xmlSecBase64Status       xmlSecBase64CtxEncodeByteFinal  (xmlSecBase64CtxPtr ctx,
                                                                 xmlSecByte* outByte);
static xmlSecBase64Status       xmlSecBase64CtxDecodeByte       (xmlSecBase64CtxPtr ctx,
                                                                 xmlSecByte inByte,
                                                                 xmlSecByte* outByte);
static int                      xmlSecBase64CtxEncode           (xmlSecBase64CtxPtr ctx,
                                                                 const xmlSecByte* inBuf,
                                                                 xmlSecSize inBufSize,
                                                                 xmlSecSize* inBufResSize,
                                                                 xmlSecByte* outBuf,
                                                                 xmlSecSize outBufSize,
                                                                 xmlSecSize* outBufResSize);
static int                      xmlSecBase64CtxEncodeFinal      (xmlSecBase64CtxPtr ctx,
                                                                 xmlSecByte* outBuf,
                                                                 xmlSecSize outBufSize,
                                                                 xmlSecSize* outBufResSize);
static int                      xmlSecBase64CtxDecode           (xmlSecBase64CtxPtr ctx,
                                                                 const xmlSecByte* inBuf,
                                                                 xmlSecSize inBufSize,
                                                                 xmlSecSize* inBufResSize,
                                                                 xmlSecByte* outBuf,
                                                                 xmlSecSize outBufSize,
                                                                 xmlSecSize* outBufResSize);
static int                      xmlSecBase64CtxDecodeIsFinished (xmlSecBase64CtxPtr ctx);


static int g_xmlsec_base64_default_line_size = XMLSEC_BASE64_LINESIZE;

/**
 * xmlSecBase64GetDefaultLineSize:
 *
 * Gets the current default line size.
 *
 * Returns: the current default line size.
 */
int
xmlSecBase64GetDefaultLineSize(void)
{
    return g_xmlsec_base64_default_line_size;
}

/**
 * xmlSecBase64SetDefaultLineSize:
 * @columns: number of columns
 *
 * Sets the current default line size.
 */
void
xmlSecBase64SetDefaultLineSize(int columns)
{
    g_xmlsec_base64_default_line_size = columns;
}

/**
 * xmlSecBase64CtxCreate:
 * @encode:             the encode/decode flag (1 - encode, 0 - decode)
 * @columns:            the max line length.
 *
 * Allocates and initializes new base64 context.
 *
 * Returns: a pointer to newly created #xmlSecBase64Ctx structure
 * or NULL if an error occurs.
 */
xmlSecBase64CtxPtr
xmlSecBase64CtxCreate(int encode, int columns) {
    xmlSecBase64CtxPtr ctx;
    int ret;

    /*
     * Allocate a new xmlSecBase64CtxPtr and fill the fields.
     */
    ctx = (xmlSecBase64CtxPtr) xmlMalloc(sizeof(xmlSecBase64Ctx));
    if (ctx == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "sizeof(xmlSecBase64Ctx)=%d",
                    (int)sizeof(xmlSecBase64Ctx));
        return(NULL);
    }

    ret = xmlSecBase64CtxInitialize(ctx, encode, columns);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBase64CtxDestroy(ctx);
        return(NULL);
    }
    return(ctx);
}

/**
 * xmlSecBase64CtxDestroy:
 * @ctx:                the pointer to #xmlSecBase64Ctx structure.
 *
 * Destroys base64 context.
 */
void
xmlSecBase64CtxDestroy(xmlSecBase64CtxPtr ctx) {
    xmlSecAssert(ctx != NULL);

    xmlSecBase64CtxFinalize(ctx);
    xmlFree(ctx);
}

/**
 * xmlSecBase64CtxInitialize:
 * @ctx:                the pointer to #xmlSecBase64Ctx structure,
 * @encode:             the encode/decode flag (1 - encode, 0 - decode)
 * @columns:            the max line length.
 *
 * Initializes new base64 context.
 *
 * Returns: 0 on success and a negative value otherwise.
 */
int
xmlSecBase64CtxInitialize(xmlSecBase64CtxPtr ctx, int encode, int columns) {
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecBase64Ctx));

    ctx->encode     = encode;
    ctx->columns    = columns;
    return(0);
}

/**
 * xmlSecBase64CtxFinalize:
 * @ctx:                the pointer to #xmlSecBase64Ctx structure,
 *
 * Frees all the resources allocated by @ctx.
 */
void
xmlSecBase64CtxFinalize(xmlSecBase64CtxPtr ctx) {
    xmlSecAssert(ctx != NULL);

    memset(ctx, 0, sizeof(xmlSecBase64Ctx));
}

/**
 * xmlSecBase64CtxUpdate:
 * @ctx:                the pointer to #xmlSecBase64Ctx structure
 * @in:                 the input buffer
 * @inSize:             the input buffer size
 * @out:                the output buffer
 * @outSize:            the output buffer size
 *
 * Encodes or decodes the next piece of data from input buffer.
 *
 * Returns: the number of bytes written to output buffer or
 * -1 if an error occurs.
 */
int
xmlSecBase64CtxUpdate(xmlSecBase64CtxPtr ctx,
                     const xmlSecByte *in, xmlSecSize inSize,
                     xmlSecByte *out, xmlSecSize outSize) {
    xmlSecSize inResSize = 0, outResSize = 0;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(in != NULL, -1);
    xmlSecAssert2(out != NULL, -1);

    if(ctx->encode != 0) {
        ret = xmlSecBase64CtxEncode(ctx, in, inSize, &inResSize,
                                    out, outSize, &outResSize);
        if((ret < 0) || (inResSize != inSize)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBase64CtxEncode",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    } else {
        ret = xmlSecBase64CtxDecode(ctx, in, inSize, &inResSize,
                                    out, outSize, &outResSize);
        if((ret < 0) || (inResSize != inSize)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBase64CtxDecode",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    return(outResSize);
}

/**
 * xmlSecBase64CtxFinal:
 * @ctx:                the pointer to #xmlSecBase64Ctx structure
 * @out:                the output buffer
 * @outSize:            the output buffer size
 *
 * Encodes or decodes the last piece of data stored in the context
 * and finalizes the result.
 *
 * Returns: the number of bytes written to output buffer or
 * -1 if an error occurs.
 */
int
xmlSecBase64CtxFinal(xmlSecBase64CtxPtr ctx,
                    xmlSecByte *out, xmlSecSize outSize) {
    xmlSecSize outResSize = 0;
    int ret;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(out != NULL, -1);
    xmlSecAssert2(outSize > 0, -1);

    if(ctx->encode != 0) {
        ret = xmlSecBase64CtxEncodeFinal(ctx, out, outSize, &outResSize);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBase64CtxEncodeFinal",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "outSize=%d", outSize);
            return(-1);
        }
    } else {
        if(!xmlSecBase64CtxDecodeIsFinished(ctx)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBase64CtxIsFinished",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    /* add \0 */
    if((outResSize + 1) < outSize) {
        out[outResSize] = '\0';
    }
    return(outResSize);
}

static xmlSecBase64Status
xmlSecBase64CtxEncodeByte(xmlSecBase64CtxPtr ctx, xmlSecByte inByte, xmlSecByte* outByte) {
    xmlSecAssert2(ctx != NULL, xmlSecBase64StatusFailed);
    xmlSecAssert2(outByte != NULL, xmlSecBase64StatusFailed);

    if((ctx->columns > 0) && (ctx->linePos >= ctx->columns)) {
        (*outByte) = '\n';
        ctx->linePos = 0;
        return(xmlSecBase64StatusConsumeAndRepeat);
    } else if(ctx->inPos == 0) {
        /* we just started new block */
        (*outByte) = base64[xmlSecBase64Encode1(inByte)];
        ctx->inByte = inByte;
        ++ctx->linePos;
        ++ctx->inPos;
        return(xmlSecBase64StatusConsumeAndNext);
    } else if(ctx->inPos == 1) {
        (*outByte) = base64[xmlSecBase64Encode2(ctx->inByte, inByte)];
        ctx->inByte = inByte;
        ++ctx->linePos;
        ++ctx->inPos;
        return(xmlSecBase64StatusConsumeAndNext);
    } else if(ctx->inPos == 2) {
        (*outByte) = base64[xmlSecBase64Encode3(ctx->inByte, inByte)];
        ctx->inByte = inByte;
        ++ctx->linePos;
        ++ctx->inPos;
        return(xmlSecBase64StatusConsumeAndRepeat);
    } else if(ctx->inPos == 3) {
        (*outByte) = base64[xmlSecBase64Encode4(ctx->inByte)];
        ++ctx->linePos;
        ctx->inByte = 0;
        ctx->inPos  = 0;
        return(xmlSecBase64StatusConsumeAndNext);
    }

    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                NULL,
                XMLSEC_ERRORS_R_INVALID_DATA,
                "ctx->inPos=%d", ctx->inPos);
    return(xmlSecBase64StatusFailed);
}

static xmlSecBase64Status
xmlSecBase64CtxEncodeByteFinal(xmlSecBase64CtxPtr ctx, xmlSecByte* outByte) {
    xmlSecAssert2(ctx != NULL, xmlSecBase64StatusFailed);
    xmlSecAssert2(outByte != NULL, xmlSecBase64StatusFailed);

    if(ctx->inPos == 0) {
        return(xmlSecBase64StatusDone);
    } else if((ctx->columns > 0) && (ctx->linePos >= ctx->columns)) {
        (*outByte) = '\n';
        ctx->linePos = 0;
        return(xmlSecBase64StatusConsumeAndRepeat);
    } else if(ctx->finished == 0) {
        ctx->finished = 1;
        return(xmlSecBase64CtxEncodeByte(ctx, 0, outByte));
    } else if(ctx->inPos < 3) {
        (*outByte) = '=';
        ++ctx->inPos;
        ++ctx->linePos;
        return(xmlSecBase64StatusConsumeAndRepeat);
    } else if(ctx->inPos == 3) {
        (*outByte) = '=';
        ++ctx->linePos;
        ctx->inPos = 0;
        return(xmlSecBase64StatusConsumeAndRepeat);
    }

    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                NULL,
                XMLSEC_ERRORS_R_INVALID_DATA,
                "ctx->inPos=%d", ctx->inPos);
    return(xmlSecBase64StatusFailed);
}

static xmlSecBase64Status
xmlSecBase64CtxDecodeByte(xmlSecBase64CtxPtr ctx, xmlSecByte inByte, xmlSecByte* outByte) {
    xmlSecAssert2(ctx != NULL, xmlSecBase64StatusFailed);
    xmlSecAssert2(outByte != NULL, xmlSecBase64StatusFailed);

    if((ctx->finished != 0) && (ctx->inPos == 0)) {
        return(xmlSecBase64StatusDone);
    } if(inByte == '=') {
        ctx->finished = 1;
        if(ctx->inPos < 2) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_DATA,
                        "ctx->inPos=%d", ctx->inPos);
            return(xmlSecBase64StatusFailed);
        } else if(ctx->inPos == 2) {
            ++ctx->inPos;
            return(xmlSecBase64StatusNext);
        } else if(ctx->inPos == 3) {
            ctx->inPos = 0;
            return(xmlSecBase64StatusNext);
        } else {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_DATA,
                        "ctx->inPos=%d", ctx->inPos);
            return(xmlSecBase64StatusFailed);
        }
    } else if(xmlSecIsBase64Space(inByte)) {
        return(xmlSecBase64StatusNext);
    } else if(!xmlSecIsBase64Char(inByte) || (ctx->finished != 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_DATA,
                    "inByte=0x%02x", inByte);
        return(xmlSecBase64StatusFailed);
    }

    /* convert from character to position in base64 array */
    if((inByte >= 'A') && (inByte <= 'Z')) {
        inByte = (inByte - 'A');
    } else if((inByte >= 'a') && (inByte <= 'z')) {
        inByte = 26 + (inByte - 'a');
    } else if((inByte >= '0') && (inByte <= '9')) {
        inByte = 52 + (inByte - '0');
    } else if(inByte == '+') {
        inByte = 62;
    } else if(inByte == '/') {
        inByte = 63;
    }

    if(ctx->inPos == 0) {
        ctx->inByte = inByte;
        ++ctx->inPos;
        return(xmlSecBase64StatusNext);
    } else if(ctx->inPos == 1) {
        (*outByte) = xmlSecBase64Decode1(ctx->inByte, inByte);
        ctx->inByte = inByte;
        ++ctx->inPos;
        return(xmlSecBase64StatusConsumeAndNext);
    } else if(ctx->inPos == 2) {
        (*outByte) = xmlSecBase64Decode2(ctx->inByte, inByte);
        ctx->inByte = inByte;
        ++ctx->inPos;
        return(xmlSecBase64StatusConsumeAndNext);
    } else if(ctx->inPos == 3) {
        (*outByte) = xmlSecBase64Decode3(ctx->inByte, inByte);
        ctx->inByte = 0;
        ctx->inPos = 0;
        return(xmlSecBase64StatusConsumeAndNext);
    }

    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                NULL,
                XMLSEC_ERRORS_R_INVALID_DATA,
                "ctx->inPos=%d", ctx->inPos);
    return(xmlSecBase64StatusFailed);
}


static int
xmlSecBase64CtxEncode(xmlSecBase64CtxPtr ctx,
                     const xmlSecByte* inBuf, xmlSecSize inBufSize, xmlSecSize* inBufResSize,
                     xmlSecByte* outBuf, xmlSecSize outBufSize, xmlSecSize* outBufResSize) {
    xmlSecBase64Status status = xmlSecBase64StatusNext;
    xmlSecSize inPos, outPos;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(inBuf != NULL, -1);
    xmlSecAssert2(inBufResSize != NULL, -1);
    xmlSecAssert2(outBuf != NULL, -1);
    xmlSecAssert2(outBufResSize != NULL, -1);

    /* encode */
    for(inPos = outPos = 0; (inPos < inBufSize) && (outPos < outBufSize); ) {
        status = xmlSecBase64CtxEncodeByte(ctx, inBuf[inPos], &(outBuf[outPos]));
        switch(status) {
            case xmlSecBase64StatusConsumeAndNext:
                ++inPos;
                ++outPos;
                break;
            case xmlSecBase64StatusConsumeAndRepeat:
                ++outPos;
                break;
            case xmlSecBase64StatusNext:
            case xmlSecBase64StatusDone:
            case xmlSecBase64StatusFailed:
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecBase64CtxEncodeByte",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "status=%d", status);
                return(-1);
        }
    }

    (*inBufResSize)  = inPos;
    (*outBufResSize) = outPos;

    return(0);
}

static int
xmlSecBase64CtxEncodeFinal(xmlSecBase64CtxPtr ctx,
                     xmlSecByte* outBuf, xmlSecSize outBufSize, xmlSecSize* outBufResSize) {
    xmlSecBase64Status status = xmlSecBase64StatusNext;
    xmlSecSize outPos;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(outBuf != NULL, -1);
    xmlSecAssert2(outBufResSize != NULL, -1);

    /* encode final bytes */
    for(outPos = 0; (outPos < outBufSize) && (status != xmlSecBase64StatusDone); ) {
        status = xmlSecBase64CtxEncodeByteFinal(ctx, &(outBuf[outPos]));
        switch(status) {
            case xmlSecBase64StatusConsumeAndNext:
            case xmlSecBase64StatusConsumeAndRepeat:
                ++outPos;
                break;
            case xmlSecBase64StatusDone:
                break;
            case xmlSecBase64StatusNext:
            case xmlSecBase64StatusFailed:
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecBase64CtxEncodeByteFinal",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "status=%d", status);
                return(-1);
        }
    }

    if(status != xmlSecBase64StatusDone) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "outBufSize=%d", outBufSize);
        return(-1);
    }
    if(outPos < outBufSize) {
        outBuf[outPos] = '\0'; /* just in case */
    }

    (*outBufResSize) = outPos;
    return(0);
}


static int
xmlSecBase64CtxDecode(xmlSecBase64CtxPtr ctx,
                     const xmlSecByte* inBuf, xmlSecSize inBufSize, xmlSecSize* inBufResSize,
                     xmlSecByte* outBuf, xmlSecSize outBufSize, xmlSecSize* outBufResSize) {
    xmlSecBase64Status status = xmlSecBase64StatusNext;
    xmlSecSize inPos, outPos;

    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(inBuf != NULL, -1);
    xmlSecAssert2(inBufResSize != NULL, -1);
    xmlSecAssert2(outBuf != NULL, -1);
    xmlSecAssert2(outBufResSize != NULL, -1);

    /* decode */
    for(inPos = outPos = 0; (inPos < inBufSize) && (outPos < outBufSize) && (status != xmlSecBase64StatusDone); ) {
        status = xmlSecBase64CtxDecodeByte(ctx, inBuf[inPos], &(outBuf[outPos]));
        switch(status) {
            case xmlSecBase64StatusConsumeAndNext:
                ++inPos;
                ++outPos;
                break;
            case xmlSecBase64StatusConsumeAndRepeat:
                ++outPos;
                break;
            case xmlSecBase64StatusNext:
                ++inPos;
                break;
            case xmlSecBase64StatusDone:
                break;
            case xmlSecBase64StatusFailed:
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecBase64CtxDecodeByte",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "status=%d", status);
                return(-1);
        }
    }

    /* skip spaces at the end */
    while((inPos < inBufSize) && xmlSecIsBase64Space(inBuf[inPos])) {
        ++inPos;
    }

    (*inBufResSize)  = inPos;
    (*outBufResSize) = outPos;

    return(0);
}

static int
xmlSecBase64CtxDecodeIsFinished(xmlSecBase64CtxPtr ctx) {
    xmlSecAssert2(ctx != NULL, -1);

    return((ctx->inPos == 0) ? 1 : 0);
}

/**
 * xmlSecBase64Encode:
 * @buf:                the input buffer.
 * @len:                the input buffer size.
 * @columns:            the output max line length (if 0 then no line breaks
 *                      would be inserted)
 *
 * Encodes the data from input buffer and allocates the string for the result.
 * The caller is responsible for freeing returned buffer using
 * xmlFree() function.
 *
 * Returns: newly allocated string with base64 encoded data
 * or NULL if an error occurs.
 */
xmlChar*
xmlSecBase64Encode(const xmlSecByte *buf, xmlSecSize len, int columns) {
    xmlSecBase64Ctx ctx;
    xmlChar *ptr;
    xmlSecSize size;
    int size_update, size_final;
    int ret;

    xmlSecAssert2(buf != NULL, NULL);

    ret = xmlSecBase64CtxInitialize(&ctx, 1, columns);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    /* create result buffer */
    size = (4 * len) / 3 + 4;
    if(columns > 0) {
        size += (size / columns) + 4;
    }
    ptr = (xmlChar*) xmlMalloc(size);
    if(ptr == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", size);
        xmlSecBase64CtxFinalize(&ctx);
        return(NULL);
    }

    ret = xmlSecBase64CtxUpdate(&ctx, buf, len, (xmlSecByte*)ptr, size);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxUpdate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "len=%d", len);
        xmlFree(ptr);
        xmlSecBase64CtxFinalize(&ctx);
        return(NULL);
    }
    size_update = ret;

    ret = xmlSecBase64CtxFinal(&ctx, ((xmlSecByte*)ptr) + size_update, size - size_update);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxFinal",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(ptr);
        xmlSecBase64CtxFinalize(&ctx);
        return(NULL);
    }
    size_final = ret;
    ptr[size_update + size_final] = '\0';

    xmlSecBase64CtxFinalize(&ctx);
    return(ptr);
}

/**
 * xmlSecBase64Decode:
 * @str:                the input buffer with base64 encoded string
 * @buf:                the output buffer
 * @len:                the output buffer size
 *
 * Decodes input base64 encoded string and puts result into
 * the output buffer.
 *
 * Returns: the number of bytes written to the output buffer or
 * a negative value if an error occurs
 */
int
xmlSecBase64Decode(const xmlChar* str, xmlSecByte *buf, xmlSecSize len) {
    xmlSecBase64Ctx ctx;
    int size_update;
    int size_final;
    int ret;

    xmlSecAssert2(str != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);

    ret = xmlSecBase64CtxInitialize(&ctx, 0, 0);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecBase64CtxUpdate(&ctx, (const xmlSecByte*)str, xmlStrlen(str), buf, len);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxUpdate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBase64CtxFinalize(&ctx);
        return(-1);
    }

    size_update = ret;
    ret = xmlSecBase64CtxFinal(&ctx, buf + size_update, len - size_update);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64CtxFinal",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBase64CtxFinalize(&ctx);
        return(-1);
    }
    size_final = ret;

    xmlSecBase64CtxFinalize(&ctx);
    return(size_update + size_final);
}

/**************************************************************
 *
 * Base64 Transform
 *
 * xmlSecBase64Ctx is located after xmlSecTransform
 *
 **************************************************************/
#define xmlSecBase64Size \
        (sizeof(xmlSecTransform) + sizeof(xmlSecBase64Ctx))
#define xmlSecBase64GetCtx(transform) \
    ((xmlSecTransformCheckSize((transform), xmlSecBase64Size)) ? \
        (xmlSecBase64CtxPtr)(((xmlSecByte*)(transform)) + sizeof(xmlSecTransform)) : \
        (xmlSecBase64CtxPtr)NULL)

static int              xmlSecBase64Initialize          (xmlSecTransformPtr transform);
static void             xmlSecBase64Finalize            (xmlSecTransformPtr transform);
static int              xmlSecBase64Execute             (xmlSecTransformPtr transform,
                                                         int last,
                                                         xmlSecTransformCtxPtr transformCtx);

static xmlSecTransformKlass xmlSecBase64Klass = {
    /* klass/object sizes */
    sizeof(xmlSecTransformKlass),               /* xmlSecSize klassSize */
    xmlSecBase64Size,                           /* xmlSecSize objSize */

    xmlSecNameBase64,                           /* const xmlChar* name; */
    xmlSecHrefBase64,                           /* const xmlChar* href; */
    xmlSecTransformUsageDSigTransform,          /* xmlSecAlgorithmUsage usage; */

    xmlSecBase64Initialize,                     /* xmlSecTransformInitializeMethod initialize; */
    xmlSecBase64Finalize,                       /* xmlSecTransformFinalizeMethod finalize; */
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
    xmlSecBase64Execute,                        /* xmlSecTransformExecuteMethod execute; */

    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecTransformBase64GetKlass:
 *
 * The Base64 transform klass (http://www.w3.org/TR/xmldsig-core/#sec-Base-64).
 * The normative specification for base64 decoding transforms is RFC 2045
 * (http://www.ietf.org/rfc/rfc2045.txt). The base64 Transform element has
 * no content. The input is decoded by the algorithms. This transform is
 * useful if an application needs to sign the raw data associated with
 * the encoded content of an element.
 *
 * Returns: base64 transform id.
 */
xmlSecTransformId
xmlSecTransformBase64GetKlass(void) {
    return(&xmlSecBase64Klass);
}

/**
 * xmlSecTransformBase64SetLineSize:
 * @transform:          the pointer to BASE64 encode transform.
 * @lineSize:           the new max line size.
 *
 * Sets the max line size to @lineSize.
 */
void
xmlSecTransformBase64SetLineSize(xmlSecTransformPtr transform, xmlSecSize lineSize) {
    xmlSecBase64CtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecTransformBase64Id));

    ctx = xmlSecBase64GetCtx(transform);
    xmlSecAssert(ctx != NULL);

    ctx->columns = lineSize;
}

static int
xmlSecBase64Initialize(xmlSecTransformPtr transform) {
    xmlSecBase64CtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformBase64Id), -1);

    ctx = xmlSecBase64GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    transform->operation = xmlSecTransformOperationDecode;
    ret = xmlSecBase64CtxInitialize(ctx, 0, xmlSecBase64GetDefaultLineSize());
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                    "xmlSecBase64CtxInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

static void
xmlSecBase64Finalize(xmlSecTransformPtr transform) {
    xmlSecBase64CtxPtr ctx;

    xmlSecAssert(xmlSecTransformCheckId(transform, xmlSecTransformBase64Id));

    ctx = xmlSecBase64GetCtx(transform);
    xmlSecAssert(ctx != NULL);

    xmlSecBase64CtxFinalize(ctx);
}

static int
xmlSecBase64Execute(xmlSecTransformPtr transform, int last, xmlSecTransformCtxPtr transformCtx) {
    xmlSecBase64CtxPtr ctx;
    xmlSecBufferPtr in, out;
    xmlSecSize inSize, outSize, outLen;
    int ret;

    xmlSecAssert2(xmlSecTransformCheckId(transform, xmlSecTransformBase64Id), -1);
    xmlSecAssert2((transform->operation == xmlSecTransformOperationEncode) || (transform->operation == xmlSecTransformOperationDecode), -1);
    xmlSecAssert2(transformCtx != NULL, -1);

    ctx = xmlSecBase64GetCtx(transform);
    xmlSecAssert2(ctx != NULL, -1);

    in = &(transform->inBuf);
    out = &(transform->outBuf);

    if(transform->status == xmlSecTransformStatusNone) {
        ctx->encode = (transform->operation == xmlSecTransformOperationEncode) ? 1 : 0;
        transform->status = xmlSecTransformStatusWorking;
    }

    switch(transform->status) {
        case xmlSecTransformStatusWorking:
            inSize = xmlSecBufferGetSize(in);
            outSize = xmlSecBufferGetSize(out);
            if(inSize > 0) {
                if(ctx->encode != 0) {
                    outLen = 4 * inSize / 3 + 8;
                    if(ctx->columns > 0) {
                        outLen += inSize / ctx->columns + 4;
                    }
                } else {
                    outLen = 3 * inSize / 4 + 8;
                }
                ret = xmlSecBufferSetMaxSize(out, outSize + outLen);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBufferSetMaxSize",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "size=%d", outSize + outLen);
                    return(-1);
                }

                /* encode/decode the next chunk */
                ret = xmlSecBase64CtxUpdate(ctx, xmlSecBufferGetData(in), inSize,
                                            xmlSecBufferGetData(out) + outSize,
                                            outLen);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBase64CtxUpdate",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    return(-1);
                }
                outLen = ret;

                /* set correct size */
                ret = xmlSecBufferSetSize(out, outSize + outLen);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBufferSetSize",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "size=%d", outSize + outLen);
                    return(-1);
                }

                /* remove chunk from input */
                ret = xmlSecBufferRemoveHead(in, inSize);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBufferRemoveHead",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "size=%d", inSize);
                    return(-1);
                }
            }

            if(last) {
                outSize = xmlSecBufferGetSize(out);

                ret = xmlSecBufferSetMaxSize(out, outSize + 16);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBufferSetMaxSize",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "size=%d", outSize + 16);
                    return(-1);
                }

                /* add from ctx buffer */
                ret = xmlSecBase64CtxFinal(ctx, xmlSecBufferGetData(out) + outSize, 16);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBase64CtxFinal",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    return(-1);
                }
                outLen = ret;

                /* set correct size */
                ret = xmlSecBufferSetSize(out, outSize + outLen);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                                "xmlSecBufferSetSize",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "size=%d", outSize + outLen);
                    return(-1);
                }
                transform->status = xmlSecTransformStatusFinished;
            }
            break;
        case xmlSecTransformStatusFinished:
            /* the only way we can get here is if there is no input */
            xmlSecAssert2(xmlSecBufferGetSize(in) == 0, -1);
            break;
        default:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecTransformGetName(transform)),
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_STATUS,
                        "status=%d", transform->status);
            return(-1);
    }
    return(0);
}



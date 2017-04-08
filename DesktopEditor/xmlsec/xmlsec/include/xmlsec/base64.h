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
#ifndef __XMLSEC_BASE64_H__
#define __XMLSEC_BASE64_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/transforms.h>

/**
 * XMLSEC_BASE64_LINESIZE:
 *
 * The default maximum base64 encoded line size.
 */
#define XMLSEC_BASE64_LINESIZE                          64

XMLSEC_EXPORT int               xmlSecBase64GetDefaultLineSize  (void);
XMLSEC_EXPORT void              xmlSecBase64SetDefaultLineSize  (int columns);


/* Base64 Context */
typedef struct _xmlSecBase64Ctx                                 xmlSecBase64Ctx,
                                                                *xmlSecBase64CtxPtr;

XMLSEC_EXPORT xmlSecBase64CtxPtr xmlSecBase64CtxCreate          (int encode,
                                                                 int columns);
XMLSEC_EXPORT void              xmlSecBase64CtxDestroy          (xmlSecBase64CtxPtr ctx);
XMLSEC_EXPORT int               xmlSecBase64CtxInitialize       (xmlSecBase64CtxPtr ctx,
                                                                 int encode,
                                                                 int columns);
XMLSEC_EXPORT void              xmlSecBase64CtxFinalize         (xmlSecBase64CtxPtr ctx);
XMLSEC_EXPORT int               xmlSecBase64CtxUpdate           (xmlSecBase64CtxPtr ctx,
                                                                 const xmlSecByte *in,
                                                                 xmlSecSize inSize,
                                                                 xmlSecByte *out,
                                                                 xmlSecSize outSize);
XMLSEC_EXPORT int               xmlSecBase64CtxFinal            (xmlSecBase64CtxPtr ctx,
                                                                 xmlSecByte *out,
                                                                 xmlSecSize outSize);

/* Standalone routines to do base64 encode/decode "at once" */
XMLSEC_EXPORT xmlChar*          xmlSecBase64Encode              (const xmlSecByte *buf,
                                                                 xmlSecSize len,
                                                                 int columns);
XMLSEC_EXPORT int               xmlSecBase64Decode              (const xmlChar* str,
                                                                 xmlSecByte *buf,
                                                                 xmlSecSize len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_BASE64_H__ */


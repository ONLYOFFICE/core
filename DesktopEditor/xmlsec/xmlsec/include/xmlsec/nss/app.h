/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#ifndef __XMLSEC_NSS_APP_H__
#define __XMLSEC_NSS_APP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <nspr.h>
#include <nss.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>

/********************************************************************
 *
 * Init/shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppInit                (const char* config);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppShutdown            (void);

/********************************************************************
 *
 * Keys Manager
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppDefaultKeysMngrInit (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppDefaultKeysMngrAdoptKey(xmlSecKeysMngrPtr mngr,
                                                                            xmlSecKeyPtr key);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppDefaultKeysMngrLoad (xmlSecKeysMngrPtr mngr,
                                                                         const char* uri);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppDefaultKeysMngrSave (xmlSecKeysMngrPtr mngr,
                                                                         const char* filename,
                                                                         xmlSecKeyDataType type);
#ifndef XMLSEC_NO_X509
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppKeysMngrCertLoad    (xmlSecKeysMngrPtr mngr,
                                                                         const char *filename,
                                                                         xmlSecKeyDataFormat format,
                                                                         xmlSecKeyDataType type);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppKeysMngrCertLoadMemory(xmlSecKeysMngrPtr mngr,
                                                                         const xmlSecByte *data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format,
                                                                         xmlSecKeyDataType type);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppKeysMngrCertLoadSECItem(xmlSecKeysMngrPtr mngr,
                                                                         SECItem* secItem,
                                                                         xmlSecKeyDataFormat format,
                                                                         xmlSecKeyDataType type);
#endif /* XMLSEC_NO_X509 */


/********************************************************************
 *
 * Keys
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppKeyLoad             (const char *filename,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void *pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppKeyLoadMemory       (const xmlSecByte *data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void *pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppKeyLoadSECItem      (SECItem* secItem,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void *pwdCallback,
                                                                         void* pwdCallbackCtx);
#ifndef XMLSEC_NO_X509
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppPkcs12Load          (const char *filename,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppPkcs12LoadMemory    (const xmlSecByte *data,
                                                                         xmlSecSize dataSize,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppPkcs12LoadSECItem   (SECItem* secItem,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppKeyCertLoad         (xmlSecKeyPtr key,
                                                                         const char* filename,
                                                                         xmlSecKeyDataFormat format);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppKeyCertLoadMemory   (xmlSecKeyPtr key,
                                                                         const xmlSecByte *data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssAppKeyCertLoadSECItem  (xmlSecKeyPtr key,
                                                                         SECItem* secItem,
                                                                         xmlSecKeyDataFormat format);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr       xmlSecNssAppKeyFromCertLoadSECItem(SECItem* secItem,
                                                                         xmlSecKeyDataFormat format);
#endif /* XMLSEC_NO_X509 */
XMLSEC_CRYPTO_EXPORT void*              xmlSecNssAppGetDefaultPwdCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_NSS_APP_H__ */


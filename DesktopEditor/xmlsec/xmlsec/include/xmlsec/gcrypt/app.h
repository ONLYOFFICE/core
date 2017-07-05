/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GCRYPT_APP_H__
#define __XMLSEC_GCRYPT_APP_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>

/********************************************************************
 *
 * Init/shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppInit                     (const char* config);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppShutdown                 (void);

/********************************************************************
 *
 * Keys Manager
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppDefaultKeysMngrInit      (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppDefaultKeysMngrAdoptKey  (xmlSecKeysMngrPtr mngr,
                                                                         xmlSecKeyPtr key);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppDefaultKeysMngrLoad      (xmlSecKeysMngrPtr mngr,
                                                                         const char* uri);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppDefaultKeysMngrSave      (xmlSecKeysMngrPtr mngr,
                                                                         const char* filename,
                                                                         xmlSecKeyDataType type);
#ifndef XMLSEC_NO_X509
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppKeysMngrCertLoad         (xmlSecKeysMngrPtr mngr,
                                                                         const char *filename,
                                                                         xmlSecKeyDataFormat format,
                                                                         xmlSecKeyDataType type);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppKeysMngrCertLoadMemory   (xmlSecKeysMngrPtr mngr,
                                                                         const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format,
                                                                         xmlSecKeyDataType type);
#endif /* XMLSEC_NO_X509 */


/********************************************************************
 *
 * Keys
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGCryptAppKeyLoad                (const char *filename,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void *pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGCryptAppKeyLoadMemory          (const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
#ifndef XMLSEC_NO_X509
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGCryptAppPkcs12Load             (const char *filename,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGCryptAppPkcs12LoadMemory       (const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppKeyCertLoad              (xmlSecKeyPtr key,
                                                                         const char* filename,
                                                                         xmlSecKeyDataFormat format);
XMLSEC_CRYPTO_EXPORT int        xmlSecGCryptAppKeyCertLoadMemory        (xmlSecKeyPtr key,
                                                                         const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format);
#endif /* XMLSEC_NO_X509 */
XMLSEC_CRYPTO_EXPORT void*      xmlSecGCryptAppGetDefaultPwdCallback    (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_GCRYPT_APP_H__ */


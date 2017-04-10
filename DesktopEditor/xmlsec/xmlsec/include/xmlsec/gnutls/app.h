/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GNUTLS_APP_H__
#define __XMLSEC_GNUTLS_APP_H__

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
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppInit                     (const char* config);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppShutdown                 (void);

/********************************************************************
 *
 * Keys Manager
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppDefaultKeysMngrInit      (xmlSecKeysMngrPtr mngr);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppDefaultKeysMngrAdoptKey  (xmlSecKeysMngrPtr mngr,
                                                                         xmlSecKeyPtr key);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppDefaultKeysMngrLoad      (xmlSecKeysMngrPtr mngr,
                                                                         const char* uri);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppDefaultKeysMngrSave      (xmlSecKeysMngrPtr mngr,
                                                                         const char* filename,
                                                                         xmlSecKeyDataType type);
#ifndef XMLSEC_NO_X509
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppKeysMngrCertLoad         (xmlSecKeysMngrPtr mngr,
                                                                         const char *filename,
                                                                         xmlSecKeyDataFormat format,
                                                                         xmlSecKeyDataType type);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppKeysMngrCertLoadMemory   (xmlSecKeysMngrPtr mngr,
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
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGnuTLSAppKeyLoad                (const char *filename,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void *pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGnuTLSAppKeyLoadMemory          (const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
#ifndef XMLSEC_NO_X509
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGnuTLSAppPkcs12Load             (const char *filename,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT xmlSecKeyPtr xmlSecGnuTLSAppPkcs12LoadMemory       (const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         const char *pwd,
                                                                         void* pwdCallback,
                                                                         void* pwdCallbackCtx);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppKeyCertLoad              (xmlSecKeyPtr key,
                                                                         const char* filename,
                                                                         xmlSecKeyDataFormat format);
XMLSEC_CRYPTO_EXPORT int        xmlSecGnuTLSAppKeyCertLoadMemory        (xmlSecKeyPtr key,
                                                                         const xmlSecByte* data,
                                                                         xmlSecSize dataSize,
                                                                         xmlSecKeyDataFormat format);
#endif /* XMLSEC_NO_X509 */
XMLSEC_CRYPTO_EXPORT void*      xmlSecGnuTLSAppGetDefaultPwdCallback    (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_GNUTLS_APP_H__ */


/**
 * XMLSec library
 *
 * THIS IS A PRIVATE XMLSEC HEADER FILE
 * DON'T USE IT IN YOUR APPLICATION
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_MSCRYPTO_PRIVATE_H__
#define __XMLSEC_MSCRYPTO_PRIVATE_H__

#ifndef XMLSEC_PRIVATE
#error "private.h file contains private xmlsec definitions and should not be used outside xmlsec or xmlsec-$crypto libraries"
#endif /* XMLSEC_PRIVATE */

#if defined(__MINGW32__)
#  include "xmlsec-mingw.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************************************************************
 *
 * Utils
 *
 ********************************************************************/
int                ConvertEndian                    (const xmlSecByte * src, 
                                                     xmlSecByte * dst, 
                                                     xmlSecSize size);
int                ConvertEndianInPlace             (xmlSecByte * buf, 
                                                     xmlSecSize size);

/********************************************************************
 *
 * Crypto Providers
 *
 ********************************************************************/

/* We need to redefine both to ensure that we can pick the right one at runtime (instead of compile time) */
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE_A     "Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)"
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE_W     L"Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)"
#ifdef UNICODE
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE_W
#else
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_PROTOTYPE_A
#endif

#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_A               "Microsoft Enhanced RSA and AES Cryptographic Provider"
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_W               L"Microsoft Enhanced RSA and AES Cryptographic Provider"
#ifdef UNICODE
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_W
#else
#define XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV XMLSEC_CRYPTO_MS_ENH_RSA_AES_PROV_A
#endif

/**
 * xmlSecMSCryptoProviderInfo:
 *
 * Contains information for looking up provider from MS Crypto.
 */
typedef struct _xmlSecMSCryptoProviderInfo {
    LPCTSTR                 providerName;
    DWORD                   providerType;
} xmlSecMSCryptoProviderInfo;

HCRYPTPROV         xmlSecMSCryptoFindProvider                   (const xmlSecMSCryptoProviderInfo * providers,
                                                                 LPCTSTR pszContainer,
                                                                 DWORD dwFlags,
                                                                 BOOL bUseXmlSecContainer);


/******************************************************************************
 *
 * SymKey Util functions
 *
 * Low level helper routines for importing plain text keys in MS HKEY handle,
 * since MSCrypto API does not support import of plain text (session) keys
 * just like that. These functions are based upon MS kb article #228786
 * and "Base Provider Key BLOBs" article for priv key blob format.
 *
 ******************************************************************************/
BOOL               xmlSecMSCryptoCreatePrivateExponentOneKey    (HCRYPTPROV hProv,
                                                                 HCRYPTKEY *hPrivateKey);

BOOL               xmlSecMSCryptoImportPlainSessionBlob         (HCRYPTPROV hProv,
                                                                 HCRYPTKEY hPrivateKey,
                                                                 ALG_ID dwAlgId,
                                                                 LPBYTE pbKeyMaterial,
                                                                 DWORD dwKeyMaterial,
                                                                 BOOL bCheckKeyLength,
                                                                 HCRYPTKEY *hSessionKey);

/******************************************************************************
 *
 * X509 Util functions
 *
 ******************************************************************************/
#ifndef XMLSEC_NO_X509
PCCERT_CONTEXT     xmlSecMSCryptoX509FindCertBySubject          (HCERTSTORE store,
                                                                 const LPTSTR wcSubject,
                                                                 DWORD dwCertEncodingType);

PCCERT_CONTEXT     xmlSecMSCryptoX509StoreFindCert              (xmlSecKeyDataStorePtr store,
                                                                 xmlChar *subjectName,
                                                                 xmlChar *issuerName,
                                                                 xmlChar *issuerSerial,
                                                                 xmlChar *ski,
                                                                 xmlSecKeyInfoCtx* keyInfoCtx);

xmlChar *          xmlSecMSCryptoX509GetNameString              (PCCERT_CONTEXT pCertContext,
                                                                 DWORD dwType,
                                                                 DWORD dwFlags,
                                                                 void *pvTypePara);

PCCERT_CONTEXT     xmlSecMSCryptoX509StoreVerify                (xmlSecKeyDataStorePtr store,
                                                                 HCERTSTORE certs,
                                                                 xmlSecKeyInfoCtx* keyInfoCtx);

#endif /* XMLSEC_NO_X509 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_MSCRYPTO_PRIVATE_H__ */

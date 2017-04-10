/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#ifndef __XMLSEC_NSS_SYMBOLS_H__
#define __XMLSEC_NSS_SYMBOLS_H__

#if !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
#error To disable dynamic loading of xmlsec-crypto libraries undefine XMLSEC_CRYPTO_DYNAMIC_LOADING
#endif /* !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef XMLSEC_CRYPTO_NSS

/********************************************************************
 *
 * Crypto Init/shutdown
 *
 ********************************************************************/
#define xmlSecCryptoInit                        xmlSecNssInit
#define xmlSecCryptoShutdown                    xmlSecNssShutdown

#define xmlSecCryptoKeysMngrInit                xmlSecNssKeysMngrInit

/********************************************************************
 *
 * Key data ids
 *
 ********************************************************************/
#define xmlSecKeyDataAesId                      xmlSecNssKeyDataAesId
#define xmlSecKeyDataDesId                      xmlSecNssKeyDataDesId
#define xmlSecKeyDataDsaId                      xmlSecNssKeyDataDsaId
#define xmlSecKeyDataHmacId                     xmlSecNssKeyDataHmacId
#define xmlSecKeyDataRsaId                      xmlSecNssKeyDataRsaId
#define xmlSecKeyDataX509Id                     xmlSecNssKeyDataX509Id
#define xmlSecKeyDataRawX509CertId              xmlSecNssKeyDataRawX509CertId

/********************************************************************
 *
 * Key data store ids
 *
 ********************************************************************/
#define xmlSecX509StoreId                       xmlSecNssX509StoreId

/********************************************************************
 *
 * Crypto transforms ids
 *
 ********************************************************************/
#define xmlSecTransformAes128CbcId              xmlSecNssTransformAes128CbcId
#define xmlSecTransformAes192CbcId              xmlSecNssTransformAes192CbcId
#define xmlSecTransformAes256CbcId              xmlSecNssTransformAes256CbcId
#define xmlSecTransformKWAes128Id               xmlSecNssTransformKWAes128Id
#define xmlSecTransformKWAes192Id               xmlSecNssTransformKWAes192Id
#define xmlSecTransformKWAes256Id               xmlSecNssTransformKWAes256Id
#define xmlSecTransformDes3CbcId                xmlSecNssTransformDes3CbcId
#define xmlSecTransformKWDes3Id                 xmlSecNssTransformKWDes3Id
#define xmlSecTransformDsaSha1Id                xmlSecNssTransformDsaSha1Id
#define xmlSecTransformHmacMd5Id                xmlSecNssTransformHmacMd5Id
#define xmlSecTransformHmacRipemd160Id          xmlSecNssTransformHmacRipemd160Id
#define xmlSecTransformHmacSha1Id               xmlSecNssTransformHmacSha1Id
#define xmlSecTransformRipemd160Id              xmlSecNssTransformRipemd160Id
#define xmlSecTransformRsaSha1Id                xmlSecNssTransformRsaSha1Id
#define xmlSecTransformRsaPkcs1Id               xmlSecNssTransformRsaPkcs1Id
#define xmlSecTransformRsaOaepId                xmlSecNssTransformRsaOaepId
#define xmlSecTransformSha1Id                   xmlSecNssTransformSha1Id

/********************************************************************
 *
 * High level routines form xmlsec command line utility
 *
 ********************************************************************/
#define xmlSecCryptoAppInit                     xmlSecNssAppInit
#define xmlSecCryptoAppShutdown                 xmlSecNssAppShutdown
#define xmlSecCryptoAppDefaultKeysMngrInit      xmlSecNssAppDefaultKeysMngrInit
#define xmlSecCryptoAppDefaultKeysMngrAdoptKey  xmlSecNssAppDefaultKeysMngrAdoptKey
#define xmlSecCryptoAppDefaultKeysMngrLoad      xmlSecNssAppDefaultKeysMngrLoad
#define xmlSecCryptoAppDefaultKeysMngrSave      xmlSecNssAppDefaultKeysMngrSave
#define xmlSecCryptoAppKeysMngrCertLoad         xmlSecNssAppKeysMngrCertLoad
#define xmlSecCryptoAppKeysMngrCertLoadMemory   xmlSecNssAppKeysMngrCertLoadMemory
#define xmlSecCryptoAppKeyLoad                  xmlSecNssAppKeyLoad
#define xmlSecCryptoAppPkcs12Load               xmlSecNssAppPkcs12Load
#define xmlSecCryptoAppKeyCertLoad              xmlSecNssAppKeyCertLoad
#define xmlSecCryptoAppKeyLoadMemory            xmlSecNssAppKeyLoadMemory
#define xmlSecCryptoAppPkcs12LoadMemory         xmlSecNssAppPkcs12LoadMemory
#define xmlSecCryptoAppKeyCertLoadMemory        xmlSecNssAppKeyCertLoadMemory
#define xmlSecCryptoAppGetDefaultPwdCallback    xmlSecNssAppGetDefaultPwdCallback

#endif /* XMLSEC_CRYPTO_NSS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_NSS_CRYPTO_H__ */

#define __XMLSEC_NSS_CRYPTO_H__

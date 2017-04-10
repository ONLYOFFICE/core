/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GCRYPT_SYMBOLS_H__
#define __XMLSEC_GCRYPT_SYMBOLS_H__

#if !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
#error To disable dynamic loading of xmlsec-crypto libraries undefine XMLSEC_CRYPTO_DYNAMIC_LOADING
#endif /* !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef XMLSEC_CRYPTO_GCRYPT

/********************************************************************
 *
 * Crypto Init/shutdown
 *
 ********************************************************************/
#define xmlSecCryptoInit                        xmlSecGCryptInit
#define xmlSecCryptoShutdown                    xmlSecGCryptShutdown

#define xmlSecCryptoKeysMngrInit                xmlSecGCryptKeysMngrInit

/********************************************************************
 *
 * Key data ids
 *
 ********************************************************************/
#define xmlSecKeyDataAesId                      xmlSecGCryptKeyDataAesId
#define xmlSecKeyDataDesId                      xmlSecGCryptKeyDataDesId
#define xmlSecKeyDataDsaId                      xmlSecGCryptKeyDataDsaId
#define xmlSecKeyDataHmacId                     xmlSecGCryptKeyDataHmacId
#define xmlSecKeyDataRsaId                      xmlSecGCryptKeyDataRsaId
#define xmlSecKeyDataX509Id                     xmlSecGCryptKeyDataX509Id
#define xmlSecKeyDataRawX509CertId              xmlSecGCryptKeyDataRawX509CertId

/********************************************************************
 *
 * Key data store ids
 *
 ********************************************************************/
#define xmlSecX509StoreId                       xmlSecGCryptX509StoreId

/********************************************************************
 *
 * Crypto transforms ids
 *
 ********************************************************************/
#define xmlSecTransformAes128CbcId              xmlSecGCryptTransformAes128CbcId
#define xmlSecTransformAes192CbcId              xmlSecGCryptTransformAes192CbcId
#define xmlSecTransformAes256CbcId              xmlSecGCryptTransformAes256CbcId
#define xmlSecTransformKWAes128Id               xmlSecGCryptTransformKWAes128Id
#define xmlSecTransformKWAes192Id               xmlSecGCryptTransformKWAes192Id
#define xmlSecTransformKWAes256Id               xmlSecGCryptTransformKWAes256Id
#define xmlSecTransformDes3CbcId                xmlSecGCryptTransformDes3CbcId
#define xmlSecTransformKWDes3Id                 xmlSecGCryptTransformKWDes3Id
#define xmlSecTransformDsaSha1Id                xmlSecGCryptTransformDsaSha1Id
#define xmlSecTransformHmacMd5Id                xmlSecGCryptTransformHmacMd5Id
#define xmlSecTransformHmacRipemd160Id          xmlSecGCryptTransformHmacRipemd160Id
#define xmlSecTransformHmacSha1Id               xmlSecGCryptTransformHmacSha1Id
#define xmlSecTransformRipemd160Id              xmlSecGCryptTransformRipemd160Id
#define xmlSecTransformRsaSha1Id                xmlSecGCryptTransformRsaSha1Id
#define xmlSecTransformRsaPkcs1Id               xmlSecGCryptTransformRsaPkcs1Id
#define xmlSecTransformRsaOaepId                xmlSecGCryptTransformRsaOaepId
#define xmlSecTransformSha1Id                   xmlSecGCryptTransformSha1Id

/********************************************************************
 *
 * High level routines form xmlsec command line utility
 *
 ********************************************************************/
#define xmlSecCryptoAppInit                     xmlSecGCryptAppInit
#define xmlSecCryptoAppShutdown                 xmlSecGCryptAppShutdown
#define xmlSecCryptoAppDefaultKeysMngrInit      xmlSecGCryptAppDefaultKeysMngrInit
#define xmlSecCryptoAppDefaultKeysMngrAdoptKey  xmlSecGCryptAppDefaultKeysMngrAdoptKey
#define xmlSecCryptoAppDefaultKeysMngrLoad      xmlSecGCryptAppDefaultKeysMngrLoad
#define xmlSecCryptoAppDefaultKeysMngrSave      xmlSecGCryptAppDefaultKeysMngrSave
#define xmlSecCryptoAppKeysMngrCertLoad         xmlSecGCryptAppKeysMngrCertLoad
#define xmlSecCryptoAppKeysMngrCertLoadMemory   xmlSecGCryptAppKeysMngrCertLoadMemory
#define xmlSecCryptoAppKeyLoad                  xmlSecGCryptAppKeyLoad
#define xmlSecCryptoAppPkcs12Load               xmlSecGCryptAppPkcs12Load
#define xmlSecCryptoAppKeyCertLoad              xmlSecGCryptAppKeyCertLoad
#define xmlSecCryptoAppKeyLoadMemory            xmlSecGCryptAppKeyLoadMemory
#define xmlSecCryptoAppPkcs12LoadMemory         xmlSecGCryptAppPkcs12LoadMemory
#define xmlSecCryptoAppKeyCertLoadMemory        xmlSecGCryptAppKeyCertLoadMemory
#define xmlSecCryptoAppGetDefaultPwdCallback    xmlSecGCryptAppGetDefaultPwdCallback

#endif /* XMLSEC_CRYPTO_GCRYPT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_GCRYPT_CRYPTO_H__ */

#define __XMLSEC_GCRYPT_CRYPTO_H__

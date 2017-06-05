/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_SKELETON_SYMBOLS_H__
#define __XMLSEC_SKELETON_SYMBOLS_H__

#if !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
#error To disable dynamic loading of xmlsec-crypto libraries undefine XMLSEC_CRYPTO_DYNAMIC_LOADING
#endif /* !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef XMLSEC_CRYPTO_SKELETON

/********************************************************************
 *
 * Crypto Init/shutdown
 *
 ********************************************************************/
#define xmlSecCryptoInit                        xmlSecSkeletonInit
#define xmlSecCryptoShutdown                    xmlSecSkeletonShutdown

#define xmlSecCryptoKeysMngrInit                xmlSecSkeletonKeysMngrInit

/********************************************************************
 *
 * Key data ids
 *
 ********************************************************************/
#define xmlSecKeyDataAesId                      xmlSecSkeletonKeyDataAesId
#define xmlSecKeyDataDesId                      xmlSecSkeletonKeyDataDesId
#define xmlSecKeyDataDsaId                      xmlSecSkeletonKeyDataDsaId
#define xmlSecKeyDataEcdsaId                    xmlSecSkeletonKeyDataEcdsaId
#define xmlSecKeyDataHmacId                     xmlSecSkeletonKeyDataHmacId
#define xmlSecKeyDataRsaId                      xmlSecSkeletonKeyDataRsaId
#define xmlSecKeyDataX509Id                     xmlSecSkeletonKeyDataX509Id
#define xmlSecKeyDataRawX509CertId              xmlSecSkeletonKeyDataRawX509CertId

/********************************************************************
 *
 * Key data store ids
 *
 ********************************************************************/
#define xmlSecX509StoreId                       xmlSecSkeletonX509StoreId

/********************************************************************
 *
 * Crypto transforms ids
 *
 ********************************************************************/
#define xmlSecTransformAes128CbcId              xmlSecSkeletonTransformAes128CbcId
#define xmlSecTransformAes192CbcId              xmlSecSkeletonTransformAes192CbcId
#define xmlSecTransformAes256CbcId              xmlSecSkeletonTransformAes256CbcId
#define xmlSecTransformKWAes128Id               xmlSecSkeletonTransformKWAes128Id
#define xmlSecTransformKWAes192Id               xmlSecSkeletonTransformKWAes192Id
#define xmlSecTransformKWAes256Id               xmlSecSkeletonTransformKWAes256Id
#define xmlSecTransformDes3CbcId                xmlSecSkeletonTransformDes3CbcId
#define xmlSecTransformKWDes3Id                 xmlSecSkeletonTransformKWDes3Id
#define xmlSecTransformDsaSha1Id                xmlSecSkeletonTransformDsaSha1Id
#define xmlSecTransformDsaSha256Id              xmlSecSkeletonTransformDsaSha256Id
#define xmlSecTransformEcdsaSha1Id              xmlSecSkeletonTransformEcdsaSha1Id
#define xmlSecTransformEcdsaSha224Id            xmlSecSkeletonTransformEcdsaSha224Id
#define xmlSecTransformEcdsaSha256Id            xmlSecSkeletonTransformEcdsaSha256Id
#define xmlSecTransformEcdsaSha384Id            xmlSecSkeletonTransformEcdsaSha384Id
#define xmlSecTransformEcdsaSha512Id            xmlSecSkeletonTransformEcdsaSha512Id
#define xmlSecTransformHmacMd5Id                xmlSecSkeletonTransformHmacMd5Id
#define xmlSecTransformHmacRipemd160Id          xmlSecSkeletonTransformHmacRipemd160Id
#define xmlSecTransformHmacSha1Id               xmlSecSkeletonTransformHmacSha1Id
#define xmlSecTransformHmacSha224Id             xmlSecSkeletonTransformHmacSha224Id
#define xmlSecTransformHmacSha256Id             xmlSecSkeletonTransformHmacSha256Id
#define xmlSecTransformHmacSha384Id             xmlSecSkeletonTransformHmacSha384Id
#define xmlSecTransformHmacSha512Id             xmlSecSkeletonTransformHmacSha512Id
#define xmlSecTransformMd5Id                    xmlSecSkeletonTransformMd5Id
#define xmlSecTransformRipemd160Id              xmlSecSkeletonTransformRipemd160Id
#define xmlSecTransformRsaSha1Id                xmlSecSkeletonTransformRsaSha1Id
#define xmlSecTransformRsaSha224Id              xmlSecSkeletonTransformRsaSha224Id
#define xmlSecTransformRsaSha256Id              xmlSecSkeletonTransformRsaSha256Id
#define xmlSecTransformRsaSha384Id              xmlSecSkeletonTransformRsaSha384Id
#define xmlSecTransformRsaSha512Id              xmlSecSkeletonTransformRsaSha512Id
#define xmlSecTransformRsaPkcs1Id               xmlSecSkeletonTransformRsaPkcs1Id
#define xmlSecTransformRsaOaepId                xmlSecSkeletonTransformRsaOaepId
#define xmlSecTransformSha1Id                   xmlSecSkeletonTransformSha1Id
#define xmlSecTransformSha224Id                 xmlSecSkeletonTransformSha224Id
#define xmlSecTransformSha256Id                 xmlSecSkeletonTransformSha256Id
#define xmlSecTransformSha384Id                 xmlSecSkeletonTransformSha384Id
#define xmlSecTransformSha512Id                 xmlSecSkeletonTransformSha512Id

/********************************************************************
 *
 * High level routines form xmlsec command line utility
 *
 ********************************************************************/
#define xmlSecCryptoAppInit                     xmlSecSkeletonAppInit
#define xmlSecCryptoAppShutdown                 xmlSecSkeletonAppShutdown
#define xmlSecCryptoAppDefaultKeysMngrInit      xmlSecSkeletonAppDefaultKeysMngrInit
#define xmlSecCryptoAppDefaultKeysMngrAdoptKey  xmlSecSkeletonAppDefaultKeysMngrAdoptKey
#define xmlSecCryptoAppDefaultKeysMngrLoad      xmlSecSkeletonAppDefaultKeysMngrLoad
#define xmlSecCryptoAppDefaultKeysMngrSave      xmlSecSkeletonAppDefaultKeysMngrSave
#define xmlSecCryptoAppKeysMngrCertLoad         xmlSecSkeletonAppKeysMngrCertLoad
#define xmlSecCryptoAppKeysMngrCertLoadMemory   xmlSecSkeletonAppKeysMngrCertLoadMemory
#define xmlSecCryptoAppKeyLoad                  xmlSecSkeletonAppKeyLoad
#define xmlSecCryptoAppPkcs12Load               xmlSecSkeletonAppPkcs12Load
#define xmlSecCryptoAppKeyCertLoad              xmlSecSkeletonAppKeyCertLoad
#define xmlSecCryptoAppKeyLoadMemory            xmlSecSkeletonAppKeyLoadMemory
#define xmlSecCryptoAppPkcs12LoadMemory         xmlSecSkeletonAppPkcs12LoadMemory
#define xmlSecCryptoAppKeyCertLoadMemory        xmlSecSkeletonAppKeyCertLoadMemory
#define xmlSecCryptoAppGetDefaultPwdCallback    xmlSecSkeletonAppGetDefaultPwdCallback

#endif /* XMLSEC_CRYPTO_SKELETON */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_SKELETON_CRYPTO_H__ */

#define __XMLSEC_SKELETON_CRYPTO_H__

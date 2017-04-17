/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 */
#ifndef __XMLSEC_MSCRYPTO_SYMBOLS_H__
#define __XMLSEC_MSCRYPTO_SYMBOLS_H__

#if !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
#error To disable dynamic loading of xmlsec-crypto libraries undefine XMLSEC_CRYPTO_DYNAMIC_LOADING
#endif /* !defined(IN_XMLSEC) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef XMLSEC_CRYPTO_MSCRYPTO

/********************************************************************
 *
 * Crypto Init/shutdown
 *
 ********************************************************************/
#define xmlSecCryptoInit                        xmlSecMSCryptoInit
#define xmlSecCryptoShutdown                    xmlSecMSCryptoShutdown

#define xmlSecCryptoKeysMngrInit                xmlSecMSCryptoKeysMngrInit

/********************************************************************
 *
 * Key data ids
 *
 ********************************************************************/
#define xmlSecKeyDataAesId                      xmlSecMSCryptoKeyDataAesId
#define xmlSecKeyDataDesId                      xmlSecMSCryptoKeyDataDesId
#define xmlSecKeyDataDsaId                      xmlSecMSCryptoKeyDataDsaId
#define xmlSecKeyDataGost2001Id                 xmlSecMSCryptoKeyDataGost2001Id
#define xmlSecKeyDataHmacId                     xmlSecMSCryptoKeyDataHmacId
#define xmlSecKeyDataRsaId                      xmlSecMSCryptoKeyDataRsaId
#define xmlSecKeyDataX509Id                     xmlSecMSCryptoKeyDataX509Id
#define xmlSecKeyDataRawX509CertId              xmlSecMSCryptoKeyDataRawX509CertId

/********************************************************************
 *
 * Key data store ids
 *
 ********************************************************************/
#define xmlSecX509StoreId                       xmlSecMSCryptoX509StoreId

/********************************************************************
 *
 * Crypto transforms ids
 *
 ********************************************************************/
#define xmlSecTransformAes128CbcId              xmlSecMSCryptoTransformAes128CbcId
#define xmlSecTransformAes192CbcId              xmlSecMSCryptoTransformAes192CbcId
#define xmlSecTransformAes256CbcId              xmlSecMSCryptoTransformAes256CbcId
#define xmlSecTransformKWAes128Id               xmlSecMSCryptoTransformKWAes128Id
#define xmlSecTransformKWAes192Id               xmlSecMSCryptoTransformKWAes192Id
#define xmlSecTransformKWAes256Id               xmlSecMSCryptoTransformKWAes256Id
#define xmlSecTransformDes3CbcId                xmlSecMSCryptoTransformDes3CbcId
#define xmlSecTransformKWDes3Id                 xmlSecMSCryptoTransformKWDes3Id
#define xmlSecTransformDsaSha1Id                xmlSecMSCryptoTransformDsaSha1Id
#define xmlSecTransformGost2001GostR3411_94Id   xmlSecMSCryptoTransformGost2001GostR3411_94Id
#define xmlSecTransformHmacMd5Id                xmlSecMSCryptoTransformHmacMd5Id
#define xmlSecTransformHmacRipemd160Id          xmlSecMSCryptoTransformHmacRipemd160Id
#define xmlSecTransformHmacSha1Id               xmlSecMSCryptoTransformHmacSha1Id
#define xmlSecTransformRipemd160Id              xmlSecMSCryptoTransformRipemd160Id
#define xmlSecTransformRsaSha1Id                xmlSecMSCryptoTransformRsaSha1Id
#define xmlSecTransformRsaSha256Id              xmlSecMSCryptoTransformRsaSha256Id
#define xmlSecTransformRsaSha384Id              xmlSecMSCryptoTransformRsaSha384Id
#define xmlSecTransformRsaSha512Id              xmlSecMSCryptoTransformRsaSha512Id
#define xmlSecTransformRsaPkcs1Id               xmlSecMSCryptoTransformRsaPkcs1Id
#define xmlSecTransformRsaOaepId                xmlSecMSCryptoTransformRsaOaepId
#define xmlSecTransformSha1Id                   xmlSecMSCryptoTransformSha1Id
#define xmlSecTransformSha256Id                 xmlSecMSCryptoTransformSha256Id
#define xmlSecTransformSha384Id                 xmlSecMSCryptoTransformSha384Id
#define xmlSecTransformSha512Id                 xmlSecMSCryptoTransformSha512Id
#define xmlSecTransformGostR3411_94Id           xmlSecMSCryptoTransformGostR3411_94Id

/********************************************************************
 *
 * High level routines form xmlsec command line utility
 *
 ********************************************************************/
#define xmlSecCryptoAppInit                     xmlSecMSCryptoAppInit
#define xmlSecCryptoAppShutdown                 xmlSecMSCryptoAppShutdown
#define xmlSecCryptoAppDefaultKeysMngrInit      xmlSecMSCryptoAppDefaultKeysMngrInit
#define xmlSecCryptoAppDefaultKeysMngrAdoptKey  xmlSecMSCryptoAppDefaultKeysMngrAdoptKey
#define xmlSecCryptoAppDefaultKeysMngrLoad      xmlSecMSCryptoAppDefaultKeysMngrLoad
#define xmlSecCryptoAppDefaultKeysMngrSave      xmlSecMSCryptoAppDefaultKeysMngrSave
#define xmlSecCryptoAppKeysMngrCertLoad         xmlSecMSCryptoAppKeysMngrCertLoad
#define xmlSecCryptoAppKeysMngrCertLoadMemory   xmlSecMSCryptoAppKeysMngrCertLoadMemory
#define xmlSecCryptoAppKeyLoad                  xmlSecMSCryptoAppKeyLoad
#define xmlSecCryptoAppPkcs12Load               xmlSecMSCryptoAppPkcs12Load
#define xmlSecCryptoAppKeyCertLoad              xmlSecMSCryptoAppKeyCertLoad
#define xmlSecCryptoAppKeyLoadMemory            xmlSecMSCryptoAppKeyLoadMemory
#define xmlSecCryptoAppPkcs12LoadMemory         xmlSecMSCryptoAppPkcs12LoadMemory
#define xmlSecCryptoAppKeyCertLoadMemory        xmlSecMSCryptoAppKeyCertLoadMemory
#define xmlSecCryptoAppGetDefaultPwdCallback    xmlSecMSCryptoAppGetDefaultPwdCallback

#endif /* XMLSEC_CRYPTO_MSCRYPTO */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_MSCRYPTO_CRYPTO_H__ */

#define __XMLSEC_MSCRYPTO_CRYPTO_H__


/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 */
#ifndef __XMLSEC_MSCRYPTO_X509_H__
#define __XMLSEC_MSCRYPTO_X509_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef XMLSEC_NO_X509

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>

#include <windows.h>
#include <wincrypt.h>

/**
 * xmlSecMSCryptoKeyDataX509Id:
 *
 * The MSCrypto X509 data klass.
 */
#define xmlSecMSCryptoKeyDataX509Id \
        xmlSecMSCryptoKeyDataX509GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataX509GetKlass(void);

XMLSEC_CRYPTO_EXPORT PCCERT_CONTEXT     xmlSecMSCryptoKeyDataX509GetKeyCert     (xmlSecKeyDataPtr data);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeyDataX509AdoptKeyCert   (xmlSecKeyDataPtr data,
                                                                                PCCERT_CONTEXT cert);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeyDataX509AdoptCert      (xmlSecKeyDataPtr data,
                                                                                 PCCERT_CONTEXT cert);
XMLSEC_CRYPTO_EXPORT PCCERT_CONTEXT     xmlSecMSCryptoKeyDataX509GetCert        (xmlSecKeyDataPtr data,
                                                                                 xmlSecSize pos);
XMLSEC_CRYPTO_EXPORT xmlSecSize         xmlSecMSCryptoKeyDataX509GetCertsSize   (xmlSecKeyDataPtr data);

XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeyDataX509AdoptCrl       (xmlSecKeyDataPtr data,
                                                                                 PCCRL_CONTEXT crl);
XMLSEC_CRYPTO_EXPORT PCCRL_CONTEXT      xmlSecMSCryptoKeyDataX509GetCrl         (xmlSecKeyDataPtr data,
                                                                                 xmlSecSize pos);
XMLSEC_CRYPTO_EXPORT xmlSecSize         xmlSecMSCryptoKeyDataX509GetCrlsSize    (xmlSecKeyDataPtr data);


/**
 * xmlSecMSCryptoKeyDataRawX509CertId:
 *
 * The MSCrypto raw X509 certificate klass.
 */
#define xmlSecMSCryptoKeyDataRawX509CertId \
        xmlSecMSCryptoKeyDataRawX509CertGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecMSCryptoKeyDataRawX509CertGetKlass(void);

/**
 * xmlSecMSCryptoX509StoreId:
 *
 * The MSCrypto X509 store klass.
 */
#define xmlSecMSCryptoX509StoreId \
        xmlSecMSCryptoX509StoreGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataStoreId xmlSecMSCryptoX509StoreGetKlass(void);

XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoX509StoreAdoptCert        (xmlSecKeyDataStorePtr store,
                                                                                 PCCERT_CONTEXT cert,
                                                                                 xmlSecKeyDataType type);

XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoX509StoreAdoptKeyStore    (xmlSecKeyDataStorePtr store,
                                                                                 HCERTSTORE keyStore);

XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoX509StoreAdoptTrustedStore(xmlSecKeyDataStorePtr store,
                                                                                 HCERTSTORE trustedStore);

XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoX509StoreAdoptUntrustedStore(xmlSecKeyDataStorePtr store,
                                                                                 HCERTSTORE untrustedStore);

XMLSEC_CRYPTO_EXPORT void               xmlSecMSCryptoX509StoreEnableSystemTrustedCerts(xmlSecKeyDataStorePtr store,
                                                                                 int val);



#endif /* XMLSEC_NO_X509 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_MSCRYPTO_X509_H__ */

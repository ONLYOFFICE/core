/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#ifndef __XMLSEC_NSS_X509_H__
#define __XMLSEC_NSS_X509_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef XMLSEC_NO_X509

#include <nspr.h>
#include <nss.h>
#include <cert.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>

/**
 * xmlSecNssKeyDataX509Id:
 *
 * The NSS X509 data klass.
 */
#define xmlSecNssKeyDataX509Id \
        xmlSecNssKeyDataX509GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataX509GetKlass(void);

XMLSEC_CRYPTO_EXPORT CERTCertificate*   xmlSecNssKeyDataX509GetKeyCert(xmlSecKeyDataPtr data);
XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeyDataX509AdoptKeyCert(xmlSecKeyDataPtr data,
                                                                         CERTCertificate* cert);

XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeyDataX509AdoptCert(xmlSecKeyDataPtr data,
                                                                         CERTCertificate* cert);
XMLSEC_CRYPTO_EXPORT CERTCertificate*   xmlSecNssKeyDataX509GetCert     (xmlSecKeyDataPtr data,
                                                                         xmlSecSize pos);
XMLSEC_CRYPTO_EXPORT xmlSecSize         xmlSecNssKeyDataX509GetCertsSize(xmlSecKeyDataPtr data);

XMLSEC_CRYPTO_EXPORT int                xmlSecNssKeyDataX509AdoptCrl(xmlSecKeyDataPtr data,
                                                                         CERTSignedCrl* crl);
XMLSEC_CRYPTO_EXPORT CERTSignedCrl*     xmlSecNssKeyDataX509GetCrl      (xmlSecKeyDataPtr data,
                                                                         xmlSecSize pos);
XMLSEC_CRYPTO_EXPORT xmlSecSize         xmlSecNssKeyDataX509GetCrlsSize(xmlSecKeyDataPtr data);
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataPtr   xmlSecNssX509CertGetKey         (CERTCertificate* cert);


/**
 * xmlSecNssKeyDataRawX509CertId:
 *
 * The NSS raw X509 certificate klass.
 */
#define xmlSecNssKeyDataRawX509CertId \
        xmlSecNssKeyDataRawX509CertGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecNssKeyDataRawX509CertGetKlass(void);

/**
 * xmlSecNssX509StoreId:
 *
 * The NSS X509 store klass.
 */
#define xmlSecNssX509StoreId \
        xmlSecNssX509StoreGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataStoreId xmlSecNssX509StoreGetKlass(void);
XMLSEC_CRYPTO_EXPORT CERTCertificate*           xmlSecNssX509StoreFindCert      (xmlSecKeyDataStorePtr store,
                                                                                 xmlChar *subjectName,
                                                                                 xmlChar *issuerName,
                                                                                 xmlChar *issuerSerial,
                                                                                 xmlChar *ski,
                                                                                 xmlSecKeyInfoCtx* keyInfoCtx);

XMLSEC_CRYPTO_EXPORT CERTCertificate*           xmlSecNssX509StoreVerify        (xmlSecKeyDataStorePtr store,
                                                                                 CERTCertList* certs,
                                                                                 xmlSecKeyInfoCtx* keyInfoCtx);
XMLSEC_CRYPTO_EXPORT int                        xmlSecNssX509StoreAdoptCert (xmlSecKeyDataStorePtr store,
                                                                             CERTCertificate* cert,
                                                                             xmlSecKeyDataType type);


#endif /* XMLSEC_NO_X509 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_NSS_X509_H__ */

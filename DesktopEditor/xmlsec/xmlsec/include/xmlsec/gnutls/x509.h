/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GNUTLS_X509_H__
#define __XMLSEC_GNUTLS_X509_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef XMLSEC_NO_X509

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>


/**************************************************************************
 *
 * X509 Key Data
 *
 *****************************************************************************/
/**
 * xmlSecGnuTLSKeyDataX509Id:
 *
 * The GnuTLS X509 data klass.
 */
#define xmlSecGnuTLSKeyDataX509Id \
        xmlSecGnuTLSKeyDataX509GetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataX509GetKlass         (void);


XMLSEC_CRYPTO_EXPORT gnutls_x509_crt_t  xmlSecGnuTLSKeyDataX509GetKeyCert       (xmlSecKeyDataPtr data);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataX509AdoptKeyCert     (xmlSecKeyDataPtr data,
                                                                                 gnutls_x509_crt_t cert);

XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataX509AdoptCert        (xmlSecKeyDataPtr data,
                                                                                 gnutls_x509_crt_t cert);
XMLSEC_CRYPTO_EXPORT gnutls_x509_crt_t  xmlSecGnuTLSKeyDataX509GetCert          (xmlSecKeyDataPtr data,
                                                                                 xmlSecSize pos);
XMLSEC_CRYPTO_EXPORT xmlSecSize         xmlSecGnuTLSKeyDataX509GetCertsSize     (xmlSecKeyDataPtr data);

XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSKeyDataX509AdoptCrl         (xmlSecKeyDataPtr data,
                                                                                 gnutls_x509_crl_t crl);
XMLSEC_CRYPTO_EXPORT gnutls_x509_crl_t  xmlSecGnuTLSKeyDataX509GetCrl           (xmlSecKeyDataPtr data,
                                                                                 xmlSecSize pos);
XMLSEC_CRYPTO_EXPORT xmlSecSize         xmlSecGnuTLSKeyDataX509GetCrlsSize      (xmlSecKeyDataPtr data);


XMLSEC_CRYPTO_EXPORT xmlSecKeyDataPtr   xmlSecGnuTLSX509CertGetKey              (gnutls_x509_crt_t cert);

/**************************************************************************
 *
 * X509 raw cert
 *
 *****************************************************************************/
/**
 * xmlSecGnuTLSKeyDataRawX509CertId:
 *
 * The GnuTLS raw X509 certificate klass.
 */
#define xmlSecGnuTLSKeyDataRawX509CertId \
        xmlSecGnuTLSKeyDataRawX509CertGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataId    xmlSecGnuTLSKeyDataRawX509CertGetKlass  (void);

/**************************************************************************
 *
 * X509 certs store
 *
 *****************************************************************************/
/**
 * xmlSecGnuTLSX509StoreId:
 *
 * The GnuTLS X509 store klass.
 */
#define xmlSecGnuTLSX509StoreId \
        xmlSecGnuTLSX509StoreGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyDataStoreId xmlSecGnuTLSX509StoreGetKlass (void);
XMLSEC_CRYPTO_EXPORT gnutls_x509_crt_t  xmlSecGnuTLSX509StoreFindCert   (const xmlSecKeyDataStorePtr store,
                                                                         const xmlChar *subjectName,
                                                                         const xmlChar *issuerName,
                                                                         const xmlChar *issuerSerial,
                                                                         const xmlChar *ski,
                                                                         const xmlSecKeyInfoCtx* keyInfoCtx);
XMLSEC_CRYPTO_EXPORT gnutls_x509_crt_t  xmlSecGnuTLSX509StoreVerify     (xmlSecKeyDataStorePtr store,
                                                                         xmlSecPtrListPtr certs,
                                                                         xmlSecPtrListPtr crls,
                                                                         const xmlSecKeyInfoCtx* keyInfoCtx);
XMLSEC_CRYPTO_EXPORT int                xmlSecGnuTLSX509StoreAdoptCert  (xmlSecKeyDataStorePtr store,
                                                                         gnutls_x509_crt_t cert,
                                                                         xmlSecKeyDataType type);




#endif /* XMLSEC_NO_X509 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_GNUTLS_X509_H__ */

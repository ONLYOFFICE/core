/*
 * XML Security Library
 *
 * THIS IS A PRIVATE XMLSEC HEADER FILE
 * DON'T USE IT IN YOUR APPLICATION
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GNUTLS_X509UTILS_H__
#define __XMLSEC_GNUTLS_X509UTILS_H__

#ifndef XMLSEC_PRIVATE
#error "gnutls/x509utils.h file contains private xmlsec definitions and should not be used outside xmlsec or xmlsec-$crypto libraries"
#endif /* XMLSEC_PRIVATE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef XMLSEC_NO_X509

/**************************************************************************
 *
 * X509 certs list
 *
 *****************************************************************************/
#define xmlSecGnuTLSX509CrtListId   \
        xmlSecGnuTLSX509CrtListGetKlass()
xmlSecPtrListId         xmlSecGnuTLSX509CrtListGetKlass         (void);

/**************************************************************************
 *
 * X509 crls list
 *
 *****************************************************************************/
#define xmlSecGnuTLSX509CrlListId   \
        xmlSecGnuTLSX509CrlListGetKlass()
xmlSecPtrListId         xmlSecGnuTLSX509CrlListGetKlass         (void);

/*************************************************************************
 *
 * x509 certs utils/helpers
 *
 ************************************************************************/
gnutls_x509_crt_t       xmlSecGnuTLSX509CertDup                 (gnutls_x509_crt_t src);
xmlChar *               xmlSecGnuTLSX509CertGetSubjectDN        (gnutls_x509_crt_t cert);
xmlChar *               xmlSecGnuTLSX509CertGetIssuerDN         (gnutls_x509_crt_t cert);
xmlChar *               xmlSecGnuTLSX509CertGetIssuerSerial     (gnutls_x509_crt_t cert);
xmlChar *               xmlSecGnuTLSX509CertGetSKI              (gnutls_x509_crt_t cert);
gnutls_x509_crt_t       xmlSecGnuTLSX509CertRead                (const xmlSecByte* buf,
                                                                 xmlSecSize size,
                                                                 xmlSecKeyDataFormat format);
gnutls_x509_crt_t       xmlSecGnuTLSX509CertBase64DerRead       (xmlChar* buf);
xmlChar*                xmlSecGnuTLSX509CertBase64DerWrite      (gnutls_x509_crt_t cert,
                                                                 int base64LineWrap);
void                    xmlSecGnuTLSX509CertDebugDump           (gnutls_x509_crt_t cert,
                                                                 FILE* output);
void                    xmlSecGnuTLSX509CertDebugXmlDump        (gnutls_x509_crt_t cert,
                                                                 FILE* output);

/*************************************************************************
 *
 * x509 crls utils/helpers
 *
 ************************************************************************/
gnutls_x509_crl_t       xmlSecGnuTLSX509CrlDup                  (gnutls_x509_crl_t src);
xmlChar *               xmlSecGnuTLSX509CrlGetIssuerDN          (gnutls_x509_crl_t crl);
gnutls_x509_crl_t       xmlSecGnuTLSX509CrlRead                 (const xmlSecByte* buf,
                                                                 xmlSecSize size,
                                                                 xmlSecKeyDataFormat format);
gnutls_x509_crl_t       xmlSecGnuTLSX509CrlBase64DerRead        (xmlChar* buf);
xmlChar*                xmlSecGnuTLSX509CrlBase64DerWrite       (gnutls_x509_crl_t crl,
                                                                 int base64LineWrap);
void                    xmlSecGnuTLSX509CrlDebugDump            (gnutls_x509_crl_t crl,
                                                                 FILE* output);
void                    xmlSecGnuTLSX509CrlDebugXmlDump         (gnutls_x509_crl_t crl,
                                                                 FILE* output);

/*************************************************************************
 *
 * Misc. utils/helpers
 *
 ************************************************************************/
xmlChar*                xmlSecGnuTLSASN1IntegerWrite            (const unsigned char * data, 
                                                                 size_t len);



/*************************************************************************
 *
 * pkcs12 utils/helpers
 *
 ************************************************************************/
int                     xmlSecGnuTLSPkcs12LoadMemory            (const xmlSecByte* data,
                                                                 xmlSecSize dataSize,
                                                                 const char *pwd,
                                                                 gnutls_x509_privkey_t * priv_key,
                                                                 gnutls_x509_crt_t * key_cert,
                                                                 xmlSecPtrListPtr certsList);

/*************************************************************************
 *
 * keydata utils/helpers
 *
 ************************************************************************/
xmlSecKeyDataPtr        xmlSecGnuTLSCreateKeyDataAndAdoptPrivKey(gnutls_x509_privkey_t priv_key);


/*************************************************************************
 *
 * LDAP DN parser
 *
 ************************************************************************/
typedef struct _xmlSecGnuTLSDnAttr {
    xmlChar * key;
    xmlChar * value;
} xmlSecGnuTLSDnAttr;

void                    xmlSecGnuTLSDnAttrsInitialize           (xmlSecGnuTLSDnAttr * attrs,
                                                                 xmlSecSize attrsSize);
void                    xmlSecGnuTLSDnAttrsDeinitialize         (xmlSecGnuTLSDnAttr * attrs,
                                                                 xmlSecSize attrsSize);
const xmlSecGnuTLSDnAttr * xmlSecGnuTLSDnAttrrsFind             (const xmlSecGnuTLSDnAttr * attrs,
                                                                 xmlSecSize attrsSize,
                                                                 const xmlChar * key);
int                     xmlSecGnuTLSDnAttrsEqual                (const xmlSecGnuTLSDnAttr * ll,
                                                                 xmlSecSize llSize,
                                                                 const xmlSecGnuTLSDnAttr * rr,
                                                                 xmlSecSize rrSize);
int                     xmlSecGnuTLSDnAttrsParse                (const xmlChar * dn,
                                                                 xmlSecGnuTLSDnAttr * attrs,
                                                                 xmlSecSize attrsSize);
#endif /* XMLSEC_NO_X509 */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* ! __XMLSEC_GNUTLS_X509UTILS_H__ */

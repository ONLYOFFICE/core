/**
 * XMLSec library
 *
 * X509 support
 *
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_X509

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <prmem.h>
#include <pratom.h>
#include <keyhi.h>
#include <cert.h>
#include <certdb.h>
#include <pk11func.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/x509.h>
#include <xmlsec/base64.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/x509.h>
#include <xmlsec/nss/pkikeys.h>


/* workaround - NSS exports this but doesn't declare it */
extern CERTCertificate * __CERT_NewTempCertificate(CERTCertDBHandle *handle,
                                                   SECItem *derCert,
                                                   char *nickname,
                                                   PRBool isperm,
                                                   PRBool copyDER);

/*************************************************************************
 *
 * X509 utility functions
 *
 ************************************************************************/
static int              xmlSecNssX509DataNodeRead               (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509CertificateNodeRead        (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509CertificateNodeWrite       (CERTCertificate* cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509SubjectNameNodeRead        (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509SubjectNameNodeWrite       (CERTCertificate* cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509IssuerSerialNodeRead       (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509IssuerSerialNodeWrite      (CERTCertificate* cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509SKINodeRead                (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509SKINodeWrite               (CERTCertificate* cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509CRLNodeRead                (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssX509CRLNodeWrite               (CERTSignedCrl* crl,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssKeyDataX509VerifyAndExtractKey(xmlSecKeyDataPtr data,
                                                                xmlSecKeyPtr key,
                                                                xmlSecKeyInfoCtxPtr keyInfoCtx);

static CERTCertificate* xmlSecNssX509CertDerRead                (const xmlSecByte* buf,
                                                                 xmlSecSize size);
static CERTCertificate* xmlSecNssX509CertBase64DerRead          (xmlChar* buf);
static xmlChar*         xmlSecNssX509CertBase64DerWrite         (CERTCertificate* cert,
                                                                 int base64LineWrap);
static CERTSignedCrl*   xmlSecNssX509CrlDerRead                 (xmlSecByte* buf,
                                                                 xmlSecSize size,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static CERTSignedCrl*   xmlSecNssX509CrlBase64DerRead           (xmlChar* buf,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static xmlChar*         xmlSecNssX509CrlBase64DerWrite          (CERTSignedCrl* crl,
                                                                 int base64LineWrap);
static xmlChar*         xmlSecNssX509NameWrite                  (CERTName* nm);
static xmlChar*         xmlSecNssASN1IntegerWrite               (SECItem *num);
static xmlChar*         xmlSecNssX509SKIWrite                   (CERTCertificate* cert);
static void             xmlSecNssX509CertDebugDump              (CERTCertificate* cert,
                                                                 FILE* output);
static void             xmlSecNssX509CertDebugXmlDump           (CERTCertificate* cert,
                                                                 FILE* output);
static int              xmlSecNssX509CertGetTime                (PRTime* t,
                                                                 time_t* res);

/*************************************************************************
 *
 * Internal NSS X509 data CTX
 *
 ************************************************************************/
typedef struct _xmlSecNssX509DataCtx            xmlSecNssX509DataCtx,
                                                *xmlSecNssX509DataCtxPtr;
typedef struct _xmlSecNssX509CrlNode            xmlSecNssX509CrlNode,
                                                *xmlSecNssX509CrlNodePtr;
struct _xmlSecNssX509CrlNode {
    xmlSecNssX509CrlNodePtr  next;
    CERTSignedCrl           *crl;
};

struct _xmlSecNssX509DataCtx {
    CERTCertificate*  keyCert;

    CERTCertList*    certsList;
    unsigned int     numCerts;

    xmlSecNssX509CrlNodePtr crlsList;
    unsigned int     numCrls;
};

/**************************************************************************
 *
 * <dsig:X509Data> processing
 *
 *
 * The X509Data  Element (http://www.w3.org/TR/xmldsig-core/#sec-X509Data)
 *
 * An X509Data element within KeyInfo contains one or more identifiers of keys
 * or X509 certificates (or certificates' identifiers or a revocation list).
 * The content of X509Data is:
 *
 *  1. At least one element, from the following set of element types; any of these may appear together or more than once iff (if and only if) each instance describes or is related to the same certificate:
 *  2.
 *    * The X509IssuerSerial element, which contains an X.509 issuer
 *      distinguished name/serial number pair that SHOULD be compliant
 *      with RFC2253 [LDAP-DN],
 *    * The X509SubjectName element, which contains an X.509 subject
 *      distinguished name that SHOULD be compliant with RFC2253 [LDAP-DN],
 *    * The X509SKI element, which contains the base64 encoded plain (i.e.
 *      non-DER-encoded) value of a X509 V.3 SubjectKeyIdentifier extension.
 *    * The X509Certificate element, which contains a base64-encoded [X509v3]
 *      certificate, and
 *    * Elements from an external namespace which accompanies/complements any
 *      of the elements above.
 *    * The X509CRL element, which contains a base64-encoded certificate
 *      revocation list (CRL) [X509v3].
 *
 * Any X509IssuerSerial, X509SKI, and X509SubjectName elements that appear
 * MUST refer to the certificate or certificates containing the validation key.
 * All such elements that refer to a particular individual certificate MUST be
 * grouped inside a single X509Data element and if the certificate to which
 * they refer appears, it MUST also be in that X509Data element.
 *
 * Any X509IssuerSerial, X509SKI, and X509SubjectName elements that relate to
 * the same key but different certificates MUST be grouped within a single
 * KeyInfo but MAY occur in multiple X509Data elements.
 *
 * All certificates appearing in an X509Data element MUST relate to the
 * validation key by either containing it or being part of a certification
 * chain that terminates in a certificate containing the validation key.
 *
 * No ordering is implied by the above constraints.
 *
 * Note, there is no direct provision for a PKCS#7 encoded "bag" of
 * certificates or CRLs. However, a set of certificates and CRLs can occur
 * within an X509Data element and multiple X509Data elements can occur in a
 * KeyInfo. Whenever multiple certificates occur in an X509Data element, at
 * least one such certificate must contain the public key which verifies the
 * signature.
 *
 * Schema Definition
 *
 *  <element name="X509Data" type="ds:X509DataType"/>
 *  <complexType name="X509DataType">
 *    <sequence maxOccurs="unbounded">
 *      <choice>
 *        <element name="X509IssuerSerial" type="ds:X509IssuerSerialType"/>
 *        <element name="X509SKI" type="base64Binary"/>
 *        <element name="X509SubjectName" type="string"/>
 *        <element name="X509Certificate" type="base64Binary"/>
 *        <element name="X509CRL" type="base64Binary"/>
 *        <any namespace="##other" processContents="lax"/>
 *      </choice>
 *    </sequence>
 *  </complexType>
 *  <complexType name="X509IssuerSerialType">
 *    <sequence>
 *       <element name="X509IssuerName" type="string"/>
 *       <element name="X509SerialNumber" type="integer"/>
 *     </sequence>
 *  </complexType>
 *
 *  DTD
 *
 *    <!ELEMENT X509Data ((X509IssuerSerial | X509SKI | X509SubjectName |
 *                          X509Certificate | X509CRL)+ %X509.ANY;)>
 *    <!ELEMENT X509IssuerSerial (X509IssuerName, X509SerialNumber) >
 *    <!ELEMENT X509IssuerName (#PCDATA) >
 *    <!ELEMENT X509SubjectName (#PCDATA) >
 *    <!ELEMENT X509SerialNumber (#PCDATA) >
 *    <!ELEMENT X509SKI (#PCDATA) >
 *    <!ELEMENT X509Certificate (#PCDATA) >
 *    <!ELEMENT X509CRL (#PCDATA) >
 *
 * -----------------------------------------------------------------------
 *
 * xmlSecNssX509DataCtx is located after xmlSecTransform
 *
 *************************************************************************/
#define xmlSecNssX509DataSize   \
    (sizeof(xmlSecKeyData) + sizeof(xmlSecNssX509DataCtx))
#define xmlSecNssX509DataGetCtx(data) \
    ((xmlSecNssX509DataCtxPtr)(((xmlSecByte*)(data)) + sizeof(xmlSecKeyData)))

static int              xmlSecNssKeyDataX509Initialize  (xmlSecKeyDataPtr data);
static int              xmlSecNssKeyDataX509Duplicate   (xmlSecKeyDataPtr dst,
                                                         xmlSecKeyDataPtr src);
static void             xmlSecNssKeyDataX509Finalize    (xmlSecKeyDataPtr data);
static int              xmlSecNssKeyDataX509XmlRead     (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssKeyDataX509XmlWrite    (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static xmlSecKeyDataType xmlSecNssKeyDataX509GetType    (xmlSecKeyDataPtr data);
static const xmlChar* xmlSecNssKeyDataX509GetIdentifier (xmlSecKeyDataPtr data);

static void             xmlSecNssKeyDataX509DebugDump   (xmlSecKeyDataPtr data,
                                                         FILE* output);
static void             xmlSecNssKeyDataX509DebugXmlDump(xmlSecKeyDataPtr data,
                                                         FILE* output);



static xmlSecKeyDataKlass xmlSecNssKeyDataX509Klass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecNssX509DataSize,

    /* data */
    xmlSecNameX509Data,
    xmlSecKeyDataUsageKeyInfoNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefX509Data,                         /* const xmlChar* href; */
    xmlSecNodeX509Data,                         /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                               /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecNssKeyDataX509Initialize,             /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecNssKeyDataX509Duplicate,              /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecNssKeyDataX509Finalize,               /* xmlSecKeyDataFinalizeMethod finalize; */
    NULL,                                       /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecNssKeyDataX509GetType,                /* xmlSecKeyDataGetTypeMethod getType; */
    NULL,                                       /* xmlSecKeyDataGetSizeMethod getSize; */
    xmlSecNssKeyDataX509GetIdentifier,          /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecNssKeyDataX509XmlRead,                /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecNssKeyDataX509XmlWrite,               /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    NULL,                                       /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                                       /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecNssKeyDataX509DebugDump,              /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecNssKeyDataX509DebugXmlDump,           /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataX509GetKlass:
 *
 * The NSS X509 key data klass (http://www.w3.org/TR/xmldsig-core/#sec-X509Data).
 *
 * Returns: the X509 data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataX509GetKlass(void) {
    return(&xmlSecNssKeyDataX509Klass);
}

/**
 * xmlSecNssKeyDataX509GetKeyCert:
 * @data:               the pointer to X509 key data.
 *
 * Gets the certificate from which the key was extracted.
 *
 * Returns: the key's certificate or NULL if key data was not used for key
 * extraction or an error occurs.
 */
CERTCertificate*
xmlSecNssKeyDataX509GetKeyCert(xmlSecKeyDataPtr data) {
    xmlSecNssX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), NULL);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    return(ctx->keyCert);
}

/**
 * xmlSecNssKeyDataX509AdoptKeyCert:
 * @data:               the pointer to X509 key data.
 * @cert:               the pointer to NSS X509 certificate.
 *
 * Sets the key's certificate in @data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssKeyDataX509AdoptKeyCert(xmlSecKeyDataPtr data, CERTCertificate* cert) {
    xmlSecNssX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(cert != NULL, -1);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    if(ctx->keyCert != NULL) {
        CERT_DestroyCertificate(ctx->keyCert);
    }
    ctx->keyCert = cert;
    return(0);
}

/**
 * xmlSecNssKeyDataX509AdoptCert:
 * @data:               the pointer to X509 key data.
 * @cert:               the pointer to NSS X509 certificate.
 *
 * Adds certificate to the X509 key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssKeyDataX509AdoptCert(xmlSecKeyDataPtr data, CERTCertificate* cert) {
    xmlSecNssX509DataCtxPtr ctx;
    SECStatus ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(cert != NULL, -1);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    if(ctx->certsList == NULL) {
        ctx->certsList = CERT_NewCertList();
        if(ctx->certsList == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "CERT_NewCertList",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            return(-1);
        }
    }

    ret = CERT_AddCertToListTail(ctx->certsList, cert);
    if(ret != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "CERT_AddCertToListTail",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        return(-1);
    }
    ctx->numCerts++;

    return(0);
}

/**
 * xmlSecNssKeyDataX509GetCert:
 * @data:               the pointer to X509 key data.
 * @pos:                the desired certificate position.
 *
 * Gets a certificate from X509 key data.
 *
 * Returns: the pointer to certificate or NULL if @pos is larger than the
 * number of certificates in @data or an error occurs.
 */
CERTCertificate*
xmlSecNssKeyDataX509GetCert(xmlSecKeyDataPtr data, xmlSecSize pos) {
    xmlSecNssX509DataCtxPtr ctx;
    CERTCertListNode*       head;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), NULL);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);
    xmlSecAssert2(ctx->certsList != NULL, NULL);
    xmlSecAssert2(pos < ctx->numCerts, NULL);

    head = CERT_LIST_HEAD(ctx->certsList);
    while (pos > 0)
    {
        head = CERT_LIST_NEXT(head);
        pos--;
    }

    return (head->cert);
}

/**
 * xmlSecNssKeyDataX509GetCertsSize:
 * @data:               the pointer to X509 key data.
 *
 * Gets the number of certificates in @data.
 *
 * Returns: te number of certificates in @data.
 */
xmlSecSize
xmlSecNssKeyDataX509GetCertsSize(xmlSecKeyDataPtr data) {
    xmlSecNssX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), 0);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, 0);

    return(ctx->numCerts);
}

/**
 * xmlSecNssKeyDataX509AdoptCrl:
 * @data:               the pointer to X509 key data.
 * @crl:                the pointer to NSS X509 CRL.
 *
 * Adds CRL to the X509 key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssKeyDataX509AdoptCrl(xmlSecKeyDataPtr data, CERTSignedCrl* crl) {
    xmlSecNssX509DataCtxPtr ctx;
    xmlSecNssX509CrlNodePtr crlnode;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(crl != NULL, -1);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    crlnode = (xmlSecNssX509CrlNodePtr)PR_Malloc(sizeof(xmlSecNssX509CrlNode));

    if(crlnode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "PR_Malloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    memset(crlnode, 0, sizeof(xmlSecNssX509CrlNode));
    crlnode->next = ctx->crlsList;
    crlnode->crl = crl;
    ctx->crlsList = crlnode;
    ctx->numCrls++;

    return(0);
}

/**
 * xmlSecNssKeyDataX509GetCrl:
 * @data:               the pointer to X509 key data.
 * @pos:                the desired CRL position.
 *
 * Gets a CRL from X509 key data.
 *
 * Returns: the pointer to CRL or NULL if @pos is larger than the
 * number of CRLs in @data or an error occurs.
 */
CERTSignedCrl *
xmlSecNssKeyDataX509GetCrl(xmlSecKeyDataPtr data, xmlSecSize pos) {
    xmlSecNssX509DataCtxPtr ctx;
    xmlSecNssX509CrlNodePtr head;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), NULL);
    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    xmlSecAssert2(ctx->crlsList != NULL, NULL);
    xmlSecAssert2(pos < ctx->numCrls, NULL);

    head = ctx->crlsList;
    while (pos > 0)
    {
        head = head->next;
        pos--;
    }

    return (head->crl);
}

/**
 * xmlSecNssKeyDataX509GetCrlsSize:
 * @data:               the pointer to X509 key data.
 *
 * Gets the number of CRLs in @data.
 *
 * Returns: te number of CRLs in @data.
 */
xmlSecSize
xmlSecNssKeyDataX509GetCrlsSize(xmlSecKeyDataPtr data) {
    xmlSecNssX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), 0);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, 0);

    return(ctx->numCrls);
}

static int
xmlSecNssKeyDataX509Initialize(xmlSecKeyDataPtr data) {
    xmlSecNssX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecNssX509DataCtx));
    return(0);
}

static int
xmlSecNssKeyDataX509Duplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    CERTCertificate* certSrc;
    CERTCertificate* certDst;
    CERTSignedCrl* crlSrc;
    CERTSignedCrl* crlDst;
    xmlSecSize size, pos;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(dst, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(src, xmlSecNssKeyDataX509Id), -1);

    /* copy certsList */
    size = xmlSecNssKeyDataX509GetCertsSize(src);
    for(pos = 0; pos < size; ++pos) {
        /* TBD: function below does linear scan, eliminate loop within
         * loop
         */
        certSrc = xmlSecNssKeyDataX509GetCert(src, pos);
        if(certSrc == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(src)),
                        "xmlSecNssKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return(-1);
        }

        certDst = CERT_DupCertificate(certSrc);
        if(certDst == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "CERT_DupCertificate",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            return(-1);
        }

        ret = xmlSecNssKeyDataX509AdoptCert(dst, certDst);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "xmlSecNssKeyDataX509AdoptCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            CERT_DestroyCertificate(certDst);
            return(-1);
        }
    }

    /* copy crls */
    size = xmlSecNssKeyDataX509GetCrlsSize(src);
    for(pos = 0; pos < size; ++pos) {
        crlSrc = xmlSecNssKeyDataX509GetCrl(src, pos);
        if(crlSrc == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(src)),
                        "xmlSecNssKeyDataX509GetCrl",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return(-1);
        }

        crlDst = SEC_DupCrl(crlSrc);
        if(crlDst == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "SEC_DupCrl",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            return(-1);
        }

        ret = xmlSecNssKeyDataX509AdoptCrl(dst, crlDst);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "xmlSecNssKeyDataX509AdoptCrl",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            SEC_DestroyCrl(crlDst);
            return(-1);
        }
    }

    /* copy key cert if exist */
    certSrc = xmlSecNssKeyDataX509GetKeyCert(src);
    if(certSrc != NULL) {
        certDst = CERT_DupCertificate(certSrc);
        if(certDst == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "CERT_DupCertificate",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            return(-1);
        }
        ret = xmlSecNssKeyDataX509AdoptKeyCert(dst, certDst);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "xmlSecNssKeyDataX509AdoptKeyCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            CERT_DestroyCertificate(certDst);
            return(-1);
        }
    }
    return(0);
}

static void
xmlSecNssKeyDataX509Finalize(xmlSecKeyDataPtr data) {
    xmlSecNssX509DataCtxPtr ctx;

    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id));

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert(ctx != NULL);

    if(ctx->certsList != NULL) {
        CERT_DestroyCertList(ctx->certsList);
    }

    if(ctx->crlsList != NULL) {
        xmlSecNssX509CrlNodePtr head;
        xmlSecNssX509CrlNodePtr tmp;

        head = ctx->crlsList;
        while (head)
        {
            tmp = head->next;
            SEC_DestroyCrl(head->crl);
            PR_Free(head);
            head = tmp;
        }
    }

    if(ctx->keyCert != NULL) {
        CERT_DestroyCertificate(ctx->keyCert);
    }

    memset(ctx, 0, sizeof(xmlSecNssX509DataCtx));
}

static int
xmlSecNssKeyDataX509XmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    int ret;

    xmlSecAssert2(id == xmlSecNssKeyDataX509Id, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    data = xmlSecKeyEnsureData(key, id);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyEnsureData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecNssX509DataNodeRead(data, node, keyInfoCtx);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssX509DataNodeRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_DONT_VERIFY_CERTS) == 0) {
        ret = xmlSecNssKeyDataX509VerifyAndExtractKey(data, key, keyInfoCtx);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecNssKeyDataX509VerifyAndExtractKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }
    return(0);
}

static int
xmlSecNssKeyDataX509XmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    CERTCertificate* cert;
    CERTSignedCrl* crl;
    xmlSecSize size, pos;
    int content = 0;
    int ret;

    xmlSecAssert2(id == xmlSecNssKeyDataX509Id, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    content = xmlSecX509DataGetNodeContent (node, keyInfoCtx);
    if (content < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecX509DataGetNodeContent",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "content=%d", content);
        return(-1);
    } else if(content == 0) {
        /* by default we are writing certificates and crls */
        content = XMLSEC_X509DATA_DEFAULT;
    }

    /* get x509 data */
    data = xmlSecKeyGetData(key, id);
    if(data == NULL) {
        /* no x509 data in the key */
        return(0);
    }

    /* write certs */
    size = xmlSecNssKeyDataX509GetCertsSize(data);
    for(pos = 0; pos < size; ++pos) {
        cert = xmlSecNssKeyDataX509GetCert(data, pos);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecNssKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return(-1);
        }

        if((content & XMLSEC_X509DATA_CERTIFICATE_NODE) != 0) {
            ret = xmlSecNssX509CertificateNodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecNssX509CertificateNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }

        if((content & XMLSEC_X509DATA_SUBJECTNAME_NODE) != 0) {
            ret = xmlSecNssX509SubjectNameNodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecNssX509SubjectNameNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }

        if((content & XMLSEC_X509DATA_ISSUERSERIAL_NODE) != 0) {
            ret = xmlSecNssX509IssuerSerialNodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecNssX509IssuerSerialNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }

        if((content & XMLSEC_X509DATA_SKI_NODE) != 0) {
            ret = xmlSecNssX509SKINodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecNssX509SKINodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }
    }

    /* write crls if needed */
    if((content & XMLSEC_X509DATA_CRL_NODE) != 0) {
        size = xmlSecNssKeyDataX509GetCrlsSize(data);
        for(pos = 0; pos < size; ++pos) {
            crl = xmlSecNssKeyDataX509GetCrl(data, pos);
            if(crl == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecNssKeyDataX509GetCrl",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }

            ret = xmlSecNssX509CRLNodeWrite(crl, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecNssX509CRLNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }
    }

    return(0);
}

static xmlSecKeyDataType
xmlSecNssKeyDataX509GetType(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), xmlSecKeyDataTypeUnknown);

    /* TODO: return verified/not verified status */
    return(xmlSecKeyDataTypeUnknown);
}

static const xmlChar*
xmlSecNssKeyDataX509GetIdentifier(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), NULL);

    /* TODO */
    return(NULL);
}

static void
xmlSecNssKeyDataX509DebugDump(xmlSecKeyDataPtr data, FILE* output) {
    CERTCertificate* cert;
    xmlSecSize size, pos;

    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id));
    xmlSecAssert(output != NULL);

    fprintf(output, "=== X509 Data:\n");
    cert = xmlSecNssKeyDataX509GetKeyCert(data);
    if(cert != NULL) {
        fprintf(output, "==== Key Certificate:\n");
        xmlSecNssX509CertDebugDump(cert, output);
    }

    size = xmlSecNssKeyDataX509GetCertsSize(data);
    for(pos = 0; pos < size; ++pos) {
        cert = xmlSecNssKeyDataX509GetCert(data, pos);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "xmlSecNssKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return;
        }
        fprintf(output, "==== Certificate:\n");
        xmlSecNssX509CertDebugDump(cert, output);
    }

    /* we don't print out crls */
}

static void
xmlSecNssKeyDataX509DebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    CERTCertificate* cert;
    xmlSecSize size, pos;

    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id));
    xmlSecAssert(output != NULL);

    fprintf(output, "<X509Data>\n");
    cert = xmlSecNssKeyDataX509GetKeyCert(data);
    if(cert != NULL) {
        fprintf(output, "<KeyCertificate>\n");
        xmlSecNssX509CertDebugXmlDump(cert, output);
        fprintf(output, "</KeyCertificate>\n");
    }

    size = xmlSecNssKeyDataX509GetCertsSize(data);
    for(pos = 0; pos < size; ++pos) {
        cert = xmlSecNssKeyDataX509GetCert(data, pos);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "xmlSecNssKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return;
        }
        fprintf(output, "<Certificate>\n");
        xmlSecNssX509CertDebugXmlDump(cert, output);
        fprintf(output, "</Certificate>\n");
    }

    /* we don't print out crls */
    fprintf(output, "</X509Data>\n");
}

static int
xmlSecNssX509DataNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlNodePtr cur;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    for(cur = xmlSecGetNextElementNode(node->children);
        cur != NULL;
        cur = xmlSecGetNextElementNode(cur->next)) {

        ret = 0;
        if(xmlSecCheckNodeName(cur, xmlSecNodeX509Certificate, xmlSecDSigNs)) {
            ret = xmlSecNssX509CertificateNodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509SubjectName, xmlSecDSigNs)) {
            ret = xmlSecNssX509SubjectNameNodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509IssuerSerial, xmlSecDSigNs)) {
            ret = xmlSecNssX509IssuerSerialNodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509SKI, xmlSecDSigNs)) {
            ret = xmlSecNssX509SKINodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509CRL, xmlSecDSigNs)) {
            ret = xmlSecNssX509CRLNodeRead(data, cur, keyInfoCtx);
        } else if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_STOP_ON_UNKNOWN_CHILD) != 0) {
            /* laxi schema validation: ignore unknown nodes */
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                        XMLSEC_ERRORS_R_UNEXPECTED_NODE,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "read node failed");
            return(-1);
        }
    }
    return(0);
}

static int
xmlSecNssX509CertificateNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar *content;
    CERTCertificate* cert;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    content = xmlNodeGetContent(node);
    if((content == NULL) || (xmlSecIsEmptyString(content) == 1)) {
        if(content != NULL) {
            xmlFree(content);
        }
        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_STOP_ON_EMPTY_NODE) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(node)),
                        XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        return(0);
    }

    cert = xmlSecNssX509CertBase64DerRead(content);
    if(cert == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssX509CertBase64DerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(content);
        return(-1);
    }

    ret = xmlSecNssKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        xmlFree(content);
        return(-1);
    }

    xmlFree(content);
    return(0);
}

static int
xmlSecNssX509CertificateNodeWrite(CERTCertificate* cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar* buf;
    xmlNodePtr cur;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    /* set base64 lines size from context */
    buf = xmlSecNssX509CertBase64DerWrite(cert, keyInfoCtx->base64LineSize);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509CertBase64DerWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509Certificate, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509Certificate));
        xmlFree(buf);
        return(-1);
    }

    /* todo: add \n around base64 data - from context */
    /* todo: add errors check */
    xmlNodeSetContent(cur, xmlSecStringCR);
    xmlNodeSetContent(cur, buf);
    xmlFree(buf);
    return(0);
}

static int
xmlSecNssX509SubjectNameNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataStorePtr x509Store;
    xmlChar* subject;
    CERTCertificate* cert;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecNssX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    subject = xmlNodeGetContent(node);
    if((subject == NULL) || (xmlSecIsEmptyString(subject) == 1)) {
        if(subject != NULL) {
            xmlFree(subject);
        }
        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_STOP_ON_EMPTY_NODE) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(node)),
                        XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        return(0);
    }

    cert = xmlSecNssX509StoreFindCert(x509Store, subject, NULL, NULL, NULL, keyInfoCtx);
    if(cert == NULL){

        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_STOP_ON_UNKNOWN_CERT) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_NOT_FOUND,
                        "subject=%s",
                        xmlSecErrorsSafeString(subject));
            xmlFree(subject);
            return(-1);
        }

        xmlFree(subject);
        return(0);
    }

    ret = xmlSecNssKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        xmlFree(subject);
        return(-1);
    }

    xmlFree(subject);
    return(0);
}

static int
xmlSecNssX509SubjectNameNodeWrite(CERTCertificate* cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx ATTRIBUTE_UNUSED) {
    xmlChar* buf = NULL;
    xmlNodePtr cur = NULL;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    buf = xmlSecNssX509NameWrite(&(cert->subject));
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
            NULL,
            "xmlSecNssX509NameWrite(&(cert->subject))",
            XMLSEC_ERRORS_R_XMLSEC_FAILED,
            XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509SubjectName, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
            NULL,
            "xmlSecEnsureEmptyChild",
            XMLSEC_ERRORS_R_XMLSEC_FAILED,
            "node=%s",
            xmlSecErrorsSafeString(xmlSecNodeX509SubjectName));
        xmlFree(buf);
        return(-1);
    }
    xmlSecNodeEncodeAndSetContent(cur, buf);
    xmlFree(buf);
    return(0);
}

static int
xmlSecNssX509IssuerSerialNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataStorePtr x509Store;
    xmlNodePtr cur;
    xmlChar *issuerName;
    xmlChar *issuerSerial;
    CERTCertificate* cert;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecNssX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecGetNextElementNode(node->children);
    if(cur == NULL) {
        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_STOP_ON_EMPTY_NODE) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeX509IssuerName),
                        XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeGetName(cur)));
            return(-1);
        }
        return(0);
    }

    /* the first is required node X509IssuerName */
    if(!xmlSecCheckNodeName(cur, xmlSecNodeX509IssuerName, xmlSecDSigNs)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerName),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)));
        return(-1);
    }
    issuerName = xmlNodeGetContent(cur);
    if(issuerName == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerName));
        return(-1);
    }
    cur = xmlSecGetNextElementNode(cur->next);

    /* next is required node X509SerialNumber */
    if((cur == NULL) || !xmlSecCheckNodeName(cur, xmlSecNodeX509SerialNumber, xmlSecDSigNs)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509SerialNumber));
        xmlFree(issuerName);
        return(-1);
    }
    issuerSerial = xmlNodeGetContent(cur);
    if(issuerSerial == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    xmlSecErrorsSafeString(xmlSecNodeX509SerialNumber),
                    XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)));
        xmlFree(issuerName);
        return(-1);
    }
    cur = xmlSecGetNextElementNode(cur->next);

    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_UNEXPECTED_NODE,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(issuerSerial);
        xmlFree(issuerName);
        return(-1);
    }

    cert = xmlSecNssX509StoreFindCert(x509Store, NULL, issuerName, issuerSerial, NULL, keyInfoCtx);
    if(cert == NULL){
        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_STOP_ON_UNKNOWN_CERT) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_NOT_FOUND,
                        "issuerName=%s;issuerSerial=%s",
                        xmlSecErrorsSafeString(issuerName),
                        xmlSecErrorsSafeString(issuerSerial));
            xmlFree(issuerSerial);
            xmlFree(issuerName);
            return(-1);
        }

        xmlFree(issuerSerial);
        xmlFree(issuerName);
        return(0);
    }

    ret = xmlSecNssKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        xmlFree(issuerSerial);
        xmlFree(issuerName);
        return(-1);
    }

    xmlFree(issuerSerial);
    xmlFree(issuerName);
    return(0);
}

static int
xmlSecNssX509IssuerSerialNodeWrite(CERTCertificate* cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx ATTRIBUTE_UNUSED) {
    xmlNodePtr cur;
    xmlNodePtr issuerNameNode;
    xmlNodePtr issuerNumberNode;
    xmlChar* buf;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    /* create xml nodes */
    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509IssuerSerial, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerSerial));
        return(-1);
    }

    issuerNameNode = xmlSecEnsureEmptyChild(cur, xmlSecNodeX509IssuerName, xmlSecDSigNs);
    if(issuerNameNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerName));
        return(-1);
    }

    issuerNumberNode = xmlSecEnsureEmptyChild(cur, xmlSecNodeX509SerialNumber, xmlSecDSigNs);
    if(issuerNumberNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509SerialNumber));
        return(-1);
    }

    /* write data */
    buf = xmlSecNssX509NameWrite(&(cert->issuer));
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509NameWrite(&(cert->issuer))",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecNodeEncodeAndSetContent(issuerNameNode, buf);
    xmlFree(buf);

    buf = xmlSecNssASN1IntegerWrite(&(cert->serialNumber));
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssASN1IntegerWrite(&(cert->serialNumber))",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlNodeSetContent(issuerNumberNode, buf);
    xmlFree(buf);

    return(0);
}

static int
xmlSecNssX509SKINodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataStorePtr x509Store;
    xmlChar* ski;
    CERTCertificate* cert;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecNssX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ski = xmlNodeGetContent(node);
    if((ski == NULL) || (xmlSecIsEmptyString(ski) == 1)) {
        if(ski != NULL) {
            xmlFree(ski);
        }
        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_STOP_ON_EMPTY_NODE) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(node)),
                        XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeX509SKI));
            return(-1);
        }
        return(0);
    }

    cert = xmlSecNssX509StoreFindCert(x509Store, NULL, NULL, NULL, ski, keyInfoCtx);
    if(cert == NULL){
        xmlFree(ski);

        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_STOP_ON_UNKNOWN_CERT) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_NOT_FOUND,
                        "ski=%s",
                        xmlSecErrorsSafeString(ski));
            return(-1);
        }
        return(0);
    }

    ret = xmlSecNssKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        xmlFree(ski);
        return(-1);
    }

    xmlFree(ski);
    return(0);
}

static int
xmlSecNssX509SKINodeWrite(CERTCertificate* cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx ATTRIBUTE_UNUSED) {
    xmlChar *buf = NULL;
    xmlNodePtr cur = NULL;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    buf = xmlSecNssX509SKIWrite(cert);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509SKIWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509SKI, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "new_node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509SKI));
        xmlFree(buf);
        return(-1);
    }
    xmlSecNodeEncodeAndSetContent(cur, buf);
    xmlFree(buf);

    return(0);
}

static int
xmlSecNssX509CRLNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar *content;
    CERTSignedCrl* crl;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    content = xmlNodeGetContent(node);
   if((content == NULL) || (xmlSecIsEmptyString(content) == 1)) {
        if(content != NULL) {
            xmlFree(content);
        }
        if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_STOP_ON_EMPTY_NODE) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(node)),
                        XMLSEC_ERRORS_R_INVALID_NODE_CONTENT,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        return(0);
    }

    crl = xmlSecNssX509CrlBase64DerRead(content, keyInfoCtx);
    if(crl == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssX509CrlBase64DerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(content);
        return(-1);
    }

    SEC_DestroyCrl(crl);
    xmlFree(content);
    return(0);
}

static int
xmlSecNssX509CRLNodeWrite(CERTSignedCrl* crl, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar* buf = NULL;
    xmlNodePtr cur = NULL;

    xmlSecAssert2(crl != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    /* set base64 lines size from context */
    buf = xmlSecNssX509CrlBase64DerWrite(crl, keyInfoCtx->base64LineSize);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509CrlBase64DerWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509CRL, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "new_node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509CRL));
        xmlFree(buf);
        return(-1);
    }
    /* todo: add \n around base64 data - from context */
    /* todo: add errors check */
    xmlNodeSetContent(cur, xmlSecStringCR);
    xmlNodeSetContent(cur, buf);
    xmlFree(buf);

    return(0);
}


static int
xmlSecNssKeyDataX509VerifyAndExtractKey(xmlSecKeyDataPtr data, xmlSecKeyPtr key,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecNssX509DataCtxPtr ctx;
    xmlSecKeyDataStorePtr x509Store;
    int ret;
    SECStatus status;
    PRTime notBefore, notAfter;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataX509Id), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    ctx = xmlSecNssX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecNssX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((ctx->keyCert == NULL) && (ctx->certsList != NULL) && (xmlSecKeyGetValue(key) == NULL)) {
        CERTCertificate* cert;

        cert = xmlSecNssX509StoreVerify(x509Store, ctx->certsList, keyInfoCtx);
        if(cert != NULL) {
            xmlSecKeyDataPtr keyValue;

            ctx->keyCert = CERT_DupCertificate(cert);
            if(ctx->keyCert == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "CERT_DupCertificate",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            keyValue = xmlSecNssX509CertGetKey(ctx->keyCert);
            if(keyValue == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "xmlSecNssX509CertGetKey",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            /* verify that the key matches our expectations */
            if(xmlSecKeyReqMatchKeyValue(&(keyInfoCtx->keyReq), keyValue) != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "xmlSecKeyReqMatchKeyValue",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlSecKeyDataDestroy(keyValue);
                return(-1);
            }

            ret = xmlSecKeySetValue(key, keyValue);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "xmlSecKeySetValue",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlSecKeyDataDestroy(keyValue);
                return(-1);
            }

            status = CERT_GetCertTimes(ctx->keyCert, &notBefore, &notAfter);
            if (status == SECSuccess) {
                ret = xmlSecNssX509CertGetTime(&notBefore, &(key->notValidBefore));
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                                "xmlSecNssX509CertGetTime",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "notValidBefore");
                    return(-1);
                }
                ret = xmlSecNssX509CertGetTime(&notAfter, &(key->notValidAfter));
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                                "xmlSecNssX509CertGetTime",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "notValidAfter");
                    return(-1);
                }
            } else {
                key->notValidBefore = key->notValidAfter = 0;
            }
        } else if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_STOP_ON_INVALID_CERT) != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_NOT_FOUND,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }
    return(0);
}

static int
xmlSecNssX509CertGetTime(PRTime* t, time_t* res) {

    PRTime tmp64_1, tmp64_2;
    PRUint32 tmp32 = 1000000;

    xmlSecAssert2(t != NULL, -1);
    xmlSecAssert2(res != NULL, -1);

    /* PRTime is time in microseconds since epoch. Divide by 1000000 to
     * convert to seconds, then convert to an unsigned 32 bit number
     */
    (*res) = 0;
    LL_UI2L(tmp64_1, tmp32);
    LL_DIV(tmp64_2, *t, tmp64_1);
    LL_L2UI(tmp32, tmp64_2);

    (*res) = (time_t)(tmp32);

    return(0);
}

/**
 * xmlSecNssX509CertGetKey:
 * @cert:               the certificate.
 *
 * Extracts public key from the @cert.
 *
 * Returns: public key value or NULL if an error occurs.
 */
xmlSecKeyDataPtr
xmlSecNssX509CertGetKey(CERTCertificate* cert) {
    xmlSecKeyDataPtr data;
    SECKEYPublicKey *pubkey = NULL;

    xmlSecAssert2(cert != NULL, NULL);

    pubkey = CERT_ExtractPublicKey(cert);
    if(pubkey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CERT_ExtractPublicKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        return(NULL);
    }

    data = xmlSecNssPKIAdoptKey(NULL, pubkey);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssPKIAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECKEY_DestroyPublicKey(pubkey);
        return(NULL);
    }

    return(data);
}

static CERTCertificate*
xmlSecNssX509CertBase64DerRead(xmlChar* buf) {
    int ret;

    xmlSecAssert2(buf != NULL, NULL);

    /* usual trick with base64 decoding "in-place" */
    ret = xmlSecBase64Decode(buf, (xmlSecByte*)buf, xmlStrlen(buf));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Decode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecNssX509CertDerRead((xmlSecByte*)buf, ret));
}


static CERTCertificate*
xmlSecNssX509CertDerRead(const xmlSecByte* buf, xmlSecSize size) {
    CERTCertificate *cert;
    SECItem  derCert;

    xmlSecAssert2(buf != NULL, NULL);
    xmlSecAssert2(size > 0, NULL);

    derCert.data = (unsigned char *)buf;
    derCert.len = size;

    /* decode cert and import to temporary cert db */
    cert = __CERT_NewTempCertificate(CERT_GetDefaultCertDB(), &derCert,
                                     NULL, PR_FALSE, PR_TRUE);
    if(cert == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "__CERT_NewTempCertificate",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        return(NULL);
    }


    return(cert);
}

static xmlChar*
xmlSecNssX509CertBase64DerWrite(CERTCertificate* cert, int base64LineWrap) {
    xmlChar *res = NULL;
    xmlSecByte *p = NULL;
    long size;

    xmlSecAssert2(cert != NULL, NULL);

    p = cert->derCert.data;
    size = cert->derCert.len;
    if((size <= 0) || (p == NULL)){
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "cert->derCert",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        return(NULL);
    }

    res = xmlSecBase64Encode(p, size, base64LineWrap);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(res);
}

static CERTSignedCrl*
xmlSecNssX509CrlBase64DerRead(xmlChar* buf,
                              xmlSecKeyInfoCtxPtr keyInfoCtx) {
    int ret;

    xmlSecAssert2(buf != NULL, NULL);

    /* usual trick with base64 decoding "in-place" */
    ret = xmlSecBase64Decode(buf, (xmlSecByte*)buf, xmlStrlen(buf));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Decode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecNssX509CrlDerRead((xmlSecByte*)buf, ret, keyInfoCtx));
}


static CERTSignedCrl*
xmlSecNssX509CrlDerRead(xmlSecByte* buf, xmlSecSize size,
                        xmlSecKeyInfoCtxPtr keyInfoCtx) {
    CERTSignedCrl *crl = NULL;
    SECItem derCrl;
    PK11SlotInfo *slot = NULL;
    PRInt32 importOptions = CRL_IMPORT_DEFAULT_OPTIONS;

    xmlSecAssert2(buf != NULL, NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);
    xmlSecAssert2(size > 0, NULL);

    derCrl.data = buf;
    derCrl.len = size;

    /* we're importing a CRL, it is ok to use the internal slot.
     * crlutil does it :)
     */
    slot = xmlSecNssGetInternalKeySlot();
    if (slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssGetInternalKeySlot",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
        return NULL;
    }

    if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_SKIP_STRICT_CHECKS) != 0)
        importOptions |= CRL_IMPORT_BYPASS_CHECKS;

    crl = PK11_ImportCRL(slot, &derCrl, NULL, SEC_CRL_TYPE, NULL,
                         importOptions, NULL, CRL_DECODE_DEFAULT_OPTIONS);

    if(crl == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PK11_ImportCRL",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        PK11_FreeSlot(slot);
        return(NULL);
    }

    PK11_FreeSlot(slot);
    return(crl);
}

static xmlChar*
xmlSecNssX509CrlBase64DerWrite(CERTSignedCrl* crl, int base64LineWrap) {
    xmlChar *res = NULL;
    xmlSecByte *p = NULL;
    long size;

    xmlSecAssert2(crl != NULL && crl->derCrl != NULL, NULL);

    p = crl->derCrl->data;
    size = crl->derCrl->len;
    if((size <= 0) || (p == NULL)){
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "crl->derCrl",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        return(NULL);
    }

    res = xmlSecBase64Encode(p, size, base64LineWrap);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(res);
}

static xmlChar*
xmlSecNssX509NameWrite(CERTName* nm) {
    xmlChar *res = NULL;
    char *str;

    xmlSecAssert2(nm != NULL, NULL);

    str = CERT_NameToAscii(nm);
    if (str == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CERT_NameToAscii",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlStrdup(BAD_CAST str);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlStrdup",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        PORT_Free(str);
        return(NULL);
    }
    PORT_Free(str);
    return(res);
}

static xmlChar*
xmlSecNssASN1IntegerWrite(SECItem *num) {
    xmlChar *res = NULL;
    int resLen = 64; /* not more than 64 chars */
    PRUint64 val = 0;
    unsigned int ii = 0;
    int shift = 0;

    xmlSecAssert2(num != NULL, NULL);
    xmlSecAssert2(num->type == siBuffer, NULL);
    xmlSecAssert2(num->len <= 9, NULL);
    xmlSecAssert2(num->data != NULL, NULL);

    /* HACK : to be fixed after
     * NSS bug http://bugzilla.mozilla.org/show_bug.cgi?id=212864 is fixed
     */
    for(ii = num->len; ii > 0; --ii, shift += 8) {
        val |= ((PRUint64)num->data[ii - 1]) << shift;
    }

    res = (xmlChar*)xmlMalloc(resLen + 1);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlStrdup",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return (NULL);
    }

    PR_snprintf((char*)res, resLen, "%llu", val);
    return(res);
}

static xmlChar*
xmlSecNssX509SKIWrite(CERTCertificate* cert) {
    xmlChar *res = NULL;
    SECItem ski;
    SECStatus rv;

    xmlSecAssert2(cert != NULL, NULL);

    memset(&ski, 0, sizeof(ski));

    rv = CERT_FindSubjectKeyIDExtension(cert, &ski);
    if (rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CERT_FindSubjectKeyIDExtension",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&ski, PR_FALSE);
        return(NULL);
    }

    res = xmlSecBase64Encode(ski.data, ski.len, 0);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        SECITEM_FreeItem(&ski, PR_FALSE);
        return(NULL);
    }
    SECITEM_FreeItem(&ski, PR_FALSE);

    return(res);
}


static void
xmlSecNssX509CertDebugDump(CERTCertificate* cert, FILE* output) {
    SECItem *sn;
    unsigned int i;

    xmlSecAssert(cert != NULL);
    xmlSecAssert(output != NULL);

    fprintf(output, "==== Subject Name: %s\n", cert->subjectName);
    fprintf(output, "==== Issuer Name: %s\n", cert->issuerName);
    sn = &cert->serialNumber;

    for (i = 0; i < sn->len; i++) {
        if (i != sn->len - 1) {
            fprintf(output, "%02x:", sn->data[i]);
        } else {
            fprintf(output, "%02x", sn->data[i]);
        }
    }
    fprintf(output, "\n");
}


static void
xmlSecNssX509CertDebugXmlDump(CERTCertificate* cert, FILE* output) {
    SECItem *sn;
    unsigned int i;

    xmlSecAssert(cert != NULL);
    xmlSecAssert(output != NULL);

    fprintf(output, "<SubjectName>");
    xmlSecPrintXmlString(output, BAD_CAST cert->subjectName);
    fprintf(output, "</SubjectName>\n");

    fprintf(output, "<IssuerName>");
    xmlSecPrintXmlString(output, BAD_CAST cert->issuerName);
    fprintf(output, "</IssuerName>\n");

    fprintf(output, "<SerialNumber>");
    sn = &cert->serialNumber;
    for (i = 0; i < sn->len; i++) {
        if (i != sn->len - 1) {
            fprintf(output, "%02x:", sn->data[i]);
        } else {
            fprintf(output, "%02x", sn->data[i]);
        }
    }
    fprintf(output, "</SerialNumber>\n");
}


/**************************************************************************
 *
 * Raw X509 Certificate processing
 *
 *
 *************************************************************************/
static int              xmlSecNssKeyDataRawX509CertBinRead      (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 const xmlSecByte* buf,
                                                                 xmlSecSize bufSize,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);

static xmlSecKeyDataKlass xmlSecNssKeyDataRawX509CertKlass = {
    sizeof(xmlSecKeyDataKlass),
    sizeof(xmlSecKeyData),

    /* data */
    xmlSecNameRawX509Cert,
    xmlSecKeyDataUsageRetrievalMethodNodeBin,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefRawX509Cert,                      /* const xmlChar* href; */
    NULL,                                       /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                               /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    NULL,                                       /* xmlSecKeyDataInitializeMethod initialize; */
    NULL,                                       /* xmlSecKeyDataDuplicateMethod duplicate; */
    NULL,                                       /* xmlSecKeyDataFinalizeMethod finalize; */
    NULL,                                       /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    NULL,                                       /* xmlSecKeyDataGetTypeMethod getType; */
    NULL,                                       /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    NULL,                                       /* xmlSecKeyDataXmlReadMethod xmlRead; */
    NULL,                                       /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    xmlSecNssKeyDataRawX509CertBinRead, /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                                       /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    NULL,                                       /* xmlSecKeyDataDebugDumpMethod debugDump; */
    NULL,                                       /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataRawX509CertGetKlass:
 *
 * The raw X509 certificates key data klass.
 *
 * Returns: raw X509 certificates key data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataRawX509CertGetKlass(void) {
    return(&xmlSecNssKeyDataRawX509CertKlass);
}

static int
xmlSecNssKeyDataRawX509CertBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    const xmlSecByte* buf, xmlSecSize bufSize,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    CERTCertificate* cert;
    int ret;

    xmlSecAssert2(id == xmlSecNssKeyDataRawX509CertId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    cert = xmlSecNssX509CertDerRead(buf, bufSize);
    if(cert == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509CertDerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    data = xmlSecKeyEnsureData(key, xmlSecNssKeyDataX509Id);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyEnsureData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        return(-1);
    }

    ret = xmlSecNssKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        CERT_DestroyCertificate(cert);
        return(-1);
    }

    ret = xmlSecNssKeyDataX509VerifyAndExtractKey(data, key, keyInfoCtx);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssKeyDataX509VerifyAndExtractKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(0);
}

#endif /* XMLSEC_NO_X509 */

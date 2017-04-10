/**
 * XMLSec library
 *
 * X509 support
 *
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_X509

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/x509.h>
#include <xmlsec/base64.h>
#include <xmlsec/errors.h>
#include <xmlsec/private.h>

#include <xmlsec/gnutls/crypto.h>
#include <xmlsec/gnutls/x509.h>

#include "x509utils.h"

/*************************************************************************
 *
 * X509 utility functions
 *
 ************************************************************************/
static int              xmlSecGnuTLSX509DataNodeRead            (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509CertificateNodeRead     (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509CertificateNodeWrite    (gnutls_x509_crt_t cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509SubjectNameNodeRead     (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509SubjectNameNodeWrite    (gnutls_x509_crt_t cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509IssuerSerialNodeRead    (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509IssuerSerialNodeWrite   (gnutls_x509_crt_t cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509SKINodeRead             (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509SKINodeWrite            (gnutls_x509_crt_t cert,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509CRLNodeRead             (xmlSecKeyDataPtr data,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSX509CRLNodeWrite            (gnutls_x509_crl_t crl,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSKeyDataX509VerifyAndExtractKey(xmlSecKeyDataPtr data,
                                                                 xmlSecKeyPtr key,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);

/*************************************************************************
 *
 * Internal GnuTLS X509 data CTX
 *
 ************************************************************************/
typedef struct _xmlSecGnuTLSX509DataCtx                         xmlSecGnuTLSX509DataCtx,
                                                                *xmlSecGnuTLSX509DataCtxPtr;
struct _xmlSecGnuTLSX509DataCtx {
    gnutls_x509_crt_t   keyCert;
    xmlSecPtrList       certsList;
    xmlSecPtrList       crlsList;
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
 * xmlSecGnuTLSX509DataCtx is located after xmlSecTransform
 *
 *************************************************************************/
#define xmlSecGnuTLSX509DataSize       \
    (sizeof(xmlSecKeyData) + sizeof(xmlSecGnuTLSX509DataCtx))
#define xmlSecGnuTLSX509DataGetCtx(data) \
    ((xmlSecGnuTLSX509DataCtxPtr)(((xmlSecByte*)(data)) + sizeof(xmlSecKeyData)))

static int              xmlSecGnuTLSKeyDataX509Initialize      (xmlSecKeyDataPtr data);
static int              xmlSecGnuTLSKeyDataX509Duplicate       (xmlSecKeyDataPtr dst,
                                                                 xmlSecKeyDataPtr src);
static void             xmlSecGnuTLSKeyDataX509Finalize        (xmlSecKeyDataPtr data);
static int              xmlSecGnuTLSKeyDataX509XmlRead         (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGnuTLSKeyDataX509XmlWrite        (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static xmlSecKeyDataType xmlSecGnuTLSKeyDataX509GetType        (xmlSecKeyDataPtr data);
static const xmlChar*   xmlSecGnuTLSKeyDataX509GetIdentifier   (xmlSecKeyDataPtr data);

static void             xmlSecGnuTLSKeyDataX509DebugDump       (xmlSecKeyDataPtr data,
                                                                 FILE* output);
static void             xmlSecGnuTLSKeyDataX509DebugXmlDump    (xmlSecKeyDataPtr data,
                                                                 FILE* output);



static xmlSecKeyDataKlass xmlSecGnuTLSKeyDataX509Klass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecGnuTLSX509DataSize,

    /* data */
    xmlSecNameX509Data,
    xmlSecKeyDataUsageKeyInfoNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefX509Data,                         /* const xmlChar* href; */
    xmlSecNodeX509Data,                         /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                               /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecGnuTLSKeyDataX509Initialize,         /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecGnuTLSKeyDataX509Duplicate,          /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecGnuTLSKeyDataX509Finalize,           /* xmlSecKeyDataFinalizeMethod finalize; */
    NULL,                                       /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecGnuTLSKeyDataX509GetType,            /* xmlSecKeyDataGetTypeMethod getType; */
    NULL,                                       /* xmlSecKeyDataGetSizeMethod getSize; */
    xmlSecGnuTLSKeyDataX509GetIdentifier,      /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecGnuTLSKeyDataX509XmlRead,            /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecGnuTLSKeyDataX509XmlWrite,           /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    NULL,                                       /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                                       /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecGnuTLSKeyDataX509DebugDump,          /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecGnuTLSKeyDataX509DebugXmlDump,       /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGnuTLSKeyDataX509GetKlass:
 *
 * The GnuTLS X509 key data klass (http://www.w3.org/TR/xmldsig-core/#sec-X509Data).
 *
 * Returns: the X509 data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataX509GetKlass(void) {
    return(&xmlSecGnuTLSKeyDataX509Klass);
}

/**
 * xmlSecGnuTLSKeyDataX509GetKeyCert:
 * @data:               the pointer to X509 key data.
 *
 * Gets the certificate from which the key was extracted.
 *
 * Returns: the key's certificate or NULL if key data was not used for key
 * extraction or an error occurs.
 */
gnutls_x509_crt_t
xmlSecGnuTLSKeyDataX509GetKeyCert(xmlSecKeyDataPtr data) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), NULL);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    return(ctx->keyCert);
}

/**
 * xmlSecGnuTLSKeyDataX509AdoptKeyCert:
 * @data:               the pointer to X509 key data.
 * @cert:               the pointer to GnuTLS X509 certificate.
 *
 * Sets the key's certificate in @data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGnuTLSKeyDataX509AdoptKeyCert(xmlSecKeyDataPtr data, gnutls_x509_crt_t cert) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(cert != NULL, -1);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    if(ctx->keyCert != NULL) {
        gnutls_x509_crt_deinit(ctx->keyCert);
    }
    ctx->keyCert = cert;
    return(0);
}

/**
 * xmlSecGnuTLSKeyDataX509AdoptCert:
 * @data:               the pointer to X509 key data.
 * @cert:               the pointer to GnuTLS X509 certificate.
 *
 * Adds certificate to the X509 key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGnuTLSKeyDataX509AdoptCert(xmlSecKeyDataPtr data, gnutls_x509_crt_t cert) {
    xmlSecGnuTLSX509DataCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(cert != NULL, -1);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    ret = xmlSecPtrListAdd(&(ctx->certsList), cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecPtrListAdd",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecGnuTLSKeyDataX509GetCert:
 * @data:               the pointer to X509 key data.
 * @pos:                the desired certificate position.
 *
 * Gets a certificate from X509 key data.
 *
 * Returns: the pointer to certificate or NULL if @pos is larger than the
 * number of certificates in @data or an error occurs.
 */
gnutls_x509_crt_t
xmlSecGnuTLSKeyDataX509GetCert(xmlSecKeyDataPtr data, xmlSecSize pos) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), NULL);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    return(xmlSecPtrListGetItem(&(ctx->certsList), pos));
}

/**
 * xmlSecGnuTLSKeyDataX509GetCertsSize:
 * @data:               the pointer to X509 key data.
 *
 * Gets the number of certificates in @data.
 *
 * Returns: te number of certificates in @data.
 */
xmlSecSize
xmlSecGnuTLSKeyDataX509GetCertsSize(xmlSecKeyDataPtr data) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), 0);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, 0);

    return(xmlSecPtrListGetSize(&(ctx->certsList)));
}

/**
 * xmlSecGnuTLSKeyDataX509AdoptCrl:
 * @data:               the pointer to X509 key data.
 * @crl:                the pointer to GnuTLS X509 crl.
 *
 * Adds crl to the X509 key data.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecGnuTLSKeyDataX509AdoptCrl(xmlSecKeyDataPtr data, gnutls_x509_crl_t crl) {
    xmlSecGnuTLSX509DataCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(crl != NULL, -1);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    ret = xmlSecPtrListAdd(&(ctx->crlsList), crl);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecPtrListAdd",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecGnuTLSKeyDataX509GetCrl:
 * @data:               the pointer to X509 key data.
 * @pos:                the desired crl position.
 *
 * Gets a crl from X509 key data.
 *
 * Returns: the pointer to crl or NULL if @pos is larger than the
 * number of crls in @data or an error occurs.
 */
gnutls_x509_crl_t
xmlSecGnuTLSKeyDataX509GetCrl(xmlSecKeyDataPtr data, xmlSecSize pos) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), NULL);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    return(xmlSecPtrListGetItem(&(ctx->crlsList), pos));
}

/**
 * xmlSecGnuTLSKeyDataX509GetCrlsSize:
 * @data:               the pointer to X509 key data.
 *
 * Gets the number of crls in @data.
 *
 * Returns: te number of crls in @data.
 */
xmlSecSize
xmlSecGnuTLSKeyDataX509GetCrlsSize(xmlSecKeyDataPtr data) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), 0);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, 0);

    return(xmlSecPtrListGetSize(&(ctx->crlsList)));
}


static int
xmlSecGnuTLSKeyDataX509Initialize(xmlSecKeyDataPtr data) {
    xmlSecGnuTLSX509DataCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecGnuTLSX509DataCtx));

    ret = xmlSecPtrListInitialize(&(ctx->certsList), xmlSecGnuTLSX509CrtListId);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecPtrListInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "certsList");
        return(-1);
    }

    ret = xmlSecPtrListInitialize(&(ctx->crlsList), xmlSecGnuTLSX509CrlListId);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecPtrListInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "crlsList");
        return(-1);
    }

    return(0);
}

static int
xmlSecGnuTLSKeyDataX509Duplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecGnuTLSX509DataCtxPtr ctxSrc;
    xmlSecGnuTLSX509DataCtxPtr ctxDst;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(dst, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(src, xmlSecGnuTLSKeyDataX509Id), -1);

    ctxSrc = xmlSecGnuTLSX509DataGetCtx(src);
    xmlSecAssert2(ctxSrc != NULL, 0);
    ctxDst = xmlSecGnuTLSX509DataGetCtx(dst);
    xmlSecAssert2(ctxDst != NULL, 0);

    /* copy key cert if exist */
    if(ctxDst->keyCert != NULL) {
        gnutls_x509_crt_deinit(ctxDst->keyCert);
        ctxDst->keyCert = NULL;
    }
    if(ctxSrc->keyCert != NULL) {
        ctxDst->keyCert = xmlSecGnuTLSX509CertDup(ctxSrc->keyCert);
        if(ctxDst->keyCert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(src)),
                        "xmlSecGnuTLSX509CertDup",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    /* copy certsList if exists */
    xmlSecPtrListEmpty(&(ctxDst->certsList));
    ret = xmlSecPtrListCopy(&(ctxDst->certsList), &(ctxSrc->certsList));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(src)),
                    "xmlSecPtrListCopy",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "certsList");
        return(-1);
    }

    /* copy crlsList if exists */
    xmlSecPtrListEmpty(&(ctxDst->crlsList));
    ret = xmlSecPtrListCopy(&(ctxDst->crlsList), &(ctxSrc->crlsList));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(src)),
                    "xmlSecPtrListCopy",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "crlsList");
        return(-1);
    }
    /* done */
    return(0);
}

static void
xmlSecGnuTLSKeyDataX509Finalize(xmlSecKeyDataPtr data) {
    xmlSecGnuTLSX509DataCtxPtr ctx;

    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id));

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert(ctx != NULL);

    xmlSecPtrListFinalize(&(ctx->crlsList));
    xmlSecPtrListFinalize(&(ctx->certsList));
    if(ctx->keyCert != NULL) {
        gnutls_x509_crt_deinit(ctx->keyCert);
    }
    memset(ctx, 0, sizeof(xmlSecGnuTLSX509DataCtx));
}

static int
xmlSecGnuTLSKeyDataX509XmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    int ret;

    xmlSecAssert2(id == xmlSecGnuTLSKeyDataX509Id, -1);
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

    ret = xmlSecGnuTLSX509DataNodeRead(data, node, keyInfoCtx);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGnuTLSX509DataNodeRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((keyInfoCtx->flags & XMLSEC_KEYINFO_FLAGS_X509DATA_DONT_VERIFY_CERTS) == 0) {
        ret = xmlSecGnuTLSKeyDataX509VerifyAndExtractKey(data, key, keyInfoCtx);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGnuTLSKeyDataX509VerifyAndExtractKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }
    return(0);
}

static int
xmlSecGnuTLSKeyDataX509XmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    gnutls_x509_crt_t cert;
    gnutls_x509_crl_t crl;
    xmlSecSize size, pos;
    int content;
    int ret;

    xmlSecAssert2(id == xmlSecGnuTLSKeyDataX509Id, -1);
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
    size = xmlSecGnuTLSKeyDataX509GetCertsSize(data);
    for(pos = 0; pos < size; ++pos) {
        cert = xmlSecGnuTLSKeyDataX509GetCert(data, pos);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGnuTLSKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return(-1);
        }

        if((content & XMLSEC_X509DATA_CERTIFICATE_NODE) != 0) {
            ret = xmlSecGnuTLSX509CertificateNodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecGnuTLSX509CertificateNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }

        if((content & XMLSEC_X509DATA_SUBJECTNAME_NODE) != 0) {
            ret = xmlSecGnuTLSX509SubjectNameNodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecGnuTLSX509SubjectNameNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }

        if((content & XMLSEC_X509DATA_ISSUERSERIAL_NODE) != 0) {
            ret = xmlSecGnuTLSX509IssuerSerialNodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecGnuTLSX509IssuerSerialNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }

        if((content & XMLSEC_X509DATA_SKI_NODE) != 0) {
            ret = xmlSecGnuTLSX509SKINodeWrite(cert, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecGnuTLSX509SKINodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }
    }

    /* write crls if needed */
    if((content & XMLSEC_X509DATA_CRL_NODE) != 0) {
        size = xmlSecGnuTLSKeyDataX509GetCrlsSize(data);
        for(pos = 0; pos < size; ++pos) {
            crl = xmlSecGnuTLSKeyDataX509GetCrl(data, pos);
            if(crl == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecGnuTLSKeyDataX509GetCrl",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }

            ret = xmlSecGnuTLSX509CRLNodeWrite(crl, node, keyInfoCtx);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                            "xmlSecGnuTLSX509CRLNodeWrite",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "pos=%d", pos);
                return(-1);
            }
        }
    }

    /* done */
    return(0);
}


static xmlSecKeyDataType
xmlSecGnuTLSKeyDataX509GetType(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), xmlSecKeyDataTypeUnknown);

    /* TODO: return verified/not verified status */
    return(xmlSecKeyDataTypeUnknown);
}

static const xmlChar*
xmlSecGnuTLSKeyDataX509GetIdentifier(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), NULL);

    /* TODO */
    return(NULL);
}

static void
xmlSecGnuTLSKeyDataX509DebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecSize size, pos;

    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id));
    xmlSecAssert(output != NULL);

    fprintf(output, "=== X509 Data:\n");

    /* key cert */
    {
        gnutls_x509_crt_t cert;

        cert = xmlSecGnuTLSKeyDataX509GetKeyCert(data);
        if(cert != NULL) {
            fprintf(output, "==== Key Certificate:\n");
            xmlSecGnuTLSX509CertDebugDump(cert, output);
        }
    }

    /* other certs */
    size = xmlSecGnuTLSKeyDataX509GetCertsSize(data);
    for(pos = 0; pos < size; ++pos) {
        gnutls_x509_crt_t cert;

        cert = xmlSecGnuTLSKeyDataX509GetCert(data, pos);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "xmlSecGnuTLSKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return;
        }
        fprintf(output, "==== Certificate:\n");
        xmlSecGnuTLSX509CertDebugDump(cert, output);
    }

    /* crls */
    size = xmlSecGnuTLSKeyDataX509GetCrlsSize(data);
    for(pos = 0; pos < size; ++pos) {
        gnutls_x509_crl_t crl;

        crl = xmlSecGnuTLSKeyDataX509GetCrl(data, pos);
        if(crl == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "xmlSecGnuTLSKeyDataX509GetCrl",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return;
        }
        fprintf(output, "==== Crl:\n");
        xmlSecGnuTLSX509CrlDebugDump(crl, output);
    }
}

static void
xmlSecGnuTLSKeyDataX509DebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecSize size, pos;

    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id));
    xmlSecAssert(output != NULL);

    fprintf(output, "<X509Data>\n");

    /* key cert */
    {
        gnutls_x509_crt_t cert;

        cert = xmlSecGnuTLSKeyDataX509GetKeyCert(data);
        if(cert != NULL) {
            fprintf(output, "<KeyCertificate>\n");
            xmlSecGnuTLSX509CertDebugXmlDump(cert, output);
            fprintf(output, "</KeyCertificate>\n");
        }
    }

    /* other certs */
    size = xmlSecGnuTLSKeyDataX509GetCertsSize(data);
    for(pos = 0; pos < size; ++pos) {
        gnutls_x509_crt_t cert;

        cert = xmlSecGnuTLSKeyDataX509GetCert(data, pos);
        if(cert == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "xmlSecGnuTLSKeyDataX509GetCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return;
        }
        fprintf(output, "<Certificate>\n");
        xmlSecGnuTLSX509CertDebugXmlDump(cert, output);
        fprintf(output, "</Certificate>\n");
    }

    /* other crls */
    size = xmlSecGnuTLSKeyDataX509GetCrlsSize(data);
    for(pos = 0; pos < size; ++pos) {
        gnutls_x509_crl_t crl;

        crl = xmlSecGnuTLSKeyDataX509GetCrl(data, pos);
        if(crl == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "xmlSecGnuTLSKeyDataX509GetCrl",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "pos=%d", pos);
            return;
        }
        fprintf(output, "<CRL>\n");
        xmlSecGnuTLSX509CrlDebugXmlDump(crl, output);
        fprintf(output, "</CRL>\n");
    }

    /* we don't print out crls */
    fprintf(output, "</X509Data>\n");
}

static int
xmlSecGnuTLSX509DataNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlNodePtr cur;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    for(cur = xmlSecGetNextElementNode(node->children);
        cur != NULL;
        cur = xmlSecGetNextElementNode(cur->next)) {

        ret = 0;
        if(xmlSecCheckNodeName(cur, xmlSecNodeX509Certificate, xmlSecDSigNs)) {
            ret = xmlSecGnuTLSX509CertificateNodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509SubjectName, xmlSecDSigNs)) {
            ret = xmlSecGnuTLSX509SubjectNameNodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509IssuerSerial, xmlSecDSigNs)) {
            ret = xmlSecGnuTLSX509IssuerSerialNodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509SKI, xmlSecDSigNs)) {
            ret = xmlSecGnuTLSX509SKINodeRead(data, cur, keyInfoCtx);
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509CRL, xmlSecDSigNs)) {
            ret = xmlSecGnuTLSX509CRLNodeRead(data, cur, keyInfoCtx);
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
xmlSecGnuTLSX509CertificateNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar *content;
    gnutls_x509_crt_t cert;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
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

    cert = xmlSecGnuTLSX509CertBase64DerRead(content);
    if(cert == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSX509CertBase64DerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(content);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crt_deinit(cert);
        xmlFree(content);
        return(-1);
    }

    xmlFree(content);
    return(0);
}

static int
xmlSecGnuTLSX509CertificateNodeWrite(gnutls_x509_crt_t cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar* buf;
    xmlNodePtr cur;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    /* set base64 lines size from context */
    buf = xmlSecGnuTLSX509CertBase64DerWrite(cert, keyInfoCtx->base64LineSize);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertBase64DerWrite",
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



#define XMLSEC_GNUTLS_IS_SPACE(ch) \
    (((ch) == ' ') || ((ch) == '\r') || ((ch) == '\n'))

static void
xmlSecGnuTLSX509Trim(xmlChar * str) {
    xmlChar * p, * q;

    xmlSecAssert(str != NULL);

    /* skip spaces from the beggining */
    p = str;
    while(XMLSEC_GNUTLS_IS_SPACE(*p) && ((*p) != '\0')) ++p;
    if(p != str) {
        for(q = str; ; ++q, ++p) {
            (*q) = (*p);
            if((*p) == '\0') {
                break;
            }
        }
    }

    /* skip spaces from the end */
    for(p = str; (*p) != '\0'; ++p);
    while((p > str) && (XMLSEC_GNUTLS_IS_SPACE(*(p - 1)))) *(--p) = '\0';
}

static int
xmlSecGnuTLSX509SubjectNameNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataStorePtr x509Store;
    xmlChar* subject;
    gnutls_x509_crt_t cert;
    gnutls_x509_crt_t cert2;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecGnuTLSX509StoreId);
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

    xmlSecGnuTLSX509Trim(subject);
    cert = xmlSecGnuTLSX509StoreFindCert(x509Store, subject, NULL, NULL, NULL, keyInfoCtx);
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

    cert2 = xmlSecGnuTLSX509CertDup(cert);
    if(cert2 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSX509CertDup",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);

        xmlFree(subject);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509AdoptCert(data, cert2);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crt_deinit(cert2);
        xmlFree(subject);
        return(-1);
    }

    xmlFree(subject);
    return(0);
}

static int
xmlSecGnuTLSX509SubjectNameNodeWrite(gnutls_x509_crt_t cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx ATTRIBUTE_UNUSED) {
    xmlChar* buf = NULL;
    xmlNodePtr cur = NULL;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    /* add node */
    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509SubjectName, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
            NULL,
            "xmlSecEnsureEmptyChild",
            XMLSEC_ERRORS_R_XMLSEC_FAILED,
            "node=%s",
            xmlSecErrorsSafeString(xmlSecNodeX509SubjectName));
        return(-1);
    }

    /* get subject */
    buf = xmlSecGnuTLSX509CertGetSubjectDN(cert);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
            NULL,
            "xmlSecGnuTLSX509CertGetSubjectDN",
            XMLSEC_ERRORS_R_XMLSEC_FAILED,
            XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* set value */
    xmlSecNodeEncodeAndSetContent(cur, buf);

    /* done */
    xmlFree(buf);
    return(0);
}

static int
xmlSecGnuTLSX509IssuerSerialNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataStorePtr x509Store;
    xmlNodePtr cur;
    xmlChar *issuerName;
    xmlChar *issuerSerial;
    gnutls_x509_crt_t cert;
    gnutls_x509_crt_t cert2;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecGnuTLSX509StoreId);
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

    xmlSecGnuTLSX509Trim(issuerName);
    xmlSecGnuTLSX509Trim(issuerSerial);
    cert = xmlSecGnuTLSX509StoreFindCert(x509Store, NULL, issuerName, issuerSerial, NULL, keyInfoCtx);
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

    cert2 = xmlSecGnuTLSX509CertDup(cert);
    if(cert2 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSX509CertDup",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(issuerSerial);
        xmlFree(issuerName);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509AdoptCert(data, cert2);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crt_deinit(cert2);
        xmlFree(issuerSerial);
        xmlFree(issuerName);
        return(-1);
    }

    xmlFree(issuerSerial);
    xmlFree(issuerName);
    return(0);
}

static int
xmlSecGnuTLSX509IssuerSerialNodeWrite(gnutls_x509_crt_t cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx ATTRIBUTE_UNUSED) {
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
    buf = xmlSecGnuTLSX509CertGetIssuerDN(cert);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertGetIssuerDN",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecNodeEncodeAndSetContent(issuerNameNode, buf);
    xmlFree(buf);

    buf = xmlSecGnuTLSX509CertGetIssuerSerial(cert);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertGetIssuerSerial",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    xmlSecNodeEncodeAndSetContent(issuerNumberNode, buf);
    xmlFree(buf);

    return(0);
}


static int
xmlSecGnuTLSX509SKINodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataStorePtr x509Store;
    xmlChar* ski;
    gnutls_x509_crt_t cert;
    gnutls_x509_crt_t cert2;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecGnuTLSX509StoreId);
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

    xmlSecGnuTLSX509Trim(ski);
    cert = xmlSecGnuTLSX509StoreFindCert(x509Store, NULL, NULL, NULL, ski, keyInfoCtx);
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

    cert2 = xmlSecGnuTLSX509CertDup(cert);
    if(cert2 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSX509CertDup",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(ski);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509AdoptCert(data, cert2);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crt_deinit(cert2);
        xmlFree(ski);
        return(-1);
    }

    xmlFree(ski);
    return(0);
}

static int
xmlSecGnuTLSX509SKINodeWrite(gnutls_x509_crt_t cert, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx ATTRIBUTE_UNUSED) {
    xmlChar *buf = NULL;
    xmlNodePtr cur = NULL;

    xmlSecAssert2(cert != NULL, -1);
    xmlSecAssert2(node != NULL, -1);

    /* add node */
    cur = xmlSecEnsureEmptyChild(node, xmlSecNodeX509SKI, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecEnsureEmptyChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "new_node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509SKI));
        return(-1);
    }

    /* write value */
    buf = xmlSecGnuTLSX509CertGetSKI(cert);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertGetSKI",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    xmlSecNodeEncodeAndSetContent(cur, buf);
    xmlFree(buf);

    return(0);
}

static int
xmlSecGnuTLSX509CRLNodeRead(xmlSecKeyDataPtr data, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar *content;
    gnutls_x509_crl_t crl;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
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

    crl = xmlSecGnuTLSX509CrlBase64DerRead(content);
    if(crl == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSX509CrlBase64DerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(content);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509AdoptCrl(data, crl);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGnuTLSKeyDataX509AdoptCrl",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crl_deinit(crl);
        xmlFree(content);
        return(-1);
    }

    xmlFree(content);
    return(0);
}

static int
xmlSecGnuTLSX509CRLNodeWrite(gnutls_x509_crl_t crl, xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlChar* buf = NULL;
    xmlNodePtr cur = NULL;

    xmlSecAssert2(crl != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    /* set base64 lines size from context */
    buf = xmlSecGnuTLSX509CrlBase64DerWrite(crl, keyInfoCtx->base64LineSize);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CrlBase64DerWrite",
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
xmlSecGnuTLSKeyDataX509VerifyAndExtractKey(xmlSecKeyDataPtr data, xmlSecKeyPtr key,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecGnuTLSX509DataCtxPtr ctx;
    xmlSecKeyDataStorePtr x509Store;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataX509Id), -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);
    xmlSecAssert2(keyInfoCtx->keysMngr != NULL, -1);

    ctx = xmlSecGnuTLSX509DataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    x509Store = xmlSecKeysMngrGetDataStore(keyInfoCtx->keysMngr, xmlSecGnuTLSX509StoreId);
    if(x509Store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeysMngrGetDataStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((ctx->keyCert == NULL) && (xmlSecPtrListGetSize(&(ctx->certsList)) > 0) && (xmlSecKeyGetValue(key) == NULL)) {
        gnutls_x509_crt_t cert;

        cert = xmlSecGnuTLSX509StoreVerify(x509Store, &(ctx->certsList), &(ctx->crlsList), keyInfoCtx);
        if(cert != NULL) {
            xmlSecKeyDataPtr keyValue;

            ctx->keyCert = xmlSecGnuTLSX509CertDup(cert);
            if(ctx->keyCert == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "xmlSecGnuTLSX509CertDup",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }

            keyValue = xmlSecGnuTLSX509CertGetKey(ctx->keyCert);
            if(keyValue == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "xmlSecGnuTLSX509CertGetKey",
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

            /* get expiration time */
            key->notValidBefore = gnutls_x509_crt_get_activation_time(ctx->keyCert);
            if(key->notValidBefore == (time_t)-1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "gnutls_x509_crt_get_activation_time",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
            }
            key->notValidAfter = gnutls_x509_crt_get_expiration_time(ctx->keyCert);
            if(key->notValidAfter == (time_t)-1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                            "gnutls_x509_crt_get_expiration_time",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                return(-1);
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

/**
 * xmlSecGnuTLSX509CertGetKey:
 * @cert:               the certificate.
 *
 * Extracts public key from the @cert.
 *
 * Returns: public key value or NULL if an error occurs.
 */
xmlSecKeyDataPtr
xmlSecGnuTLSX509CertGetKey(gnutls_x509_crt_t cert) {
    xmlSecKeyDataPtr data;
    int alg;
    unsigned int bits;
    int err;
    int ret;

    xmlSecAssert2(cert != NULL, NULL);

    alg = gnutls_x509_crt_get_pk_algorithm(cert, &bits);
    if(alg < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_pk_algorithm",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(alg));
        return(NULL);
    }

    switch(alg) {
#ifndef XMLSEC_NO_RSA
    case GNUTLS_PK_RSA:
        {
            gnutls_datum_t m, e;

            data = xmlSecKeyDataCreate(xmlSecGnuTLSKeyDataRsaId);
            if(data == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecKeyDataCreate",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "xmlSecGnuTLSKeyDataRsaId");
                return(NULL);
            }

            err = gnutls_x509_crt_get_pk_rsa_raw(cert, &m, &e);
            if(err != GNUTLS_E_SUCCESS) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "gnutls_x509_crt_get_pk_rsa_raw",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_GNUTLS_REPORT_ERROR(err));
                return(NULL);
            }

            ret = xmlSecGnuTLSKeyDataRsaAdoptPublicKey(data, &m, &e);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecGnuTLSKeyDataRsaAdoptPublicKey",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                gnutls_free(m.data);
                gnutls_free(e.data);
                return(NULL);
            }
            /* m and e are owned by data now */
        }
        break;
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_DSA
    case GNUTLS_PK_DSA:
        {
            gnutls_datum_t p, q, g, y;

            data = xmlSecKeyDataCreate(xmlSecGnuTLSKeyDataDsaId);
            if(data == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecKeyDataCreate",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            "xmlSecGnuTLSKeyDataDsaId");
                return(NULL);
            }

            err = gnutls_x509_crt_get_pk_dsa_raw(cert, &p, &q, &g, &y);
            if(err != GNUTLS_E_SUCCESS) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "gnutls_x509_crt_get_pk_dsa_raw",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_GNUTLS_REPORT_ERROR(err));
                return(NULL);
            }

            ret = xmlSecGnuTLSKeyDataDsaAdoptPublicKey(data, &p, &q, &g, &y);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "xmlSecGnuTLSKeyDataDsaAdoptPublicKey",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                gnutls_free(p.data);
                gnutls_free(q.data);
                gnutls_free(g.data);
                gnutls_free(y.data);
                return(NULL);
            }
            /* p, q, g and y are owned by data now */
        }
        break;
#endif /* XMLSEC_NO_DSA */

    default:
        {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gnutls_x509_crt_get_pk_algorithm",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "Unsupported algorithm %d", (int)alg);
            return(NULL);
        }
    }

    /* data */
    return(data);
}


/**************************************************************************
 *
 * Raw X509 Certificate processing
 *
 *
 *************************************************************************/
static int              xmlSecGnuTLSKeyDataRawX509CertBinRead  (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 const xmlSecByte* buf,
                                                                 xmlSecSize bufSize,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);

static xmlSecKeyDataKlass xmlSecGnuTLSKeyDataRawX509CertKlass = {
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
    xmlSecGnuTLSKeyDataRawX509CertBinRead,     /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                                       /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    NULL,                                       /* xmlSecKeyDataDebugDumpMethod debugDump; */
    NULL,                                       /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGnuTLSKeyDataRawX509CertGetKlass:
 *
 * The raw X509 certificates key data klass.
 *
 * Returns: raw X509 certificates key data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataRawX509CertGetKlass(void) {
    return(&xmlSecGnuTLSKeyDataRawX509CertKlass);
}

static int
xmlSecGnuTLSKeyDataRawX509CertBinRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    const xmlSecByte* buf, xmlSecSize bufSize,
                                    xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data;
    gnutls_x509_crt_t cert;
    int ret;

    xmlSecAssert2(id == xmlSecGnuTLSKeyDataRawX509CertId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(bufSize > 0, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    cert = xmlSecGnuTLSX509CertRead(buf, bufSize, xmlSecKeyDataFormatCertDer);
    if(cert == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    data = xmlSecKeyEnsureData(key, xmlSecGnuTLSKeyDataX509Id);
    if(data == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyEnsureData",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crt_deinit(cert);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509AdoptCert(data, cert);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGnuTLSKeyDataX509AdoptCert",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gnutls_x509_crt_deinit(cert);
        return(-1);
    }

    ret = xmlSecGnuTLSKeyDataX509VerifyAndExtractKey(data, key, keyInfoCtx);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGnuTLSKeyDataX509VerifyAndExtractKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(0);
}

#endif /* XMLSEC_NO_X509 */

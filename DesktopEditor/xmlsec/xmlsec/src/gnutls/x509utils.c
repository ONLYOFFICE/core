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



#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <gnutls/pkcs12.h>

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


/**************************************************************************
 *
 * X509 crt list
 *
 *****************************************************************************/
static xmlSecPtr        xmlSecGnuTLSX509CrtListDuplicateItem    (xmlSecPtr ptr);
static void             xmlSecGnuTLSX509CrtListDestroyItem      (xmlSecPtr ptr);
static void             xmlSecGnuTLSX509CrtListDebugDumpItem    (xmlSecPtr ptr,
                                                                 FILE* output);
static void             xmlSecGnuTLSX509CrtListDebugXmlDumpItem (xmlSecPtr ptr,
                                                                 FILE* output);

static xmlSecPtrListKlass xmlSecGnuTLSX509CrtListKlass = {
    BAD_CAST "gnutls-x509-crt-list",
    xmlSecGnuTLSX509CrtListDuplicateItem,       /* xmlSecPtrDuplicateItemMethod duplicateItem; */
    xmlSecGnuTLSX509CrtListDestroyItem,         /* xmlSecPtrDestroyItemMethod destroyItem; */
    xmlSecGnuTLSX509CrtListDebugDumpItem,       /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    xmlSecGnuTLSX509CrtListDebugXmlDumpItem,    /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};

xmlSecPtrListId
xmlSecGnuTLSX509CrtListGetKlass(void) {
    return(&xmlSecGnuTLSX509CrtListKlass);
}

static xmlSecPtr
xmlSecGnuTLSX509CrtListDuplicateItem(xmlSecPtr ptr) {
    xmlSecAssert2(ptr != NULL, NULL);

    return xmlSecGnuTLSX509CertDup((gnutls_x509_crt_t)ptr);
}

static void
xmlSecGnuTLSX509CrtListDestroyItem(xmlSecPtr ptr) {
    xmlSecAssert(ptr != NULL);

    gnutls_x509_crt_deinit((gnutls_x509_crt_t)ptr);
}

static void
xmlSecGnuTLSX509CrtListDebugDumpItem(xmlSecPtr ptr, FILE* output) {
    xmlSecAssert(ptr != NULL);
    xmlSecAssert(output != NULL);

    xmlSecGnuTLSX509CertDebugDump((gnutls_x509_crt_t)ptr, output);
}


static void
xmlSecGnuTLSX509CrtListDebugXmlDumpItem(xmlSecPtr ptr, FILE* output) {
    xmlSecAssert(ptr != NULL);
    xmlSecAssert(output != NULL);

    xmlSecGnuTLSX509CertDebugXmlDump((gnutls_x509_crt_t)ptr, output);
}

/**************************************************************************
 *
 * X509 crl list
 *
 *****************************************************************************/
static xmlSecPtr        xmlSecGnuTLSX509CrlListDuplicateItem    (xmlSecPtr ptr);
static void             xmlSecGnuTLSX509CrlListDestroyItem      (xmlSecPtr ptr);
static void             xmlSecGnuTLSX509CrlListDebugDumpItem    (xmlSecPtr ptr,
                                                                 FILE* output);
static void             xmlSecGnuTLSX509CrlListDebugXmlDumpItem (xmlSecPtr ptr,
                                                                 FILE* output);

static xmlSecPtrListKlass xmlSecGnuTLSX509CrlListKlass = {
    BAD_CAST "gnutls-x509-crl-list",
    xmlSecGnuTLSX509CrlListDuplicateItem,       /* xmlSecPtrDuplicateItemMethod duplicateItem; */
    xmlSecGnuTLSX509CrlListDestroyItem,         /* xmlSecPtrDestroyItemMethod destroyItem; */
    xmlSecGnuTLSX509CrlListDebugDumpItem,       /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    xmlSecGnuTLSX509CrlListDebugXmlDumpItem,    /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};

xmlSecPtrListId
xmlSecGnuTLSX509CrlListGetKlass(void) {
    return(&xmlSecGnuTLSX509CrlListKlass);
}

static xmlSecPtr
xmlSecGnuTLSX509CrlListDuplicateItem(xmlSecPtr ptr) {
    xmlSecAssert2(ptr != NULL, NULL);

    return xmlSecGnuTLSX509CrlDup((gnutls_x509_crl_t)ptr);
}

static void
xmlSecGnuTLSX509CrlListDestroyItem(xmlSecPtr ptr) {
    xmlSecAssert(ptr != NULL);

    gnutls_x509_crl_deinit((gnutls_x509_crl_t)ptr);
}

static void
xmlSecGnuTLSX509CrlListDebugDumpItem(xmlSecPtr ptr, FILE* output) {
    xmlSecAssert(ptr != NULL);
    xmlSecAssert(output != NULL);

    xmlSecGnuTLSX509CrlDebugDump((gnutls_x509_crl_t)ptr, output);
}


static void
xmlSecGnuTLSX509CrlListDebugXmlDumpItem(xmlSecPtr ptr, FILE* output) {
    xmlSecAssert(ptr != NULL);
    xmlSecAssert(output != NULL);

    xmlSecGnuTLSX509CrlDebugXmlDump((gnutls_x509_crl_t)ptr, output);
}

/*************************************************************************
 *
 * x509 certs utils/helpers
 *
 ************************************************************************/

/* HACK: gnutls doesn't have cert duplicate function, so we simply 
 write cert out and then read it back */
gnutls_x509_crt_t
xmlSecGnuTLSX509CertDup(gnutls_x509_crt_t src) {
    xmlChar * buf = NULL;
    gnutls_x509_crt_t res = NULL;

    xmlSecAssert2(src != NULL, NULL);

    buf = xmlSecGnuTLSX509CertBase64DerWrite(src, 0);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertBase64DerWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return (NULL);
    }

    res = xmlSecGnuTLSX509CertBase64DerRead(buf);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CertBase64DerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(buf);
        return (NULL);
    }

    /* done */
    xmlFree(buf);
    return (res);
}

xmlChar *
xmlSecGnuTLSX509CertGetSubjectDN(gnutls_x509_crt_t cert) {
    char* buf = NULL;
    size_t bufSize = 0;
    int err;

    xmlSecAssert2(cert != NULL, NULL);

    /* get subject size */
    err = gnutls_x509_crt_get_dn(cert, NULL, &bufSize);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_dn",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (char *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* finally write it out */
    err = gnutls_x509_crt_get_dn(cert, buf, &bufSize);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_dn",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    return(BAD_CAST buf);
}

xmlChar *
xmlSecGnuTLSX509CertGetIssuerDN(gnutls_x509_crt_t cert) {
    char* buf = NULL;
    size_t bufSize = 0;
    int err;

    xmlSecAssert2(cert != NULL, NULL);

    /* get issuer size */
    err = gnutls_x509_crt_get_issuer_dn(cert, NULL, &bufSize);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_issuer_dn",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (char *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* finally write it out */
    err = gnutls_x509_crt_get_issuer_dn(cert, buf, &bufSize);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_issuer_dn",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    return(BAD_CAST buf);
}

xmlChar *
xmlSecGnuTLSX509CertGetIssuerSerial(gnutls_x509_crt_t cert) {
    xmlChar * res = NULL;
    unsigned char* buf = NULL;
    size_t bufSize = 0;
    int err;

    xmlSecAssert2(cert != NULL, NULL);

    /* get issuer serial size */
    err = gnutls_x509_crt_get_serial(cert, NULL, &bufSize);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_serial",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (unsigned char *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* write it out */
    err = gnutls_x509_crt_get_serial(cert, buf, &bufSize);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_serial",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* convert to string */
    res = xmlSecGnuTLSASN1IntegerWrite(buf, bufSize);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSASN1IntegerWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    xmlFree(buf);
    return(res);
}

xmlChar *
xmlSecGnuTLSX509CertGetSKI(gnutls_x509_crt_t cert) {
    xmlChar * res = NULL;
    xmlSecByte* buf = NULL;
    size_t bufSize = 0;
    unsigned int critical = 0;
    int err;

    xmlSecAssert2(cert != NULL, NULL);

    /* get ski size */
    err = gnutls_x509_crt_get_subject_key_id(cert, NULL, &bufSize, &critical);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_subject_key_id",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (xmlSecByte *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* write it out */
    err = gnutls_x509_crt_get_subject_key_id(cert, buf, &bufSize, &critical);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_get_subject_key_id",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* convert to string */
    res = xmlSecBase64Encode(buf, bufSize, 0);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    xmlFree(buf);
    return(res);
}


gnutls_x509_crt_t
xmlSecGnuTLSX509CertBase64DerRead(xmlChar* buf) {
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

    return(xmlSecGnuTLSX509CertRead((const xmlSecByte*)buf, ret, xmlSecKeyDataFormatCertDer));
}

gnutls_x509_crt_t
xmlSecGnuTLSX509CertRead(const xmlSecByte* buf, xmlSecSize size, xmlSecKeyDataFormat format) {
    gnutls_x509_crt_t cert = NULL;
    gnutls_x509_crt_fmt_t fmt;
    gnutls_datum_t data;
    int err;

    xmlSecAssert2(buf != NULL, NULL);
    xmlSecAssert2(size > 0, NULL);

    /* figure out format */
    switch(format) {
    case xmlSecKeyDataFormatPem:
    case xmlSecKeyDataFormatCertPem:
        fmt = GNUTLS_X509_FMT_PEM;
        break;
    case xmlSecKeyDataFormatDer:
    case xmlSecKeyDataFormatCertDer:
        fmt = GNUTLS_X509_FMT_DER;
        break;
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", format);
        return(NULL);
    }

    /* read cert */
    err = gnutls_x509_crt_init(&cert);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_init",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    data.data = (unsigned char*)buf;
    data.size = size;
    err = gnutls_x509_crt_import(cert, &data, fmt);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_import",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        gnutls_x509_crt_deinit(cert);
        return(NULL);
    }

    return(cert);
}

xmlChar*
xmlSecGnuTLSX509CertBase64DerWrite(gnutls_x509_crt_t cert, int base64LineWrap) {
    xmlChar * res = NULL;
    xmlSecByte* buf = NULL;
    size_t bufSize = 0;
    int err;

    xmlSecAssert2(cert != NULL, NULL);

    /* get size */
    err = gnutls_x509_crt_export(cert, GNUTLS_X509_FMT_DER, NULL, &bufSize);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_export(GNUTLS_X509_FMT_DER)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (xmlSecByte *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* write it out */
    err = gnutls_x509_crt_export(cert, GNUTLS_X509_FMT_DER, buf, &bufSize);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crt_export(GNUTLS_X509_FMT_DER)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* convert to string */
    res = xmlSecBase64Encode(buf, bufSize, base64LineWrap);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    xmlFree(buf);
    return(res);
}

void
xmlSecGnuTLSX509CertDebugDump(gnutls_x509_crt_t cert, FILE* output) {
    xmlChar * buf;

    xmlSecAssert(cert != NULL);
    xmlSecAssert(output != NULL);

    buf = xmlSecGnuTLSX509CertGetSubjectDN(cert);
    if(buf != NULL) {
        fprintf(output, "==== Subject Name: %s\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "==== Subject Name: unknown\n");
    }

    buf = xmlSecGnuTLSX509CertGetIssuerDN(cert);
    if(buf != NULL) {
        fprintf(output, "==== Issuer Name: %s\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "==== Issuer Name: unknown\n");
    }

    buf = xmlSecGnuTLSX509CertGetIssuerSerial(cert);
    if(buf != NULL) {
        fprintf(output, "==== Issuer Serial: %s\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "==== Issuer Serial: unknown\n");
    }
}

void
xmlSecGnuTLSX509CertDebugXmlDump(gnutls_x509_crt_t cert, FILE* output) {
    xmlChar * buf;

    xmlSecAssert(cert != NULL);
    xmlSecAssert(output != NULL);

    buf = xmlSecGnuTLSX509CertGetSubjectDN(cert);
    if(buf != NULL) {
        fprintf(output, "<SubjectName>%s</SubjectName>\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "<SubjectName>unknown</SubjectName>\n");
    }

    buf = xmlSecGnuTLSX509CertGetIssuerDN(cert);
    if(buf != NULL) {
        fprintf(output, "<IssuerName>%s</IssuerName>\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "<IssuerName>unknown</IssuerName>\n");
    }

    buf = xmlSecGnuTLSX509CertGetIssuerSerial(cert);
    if(buf != NULL) {
        fprintf(output, "<SerialNumber>%s</SerialNumber>\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "<SerialNumber>unknown</SerialNumber>\n");
    }
}

/*************************************************************************
 *
 * x509 crls utils/helpers
 *
 ************************************************************************/

/* HACK: gnutls doesn't have crl duplicate function, so we simply 
 write crl out and then read it back */
gnutls_x509_crl_t
xmlSecGnuTLSX509CrlDup(gnutls_x509_crl_t src) {
    xmlChar * buf = NULL;
    gnutls_x509_crl_t res = NULL;

    xmlSecAssert2(src != NULL, NULL);

    buf = xmlSecGnuTLSX509CrlBase64DerWrite(src, 0);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CrlBase64DerWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return (NULL);
    }

    res = xmlSecGnuTLSX509CrlBase64DerRead(buf);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSX509CrlBase64DerRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(buf);
        return (NULL);
    }

    /* done */
    xmlFree(buf);
    return (res);
}

xmlChar *
xmlSecGnuTLSX509CrlGetIssuerDN(gnutls_x509_crl_t crl) {
    char* buf = NULL;
    size_t bufSize = 0;
    int err;

    xmlSecAssert2(crl != NULL, NULL);

    /* get issuer size */
    err = gnutls_x509_crl_get_issuer_dn(crl, NULL, &bufSize);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crl_get_issuer_dn",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (char *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* finally write it out */
    err = gnutls_x509_crl_get_issuer_dn(crl, buf, &bufSize);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crl_get_issuer_dn",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    return(BAD_CAST buf);
}

gnutls_x509_crl_t
xmlSecGnuTLSX509CrlBase64DerRead(xmlChar* buf) {
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

    return(xmlSecGnuTLSX509CrlRead((const xmlSecByte*)buf, ret, xmlSecKeyDataFormatCertDer));
}

gnutls_x509_crl_t
xmlSecGnuTLSX509CrlRead(const xmlSecByte* buf, xmlSecSize size, xmlSecKeyDataFormat format) {
    gnutls_x509_crl_t crl = NULL;
    gnutls_x509_crt_fmt_t fmt;
    gnutls_datum_t data;
    int err;

    xmlSecAssert2(buf != NULL, NULL);
    xmlSecAssert2(size > 0, NULL);

    /* figure out format */
    switch(format) {
    case xmlSecKeyDataFormatPem:
    case xmlSecKeyDataFormatCertPem:
        fmt = GNUTLS_X509_FMT_PEM;
        break;
    case xmlSecKeyDataFormatDer:
    case xmlSecKeyDataFormatCertDer:
        fmt = GNUTLS_X509_FMT_DER;
        break;
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", format);
        return(NULL);
    }

    /* read crl */
    err = gnutls_x509_crl_init(&crl);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crl_init",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    data.data = (unsigned char*)buf;
    data.size = size;
    err = gnutls_x509_crl_import(crl, &data, fmt);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crl_import",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        gnutls_x509_crl_deinit(crl);
        return(NULL);
    }

    return(crl);
}

xmlChar*
xmlSecGnuTLSX509CrlBase64DerWrite(gnutls_x509_crl_t crl, int base64LineWrap) {
    xmlChar * res = NULL;
    xmlSecByte* buf = NULL;
    size_t bufSize = 0;
    int err;

    xmlSecAssert2(crl != NULL, NULL);

    /* get size */
    err = gnutls_x509_crl_export(crl, GNUTLS_X509_FMT_DER, NULL, &bufSize);
    if((err != GNUTLS_E_SHORT_MEMORY_BUFFER) || (bufSize <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crl_export(GNUTLS_X509_FMT_DER)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(NULL);
    }

    /* allocate buffer */
    buf = (xmlSecByte *)xmlMalloc(bufSize + 1);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)bufSize);
        return(NULL);
    }

    /* write it out */
    err = gnutls_x509_crl_export(crl, GNUTLS_X509_FMT_DER, buf, &bufSize);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_crl_export(GNUTLS_X509_FMT_DER)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        xmlFree(buf);
        return(NULL);
    }

    /* convert to string */
    res = xmlSecBase64Encode(buf, bufSize, base64LineWrap);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFree(buf);
        return(NULL);
    }

    /* done */
    xmlFree(buf);
    return(res);
}

void
xmlSecGnuTLSX509CrlDebugDump(gnutls_x509_crl_t crl, FILE* output) {
    xmlChar * buf;

    xmlSecAssert(crl != NULL);
    xmlSecAssert(output != NULL);

    buf = xmlSecGnuTLSX509CrlGetIssuerDN(crl);
    if(buf != NULL) {
        fprintf(output, "==== Issuer Name: %s\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "==== Issuer Name: unknown\n");
    }
}

void
xmlSecGnuTLSX509CrlDebugXmlDump(gnutls_x509_crl_t crl, FILE* output) {
    xmlChar * buf;

    xmlSecAssert(crl != NULL);
    xmlSecAssert(output != NULL);

    buf = xmlSecGnuTLSX509CrlGetIssuerDN(crl);
    if(buf != NULL) {
        fprintf(output, "<IssuerName>%s</IssuerName>\n", buf);
        xmlFree(buf);
    } else {
        fprintf(output, "<IssuerName>unknown</IssuerName>\n");
    }
}

/*************************************************************************
 *
 * Misc. utils/helpers
 *
 ************************************************************************/
xmlChar*
xmlSecGnuTLSASN1IntegerWrite(const unsigned char * data, size_t len) {
    xmlChar *res = NULL;
    int resLen = 64; /* not more than 64 chars */
    unsigned long long int val = 0;
    size_t ii = 0;
    int shift = 0;

    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(len <= 9, NULL);

    /* HACK : to be fixed after GnuTLS provides a way to read opaque ASN1 integer */
    for(ii = len; ii > 0; --ii, shift += 8) {
        val |= ((unsigned long long)data[ii - 1]) << shift;
    }

    res = (xmlChar*)xmlMalloc(resLen + 1);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)resLen);
        return (NULL);
    }

    xmlSecStrPrintf(res, resLen, BAD_CAST "%llu", val);
    return(res);
}

/*************************************************************************
 *
 * pkcs12 utils/helpers
 *
 ************************************************************************/
int
xmlSecGnuTLSPkcs12LoadMemory(const xmlSecByte* data, xmlSecSize dataSize,
                             const char *pwd,
                             gnutls_x509_privkey_t * priv_key,
                             gnutls_x509_crt_t * key_cert,
                             xmlSecPtrListPtr certsList)
{
    gnutls_pkcs12_t pkcs12 = NULL;
    gnutls_pkcs12_bag_t bag = NULL;
    gnutls_x509_crt_t cert = NULL;
    gnutls_datum_t datum;
    xmlSecSize certsSize;
    int res = -1;
    int idx;
    int err;
    int ret;

    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize > 0, -1);
    xmlSecAssert2(priv_key != NULL, -1);
    xmlSecAssert2((*priv_key) == NULL, -1);
    xmlSecAssert2(key_cert!= NULL, -1);
    xmlSecAssert2((*key_cert) == NULL, -1);
    xmlSecAssert2(certsList != NULL, -1);

    /* read pkcs12 in internal structure */
    err = gnutls_pkcs12_init(&pkcs12);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_pkcs12_init",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        goto done;
    }

    datum.data = (unsigned char *)data;
    datum.size = dataSize;
    err = gnutls_pkcs12_import(pkcs12, &datum, GNUTLS_X509_FMT_DER, 0);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_pkcs12_import",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        goto done;
    }

    /* verify */
    err = gnutls_pkcs12_verify_mac(pkcs12, pwd);
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_pkcs12_verify_mac",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        goto done;
    }

    /* scan the pkcs structure and find the first private key */
    for(idx = 0; ; ++idx) {
        int bag_type;
        int elements_in_bag;
        int ii;

        err = gnutls_pkcs12_bag_init(&bag);
        if(err != GNUTLS_E_SUCCESS) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gnutls_pkcs12_bag_init",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GNUTLS_REPORT_ERROR(err));
            goto done;
        }

        err = gnutls_pkcs12_get_bag(pkcs12, idx, bag);
        if(err == GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE) {
            /* scanned the whole pkcs12, stop */
            break;
        } else if(err != GNUTLS_E_SUCCESS) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gnutls_pkcs12_get_bag",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GNUTLS_REPORT_ERROR(err));
            goto done;
        }

        /* check if we need to decrypt the bag */
        bag_type = gnutls_pkcs12_bag_get_type(bag, 0);
        if(bag_type < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gnutls_pkcs12_bag_get_type",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GNUTLS_REPORT_ERROR(bag_type));
            goto done;
        }
        if(bag_type == GNUTLS_BAG_ENCRYPTED) {
            err = gnutls_pkcs12_bag_decrypt(bag, pwd);
            if(err != GNUTLS_E_SUCCESS) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "gnutls_pkcs12_bag_decrypt",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_GNUTLS_REPORT_ERROR(err));
                goto done;
            }
        }

        /* scan elements in bag */
        elements_in_bag = gnutls_pkcs12_bag_get_count(bag);
        if(elements_in_bag < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gnutls_pkcs12_bag_get_count",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GNUTLS_REPORT_ERROR(bag_type));
            goto done;
        }
        for(ii = 0; ii < elements_in_bag; ++ii) {
            bag_type = gnutls_pkcs12_bag_get_type(bag, ii);
            if(bag_type < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "gnutls_pkcs12_bag_get_type",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_GNUTLS_REPORT_ERROR(bag_type));
                goto done;
            }

            err = gnutls_pkcs12_bag_get_data(bag, ii, &datum);
            if(err != GNUTLS_E_SUCCESS) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "gnutls_pkcs12_bag_get_data",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_GNUTLS_REPORT_ERROR(err));
                goto done;
            }

            switch(bag_type) {
            case GNUTLS_BAG_PKCS8_ENCRYPTED_KEY:
            case GNUTLS_BAG_PKCS8_KEY:
                /* we want only the first private key */
                if((*priv_key) == NULL) {
                    err = gnutls_x509_privkey_init(priv_key);
                    if(err != GNUTLS_E_SUCCESS) {
                        xmlSecError(XMLSEC_ERRORS_HERE,
                                    NULL,
                                    "gnutls_x509_privkey_init",
                                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                    XMLSEC_GNUTLS_REPORT_ERROR(err));
                        goto done;
                    }

                    err = gnutls_x509_privkey_import_pkcs8((*priv_key),
                                &datum, GNUTLS_X509_FMT_DER,
                                pwd,
                                (bag_type == GNUTLS_BAG_PKCS8_KEY) ? GNUTLS_PKCS_PLAIN : 0);
                    if(err != GNUTLS_E_SUCCESS) {
                        xmlSecError(XMLSEC_ERRORS_HERE,
                                    NULL,
                                    "gnutls_x509_privkey_import_pkcs8",
                                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                    XMLSEC_GNUTLS_REPORT_ERROR(err));
                        goto done;
                    }
                }
                break;
            case GNUTLS_BAG_CERTIFICATE:
                err = gnutls_x509_crt_init(&cert);
                if(err != GNUTLS_E_SUCCESS) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "gnutls_x509_crt_init",
                                XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                XMLSEC_GNUTLS_REPORT_ERROR(err));
                    goto done;
                }

                err = gnutls_x509_crt_import(cert, &datum, GNUTLS_X509_FMT_DER);
                if(err != GNUTLS_E_SUCCESS) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "gnutls_x509_crt_import",
                                XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                XMLSEC_GNUTLS_REPORT_ERROR(err));
                    goto done;
                }

                ret = xmlSecPtrListAdd(certsList, cert);
                if(ret < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecPtrListAdd(certsList)",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }
                cert = NULL; /* owned by certsList now */
                break;
            default:
                /* ignore unknown bag element */
                break;
            }
        }

        /* done with bag */
        gnutls_pkcs12_bag_deinit(bag);
        bag = NULL;
    }

    /* check we have private key */
    if((*priv_key) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "Private key was not found in pkcs12 object");
        goto done;
    }

    /* we will search for key cert using the key id */
    certsSize = xmlSecPtrListGetSize(certsList);
    if(certsSize > 0) {
        size_t cert_id_size = 0;
        size_t key_id_size = 0;
        xmlSecByte cert_id[100];
        xmlSecByte key_id[100];
        xmlSecSize ii;

        key_id_size = sizeof(key_id);
        err = gnutls_x509_privkey_get_key_id((*priv_key), 0, key_id, &key_id_size);
        if(err != GNUTLS_E_SUCCESS) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gnutls_x509_privkey_get_key_id",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GNUTLS_REPORT_ERROR(err));
            goto done;
        }
        for(ii = 0; ii < certsSize; ++ii) {
            gnutls_x509_crt_t tmp;

            tmp = xmlSecPtrListGetItem(certsList, ii);
            if(tmp == NULL) {
                continue;
            }

            cert_id_size = sizeof(cert_id);
            err = gnutls_x509_crt_get_key_id(tmp, 0, cert_id, &cert_id_size);
            if(err != GNUTLS_E_SUCCESS) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "gnutls_x509_crt_get_key_id",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_GNUTLS_REPORT_ERROR(err));
                goto done;
            }

            /* if key ids match, then this is THE key cert!!! */
            if((key_id_size == cert_id_size) && (memcmp(key_id, cert_id, key_id_size) == 0)) {
                (*key_cert) = xmlSecGnuTLSX509CertDup(tmp);
                if((*key_cert) == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecGnuTLSX509CertDup",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }

                break;
            }
        }

        /* check we have key cert */
        if((*key_cert) == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "Certificate for the private key was not found in pkcs12 object");
            goto done;
        }
    }


    /* success!!! */
    res = 0;

done:
    if(cert != NULL) {
        gnutls_x509_crt_deinit(cert);
    }
    if(bag != NULL) {
        gnutls_pkcs12_bag_deinit(bag);
    }
    if(pkcs12 != NULL) {
        gnutls_pkcs12_deinit(pkcs12);
    }
    return(res);
}

xmlSecKeyDataPtr
xmlSecGnuTLSCreateKeyDataAndAdoptPrivKey(gnutls_x509_privkey_t priv_key) {
    xmlSecKeyDataPtr res = NULL;
    int key_alg;
    int ret;

    xmlSecAssert2(priv_key != NULL, NULL);

    /* create key value data */
    key_alg = gnutls_x509_privkey_get_pk_algorithm(priv_key);
    if(key_alg < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_privkey_get_pk_algorithm",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(key_alg));
        return (NULL);
    }
    switch(key_alg) {
#ifndef XMLSEC_NO_RSA
    case GNUTLS_PK_RSA:
        res = xmlSecKeyDataCreate(xmlSecGnuTLSKeyDataRsaId);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGnuTLSKeyDataRsaId");
            return(NULL);
        }

        ret = xmlSecGnuTLSKeyDataRsaAdoptPrivateKey(res, priv_key);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGnuTLSKeyDataRsaAdoptPrivateKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGnuTLSKeyDataRsaId");
            xmlSecKeyDataDestroy(res);
            return(NULL);
        }
        break;
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_DSA
    case GNUTLS_PK_DSA:
        res = xmlSecKeyDataCreate(xmlSecGnuTLSKeyDataDsaId);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGnuTLSKeyDataDsaId");
            return(NULL);
        }

        ret = xmlSecGnuTLSKeyDataDsaAdoptPrivateKey(res, priv_key);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGnuTLSKeyDataDsaAdoptPrivateKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGnuTLSKeyDataDsaId");
            xmlSecKeyDataDestroy(res);
            return(NULL);
        }
        break;
#endif /* XMLSEC_NO_DSA */
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_privkey_get_pk_algorithm",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "Unsupported algorithm %d", (int)key_alg);
        return(NULL);
    }

    /* done */
    return(res);
}

/*************************************************************************
 *
 * LDAP DN parser
 *
 ************************************************************************/
void
xmlSecGnuTLSDnAttrsInitialize(xmlSecGnuTLSDnAttr * attrs, xmlSecSize attrsSize) {
    xmlSecAssert(attrs != NULL);
    xmlSecAssert(attrsSize > 0);

    memset(attrs, 0, attrsSize * sizeof(xmlSecGnuTLSDnAttr));
}

void
xmlSecGnuTLSDnAttrsDeinitialize(xmlSecGnuTLSDnAttr * attrs, xmlSecSize attrsSize) {
    xmlSecSize ii;

    xmlSecAssert(attrs != NULL);
    xmlSecAssert(attrsSize > 0);

    for(ii = 0; ii < attrsSize; ++ii) {
        if(attrs[ii].key != NULL) {
            xmlFree(attrs[ii].key);
        }
        if(attrs[ii].value != NULL) {
            xmlFree(attrs[ii].value);
        }
    }
    memset(attrs, 0, attrsSize * sizeof(xmlSecGnuTLSDnAttr));
}

const xmlSecGnuTLSDnAttr *
xmlSecGnuTLSDnAttrrsFind(const xmlSecGnuTLSDnAttr * attrs,
                         xmlSecSize attrsSize,
                         const xmlChar * key)
{
    xmlSecSize ii;

    xmlSecAssert2(attrs != NULL, NULL);
    xmlSecAssert2(attrsSize > 0, NULL);
    xmlSecAssert2(key != NULL, NULL);

    for(ii = 0; ii < attrsSize; ++ii) {
        /* simple case */
        if(xmlStrcasecmp(key, attrs[ii].key) == 0) {
            return(&(attrs[ii]));
        }

        /* special case for emailAddress (as usual) */
        if((xmlStrcasecmp(key, BAD_CAST "emailAddress") == 0) &&
           (xmlStrcasecmp(attrs[ii].key, BAD_CAST "email") == 0))
        {
            return(&(attrs[ii]));
        }
        if((xmlStrcasecmp(key, BAD_CAST "email") == 0) &&
           (xmlStrcasecmp(attrs[ii].key, BAD_CAST "emailAddress") == 0))
        {
            return(&(attrs[ii]));
        }
    }

    /* not found :( */
    return(NULL);
}

int
xmlSecGnuTLSDnAttrsEqual(const xmlSecGnuTLSDnAttr * ll, xmlSecSize llSize,
                         const xmlSecGnuTLSDnAttr * rr, xmlSecSize rrSize) 
{
    xmlSecSize llNum = 0;
    xmlSecSize rrNum = 0;
    const xmlSecGnuTLSDnAttr * tmp;
    xmlSecSize ii;

    xmlSecAssert2(ll != NULL, -1);
    xmlSecAssert2(llSize > 0, -1);
    xmlSecAssert2(rr != NULL, -1);
    xmlSecAssert2(rrSize > 0, -1);

    /* compare number of non-nullattributes */
    for(ii = 0; ii < llSize; ++ii) {
        if(ll[ii].key != NULL) {
            ++llNum;
        }
    }
    for(ii = 0; ii < rrSize; ++ii) {
        if(rr[ii].key != NULL) {
            ++rrNum;
        }
    }
    if(llNum != rrNum) {
        return(0);
    }

    /* make sure that all ll attrs are equal to rr attrs */
    for(ii = 0; ii < llSize; ++ii) {
        if(ll[ii].key == NULL) {
            continue;
        }

        tmp = xmlSecGnuTLSDnAttrrsFind(rr, rrSize, ll[ii].key);
        if(tmp == NULL) {
            return(0); /* attribute was not found */
        }

        if(!xmlStrEqual(ll[ii].value, tmp->value)) {
            return(0); /* different values */
        }
    }

    /* good!!! */
    return(1);
}

/*
Distinguished name syntax

The formal syntax for a Distinguished Name (DN) is based on RFC 2253. 
The Backus Naur Form (BNF) syntax is defined as follows:

    <name> ::= <name-component> ( <spaced-separator> )
          | <name-component> <spaced-separator> <name>

   <spaced-separator> ::= <optional-space>
                   <separator>
                   <optional-space>

   <separator> ::=  "," | ";"

   <optional-space> ::= ( <CR> ) *( " " )

   <name-component> ::= <attribute>
           | <attribute> <optional-space> "+"
             <optional-space> <name-component>

   <attribute> ::= <string>
           | <key> <optional-space> "=" <optional-space> <string>

   <key> ::= 1*( <keychar> ) | "OID." <oid> | "oid." <oid>
   <keychar> ::= letters, numbers, and space

   <oid> ::= <digitstring> | <digitstring> "." <oid>
   <digitstring> ::= 1*<digit>
   <digit> ::= digits 0-9

   <string> ::= *( <stringchar> | <pair> )
            | '"' *( <stringchar> | <special> | <pair> ) '"'
            | "#" <hex>


   <special> ::= "," | "=" | <CR> | "+" | "<" |  ">"
            | "#" | ";"

   <pair> ::= "\" ( <special> | "\" | '"')
   <stringchar> ::= any character except <special> or "\" or '"'


   <hex> ::= 2*<hexchar>
   <hexchar> ::= 0-9, a-f, A-F

A semicolon (;) character can be used to separate RDNs in a distinguished name,
although the comma (,) character is the typical notation.

White-space characters (spaces) might be present on either side of the comma or
semicolon. The white-space characters are ignored, and the semicolon is replaced
with a comma.

In addition, space (' ' ASCII 32) characters may be present either before or 
after a '+' or '='. These space characters are ignored when parsing.
*/
enum xmlSecGnuTLSDnParseState {
    xmlSecGnuTLSDnParseState_BeforeNameComponent = 0,
    xmlSecGnuTLSDnParseState_Key,
    xmlSecGnuTLSDnParseState_BeforeString,
    xmlSecGnuTLSDnParseState_String,
    xmlSecGnuTLSDnParseState_QuotedString,
    xmlSecGnuTLSDnParseState_AfterQuotedString
};

#define XMLSEC_GNUTLS_IS_SPACE(ch)      \
        (((ch) == ' ') || ((ch) == '\n') || ((ch) == '\r'))

int
xmlSecGnuTLSDnAttrsParse(const xmlChar * dn,
                         xmlSecGnuTLSDnAttr * attrs, xmlSecSize attrsSize)
{
    xmlChar * tmp = NULL;
    xmlChar * p;
    xmlChar ch;
    enum xmlSecGnuTLSDnParseState state;
    int slash;
    xmlSecSize pos;
    int res = -1;

    xmlSecAssert2(dn != NULL, -1);
    xmlSecAssert2(attrs != NULL, -1);
    xmlSecAssert2(attrsSize > 0, -1);

    /* allocate buffer, we don't need more than string */
    tmp = (xmlChar *)xmlMalloc(xmlStrlen(dn) + 1);
    if(tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)(xmlStrlen(dn) + 1));
        goto done;
    }

    /* state machine */
    state = xmlSecGnuTLSDnParseState_BeforeNameComponent;
    slash = 0;
    pos = 0;
    p = tmp;
    for(ch = (*dn); ; ch = *(++dn)) {
        switch(state) {
        case xmlSecGnuTLSDnParseState_BeforeNameComponent:
            if(!XMLSEC_GNUTLS_IS_SPACE(ch)) {
                *(p++) = ch; /* we are sure we have enough buffer */
                state = xmlSecGnuTLSDnParseState_Key;
            } else {
                /* just skip space */
            }
            break;
        case xmlSecGnuTLSDnParseState_Key:
            /* we don't support
            1) <attribute><optional-space>"+"<optional-space><name-component>
            2) <attribute> ::= <string>
            */
            if(ch != '=') {
                *(p++) = ch; /* we are sure we have enough buffer */
            } else {
                *(p) = '\0';
                /* remove spaces back */
                while((p > tmp) && (XMLSEC_GNUTLS_IS_SPACE(*(p - 1)))) {
                    *(--p) = '\0';
                }

                /* insert into the attrs */
                if(pos >= attrsSize) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "Not enough space: size=%d", (int)attrsSize);
                    goto done;
                }
                attrs[pos].key = xmlStrdup(tmp);
                if(attrs[pos].key == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlStrdup",
                                XMLSEC_ERRORS_R_MALLOC_FAILED,
                                "size=%d", (int)(xmlStrlen(tmp) + 1));
                    goto done;
                }

                state = xmlSecGnuTLSDnParseState_BeforeString;
                p = tmp;
            }
            break;
        case xmlSecGnuTLSDnParseState_BeforeString:
            if(!XMLSEC_GNUTLS_IS_SPACE(ch)) {
                if(ch != '\"') {
                    state = xmlSecGnuTLSDnParseState_String;
                    slash = 0;
                    --dn; /* small hack, so we can look at the same char 
                           again with the correct state */
                } else {
                    state = xmlSecGnuTLSDnParseState_QuotedString;
                    slash = 0;
                }
            } else {
                /* just skip space */
            }
            break;
        case xmlSecGnuTLSDnParseState_String:
            if(slash == 1) {
                *(p++) = ch; /* we are sure we have enough buffer */
                slash = 0;
            } else if(ch == '\\') {
                slash = 1;
            } else if((ch == ',') || (ch == ';') || (ch == '\0')) {
                *(p) = '\0';
                /* remove spaces back */
                while((p > tmp) && (XMLSEC_GNUTLS_IS_SPACE(*(p - 1)))) {
                    *(--p) = '\0';
                }

                attrs[pos].value = xmlStrdup(tmp);
                if(attrs[pos].value == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlStrdup",
                                XMLSEC_ERRORS_R_MALLOC_FAILED,
                                "size=%d", (int)(xmlStrlen(tmp) + 1));
                    goto done;
                }
                state = xmlSecGnuTLSDnParseState_BeforeNameComponent;
                ++pos;
                p = tmp;
            } else {
                *(p++) = ch; /* we are sure we have enough buffer */
            }
            break;
        case xmlSecGnuTLSDnParseState_QuotedString:
            if(slash == 1) {
                *(p++) = ch; /* we are sure we have enough buffer */
                slash = 0;
            } else if(ch == '\\') {
                slash = 1;
            } else if(ch == '\"') {
                *(p) = '\0'; 
                /* don't remove spaces for quoted string */

                attrs[pos].value = xmlStrdup(tmp);
                if(attrs[pos].value == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlStrdup",
                                XMLSEC_ERRORS_R_MALLOC_FAILED,
                                "size=%d", (int)(xmlStrlen(tmp) + 1));
                    goto done;
                }
                state = xmlSecGnuTLSDnParseState_AfterQuotedString;
                ++pos;
                p = tmp;
            } else {
                *(p++) = ch; /* we are sure we have enough buffer */
            }
            break;
        case xmlSecGnuTLSDnParseState_AfterQuotedString:
            if(!XMLSEC_GNUTLS_IS_SPACE(ch)) {
                if((ch == ',') || (ch == ';') || (ch == '\0')) {
                    state = xmlSecGnuTLSDnParseState_BeforeNameComponent;
                } else {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "Unexpected character %c (expected space or ',' or ';')",
                                ch);
                    goto done;
                }
            } else {
                /* just skip space */
            }
            break;
        }

        if(ch == '\0') {
            /* done */
            break;
        }
    }

    /* check end state */
    if(state != xmlSecGnuTLSDnParseState_BeforeNameComponent) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "Unexpected state %d at the end of parsing",
                    (int)state);
        goto done;
    }

    /* debug
    {
        xmlSecSize ii;
        for(ii = 0; ii < attrsSize; ++ii) {
            if(attrs[ii].key != NULL) {
                printf("DEBUG: attrs - %s=>%s\n", attrs[ii].key, attrs[ii].value);
            }
        }
    }
    */

    /* done */
    res = 0;

done:
    if(tmp != NULL) {
        xmlFree(tmp);
    }
    return(res);
}


#endif /* XMLSEC_NO_X509 */




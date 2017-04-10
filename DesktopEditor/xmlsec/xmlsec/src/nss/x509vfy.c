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

#include <cert.h>
#include <secerr.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/base64.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/x509.h>

/**************************************************************************
 *
 * Internal NSS X509 store CTX
 *
 *************************************************************************/
typedef struct _xmlSecNssX509StoreCtx           xmlSecNssX509StoreCtx,
                                                *xmlSecNssX509StoreCtxPtr;
struct _xmlSecNssX509StoreCtx {
    /* Two uses:
     *
     * 1) Just keeping a reference to destroy later.
     *
     * 2) NSS doesn't update it's cache correctly when new certs are added 
     *          https://bugzilla.mozilla.org/show_bug.cgi?id=211051
     *    we use this list to perform search ourselves.
     */

    CERTCertList* certsList; /* just keeping a reference to destroy later */
};

/****************************************************************************
 *
 * xmlSecNssKeyDataStoreX509Id:
 *
 * xmlSecNssX509StoreCtx is located after xmlSecTransform
 *
 ***************************************************************************/
#define xmlSecNssX509StoreGetCtx(store) \
    ((xmlSecNssX509StoreCtxPtr)(((xmlSecByte*)(store)) + \
                                    sizeof(xmlSecKeyDataStoreKlass)))
#define xmlSecNssX509StoreSize  \
    (sizeof(xmlSecKeyDataStoreKlass) + sizeof(xmlSecNssX509StoreCtx))

static int              xmlSecNssX509StoreInitialize    (xmlSecKeyDataStorePtr store);
static void             xmlSecNssX509StoreFinalize      (xmlSecKeyDataStorePtr store);
static int              xmlSecNssX509NameStringRead     (xmlSecByte **str,
                                                         int *strLen,
                                                         xmlSecByte *res,
                                                         int resLen,
                                                         xmlSecByte delim,
                                                         int ingoreTrailingSpaces);
static xmlSecByte *     xmlSecNssX509NameRead           (xmlSecByte *str,
                                                         int len);

static int              xmlSecNssNumToItem              (SECItem *it, 
                                                         PRUint64 num);


static xmlSecKeyDataStoreKlass xmlSecNssX509StoreKlass = {
    sizeof(xmlSecKeyDataStoreKlass),
    xmlSecNssX509StoreSize,

    /* data */
    xmlSecNameX509Store,                        /* const xmlChar* name; */

    /* constructors/destructor */
    xmlSecNssX509StoreInitialize,               /* xmlSecKeyDataStoreInitializeMethod initialize; */
    xmlSecNssX509StoreFinalize,                 /* xmlSecKeyDataStoreFinalizeMethod finalize; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

static CERTCertificate*         xmlSecNssX509FindCert(CERTCertList* certsList,
                                                      const xmlChar *subjectName,
                                                      const xmlChar *issuerName,
                                                      const xmlChar *issuerSerial,
                                                      xmlChar *ski);


/**
 * xmlSecNssX509StoreGetKlass:
 *
 * The NSS X509 certificates key data store klass.
 *
 * Returns: pointer to NSS X509 certificates key data store klass.
 */
xmlSecKeyDataStoreId
xmlSecNssX509StoreGetKlass(void) {
    return(&xmlSecNssX509StoreKlass);
}

/**
 * xmlSecNssX509StoreFindCert:
 * @store:              the pointer to X509 key data store klass.
 * @subjectName:        the desired certificate name.
 * @issuerName:         the desired certificate issuer name.
 * @issuerSerial:       the desired certificate issuer serial number.
 * @ski:                the desired certificate SKI.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Searches @store for a certificate that matches given criteria.
 *
 * Returns: pointer to found certificate or NULL if certificate is not found
 * or an error occurs.
 */
CERTCertificate *
xmlSecNssX509StoreFindCert(xmlSecKeyDataStorePtr store, xmlChar *subjectName,
                                xmlChar *issuerName, xmlChar *issuerSerial,
                                xmlChar *ski, xmlSecKeyInfoCtx* keyInfoCtx) {
    xmlSecNssX509StoreCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecNssX509StoreId), NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);

    ctx = xmlSecNssX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, NULL);

    return xmlSecNssX509FindCert(ctx->certsList, subjectName, issuerName, issuerSerial, ski);
}

/**
 * xmlSecNssX509StoreVerify:
 * @store:              the pointer to X509 key data store klass.
 * @certs:              the untrusted certificates stack.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Verifies @certs list.
 *
 * Returns: pointer to the first verified certificate from @certs.
 */
CERTCertificate *
xmlSecNssX509StoreVerify(xmlSecKeyDataStorePtr store, CERTCertList* certs,
                         xmlSecKeyInfoCtx* keyInfoCtx) {
    xmlSecNssX509StoreCtxPtr ctx;
    CERTCertListNode*       head;
    CERTCertificate*       cert = NULL;
    CERTCertListNode*       head1;
    CERTCertificate*       cert1 = NULL;
    SECStatus status = SECFailure;
    int64 timeboundary;
    int64 tmp1, tmp2;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecNssX509StoreId), NULL);
    xmlSecAssert2(certs != NULL, NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);

    ctx = xmlSecNssX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, NULL);

    for (head = CERT_LIST_HEAD(certs);
         !CERT_LIST_END(head, certs);
         head = CERT_LIST_NEXT(head)) {
        cert = head->cert;
        if(keyInfoCtx->certsVerificationTime > 0) {
            /* convert the time since epoch in seconds to microseconds */
            LL_UI2L(timeboundary, keyInfoCtx->certsVerificationTime);
            tmp1 = (int64)PR_USEC_PER_SEC;
            tmp2 = timeboundary;
            LL_MUL(timeboundary, tmp1, tmp2);
        } else {
            timeboundary = PR_Now();
        }

        /* if cert is the issuer of any other cert in the list, then it is
         * to be skipped */
        for (head1 = CERT_LIST_HEAD(certs);
             !CERT_LIST_END(head1, certs);
             head1 = CERT_LIST_NEXT(head1)) {

            cert1 = head1->cert;
            if (cert1 == cert) {
                continue;
            }

            if (SECITEM_CompareItem(&cert1->derIssuer, &cert->derSubject)
                                      == SECEqual) {
                break;
            }
        }

        if (!CERT_LIST_END(head1, certs)) {
            continue;
        }

        status = CERT_VerifyCertificate(CERT_GetDefaultCertDB(),
                                        cert, PR_FALSE,
                                        (SECCertificateUsage)0,
                                        timeboundary , NULL, NULL, NULL);
        if (status == SECSuccess) {
            break;
        }
    }

    if (status == SECSuccess) {
        return (cert);
    }

    switch(PORT_GetError()) {
        case SEC_ERROR_EXPIRED_ISSUER_CERTIFICATE:
        case SEC_ERROR_CA_CERT_INVALID:
        case SEC_ERROR_UNKNOWN_SIGNER:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_ISSUER_FAILED,
                        "cert with subject name %s could not be verified because the issuer's cert is expired/invalid or not found",
                        (cert != NULL) ? cert->subjectName : "(NULL)"
            );
            break;
        case SEC_ERROR_EXPIRED_CERTIFICATE:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_HAS_EXPIRED,
                        "cert with subject name %s has expired",
                        (cert != NULL) ? cert->subjectName : "(NULL)"
            );
            break;
        case SEC_ERROR_REVOKED_CERTIFICATE:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_REVOKED,
                        "cert with subject name %s has been revoked",
                        (cert != NULL) ? cert->subjectName : "(NULL)"
            );
            break;
        default:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_VERIFY_FAILED,
                        "cert with subject name %s could not be verified, errcode %d",
                        (cert != NULL) ? cert->subjectName : "(NULL)",
                        PORT_GetError());
            break;
    }

    return (NULL);
}

/**
 * xmlSecNssX509StoreAdoptCert:
 * @store:              the pointer to X509 key data store klass.
 * @cert:               the pointer to NSS X509 certificate.
 * @type:               the certificate type (trusted/untrusted).
 *
 * Adds trusted (root) or untrusted certificate to the store.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecNssX509StoreAdoptCert(xmlSecKeyDataStorePtr store, CERTCertificate* cert, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecNssX509StoreCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecNssX509StoreId), -1);
    xmlSecAssert2(cert != NULL, -1);

    ctx = xmlSecNssX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);

    if(ctx->certsList == NULL) {
        ctx->certsList = CERT_NewCertList();
        if(ctx->certsList == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        "CERT_NewCertList",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                                "error code=%d", PORT_GetError());
            return(-1);
        }
    }

    ret = CERT_AddCertToListTail(ctx->certsList, cert);
    if(ret != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "CERT_AddCertToListTail",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                        "error code=%d", PORT_GetError());
        return(-1);
    }

    return(0);
}

static int
xmlSecNssX509StoreInitialize(xmlSecKeyDataStorePtr store) {
    xmlSecNssX509StoreCtxPtr ctx;
    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecNssX509StoreId), -1);

    ctx = xmlSecNssX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecNssX509StoreCtx));

    return(0);
}

static void
xmlSecNssX509StoreFinalize(xmlSecKeyDataStorePtr store) {
    xmlSecNssX509StoreCtxPtr ctx;
    xmlSecAssert(xmlSecKeyDataStoreCheckId(store, xmlSecNssX509StoreId));

    ctx = xmlSecNssX509StoreGetCtx(store);
    xmlSecAssert(ctx != NULL);

    if (ctx->certsList) {
        CERT_DestroyCertList(ctx->certsList);
        ctx->certsList = NULL;
    }

    memset(ctx, 0, sizeof(xmlSecNssX509StoreCtx));
}


/*****************************************************************************
 *
 * Low-level x509 functions
 *
 *****************************************************************************/
static CERTName *
xmlSecNssGetCertName(const xmlChar * name) {
    xmlChar *tmp, *name2;
    xmlChar *p;
    CERTName *res;

    xmlSecAssert2(name != NULL, NULL);

    /* nss doesn't support emailAddress (see https://bugzilla.mozilla.org/show_bug.cgi?id=561689)
     * This code is not bullet proof and may produce incorrect results if someone has
     * "emailAddress=" string in one of the fields, but it is best I can suggest to fix 
     * this problem.
     */
    name2 = xmlStrdup(name);
    if(name2 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "xmlStrlen(name)=%d",
                    xmlStrlen(name));
        return(NULL);
    }
    while( (p = (xmlChar*)xmlStrstr(name2, BAD_CAST "emailAddress=")) != NULL) {
        memcpy(p, "           E=", 13);
    }

    tmp = xmlSecNssX509NameRead(name2, xmlStrlen(name2));
    if(tmp == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssX509NameRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "name2=\"%s\"",
                    xmlSecErrorsSafeString(name2));
        xmlFree(name2);
        return(NULL);
    }

    res = CERT_AsciiToName((char*)tmp);
    if (name == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CERT_AsciiToName",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "ascii=\"%s\", error code=%d",
                    xmlSecErrorsSafeString((char*)tmp),
                    PORT_GetError());
        PORT_Free(tmp);
        xmlFree(name2);
        return(NULL);
    }

    PORT_Free(tmp);
    return(res);
}

static CERTCertificate*
xmlSecNssX509FindCert(CERTCertList* certsList, const xmlChar *subjectName,
                      const xmlChar *issuerName, const xmlChar *issuerSerial,
                      xmlChar *ski) {
    CERTCertificate *cert = NULL;
    CERTName *name = NULL;
    SECItem *nameitem = NULL;
    CERTCertListNode* head;
    SECItem tmpitem;
    SECStatus status;
    PRArenaPool *arena = NULL;
    int rv;

    if ((cert == NULL) && (subjectName != NULL)) {
        name = xmlSecNssGetCertName(subjectName);
        if (name == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssGetCertName",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "subject=%s",
                        xmlSecErrorsSafeString(subjectName));
            goto done;
        }

        if(arena == NULL) {
            arena = PORT_NewArena(DER_DEFAULT_CHUNKSIZE);
            if (arena == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "PORT_NewArena",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        }

        nameitem = SEC_ASN1EncodeItem(arena, NULL, (void *)name,
                                      SEC_ASN1_GET(CERT_NameTemplate));
        if (nameitem == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "SEC_ASN1EncodeItem",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "error code=%d", PORT_GetError());
            goto done;
        }

        cert = CERT_FindCertByName(CERT_GetDefaultCertDB(), nameitem);
    }

    if((cert == NULL) && (issuerName != NULL) && (issuerSerial != NULL)) {
        CERTIssuerAndSN issuerAndSN;
        PRUint64 issuerSN = 0;

        name = xmlSecNssGetCertName(issuerName);
        if (name == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssGetCertName",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "issuer=%s",
                        xmlSecErrorsSafeString(issuerName));
            goto done;
        }

        if(arena == NULL) {
            arena = PORT_NewArena(DER_DEFAULT_CHUNKSIZE);
            if (arena == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            "PORT_NewArena",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        }

        nameitem = SEC_ASN1EncodeItem(arena, NULL, (void *)name,
                                      SEC_ASN1_GET(CERT_NameTemplate));
        if (nameitem == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "SEC_ASN1EncodeItem",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "error code=%d", PORT_GetError());
            goto done;
        }

        memset(&issuerAndSN, 0, sizeof(issuerAndSN));

        issuerAndSN.derIssuer.data = nameitem->data;
        issuerAndSN.derIssuer.len = nameitem->len;

        /* TBD: serial num can be arbitrarily long */
        if(PR_sscanf((char *)issuerSerial, "%llu", &issuerSN) != 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "PR_sscanf",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "error code=%d", PR_GetError());
            SECITEM_FreeItem(&issuerAndSN.serialNumber, PR_FALSE);
            goto done;
        }

        rv = xmlSecNssNumToItem(&issuerAndSN.serialNumber, issuerSN);
        if(rv <= 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssNumToItem",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "error code=%d", PR_GetError());
            SECITEM_FreeItem(&issuerAndSN.serialNumber, PR_FALSE);
            goto done;
        }

        cert = CERT_FindCertByIssuerAndSN(CERT_GetDefaultCertDB(), &issuerAndSN);
        SECITEM_FreeItem(&issuerAndSN.serialNumber, PR_FALSE);
    }

    if((cert == NULL) && (ski != NULL)) {
        SECItem subjKeyID;
        int len;

        len = xmlSecBase64Decode(ski, (xmlSecByte*)ski, xmlStrlen(ski));
        if(len < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBase64Decode",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "ski=%s",
                        xmlSecErrorsSafeString(ski));
            goto done;
        }

        memset(&subjKeyID, 0, sizeof(subjKeyID));
        subjKeyID.data = ski;
        subjKeyID.len = len;
        cert = CERT_FindCertBySubjectKeyID(CERT_GetDefaultCertDB(),
                                           &subjKeyID);

        /* try to search in our list - NSS doesn't update it's cache correctly
         * when new certs are added https://bugzilla.mozilla.org/show_bug.cgi?id=211051
         */
        if((cert == NULL) && (certsList != NULL)) {

            for(head = CERT_LIST_HEAD(certsList);
                (cert == NULL) && !CERT_LIST_END(head, certsList) &&
                (head != NULL) && (head->cert != NULL);
                head = CERT_LIST_NEXT(head)
            ) {

                memset(&tmpitem, 0, sizeof(tmpitem));
                status = CERT_FindSubjectKeyIDExtension(head->cert, &tmpitem);
                if (status != SECSuccess)  {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "CERT_FindSubjectKeyIDExtension",
                                XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                "ski");
                    SECITEM_FreeItem(&tmpitem, PR_FALSE);
                    goto done;
                }

                if((tmpitem.len == subjKeyID.len) &&
                   (memcmp(tmpitem.data, subjKeyID.data, subjKeyID.len) == 0)
                ) {
                    cert = CERT_DupCertificate(head->cert);
                    if(cert == NULL) {
                        xmlSecError(XMLSEC_ERRORS_HERE,
                                    NULL,
                                    "CERT_DupCertificate",
                                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                    "error code=%d", PORT_GetError());
                        SECITEM_FreeItem(&tmpitem, PR_FALSE);
                        goto done;
                    }
                }
                SECITEM_FreeItem(&tmpitem, PR_FALSE);
            }
        }
    }

done:
    if (arena != NULL) {
        PORT_FreeArena(arena, PR_FALSE);
    }
    if (name != NULL) {
        CERT_DestroyName(name);
    }

    return(cert);
}

static xmlSecByte *
xmlSecNssX509NameRead(xmlSecByte *str, int len) {
    xmlSecByte name[256];
    xmlSecByte value[256];
    xmlSecByte *retval = NULL;
    xmlSecByte *p = NULL;
    int nameLen, valueLen;

    xmlSecAssert2(str != NULL, NULL);

    /* return string should be no longer than input string */
    retval = (xmlSecByte *)PORT_Alloc(len+1);
    if(retval == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "PORT_Alloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }
    p = retval;

    while(len > 0) {
        /* skip spaces after comma or semicolon */
        while((len > 0) && isspace(*str)) {
            ++str; --len;
        }

        nameLen = xmlSecNssX509NameStringRead(&str, &len, name, sizeof(name), '=', 0);
        if(nameLen < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecNssX509NameStringRead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }
        memcpy(p, name, nameLen);
        p+=nameLen;
        *p++='=';
        if(len > 0) {
            ++str; --len;
            if((*str) == '\"') {
                valueLen = xmlSecNssX509NameStringRead(&str, &len,
                                        value, sizeof(value), '"', 1);
                if(valueLen < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecNssX509NameStringRead",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }
                /* skip spaces before comma or semicolon */
                while((len > 0) && isspace(*str)) {
                    ++str; --len;
                }
                if((len > 0) && ((*str) != ',')) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                NULL,
                                XMLSEC_ERRORS_R_INVALID_DATA,
                                "comma is expected");
                    goto done;
                }
                if(len > 0) {
                    ++str; --len;
                }
                *p++='\"';
                memcpy(p, value, valueLen);
                p+=valueLen;
                *p++='\"';
            } else if((*str) == '#') {
                /* TODO: read octect values */
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            NULL,
                            XMLSEC_ERRORS_R_INVALID_DATA,
                            "reading octect values is not implemented yet");
                goto done;
            } else {
                valueLen = xmlSecNssX509NameStringRead(&str, &len,
                                        value, sizeof(value), ',', 1);
                if(valueLen < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecNssX509NameStringRead",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }
                memcpy(p, value, valueLen);
                p+=valueLen;
                if (len > 0) {
                    *p++=',';
                }
            }
        }
        if(len > 0) {
            ++str; --len;
        }
    }

    *p = 0;
    return(retval);

done:
    PORT_Free(retval);
    return (NULL);
}

static int
xmlSecNssX509NameStringRead(xmlSecByte **str, int *strLen,
                            xmlSecByte *res, int resLen,
                            xmlSecByte delim, int ingoreTrailingSpaces) {
    xmlSecByte *p, *q, *nonSpace;

    xmlSecAssert2(str != NULL, -1);
    xmlSecAssert2(strLen != NULL, -1);
    xmlSecAssert2(res != NULL, -1);

    p = (*str);
    nonSpace = q = res;
    while(((p - (*str)) < (*strLen)) && ((*p) != delim) && ((q - res) < resLen)) {
        if((*p) != '\\') {
            if(ingoreTrailingSpaces && !isspace(*p)) {
                nonSpace = q;
            }
            *(q++) = *(p++);
        } else {
            ++p;
            nonSpace = q;
            if(xmlSecIsHex((*p))) {
                if((p - (*str) + 1) >= (*strLen)) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                NULL,
                                XMLSEC_ERRORS_R_INVALID_DATA,
                                "two hex digits expected");
                    return(-1);
                }
                *(q++) = xmlSecGetHex(p[0]) * 16 + xmlSecGetHex(p[1]);
                p += 2;
            } else {
                if(((++p) - (*str)) >= (*strLen)) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                NULL,
                                XMLSEC_ERRORS_R_INVALID_DATA,
                                "escaped symbol missed");
                    return(-1);
                }
                *(q++) = *(p++);
            }
        }
    }
    if(((p - (*str)) < (*strLen)) && ((*p) != delim)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_SIZE,
                    "buffer is too small");
        return(-1);
    }
    (*strLen) -= (p - (*str));
    (*str) = p;
    return((ingoreTrailingSpaces) ? nonSpace - res + 1 : q - res);
}

/* code lifted from NSS */
static int
xmlSecNssNumToItem(SECItem *it, PRUint64 ui)
{
    unsigned char bb[9];
    unsigned int zeros_len;

    xmlSecAssert2(it != NULL, -1);

    bb[0] = 0; /* important: we should have 0 at the beginning! */
    bb[1] = (unsigned char) (ui >> 56);
    bb[2] = (unsigned char) (ui >> 48);
    bb[3] = (unsigned char) (ui >> 40);
    bb[4] = (unsigned char) (ui >> 32);
    bb[5] = (unsigned char) (ui >> 24);
    bb[6] = (unsigned char) (ui >> 16);
    bb[7] = (unsigned char) (ui >> 8);
    bb[8] = (unsigned char) (ui);

    /*
    ** Small integers are encoded in a single byte. Larger integers
    ** require progressively more space. Start from 1 because byte at 
    ** position 0 is zero
    */
    for(zeros_len = 1; (zeros_len < sizeof(bb)) && (bb[zeros_len] == 0); ++zeros_len);

    it->len = sizeof(bb) - (zeros_len - 1);
    it->data = (unsigned char *)PORT_Alloc(it->len);
    if (it->data == NULL) {
        it->len = 0;
        return (-1);
    }

    PORT_Memcpy(it->data, bb + (zeros_len - 1), it->len);
    return(it->len);
}
#endif /* XMLSEC_NO_X509 */



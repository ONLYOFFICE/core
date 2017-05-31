/**
 * XMLSec library
 *
 * X509 support
 *
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_X509

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <libxml/tree.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/x509v3.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/base64.h>
#include <xmlsec/errors.h>

#include <xmlsec/openssl/crypto.h>
#include <xmlsec/openssl/evp.h>
#include <xmlsec/openssl/x509.h>
#include "openssl11_wrapper.h"

/**************************************************************************
 *
 * Internal OpenSSL X509 store CTX
 *
 *************************************************************************/
typedef struct _xmlSecOpenSSLX509StoreCtx               xmlSecOpenSSLX509StoreCtx,
                                                        *xmlSecOpenSSLX509StoreCtxPtr;
struct _xmlSecOpenSSLX509StoreCtx {
    X509_STORE*         xst;
    STACK_OF(X509)*     untrusted;
    STACK_OF(X509_CRL)* crls;
    X509_VERIFY_PARAM * vpm;
};

/****************************************************************************
 *
 * xmlSecOpenSSLKeyDataStoreX509Id:
 *
 * xmlSecOpenSSLX509StoreCtx is located after xmlSecTransform
 *
 ***************************************************************************/
#define xmlSecOpenSSLX509StoreGetCtx(store) \
    ((xmlSecOpenSSLX509StoreCtxPtr)(((xmlSecByte*)(store)) + \
                                    sizeof(xmlSecKeyDataStoreKlass)))
#define xmlSecOpenSSLX509StoreSize      \
    (sizeof(xmlSecKeyDataStoreKlass) + sizeof(xmlSecOpenSSLX509StoreCtx))

static int              xmlSecOpenSSLX509StoreInitialize        (xmlSecKeyDataStorePtr store);
static void             xmlSecOpenSSLX509StoreFinalize          (xmlSecKeyDataStorePtr store);

static xmlSecKeyDataStoreKlass xmlSecOpenSSLX509StoreKlass = {
    sizeof(xmlSecKeyDataStoreKlass),
    xmlSecOpenSSLX509StoreSize,

    /* data */
    xmlSecNameX509Store,                        /* const xmlChar* name; */

    /* constructors/destructor */
    xmlSecOpenSSLX509StoreInitialize,           /* xmlSecKeyDataStoreInitializeMethod initialize; */
    xmlSecOpenSSLX509StoreFinalize,             /* xmlSecKeyDataStoreFinalizeMethod finalize; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

static int              xmlSecOpenSSLX509VerifyCRL                      (X509_STORE* xst,
                                                                         X509_CRL *crl );
static X509*            xmlSecOpenSSLX509FindCert                       (STACK_OF(X509) *certs,
                                                                         xmlChar *subjectName,
                                                                         xmlChar *issuerName,
                                                                         xmlChar *issuerSerial,
                                                                         xmlChar *ski);
static X509*            xmlSecOpenSSLX509FindNextChainCert              (STACK_OF(X509) *chain,
                                                                         X509 *cert);
static int              xmlSecOpenSSLX509VerifyCertAgainstCrls          (STACK_OF(X509_CRL) *crls,
                                                                         X509* cert);
static X509_NAME*       xmlSecOpenSSLX509NameRead                       (xmlSecByte *str,
                                                                         int len);
static int              xmlSecOpenSSLX509NameStringRead                 (xmlSecByte **str,
                                                                         int *strLen,
                                                                         xmlSecByte *res,
                                                                         int resLen,
                                                                         xmlSecByte delim,
                                                                         int ingoreTrailingSpaces);
static int              xmlSecOpenSSLX509NamesCompare                   (X509_NAME *a,
                                                                         X509_NAME *b);
static STACK_OF(X509_NAME_ENTRY)*  xmlSecOpenSSLX509_NAME_ENTRIES_copy  (X509_NAME *a);
static int              xmlSecOpenSSLX509_NAME_ENTRIES_cmp              (STACK_OF(X509_NAME_ENTRY) * a,
                                                                         STACK_OF(X509_NAME_ENTRY) * b);
static int              xmlSecOpenSSLX509_NAME_ENTRY_cmp                (const X509_NAME_ENTRY * const *a,
                                                                         const X509_NAME_ENTRY * const *b);


/**
 * xmlSecOpenSSLX509StoreGetKlass:
 *
 * The OpenSSL X509 certificates key data store klass.
 *
 * Returns: pointer to OpenSSL X509 certificates key data store klass.
 */
xmlSecKeyDataStoreId
xmlSecOpenSSLX509StoreGetKlass(void) {
    return(&xmlSecOpenSSLX509StoreKlass);
}

/**
 * xmlSecOpenSSLX509StoreFindCert:
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
X509*
xmlSecOpenSSLX509StoreFindCert(xmlSecKeyDataStorePtr store, xmlChar *subjectName,
                                xmlChar *issuerName, xmlChar *issuerSerial,
                                xmlChar *ski, xmlSecKeyInfoCtx* keyInfoCtx) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    X509* res = NULL;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, NULL);

    if((res == NULL) && (ctx->untrusted != NULL)) {
        res = xmlSecOpenSSLX509FindCert(ctx->untrusted, subjectName, issuerName, issuerSerial, ski);
    }
    return(res);
}

/**
 * xmlSecOpenSSLX509StoreVerify:
 * @store:              the pointer to X509 key data store klass.
 * @certs:              the untrusted certificates stack.
 * @crls:               the crls stack.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> element processing context.
 *
 * Verifies @certs list.
 *
 * Returns: pointer to the first verified certificate from @certs.
 */
X509*
xmlSecOpenSSLX509StoreVerify(xmlSecKeyDataStorePtr store, XMLSEC_STACK_OF_X509* certs,
                             XMLSEC_STACK_OF_X509_CRL* crls, xmlSecKeyInfoCtx* keyInfoCtx) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    STACK_OF(X509)* certs2 = NULL;
    STACK_OF(X509_CRL)* crls2 = NULL;
    X509 * res = NULL;
    X509 * cert;
    X509 * err_cert = NULL;
    X509_STORE_CTX *xsc;
    char buf[256];
    int err = 0;
    int i;
    int ret;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), NULL);
    xmlSecAssert2(certs != NULL, NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);

    xsc = X509_STORE_CTX_new();
    if(xsc == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_STORE_CTX_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, NULL);
    xmlSecAssert2(ctx->xst != NULL, NULL);

    /* dup certs */
    certs2 = sk_X509_dup(certs);
    if(certs2 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "sk_X509_dup",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* add untrusted certs from the store */
    if(ctx->untrusted != NULL) {
        for(i = 0; i < sk_X509_num(ctx->untrusted); ++i) {
            ret = sk_X509_push(certs2, sk_X509_value(ctx->untrusted, i));
            if(ret < 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "sk_X509_push",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        }
    }

    /* dup crls but remove all non-verified */
    if(crls != NULL) {
        crls2 = sk_X509_CRL_dup(crls);
        if(crls2 == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        "sk_X509_CRL_dup",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }

        for(i = 0; i < sk_X509_CRL_num(crls2); ) {
            ret = xmlSecOpenSSLX509VerifyCRL(ctx->xst, sk_X509_CRL_value(crls2, i));
            if(ret == 1) {
                ++i;
            } else if(ret == 0) {
                (void)sk_X509_CRL_delete(crls2, i);
            } else {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "xmlSecOpenSSLX509VerifyCRL",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        }
    }

    /* remove all revoked certs */
    for(i = 0; i < sk_X509_num(certs2);) {
        cert = sk_X509_value(certs2, i);

        if(crls2 != NULL) {
            ret = xmlSecOpenSSLX509VerifyCertAgainstCrls(crls2, cert);
            if(ret == 0) {
                (void)sk_X509_delete(certs2, i);
                continue;
            } else if(ret != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "xmlSecOpenSSLX509VerifyCertAgainstCrls",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        }

        if(ctx->crls != NULL) {
            ret = xmlSecOpenSSLX509VerifyCertAgainstCrls(ctx->crls, cert);
            if(ret == 0) {
                (void)sk_X509_delete(certs2, i);
                continue;
            } else if(ret != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "xmlSecOpenSSLX509VerifyCertAgainstCrls",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }
        }
        ++i;
    }

    /* get one cert after another and try to verify */
    for(i = 0; i < sk_X509_num(certs2); ++i) {
        cert = sk_X509_value(certs2, i);
        if(xmlSecOpenSSLX509FindNextChainCert(certs2, cert) == NULL) {

            ret = X509_STORE_CTX_init(xsc, ctx->xst, cert, certs2);
            if(ret != 1) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "X509_STORE_CTX_init",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                goto done;
            }

            if(keyInfoCtx->certsVerificationTime > 0) {
                X509_STORE_CTX_set_time(xsc, 0, keyInfoCtx->certsVerificationTime);
            }

            {
                X509_VERIFY_PARAM * vpm = NULL;
                unsigned long vpm_flags = 0;

                vpm = X509_VERIFY_PARAM_new();
                if(vpm == NULL) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                                "X509_VERIFY_PARAM_new",
                                XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    goto done;
                }
                vpm_flags = X509_VERIFY_PARAM_get_flags(vpm);
                vpm_flags &= (~X509_V_FLAG_CRL_CHECK);

                if(keyInfoCtx->certsVerificationTime > 0) {
                    vpm_flags |= X509_V_FLAG_USE_CHECK_TIME;
                    X509_VERIFY_PARAM_set_time(vpm, keyInfoCtx->certsVerificationTime);
                }

                X509_VERIFY_PARAM_set_depth(vpm, keyInfoCtx->certsVerificationDepth);
                X509_VERIFY_PARAM_set_flags(vpm, vpm_flags);
                X509_STORE_CTX_set0_param(xsc, vpm);
            }


            ret         = X509_verify_cert(xsc);
            err_cert    = X509_STORE_CTX_get_current_cert(xsc);
            err         = X509_STORE_CTX_get_error(xsc);

            X509_STORE_CTX_cleanup (xsc);

            if(ret == 1) {
                res = cert;
                goto done;
            } else if(ret < 0) {
                const char* err_msg;

                buf[0] = '\0';
                X509_NAME_oneline(X509_get_subject_name(err_cert), buf, sizeof buf);
                err_msg = X509_verify_cert_error_string(err);
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "X509_verify_cert",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "subj=%s;err=%d;msg=%s",
                            xmlSecErrorsSafeString(buf),
                            err,
                            xmlSecErrorsSafeString(err_msg));
                goto done;
            } else if(ret == 0) {
                const char* err_msg;

                buf[0] = '\0';
                X509_NAME_oneline(X509_get_subject_name(err_cert), buf, sizeof buf);
                err_msg = X509_verify_cert_error_string(err);
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                            "X509_verify_cert",
                            XMLSEC_ERRORS_R_CRYPTO_FAILED,
                            "subj=%s;err=%d;msg=%s",
                            xmlSecErrorsSafeString(buf),
                            err,
                            xmlSecErrorsSafeString(err_msg));
            }
        }
    }

    /* if we came here then we found nothing. do we have any error? */
    if((err != 0) && (err_cert != NULL)) {
        const char* err_msg;

        err_msg = X509_verify_cert_error_string(err);
        switch (err) {
        case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
            X509_NAME_oneline(X509_get_issuer_name(err_cert), buf, sizeof buf);
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_ISSUER_FAILED,
                        "err=%d;msg=%s;issuer=%s",
                        err,
                        xmlSecErrorsSafeString(err_msg),
                        xmlSecErrorsSafeString(buf));
            break;
        case X509_V_ERR_CERT_NOT_YET_VALID:
        case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_NOT_YET_VALID,
                        "err=%d;msg=%s", err,
                        xmlSecErrorsSafeString(err_msg));
            break;
        case X509_V_ERR_CERT_HAS_EXPIRED:
        case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_HAS_EXPIRED,
                        "err=%d;msg=%s", err,
                        xmlSecErrorsSafeString(err_msg));
            break;
        default:
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        NULL,
                        XMLSEC_ERRORS_R_CERT_VERIFY_FAILED,
                        "err=%d;msg=%s", err,
                        xmlSecErrorsSafeString(err_msg));
        }
    }

done:
    if(certs2 != NULL) {
        sk_X509_free(certs2);
    }
    if(crls2 != NULL) {
        sk_X509_CRL_free(crls2);
    }
    if(xsc != NULL) {
        X509_STORE_CTX_free(xsc);
    }
    return(res);
}

/**
 * xmlSecOpenSSLX509StoreAdoptCert:
 * @store:              the pointer to X509 key data store klass.
 * @cert:               the pointer to OpenSSL X509 certificate.
 * @type:               the certificate type (trusted/untrusted).
 *
 * Adds trusted (root) or untrusted certificate to the store.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecOpenSSLX509StoreAdoptCert(xmlSecKeyDataStorePtr store, X509* cert, xmlSecKeyDataType type) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), -1);
    xmlSecAssert2(cert != NULL, -1);

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);

    if((type & xmlSecKeyDataTypeTrusted) != 0) {
        xmlSecAssert2(ctx->xst != NULL, -1);

        ret = X509_STORE_add_cert(ctx->xst, cert);
        if(ret != 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        "X509_STORE_add_cert",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
        /* add cert increments the reference */
        X509_free(cert);
    } else {
        xmlSecAssert2(ctx->untrusted != NULL, -1);

        ret = sk_X509_push(ctx->untrusted, cert);
        if(ret < 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        "sk_X509_push",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }
    return(0);
}

/**
 * xmlSecOpenSSLX509StoreAdoptCrl:
 * @store:              the pointer to X509 key data store klass.
 * @crl:                the pointer to OpenSSL X509_CRL.
 *
 * Adds X509 CRL to the store.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecOpenSSLX509StoreAdoptCrl(xmlSecKeyDataStorePtr store, X509_CRL* crl) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    int ret;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), -1);
    xmlSecAssert2(crl != NULL, -1);

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);
        xmlSecAssert2(ctx->crls != NULL, -1);

        ret = sk_X509_CRL_push(ctx->crls, crl);
        if(ret < 1) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                        "sk_X509_CRL_push",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }

    return (0);
}

/**
 * xmlSecOpenSSLX509StoreAddCertsPath:
 * @store: the pointer to OpenSSL x509 store.
 * @path: the path to the certs dir.
 *
 * Adds all certs in the @path to the list of trusted certs
 * in @store.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecOpenSSLX509StoreAddCertsPath(xmlSecKeyDataStorePtr store, const char *path) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    X509_LOOKUP *lookup = NULL;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), -1);
    xmlSecAssert2(path != NULL, -1);

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->xst != NULL, -1);

    lookup = X509_STORE_add_lookup(ctx->xst, X509_LOOKUP_hash_dir());
    if(lookup == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_STORE_add_lookup",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if(!X509_LOOKUP_add_dir(lookup, path, X509_FILETYPE_PEM)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_LOOKUP_add_dir",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "path='%s'",
                    xmlSecErrorsSafeString(path)
        );
        return(-1);
    }
    return(0);
}

/**
 * xmlSecOpenSSLX509StoreAddCertsFile:
 * @store: the pointer to OpenSSL x509 store.
 * @file: the certs file.
 *
 * Adds all certs in @file to the list of trusted certs
 * in @store. It is possible for @file to contain multiple certs.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecOpenSSLX509StoreAddCertsFile(xmlSecKeyDataStorePtr store, const char *file) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    X509_LOOKUP *lookup = NULL;

    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), -1);
    xmlSecAssert2(file != NULL, -1);

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->xst != NULL, -1);

    lookup = X509_STORE_add_lookup(ctx->xst, X509_LOOKUP_file());
    if(lookup == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_STORE_add_lookup",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if(!X509_LOOKUP_load_file(lookup, file, X509_FILETYPE_PEM)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_LOOKUP_load_file",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "file='%s'",
                    xmlSecErrorsSafeString(file)
        );
        return(-1);
    }
    return(0);
}

static int
xmlSecOpenSSLX509StoreInitialize(xmlSecKeyDataStorePtr store) {
    const xmlChar* path;
    X509_LOOKUP *lookup = NULL;

    xmlSecOpenSSLX509StoreCtxPtr ctx;
    xmlSecAssert2(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId), -1);

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecOpenSSLX509StoreCtx));

    ctx->xst = X509_STORE_new();
    if(ctx->xst == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_STORE_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if(!X509_STORE_set_default_paths(ctx->xst)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_STORE_set_default_paths",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }


    lookup = X509_STORE_add_lookup(ctx->xst, X509_LOOKUP_hash_dir());
    if(lookup == NULL) {
         xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_STORE_add_lookup",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
         return(-1);
    }

    path = xmlSecOpenSSLGetDefaultTrustedCertsFolder();
    if(path != NULL) {
        if(!X509_LOOKUP_add_dir(lookup, (char*)path, X509_FILETYPE_PEM)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_LOOKUP_add_dir",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "path='%s'",
                    xmlSecErrorsSafeString(path)
            );
            return(-1);
        }
    } else {
        if(!X509_LOOKUP_add_dir(lookup, NULL, X509_FILETYPE_DEFAULT)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_LOOKUP_add_dir",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE
            );
            return(-1);
        }
    }

    ctx->untrusted = sk_X509_new_null();
    if(ctx->untrusted == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "sk_X509_new_null",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ctx->crls = sk_X509_CRL_new_null();
    if(ctx->crls == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "sk_X509_CRL_new_null",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ctx->vpm = X509_VERIFY_PARAM_new();
    if(ctx->vpm == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataStoreGetName(store)),
                    "X509_VERIFY_PARAM_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    X509_VERIFY_PARAM_set_depth(ctx->vpm, 9); /* the default cert verification path in openssl */
    X509_STORE_set1_param(ctx->xst, ctx->vpm);


    return(0);
}

static void
xmlSecOpenSSLX509StoreFinalize(xmlSecKeyDataStorePtr store) {
    xmlSecOpenSSLX509StoreCtxPtr ctx;
    xmlSecAssert(xmlSecKeyDataStoreCheckId(store, xmlSecOpenSSLX509StoreId));

    ctx = xmlSecOpenSSLX509StoreGetCtx(store);
    xmlSecAssert(ctx != NULL);


    if(ctx->xst != NULL) {
        X509_STORE_free(ctx->xst);
    }
    if(ctx->untrusted != NULL) {
        sk_X509_pop_free(ctx->untrusted, X509_free);
    }
    if(ctx->crls != NULL) {
        sk_X509_CRL_pop_free(ctx->crls, X509_CRL_free);
    }
    if(ctx->vpm != NULL) {
        X509_VERIFY_PARAM_free(ctx->vpm);
    }

    memset(ctx, 0, sizeof(xmlSecOpenSSLX509StoreCtx));
}


/*****************************************************************************
 *
 * Low-level x509 functions
 *
 *****************************************************************************/
static int
xmlSecOpenSSLX509VerifyCRL(X509_STORE* xst, X509_CRL *crl ) {
    X509_STORE_CTX *xsc = NULL;
    X509_OBJECT *xobj = NULL;
    EVP_PKEY *pkey = NULL;
    int ret;

    xmlSecAssert2(xst != NULL, -1);
    xmlSecAssert2(crl != NULL, -1);

    xsc = X509_STORE_CTX_new();
    if(xsc == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_STORE_CTX_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto err;
    }
    xobj = X509_OBJECT_new();
    if(xobj == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_OBJECT_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto err;
    }

    ret = X509_STORE_CTX_init(xsc, xst, NULL, NULL);
    if(ret != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_STORE_CTX_init",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto err;
    }
    ret = X509_STORE_get_by_subject(xsc, X509_LU_X509,
                                    X509_CRL_get_issuer(crl), xobj);
    if(ret <= 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_STORE_get_by_subject",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto err;
    }
    pkey = X509_get_pubkey(X509_OBJECT_get0_X509(xobj));
    if(pkey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_get_pubkey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto err;
    }
    ret = X509_CRL_verify(crl, pkey);
    EVP_PKEY_free(pkey);
    if(ret != 1) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_CRL_verify",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
    }
    X509_STORE_CTX_free(xsc);
    X509_OBJECT_free(xobj);
    return((ret == 1) ? 1 : 0);

err:
    X509_STORE_CTX_free(xsc);
    X509_OBJECT_free(xobj);
    return(-1);
}

static X509*
xmlSecOpenSSLX509FindCert(STACK_OF(X509) *certs, xmlChar *subjectName,
                        xmlChar *issuerName, xmlChar *issuerSerial,
                        xmlChar *ski) {
    X509 *cert = NULL;
    int i;

    xmlSecAssert2(certs != NULL, NULL);

    /* todo: may be this is not the fastest way to search certs */
    if(subjectName != NULL) {
        X509_NAME *nm;
        X509_NAME *subj;

        nm = xmlSecOpenSSLX509NameRead(subjectName, xmlStrlen(subjectName));
        if(nm == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecOpenSSLX509NameRead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "subject=%s",
                        xmlSecErrorsSafeString(subjectName));
            return(NULL);
        }

        for(i = 0; i < sk_X509_num(certs); ++i) {
            cert = sk_X509_value(certs, i);
            subj = X509_get_subject_name(cert);
            if(xmlSecOpenSSLX509NamesCompare(nm, subj) == 0) {
                X509_NAME_free(nm);
                return(cert);
            }
        }
        X509_NAME_free(nm);
    } else if((issuerName != NULL) && (issuerSerial != NULL)) {
        X509_NAME *nm;
        X509_NAME *issuer;
        BIGNUM *bn;
        ASN1_INTEGER *serial;

        nm = xmlSecOpenSSLX509NameRead(issuerName, xmlStrlen(issuerName));
        if(nm == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecOpenSSLX509NameRead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "issuer=%s",
                        xmlSecErrorsSafeString(issuerName));
            return(NULL);
        }

        bn = BN_new();
        if(bn == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "BN_new",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            X509_NAME_free(nm);
            return(NULL);
        }
        if(BN_dec2bn(&bn, (char*)issuerSerial) == 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "BN_dec2bn",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            BN_free(bn);
            X509_NAME_free(nm);
            return(NULL);
        }

        serial = BN_to_ASN1_INTEGER(bn, NULL);
        if(serial == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "BN_to_ASN1_INTEGER",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            BN_free(bn);
            X509_NAME_free(nm);
            return(NULL);
        }
        BN_free(bn);


        for(i = 0; i < sk_X509_num(certs); ++i) {
            cert = sk_X509_value(certs, i);
            if(ASN1_INTEGER_cmp(X509_get_serialNumber(cert), serial) != 0) {
                continue;
            }
            issuer = X509_get_issuer_name(cert);
            if(xmlSecOpenSSLX509NamesCompare(nm, issuer) == 0) {
                ASN1_INTEGER_free(serial);
                X509_NAME_free(nm);
                return(cert);
            }
        }

        X509_NAME_free(nm);
        ASN1_INTEGER_free(serial);
    } else if(ski != NULL) {
        int len;
        int index;
        X509_EXTENSION *ext;
        ASN1_OCTET_STRING *keyId;

        /* our usual trick with base64 decode */
        len = xmlSecBase64Decode(ski, (xmlSecByte*)ski, xmlStrlen(ski));
        if(len < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecBase64Decode",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "ski=%s",
                        xmlSecErrorsSafeString(ski));
            return(NULL);
        }
        for(i = 0; i < sk_X509_num(certs); ++i) {
            cert = sk_X509_value(certs, i);
            index = X509_get_ext_by_NID(cert, NID_subject_key_identifier, -1);
            if((index >= 0)  && (ext = X509_get_ext(cert, index))) {
                keyId = X509V3_EXT_d2i(ext);
                if((keyId != NULL) && (keyId->length == len) &&
                                    (memcmp(keyId->data, ski, len) == 0)) {
                    ASN1_OCTET_STRING_free(keyId);
                    return(cert);
                }
                ASN1_OCTET_STRING_free(keyId);
            }
        }
    }

    return(NULL);
}

static X509*
xmlSecOpenSSLX509FindNextChainCert(STACK_OF(X509) *chain, X509 *cert) {
    unsigned long certSubjHash;
    int i;

    xmlSecAssert2(chain != NULL, NULL);
    xmlSecAssert2(cert != NULL, NULL);

    certSubjHash = X509_subject_name_hash(cert);
    for(i = 0; i < sk_X509_num(chain); ++i) {
        if((sk_X509_value(chain, i) != cert) &&
           (X509_issuer_name_hash(sk_X509_value(chain, i)) == certSubjHash)) {

            return(sk_X509_value(chain, i));
        }
    }
    return(NULL);
}

static int
xmlSecOpenSSLX509VerifyCertAgainstCrls(STACK_OF(X509_CRL) *crls, X509* cert) {
    X509_NAME *issuer;
    X509_CRL *crl = NULL;
    X509_REVOKED *revoked;
    int i, n;
    int ret;

    xmlSecAssert2(crls != NULL, -1);
    xmlSecAssert2(cert != NULL, -1);

    /*
     * Try to retrieve a CRL corresponding to the issuer of
     * the current certificate
     */
    issuer = X509_get_issuer_name(cert);
    n = sk_X509_CRL_num(crls);
    for(i = 0; i < n; i++) {
        crl = sk_X509_CRL_value(crls, i);
        if(crl == NULL) {
            continue;
        }

        if(xmlSecOpenSSLX509NamesCompare(X509_CRL_get_issuer(crl), issuer) == 0) {
            break;
        }
    }
    if((i >= n) || (crl == NULL)){
        /* no crls for this issuer */
        return(1);
    }

    /*
     * Check date of CRL to make sure it's not expired
     */
    ret = X509_cmp_current_time(X509_CRL_get0_nextUpdate(crl));
    if (ret == 0) {
        /* crl expired */
        return(1);
    }

    /*
     * Check if the current certificate is revoked by this CRL
     */
    n = sk_X509_REVOKED_num(X509_CRL_get_REVOKED(crl));
    for (i = 0; i < n; i++) {
        revoked = sk_X509_REVOKED_value(X509_CRL_get_REVOKED(crl), i);
        if (ASN1_INTEGER_cmp(X509_REVOKED_get0_serialNumber(revoked), X509_get_serialNumber(cert)) == 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_CERT_REVOKED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(0);
        }
    }
    return(1);
}

static X509_NAME *
xmlSecOpenSSLX509NameRead(xmlSecByte *str, int len) {
    xmlSecByte name[256];
    xmlSecByte value[256];
    int nameLen, valueLen;
    X509_NAME *nm;
    int type = MBSTRING_ASC;

    xmlSecAssert2(str != NULL, NULL);

    nm = X509_NAME_new();
    if(nm == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "X509_NAME_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    while(len > 0) {
        /* skip spaces after comma or semicolon */
        while((len > 0) && isspace(*str)) {
            ++str; --len;
        }

        nameLen = xmlSecOpenSSLX509NameStringRead(&str, &len, name, sizeof(name), '=', 0);
        if(nameLen < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecOpenSSLX509NameStringRead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            X509_NAME_free(nm);
            return(NULL);
        }
        name[nameLen] = '\0';
        if(len > 0) {
            ++str; --len;
            if((*str) == '\"') {
                ++str; --len;
                valueLen = xmlSecOpenSSLX509NameStringRead(&str, &len,
                                        value, sizeof(value), '"', 1);
                if(valueLen < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecOpenSSLX509NameStringRead",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    X509_NAME_free(nm);
                    return(NULL);
                }

                /* skip quote */
                if((len <= 0) || ((*str) != '\"')) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                NULL,
                                XMLSEC_ERRORS_R_INVALID_DATA,
                                "quote is expected:%s",
                                xmlSecErrorsSafeString(str));
                    X509_NAME_free(nm);
                    return(NULL);
                }
                ++str; --len;

                /* skip spaces before comma or semicolon */
                while((len > 0) && isspace(*str)) {
                    ++str; --len;
                }
                if((len > 0) && ((*str) != ',')) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                NULL,
                                XMLSEC_ERRORS_R_INVALID_DATA,
                                "comma is expected:%s",
                                xmlSecErrorsSafeString(str));
                    X509_NAME_free(nm);
                    return(NULL);
                }
                if(len > 0) {
                    ++str; --len;
                }
                type = MBSTRING_ASC;
            } else if((*str) == '#') {
                /* TODO: read octect values */
                xmlSecError(XMLSEC_ERRORS_HERE,
                            NULL,
                            NULL,
                            XMLSEC_ERRORS_R_INVALID_DATA,
                            "reading octect values is not implemented yet");
                X509_NAME_free(nm);
                return(NULL);
            } else {
                valueLen = xmlSecOpenSSLX509NameStringRead(&str, &len,
                                        value, sizeof(value), ',', 1);
                if(valueLen < 0) {
                    xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecOpenSSLX509NameStringRead",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                XMLSEC_ERRORS_NO_MESSAGE);
                    X509_NAME_free(nm);
                    return(NULL);
                }
                type = MBSTRING_ASC;
            }
        } else {
            valueLen = 0;
        }
        value[valueLen] = '\0';
        if(len > 0) {
            ++str; --len;
        }
        X509_NAME_add_entry_by_txt(nm, (char*)name, type, value, valueLen, -1, 0);
    }

    return(nm);
}

static int
xmlSecOpenSSLX509NameStringRead(xmlSecByte **str, int *strLen,
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
            if(ingoreTrailingSpaces && !isspace(*p)) nonSpace = q;
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

/**
 * This function DOES NOT create duplicates for X509_NAME_ENTRY objects!
 */
static STACK_OF(X509_NAME_ENTRY)*
xmlSecOpenSSLX509_NAME_ENTRIES_copy(X509_NAME * a) {
    STACK_OF(X509_NAME_ENTRY) * res = NULL;
    int ii;

    res = sk_X509_NAME_ENTRY_new(xmlSecOpenSSLX509_NAME_ENTRY_cmp);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "sk_X509_NAME_ENTRY_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    for (ii = X509_NAME_entry_count(a) - 1; ii >= 0; --ii) {
        sk_X509_NAME_ENTRY_push(res, X509_NAME_get_entry(a, ii));
    }

    return (res);
}

static
int xmlSecOpenSSLX509_NAME_ENTRIES_cmp(STACK_OF(X509_NAME_ENTRY)* a,  STACK_OF(X509_NAME_ENTRY)* b) {
    const X509_NAME_ENTRY *na;
    const X509_NAME_ENTRY *nb;
    int ii, ret;

    xmlSecAssert2(a != NULL, -1);
    xmlSecAssert2(b != NULL, 1);

    if (sk_X509_NAME_ENTRY_num(a) != sk_X509_NAME_ENTRY_num(b)) {
        return sk_X509_NAME_ENTRY_num(a) - sk_X509_NAME_ENTRY_num(b);
    }

    for (ii = sk_X509_NAME_ENTRY_num(a) - 1; ii >= 0; --ii) {
        na = sk_X509_NAME_ENTRY_value(a, ii);
        nb = sk_X509_NAME_ENTRY_value(b, ii);

        ret = xmlSecOpenSSLX509_NAME_ENTRY_cmp(&na, &nb);
        if(ret != 0) {
            return(ret);
        }
    }

    return(0);
}


/**
 * xmlSecOpenSSLX509NamesCompare:
 *
 * We have to sort X509_NAME entries to get correct results.
 * This is ugly but OpenSSL does not support it
 */
static int
xmlSecOpenSSLX509NamesCompare(X509_NAME *a, X509_NAME *b) {
    STACK_OF(X509_NAME_ENTRY) *a1 = NULL;
    STACK_OF(X509_NAME_ENTRY) *b1 = NULL;
    int ret;

    xmlSecAssert2(a != NULL, -1);
    xmlSecAssert2(b != NULL, 1);

    a1 = xmlSecOpenSSLX509_NAME_ENTRIES_copy(a);
    if(a1 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecOpenSSLX509_NAME_ENTRIES_copy",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    b1 = xmlSecOpenSSLX509_NAME_ENTRIES_copy(b);
    if(b1 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecOpenSSLX509_NAME_ENTRIES_copy",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        sk_X509_NAME_ENTRY_free(a1);
        return(1);
    }

    /* sort both */
    (void)sk_X509_NAME_ENTRY_set_cmp_func(a1, xmlSecOpenSSLX509_NAME_ENTRY_cmp);
    sk_X509_NAME_ENTRY_sort(a1);
    (void)sk_X509_NAME_ENTRY_set_cmp_func(b1, xmlSecOpenSSLX509_NAME_ENTRY_cmp);
    sk_X509_NAME_ENTRY_sort(b1);

    /* actually compare */
    ret = xmlSecOpenSSLX509_NAME_ENTRIES_cmp(a1, b1);

    /* cleanup */
    sk_X509_NAME_ENTRY_free(a1);
    sk_X509_NAME_ENTRY_free(b1);
    return(ret);
}

static int
xmlSecOpenSSLX509_NAME_ENTRY_cmp(const X509_NAME_ENTRY * const *a, const X509_NAME_ENTRY * const *b) {
    ASN1_STRING *a_value, *b_value;
    ASN1_OBJECT *a_name,  *b_name;
    int a_len, b_len;
    int ret;

    xmlSecAssert2(a != NULL, -1);
    xmlSecAssert2(b != NULL, 1);
    xmlSecAssert2((*a) != NULL, -1);
    xmlSecAssert2((*b) != NULL, 1);


    /* first compare values */
    a_value = X509_NAME_ENTRY_get_data((X509_NAME_ENTRY*)(*a));
    b_value = X509_NAME_ENTRY_get_data((X509_NAME_ENTRY*)(*b));

    if((a_value == NULL) && (b_value != NULL)) {
        return(-1);
    } else if((a_value != NULL) && (b_value == NULL)) {
        return(1);
    } else if((a_value == NULL) && (b_value == NULL)) {
        return(0);
    }

    a_len = ASN1_STRING_length(a_value);
    b_len = ASN1_STRING_length(b_value);
    ret = a_len - b_len;
    if(ret != 0) {
        return(ret);
    }

    if(a_len > 0) {
        ret = memcmp(ASN1_STRING_get0_data(a_value), ASN1_STRING_get0_data(b_value), a_len);
        if(ret != 0) {
            return(ret);
        }
    }

    /* next compare names */
    a_name = X509_NAME_ENTRY_get_object((X509_NAME_ENTRY*)(*a));
    b_name = X509_NAME_ENTRY_get_object((X509_NAME_ENTRY*)(*b));

    if((a_name == NULL) && (b_name != NULL)) {
        return(-1);
    } else if((a_name != NULL) && (b_name == NULL)) {
        return(1);
    } else if((a_name == NULL) && (b_name == NULL)) {
        return(0);
    }

    return(OBJ_cmp(a_name, b_name));
}


#endif /* XMLSEC_NO_X509 */



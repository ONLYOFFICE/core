/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/base64.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gnutls/crypto.h>

/**************************************************************************
 *
 * We use xmlsec-gcrypt for all the basic crypto ops
 *
 *****************************************************************************/
#include <xmlsec/gcrypt/crypto.h>
#include <gcrypt.h>

static void xmlSecGnuTLSDestroyParams(gnutls_datum_t * params, xmlSecSize num) {
    xmlSecSize ii;

    xmlSecAssert(params != NULL);
    for(ii = 0; ii < num; ++ii) {
        gnutls_free(params[ii].data);
    }
}

static void xmlSecGnuTLSDestroyMpis(gcry_mpi_t * mpis, xmlSecSize num) {
    xmlSecSize ii;

    xmlSecAssert(mpis != NULL);
    for(ii = 0; ii < num; ++ii) {
        gcry_mpi_release(mpis[ii]);
    }
}

static int xmlSecGnuTLSConvertParamsToMpis(gnutls_datum_t * params, xmlSecSize paramsNum,
                                           gcry_mpi_t * mpis, xmlSecSize mpisNum) {

    xmlSecSize ii;
    int rc;

    xmlSecAssert2(params != NULL, -1);
    xmlSecAssert2(mpis != NULL, -1);
    xmlSecAssert2(paramsNum == mpisNum, -1);

    for(ii = 0; ii < paramsNum; ++ii) {
        rc = gcry_mpi_scan(&(mpis[ii]), GCRYMPI_FMT_USG, params[ii].data, params[ii].size, NULL);
        if((rc != GPG_ERR_NO_ERROR) || (mpis[ii] == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_mpi_scan",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
            xmlSecGnuTLSDestroyMpis(mpis, ii); /* destroy up to now */
            return(-1);
        }
    }

    /* done */
    return(0);
}

#ifndef XMLSEC_NO_DSA

/**
 * xmlSecGnuTLSKeyDataDsaGetKlass:
 *
 * The DSA key data klass.
 *
 * Returns: pointer to DSA key data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataDsaGetKlass(void) {
    return (xmlSecGCryptKeyDataDsaGetKlass());
}

/**
 * xmlSecGnuTLSKeyDataDsaAdoptPrivateKey:
 * @data:               the pointer to DSA key data.
 * @dsa_key:            the pointer to GnuTLS DSA private key.
 *
 * Sets the value of DSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSKeyDataDsaAdoptPrivateKey(xmlSecKeyDataPtr data, gnutls_x509_privkey_t dsa_key) {
    gnutls_datum_t params[5];
    gcry_mpi_t mpis[5];
    gcry_sexp_t priv_key = NULL;
    gcry_sexp_t pub_key = NULL;
    int rc;
    int err;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataDsaId), -1);
    xmlSecAssert2(dsa_key != NULL, -1);
    xmlSecAssert2(gnutls_x509_privkey_get_pk_algorithm(dsa_key) == GNUTLS_PK_DSA, -1);

    /* get raw values */
    err = gnutls_x509_privkey_export_dsa_raw(dsa_key,
            &(params[0]), &(params[1]), &(params[2]),
            &(params[3]), &(params[4]));
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_privkey_export_dsa_raw",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(-1);
    }

    /* convert to mpis */
    ret = xmlSecGnuTLSConvertParamsToMpis(
            params, sizeof(params)/sizeof(params[0]),
            mpis, sizeof(mpis)/sizeof(mpis[0]));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSConvertParamsToMpis",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecGnuTLSDestroyParams(params, sizeof(params)/sizeof(params[0]));
        return(-1);
    }
    xmlSecGnuTLSDestroyParams(params, sizeof(params)/sizeof(params[0]));

    /* Convert from OpenSSL parameter ordering to the OpenPGP order. */
    /* First check that x < y; if not swap x and y  */
    if (gcry_mpi_cmp (mpis[4], mpis[3]) > 0) {
        gcry_mpi_swap (mpis[3], mpis[4]);
    }

    /* build expressions */
    rc = gcry_sexp_build(&(priv_key), NULL, "(private-key(dsa(p%m)(q%m)(g%m)(y%m)(x%m)))",
                        mpis[0], mpis[1], mpis[2], mpis[3], mpis[4]);
    if((rc != GPG_ERR_NO_ERROR) || (priv_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(private/dsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
        xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));
        return(-1);
    }
    rc = gcry_sexp_build(&(pub_key), NULL, "(public-key(dsa(p%m)(q%m)(g%m)(y%m)))",
                        mpis[0], mpis[1], mpis[2], mpis[3]);
    if((rc != GPG_ERR_NO_ERROR) || (pub_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(private/rsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
        gcry_sexp_release(priv_key);
        xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));
        return(-1);
    }
    xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));

    ret = xmlSecGCryptKeyDataDsaAdoptKeyPair(data, pub_key, priv_key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKeyDataDsaAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gcry_sexp_release(pub_key);
        gcry_sexp_release(priv_key);
        return(-1);
    }

    /* done, we "adopted" the key - destroy it! */
    gnutls_x509_privkey_deinit(dsa_key);
    return(0);
}


/**
 * xmlSecGnuTLSKeyDataDsaAdoptPublicKey:
 * @data:               the pointer to DSA key data.
 * @p:                  the pointer to p component of the DSA public key
 * @q:                  the pointer to q component of the DSA public key
 * @g:                  the pointer to g component of the DSA public key
 * @y:                  the pointer to y component of the DSA public key
 *
 * Sets the value of DSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSKeyDataDsaAdoptPublicKey(xmlSecKeyDataPtr data,
                                     gnutls_datum_t * p, gnutls_datum_t * q,
                                     gnutls_datum_t * g, gnutls_datum_t * y) {
    gnutls_datum_t params[4];
    gcry_mpi_t mpis[4];
    gcry_sexp_t pub_key = NULL;
    int rc;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataDsaId), -1);
    xmlSecAssert2(p != NULL, -1);
    xmlSecAssert2(q != NULL, -1);
    xmlSecAssert2(g != NULL, -1);
    xmlSecAssert2(y != NULL, -1);

    /* copy */
    memcpy(&(params[0]), p, sizeof(*p));
    memcpy(&(params[1]), q, sizeof(*q));
    memcpy(&(params[2]), g, sizeof(*g));
    memcpy(&(params[3]), y, sizeof(*y));

    /* convert to mpis */
    ret = xmlSecGnuTLSConvertParamsToMpis(
            params, sizeof(params)/sizeof(params[0]),
            mpis, sizeof(mpis)/sizeof(mpis[0]));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSConvertParamsToMpis",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        /* don't destroy params - we got them from outside !!! */
        return(-1);
    }
    /* don't destroy params - we got them from outside !!! */

    /* build expressions */
    rc = gcry_sexp_build(&(pub_key), NULL, "(public-key(dsa(p%m)(q%m)(g%m)(y%m)))",
                        mpis[0], mpis[1], mpis[2], mpis[3]);
    if((rc != GPG_ERR_NO_ERROR) || (pub_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(private/rsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
        xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));
        return(-1);
    }
    xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));

    ret = xmlSecGCryptKeyDataDsaAdoptKeyPair(data, pub_key, NULL);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKeyDataDsaAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gcry_sexp_release(pub_key);
        return(-1);
    }

    /* done, we "adopted" the key - destroy it! */
    gnutls_free(p->data);
    gnutls_free(q->data);
    gnutls_free(g->data);
    gnutls_free(y->data);
    return(0);
}

#endif /* XMLSEC_NO_DSA */


#ifndef XMLSEC_NO_RSA

/**
 * xmlSecGnuTLSKeyDataRsaGetKlass:
 *
 * The GnuTLS RSA key data klass.
 *
 * Returns: pointer to GnuTLS RSA key data klass.
 */
xmlSecKeyDataId
xmlSecGnuTLSKeyDataRsaGetKlass(void) {
    return (xmlSecGCryptKeyDataRsaGetKlass());
}

/**
 * xmlSecGnuTLSKeyDataRsaAdoptPrivateKey:
 * @data:               the pointer to RSA key data.
 * @rsa_key:            the pointer to GnuTLS RSA private key.
 *
 * Sets the value of RSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSKeyDataRsaAdoptPrivateKey(xmlSecKeyDataPtr data, gnutls_x509_privkey_t rsa_key) {
    gnutls_datum_t params[6];
    gcry_mpi_t mpis[6];
    gcry_sexp_t priv_key = NULL;
    gcry_sexp_t pub_key = NULL;
    int rc;
    int err;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataRsaId), -1);
    xmlSecAssert2(rsa_key != NULL, -1);
    xmlSecAssert2(gnutls_x509_privkey_get_pk_algorithm(rsa_key) == GNUTLS_PK_RSA, -1);

    /* get raw values */
    err = gnutls_x509_privkey_export_rsa_raw(rsa_key,
            &(params[0]), &(params[1]), &(params[2]),
            &(params[3]), &(params[4]), &(params[5]));
    if(err != GNUTLS_E_SUCCESS) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gnutls_x509_privkey_export_rsa_raw",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GNUTLS_REPORT_ERROR(err));
        return(-1);
    }

    /* convert to mpis */
    ret = xmlSecGnuTLSConvertParamsToMpis(
            params, sizeof(params)/sizeof(params[0]),
            mpis, sizeof(mpis)/sizeof(mpis[0]));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSConvertParamsToMpis",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecGnuTLSDestroyParams(params, sizeof(params)/sizeof(params[0]));
        return(-1);
    }
    xmlSecGnuTLSDestroyParams(params, sizeof(params)/sizeof(params[0]));

    /* Convert from OpenSSL parameter ordering to the OpenPGP order. */
    /* (http://gnupg.10057.n7.nabble.com/RSA-PKCS-1-signing-differs-from-OpenSSL-s-td27920.html) */
    /* First check that p < q; if not swap p and q and recompute u.  */ 
    if (gcry_mpi_cmp(mpis[3], mpis[4]) > 0) {
        gcry_mpi_swap(mpis[3], mpis[4]);
        gcry_mpi_invm(mpis[5], mpis[3], mpis[4]);
    }

    /* build expressions */
    rc = gcry_sexp_build(&(priv_key), NULL, "(private-key(rsa((n%m)(e%m)(d%m)(p%m)(q%m)(u%m))))",
                        mpis[0], mpis[1], mpis[2],
                        mpis[3], mpis[4], mpis[5]);
    if((rc != GPG_ERR_NO_ERROR) || (priv_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(private/rsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
        xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));
        return(-1);
    }
    rc = gcry_sexp_build(&(pub_key), NULL, "(public-key(rsa((n%m)(e%m))))",
                        mpis[0], mpis[1]);
    if((rc != GPG_ERR_NO_ERROR) || (pub_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(private/rsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
        gcry_sexp_release(priv_key);
        xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));
        return(-1);
    }
    xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));

    ret = xmlSecGCryptKeyDataRsaAdoptKeyPair(data, pub_key, priv_key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKeyDataRsaAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gcry_sexp_release(pub_key);
        gcry_sexp_release(priv_key);
        return(-1);
    }

    /* done, we "adopted" the key - destroy it! */
    gnutls_x509_privkey_deinit(rsa_key);
    return(0);
}


/**
 * xmlSecGnuTLSKeyDataRsaAdoptPublicKey:
 * @data:               the pointer to RSA key data.
 * @m:                  the pointer to m component of the RSA public key
 * @e:                  the pointer to e component of the RSA public key
 *
 * Sets the value of RSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGnuTLSKeyDataRsaAdoptPublicKey(xmlSecKeyDataPtr data,
                                     gnutls_datum_t * m, gnutls_datum_t * e) {
    gnutls_datum_t params[2];
    gcry_mpi_t mpis[2];
    gcry_sexp_t pub_key = NULL;
    int rc;
    int ret;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGnuTLSKeyDataRsaId), -1);
    xmlSecAssert2(m != NULL, -1);
    xmlSecAssert2(e != NULL, -1);

    /* copy */
    memcpy(&(params[0]), m, sizeof(*m));
    memcpy(&(params[1]), e, sizeof(*e));

    /* convert to mpis */
    ret = xmlSecGnuTLSConvertParamsToMpis(
            params, sizeof(params)/sizeof(params[0]),
            mpis, sizeof(mpis)/sizeof(mpis[0]));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGnuTLSConvertParamsToMpis",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        /* don't destroy params - we got them from outside !!! */
        return(-1);
    }
    /* don't destroy params - we got them from outside !!! */

    /* build expressions */
    rc = gcry_sexp_build(&(pub_key), NULL, "(public-key(rsa((n%m)(e%m))))",
                        mpis[0], mpis[1]);
    if((rc != GPG_ERR_NO_ERROR) || (pub_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(private/rsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GNUTLS_GCRYPT_REPORT_ERROR(rc));
        xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));
        return(-1);
    }
    xmlSecGnuTLSDestroyMpis(mpis, sizeof(mpis)/sizeof(mpis[0]));

    ret = xmlSecGCryptKeyDataRsaAdoptKeyPair(data, pub_key, NULL);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKeyDataRsaAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        gcry_sexp_release(pub_key);
        return(-1);
    }

    /* done, we "adopted" the key - destroy it! */
    gnutls_free(m->data);
    gnutls_free(e->data);
    return(0);
}
#endif /* XMLSEC_NO_RSA */

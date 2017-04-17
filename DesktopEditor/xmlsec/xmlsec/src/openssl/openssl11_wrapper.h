#ifndef __XMLSEC_OPENSSL_OPENSSL11_WRAPPER_H__
#define __XMLSEC_OPENSSL_OPENSSL11_WRAPPER_H__
/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * This file provides a compatibility layer for pre-OpenSSL 1.1.0 versions.
 * The functions here provide accessors for structs which were made opaque in
 * 1.1.0 so they an be accessed in earlier versions of the library using the
 * same syntax. This file won't be required once OpenSSL 1.1.0 is the minimum
 * suported version.
 */
#if (OPENSSL_VERSION_NUMBER < 0x10100000)

#define EVP_PKEY_up_ref(pKey)  CRYPTO_add(&((pKey)->references), 1, CRYPTO_LOCK_EVP_PKEY)

#ifndef XMLSEC_NO_X509
#include <openssl/x509_vfy.h>

#define ASN1_STRING_get0_data(data)        ASN1_STRING_data((data))
#define X509_CRL_get0_nextUpdate(crl)      X509_CRL_get_nextUpdate((crl))
#define X509_REVOKED_get0_serialNumber(r)  (((r) != NULL) ? ((r)->serialNumber) : (const ASN1_INTEGER *)NULL)
#define X509_OBJECT_get0_X509(x)           (((x) != NULL) ? ((x)->data.x509) : (X509 *)NULL)

static inline X509_OBJECT *X509_OBJECT_new() {
    return(calloc(1, sizeof(X509_OBJECT)));
}

static inline void X509_OBJECT_free(X509_OBJECT *a) {
    if(a == NULL) {
        return;
    }
    X509_OBJECT_free_contents(a);
    free(a);
}

#endif /* XMLSEC_NO_X509 */

#ifndef XMLSEC_NO_RSA

#define EVP_PKEY_get0_RSA(pKey)  (((pKey) != NULL) ? ((pKey)->pkey.rsa) : (RSA*)NULL)

static inline void RSA_get0_key(const RSA *r, const BIGNUM **n, const BIGNUM **e, const BIGNUM **d) {
    xmlSecAssert(r != NULL);

    if(n != NULL) {
        (*n) = r->n;
    }
    if(e != NULL) {
        (*e) = r->e;
    }
    if(d != NULL) {
        (*d) = r->d;
    }
}

static inline int RSA_test_flags(const RSA *r, int flags) {
    xmlSecAssert2(r != NULL, 0);
    return(r->flags & flags);
}

static inline int RSA_set0_key(RSA *r, BIGNUM *n, BIGNUM *e, BIGNUM *d) {
    xmlSecAssert2(r != NULL, 0);

    if(((r->n == NULL) && (n == NULL)) || ((r->e == NULL) && (e == NULL))) {
        return(0);
    }
    if(n != NULL) {
        BN_free(r->n);
        r->n = n;
    }
    if(e != NULL) {
        BN_free(r->e);
        r->e = e;
    }
    if(d != NULL) {
        BN_free(r->d);
        r->d = d;
    }
    return(1);
}
#endif /* XMLSEC_NO_RSA */

#ifndef XMLSEC_NO_ECDSA

#define EVP_PKEY_get0_EC_KEY(pKey)  (((pKey) != NULL) ? ((pKey)->pkey.ec) : (EC_KEY*)NULL)

static inline void ECDSA_SIG_get0(const ECDSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps) {
    xmlSecAssert(sig != NULL);

    if(pr != NULL) {
        (*pr) = sig->r;
    }
    if(ps != NULL) {
        (*ps) = sig->s;
    }
}

static inline int ECDSA_SIG_set0(ECDSA_SIG *sig, BIGNUM *r, BIGNUM *s) {
    xmlSecAssert2(sig != NULL, 0);

    if((r == NULL) || (s == NULL)) {
        return(0);
    }
    BN_clear_free(sig->r);
    BN_clear_free(sig->s);
    sig->r = r;
    sig->s = s;
    return(1);
}
#endif /* XMLSEC_NO_ECDSA */

#ifndef XMLSEC_NO_DSA

#define EVP_PKEY_get0_DSA(pKey)  (((pKey) != NULL) ? ((pKey)->pkey.dsa) : (DSA*)NULL)

static inline void DSA_SIG_get0(const DSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps) {
    xmlSecAssert(sig != NULL);
    
    if(pr != NULL) {
        (*pr) = sig->r;
    }
    if(ps != NULL) {
        (*ps) = sig->s;
    }
}

static inline int DSA_SIG_set0(DSA_SIG *sig, BIGNUM *r, BIGNUM *s) {
    xmlSecAssert2(sig != NULL, 0);
    
    if(r == NULL || s == NULL) {
        return(0);
    }
    BN_clear_free(sig->r);
    BN_clear_free(sig->s);
    
    sig->r = r;
    sig->s = s;
    return(1);
}

static inline void DSA_get0_pqg(const DSA *d, const BIGNUM **p, const BIGNUM **q, const BIGNUM **g) {
    xmlSecAssert(d != NULL);

    if(p != NULL) {
        (*p) = d->p;
    }
    if(q != NULL) {
        (*q) = d->q;
    }
    if(g != NULL) {
        (*g) = d->g;
    }
}

static inline void DSA_get0_key(const DSA *d, const BIGNUM **pub_key, const BIGNUM **priv_key) {
    xmlSecAssert(d != NULL);

    if(pub_key != NULL) {
        (*pub_key) = d->pub_key;
    }
    if(priv_key != NULL) {
        (*priv_key) = d->priv_key;
    }
}

static inline ENGINE *DSA_get0_engine(DSA *d) {
    xmlSecAssert2(d != NULL, NULL);
    return(d->engine);
}

static inline int DSA_set0_pqg(DSA *d, BIGNUM *p, BIGNUM *q, BIGNUM *g) {
    xmlSecAssert2(d != NULL, 0);
    
    if(((d->p == NULL) && (p == NULL)) || ((d->q == NULL) && (q == NULL)) || ((d->g == NULL) && (g == NULL))) {
        return(0);
    }

    if(p != NULL) {
        BN_free(d->p);
        d->p = p;
    }
    if(q != NULL) {
        BN_free(d->q);
        d->q = q;
    }
    if(g != NULL) {
        BN_free(d->g);
        d->g = g;
    }
    return(1);
}

static inline int DSA_set0_key(DSA *d, BIGNUM *pub_key, BIGNUM *priv_key) {
    xmlSecAssert2(d != NULL, 0);

    if((d->pub_key == NULL) && (pub_key == NULL)) {
        return(0);
    }

    if(pub_key != NULL) {
        BN_free(d->pub_key);
        d->pub_key = pub_key;
    }
    if(priv_key != NULL) {
        BN_free(d->priv_key);
        d->priv_key = priv_key;
    }
    return(1);
}
#endif /* XMLSEC_NO_DSA */

#endif /* (OPENSSL_VERSION_NUMBER < 0x10100000) */
#endif /* __XMLSEC_OPENSSL_OPENSSL11_WRAPPER_H__ */

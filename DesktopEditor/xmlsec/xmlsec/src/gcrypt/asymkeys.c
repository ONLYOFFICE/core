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

#include <gcrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/base64.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/crypto.h>

/**************************************************************************
 *
 * Helpers
 *
 *************************************************************************/
static gcry_sexp_t             xmlSecGCryptAsymSExpDup    (gcry_sexp_t sexp);


/**************************************************************************
 *
 * Internal GCrypt asym key CTX
 *
 *************************************************************************/
typedef struct _xmlSecGCryptAsymKeyDataCtx       xmlSecGCryptAsymKeyDataCtx,
                                                *xmlSecGCryptAsymKeyDataCtxPtr;
struct _xmlSecGCryptAsymKeyDataCtx {
    gcry_sexp_t pub_key;
    gcry_sexp_t priv_key;
};

/******************************************************************************
 *
 * Asym key (dsa/rsa)
 *
 * xmlSecGCryptAsymKeyDataCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecGCryptAsymKeyDataSize     \
    (sizeof(xmlSecKeyData) + sizeof(xmlSecGCryptAsymKeyDataCtx))
#define xmlSecGCryptAsymKeyDataGetCtx(data) \
    ((xmlSecGCryptAsymKeyDataCtxPtr)(((xmlSecByte*)(data)) + sizeof(xmlSecKeyData)))

static int              xmlSecGCryptAsymKeyDataInitialize       (xmlSecKeyDataPtr data);
static int              xmlSecGCryptAsymKeyDataDuplicate        (xmlSecKeyDataPtr dst,
                                                                 xmlSecKeyDataPtr src);
static void             xmlSecGCryptAsymKeyDataFinalize         (xmlSecKeyDataPtr data);

static int              xmlSecGCryptAsymKeyDataAdoptKey         (xmlSecKeyDataPtr data,
                                                                 gcry_sexp_t key_pair);
static int              xmlSecGCryptAsymKeyDataAdoptKeyPair     (xmlSecKeyDataPtr data,
                                                                 gcry_sexp_t pub_key,
                                                                 gcry_sexp_t priv_key);
static gcry_sexp_t      xmlSecGCryptAsymKeyDataGetPublicKey     (xmlSecKeyDataPtr data);
static gcry_sexp_t      xmlSecGCryptAsymKeyDataGetPrivateKey    (xmlSecKeyDataPtr data);
static int              xmlSecGCryptAsymKeyDataGenerate         (xmlSecKeyDataPtr data,
                                                                 const char * alg,
                                                                 xmlSecSize key_size);
static xmlSecKeyDataType xmlSecGCryptAsymKeyDataGetType         (xmlSecKeyDataPtr data);
static xmlSecSize       xmlSecGCryptAsymKeyDataGetSize          (xmlSecKeyDataPtr data);


static int
xmlSecGCryptAsymKeyDataInitialize(xmlSecKeyDataPtr data) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), -1);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecGCryptAsymKeyDataCtx));

    return(0);
}

static int
xmlSecGCryptAsymKeyDataDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecGCryptAsymKeyDataCtxPtr ctxDst;
    xmlSecGCryptAsymKeyDataCtxPtr ctxSrc;

    xmlSecAssert2(xmlSecKeyDataIsValid(dst), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(dst, xmlSecGCryptAsymKeyDataSize), -1);
    xmlSecAssert2(xmlSecKeyDataIsValid(src), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(src, xmlSecGCryptAsymKeyDataSize), -1);

    ctxDst = xmlSecGCryptAsymKeyDataGetCtx(dst);
    xmlSecAssert2(ctxDst != NULL, -1);
    xmlSecAssert2(ctxDst->pub_key == NULL, -1);
    xmlSecAssert2(ctxDst->priv_key == NULL, -1);

    ctxSrc = xmlSecGCryptAsymKeyDataGetCtx(src);
    xmlSecAssert2(ctxSrc != NULL, -1);

    if(ctxSrc->pub_key != NULL) {
        ctxDst->pub_key = xmlSecGCryptAsymSExpDup(ctxSrc->pub_key);
        if(ctxDst->pub_key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "xmlSecGCryptAsymSExpDup(pub_key)",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    if(ctxSrc->priv_key != NULL) {
        ctxDst->priv_key = xmlSecGCryptAsymSExpDup(ctxSrc->priv_key);
        if(ctxDst->priv_key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                        "xmlSecGCryptAsymSExpDup(priv_key)",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(-1);
        }
    }

    return(0);
}

static void
xmlSecGCryptAsymKeyDataFinalize(xmlSecKeyDataPtr data) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize));

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert(ctx != NULL);

    if(ctx->pub_key != NULL) {
        gcry_sexp_release(ctx->pub_key);
    }
    if(ctx->priv_key != NULL) {
        gcry_sexp_release(ctx->priv_key);
    }
    memset(ctx, 0, sizeof(xmlSecGCryptAsymKeyDataCtx));
}

static int
xmlSecGCryptAsymKeyDataAdoptKey(xmlSecKeyDataPtr data, gcry_sexp_t key_pair) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;
    gcry_sexp_t pub_key = NULL;
    gcry_sexp_t priv_key = NULL;
    int res = -1;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), -1);
    xmlSecAssert2(key_pair != NULL, -1);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    /* split the key pair, public part should be always present, private might 
       not be present */
    pub_key = gcry_sexp_find_token(key_pair, "public-key", 0);
    if(pub_key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token(public-key)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    priv_key = gcry_sexp_find_token(key_pair, "private-key", 0);

    /* assign */
    if(xmlSecGCryptAsymKeyDataAdoptKeyPair(data, pub_key, priv_key) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAsymKeyDataAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    pub_key = NULL; /* data owns it now */
    priv_key = NULL; /* data owns it now */

    /* success */
    res = 0;

done:
    if(pub_key != NULL) {
        gcry_sexp_release(pub_key);
    }

    if(priv_key != NULL) {
        gcry_sexp_release(priv_key);
    }

    /* done */
    return(res);
}

static int
xmlSecGCryptAsymKeyDataAdoptKeyPair(xmlSecKeyDataPtr data, gcry_sexp_t pub_key, gcry_sexp_t priv_key) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), -1);
    xmlSecAssert2(pub_key != NULL, -1); /* public key should present always */
/*
    aleksey - we don't set optional parameters for RSA keys (p, k, u) and
    because of that we can't actually test the key

    xmlSecAssert2(((priv_key == NULL) || (gcry_pk_testkey(priv_key) == GPG_ERR_NO_ERROR)), -1);
*/

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    /* release prev values and assign new ones */
    if(ctx->pub_key != NULL) {
        gcry_sexp_release(ctx->pub_key);
    }
    if(ctx->priv_key != NULL) {
        gcry_sexp_release(ctx->priv_key);
    }

    ctx->pub_key = pub_key;
    ctx->priv_key = priv_key;

    /* done */
    return(0);
}

static gcry_sexp_t
xmlSecGCryptAsymKeyDataGetPublicKey(xmlSecKeyDataPtr data) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), NULL);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    return(ctx->pub_key);
}

static gcry_sexp_t
xmlSecGCryptAsymKeyDataGetPrivateKey(xmlSecKeyDataPtr data) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), NULL);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);

    return(ctx->priv_key);
}

static int
xmlSecGCryptAsymKeyDataGenerate(xmlSecKeyDataPtr data, const char * alg, xmlSecSize key_size) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;
    gcry_sexp_t key_spec = NULL;
    gcry_sexp_t key_pair = NULL;
    gcry_error_t err;
    int ret;
    int res = -1;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), -1);
    xmlSecAssert2(alg != NULL, -1);
    xmlSecAssert2(key_size > 0, -1);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    err = gcry_sexp_build(&key_spec, NULL,
                          "(genkey (%s (nbits %d)(transient-key)))",
                          alg, (int)key_size);
    if((err != GPG_ERR_NO_ERROR) || (key_spec == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_build(genkey)",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    err = gcry_pk_genkey(&key_pair, key_spec);
    if((err != GPG_ERR_NO_ERROR) || (key_pair == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_pk_genkey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

    ret = xmlSecGCryptAsymKeyDataAdoptKey(data, key_pair);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAsymKeyDataAdopt",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "ret=%d", (int)ret);
        goto done;
    }
    key_pair = NULL; /* now owned by data */

    /* success */
    res = 0;

done:
    if(key_spec != NULL) {
        gcry_sexp_release(key_spec);
    }
    if(key_pair != NULL) {
        gcry_sexp_release(key_pair);
    }

    return(res);
}

static xmlSecKeyDataType
xmlSecGCryptAsymKeyDataGetType(xmlSecKeyDataPtr data) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), xmlSecKeyDataTypeUnknown);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), xmlSecKeyDataTypeUnknown);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, xmlSecKeyDataTypeUnknown);

    if((ctx->priv_key != NULL) && (ctx->pub_key != NULL)) {
        return (xmlSecKeyDataTypePrivate | xmlSecKeyDataTypePublic);
    } else if(ctx->pub_key != NULL) {
        return (xmlSecKeyDataTypePublic);
    }

    return (xmlSecKeyDataTypeUnknown);
}

static xmlSecSize
xmlSecGCryptAsymKeyDataGetSize(xmlSecKeyDataPtr data) {
    xmlSecGCryptAsymKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), xmlSecKeyDataTypeUnknown);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecGCryptAsymKeyDataSize), xmlSecKeyDataTypeUnknown);

    ctx = xmlSecGCryptAsymKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, 0);

    /* use pub key since it is more often you have it than not */
    return (ctx->pub_key != NULL) ? gcry_pk_get_nbits(ctx->pub_key) : 0;
}

/******************************************************************************
 *
 * helper functions
 *
 *****************************************************************************/
static gcry_sexp_t
xmlSecGCryptAsymSExpDup(gcry_sexp_t pKey) {
    gcry_sexp_t res = NULL;
    xmlSecByte *buf = NULL;
    gcry_error_t err;
    size_t size;

    xmlSecAssert2(pKey != NULL, NULL);

    size = gcry_sexp_sprint(pKey, GCRYSEXP_FMT_ADVANCED, NULL, 0);
    if(size == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_sprint",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    buf = (xmlSecByte *)xmlMalloc(size);
    if(buf == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlMalloc",
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)size);
        goto done;
    }

    size = gcry_sexp_sprint(pKey, GCRYSEXP_FMT_ADVANCED, buf, size);
    if(size == 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_sprint",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "size=%d", (int)size);
        goto done;
    }

    err = gcry_sexp_new(&res, buf, size, 1);
    if((err != GPG_ERR_NO_ERROR) || (res == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_new",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }

done:
    if(buf != NULL) {
        xmlFree(buf);
    }
    return (res);
}

/**
 * xmlSecGCryptNodeGetMpiValue:
 * @cur: the poitner to an XML node.
 *
 * Converts the node content from CryptoBinary format
 * (http://www.w3.org/TR/xmldsig-core/#sec-CryptoBinary)
 * to a BIGNUM. If no BIGNUM buffer provided then a new
 * BIGNUM is created (caller is responsible for freeing it).
 *
 * Returns: a pointer to MPI produced from CryptoBinary string
 * or NULL if an error occurs.
 */
static gcry_mpi_t
xmlSecGCryptNodeGetMpiValue(const xmlNodePtr cur) {
    xmlSecBuffer buf;
    gcry_mpi_t res = NULL;
    gcry_error_t err;
    int ret;

    xmlSecAssert2(cur != NULL, NULL);

    ret = xmlSecBufferInitialize(&buf, 128);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    ret = xmlSecBufferBase64NodeContentRead(&buf, cur);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferBase64NodeContentRead",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBufferFinalize(&buf);
        return(NULL);
    }

    err = gcry_mpi_scan(&res, GCRYMPI_FMT_USG,
                         xmlSecBufferGetData(&buf),
                         xmlSecBufferGetSize(&buf),
                         NULL);
    if((err != GPG_ERR_NO_ERROR) || (res == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_scan",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        xmlSecBufferFinalize(&buf);
        return(NULL);
    }

    /* done */
    xmlSecBufferFinalize(&buf);
    return(res);
}

/**
 * xmlSecGCryptNodeSetMpiValue:
 * @cur: the pointer to an XML node.
 * @a: the mpi value
 * @addLineBreaks: if the flag is equal to 1 then
 *              linebreaks will be added before and after
 *              new buffer content.
 *
 * Converts MPI to CryptoBinary string
 * (http://www.w3.org/TR/xmldsig-core/#sec-CryptoBinary)
 * and sets it as the content of the given node. If the
 * addLineBreaks is set then line breaks are added
 * before and after the CryptoBinary string.
 *
 * Returns: 0 on success or -1 otherwise.
 */
static int
xmlSecGCryptNodeSetMpiValue(xmlNodePtr cur, const gcry_mpi_t a, int addLineBreaks) {
    xmlSecBuffer buf;
    gcry_error_t err;
    size_t written = 0;
    int ret;

    xmlSecAssert2(a != NULL, -1);
    xmlSecAssert2(cur != NULL, -1);

    written = 0;
    err = gcry_mpi_print(GCRYMPI_FMT_USG, NULL, 0, &written, a);
    if((err != GPG_ERR_NO_ERROR) || (written == 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_print",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        return(-1);
    }

    ret = xmlSecBufferInitialize(&buf, written + 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", (int)written + 1);
        return(-1);
    }

    written = 0;
    err = gcry_mpi_print(GCRYMPI_FMT_USG,
            xmlSecBufferGetData(&buf),
            xmlSecBufferGetMaxSize(&buf),
            &written, a);
    if((err != GPG_ERR_NO_ERROR) || (written == 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_mpi_print",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        xmlSecBufferFinalize(&buf);
        return(-1);
    }

    ret = xmlSecBufferSetSize(&buf, written);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferSetSize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "written=%d", (int)written);
        xmlSecBufferFinalize(&buf);
        return(-1);
    }

    if(addLineBreaks) {
        xmlNodeSetContent(cur, xmlSecStringCR);
    } else {
        xmlNodeSetContent(cur, xmlSecStringEmpty);
    }

    ret = xmlSecBufferBase64NodeContentWrite(&buf, cur, xmlSecBase64GetDefaultLineSize());
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferBase64NodeContentWrite",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecBufferFinalize(&buf);
        return(-1);
    }

    if(addLineBreaks) {
        xmlNodeAddContent(cur, xmlSecStringCR);
    }

    xmlSecBufferFinalize(&buf);
    return(0);
}

/**
 * xmlSecGCryptNodeSetSExpTokValue:
 * @cur: the pointer to an XML node.
 * @sexp: the sexp
 * @tok: the token
 * @addLineBreaks: if the flag is equal to 1 then
 *              linebreaks will be added before and after
 *              new buffer content.
 *
 * Converts MPI to CryptoBinary string
 * (http://www.w3.org/TR/xmldsig-core/#sec-CryptoBinary)
 * and sets it as the content of the given node. If the
 * addLineBreaks is set then line breaks are added
 * before and after the CryptoBinary string.
 *
 * Returns: 0 on success or -1 otherwise.
 */
static int
xmlSecGCryptNodeSetSExpTokValue(xmlNodePtr cur, const gcry_sexp_t sexp,
                                const char * tok, int addLineBreaks)
{
    gcry_sexp_t val = NULL;
    gcry_mpi_t mpi = NULL;
    int res = -1;

    xmlSecAssert2(cur != NULL, -1);
    xmlSecAssert2(sexp != NULL, -1);
    xmlSecAssert2(tok != NULL, -1);

    val = gcry_sexp_find_token(sexp, tok, 0);
    if(val == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_find_token",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "tok=%s",
                    xmlSecErrorsSafeString(tok));
        goto done;
    }

    mpi = gcry_sexp_nth_mpi(val, 1, GCRYMPI_FMT_USG);
    if(mpi == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_sexp_nth_mpi",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "tok=%s",
                    xmlSecErrorsSafeString(tok));
        goto done;
    }

    /* almost done */
    res = xmlSecGCryptNodeSetMpiValue(cur, mpi, addLineBreaks);

done:
    if(mpi != NULL) {
        gcry_mpi_release(mpi);
    }
    if(val != NULL) {
        gcry_sexp_release(val);
    }

    return(res);
}

#ifndef XMLSEC_NO_DSA
/**************************************************************************
 *
 * <dsig:DSAKeyValue> processing
 *
 *
 * The DSAKeyValue Element (http://www.w3.org/TR/xmldsig-core/#sec-DSAKeyValue)
 *
 * DSA keys and the DSA signature algorithm are specified in [DSS].
 * DSA public key values can have the following fields:
 *
 *   * P - a prime modulus meeting the [DSS] requirements
 *   * Q - an integer in the range 2**159 < Q < 2**160 which is a prime
 *         divisor of P-1
 *   * G - an integer with certain properties with respect to P and Q
 *   * Y - G**X mod P (where X is part of the private key and not made
 *         public)
 *   * J - (P - 1) / Q
 *   * seed - a DSA prime generation seed
 *   * pgenCounter - a DSA prime generation counter
 *
 * Parameter J is available for inclusion solely for efficiency as it is
 * calculatable from P and Q. Parameters seed and pgenCounter are used in the
 * DSA prime number generation algorithm specified in [DSS]. As such, they are
 * optional but must either both be present or both be absent. This prime
 * generation algorithm is designed to provide assurance that a weak prime is
 * not being used and it yields a P and Q value. Parameters P, Q, and G can be
 * public and common to a group of users. They might be known from application
 * context. As such, they are optional but P and Q must either both appear or
 * both be absent. If all of P, Q, seed, and pgenCounter are present,
 * implementations are not required to check if they are consistent and are
 * free to use either P and Q or seed and pgenCounter. All parameters are
 * encoded as base64 [MIME] values.
 *
 * Arbitrary-length integers (e.g. "bignums" such as RSA moduli) are
 * represented in XML as octet strings as defined by the ds:CryptoBinary type.
 *
 * Schema Definition:
 *
 * <element name="DSAKeyValue" type="ds:DSAKeyValueType"/>
 * <complexType name="DSAKeyValueType">
 *   <sequence>
 *     <sequence minOccurs="0">
 *        <element name="P" type="ds:CryptoBinary"/>
 *        <element name="Q" type="ds:CryptoBinary"/>
 *     </sequence>
 *     <element name="G" type="ds:CryptoBinary" minOccurs="0"/>
 *     <element name="Y" type="ds:CryptoBinary"/>
 *     <element name="J" type="ds:CryptoBinary" minOccurs="0"/>
 *     <sequence minOccurs="0">
 *       <element name="Seed" type="ds:CryptoBinary"/>
 *       <element name="PgenCounter" type="ds:CryptoBinary"/>
 *     </sequence>
 *   </sequence>
 * </complexType>
 *
 * DTD Definition:
 *
 *  <!ELEMENT DSAKeyValue ((P, Q)?, G?, Y, J?, (Seed, PgenCounter)?) >
 *  <!ELEMENT P (#PCDATA) >
 *  <!ELEMENT Q (#PCDATA) >
 *  <!ELEMENT G (#PCDATA) >
 *  <!ELEMENT Y (#PCDATA) >
 *  <!ELEMENT J (#PCDATA) >
 *  <!ELEMENT Seed (#PCDATA) >
 *  <!ELEMENT PgenCounter (#PCDATA) >
 *
 * ============================================================================
 *
 * To support reading/writing private keys an X element added (before Y).
 * todo: The current implementation does not support Seed and PgenCounter!
 * by this the P, Q and G are *required*!
 *
 *************************************************************************/
static int              xmlSecGCryptKeyDataDsaInitialize        (xmlSecKeyDataPtr data);
static int              xmlSecGCryptKeyDataDsaDuplicate         (xmlSecKeyDataPtr dst,
                                                                 xmlSecKeyDataPtr src);
static void             xmlSecGCryptKeyDataDsaFinalize          (xmlSecKeyDataPtr data);
static int              xmlSecGCryptKeyDataDsaXmlRead           (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGCryptKeyDataDsaXmlWrite          (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGCryptKeyDataDsaGenerate          (xmlSecKeyDataPtr data,
                                                                 xmlSecSize sizeBits,
                                                                 xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecGCryptKeyDataDsaGetType          (xmlSecKeyDataPtr data);
static xmlSecSize       xmlSecGCryptKeyDataDsaGetSize           (xmlSecKeyDataPtr data);
static void             xmlSecGCryptKeyDataDsaDebugDump         (xmlSecKeyDataPtr data,
                                                                 FILE* output);
static void             xmlSecGCryptKeyDataDsaDebugXmlDump      (xmlSecKeyDataPtr data,
                                                                 FILE* output);

static xmlSecKeyDataKlass xmlSecGCryptKeyDataDsaKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecGCryptAsymKeyDataSize,

    /* data */
    xmlSecNameDSAKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefDSAKeyValue,                      /* const xmlChar* href; */
    xmlSecNodeDSAKeyValue,                      /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                               /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecGCryptKeyDataDsaInitialize,          /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecGCryptKeyDataDsaDuplicate,           /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecGCryptKeyDataDsaFinalize,            /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecGCryptKeyDataDsaGenerate,            /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecGCryptKeyDataDsaGetType,             /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecGCryptKeyDataDsaGetSize,             /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecGCryptKeyDataDsaXmlRead,             /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecGCryptKeyDataDsaXmlWrite,            /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    NULL,                                       /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                                       /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecGCryptKeyDataDsaDebugDump,           /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecGCryptKeyDataDsaDebugXmlDump,        /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptKeyDataDsaGetKlass:
 *
 * The DSA key data klass.
 *
 * Returns: pointer to DSA key data klass.
 */
xmlSecKeyDataId
xmlSecGCryptKeyDataDsaGetKlass(void) {
    return(&xmlSecGCryptKeyDataDsaKlass);
}

/**
 * xmlSecGCryptKeyDataDsaAdoptKey:
 * @data:               the pointer to DSA key data.
 * @dsa_key:            the pointer to GCrypt DSA key.
 *
 * Sets the value of DSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptKeyDataDsaAdoptKey(xmlSecKeyDataPtr data, gcry_sexp_t dsa_key) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), -1);
    xmlSecAssert2(dsa_key != NULL, -1);

    return xmlSecGCryptAsymKeyDataAdoptKey(data, dsa_key);
}


/**
 * xmlSecGCryptKeyDataDsaAdoptKeyPair:
 * @data:               the pointer to DSA key data.
 * @pub_key:            the pointer to GCrypt DSA pub key.
 * @priv_key:           the pointer to GCrypt DSA priv key.
 *
 * Sets the value of DSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptKeyDataDsaAdoptKeyPair(xmlSecKeyDataPtr data, gcry_sexp_t pub_key, gcry_sexp_t priv_key) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), -1);
    xmlSecAssert2(pub_key != NULL, -1);

    return xmlSecGCryptAsymKeyDataAdoptKeyPair(data, pub_key, priv_key);
}

/**
 * xmlSecGCryptKeyDataDsaGetPublicKey:
 * @data:               the pointer to DSA key data.
 *
 * Gets the GCrypt DSA public key from DSA key data.
 *
 * Returns: pointer to GCrypt public DSA key or NULL if an error occurs.
 */
gcry_sexp_t
xmlSecGCryptKeyDataDsaGetPublicKey(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), NULL);
    return xmlSecGCryptAsymKeyDataGetPublicKey(data);
}

/**
 * xmlSecGCryptKeyDataDsaGetPrivateKey:
 * @data:               the pointer to DSA key data.
 *
 * Gets the GCrypt DSA private key from DSA key data.
 *
 * Returns: pointer to GCrypt private DSA key or NULL if an error occurs.
 */
gcry_sexp_t
xmlSecGCryptKeyDataDsaGetPrivateKey(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), NULL);
    return xmlSecGCryptAsymKeyDataGetPrivateKey(data);
}

static int
xmlSecGCryptKeyDataDsaInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), -1);

    return(xmlSecGCryptAsymKeyDataInitialize(data));
}

static int
xmlSecGCryptKeyDataDsaDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecKeyDataCheckId(dst, xmlSecGCryptKeyDataDsaId), -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(src, xmlSecGCryptKeyDataDsaId), -1);

    return(xmlSecGCryptAsymKeyDataDuplicate(dst, src));
}

static void
xmlSecGCryptKeyDataDsaFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId));

    xmlSecGCryptAsymKeyDataFinalize(data);
}

static int
xmlSecGCryptKeyDataDsaGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    return xmlSecGCryptAsymKeyDataGenerate(data, "dsa", sizeBits);
}

static xmlSecKeyDataType
xmlSecGCryptKeyDataDsaGetType(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), xmlSecKeyDataTypeUnknown);

    return xmlSecGCryptAsymKeyDataGetType(data);
}

static xmlSecSize
xmlSecGCryptKeyDataDsaGetSize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId), 0);

    return xmlSecGCryptAsymKeyDataGetSize(data);
}

static void
xmlSecGCryptKeyDataDsaDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "=== dsa key: size = %d\n",
            xmlSecGCryptKeyDataDsaGetSize(data));
}

static void
xmlSecGCryptKeyDataDsaDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataDsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "<DSAKeyValue size=\"%d\" />\n",
            xmlSecGCryptKeyDataDsaGetSize(data));
}

static int
xmlSecGCryptKeyDataDsaXmlRead(xmlSecKeyDataId id,
                              xmlSecKeyPtr key,
                              xmlNodePtr node,
                              xmlSecKeyInfoCtxPtr keyInfoCtx)
{
    xmlNodePtr cur;
    xmlSecKeyDataPtr data = NULL;
    gcry_mpi_t p = NULL;
    gcry_mpi_t q = NULL;
    gcry_mpi_t g = NULL;
    gcry_mpi_t x = NULL;
    gcry_mpi_t y = NULL;
    gcry_sexp_t pub_key = NULL;
    gcry_sexp_t priv_key = NULL;
    gcry_error_t err;
    int res = -1;
    int ret;

    xmlSecAssert2(id == xmlSecGCryptKeyDataDsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if(xmlSecKeyGetValue(key) != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    cur = xmlSecGetNextElementNode(node->children);

    /* first is P node. It is REQUIRED because we do not support Seed and PgenCounter*/
    if((cur == NULL) || (!xmlSecCheckNodeName(cur,  xmlSecNodeDSAP, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        goto done;
    }
    p = xmlSecGCryptNodeGetMpiValue(cur);
    if(p == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeGetMpiValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    /* next is Q node. It is REQUIRED because we do not support Seed and PgenCounter*/
    if((cur == NULL) || (!xmlSecCheckNodeName(cur, xmlSecNodeDSAQ, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAQ));
        goto done;
    }
    q = xmlSecGCryptNodeGetMpiValue(cur);
    if(q == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeGetMpiValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAQ));
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    /* next is G node. It is REQUIRED because we do not support Seed and PgenCounter*/
    if((cur == NULL) || (!xmlSecCheckNodeName(cur, xmlSecNodeDSAG, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAG));
        goto done;
    }
    g = xmlSecGCryptNodeGetMpiValue(cur);
    if(g == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeGetMpiValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAG));
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeDSAX, xmlSecNs))) {
        /* next is X node. It is REQUIRED for private key but
         * we are not sure exactly what do we read */
        x = xmlSecGCryptNodeGetMpiValue(cur);
        if(x == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGCryptNodeGetMpiValue",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeDSAX));
            goto done;
        }
        cur = xmlSecGetNextElementNode(cur->next);
    }

    /* next is Y node. */
    if((cur == NULL) || (!xmlSecCheckNodeName(cur, xmlSecNodeDSAY, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAY));
        goto done;
    }
    y = xmlSecGCryptNodeGetMpiValue(cur);
    if(y == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeGetMpiValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s", xmlSecErrorsSafeString(xmlSecNodeDSAY));
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    /* todo: add support for J */
    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeDSAJ, xmlSecDSigNs))) {
        cur = xmlSecGetNextElementNode(cur->next);
    }

    /* todo: add support for seed */
    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeDSASeed, xmlSecDSigNs))) {
        cur = xmlSecGetNextElementNode(cur->next);
    }

    /* todo: add support for pgencounter */
    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeDSAPgenCounter, xmlSecDSigNs))) {
        cur = xmlSecGetNextElementNode(cur->next);
    }

    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_UNEXPECTED_NODE,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* Convert from OpenSSL parameter ordering to the OpenPGP order. */
    /* First check that x < y; if not swap x and y  */
    if((x != NULL) && (gcry_mpi_cmp (x, y) > 0)) {
        gcry_mpi_swap (x, y);
    }

    /* construct pub/priv key pairs */
    err = gcry_sexp_build(&pub_key, NULL,
             "(public-key(dsa(p%m)(q%m)(g%m)(y%m)))",
             p, q, g, y);
    if((err != GPG_ERR_NO_ERROR) || (pub_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "gcry_sexp_build(public)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }
    if(x != NULL) {
        err = gcry_sexp_build(&priv_key, NULL,
                 "(private-key(dsa(p%m)(q%m)(g%m)(x%m)(y%m)))",
                 p, q, g, x, y);
        if((err != GPG_ERR_NO_ERROR) || (priv_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "gcry_sexp_build(private)",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }
    }

    /* create key data */
    data = xmlSecKeyDataCreate(id);
    if(data == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xmlSecGCryptKeyDataDsaAdoptKeyPair(data, pub_key, priv_key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGCryptKeyDataDsaAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    pub_key = NULL; /* pub_key is owned by data now */
    priv_key = NULL; /* priv_key is owned by data now */

    /* set key */
    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    data = NULL; /* data is owned by key now */

    /* success */
    res = 0;

done:
    /* cleanup */
    if(p != NULL) {
        gcry_mpi_release(p);
    }

    if(q != NULL) {
        gcry_mpi_release(q);
    }

    if(g != NULL) {
        gcry_mpi_release(g);
    }

    if(x != NULL) {
        gcry_mpi_release(x);
    }

    if(y != NULL) {
        gcry_mpi_release(y);
    }

    if(pub_key != NULL) {
        gcry_sexp_release(pub_key);
    }

    if(priv_key != NULL) {
        gcry_sexp_release(priv_key);
    }

    if(data != NULL) {
        xmlSecKeyDataDestroy(data);
    }
    return(res);
}

static int
xmlSecGCryptKeyDataDsaXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlNodePtr cur;
    gcry_sexp_t pub_priv_key;
    gcry_sexp_t dsa = NULL;
    int private = 0;
    int res = -1;
    int ret;

    xmlSecAssert2(id == xmlSecGCryptKeyDataDsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecGCryptKeyDataDsaId), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if(((xmlSecKeyDataTypePublic | xmlSecKeyDataTypePrivate) & keyInfoCtx->keyReq.keyType) == 0) {
        /* we can have only private key or public key */
        return(0);
    }

    /* find the private or public key */
    pub_priv_key = xmlSecGCryptKeyDataDsaGetPrivateKey(xmlSecKeyGetValue(key));
    if(pub_priv_key == NULL) {
        pub_priv_key = xmlSecGCryptKeyDataDsaGetPublicKey(xmlSecKeyGetValue(key));
        if(pub_priv_key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGCryptKeyDataDsaGetPublicKey()",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }
    } else {
        private = 1;
    }

    dsa = gcry_sexp_find_token(pub_priv_key, "dsa", 0);
    if(dsa == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "gcry_sexp_find_token(dsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* first is P node */
    cur = xmlSecAddChild(node, xmlSecNodeDSAP, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        goto done;
    }
    ret = xmlSecGCryptNodeSetSExpTokValue(cur, dsa, "p", 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeSetSExpTokValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        goto done;
    }

    /* next is Q node. */
    cur = xmlSecAddChild(node, xmlSecNodeDSAQ, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAQ));
        goto done;
    }
    ret = xmlSecGCryptNodeSetSExpTokValue(cur, dsa, "q", 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeSetSExpTokValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAQ));
        goto done;
    }

    /* next is G node. */
    cur = xmlSecAddChild(node, xmlSecNodeDSAG, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAG));
        goto done;
    }
    ret = xmlSecGCryptNodeSetSExpTokValue(cur, dsa, "g", 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeSetSExpTokValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAG));
        goto done;
    }

    /* next is X node: write it ONLY for private keys and ONLY if it is requested */
    if(((keyInfoCtx->keyReq.keyType & xmlSecKeyDataTypePrivate) != 0) && (private != 0)) {
        cur = xmlSecAddChild(node, xmlSecNodeDSAX, xmlSecNs);
        if(cur == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeDSAX));
            goto done;
        }
        ret = xmlSecGCryptNodeSetSExpTokValue(cur, dsa, "x", 1);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGCryptNodeSetSExpTokValue",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeDSAX));
            goto done;
        }
    }

    /* next is Y node. */
    cur = xmlSecAddChild(node, xmlSecNodeDSAY, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAY));
        goto done;
    }
    ret = xmlSecGCryptNodeSetSExpTokValue(cur, dsa, "y", 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeSetSExpTokValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAY));
        goto done;
    }

    /* success */
    res = 0;

done:
    if(dsa != NULL) {
        gcry_sexp_release(dsa);
    }

    return(res);
}

#endif /* XMLSEC_NO_DSA */


#ifndef XMLSEC_NO_RSA
/**************************************************************************
 *
 * <dsig:RSAKeyValue> processing
 *
 * http://www.w3.org/TR/xmldsig-core/#sec-RSAKeyValue
 * The RSAKeyValue Element
 *
 * RSA key values have two fields: Modulus and Exponent.
 *
 * <RSAKeyValue>
 *   <Modulus>xA7SEU+e0yQH5rm9kbCDN9o3aPIo7HbP7tX6WOocLZAtNfyxSZDU16ksL6W
 *     jubafOqNEpcwR3RdFsT7bCqnXPBe5ELh5u4VEy19MzxkXRgrMvavzyBpVRgBUwUlV
 *        5foK5hhmbktQhyNdy/6LpQRhDUDsTvK+g9Ucj47es9AQJ3U=
 *   </Modulus>
 *   <Exponent>AQAB</Exponent>
 * </RSAKeyValue>
 *
 * Arbitrary-length integers (e.g. "bignums" such as RSA moduli) are
 * represented in XML as octet strings as defined by the ds:CryptoBinary type.
 *
 * Schema Definition:
 *
 * <element name="RSAKeyValue" type="ds:RSAKeyValueType"/>
 * <complexType name="RSAKeyValueType">
 *   <sequence>
 *     <element name="Modulus" type="ds:CryptoBinary"/>
 *     <element name="Exponent" type="ds:CryptoBinary"/>
 *   </sequence>
 * </complexType>
 *
 * DTD Definition:
 *
 * <!ELEMENT RSAKeyValue (Modulus, Exponent) >
 * <!ELEMENT Modulus (#PCDATA) >
 * <!ELEMENT Exponent (#PCDATA) >
 *
 * ============================================================================
 *
 * To support reading/writing private keys an PrivateExponent element is added
 * to the end
 *
 *************************************************************************/

static int              xmlSecGCryptKeyDataRsaInitialize       (xmlSecKeyDataPtr data);
static int              xmlSecGCryptKeyDataRsaDuplicate        (xmlSecKeyDataPtr dst,
                                                                 xmlSecKeyDataPtr src);
static void             xmlSecGCryptKeyDataRsaFinalize         (xmlSecKeyDataPtr data);
static int              xmlSecGCryptKeyDataRsaXmlRead          (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGCryptKeyDataRsaXmlWrite         (xmlSecKeyDataId id,
                                                                 xmlSecKeyPtr key,
                                                                 xmlNodePtr node,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecGCryptKeyDataRsaGenerate         (xmlSecKeyDataPtr data,
                                                                 xmlSecSize sizeBits,
                                                                 xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecGCryptKeyDataRsaGetType         (xmlSecKeyDataPtr data);
static xmlSecSize               xmlSecGCryptKeyDataRsaGetSize          (xmlSecKeyDataPtr data);
static void             xmlSecGCryptKeyDataRsaDebugDump        (xmlSecKeyDataPtr data,
                                                                 FILE* output);
static void             xmlSecGCryptKeyDataRsaDebugXmlDump     (xmlSecKeyDataPtr data,
                                                                 FILE* output);
static xmlSecKeyDataKlass xmlSecGCryptKeyDataRsaKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecGCryptAsymKeyDataSize,

    /* data */
    xmlSecNameRSAKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                                /* xmlSecKeyDataUsage usage; */
    xmlSecHrefRSAKeyValue,                      /* const xmlChar* href; */
    xmlSecNodeRSAKeyValue,                      /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                               /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecGCryptKeyDataRsaInitialize,          /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecGCryptKeyDataRsaDuplicate,           /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecGCryptKeyDataRsaFinalize,            /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecGCryptKeyDataRsaGenerate,            /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecGCryptKeyDataRsaGetType,             /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecGCryptKeyDataRsaGetSize,             /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                                       /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecGCryptKeyDataRsaXmlRead,             /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecGCryptKeyDataRsaXmlWrite,            /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    NULL,                                       /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                                       /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecGCryptKeyDataRsaDebugDump,           /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecGCryptKeyDataRsaDebugXmlDump,        /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecGCryptKeyDataRsaGetKlass:
 *
 * The GCrypt RSA key data klass.
 *
 * Returns: pointer to GCrypt RSA key data klass.
 */
xmlSecKeyDataId
xmlSecGCryptKeyDataRsaGetKlass(void) {
    return(&xmlSecGCryptKeyDataRsaKlass);
}

/**
 * xmlSecGCryptKeyDataRsaAdoptKey:
 * @data:               the pointer to RSA key data.
 * @rsa_key:            the pointer to GCrypt RSA key.
 *
 * Sets the value of RSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptKeyDataRsaAdoptKey(xmlSecKeyDataPtr data, gcry_sexp_t rsa_key) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), -1);
    xmlSecAssert2(rsa_key != NULL, -1);

    return xmlSecGCryptAsymKeyDataAdoptKey(data, rsa_key);
}


/**
 * xmlSecGCryptKeyDataRsaAdoptKeyPair:
 * @data:               the pointer to RSA key data.
 * @pub_key:            the pointer to GCrypt RSA pub key.
 * @priv_key:           the pointer to GCrypt RSA priv key.
 *
 * Sets the value of RSA key data.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptKeyDataRsaAdoptKeyPair(xmlSecKeyDataPtr data, gcry_sexp_t pub_key, gcry_sexp_t priv_key) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), -1);
    xmlSecAssert2(pub_key != NULL, -1);

    return xmlSecGCryptAsymKeyDataAdoptKeyPair(data, pub_key, priv_key);
}

/**
 * xmlSecGCryptKeyDataRsaGetPublicKey:
 * @data:               the pointer to RSA key data.
 *
 * Gets the GCrypt RSA public key from RSA key data.
 *
 * Returns: pointer to GCrypt public RSA key or NULL if an error occurs.
 */
gcry_sexp_t
xmlSecGCryptKeyDataRsaGetPublicKey(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), NULL);
    return xmlSecGCryptAsymKeyDataGetPublicKey(data);
}

/**
 * xmlSecGCryptKeyDataRsaGetPrivateKey:
 * @data:               the pointer to RSA key data.
 *
 * Gets the GCrypt RSA private key from RSA key data.
 *
 * Returns: pointer to GCrypt private RSA key or NULL if an error occurs.
 */
gcry_sexp_t
xmlSecGCryptKeyDataRsaGetPrivateKey(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), NULL);
    return xmlSecGCryptAsymKeyDataGetPrivateKey(data);
}

static int
xmlSecGCryptKeyDataRsaInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), -1);

    return(xmlSecGCryptAsymKeyDataInitialize(data));
}

static int
xmlSecGCryptKeyDataRsaDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecKeyDataCheckId(dst, xmlSecGCryptKeyDataRsaId), -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(src, xmlSecGCryptKeyDataRsaId), -1);

    return(xmlSecGCryptAsymKeyDataDuplicate(dst, src));
}

static void
xmlSecGCryptKeyDataRsaFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId));

    xmlSecGCryptAsymKeyDataFinalize(data);
}

static int
xmlSecGCryptKeyDataRsaGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    return xmlSecGCryptAsymKeyDataGenerate(data, "rsa", sizeBits);
}

static xmlSecKeyDataType
xmlSecGCryptKeyDataRsaGetType(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), xmlSecKeyDataTypeUnknown);

    return xmlSecGCryptAsymKeyDataGetType(data);
}

static xmlSecSize
xmlSecGCryptKeyDataRsaGetSize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId), 0);

    return xmlSecGCryptAsymKeyDataGetSize(data);
}

static void
xmlSecGCryptKeyDataRsaDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "=== rsa key: size = %d\n",
            xmlSecGCryptKeyDataRsaGetSize(data));
}

static void
xmlSecGCryptKeyDataRsaDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecGCryptKeyDataRsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "<RSAKeyValue size=\"%d\" />\n",
            xmlSecGCryptKeyDataRsaGetSize(data));
}

static int
xmlSecGCryptKeyDataRsaXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                    xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlNodePtr cur;
    xmlSecKeyDataPtr data = NULL;
    gcry_mpi_t n = NULL;
    gcry_mpi_t e = NULL;
    gcry_mpi_t d = NULL;
    gcry_sexp_t pub_key = NULL;
    gcry_sexp_t priv_key = NULL;
    gcry_error_t err;
    int res = -1;
    int ret;

    xmlSecAssert2(id == xmlSecGCryptKeyDataRsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if(xmlSecKeyGetValue(key) != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA,
                    "key already has a value");
        goto done;
    }

    cur = xmlSecGetNextElementNode(node->children);

    /* first is Modulus node. It is REQUIRED */
    if((cur == NULL) || (!xmlSecCheckNodeName(cur,  xmlSecNodeRSAModulus, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
        goto done;
    }
    n = xmlSecGCryptNodeGetMpiValue(cur);
    if(n == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeGetMpiValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    /* next is Exponent node. It is REQUIRED */
    if((cur == NULL) || (!xmlSecCheckNodeName(cur, xmlSecNodeRSAExponent, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
        goto done;
    }
    e = xmlSecGCryptNodeGetMpiValue(cur);
    if(e == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeGetMpiValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeRSAPrivateExponent, xmlSecNs))) {
        /* next is PrivateExponent node. It is REQUIRED for private key */
        d = xmlSecGCryptNodeGetMpiValue(cur);
        if(d == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGCryptNodeGetMpiValue",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeRSAPrivateExponent));
            goto done;
        }
        cur = xmlSecGetNextElementNode(cur->next);
    }

    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "no nodes expected");
        goto done;
    }

    /* construct pub/priv key pairs */
    err = gcry_sexp_build(&pub_key, NULL,
             "(public-key(rsa(n%m)(e%m)))",
             n, e);
    if((err != GPG_ERR_NO_ERROR) || (pub_key == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "gcry_sexp_build(public)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_GCRYPT_REPORT_ERROR(err));
        goto done;
    }
    if(d != NULL) {
        err = gcry_sexp_build(&priv_key, NULL,
                 "(private-key(rsa(n%m)(e%m)(d%m)))",
                 n, e, d);
        if((err != GPG_ERR_NO_ERROR) || (priv_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                        "gcry_sexp_build(private)",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }
    }


    /* create key data */
    data = xmlSecKeyDataCreate(id);
    if(data == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = xmlSecGCryptKeyDataRsaAdoptKeyPair(data, pub_key, priv_key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecGCryptKeyDataRsaAdoptKeyPair",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    pub_key = NULL; /* pub_key is owned by data now */
    priv_key = NULL; /* priv_key is owned by data now */

    /* set key */
    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    data = NULL; /* data is owned by key now */


    /* success */
    res = 0;

done:
    /* cleanup */
    if(n != NULL) {
        gcry_mpi_release(n);
    }

    if(e != NULL) {
        gcry_mpi_release(e);
    }

    if(d != NULL) {
        gcry_mpi_release(d);
    }

    if(pub_key != NULL) {
        gcry_sexp_release(pub_key);
    }

    if(priv_key != NULL) {
        gcry_sexp_release(priv_key);
    }

    if(data != NULL) {
        xmlSecKeyDataDestroy(data);
    }
    return(res);

}

static int
xmlSecGCryptKeyDataRsaXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                            xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlNodePtr cur;
    gcry_sexp_t pub_priv_key;
    gcry_sexp_t rsa = NULL;
    int private = 0;
    int res = -1;
    int ret;

    xmlSecAssert2(id == xmlSecGCryptKeyDataRsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecGCryptKeyDataRsaId), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if(((xmlSecKeyDataTypePublic | xmlSecKeyDataTypePrivate) & keyInfoCtx->keyReq.keyType) == 0) {
        /* we can have only private key or public key */
        return(0);
    }

    /* find the private or public key */
    pub_priv_key = xmlSecGCryptKeyDataRsaGetPrivateKey(xmlSecKeyGetValue(key));
    if(pub_priv_key == NULL) {
        pub_priv_key = xmlSecGCryptKeyDataRsaGetPublicKey(xmlSecKeyGetValue(key));
        if(pub_priv_key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGCryptKeyDataRsaGetPublicKey()",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }
    } else {
        private = 1;
    }

    rsa = gcry_sexp_find_token(pub_priv_key, "rsa", 0);
    if(rsa == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "gcry_sexp_find_token(rsa)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    /* first is Modulus node */
    cur = xmlSecAddChild(node, xmlSecNodeRSAModulus, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
       goto done;
    }
    ret = xmlSecGCryptNodeSetSExpTokValue(cur, rsa, "n", 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeSetSExpTokValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
        goto done;
    }

    /* next is Exponent node. */
    cur = xmlSecAddChild(node, xmlSecNodeRSAExponent, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
       goto done;
    }
    ret = xmlSecGCryptNodeSetSExpTokValue(cur, rsa, "e", 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecGCryptNodeSetSExpTokValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
       goto done;
    }

    /* next is PrivateExponent node: write it ONLY for private keys and ONLY if it is requested */
    if(((keyInfoCtx->keyReq.keyType & xmlSecKeyDataTypePrivate) != 0) && (private != 0)) {
        cur = xmlSecAddChild(node, xmlSecNodeRSAPrivateExponent, xmlSecNs);
        if(cur == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeRSAPrivateExponent));
           goto done;
        }
        ret = xmlSecGCryptNodeSetSExpTokValue(cur, rsa, "d", 1);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                        "xmlSecGCryptNodeSetSExpTokValue",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeRSAPrivateExponent));
           goto done;
        }
    }

    /* success */
    res = 0;

done:
    if(rsa != NULL) {
        gcry_sexp_release(rsa);
    }

    return(res);
}

#endif /* XMLSEC_NO_RSA */

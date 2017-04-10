/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <string.h>

#include <gcrypt.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/crypto.h>

#include "asn1.h"

/**************************************************************************
 *
 * ASN.1 parser is taken from GCrypt tests
 *
 *************************************************************************/

/* ASN.1 classes.  */
enum
{
  UNIVERSAL = 0,
  APPLICATION = 1,
  ASNCONTEXT = 2,
  PRIVATE = 3
};


/* ASN.1 tags.  */
enum
{
  TAG_NONE = 0,
  TAG_BOOLEAN = 1,
  TAG_INTEGER = 2,
  TAG_BIT_STRING = 3,
  TAG_OCTET_STRING = 4,
  TAG_NULL = 5,
  TAG_OBJECT_ID = 6,
  TAG_OBJECT_DESCRIPTOR = 7,
  TAG_EXTERNAL = 8,
  TAG_REAL = 9,
  TAG_ENUMERATED = 10,
  TAG_EMBEDDED_PDV = 11,
  TAG_UTF8_STRING = 12,
  TAG_REALTIVE_OID = 13,
  TAG_SEQUENCE = 16,
  TAG_SET = 17,
  TAG_NUMERIC_STRING = 18,
  TAG_PRINTABLE_STRING = 19,
  TAG_TELETEX_STRING = 20,
  TAG_VIDEOTEX_STRING = 21,
  TAG_IA5_STRING = 22,
  TAG_UTC_TIME = 23,
  TAG_GENERALIZED_TIME = 24,
  TAG_GRAPHIC_STRING = 25,
  TAG_VISIBLE_STRING = 26,
  TAG_GENERAL_STRING = 27,
  TAG_UNIVERSAL_STRING = 28,
  TAG_CHARACTER_STRING = 29,
  TAG_BMP_STRING = 30
};

/* ASN.1 Parser object.  */
struct tag_info
{
  int class;             /* Object class.  */
  unsigned long tag;     /* The tag of the object.  */
  unsigned long length;  /* Length of the values.  */
  int nhdr;              /* Length of the header (TL).  */
  unsigned int ndef:1;   /* The object has an indefinite length.  */
  unsigned int cons:1;   /* This is a constructed object.  */
};

/* Parse the buffer at the address BUFFER which consists of the number
   of octets as stored at BUFLEN.  Return the tag and the length part
   from the TLV triplet.  Update BUFFER and BUFLEN on success.  Checks
   that the encoded length does not exhaust the length of the provided
   buffer. */
static int
xmlSecGCryptAsn1ParseTag (xmlSecByte const **buffer, xmlSecSize *buflen, struct tag_info *ti)
{
    int c;
    unsigned long tag;
    const xmlSecByte *buf;
    xmlSecSize length;

    xmlSecAssert2(buffer != NULL, -1);
    xmlSecAssert2((*buffer) != NULL, -1);
    xmlSecAssert2(buflen != NULL, -1);
    xmlSecAssert2(ti != NULL, -1);

    /* initialize */
    buf = *buffer;
    length = *buflen;

    ti->length = 0;
    ti->ndef = 0;
    ti->nhdr = 0;

    /* Get the tag */
    if (length <= 0) {
        return(-1); /* Premature EOF.  */
    }
    c = *buf++; 
    length--;
    ti->nhdr++;

    ti->class = (c & 0xc0) >> 6;
    ti->cons  = !!(c & 0x20);
    tag       = (c & 0x1f);

    if (tag == 0x1f) {
        tag = 0;
        do {
            tag <<= 7;
            if (length <= 0) {
                return(-1); /* Premature EOF.  */
            }
            c = *buf++; 
            length--;
            ti->nhdr++;
            tag |= (c & 0x7f);
        } while ( (c & 0x80) );
    }
    ti->tag = tag;

    /* Get the length */
    if(length <= 0) {
        return -1; /* Premature EOF. */
    }
    c = *buf++; 
    length--;
    ti->nhdr++;

    if ( !(c & 0x80) ) {
        ti->length = c;
    } else if (c == 0x80) {
        ti->ndef = 1;
    } else if (c == 0xff) {
        return -1; /* Forbidden length value.  */
    } else {
        xmlSecSize len = 0;
        int count = c & 0x7f;

        for (; count; count--) {
            len <<= 8;
            if (length <= 0) {
                return -1; /* Premature EOF.  */
            }
            c = *buf++; length--;
            ti->nhdr++;
            len |= (c & 0xff);
        }
        ti->length = len;
    }

    if (ti->class == UNIVERSAL && !ti->tag) {
        ti->length = 0;
    }

    if (ti->length > length) {
        return(-1); /* Data larger than buffer.  */
    }

    /* done */
    *buffer = buf;
    *buflen = length;
    return(0);
}

static int
xmlSecGCryptAsn1ParseIntegerSequence(xmlSecByte const **buffer, xmlSecSize *buflen,
                                     gcry_mpi_t * params, int params_size) {
    const xmlSecByte *buf;
    xmlSecSize length;
    struct tag_info ti;
    gcry_error_t err;
    int idx = 0;
    int ret;

    xmlSecAssert2(buffer != NULL, -1);
    xmlSecAssert2((*buffer) != NULL, -1);
    xmlSecAssert2(buflen != NULL, -1);
    xmlSecAssert2(params != NULL, -1);
    xmlSecAssert2(params_size > 0, -1);

    /* initialize */
    buf = *buffer;
    length = *buflen;

    /* read SEQUENCE */
    memset(&ti, 0, sizeof(ti));
    ret = xmlSecGCryptAsn1ParseTag (&buf, &length, &ti);
    if((ret != 0)  || (ti.tag != TAG_SEQUENCE) || ti.class || !ti.cons || ti.ndef) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAsn1ParseTag",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "TAG_SEQUENCE is expected: tag=%d",
                    (int)ti.tag);
        return(-1);
    }

    /* read INTEGERs */
    for (idx = 0; ((idx < params_size) && (length > 0)); idx++) {
        memset(&ti, 0, sizeof(ti));
        ret = xmlSecGCryptAsn1ParseTag (&buf, &length, &ti);
        if((ret != 0) || (ti.tag != TAG_INTEGER) || ti.class || ti.cons || ti.ndef)
        {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGCryptAsn1ParseTag",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "TAG_INTEGER is expected - index=%d, tag=%d",
                        (int)idx, (int)ti.tag);
            return(-1);
        }

        err = gcry_mpi_scan(&(params[idx]), GCRYMPI_FMT_USG, buf, ti.length, NULL);
        if((err != GPG_ERR_NO_ERROR) || (params[idx] == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_mpi_scan",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            return(-1);
        }
        buf += ti.length;
        length -= ti.length;
    }

    /* did we parse everything? */
    if(length > 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAsn1ParseTag",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "too many params - cur=%d, expected=%d",
                    (int)(idx - 1), (int)params_size);
        return(-1);
    }

    /* done */
    *buffer = buf;
    *buflen = length;
    return(idx);
}

xmlSecKeyDataPtr
xmlSecGCryptParseDer(const xmlSecByte * der, xmlSecSize derlen,
                     enum xmlSecGCryptDerKeyType type) {
    xmlSecKeyDataPtr key_data = NULL;
    gcry_sexp_t s_pub_key = NULL;
    gcry_sexp_t s_priv_key = NULL;
    gcry_error_t err;
    gcry_mpi_t keyparms[20];
    int keyparms_num;
    unsigned int idx;
    int ret;

    xmlSecAssert2(der != NULL, NULL);
    xmlSecAssert2(derlen > 0, NULL);

    /* Parse the ASN.1 structure.  */
    memset(&keyparms, 0, sizeof(keyparms));
    ret = xmlSecGCryptAsn1ParseIntegerSequence(
        &der, &derlen,
        keyparms,  sizeof(keyparms) / sizeof(keyparms[0])
    );
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAsn1ParseIntegerSequence",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    keyparms_num = ret;

    /* The value of the first integer should be 0. */
    if ((keyparms_num < 1) || (gcry_mpi_cmp_ui(keyparms[0], 0) != 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAsn1ParseTag",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "num=%d",
                    (int)keyparms_num);
        goto done;
    }

    /* do we need to guess the key type? not robust but the best we can do */
    if(type == xmlSecGCryptDerKeyTypeAuto) {
        switch(keyparms_num) {
        case 3:
            /* Public RSA */
            type = xmlSecGCryptDerKeyTypePublicRsa;
            break;
        case 5:
            /* Public DSA */
            type = xmlSecGCryptDerKeyTypePublicDsa;
            break;
        case 6:
            /* Private DSA */
            type = xmlSecGCryptDerKeyTypePrivateDsa;
            break;
        case 9:
            /* Private RSA */
            type = xmlSecGCryptDerKeyTypePrivateRsa;
            break;
        default:
            /* unknown */
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "Unexpected number of parameters, unknown key type",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "keyparms_num=%d", (int)keyparms_num);
            goto done;
        }
    }


    switch(type) {
#ifndef XMLSEC_NO_DSA
    case xmlSecGCryptDerKeyTypePrivateDsa:
        /* check we have enough params */
        if(keyparms_num != 6) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "Private DSA key: 6 parameters exepcted",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "parms_num=%d", (int)keyparms_num);
            goto done;
        }

        /* Convert from OpenSSL parameter ordering to the OpenPGP order. */
        /* First check that x < y; if not swap x and y  */
        if (gcry_mpi_cmp (keyparms[4], keyparms[5]) > 0) {
            gcry_mpi_swap (keyparms[4], keyparms[5]);
        }

        /* Build the S-expressions  */
        err = gcry_sexp_build (&s_priv_key, NULL,
                "(private-key(dsa(p%m)(q%m)(g%m)(x%m)(y%m)))",
                keyparms[1], keyparms[2], keyparms[3], keyparms[4], keyparms[5]
        );
        if((err != GPG_ERR_NO_ERROR) || (s_priv_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_sexp_build(private-key/dsa)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }

        err = gcry_sexp_build (&s_pub_key, NULL,
                "(public-key(dsa(p%m)(q%m)(g%m)(y%m)))",
                keyparms[1], keyparms[2], keyparms[3], keyparms[5]
        );
        if((err != GPG_ERR_NO_ERROR) || (s_pub_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_sexp_build(public-key/dsa)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }

        /* construct key and key data */
        key_data = xmlSecKeyDataCreate(xmlSecGCryptKeyDataDsaId);
        if(key_data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataDsaId");
            goto done;
        }

        ret = xmlSecGCryptKeyDataDsaAdoptKeyPair(key_data, s_pub_key, s_priv_key);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGCryptKeyDataDsaAdoptKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataDsaId");
            xmlSecKeyDataDestroy(key_data);
            key_data = NULL;
            goto done;
        }
        s_pub_key = NULL; /* owned by key_data now */
        s_priv_key = NULL; /* owned by key_data now */
        break;

    case xmlSecGCryptDerKeyTypePublicDsa:
        /* check we have enough params */
        if(keyparms_num != 5) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "Public DSA key: 5 parameters exepcted",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "parms_num=%d", (int)keyparms_num);
            goto done;
        }

        /* Build the S-expression.  */
        err = gcry_sexp_build (&s_pub_key, NULL,
                "(public-key(dsa(p%m)(q%m)(g%m)(y%m)))",
                keyparms[2], keyparms[3], keyparms[4], keyparms[1]
        );
        if((err != GPG_ERR_NO_ERROR) || (s_pub_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_sexp_build(public-key/dsa)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }

        /* construct key and key data */
        key_data = xmlSecKeyDataCreate(xmlSecGCryptKeyDataDsaId);
        if(key_data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataDsaId");
            goto done;
        }

        ret = xmlSecGCryptKeyDataDsaAdoptKeyPair(key_data, s_pub_key, NULL);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGCryptKeyDataDsaAdoptKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataDsaId");
            xmlSecKeyDataDestroy(key_data);
            key_data = NULL;
            goto done;
        }
        s_pub_key = NULL; /* owned by key_data now */
        break;
#endif /* XMLSEC_NO_DSA */

#ifndef XMLSEC_NO_RSA
    case xmlSecGCryptDerKeyTypePrivateRsa:
        /* check we have enough params */
        if(keyparms_num != 9) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "Private RSA key: 9 parameters exepcted",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "parms_num=%d", (int)keyparms_num);
            goto done;
        }

        /* Convert from OpenSSL parameter ordering to the OpenPGP order. */
        /* (http://gnupg.10057.n7.nabble.com/RSA-PKCS-1-signing-differs-from-OpenSSL-s-td27920.html) */
        /* First check that p < q; if not swap p and q and recompute u.  */ 
        if (gcry_mpi_cmp (keyparms[4], keyparms[5]) > 0) {
            gcry_mpi_swap (keyparms[4], keyparms[5]);
            gcry_mpi_invm (keyparms[8], keyparms[4], keyparms[5]);
        }

        /* Build the S-expression.  */
        err = gcry_sexp_build (&s_priv_key, NULL,
                         "(private-key(rsa(n%m)(e%m)(d%m)(p%m)(q%m)(u%m)))",
                         keyparms[1], keyparms[2], keyparms[3],
                         keyparms[4], keyparms[5], keyparms[8]
        );
        if((err != GPG_ERR_NO_ERROR) || (s_priv_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_sexp_build(private-key/rsa)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }

        err = gcry_sexp_build (&s_pub_key, NULL,
                         "(public-key(rsa(n%m)(e%m)))",
                         keyparms[1], keyparms[2]
        );
        if((err != GPG_ERR_NO_ERROR) || (s_pub_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_sexp_build(public-key/rsa)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }

        /* construct key and key data */
        key_data = xmlSecKeyDataCreate(xmlSecGCryptKeyDataRsaId);
        if(key_data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataRsaId");
            goto done;
        }

        ret = xmlSecGCryptKeyDataRsaAdoptKeyPair(key_data, s_pub_key, s_priv_key);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGCryptKeyDataRsaAdoptKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataRsaId");
            xmlSecKeyDataDestroy(key_data);
            key_data = NULL;
            goto done;
        }
        s_pub_key = NULL; /* owned by key_data now */
        s_priv_key = NULL; /* owned by key_data now */
        break;

    case xmlSecGCryptDerKeyTypePublicRsa:
        /* check we have enough params */
        if(keyparms_num != 3) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "Public RSA key: 3 parameters exepcted",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "parms_num=%d", (int)keyparms_num);
            goto done;
        }

        /* Build the S-expression.  */
        err = gcry_sexp_build (&s_pub_key, NULL,
                         "(public-key(rsa(n%m)(e%m)))",
                         keyparms[1], keyparms[2]
        );
        if((err != GPG_ERR_NO_ERROR) || (s_pub_key == NULL)) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "gcry_sexp_build(public-key/rsa)",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        XMLSEC_GCRYPT_REPORT_ERROR(err));
            goto done;
        }

        /* construct key and key data */
        key_data = xmlSecKeyDataCreate(xmlSecGCryptKeyDataRsaId);
        if(key_data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataRsaId");
            goto done;
        }

        ret = xmlSecGCryptKeyDataRsaAdoptKeyPair(key_data, s_pub_key, NULL);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGCryptKeyDataRsaAdoptKey",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecGCryptKeyDataRsaId");
            xmlSecKeyDataDestroy(key_data);
            key_data = NULL;
            goto done;
        }
        s_pub_key = NULL; /* owned by key_data now */
        break;
#endif /* XMLSEC_NO_RSA */

    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "Unsupported key type",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "type=%d", (int)type);
        goto done;
        break;
    }

done:
    if(s_priv_key != NULL) {
        gcry_sexp_release(s_priv_key);
    }
    if(s_pub_key != NULL) {
        gcry_sexp_release(s_pub_key);
    }
    for (idx = 0; idx < sizeof(keyparms) / sizeof(keyparms[0]); idx++) {
        if(keyparms[idx] != NULL) {
            gcry_mpi_release (keyparms[idx]);
        }
    }

    return(key_data);
}

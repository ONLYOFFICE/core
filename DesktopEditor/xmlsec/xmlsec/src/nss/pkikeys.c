/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#include "globals.h"

#include <string.h>

#include <pk11func.h>
#include <keyhi.h>
#include <pk11pqg.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/nss/crypto.h>
#include <xmlsec/nss/bignum.h>
#include <xmlsec/nss/pkikeys.h>

/**************************************************************************
 *
 * Internal NSS PKI key CTX
 *
 *************************************************************************/
typedef struct _xmlSecNssPKIKeyDataCtx  xmlSecNssPKIKeyDataCtx,
                                                *xmlSecNssPKIKeyDataCtxPtr;
struct _xmlSecNssPKIKeyDataCtx {
    SECKEYPublicKey  *pubkey;
    SECKEYPrivateKey *privkey;
};

/******************************************************************************
 *
 * PKI key (dsa/rsa)
 *
 * xmlSecNssPKIKeyDataCtx is located after xmlSecTransform
 *
 *****************************************************************************/
#define xmlSecNssPKIKeyDataSize \
    (sizeof(xmlSecKeyData) + sizeof(xmlSecNssPKIKeyDataCtx))
#define xmlSecNssPKIKeyDataGetCtx(data) \
    ((xmlSecNssPKIKeyDataCtxPtr)(((xmlSecByte*)(data)) + sizeof(xmlSecKeyData)))


static int              xmlSecNssPKIKeyDataInitialize   (xmlSecKeyDataPtr data);
static void             xmlSecNssPKIKeyDataFinalize     (xmlSecKeyDataPtr data);


static void             xmlSecNSSPKIKeyDataCtxFree      (xmlSecNssPKIKeyDataCtxPtr ctx);
static int              xmlSecNSSPKIKeyDataCtxDup       (xmlSecNssPKIKeyDataCtxPtr ctxDst,
                                                         xmlSecNssPKIKeyDataCtxPtr ctxSrc);
static int              xmlSecNssPKIKeyDataAdoptKey     (xmlSecKeyDataPtr data,
                                                         SECKEYPrivateKey *privkey,
                                                         SECKEYPublicKey  *pubkey);


static int
xmlSecNssPKIKeyDataInitialize(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecNssPKIKeyDataSize), -1);

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    memset(ctx, 0, sizeof(xmlSecNssPKIKeyDataCtx));

    return(0);
}


static void
xmlSecNssPKIKeyDataFinalize(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;

    xmlSecAssert(xmlSecKeyDataIsValid(data));
    xmlSecAssert(xmlSecKeyDataCheckSize(data, xmlSecNssPKIKeyDataSize));

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert(ctx != NULL);

    xmlSecNSSPKIKeyDataCtxFree(ctx);
    memset(ctx, 0, sizeof(xmlSecNssPKIKeyDataCtx));
}


static void
xmlSecNSSPKIKeyDataCtxFree(xmlSecNssPKIKeyDataCtxPtr ctx)
{
    xmlSecAssert(ctx != NULL);
    if (ctx->privkey != NULL) {
        SECKEY_DestroyPrivateKey(ctx->privkey);
        ctx->privkey = NULL;
    }

    if (ctx->pubkey)
    {
        SECKEY_DestroyPublicKey(ctx->pubkey);
        ctx->pubkey = NULL;
    }

}

static int
xmlSecNSSPKIKeyDataCtxDup(xmlSecNssPKIKeyDataCtxPtr ctxDst,
                          xmlSecNssPKIKeyDataCtxPtr ctxSrc)
{
    xmlSecNSSPKIKeyDataCtxFree(ctxDst);
    if (ctxSrc->privkey != NULL) {
        ctxDst->privkey = SECKEY_CopyPrivateKey(ctxSrc->privkey);
        if(ctxDst->privkey == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "SECKEY_CopyPrivateKey",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                "error code=%d", PORT_GetError());
            return(-1);
        }
    }

    if (ctxSrc->pubkey != NULL) {
        ctxDst->pubkey = SECKEY_CopyPublicKey(ctxSrc->pubkey);
        if(ctxDst->pubkey == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "SECKEY_CopyPublicKey",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                                "error code=%d", PORT_GetError());
            return(-1);
        }
    }
    return (0);
}

static int
xmlSecNssPKIKeyDataAdoptKey(xmlSecKeyDataPtr data,
                            SECKEYPrivateKey *privkey,
                            SECKEYPublicKey  *pubkey)
{
    xmlSecNssPKIKeyDataCtxPtr ctx;
        KeyType                                 pubType = nullKey ;
        KeyType                                 priType = nullKey ;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecNssPKIKeyDataSize), -1);

        if( privkey != NULL ) {
                priType = SECKEY_GetPrivateKeyType( privkey ) ;
        }

        if( pubkey != NULL ) {
                pubType = SECKEY_GetPublicKeyType( pubkey ) ;
        }

        if( priType != nullKey && pubType != nullKey ) {
                if( pubType != priType ) {
                        xmlSecError( XMLSEC_ERRORS_HERE ,
                                NULL ,
                                NULL ,
                                XMLSEC_ERRORS_R_CRYPTO_FAILED ,
                                "different type of private and public key" ) ;
                        return -1 ;
                }
        }

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);

    if (ctx->privkey) {
        SECKEY_DestroyPrivateKey(ctx->privkey);
    }
    ctx->privkey = privkey;

    if (ctx->pubkey) {
        SECKEY_DestroyPublicKey(ctx->pubkey);
    }
    ctx->pubkey = pubkey;

    return(0);
}

/**
 * xmlSecNssPKIAdoptKey:
 * @privkey:        the NSS Private Key handle
 * @pubkey:         the NSS Public Key handle
 *
 * Build a KeyData object from the given Private Key and Public
 * Key handles.
 *
 * Returns: pointer to KeyData object or NULL if an error occurs.
 */
xmlSecKeyDataPtr
xmlSecNssPKIAdoptKey(SECKEYPrivateKey *privkey,
                     SECKEYPublicKey  *pubkey)
{
    xmlSecKeyDataPtr data = NULL;
    int ret;
        KeyType                                 pubType = nullKey ;
        KeyType                                 priType = nullKey ;

        if( privkey != NULL ) {
                priType = SECKEY_GetPrivateKeyType( privkey ) ;
        }

        if( pubkey != NULL ) {
                pubType = SECKEY_GetPublicKeyType( pubkey ) ;
        }

        if( priType != nullKey && pubType != nullKey ) {
                if( pubType != priType ) {
                        xmlSecError( XMLSEC_ERRORS_HERE ,
                                NULL ,
                                NULL ,
                                XMLSEC_ERRORS_R_CRYPTO_FAILED ,
                                "different type of private and public key" ) ;
                        return( NULL ) ;
                }
        }

        pubType = priType != nullKey ? priType : pubType ;
    switch(pubType) {
#ifndef XMLSEC_NO_RSA
    case rsaKey:
        data = xmlSecKeyDataCreate(xmlSecNssKeyDataRsaId);
        if(data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecNssKeyDataRsaId");
            return(NULL);
        }
        break;
#endif /* XMLSEC_NO_RSA */
#ifndef XMLSEC_NO_DSA
    case dsaKey:
        data = xmlSecKeyDataCreate(xmlSecNssKeyDataDsaId);
        if(data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecNssKeyDataDsaId");
            return(NULL);
        }
        break;
#endif /* XMLSEC_NO_DSA */
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_TYPE,
                    "PKI key type %d not supported", pubType);
        return(NULL);
    }

    xmlSecAssert2(data != NULL, NULL);
    ret = xmlSecNssPKIKeyDataAdoptKey(data, privkey, pubkey);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecNssPKIKeyDataAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        return(NULL);
    }
    return(data);
}

/**
 * xmlSecNssPKIKeyDataGetPubKey:
 * @data:               the pointer to NSS Key data.
 *
 * Gets the Public Key from the key data.
 *
 * Returns: pointer to SECKEYPublicKey or NULL if an error occurs.
 * Caller is responsible for freeing the key when done
 */
SECKEYPublicKey *
xmlSecNssPKIKeyDataGetPubKey(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;
    SECKEYPublicKey *ret;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecNssPKIKeyDataSize), NULL);

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);
    xmlSecAssert2(ctx->pubkey != NULL, NULL);

    ret = SECKEY_CopyPublicKey(ctx->pubkey);
    return(ret);
}

/**
 * xmlSecNssPKIKeyDataGetPrivKey:
 * @data:               the pointer to NSS Key data.
 *
 * Gets the Private Key from the key data.
 *
 * Returns: pointer to SECKEYPrivateKey or NULL if an error occurs.
 * Caller is responsible for freeing the key when done
 */
SECKEYPrivateKey*
xmlSecNssPKIKeyDataGetPrivKey(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;
    SECKEYPrivateKey* ret;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), NULL);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecNssPKIKeyDataSize), NULL);

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, NULL);
    xmlSecAssert2(ctx->privkey != NULL, NULL);

    ret = SECKEY_CopyPrivateKey(ctx->privkey);
    return(ret);
}

/**
 * xmlSecNssPKIKeyDataGetKeyType:
 * @data:               the pointer to NSS Key data.
 *
 * Gets the Key Type from the key data.
 *
 * Returns: Key Type
 */
KeyType
xmlSecNssPKIKeyDataGetKeyType(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;
    KeyType kt;

    xmlSecAssert2(xmlSecKeyDataIsValid(data), nullKey);
    xmlSecAssert2(xmlSecKeyDataCheckSize(data, xmlSecNssPKIKeyDataSize), nullKey);

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, nullKey);

    if (ctx->pubkey != NULL) {
        kt = SECKEY_GetPublicKeyType(ctx->pubkey);
    } else {
        kt = SECKEY_GetPrivateKeyType(ctx->privkey);
    }
    return(kt);
}

/**
 * xmlSecNssPKIKeyDataDuplicate
 * @dst:               the pointer to NSS Key data to copy to.
 * @src:               the pointer to NSS Key data to copy from.
 *
 * Duplicates the keydata from src to dst
 *
 * Returns: -1 on error, 0 on success
 */
int
xmlSecNssPKIKeyDataDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecNssPKIKeyDataCtxPtr ctxDst;
    xmlSecNssPKIKeyDataCtxPtr ctxSrc;

    xmlSecAssert2(xmlSecKeyDataIsValid(dst), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(dst, xmlSecNssPKIKeyDataSize), -1);
    xmlSecAssert2(xmlSecKeyDataIsValid(src), -1);
    xmlSecAssert2(xmlSecKeyDataCheckSize(src, xmlSecNssPKIKeyDataSize), -1);

    ctxDst = xmlSecNssPKIKeyDataGetCtx(dst);
    xmlSecAssert2(ctxDst != NULL, -1);

    ctxSrc = xmlSecNssPKIKeyDataGetCtx(src);
    xmlSecAssert2(ctxSrc != NULL, -1);

    if (xmlSecNSSPKIKeyDataCtxDup(ctxDst, ctxSrc) != 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                xmlSecErrorsSafeString(xmlSecKeyDataGetName(dst)),
                "xmlSecNssPKIKeydataCtxDup",
                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
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
static int              xmlSecNssKeyDataDsaInitialize   (xmlSecKeyDataPtr data);
static int              xmlSecNssKeyDataDsaDuplicate    (xmlSecKeyDataPtr dst,
                                                         xmlSecKeyDataPtr src);
static void             xmlSecNssKeyDataDsaFinalize     (xmlSecKeyDataPtr data);
static int              xmlSecNssKeyDataDsaXmlRead      (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssKeyDataDsaXmlWrite     (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssKeyDataDsaGenerate     (xmlSecKeyDataPtr data,
                                                         xmlSecSize sizeBits,
                                                         xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecNssKeyDataDsaGetType     (xmlSecKeyDataPtr data);
static xmlSecSize        xmlSecNssKeyDataDsaGetSize     (xmlSecKeyDataPtr data);
static void              xmlSecNssKeyDataDsaDebugDump   (xmlSecKeyDataPtr data,
                                                         FILE* output);
static void             xmlSecNssKeyDataDsaDebugXmlDump (xmlSecKeyDataPtr data,
                                                         FILE* output);

static xmlSecKeyDataKlass xmlSecNssKeyDataDsaKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecNssPKIKeyDataSize,

    /* data */
    xmlSecNameDSAKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                        /* xmlSecKeyDataUsage usage; */
    xmlSecHrefDSAKeyValue,              /* const xmlChar* href; */
    xmlSecNodeDSAKeyValue,              /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                       /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecNssKeyDataDsaInitialize,      /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecNssKeyDataDsaDuplicate,       /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecNssKeyDataDsaFinalize,        /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecNssKeyDataDsaGenerate,        /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecNssKeyDataDsaGetType,         /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecNssKeyDataDsaGetSize,         /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                               /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecNssKeyDataDsaXmlRead,         /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecNssKeyDataDsaXmlWrite,        /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    NULL,                               /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                               /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecNssKeyDataDsaDebugDump,       /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecNssKeyDataDsaDebugXmlDump,    /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                               /* void* reserved0; */
    NULL,                               /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataDsaGetKlass:
 *
 * The DSA key data klass.
 *
 * Returns: pointer to DSA key data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataDsaGetKlass(void) {
    return(&xmlSecNssKeyDataDsaKlass);
}


static int
xmlSecNssKeyDataDsaInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId), -1);

    return(xmlSecNssPKIKeyDataInitialize(data));
}

static int
xmlSecNssKeyDataDsaDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecKeyDataCheckId(dst, xmlSecNssKeyDataDsaId), -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(src, xmlSecNssKeyDataDsaId), -1);

    return(xmlSecNssPKIKeyDataDuplicate(dst, src));
}

static void
xmlSecNssKeyDataDsaFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId));

    xmlSecNssPKIKeyDataFinalize(data);
}

static int
xmlSecNssKeyDataDsaXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                           xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data = NULL;
    xmlNodePtr cur;
    int ret;
    PK11SlotInfo *slot = NULL;
    CK_OBJECT_HANDLE handle;
    SECKEYPublicKey *pubkey=NULL;
    PRArenaPool *arena = NULL;


    xmlSecAssert2(id == xmlSecNssKeyDataDsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if(xmlSecKeyGetValue(key) != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA,
                    XMLSEC_ERRORS_NO_MESSAGE);
        ret = -1;
        goto done;
    }

    slot = PK11_GetBestSlot(CKM_DSA, NULL);
    if(slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PK11_GetBestSlot",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        ret = -1;
        goto done;
    }

    arena = PORT_NewArena(DER_DEFAULT_CHUNKSIZE);
    if(arena == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PORT_NewArena",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        ret = -1;
        goto done;
    }

    pubkey = (SECKEYPublicKey *)PORT_ArenaZAlloc(arena,
                                                 sizeof(SECKEYPublicKey));
    if(pubkey == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PORT_ArenaZAlloc",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        PORT_FreeArena(arena, PR_FALSE);
        ret = -1;
        goto done;
    }
    pubkey->arena = arena;
    pubkey->u.dsa.params.arena = arena;
    pubkey->keyType = dsaKey;

    cur = xmlSecGetNextElementNode(node->children);

    /* first is P node. It is REQUIRED because we do not support Seed and PgenCounter*/
    if((cur == NULL) || (!xmlSecCheckNodeName(cur,  xmlSecNodeDSAP, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        ret = -1;
        goto done;
    }
    if(xmlSecNssNodeGetBigNumValue(arena, cur, &(pubkey->u.dsa.params.prime)) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeGetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        ret = -1;
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
        ret = -1;
        goto done;
    }
    if(xmlSecNssNodeGetBigNumValue(arena, cur, &(pubkey->u.dsa.params.subPrime)) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeGetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAQ));
        ret = -1;
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
        ret = -1;
        goto done;
    }
    if(xmlSecNssNodeGetBigNumValue(arena, cur, &(pubkey->u.dsa.params.base)) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeGetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAG));
        ret = -1;
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeDSAX, xmlSecNs))) {
        /* next is X node. It is REQUIRED for private key but
         * NSS does not support it, we just ignore it */

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
        ret = -1;
        goto done;
    }
    if(xmlSecNssNodeGetBigNumValue(arena, cur, &(pubkey->u.dsa.publicValue)) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeGetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s", xmlSecErrorsSafeString(xmlSecNodeDSAY));
        ret = -1;
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
        ret = -1;
        goto done;
    }

    handle = PK11_ImportPublicKey(slot, pubkey, PR_FALSE);
    if(handle == CK_INVALID_HANDLE) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PK11_ImportPublicKey",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        ret = -1;
        goto done;
    }

    data = xmlSecKeyDataCreate(id);
    if(data == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        ret = -1;
        goto done;
    }

    ret = xmlSecNssPKIKeyDataAdoptKey(data, NULL, pubkey);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssPKIKeyDataAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    pubkey = NULL;

    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }
    data = NULL;

    ret = 0;

done:
    if (slot != NULL) {
        PK11_FreeSlot(slot);
    }
    if (ret != 0) {
        if (pubkey != NULL) {
            SECKEY_DestroyPublicKey(pubkey);
        }
        if (data != NULL) {
            xmlSecKeyDataDestroy(data);
        }
    }
    return(ret);
}

static int
xmlSecNssKeyDataDsaXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                                xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecNssPKIKeyDataCtxPtr ctx;
    xmlNodePtr cur;
    int ret;

    xmlSecAssert2(id == xmlSecNssKeyDataDsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecNssKeyDataDsaId), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    ctx = xmlSecNssPKIKeyDataGetCtx(xmlSecKeyGetValue(key));
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(SECKEY_GetPublicKeyType(ctx->pubkey) == dsaKey, -1);

    if(((xmlSecKeyDataTypePublic | xmlSecKeyDataTypePrivate) & keyInfoCtx->keyReq.keyType) == 0) {
        /* we can have only private key or public key */
        return(0);
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
        return(-1);
    }
    ret = xmlSecNssNodeSetBigNumValue(cur, &(ctx->pubkey->u.dsa.params.prime), 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeSetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAP));
        return(-1);
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
        return(-1);
    }
    ret = xmlSecNssNodeSetBigNumValue(cur, &(ctx->pubkey->u.dsa.params.subPrime), 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeSetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAQ));
        return(-1);
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
        return(-1);
    }
    ret = xmlSecNssNodeSetBigNumValue(cur, &(ctx->pubkey->u.dsa.params.base), 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeSetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAG));
        return(-1);
    }

    /* next is X node: not supported in NSS */

    /* next is Y node. */
    cur = xmlSecAddChild(node, xmlSecNodeDSAY, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAY));
        return(-1);
    }
    ret = xmlSecNssNodeSetBigNumValue(cur, &(ctx->pubkey->u.dsa.publicValue), 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeSetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDSAY));
        return(-1);
    }

    return(0);
}

static int
xmlSecNssKeyDataDsaGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    PQGParams    *pqgParams = NULL;
    PQGVerify    *pqgVerify = NULL;
    SECStatus     rv;
    SECStatus     res;
    PK11SlotInfo *slot = NULL;
    SECKEYPrivateKey *privkey = NULL;
    SECKEYPublicKey  *pubkey = NULL;
    int               ret = -1;
    int               j;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    j = PQG_PBITS_TO_INDEX(sizeBits);
    rv = PK11_PQG_ParamGen(j, &pqgParams, &pqgVerify);
    if (rv != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "PK11_PQG_ParamGen",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "size=%d", sizeBits);
        goto done;
    }

    rv = PK11_PQG_VerifyParams(pqgParams, pqgVerify, &res);
    if (rv != SECSuccess || res != SECSuccess) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "PK11_PQG_VerifyParams",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "size=%d", sizeBits);
        goto done;
    }

    slot = PK11_GetBestSlot(CKM_DSA_KEY_PAIR_GEN, NULL);
    PK11_Authenticate(slot, PR_TRUE, NULL /* default pwd callback */);
    privkey = PK11_GenerateKeyPair(slot, CKM_DSA_KEY_PAIR_GEN, pqgParams,
                                   &pubkey, PR_FALSE, PR_TRUE, NULL);

    if((privkey == NULL) || (pubkey == NULL)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "PK11_GenerateKeyPair",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);

        goto done;
    }

    ret = xmlSecNssPKIKeyDataAdoptKey(data, privkey, pubkey);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssPKIKeyDataAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = 0;

done:
    if (slot != NULL) {
        PK11_FreeSlot(slot);
    }
    if (pqgParams != NULL) {
        PK11_PQG_DestroyParams(pqgParams);
    }
    if (pqgVerify != NULL) {
        PK11_PQG_DestroyVerify(pqgVerify);
    }
    if (ret == 0) {
        return (0);
    }
    if (pubkey != NULL) {
        SECKEY_DestroyPublicKey(pubkey);
    }
    if (privkey != NULL) {
        SECKEY_DestroyPrivateKey(privkey);
    }
    return(-1);
}

static xmlSecKeyDataType
xmlSecNssKeyDataDsaGetType(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId), xmlSecKeyDataTypeUnknown);
    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(SECKEY_GetPublicKeyType(ctx->pubkey) == dsaKey, -1);
    if (ctx->privkey != NULL) {
        return(xmlSecKeyDataTypePrivate | xmlSecKeyDataTypePublic);
    } else {
        return(xmlSecKeyDataTypePublic);
    }

    return(xmlSecKeyDataTypeUnknown);
}

static xmlSecSize
xmlSecNssKeyDataDsaGetSize(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId), 0);
    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(SECKEY_GetPublicKeyType(ctx->pubkey) == dsaKey, -1);

    return(8 * SECKEY_PublicKeyStrength(ctx->pubkey));
}

static void
xmlSecNssKeyDataDsaDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "=== dsa key: size = %d\n",
            xmlSecNssKeyDataDsaGetSize(data));
}

static void
xmlSecNssKeyDataDsaDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataDsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "<DSAKeyValue size=\"%d\" />\n",
            xmlSecNssKeyDataDsaGetSize(data));
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

static int              xmlSecNssKeyDataRsaInitialize   (xmlSecKeyDataPtr data);
static int              xmlSecNssKeyDataRsaDuplicate    (xmlSecKeyDataPtr dst,
                                                         xmlSecKeyDataPtr src);
static void             xmlSecNssKeyDataRsaFinalize     (xmlSecKeyDataPtr data);
static int              xmlSecNssKeyDataRsaXmlRead      (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssKeyDataRsaXmlWrite     (xmlSecKeyDataId id,
                                                         xmlSecKeyPtr key,
                                                         xmlNodePtr node,
                                                         xmlSecKeyInfoCtxPtr keyInfoCtx);
static int              xmlSecNssKeyDataRsaGenerate     (xmlSecKeyDataPtr data,
                                                         xmlSecSize sizeBits,
                                                        xmlSecKeyDataType type);

static xmlSecKeyDataType xmlSecNssKeyDataRsaGetType     (xmlSecKeyDataPtr data);
static xmlSecSize        xmlSecNssKeyDataRsaGetSize     (xmlSecKeyDataPtr data);
static void             xmlSecNssKeyDataRsaDebugDump    (xmlSecKeyDataPtr data,
                                                         FILE* output);
static void             xmlSecNssKeyDataRsaDebugXmlDump (xmlSecKeyDataPtr data,
                                                         FILE* output);

static xmlSecKeyDataKlass xmlSecNssKeyDataRsaKlass = {
    sizeof(xmlSecKeyDataKlass),
    xmlSecNssPKIKeyDataSize,

    /* data */
    xmlSecNameRSAKeyValue,
    xmlSecKeyDataUsageKeyValueNode | xmlSecKeyDataUsageRetrievalMethodNodeXml,
                                        /* xmlSecKeyDataUsage usage; */
    xmlSecHrefRSAKeyValue,              /* const xmlChar* href; */
    xmlSecNodeRSAKeyValue,              /* const xmlChar* dataNodeName; */
    xmlSecDSigNs,                       /* const xmlChar* dataNodeNs; */

    /* constructors/destructor */
    xmlSecNssKeyDataRsaInitialize,      /* xmlSecKeyDataInitializeMethod initialize; */
    xmlSecNssKeyDataRsaDuplicate,       /* xmlSecKeyDataDuplicateMethod duplicate; */
    xmlSecNssKeyDataRsaFinalize,        /* xmlSecKeyDataFinalizeMethod finalize; */
    xmlSecNssKeyDataRsaGenerate,        /* xmlSecKeyDataGenerateMethod generate; */

    /* get info */
    xmlSecNssKeyDataRsaGetType,         /* xmlSecKeyDataGetTypeMethod getType; */
    xmlSecNssKeyDataRsaGetSize,         /* xmlSecKeyDataGetSizeMethod getSize; */
    NULL,                               /* xmlSecKeyDataGetIdentifier getIdentifier; */

    /* read/write */
    xmlSecNssKeyDataRsaXmlRead,         /* xmlSecKeyDataXmlReadMethod xmlRead; */
    xmlSecNssKeyDataRsaXmlWrite,        /* xmlSecKeyDataXmlWriteMethod xmlWrite; */
    NULL,                               /* xmlSecKeyDataBinReadMethod binRead; */
    NULL,                               /* xmlSecKeyDataBinWriteMethod binWrite; */

    /* debug */
    xmlSecNssKeyDataRsaDebugDump,       /* xmlSecKeyDataDebugDumpMethod debugDump; */
    xmlSecNssKeyDataRsaDebugXmlDump,    /* xmlSecKeyDataDebugDumpMethod debugXmlDump; */

    /* reserved for the future */
    NULL,                               /* void* reserved0; */
    NULL,                               /* void* reserved1; */
};

/**
 * xmlSecNssKeyDataRsaGetKlass:
 *
 * The RSA key data klass.
 *
 * Returns: pointer to RSA key data klass.
 */
xmlSecKeyDataId
xmlSecNssKeyDataRsaGetKlass(void) {
    return(&xmlSecNssKeyDataRsaKlass);
}

static int
xmlSecNssKeyDataRsaInitialize(xmlSecKeyDataPtr data) {
    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId), -1);

    return(xmlSecNssPKIKeyDataInitialize(data));
}

static int
xmlSecNssKeyDataRsaDuplicate(xmlSecKeyDataPtr dst, xmlSecKeyDataPtr src) {
    xmlSecAssert2(xmlSecKeyDataCheckId(dst, xmlSecNssKeyDataRsaId), -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(src, xmlSecNssKeyDataRsaId), -1);

    return(xmlSecNssPKIKeyDataDuplicate(dst, src));
}

static void
xmlSecNssKeyDataRsaFinalize(xmlSecKeyDataPtr data) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId));

    xmlSecNssPKIKeyDataFinalize(data);
}

static int
xmlSecNssKeyDataRsaXmlRead(xmlSecKeyDataId id, xmlSecKeyPtr key,
                           xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyDataPtr data = NULL;
    xmlNodePtr cur;
    int ret;
    PK11SlotInfo *slot = NULL;
    SECKEYPublicKey *pubkey=NULL;
    PRArenaPool *arena = NULL;

    xmlSecAssert2(id == xmlSecNssKeyDataRsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    if(xmlSecKeyGetValue(key) != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_KEY_DATA,
                    "key already has a value");
        ret = -1;
        goto done;
    }

    slot = PK11_GetBestSlot(CKM_RSA_PKCS, NULL);
    if(slot == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PK11_GetBestSlot",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        ret = -1;
        goto done;
    }

    arena = PORT_NewArena(DER_DEFAULT_CHUNKSIZE);
    if(arena == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PORT_NewArena",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        ret = -1;
        goto done;
    }

    pubkey = (SECKEYPublicKey *)PORT_ArenaZAlloc(arena,
                                                 sizeof(SECKEYPublicKey));
    if(pubkey == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "PORT_ArenaZAlloc",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());
        PORT_FreeArena(arena, PR_FALSE);
        ret = -1;
        goto done;
    }
    pubkey->arena = arena;
    pubkey->keyType = rsaKey;

    cur = xmlSecGetNextElementNode(node->children);

    /* first is Modulus node. It is REQUIRED because we do not support Seed and PgenCounter*/
    if((cur == NULL) || (!xmlSecCheckNodeName(cur,  xmlSecNodeRSAModulus, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
        ret = -1;
        goto done;
    }
    if(xmlSecNssNodeGetBigNumValue(arena, cur, &(pubkey->u.rsa.modulus)) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeGetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
        ret = -1;
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    /* next is Exponent node. It is REQUIRED because we do not support Seed and PgenCounter*/
    if((cur == NULL) || (!xmlSecCheckNodeName(cur, xmlSecNodeRSAExponent, xmlSecDSigNs))) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
        ret = -1;
        goto done;
    }
    if(xmlSecNssNodeGetBigNumValue(arena, cur, &(pubkey->u.rsa.publicExponent)) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeGetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
        ret = -1;
        goto done;
    }
    cur = xmlSecGetNextElementNode(cur->next);

    if((cur != NULL) && (xmlSecCheckNodeName(cur, xmlSecNodeRSAPrivateExponent, xmlSecNs))) {
        /* next is X node. It is REQUIRED for private key but
         * NSS does not support it. We just ignore it */
        cur = xmlSecGetNextElementNode(cur->next);
    }

    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "no nodes expected");
        ret = -1;
        goto done;
    }

    data = xmlSecKeyDataCreate(id);
    if(data == NULL ) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeyDataCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        ret = -1;
        goto done;
    }

    ret = xmlSecNssPKIKeyDataAdoptKey(data, NULL, pubkey);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssPKIKeyDataAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        goto done;
    }
    pubkey = NULL;

    ret = xmlSecKeySetValue(key, data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(data);
        goto done;
    }
    data = NULL;

    ret = 0;

done:
    if (slot != 0) {
        PK11_FreeSlot(slot);
    }
    if (ret != 0) {
        if (pubkey != 0) {
            SECKEY_DestroyPublicKey(pubkey);
        }
        if (data != 0) {
            xmlSecKeyDataDestroy(data);
        }
    }
    return(ret);
}

static int
xmlSecNssKeyDataRsaXmlWrite(xmlSecKeyDataId id, xmlSecKeyPtr key,
                            xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecNssPKIKeyDataCtxPtr ctx;
    xmlNodePtr cur;
    int ret;

    xmlSecAssert2(id == xmlSecNssKeyDataRsaId, -1);
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(xmlSecKeyDataCheckId(xmlSecKeyGetValue(key), xmlSecNssKeyDataRsaId), -1);
    xmlSecAssert2(node != NULL, -1);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    ctx = xmlSecNssPKIKeyDataGetCtx(xmlSecKeyGetValue(key));
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(SECKEY_GetPublicKeyType(ctx->pubkey) == rsaKey, -1);


    if(((xmlSecKeyDataTypePublic | xmlSecKeyDataTypePrivate) & keyInfoCtx->keyReq.keyType) == 0) {
        /* we can have only private key or public key */
        return(0);
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
        return(-1);
    }
    ret = xmlSecNssNodeSetBigNumValue(cur, &(ctx->pubkey->u.rsa.modulus), 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeSetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAModulus));
        return(-1);
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
        return(-1);
    }
    ret = xmlSecNssNodeSetBigNumValue(cur, &(ctx->pubkey->u.rsa.publicExponent), 1);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(id)),
                    "xmlSecNssNodeSetBigNumValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRSAExponent));
        return(-1);
    }

    /* next is PrivateExponent node: not supported in NSS */

    return(0);
}

static int
xmlSecNssKeyDataRsaGenerate(xmlSecKeyDataPtr data, xmlSecSize sizeBits, xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    PK11RSAGenParams  params;
    PK11SlotInfo *slot = NULL;
    SECKEYPrivateKey *privkey = NULL;
    SECKEYPublicKey  *pubkey = NULL;
    int               ret = -1;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId), -1);
    xmlSecAssert2(sizeBits > 0, -1);

    params.keySizeInBits = sizeBits;
    params.pe = 65537;

    slot = PK11_GetBestSlot(CKM_RSA_PKCS_KEY_PAIR_GEN, NULL);
    PK11_Authenticate(slot, PR_TRUE, NULL /* default pwd callback */);
    privkey = PK11_GenerateKeyPair(slot, CKM_RSA_PKCS_KEY_PAIR_GEN, &params,
                                   &pubkey, PR_FALSE, PR_TRUE, NULL);

    if(privkey == NULL || pubkey == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "PK11_GenerateKeyPair",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "error code=%d", PORT_GetError());

        goto done;
    }

    ret = xmlSecNssPKIKeyDataAdoptKey(data, privkey, pubkey);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)),
                    "xmlSecNssPKIKeyDataAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        goto done;
    }

    ret = 0;

done:
    if (slot != NULL) {
        PK11_FreeSlot(slot);
    }
    if (ret == 0) {
        return (0);
    }

    if (pubkey != NULL) {
        SECKEY_DestroyPublicKey(pubkey);
    }
    if (privkey != NULL) {
        SECKEY_DestroyPrivateKey(privkey);
    }
    return(-1);
}

static xmlSecKeyDataType
xmlSecNssKeyDataRsaGetType(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId), xmlSecKeyDataTypeUnknown);

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(ctx->pubkey == NULL || SECKEY_GetPublicKeyType(ctx->pubkey) == rsaKey, -1);
    if (ctx->privkey != NULL) {
        return(xmlSecKeyDataTypePrivate | xmlSecKeyDataTypePublic);
    } else {
        return(xmlSecKeyDataTypePublic);
    }

    return(xmlSecKeyDataTypeUnknown);
}

static xmlSecSize
xmlSecNssKeyDataRsaGetSize(xmlSecKeyDataPtr data) {
    xmlSecNssPKIKeyDataCtxPtr ctx;

    xmlSecAssert2(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId), 0);

    ctx = xmlSecNssPKIKeyDataGetCtx(data);
    xmlSecAssert2(ctx != NULL, -1);
    xmlSecAssert2(SECKEY_GetPublicKeyType(ctx->pubkey) == rsaKey, -1);

    return(8 * SECKEY_PublicKeyStrength(ctx->pubkey));
}

static void
xmlSecNssKeyDataRsaDebugDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "=== rsa key: size = %d\n",
            xmlSecNssKeyDataRsaGetSize(data));
}

static void
xmlSecNssKeyDataRsaDebugXmlDump(xmlSecKeyDataPtr data, FILE* output) {
    xmlSecAssert(xmlSecKeyDataCheckId(data, xmlSecNssKeyDataRsaId));
    xmlSecAssert(output != NULL);

    fprintf(output, "<RSAKeyValue size=\"%d\" />\n",
            xmlSecNssKeyDataRsaGetSize(data));
}

#endif /* XMLSEC_NO_RSA */




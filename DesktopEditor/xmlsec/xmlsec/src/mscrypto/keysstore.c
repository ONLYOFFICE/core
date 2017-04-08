/**
 * XMLSec library
 *
 * MSCrypto keys store that uses Simple Keys Store under the hood. Uses the
 * MS Certificate store as a backing store for the finding keys, but the
 * MS Certificate store not written to by the keys store.
 * So, if store->findkey is done and the key is not found in the simple
 * keys store, the MS Certificate store is looked up.
 * Thus, the MS Certificate store can be used to pre-load keys and becomes
 * an alternate source of keys for xmlsec
 *
 * This is free software; see Copyright file in the source
 * distribution for precise wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 * Copyright (C) 2003-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/buffer.h>
#include <xmlsec/base64.h>
#include <xmlsec/errors.h>
#include <xmlsec/xmltree.h>

#include <xmlsec/keysmngr.h>

#include <xmlsec/mscrypto/app.h>
#include <xmlsec/mscrypto/crypto.h>
#include <xmlsec/mscrypto/keysstore.h>
#include <xmlsec/mscrypto/x509.h>
#include <xmlsec/mscrypto/certkeys.h>
#include "private.h"

#define XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME_A     "MY"
#define XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME_W     L"MY"
#ifdef UNICODE
#define XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME_W
#else  /* UNICODE */
#define XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME_A
#endif /* UNICODE */

/****************************************************************************
 *
 * MSCrypto Keys Store. Uses Simple Keys Store under the hood
 *
 * Simple Keys Store ptr is located after xmlSecKeyStore
 *
 ***************************************************************************/
#define xmlSecMSCryptoKeysStoreSize \
        (sizeof(xmlSecKeyStore) + sizeof(xmlSecKeyStorePtr))

#define xmlSecMSCryptoKeysStoreGetSS(store) \
    ((xmlSecKeyStoreCheckSize((store), xmlSecMSCryptoKeysStoreSize)) ? \
     (xmlSecKeyStorePtr*)(((xmlSecByte*)(store)) + sizeof(xmlSecKeyStore)) : \
     (xmlSecKeyStorePtr*)NULL)

static int                      xmlSecMSCryptoKeysStoreInitialize   (xmlSecKeyStorePtr store);
static void                     xmlSecMSCryptoKeysStoreFinalize     (xmlSecKeyStorePtr store);
static xmlSecKeyPtr             xmlSecMSCryptoKeysStoreFindKey      (xmlSecKeyStorePtr store,
                                                                     const xmlChar* name,
                                                                     xmlSecKeyInfoCtxPtr keyInfoCtx);

static xmlSecKeyStoreKlass xmlSecMSCryptoKeysStoreKlass = {
    sizeof(xmlSecKeyStoreKlass),
    xmlSecMSCryptoKeysStoreSize,

    /* data */
    BAD_CAST "MSCrypto-keys-store",             /* const xmlChar* name; */

    /* constructors/destructor */
    xmlSecMSCryptoKeysStoreInitialize,          /* xmlSecKeyStoreInitializeMethod initialize; */
    xmlSecMSCryptoKeysStoreFinalize,            /* xmlSecKeyStoreFinalizeMethod finalize; */
    xmlSecMSCryptoKeysStoreFindKey,             /* xmlSecKeyStoreFindKeyMethod findKey; */

    /* reserved for the future */
    NULL,                                       /* void* reserved0; */
    NULL,                                       /* void* reserved1; */
};

/**
 * xmlSecMSCryptoKeysStoreGetKlass:
 *
 * The MSCrypto list based keys store klass.
 *
 * Returns: MSCrypto list based keys store klass.
 */
xmlSecKeyStoreId
xmlSecMSCryptoKeysStoreGetKlass(void) {
    return(&xmlSecMSCryptoKeysStoreKlass);
}

/**
 * xmlSecMSCryptoKeysStoreAdoptKey:
 * @store:              the pointer to MSCrypto keys store.
 * @key:                the pointer to key.
 *
 * Adds @key to the @store.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoKeysStoreAdoptKey(xmlSecKeyStorePtr store, xmlSecKeyPtr key) {
    xmlSecKeyStorePtr *ss;

    xmlSecAssert2(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId), -1);
    xmlSecAssert2((key != NULL), -1);

    ss = xmlSecMSCryptoKeysStoreGetSS(store);
    xmlSecAssert2(((ss != NULL) && (*ss != NULL) &&
        (xmlSecKeyStoreCheckId(*ss, xmlSecSimpleKeysStoreId))), -1);

    return (xmlSecSimpleKeysStoreAdoptKey(*ss, key));
}

/**
 * xmlSecMSCryptoKeysStoreLoad:
 * @store:              the pointer to MSCrypto keys store.
 * @uri:                the filename.
 * @keysMngr:           the pointer to associated keys manager.
 *
 * Reads keys from an XML file.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoKeysStoreLoad(xmlSecKeyStorePtr store, const char *uri,
                            xmlSecKeysMngrPtr keysMngr) {
    xmlDocPtr doc;
    xmlNodePtr root;
    xmlNodePtr cur;
    xmlSecKeyPtr key;
    xmlSecKeyInfoCtx keyInfoCtx;
    int ret;

    xmlSecAssert2(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId), -1);
    xmlSecAssert2((uri != NULL), -1);

    doc = xmlParseFile(uri);
    if(doc == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                    "xmlParseFile",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "uri=%s",
                    xmlSecErrorsSafeString(uri));
        return(-1);
    }

    root = xmlDocGetRootElement(doc);
    if(!xmlSecCheckNodeName(root, BAD_CAST "Keys", xmlSecNs)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(root)),
                    XMLSEC_ERRORS_R_INVALID_NODE,
                    "expected-node=<xmlsec:Keys>");
        xmlFreeDoc(doc);
        return(-1);
    }

    cur = xmlSecGetNextElementNode(root->children);
    while((cur != NULL) && xmlSecCheckNodeName(cur, xmlSecNodeKeyInfo, xmlSecDSigNs)) {
        key = xmlSecKeyCreate();
        if(key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                        xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                        XMLSEC_ERRORS_R_INVALID_NODE,
                        "expected-node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeKeyInfo));
            xmlFreeDoc(doc);
            return(-1);
        }

        ret = xmlSecKeyInfoCtxInitialize(&keyInfoCtx, NULL);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                        "xmlSecKeyInfoCtxInitialize",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyDestroy(key);
            xmlFreeDoc(doc);
            return(-1);
        }

        keyInfoCtx.mode           = xmlSecKeyInfoModeRead;
        keyInfoCtx.keysMngr       = keysMngr;
        keyInfoCtx.flags          = XMLSEC_KEYINFO_FLAGS_DONT_STOP_ON_KEY_FOUND |
                                    XMLSEC_KEYINFO_FLAGS_X509DATA_DONT_VERIFY_CERTS;
        keyInfoCtx.keyReq.keyId   = xmlSecKeyDataIdUnknown;
        keyInfoCtx.keyReq.keyType = xmlSecKeyDataTypeAny;
        keyInfoCtx.keyReq.keyUsage= xmlSecKeyDataUsageAny;

        ret = xmlSecKeyInfoNodeRead(cur, key, &keyInfoCtx);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                        "xmlSecKeyInfoNodeRead",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyInfoCtxFinalize(&keyInfoCtx);
            xmlSecKeyDestroy(key);
            xmlFreeDoc(doc);
            return(-1);
        }
        xmlSecKeyInfoCtxFinalize(&keyInfoCtx);

        if(xmlSecKeyIsValid(key)) {
            ret = xmlSecMSCryptoKeysStoreAdoptKey(store, key);
            if(ret < 0) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                            "xmlSecMSCryptoKeysStoreAdoptKey",
                            XMLSEC_ERRORS_R_XMLSEC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlSecKeyDestroy(key);
                xmlFreeDoc(doc);
                return(-1);
            }
        } else {
            /* we have an unknown key in our file, just ignore it */
            xmlSecKeyDestroy(key);
        }
        cur = xmlSecGetNextElementNode(cur->next);
    }

    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                    xmlSecErrorsSafeString(xmlSecNodeGetName(cur)),
                    XMLSEC_ERRORS_R_UNEXPECTED_NODE,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFreeDoc(doc);
        return(-1);
    }

    xmlFreeDoc(doc);
    return(0);
}

/**
 * xmlSecMSCryptoKeysStoreSave:
 * @store:              the pointer to MSCrypto keys store.
 * @filename:           the filename.
 * @type:               the saved keys type (public, private, ...).
 *
 * Writes keys from @store to an XML file.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecMSCryptoKeysStoreSave(xmlSecKeyStorePtr store, const char *filename, xmlSecKeyDataType type) {
    xmlSecKeyStorePtr *ss;

    xmlSecAssert2(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId), -1);
    xmlSecAssert2((filename != NULL), -1);

    ss = xmlSecMSCryptoKeysStoreGetSS(store);
    xmlSecAssert2(((ss != NULL) && (*ss != NULL) &&
                   (xmlSecKeyStoreCheckId(*ss, xmlSecSimpleKeysStoreId))), -1);

    return (xmlSecSimpleKeysStoreSave(*ss, filename, type));
}

static int
xmlSecMSCryptoKeysStoreInitialize(xmlSecKeyStorePtr store) {
    xmlSecKeyStorePtr *ss;

    xmlSecAssert2(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId), -1);

    ss = xmlSecMSCryptoKeysStoreGetSS(store);
    xmlSecAssert2((*ss == NULL), -1);

    *ss = xmlSecKeyStoreCreate(xmlSecSimpleKeysStoreId);
    if(*ss == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                    "xmlSecKeyStoreCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecSimpleKeysStoreId");
        return(-1);
    }

    return(0);
}

static void
xmlSecMSCryptoKeysStoreFinalize(xmlSecKeyStorePtr store) {
    xmlSecKeyStorePtr *ss;

    xmlSecAssert(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId));

    ss = xmlSecMSCryptoKeysStoreGetSS(store);
    xmlSecAssert((ss != NULL) && (*ss != NULL));

    xmlSecKeyStoreDestroy(*ss);
}

static PCCERT_CONTEXT
xmlSecMSCryptoKeysStoreFindCert(xmlSecKeyStorePtr store, const xmlChar* name,
                                xmlSecKeyInfoCtxPtr keyInfoCtx) {
    LPCTSTR storeName;
    HCERTSTORE hStoreHandle = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    LPTSTR wcName = NULL;

    xmlSecAssert2(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId), NULL);
    xmlSecAssert2(name != NULL, NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);

    storeName = xmlSecMSCryptoAppGetCertStoreName();
    if(storeName == NULL) {
        storeName = XMLSEC_MSCRYPTO_APP_DEFAULT_CERT_STORE_NAME;
    }

    hStoreHandle = CertOpenSystemStore(0, storeName);
    if (NULL == hStoreHandle) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "CertOpenSystemStore",
                    XMLSEC_ERRORS_R_CRYPTO_FAILED,
                    "storeName=%s",
                    xmlSecErrorsSafeString(storeName));
        return(NULL);
    }

    /* convert name to unicode */
    wcName = xmlSecMSCryptoConvertUtf8ToTstr(name);
    if(wcName == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                        "xmlSecMSCryptoConvertUtf8ToUnicode",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "wcName");
        CertCloseStore(hStoreHandle, 0);
        return(NULL);
    }

    /* first attempt: try to find the cert with a full blown subject dn */
    if(NULL == pCertContext) {
        pCertContext = xmlSecMSCryptoX509FindCertBySubject(
            hStoreHandle,
            wcName,
            X509_ASN_ENCODING | PKCS_7_ASN_ENCODING);
    }

    /*
     * Try ro find certificate with name="Friendly Name"
     */
    if (NULL == pCertContext) {
        DWORD dwPropSize;
        PBYTE pbFriendlyName;
        PCCERT_CONTEXT pCertCtxIter = NULL;


        while (pCertCtxIter = CertEnumCertificatesInStore(hStoreHandle, pCertCtxIter)) {
            if (TRUE != CertGetCertificateContextProperty(pCertCtxIter,
                                                      CERT_FRIENDLY_NAME_PROP_ID,
                                                      NULL,
                                                      &dwPropSize)) {
                continue;
            }

            pbFriendlyName = xmlMalloc(dwPropSize);
            if(pbFriendlyName == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                            xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                            NULL,
                            XMLSEC_ERRORS_R_MALLOC_FAILED,
                            XMLSEC_ERRORS_NO_MESSAGE);
                xmlFree(wcName);
                CertCloseStore(hStoreHandle, 0);
                return(NULL);
            }

            if (TRUE != CertGetCertificateContextProperty(pCertCtxIter,
                                                      CERT_FRIENDLY_NAME_PROP_ID,
                                                      pbFriendlyName,
                                                      &dwPropSize)) {
                xmlFree(pbFriendlyName);
                continue;
            }

            /* Compare FriendlyName to name */
            if (!lstrcmp(wcName, (LPCTSTR)pbFriendlyName)) {
              pCertContext = pCertCtxIter;
              xmlFree(pbFriendlyName);
              break;
            }
            xmlFree(pbFriendlyName);
        }
    }

    /* We don't give up easily, now try to find cert with part of the name
     */
    if (NULL == pCertContext) {
        pCertContext = CertFindCertificateInStore(
            hStoreHandle,
            X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
            0,
            CERT_FIND_SUBJECT_STR,
            wcName,
            NULL);
    }


    /* We could do the following here:
     * It would be nice if we could locate the cert with issuer name and
     * serial number, the given keyname can be something like this:
     * 'serial=1234567;issuer=CN=ikke, C=NL'
     * to be implemented by the first person who reads this, and thinks it's
     * a good idea :) WK
     */

    /* OK, I give up, I'm gone :( */

    /* aleksey todo: is it a right idea to close store if we have a handle to
     * a cert in this store? */
    xmlFree(wcName);
    CertCloseStore(hStoreHandle, 0);
    return(pCertContext);
}


static xmlSecKeyPtr
xmlSecMSCryptoKeysStoreFindKey(xmlSecKeyStorePtr store, const xmlChar* name,
                               xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyStorePtr* ss;
    xmlSecKeyPtr key = NULL;
    xmlSecKeyReqPtr keyReq = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    PCCERT_CONTEXT pCertContext2 = NULL;
    xmlSecKeyDataPtr data = NULL;
    xmlSecKeyDataPtr x509Data = NULL;
    xmlSecKeyPtr res = NULL;
    int ret;

    xmlSecAssert2(xmlSecKeyStoreCheckId(store, xmlSecMSCryptoKeysStoreId), NULL);
    xmlSecAssert2(keyInfoCtx != NULL, NULL);

    ss = xmlSecMSCryptoKeysStoreGetSS(store);
    xmlSecAssert2(((ss != NULL) && (*ss != NULL)), NULL);

    /* first try to find key in the simple keys store */
    key = xmlSecKeyStoreFindKey(*ss, name, keyInfoCtx);
    if (key != NULL) {
        return (key);
    }

    /* Next try to find the key in the MS Certificate store, and construct an xmlSecKey.
    *  we must have a name to lookup keys in the certificate store.
    */
    if (name == NULL) {
        goto done;
    }

    /* what type of key are we looking for?
    * WK: For now, we'll look only for public/private keys using the
    * name as a cert nickname. Then the name is regarded as the subject
    * dn of the certificate to be searched for.
    */
    keyReq = &(keyInfoCtx->keyReq);
    if (keyReq->keyType & (xmlSecKeyDataTypePublic | xmlSecKeyDataTypePrivate)) {
        pCertContext = xmlSecMSCryptoKeysStoreFindCert(store, name, keyInfoCtx);
        if(pCertContext == NULL) {
            goto done;
        }

        /* set cert in x509 data */
        x509Data = xmlSecKeyDataCreate(xmlSecMSCryptoKeyDataX509Id);
        if(x509Data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyDataCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }

        pCertContext2 = CertDuplicateCertificateContext(pCertContext);
        if (NULL == pCertContext2) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CertDuplicateCertificateContext",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }

        ret = xmlSecMSCryptoKeyDataX509AdoptCert(x509Data, pCertContext2);
        if (ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeyDataX509AdoptCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }
        pCertContext2 = NULL;

        pCertContext2 = CertDuplicateCertificateContext(pCertContext);
        if (NULL == pCertContext2) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "CertDuplicateCertificateContext",
                        XMLSEC_ERRORS_R_CRYPTO_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }

        ret = xmlSecMSCryptoKeyDataX509AdoptKeyCert(x509Data, pCertContext2);
        if (ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoKeyDataX509AdoptKeyCert",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }
        pCertContext2 = NULL;

        /* set cert in key data */
        data = xmlSecMSCryptoCertAdopt(pCertContext, keyReq->keyType);
        if(data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecMSCryptoCertAdopt",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }
        pCertContext = NULL;

        /* create key and add key data and x509 data to it */
        key = xmlSecKeyCreate();
        if (key == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }

        ret = xmlSecKeySetValue(key, data);
        if (ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeySetValue",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(data)));
            goto done;
        }
        data = NULL;

        ret = xmlSecKeyAdoptData(key, x509Data);
        if (ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyAdoptData",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "data=%s",
                        xmlSecErrorsSafeString(xmlSecKeyDataGetName(x509Data)));
            goto done;
        }
        x509Data = NULL;

        /* Set the name of the key to the given name */
        ret = xmlSecKeySetName(key, name);
        if (ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        xmlSecErrorsSafeString(xmlSecKeyStoreGetName(store)),
                        "xmlSecKeySetName",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            goto done;
        }

        /* now that we have a key, make sure it is valid and let the simple
        * store adopt it */
        if (xmlSecKeyIsValid(key)) {
            res = key;
            key = NULL;
        }
    }

done:
    if (NULL != pCertContext) {
        CertFreeCertificateContext(pCertContext);
    }
    if (NULL != pCertContext2) {
        CertFreeCertificateContext(pCertContext2);
    }
    if (data != NULL) {
        xmlSecKeyDataDestroy(data);
    }
    if (x509Data != NULL) {
        xmlSecKeyDataDestroy(x509Data);
    }
    if (key != NULL) {
        xmlSecKeyDestroy(key);
    }

    return (res);
}

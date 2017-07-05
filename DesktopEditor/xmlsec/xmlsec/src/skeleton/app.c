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

/* TODO: aadd Skeleton include files */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/skeleton/app.h>
#include <xmlsec/skeleton/crypto.h>

/**
 * xmlSecSkeletonAppInit:
 * @config:             the path to Skeleton configuration (unused).
 *
 * General crypto engine initialization. This function is used
 * by XMLSec command line utility and called before
 * @xmlSecInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppInit(const char* config ATTRIBUTE_UNUSED) {
    /* TODO: initialize Skeleton crypto engine */
    return(0);
}

/**
 * xmlSecSkeletonAppShutdown:
 *
 * General crypto engine shutdown. This function is used
 * by XMLSec command line utility and called after
 * @xmlSecShutdown function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppShutdown(void) {
    /* TODO: shutdown Skeleton crypto engine */

    return(0);
}

/**
 * xmlSecSkeletonAppKeyLoad:
 * @filename:           the key filename.
 * @format:             the key file format.
 * @pwd:                the key file password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from the a file (not implemented yet).
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecSkeletonAppKeyLoad(const char *filename, xmlSecKeyDataFormat format,
                        const char *pwd,
                        void* pwdCallback,
                        void* pwdCallbackCtx) {
    xmlSecAssert2(filename != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    /* TODO: load key */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppKeyLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(NULL);
}

/**
 * xmlSecSkeletonAppKeyLoadMemory:
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @format:             the key data format.
 * @pwd:                the key data2 password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from a binary @data.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecSkeletonAppKeyLoadMemory(const xmlSecByte* data, xmlSecSize dataSize, xmlSecKeyDataFormat format,
                    const char *pwd, void* pwdCallback, void* pwdCallbackCtx) {
    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    /* TODO: load key */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppKeyLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(NULL);
}


#ifndef XMLSEC_NO_X509
/**
 * xmlSecSkeletonAppKeyCertLoad:
 * @key:                the pointer to key.
 * @filename:           the certificate filename.
 * @format:             the certificate file format.
 *
 * Reads the certificate from $@filename and adds it to key
 * (not implemented yet).
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppKeyCertLoad(xmlSecKeyPtr key, const char* filename,
                          xmlSecKeyDataFormat format) {
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppKeyCertLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

/**
 * xmlSecSkeletonAppKeyCertLoadMemory:
 * @key:                the pointer to key.
 * @data:               the certificate binary data.
 * @dataSize:           the certificate binary data size.
 * @format:             the certificate file format.
 *
 * Reads the certificate from memory buffer and adds it to key.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppKeyCertLoadMemory(xmlSecKeyPtr key, const xmlSecByte* data, xmlSecSize dataSize,
                                xmlSecKeyDataFormat format) {
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppKeyCertLoadMemory",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

/**
 * xmlSecSkeletonAppPkcs12Load:
 * @filename:           the PKCS12 key filename.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 file
 * (not implemented yet).
 * For uniformity, call xmlSecSkeletonAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecSkeletonAppPkcs12Load(const char *filename,
                          const char *pwd ATTRIBUTE_UNUSED,
                          void* pwdCallback ATTRIBUTE_UNUSED,
                          void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    xmlSecAssert2(filename != NULL, NULL);

    /* TODO: load pkcs12 file */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppPkcs12Load",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(NULL);
}

/**
 * xmlSecSkeletonAppPkcs12LoadMemory:
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @pwd:                the PKCS12 password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 binary data.
 * For uniformity, call xmlSecSkeletonAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecSkeletonAppPkcs12LoadMemory(const xmlSecByte* data, xmlSecSize dataSize, const char *pwd,
                       void *pwdCallback ATTRIBUTE_UNUSED,
                       void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    xmlSecAssert2(data != NULL, NULL);

    /* TODO: load pkcs12 file */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppPkcs12Load",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(NULL);
}



/**
 * xmlSecSkeletonAppKeysMngrCertLoad:
 * @mngr:               the keys manager.
 * @filename:           the certificate file.
 * @format:             the certificate file format.
 * @type:               the flag that indicates is the certificate in @filename
 *                      trusted or not.
 *
 * Reads cert from @filename and adds to the list of trusted or known
 * untrusted certs in @store (not implemented yet).
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppKeysMngrCertLoad(xmlSecKeysMngrPtr mngr, const char *filename,
                                xmlSecKeyDataFormat format,
                                xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO: load cert and add to keys manager */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppKeysMngrCertLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

/**
 * xmlSecSkeletonAppKeysMngrCertLoadMemory:
 * @mngr:               the pointer to keys manager.
 * @data:               the key binary data.
 * @dataSize:           the key binary data size.
 * @format:             the certificate format (PEM or DER).
 * @type:               the certificate type (trusted/untrusted).
 *
 * Reads cert from @data and adds to the list of trusted or known
 * untrusted certs in @store
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppKeysMngrCertLoadMemory(xmlSecKeysMngrPtr mngr, const xmlSecByte* data,
                             xmlSecSize dataSize, xmlSecKeyDataFormat format,
                             xmlSecKeyDataType type) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO: load cert and add to keys manager */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecSkeletonAppKeysMngrCertLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

#endif /* XMLSEC_NO_X509 */

/**
 * xmlSecSkeletonAppDefaultKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Initializes @mngr with simple keys store #xmlSecSimpleKeysStoreId
 * and a default Skeleton crypto key data stores.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppDefaultKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

    /* TODO: if Skeleton crypto engine has another default
     * keys storage then use it!
     */

    /* create simple keys store if needed */
    if(xmlSecKeysMngrGetKeysStore(mngr) == NULL) {
        xmlSecKeyStorePtr keysStore;

        keysStore = xmlSecKeyStoreCreate(xmlSecSimpleKeysStoreId);
        if(keysStore == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeyStoreCreate",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "xmlSecSimpleKeysStoreId");
            return(-1);
        }

        ret = xmlSecKeysMngrAdoptKeysStore(mngr, keysStore);
        if(ret < 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecKeysMngrAdoptKeysStore",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            xmlSecKeyStoreDestroy(keysStore);
            return(-1);
        }
    }

    ret = xmlSecSkeletonKeysMngrInit(mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecSkeletonKeysMngrInit",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    mngr->getKey = xmlSecKeysMngrGetKey;
    return(0);
}

/**
 * xmlSecSkeletonAppDefaultKeysMngrAdoptKey:
 * @mngr:               the pointer to keys manager.
 * @key:                the pointer to key.
 *
 * Adds @key to the keys manager @mngr created with #xmlSecSkeletonAppDefaultKeysMngrInit
 * function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppDefaultKeysMngrAdoptKey(xmlSecKeysMngrPtr mngr, xmlSecKeyPtr key) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(key != NULL, -1);

    /* TODO: if Skeleton crypto engine has another default
     * keys storage then use it!
     */

    store = xmlSecKeysMngrGetKeysStore(mngr);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetKeysStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecSimpleKeysStoreAdoptKey(store, key);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecSimpleKeysStoreAdoptKey",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecSkeletonAppDefaultKeysMngrLoad:
 * @mngr:               the pointer to keys manager.
 * @uri:                the uri.
 *
 * Loads XML keys file from @uri to the keys manager @mngr created
 * with #xmlSecSkeletonAppDefaultKeysMngrInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppDefaultKeysMngrLoad(xmlSecKeysMngrPtr mngr, const char* uri) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(uri != NULL, -1);

    /* TODO: if Skeleton crypto engine has another default
     * keys storage then use it!
     */

    store = xmlSecKeysMngrGetKeysStore(mngr);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetKeysStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecSimpleKeysStoreLoad(store, uri, mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecSimpleKeysStoreLoad",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "uri=%s", xmlSecErrorsSafeString(uri));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecSkeletonAppDefaultKeysMngrSave:
 * @mngr:               the pointer to keys manager.
 * @filename:           the destination filename.
 * @type:               the type of keys to save (public/private/symmetric).
 *
 * Saves keys from @mngr to  XML keys file.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecSkeletonAppDefaultKeysMngrSave(xmlSecKeysMngrPtr mngr, const char* filename, xmlSecKeyDataType type) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);

    /* TODO: if Skeleton crypto engine has another default
     * keys storage then use it!
     */

    store = xmlSecKeysMngrGetKeysStore(mngr);
    if(store == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeysMngrGetKeysStore",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecSimpleKeysStoreSave(store, filename, type);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecSimpleKeysStoreSave",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(filename));
        return(-1);
    }

    return(0);
}

/**
 * xmlSecSkeletonAppGetDefaultPwdCallback:
 *
 * Gets default password callback.
 *
 * Returns: default password callback.
 */
void*
xmlSecSkeletonAppGetDefaultPwdCallback(void) {
    /* TODO */
    return(NULL);
}


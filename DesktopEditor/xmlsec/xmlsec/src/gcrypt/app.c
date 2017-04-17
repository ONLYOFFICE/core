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
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gcrypt/app.h>
#include <xmlsec/gcrypt/crypto.h>

#include "asn1.h"

/**
 * xmlSecGCryptAppInit:
 * @config:             the path to GCrypt configuration (unused).
 *
 * General crypto engine initialization. This function is used
 * by XMLSec command line utility and called before
 * @xmlSecInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppInit(const char* config ATTRIBUTE_UNUSED) {
    /* Secure memory initialisation based on documentation from:
         http://www.gnupg.org/documentation/manuals/gcrypt/Initializing-the-library.html
       NOTE sample code don't check gcry_control(...) return code

       All flags from:
         http://www.gnupg.org/documentation/manuals/gcrypt/Controlling-the-library.html

       Also libgcrypt NEWS entries:
+++++
.....
Noteworthy changes in version 1.4.3 (2008-09-18)
------------------------------------------------

 * Try to auto-initialize Libgcrypt to minimize the effect of
   applications not doing that correctly.  This is not a perfect
   solution but given that many applicationion would totally fail
   without such a hack, we try to help at least with the most common
   cases.  Folks, please read the manual to learn how to properly
   initialize Libgcrypt!

 * Auto-initialize the secure memory to 32k instead of aborting the
   process.
.....
+++++
    */

    /* Version check should be the very first call because it
       makes sure that important subsystems are intialized. */

    /* NOTE configure.in defines GCRYPT_MIN_VERSION */
    if (!gcry_check_version (GCRYPT_MIN_VERSION)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_check_version",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* We don't want to see any warnings, e.g. because we have not yet
       parsed program options which might be used to suppress such
       warnings. */
    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);

    /* ... If required, other initialization goes here.  Note that the
       process might still be running with increased privileges and that
       the secure memory has not been intialized.  */

    /* Allocate a pool of 32k secure memory.  This make the secure memory
       available and also drops privileges where needed.  */
    gcry_control(GCRYCTL_INIT_SECMEM, 32768, 0);

    /* It is now okay to let Libgcrypt complain when there was/is
      a problem with the secure memory. */
    gcry_control(GCRYCTL_RESUME_SECMEM_WARN);

    /* ... If required, other initialization goes here.  */

    /* Tell Libgcrypt that initialization has completed. */
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    return(0);
}

/**
 * xmlSecGCryptAppShutdown:
 *
 * General crypto engine shutdown. This function is used
 * by XMLSec command line utility and called after
 * @xmlSecShutdown function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppShutdown(void) {
    gcry_error_t err;

    err = gcry_control(GCRYCTL_TERM_SECMEM);
    if (gcry_err_code(err)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "gcry_control(GCRYCTL_TERM_SECMEM)",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    return(0);
}

/**
 * xmlSecGCryptAppKeyLoad:
 * @filename:           the key filename.
 * @format:             the key file format.
 * @pwd:                the key file password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from the a file.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecGCryptAppKeyLoad(const char *filename, xmlSecKeyDataFormat format,
                        const char *pwd,
                        void* pwdCallback,
                        void* pwdCallbackCtx) {
    xmlSecKeyPtr key;
    xmlSecBuffer buffer;
    int ret;

    xmlSecAssert2(filename != NULL, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    ret = xmlSecBufferInitialize(&buffer, 4*1024);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    ret = xmlSecBufferReadFile(&buffer, filename);
    if((ret < 0) || (xmlSecBufferGetData(&buffer) == NULL) || (xmlSecBufferGetSize(&buffer) <= 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBufferReadFile",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s", 
                    xmlSecErrorsSafeString(filename));
        xmlSecBufferFinalize(&buffer);
        return(NULL);
    }

    key = xmlSecGCryptAppKeyLoadMemory(xmlSecBufferGetData(&buffer),
                    xmlSecBufferGetSize(&buffer),
                    format, pwd, pwdCallback, pwdCallbackCtx);
    if(key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAppKeyLoadMemory",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(filename));
        xmlSecBufferFinalize(&buffer);
        return(NULL);
    }

    /* cleanup */
    xmlSecBufferFinalize(&buffer);
    return(key);
}

/**
 * xmlSecGCryptAppKeyLoadMemory:
 * @data:               the binary key data.
 * @dataSize:           the size of binary key.
 * @format:             the key file format.
 * @pwd:                the key file password.
 * @pwdCallback:        the key password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key from the memory buffer.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecGCryptAppKeyLoadMemory(const xmlSecByte* data, xmlSecSize dataSize,
                        xmlSecKeyDataFormat format,
                        const char *pwd ATTRIBUTE_UNUSED,
                        void* pwdCallback ATTRIBUTE_UNUSED,
                        void* pwdCallbackCtx ATTRIBUTE_UNUSED)
{
    xmlSecKeyPtr key = NULL;
    xmlSecKeyDataPtr key_data = NULL;
    int ret;

    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(dataSize > 0, NULL);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, NULL);

    switch(format) {
    case xmlSecKeyDataFormatDer:
        key_data = xmlSecGCryptParseDer(data, dataSize, xmlSecGCryptDerKeyTypeAuto);
        if(key_data == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecGCryptParseDer",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }
        break;
    case xmlSecKeyDataFormatPem:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAppKeyLoadMemory",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return (NULL);
#ifndef XMLSEC_NO_X509
    case xmlSecKeyDataFormatPkcs12:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptAppKeyLoadMemory",
                    XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return (NULL);
#endif /* XMLSEC_NO_X509 */
    default:
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_INVALID_FORMAT,
                    "format=%d", format);
        return(NULL);
    }

    /* we should have key data by now */
    xmlSecAssert2(key_data != NULL, NULL);
    key = xmlSecKeyCreate();
    if(key == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeyCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecKeyDataDestroy(key_data);
        return(NULL);
    }

    ret = xmlSecKeySetValue(key, key_data);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecKeySetValue",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "data=%s",
                    xmlSecErrorsSafeString(xmlSecKeyDataGetName(key_data)));
        xmlSecKeyDestroy(key);
        xmlSecKeyDataDestroy(key_data);
        return(NULL);
    }
    key_data = NULL; /* key_data is owned by key */

    /* done */
    return(key);
}

#ifndef XMLSEC_NO_X509
/**
 * xmlSecGCryptAppKeyCertLoad:
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
xmlSecGCryptAppKeyCertLoad(xmlSecKeyPtr key, const char* filename,
                          xmlSecKeyDataFormat format) {
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecGCryptAppKeyCertLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

/**
 * xmlSecGCryptAppKeyCertLoadMemory:
 * @key:                the pointer to key.
 * @data:               the certificate binary data.
 * @dataSize:           the certificate binary data size.
 * @format:             the certificate file format.
 *
 * Reads the certificate from memory buffer and adds it to key (not implemented yet).
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppKeyCertLoadMemory(xmlSecKeyPtr key,
                                 const xmlSecByte* data,
                                 xmlSecSize dataSize,
                                 xmlSecKeyDataFormat format) {
    xmlSecAssert2(key != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize > 0, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecGCryptAppKeyCertLoadMemory",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

/**
 * xmlSecGCryptAppPkcs12Load:
 * @filename:           the PKCS12 key filename.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 file
 * (not implemented yet).
 * For uniformity, call xmlSecGCryptAppKeyLoad instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12.
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecGCryptAppPkcs12Load(const char *filename,
                          const char *pwd ATTRIBUTE_UNUSED,
                          void* pwdCallback ATTRIBUTE_UNUSED,
                          void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    xmlSecAssert2(filename != NULL, NULL);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecGCryptAppPkcs12Load",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(NULL);
}

/**
 * xmlSecGCryptAppPkcs12LoadMemory:
 * @data:               the PKCS12 binary data.
 * @dataSize:           the PKCS12 binary data size.
 * @pwd:                the PKCS12 file password.
 * @pwdCallback:        the password callback.
 * @pwdCallbackCtx:     the user context for password callback.
 *
 * Reads key and all associated certificates from the PKCS12 data in memory buffer.
 * For uniformity, call xmlSecGCryptAppKeyLoadMemory instead of this function. Pass
 * in format=xmlSecKeyDataFormatPkcs12 (not implemented yet).
 *
 * Returns: pointer to the key or NULL if an error occurs.
 */
xmlSecKeyPtr
xmlSecGCryptAppPkcs12LoadMemory(const xmlSecByte* data, xmlSecSize dataSize,
                           const char *pwd ATTRIBUTE_UNUSED,
                           void* pwdCallback ATTRIBUTE_UNUSED,
                           void* pwdCallbackCtx ATTRIBUTE_UNUSED) {
    xmlSecAssert2(data != NULL, NULL);
    xmlSecAssert2(dataSize > 0, NULL);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecGCryptAppPkcs12LoadMemory",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(NULL);
}

/**
 * xmlSecGCryptAppKeysMngrCertLoad:
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
xmlSecGCryptAppKeysMngrCertLoad(xmlSecKeysMngrPtr mngr, 
                                const char *filename,
                                xmlSecKeyDataFormat format,
                                xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecGCryptAppKeysMngrCertLoad",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

/**
 * xmlSecGCryptAppKeysMngrCertLoadMemory:
 * @mngr:               the keys manager.
 * @data:               the certificate binary data.
 * @dataSize:           the certificate binary data size.
 * @format:             the certificate file format.
 * @type:               the flag that indicates is the certificate trusted or not.
 *
 * Reads cert from binary buffer @data and adds to the list of trusted or known
 * untrusted certs in @store (not implemented yet).
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppKeysMngrCertLoadMemory(xmlSecKeysMngrPtr mngr,
                                      const xmlSecByte* data,
                                      xmlSecSize dataSize,
                                      xmlSecKeyDataFormat format,
                                      xmlSecKeyDataType type ATTRIBUTE_UNUSED) {
    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(data != NULL, -1);
    xmlSecAssert2(dataSize > 0, -1);
    xmlSecAssert2(format != xmlSecKeyDataFormatUnknown, -1);

    /* TODO */
    xmlSecError(XMLSEC_ERRORS_HERE,
                NULL,
                "xmlSecGCryptAppKeysMngrCertLoadMemory",
                XMLSEC_ERRORS_R_NOT_IMPLEMENTED,
                XMLSEC_ERRORS_NO_MESSAGE);
    return(-1);
}

#endif /* XMLSEC_NO_X509 */

/**
 * xmlSecGCryptAppDefaultKeysMngrInit:
 * @mngr:               the pointer to keys manager.
 *
 * Initializes @mngr with simple keys store #xmlSecSimpleKeysStoreId
 * and a default GCrypt crypto key data stores.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppDefaultKeysMngrInit(xmlSecKeysMngrPtr mngr) {
    int ret;

    xmlSecAssert2(mngr != NULL, -1);

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

    ret = xmlSecGCryptKeysMngrInit(mngr);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecGCryptKeysMngrInit",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* TODO */
    mngr->getKey = xmlSecKeysMngrGetKey;
    return(0);
}

/**
 * xmlSecGCryptAppDefaultKeysMngrAdoptKey:
 * @mngr:               the pointer to keys manager.
 * @key:                the pointer to key.
 *
 * Adds @key to the keys manager @mngr created with #xmlSecGCryptAppDefaultKeysMngrInit
 * function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppDefaultKeysMngrAdoptKey(xmlSecKeysMngrPtr mngr, xmlSecKeyPtr key) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(key != NULL, -1);

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
 * xmlSecGCryptAppDefaultKeysMngrLoad:
 * @mngr:               the pointer to keys manager.
 * @uri:                the uri.
 *
 * Loads XML keys file from @uri to the keys manager @mngr created
 * with #xmlSecGCryptAppDefaultKeysMngrInit function.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppDefaultKeysMngrLoad(xmlSecKeysMngrPtr mngr, const char* uri) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(uri != NULL, -1);

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
 * xmlSecGCryptAppDefaultKeysMngrSave:
 * @mngr:               the pointer to keys manager.
 * @filename:           the destination filename.
 * @type:               the type of keys to save (public/private/symmetric).
 *
 * Saves keys from @mngr to  XML keys file.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecGCryptAppDefaultKeysMngrSave(xmlSecKeysMngrPtr mngr, const char* filename, xmlSecKeyDataType type) {
    xmlSecKeyStorePtr store;
    int ret;

    xmlSecAssert2(mngr != NULL, -1);
    xmlSecAssert2(filename != NULL, -1);

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
 * xmlSecGCryptAppGetDefaultPwdCallback:
 *
 * Gets default password callback.
 *
 * Returns: default password callback.
 */
void*
xmlSecGCryptAppGetDefaultPwdCallback(void) {
    return(NULL);
}


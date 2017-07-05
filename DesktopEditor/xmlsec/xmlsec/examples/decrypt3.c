/** 
 * XML Security Library example: Decrypting an encrypted file using a custom keys manager.
 * 
 * Decrypts encrypted XML file using a custom files based keys manager.
 * We assume that key's name in <dsig:KeyName/> element is just 
 * key's file name in the current folder.
 * 
 * Usage: 
 *      ./decrypt3 <xml-enc> 
 *
 * Example:
 *      ./decrypt3 encrypt1-res.xml
 *      ./decrypt3 encrypt2-res.xml
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 * 
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#include <libxslt/security.h>
#endif /* XMLSEC_NO_XSLT */

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmlenc.h>
#include <xmlsec/crypto.h>

xmlSecKeyStoreId  files_keys_store_get_klass(void);
xmlSecKeysMngrPtr create_files_keys_mngr(void);
int decrypt_file(xmlSecKeysMngrPtr mngr, const char* enc_file);

int 
main(int argc, char **argv) {
    xmlSecKeysMngrPtr mngr;
#ifndef XMLSEC_NO_XSLT
    xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif /* XMLSEC_NO_XSLT */    

    assert(argv);

    if(argc != 2) {
        fprintf(stderr, "Error: wrong number of arguments.\n");
        fprintf(stderr, "Usage: %s <enc-file>\n", argv[0]);
        return(1);
    }

    /* Init libxml and libxslt libraries */
    xmlInitParser();
    LIBXML_TEST_VERSION
    xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
    xmlIndentTreeOutput = 1; 
#endif /* XMLSEC_NO_XSLT */

    /* Init libxslt */
#ifndef XMLSEC_NO_XSLT
    /* disable everything */
    xsltSecPrefs = xsltNewSecurityPrefs(); 
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_FILE,        xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_FILE,       xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_CREATE_DIRECTORY, xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_NETWORK,     xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_NETWORK,    xsltSecurityForbid);
    xsltSetDefaultSecurityPrefs(xsltSecPrefs); 
#endif /* XMLSEC_NO_XSLT */                

    /* Init xmlsec library */
    if(xmlSecInit() < 0) {
        fprintf(stderr, "Error: xmlsec initialization failed.\n");
        return(-1);
    }

    /* Check loaded library version */
    if(xmlSecCheckVersion() != 1) {
        fprintf(stderr, "Error: loaded xmlsec library version is not compatible.\n");
        return(-1);
    }

    /* Load default crypto engine if we are supporting dynamic
     * loading for xmlsec-crypto libraries. Use the crypto library
     * name ("openssl", "nss", etc.) to load corresponding 
     * xmlsec-crypto library.
     */
#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
    if(xmlSecCryptoDLLoadLibrary(NULL) < 0) {
        fprintf(stderr, "Error: unable to load default xmlsec-crypto library. Make sure\n"
                        "that you have it installed and check shared libraries path\n"
                        "(LD_LIBRARY_PATH) envornment variable.\n");
        return(-1);     
    }
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

    /* Init crypto library */
    if(xmlSecCryptoAppInit(NULL) < 0) {
        fprintf(stderr, "Error: crypto initialization failed.\n");
        return(-1);
    }

    /* Init xmlsec-crypto library */
    if(xmlSecCryptoInit() < 0) {
        fprintf(stderr, "Error: xmlsec-crypto initialization failed.\n");
        return(-1);
    }

    /* create keys manager and load keys */
    mngr = create_files_keys_mngr();
    if(mngr == NULL) {
        return(-1);
    }

    if(decrypt_file(mngr, argv[1]) < 0) {
        xmlSecKeysMngrDestroy(mngr);    
        return(-1);
    }    

    /* destroy keys manager */
    xmlSecKeysMngrDestroy(mngr);
    
    /* Shutdown xmlsec-crypto library */
    xmlSecCryptoShutdown();
    
    /* Shutdown crypto library */
    xmlSecCryptoAppShutdown();
    
    /* Shutdown xmlsec library */
    xmlSecShutdown();

    /* Shutdown libxslt/libxml */
#ifndef XMLSEC_NO_XSLT
    xsltFreeSecurityPrefs(xsltSecPrefs);
    xsltCleanupGlobals();            
#endif /* XMLSEC_NO_XSLT */
    xmlCleanupParser();
    
    return(0);
}

/**
 * decrypt_file:
 * @mngr:               the pointer to keys manager.
 * @enc_file:           the encrypted XML  file name.
 *
 * Decrypts the XML file #enc_file using DES key from #key_file and 
 * prints results to stdout.
 *
 * Returns 0 on success or a negative value if an error occurs.
 */
int 
decrypt_file(xmlSecKeysMngrPtr mngr, const char* enc_file) {
    xmlDocPtr doc = NULL;
    xmlNodePtr node = NULL;
    xmlSecEncCtxPtr encCtx = NULL;
    int res = -1;
    
    assert(mngr);
    assert(enc_file);

    /* load template */
    doc = xmlParseFile(enc_file);
    if ((doc == NULL) || (xmlDocGetRootElement(doc) == NULL)){
        fprintf(stderr, "Error: unable to parse file \"%s\"\n", enc_file);
        goto done;      
    }
    
    /* find start node */
    node = xmlSecFindNode(xmlDocGetRootElement(doc), xmlSecNodeEncryptedData, xmlSecEncNs);
    if(node == NULL) {
        fprintf(stderr, "Error: start node not found in \"%s\"\n", enc_file);
        goto done;      
    }

    /* create encryption context */
    encCtx = xmlSecEncCtxCreate(mngr);
    if(encCtx == NULL) {
        fprintf(stderr,"Error: failed to create encryption context\n");
        goto done;
    }

    /* decrypt the data */
    if((xmlSecEncCtxDecrypt(encCtx, node) < 0) || (encCtx->result == NULL)) {
        fprintf(stderr,"Error: decryption failed\n");
        goto done;
    }
        
    /* print decrypted data to stdout */
    if(encCtx->resultReplaced != 0) {
        fprintf(stdout, "Decrypted XML data:\n");
        xmlDocDump(stdout, doc);
    } else {
        fprintf(stdout, "Decrypted binary data (%d bytes):\n", xmlSecBufferGetSize(encCtx->result));
        if(xmlSecBufferGetData(encCtx->result) != NULL) {
            fwrite(xmlSecBufferGetData(encCtx->result), 
                  1, 
                  xmlSecBufferGetSize(encCtx->result),
                  stdout);
        }
    }
    fprintf(stdout, "\n");
        
    /* success */
    res = 0;

done:    
    /* cleanup */
    if(encCtx != NULL) {
        xmlSecEncCtxDestroy(encCtx);
    }
    
    if(doc != NULL) {
        xmlFreeDoc(doc); 
    }
    return(res);
}

/**
 * create_files_keys_mngr:
 *  
 * Creates a files based keys manager: we assume that key name is 
 * the key file name,
 *
 * Returns pointer to newly created keys manager or NULL if an error occurs.
 */
xmlSecKeysMngrPtr 
create_files_keys_mngr(void) {
    xmlSecKeyStorePtr keysStore;
    xmlSecKeysMngrPtr mngr;

    /* create files based keys store */
    keysStore = xmlSecKeyStoreCreate(files_keys_store_get_klass());
    if(keysStore == NULL) {
        fprintf(stderr, "Error: failed to create keys store.\n");
        return(NULL);
    }
    
    /* create keys manager */
    mngr = xmlSecKeysMngrCreate();
    if(mngr == NULL) {
        fprintf(stderr, "Error: failed to create keys manager.\n");
        xmlSecKeyStoreDestroy(keysStore);
        return(NULL);
    }

    /* add store to keys manager, from now on keys manager destroys the store if needed */
    if(xmlSecKeysMngrAdoptKeysStore(mngr, keysStore) < 0) {
        fprintf(stderr, "Error: failed to add keys store to keys manager.\n");
        xmlSecKeyStoreDestroy(keysStore);
        xmlSecKeysMngrDestroy(mngr);
        return(NULL);
    }
    
    /* initialize crypto library specific data in keys manager */
    if(xmlSecCryptoKeysMngrInit(mngr) < 0) {
        fprintf(stderr, "Error: failed to initialize crypto data in keys manager.\n");
        xmlSecKeysMngrDestroy(mngr);
        return(NULL);
    }

    /* set the get key callback */
    mngr->getKey = xmlSecKeysMngrGetKey;
    return(mngr);
}

/****************************************************************************
 *
 * Files Keys Store: we assume that key's name (content of the 
 * <dsig:KeyName/> element is a name of the file with a key (in the 
 * current folder).
 * Attention: this probably not a good solution for high traffic systems.
 * 
 ***************************************************************************/
static xmlSecKeyPtr             files_keys_store_find_key       (xmlSecKeyStorePtr store,
                                                                 const xmlChar* name,
                                                                 xmlSecKeyInfoCtxPtr keyInfoCtx);
static xmlSecKeyStoreKlass files_keys_store_klass = {
    sizeof(xmlSecKeyStoreKlass),
    sizeof(xmlSecKeyStore),
    BAD_CAST "files-based-keys-store",  /* const xmlChar* name; */         
    NULL,                               /* xmlSecKeyStoreInitializeMethod initialize; */
    NULL,                               /* xmlSecKeyStoreFinalizeMethod finalize; */
    files_keys_store_find_key,          /* xmlSecKeyStoreFindKeyMethod findKey; */

    /* reserved for the future */
    NULL,                               /* void* reserved0; */
    NULL,                               /* void* reserved1; */
};

/**
 * files_keys_store_get_klass:
 * 
 * The files based keys store klass: we assume that key name is the
 * key file name,
 *
 * Returns files based keys store klass.
 */
xmlSecKeyStoreId 
files_keys_store_get_klass(void) {
    return(&files_keys_store_klass);
}

/**
 * files_keys_store_find_key:
 * @store:              the pointer to simple keys store.
 * @name:               the desired key name.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> node processing context.
 *  
 * Lookups key in the @store. The caller is responsible for destroying
 * returned key with #xmlSecKeyDestroy function.
 *
 * Returns pointer to key or NULL if key not found or an error occurs.
 */
static xmlSecKeyPtr
files_keys_store_find_key(xmlSecKeyStorePtr store, const xmlChar* name, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecKeyPtr key;
    const xmlChar* p;
    
    assert(store);
    assert(keyInfoCtx);

    /* it's possible to do not have the key name or desired key type 
     * but we could do nothing in this case */
    if((name == NULL) || (keyInfoCtx->keyReq.keyId == xmlSecKeyDataIdUnknown)){
        return(NULL);
    }
    
    /* we don't want to open files in a folder other than "current";
     * to prevent it limit the characters in the key name to alpha/digit,
     * '.', '-' or '_'.
     */
    for(p = name; (*p) != '\0'; ++p) {
        if(!isalnum((*p)) && ((*p) != '.') && ((*p) != '-') && ((*p) != '_')) {
            return(NULL);
        }
    }
    
    if((keyInfoCtx->keyReq.keyId == xmlSecKeyDataDsaId) || (keyInfoCtx->keyReq.keyId == xmlSecKeyDataRsaId)) {
        /* load key from a pem file, if key is not found then it's an error (is it?) */
        key = xmlSecCryptoAppKeyLoad(name, xmlSecKeyDataFormatPem, NULL, NULL, NULL);
        if(key == NULL) {
            fprintf(stderr,"Error: failed to load public pem key from \"%s\"\n", name);
            return(NULL);
        }
    } else {
        /* otherwise it's a binary key, if key is not found then it's an error (is it?) */
        key = xmlSecKeyReadBinaryFile(keyInfoCtx->keyReq.keyId, name);
        if(key == NULL) {
            fprintf(stderr,"Error: failed to load key from binary file \"%s\"\n", name);
            return(NULL);
        }
    }

    /* set key name */
    if(xmlSecKeySetName(key, name) < 0) {
        fprintf(stderr,"Error: failed to set key name for key from \"%s\"\n", name);
        xmlSecKeyDestroy(key);
        return(NULL);   
    }

    return(key);
}


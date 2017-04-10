/** 
 * XML Security Library example: Encrypting XML file with a session key and dynamicaly created template.
 * 
 * Encrypts XML file using a dynamicaly created template file and a session 
 * DES key (encrypted with an RSA key).
 * 
 * Usage: 
 *      ./encrypt3 <xml-doc> <rsa-pem-key-file> 
 *
 * Example:
 *      ./encrypt3 encrypt3-doc.xml rsakey.pem > encrypt3-res.xml
 *
 * The result could be decrypted with decrypt3 example:
 *      ./decrypt3 encrypt3-res.xml
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 * 
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include <stdlib.h>
#include <string.h>
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
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>

xmlSecKeysMngrPtr load_rsa_keys(char* key_file);
int encrypt_file(xmlSecKeysMngrPtr mngr, const char* xml_file, const char* key_name);

int 
main(int argc, char **argv) {
    xmlSecKeysMngrPtr mngr;
#ifndef XMLSEC_NO_XSLT
    xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif /* XMLSEC_NO_XSLT */
    
    assert(argv);

    if(argc != 3) {
        fprintf(stderr, "Error: wrong number of arguments.\n");
        fprintf(stderr, "Usage: %s <xml-file> <key-file>\n", argv[0]);
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
    mngr = load_rsa_keys(argv[2]);
    if(mngr == NULL) {
        return(-1);
    }

    /* we use key filename as key name here */
    if(encrypt_file(mngr, argv[1], argv[2]) < 0) {
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
 * load_rsa_keys:
 * @key_file:           the key filename.
 *
 * Creates simple keys manager and load RSA key from #key_file in it.
 * The caller is responsible for destroing returned keys manager using
 * @xmlSecKeysMngrDestroy.
 *
 * Returns the pointer to newly created keys manager or NULL if an error
 * occurs.
 */
xmlSecKeysMngrPtr 
load_rsa_keys(char* key_file) {
    xmlSecKeysMngrPtr mngr;
    xmlSecKeyPtr key;
    
    assert(key_file);
    
    /* create and initialize keys manager, we use a simple list based
     * keys manager, implement your own xmlSecKeysStore klass if you need
     * something more sophisticated 
     */
    mngr = xmlSecKeysMngrCreate();
    if(mngr == NULL) {
        fprintf(stderr, "Error: failed to create keys manager.\n");
        return(NULL);
    }
    if(xmlSecCryptoAppDefaultKeysMngrInit(mngr) < 0) {
        fprintf(stderr, "Error: failed to initialize keys manager.\n");
        xmlSecKeysMngrDestroy(mngr);
        return(NULL);
    }    
    
    /* load private RSA key */
    key = xmlSecCryptoAppKeyLoad(key_file, xmlSecKeyDataFormatPem, NULL, NULL, NULL);
    if(key == NULL) {
        fprintf(stderr,"Error: failed to load rsa key from file \"%s\"\n", key_file);
        xmlSecKeysMngrDestroy(mngr);
        return(NULL);
    }

    /* set key name to the file name, this is just an example! */
    if(xmlSecKeySetName(key, BAD_CAST key_file) < 0) {
        fprintf(stderr,"Error: failed to set key name for key from \"%s\"\n", key_file);
        xmlSecKeyDestroy(key);  
        xmlSecKeysMngrDestroy(mngr);
        return(NULL);
    }
        
    /* add key to keys manager, from now on keys manager is responsible 
     * for destroying key 
     */
    if(xmlSecCryptoAppDefaultKeysMngrAdoptKey(mngr, key) < 0) {
        fprintf(stderr,"Error: failed to add key from \"%s\" to keys manager\n", key_file);
        xmlSecKeyDestroy(key);
        xmlSecKeysMngrDestroy(mngr);
        return(NULL);
    }

    return(mngr);
}

/**
 * encrypt_file:
 * @mngr:               the pointer to keys manager.
 * @xml_file:           the encryption template file name.
 * @key_name:           the RSA key name.
 *
 * Encrypts #xml_file using a dynamicaly created template, a session DES key 
 * and an RSA key from keys manager.
 *
 * Returns 0 on success or a negative value if an error occurs.
 */
int 
encrypt_file(xmlSecKeysMngrPtr mngr, const char* xml_file, const char* key_name) {
    xmlDocPtr doc = NULL;
    xmlNodePtr encDataNode = NULL;
    xmlNodePtr keyInfoNode = NULL;
    xmlNodePtr encKeyNode = NULL;
    xmlNodePtr keyInfoNode2 = NULL;
    xmlSecEncCtxPtr encCtx = NULL;
    int res = -1;
    
    assert(mngr);
    assert(xml_file);
    assert(key_name);

    /* load template */
    doc = xmlParseFile(xml_file);
    if ((doc == NULL) || (xmlDocGetRootElement(doc) == NULL)){
        fprintf(stderr, "Error: unable to parse file \"%s\"\n", xml_file);
        goto done;      
    }
    
    /* create encryption template to encrypt XML file and replace 
     * its content with encryption result */
    encDataNode = xmlSecTmplEncDataCreate(doc, xmlSecTransformDes3CbcId,
                                NULL, xmlSecTypeEncElement, NULL, NULL);
    if(encDataNode == NULL) {
        fprintf(stderr, "Error: failed to create encryption template\n");
        goto done;   
    }

    /* we want to put encrypted data in the <enc:CipherValue/> node */
    if(xmlSecTmplEncDataEnsureCipherValue(encDataNode) == NULL) {
        fprintf(stderr, "Error: failed to add CipherValue node\n");
        goto done;   
    }

    /* add <dsig:KeyInfo/> */
    keyInfoNode = xmlSecTmplEncDataEnsureKeyInfo(encDataNode, NULL);
    if(keyInfoNode == NULL) {
        fprintf(stderr, "Error: failed to add key info\n");
        goto done;              
    }

    /* add <enc:EncryptedKey/> to store the encrypted session key */
    encKeyNode = xmlSecTmplKeyInfoAddEncryptedKey(keyInfoNode, 
                                    xmlSecTransformRsaPkcs1Id, 
                                    NULL, NULL, NULL);
    if(encKeyNode == NULL) {
        fprintf(stderr, "Error: failed to add key info\n");
        goto done;              
    }

    /* we want to put encrypted key in the <enc:CipherValue/> node */
    if(xmlSecTmplEncDataEnsureCipherValue(encKeyNode) == NULL) {
        fprintf(stderr, "Error: failed to add CipherValue node\n");
        goto done;   
    }

    /* add <dsig:KeyInfo/> and <dsig:KeyName/> nodes to <enc:EncryptedKey/> */
    keyInfoNode2 = xmlSecTmplEncDataEnsureKeyInfo(encKeyNode, NULL);
    if(keyInfoNode2 == NULL) {
        fprintf(stderr, "Error: failed to add key info\n");
        goto done;              
    }
    
    /* set key name so we can lookup key when needed */
    if(xmlSecTmplKeyInfoAddKeyName(keyInfoNode2, key_name) == NULL) {
        fprintf(stderr, "Error: failed to add key name\n");
        goto done;              
    }

    /* create encryption context */
    encCtx = xmlSecEncCtxCreate(mngr);
    if(encCtx == NULL) {
        fprintf(stderr,"Error: failed to create encryption context\n");
        goto done;
    }

    /* generate a Triple DES key */
    encCtx->encKey = xmlSecKeyGenerate(xmlSecKeyDataDesId, 192, xmlSecKeyDataTypeSession);
    if(encCtx->encKey == NULL) {
        fprintf(stderr,"Error: failed to generate session des key\n");
        goto done;
    }

    /* encrypt the data */
    if(xmlSecEncCtxXmlEncrypt(encCtx, encDataNode, xmlDocGetRootElement(doc)) < 0) {
        fprintf(stderr,"Error: encryption failed\n");
        goto done;
    }
    
    /* we template is inserted in the doc */
    encDataNode = NULL;
        
    /* print encrypted data with document to stdout */
    xmlDocDump(stdout, doc);
    
    /* success */
    res = 0;

done:    

    /* cleanup */
    if(encCtx != NULL) {
        xmlSecEncCtxDestroy(encCtx);
    }

    if(encDataNode != NULL) {
        xmlFreeNode(encDataNode);
    }
        
    if(doc != NULL) {
        xmlFreeDoc(doc); 
    }
    return(res);
}


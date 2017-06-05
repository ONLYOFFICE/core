/** 
 * XML Security Library example: CGI verification script.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 * 
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#include <libxslt/security.h>
#endif /* XMLSEC_NO_XSLT */

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/crypto.h>

/* #define XMLDSIGVERIFY_DEFAULT_TRUSTED_CERTS_FOLDER   "/etc/httpd/conf/ssl.crt" */
#define XMLDSIGVERIFY_DEFAULT_TRUSTED_CERTS_FOLDER      "/var/www/cgi-bin/keys-certs.def"
#define XMLDSIGVERIFY_KEY_AND_CERTS_FOLDER              "/var/www/cgi-bin/keys-certs"


int load_keys(xmlSecKeysMngrPtr mngr, const char* path, int report_loaded_keys);
int load_trusted_certs(xmlSecKeysMngrPtr mngr, const char* path, int report_loaded_certs);
int verify_request(xmlSecKeysMngrPtr mngr);
int url_decode(char *buf, size_t size);

int 
main(int argc, char **argv) {
    xmlSecKeysMngrPtr mngr;
#ifndef XMLSEC_NO_XSLT
    xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif /* XMLSEC_NO_XSLT */
        
    /* start response */
    fprintf(stdout, "Content-type: text/plain\n");
    fprintf(stdout, "\n");
    
    /* Init libxml and libxslt libraries */
    xmlInitParser();
    LIBXML_TEST_VERSION
    xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
    xmlIndentTreeOutput = 1; 
#endif /* XMLSEC_NO_XSLT */
    
    /* make sure that we print out everything to stdout */
    xmlGenericErrorContext = stdout;

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
        fprintf(stdout, "Error: xmlsec initialization failed.\n");
        return(-1);
    }

    /* Check loaded library version */
    if(xmlSecCheckVersion() != 1) {
        fprintf(stdout, "Error: loaded xmlsec library version is not compatible.\n");
        return(-1);
    }

    /* Load default crypto engine if we are supporting dynamic
     * loading for xmlsec-crypto libraries. Use the crypto library
     * name ("openssl", "nss", etc.) to load corresponding 
     * xmlsec-crypto library.
     */
#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
    if(xmlSecCryptoDLLoadLibrary(NULL) < 0) {
        fprintf(stdout, "Error: unable to load default xmlsec-crypto library. Make sure\n"
                        "that you have it installed and check shared libraries path\n"
                        "(LD_LIBRARY_PATH) envornment variable.\n");
        return(-1);     
    }
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

    /* Init crypto library */
    if(xmlSecCryptoAppInit(XMLDSIGVERIFY_DEFAULT_TRUSTED_CERTS_FOLDER) < 0) {
        fprintf(stdout, "Error: crypto initialization failed.\n");
        return(-1);
    }

    /* Init xmlsec-crypto library */
    if(xmlSecCryptoInit() < 0) {
        fprintf(stdout, "Error: xmlsec-crypto initialization failed.\n");
        return(-1);
    }

    /* create keys manager */
    mngr = xmlSecKeysMngrCreate();
    if(mngr == NULL) {
        fprintf(stdout, "Error: failed to create keys manager.\n");
        return(-1);
    }
    if(xmlSecCryptoAppDefaultKeysMngrInit(mngr) < 0) {
        fprintf(stdout, "Error: failed to initialize keys manager.\n");
        return(-1);
    }    

    if(load_keys(mngr, XMLDSIGVERIFY_KEY_AND_CERTS_FOLDER, 0) < 0) {
        xmlSecKeysMngrDestroy(mngr);
        return(-1);
    }
    
    if(load_trusted_certs(mngr, XMLDSIGVERIFY_KEY_AND_CERTS_FOLDER, 0) < 0) {
        xmlSecKeysMngrDestroy(mngr);
        return(-1);
    }

    if(verify_request(mngr) < 0) {
        xmlSecKeysMngrDestroy(mngr);
        return(-1);
    }    

    /* Destroy keys manager */
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
 * load_trusted_certs:
 * @mngr:       the keys manager.
 * @path:       the path to a folder that contains trusted certificates.
 * 
 * Loads trusted certificates from @path.
 *
 * Returns 0 on success or a negative value if an error occurs.
 */ 
int load_trusted_certs(xmlSecKeysMngrPtr mngr, const char* path, int report_loaded_certs) {
    DIR* dir;
    struct dirent* entry;
    char filename[256];
    int len;
    
    assert(mngr);
    assert(path);
    
    dir = opendir(path);
    if(dir == NULL) {
        fprintf(stdout, "Error: failed to open folder \"%s\".\n", path);
        return(-1);
    }    
    while((entry = readdir(dir)) != NULL) {
        assert(entry->d_name);
        len = strlen(entry->d_name);
        if((len > 4) && (strcmp(entry->d_name + len - 4, ".pem") == 0)) {
            snprintf(filename, sizeof(filename), "%s/%s", path, entry->d_name);
            if(xmlSecCryptoAppKeysMngrCertLoad(mngr, filename, xmlSecKeyDataFormatPem, xmlSecKeyDataTypeTrusted) < 0) {
                fprintf(stdout,"Error: failed to load pem certificate from \"%s\"\n", filename);
                closedir(dir);
                return(-1);
            }
            if(report_loaded_certs) {                       
                fprintf(stdout, "Loaded trusted certificate from \"%s\"...\n", filename);
            }
        } else if((len > 4) && (strcmp(entry->d_name + len - 4, ".der") == 0)) {
            snprintf(filename, sizeof(filename), "%s/%s", path, entry->d_name);
            if(xmlSecCryptoAppKeysMngrCertLoad(mngr, filename, xmlSecKeyDataFormatDer, xmlSecKeyDataTypeTrusted) < 0) {
                fprintf(stdout,"Error: failed to load der certificate from \"%s\"\n", filename);
                closedir(dir);
                return(-1);
            }
            if(report_loaded_certs) {                       
                fprintf(stdout, "Loaded trusted certificate from \"%s\"...\n", filename);
            }
        }
    }
    closedir(dir);
    return(0);
}

int load_keys(xmlSecKeysMngrPtr mngr, const char* path, int report_loaded_keys) {
    char filename[256];

    assert(mngr);

    snprintf(filename, sizeof(filename), "%s/keys.xml", path);
    if(xmlSecCryptoAppDefaultKeysMngrLoad(mngr, filename) < 0) {
        fprintf(stdout,"Error: failed to load keys from \"%s\"\n", filename);
        return(-1);
    }
    
    if(report_loaded_keys) {
        fprintf(stdout, "Loaded keys from \"%s\"...\n", filename);
    }
    return(0);
}


/** 
 * verify_request:
 * @mng:                the keys manager
 *
 * Verifies XML signature in the request (stdin).
 *
 * Returns 0 on success or a negative value if an error occurs.
 */
int 
verify_request(xmlSecKeysMngrPtr mngr) {
    xmlBufferPtr buffer = NULL;
    char buf[256];
    xmlDocPtr doc = NULL;
    xmlNodePtr node = NULL;
    xmlSecDSigCtxPtr dsigCtx = NULL;
    int ret;
    int res = -1;
    
    assert(mngr);

    /* load request in the buffer */    
    buffer = xmlBufferCreate();
    if(buffer == NULL) {
        fprintf(stdout,"Error: failed to create buffer\n");
        goto done;      
    }
    
    while(!feof(stdin)) {
        ret = fread(buf, 1, sizeof(buf), stdin);
        if(ret < 0) {
            fprintf(stdout,"Error: read failed\n");
            goto done;  
        }
        xmlBufferAdd(buffer, buf, ret);
    }

    /* is the document subbmitted from the form? */
    if(strncmp((char*)xmlBufferContent(buffer), "_xmldoc=", 8) == 0) {
        xmlBufferShrink(buffer, 8);
        buffer->use = url_decode((char*)xmlBufferContent(buffer), xmlBufferLength(buffer)); 
    }
        
    /** 
     * Load doc 
     */
    doc = xmlReadMemory(xmlBufferContent(buffer), xmlBufferLength(buffer),
                        NULL, NULL,
                        XML_PARSE_NOENT | XML_PARSE_NOCDATA | 
                        XML_PARSE_PEDANTIC | XML_PARSE_NOCDATA);
    if (doc == NULL) {
        fprintf(stdout, "Error: unable to parse xml document (syntax error)\n");
        goto done;
    }
    
    /*
     * Check the document is of the right kind
     */    
    if(xmlDocGetRootElement(doc) == NULL) {
        fprintf(stdout,"Error: empty document\n");
        goto done;
    }
    
    /* find start node */
    node = xmlSecFindNode(xmlDocGetRootElement(doc), xmlSecNodeSignature, xmlSecDSigNs);
    if(node == NULL) {
        fprintf(stdout, "Error: start <dsig:Signature/> node not found\n");
        goto done;      
    }

    /* create signature context */
    dsigCtx = xmlSecDSigCtxCreate(mngr);
    if(dsigCtx == NULL) {
        fprintf(stdout,"Error: failed to create signature context\n");
        goto done;
    }
    
    /* we would like to store and print out everything */
    /* actually we would not because it opens a security hole
    dsigCtx->flags = XMLSEC_DSIG_FLAGS_STORE_SIGNEDINFO_REFERENCES |
                     XMLSEC_DSIG_FLAGS_STORE_MANIFEST_REFERENCES |
                     XMLSEC_DSIG_FLAGS_STORE_SIGNATURE;
    */

    /* Verify signature */
    if(xmlSecDSigCtxVerify(dsigCtx, node) < 0) {
        fprintf(stdout,"Error: signature verification failed\n");
        goto done;
    }
        
    /* print verification result to stdout */
    if(dsigCtx->status == xmlSecDSigStatusSucceeded) {
        fprintf(stdout, "RESULT: Signature is OK\n");
    } else {
        fprintf(stdout, "RESULT: Signature is INVALID\n");
    }    
    fprintf(stdout, "---------------------------------------------------\n");
    xmlSecDSigCtxDebugDump(dsigCtx, stdout);

    /* success */
    res = 0;

done:    
    /* cleanup */
    if(dsigCtx != NULL) {
        xmlSecDSigCtxDestroy(dsigCtx);
    }
    
    if(doc != NULL) {
        xmlFreeDoc(doc); 
    }
    
    if(buffer != NULL) {
        xmlBufferFree(buffer);
    }
    return(res);
}

/* not the best way to do it */
#define toHex(c) ( ( ('0' <= (c)) && ((c) <= '9') ) ? (c) - '0' : \
                 ( ( ('A' <= (c)) && ((c) <= 'F') ) ? (c) - 'A' + 10 : 0 ) )        

/**
 * url_decode:
 * @buf:        the input buffer.
 * @size:       the input buffer size.
 *
 * Does url decoding in-place.
 *               
 * Returns length of the decoded result on success or 
 * a negative value if an error occurs.
 */
int url_decode(char *buf, size_t size) {
    char *p1, *p2;
    
    assert(buf);
    
    p1 = p2 = buf;
    while(p1 - buf < size) {
        if(((*p1) == '%') && ((p1 - buf) <= (size - 3))) {
            *(p2++) = (char)(toHex(p1[1]) * 16 + toHex(p1[2]));
            p1 += 3;        
        } else if((*p1) == '+') {
            *(p2++) = ' ';
            p1++;           
        } else {
            *(p2++) = *(p1++);
        }
    }
    return(p2 - buf);
}



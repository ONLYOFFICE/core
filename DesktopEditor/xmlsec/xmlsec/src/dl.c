/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/app.h>
#include <xmlsec/list.h>
#include <xmlsec/keysdata.h>
#include <xmlsec/keys.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>
#include <xmlsec/private.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/errors.h>
#include <xmlsec/dl.h>

#ifndef XMLSEC_NO_CRYPTO_DYNAMIC_LOADING


#ifdef XMLSEC_DL_LIBLTDL
#include <ltdl.h>
#endif /* XMLSEC_DL_LIBLTDL */

#ifdef XMLSEC_DL_WIN32
#include <windows.h>
#endif /* XMLSEC_DL_WIN32 */

/***********************************************************************
 *
 * loaded libraries list
 *
 **********************************************************************/
typedef struct _xmlSecCryptoDLLibrary                                   xmlSecCryptoDLLibrary,
                                                                        *xmlSecCryptoDLLibraryPtr;
struct _xmlSecCryptoDLLibrary {
    xmlChar*    name;
    xmlChar*    filename;
    xmlChar*    getFunctionsName;
    xmlSecCryptoDLFunctionsPtr functions;

#ifdef XMLSEC_DL_LIBLTDL
    lt_dlhandle handle;
#endif /* XMLSEC_DL_LIBLTDL */

#ifdef XMLSEC_DL_WIN32
    HINSTANCE   handle;
#endif /* XMLSEC_DL_WIN32 */
};

static xmlSecCryptoDLLibraryPtr xmlSecCryptoDLLibraryCreate             (const xmlChar* name);
static void                     xmlSecCryptoDLLibraryDestroy            (xmlSecCryptoDLLibraryPtr lib);
static xmlSecCryptoDLLibraryPtr xmlSecCryptoDLLibraryDuplicate          (xmlSecCryptoDLLibraryPtr lib);
static xmlChar*                 xmlSecCryptoDLLibraryConstructFilename  (const xmlChar* name);
static xmlChar*                 xmlSecCryptoDLLibraryConstructGetFunctionsName(const xmlChar* name);


static xmlSecPtrListKlass xmlSecCryptoDLLibrariesListKlass = {
    BAD_CAST "dl-libraries-list",
    (xmlSecPtrDuplicateItemMethod)xmlSecCryptoDLLibraryDuplicate,/* xmlSecPtrDuplicateItemMethod duplicateItem; */
    (xmlSecPtrDestroyItemMethod)xmlSecCryptoDLLibraryDestroy,   /* xmlSecPtrDestroyItemMethod destroyItem; */
    NULL,                                                       /* xmlSecPtrDebugDumpItemMethod debugDumpItem; */
    NULL,                                                       /* xmlSecPtrDebugDumpItemMethod debugXmlDumpItem; */
};
static xmlSecPtrListId          xmlSecCryptoDLLibrariesListGetKlass     (void);
static int                      xmlSecCryptoDLLibrariesListFindByName   (xmlSecPtrListPtr list,
                                                                         const xmlChar* name);

typedef xmlSecCryptoDLFunctionsPtr xmlSecCryptoGetFunctionsCallback(void);

/* conversion from ptr to func "the right way" */
XMLSEC_PTR_TO_FUNC_IMPL(xmlSecCryptoGetFunctionsCallback)


static xmlSecCryptoDLLibraryPtr
xmlSecCryptoDLLibraryCreate(const xmlChar* name) {
    xmlSecCryptoDLLibraryPtr lib;
    xmlSecCryptoGetFunctionsCallback * getFunctions = NULL;

    xmlSecAssert2(name != NULL, NULL);

    /* fprintf (stderr, "loading \"library %s\"...\n", name); */

    /* Allocate a new xmlSecCryptoDLLibrary and fill the fields. */
    lib = (xmlSecCryptoDLLibraryPtr)xmlMalloc(sizeof(xmlSecCryptoDLLibrary));
    if(lib == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", (int)sizeof(lib));
        return(NULL);
    }
    memset(lib, 0, sizeof(xmlSecCryptoDLLibrary));

    lib->name = xmlStrdup(name);
    if(lib->name == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "xmlStrdup",
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    lib->filename = xmlSecCryptoDLLibraryConstructFilename(name);
    if(lib->filename == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "xmlSecCryptoDLLibraryConstructFilename",
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    lib->getFunctionsName = xmlSecCryptoDLLibraryConstructGetFunctionsName(name);
    if(lib->getFunctionsName == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "xmlSecCryptoDLLibraryConstructGetFunctionsName",
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

#ifdef XMLSEC_DL_LIBLTDL
    lib->handle = lt_dlopenext((char*)lib->filename);
    if(lib->handle == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "lt_dlopenext",
                    NULL,
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(lib->filename));
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    getFunctions = XMLSEC_PTR_TO_FUNC(xmlSecCryptoGetFunctionsCallback,
                        lt_dlsym(lib->handle, (char*)lib->getFunctionsName)
                    );
    if(getFunctions == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "lt_dlsym",
                    NULL,
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "function=%s",
                    xmlSecErrorsSafeString(lib->getFunctionsName));
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }
#endif /* XMLSEC_DL_LIBLTDL */

#ifdef XMLSEC_DL_WIN32
    lib->handle = LoadLibraryA((char*)lib->filename);
    if(lib->handle == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "LoadLibraryA",
                    NULL,
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "filename=%s",
                    xmlSecErrorsSafeString(lib->filename));
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    getFunctions = XMLSEC_PTR_TO_FUNC(xmlSecCryptoGetFunctionsCallback,
                        GetProcAddress(
                            lib->handle,
                            (const char*)lib->getFunctionsName
                        )
                    );
    if(getFunctions == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "GetProcAddressA",
                    NULL,
                    XMLSEC_ERRORS_R_IO_FAILED,
                    "function=%s",
                    xmlSecErrorsSafeString(lib->getFunctionsName));
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }
#endif /* XMLSEC_DL_WIN32 */

    if(getFunctions == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "invalid configuration: no way to load library");
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    lib->functions = getFunctions();
    if(lib->functions == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    "getFunctions",
                    NULL,
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    /* fprintf (stderr, "library %s loaded\n", name); */
    return(lib);
}

static void
xmlSecCryptoDLLibraryDestroy(xmlSecCryptoDLLibraryPtr lib) {
    xmlSecAssert(lib != NULL);

    /* fprintf (stderr, "unloading \"library %s\"...\n", lib->name); */
    if(lib->name != NULL) {
        xmlFree(lib->name);
    }

    if(lib->filename != NULL) {
        xmlFree(lib->filename);
    }

    if(lib->getFunctionsName != NULL) {
        xmlFree(lib->getFunctionsName);
    }

#ifdef XMLSEC_DL_LIBLTDL
    if(lib->handle != NULL) {
        int ret;

        ret = lt_dlclose(lib->handle);
        if(ret != 0) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        "lt_dlclose",
                        NULL,
                        XMLSEC_ERRORS_R_IO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
        }
    }
#endif /* XMLSEC_DL_LIBLTDL */

#ifdef XMLSEC_DL_WIN32
    if(lib->handle != NULL) {
        BOOL res;

        res = FreeLibrary(lib->handle);
        if(!res) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        "FreeLibrary",
                        NULL,
                        XMLSEC_ERRORS_R_IO_FAILED,
                        XMLSEC_ERRORS_NO_MESSAGE);
        }
        }
#endif /* XMLSEC_DL_WIN32*/

    memset(lib, 0, sizeof(xmlSecCryptoDLLibrary));
    xmlFree(lib);
}

static xmlSecCryptoDLLibraryPtr
xmlSecCryptoDLLibraryDuplicate(xmlSecCryptoDLLibraryPtr lib) {
    xmlSecAssert2(lib != NULL, NULL);
    xmlSecAssert2(lib->name != NULL, NULL);

    return(xmlSecCryptoDLLibraryCreate(lib->name));
}

static xmlChar*
xmlSecCryptoDLLibraryConstructFilename(const xmlChar* name) {
    static xmlChar tmpl[] = "lib%s-%s";
    xmlChar* res;
    int len;

    xmlSecAssert2(name != NULL, NULL);

    /* TODO */
    len = xmlStrlen(BAD_CAST PACKAGE) + xmlStrlen(name) + xmlStrlen(tmpl) + 1;
    res = (xmlChar*)xmlMalloc(len + 1);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", len + 1);
        return(NULL);
    }
    xmlSecStrPrintf(res, len, tmpl, PACKAGE, name);

    return(res);
}

static xmlChar*
xmlSecCryptoDLLibraryConstructGetFunctionsName(const xmlChar* name) {
    static xmlChar tmpl[] = "xmlSecCryptoGetFunctions_%s";
    xmlChar* res;
    int len;

    xmlSecAssert2(name != NULL, NULL);

    len = xmlStrlen(name) + xmlStrlen(tmpl) + 1;
    res = (xmlChar*)xmlMalloc(len + 1);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    NULL,
                    XMLSEC_ERRORS_R_MALLOC_FAILED,
                    "size=%d", len + 1);
        return(NULL);
    }
    xmlSecStrPrintf(res, len, tmpl, name);

    return(res);
}

static xmlSecPtrListId
xmlSecCryptoDLLibrariesListGetKlass(void) {
    return(&xmlSecCryptoDLLibrariesListKlass);
}

static int
xmlSecCryptoDLLibrariesListFindByName(xmlSecPtrListPtr list, const xmlChar* name) {
    xmlSecSize i, size;
    xmlSecCryptoDLLibraryPtr lib;

    xmlSecAssert2(xmlSecPtrListCheckId(list, xmlSecCryptoDLLibrariesListGetKlass()), -1);
    xmlSecAssert2(name != NULL, -1);

    size = xmlSecPtrListGetSize(list);
    for(i = 0; i < size; ++i) {
        lib = (xmlSecCryptoDLLibraryPtr)xmlSecPtrListGetItem(list, i);
        if((lib != NULL) && (lib->name != NULL) && (xmlStrcmp(lib->name, name) == 0)) {
            return(i);
        }
    }
    return(-1);
}

/******************************************************************************
 *
 * Dynamic load functions
 *
 *****************************************************************************/
static xmlSecCryptoDLFunctionsPtr gXmlSecCryptoDLFunctions = NULL;
static xmlSecPtrList gXmlSecCryptoDLLibraries;

/**
 * xmlSecCryptoDLInit:
 *
 * Initializes dynamic loading engine. This is an internal function
 * and should not be called by application directly.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecCryptoDLInit(void) {
    int ret;

    ret = xmlSecPtrListInitialize(&gXmlSecCryptoDLLibraries, xmlSecCryptoDLLibrariesListGetKlass());
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListPtrInitialize",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "xmlSecCryptoDLLibrariesListGetKlass");
        return(-1);
    }

#ifdef XMLSEC_DL_LIBLTDL
    ret = lt_dlinit ();
    if(ret != 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "lt_dlinit",
                    XMLSEC_ERRORS_R_IO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
#endif /* XMLSEC_DL_LIBLTDL */

    return(0);
}


/**
 * xmlSecCryptoDLShutdown:
 *
 * Shutdowns dynamic loading engine. This is an internal function
 * and should not be called by application directly.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecCryptoDLShutdown(void) {
    int ret;

    xmlSecPtrListFinalize(&gXmlSecCryptoDLLibraries);

#ifdef XMLSEC_DL_LIBLTDL
    ret = lt_dlexit ();
    if(ret != 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "lt_dlexit",
                    XMLSEC_ERRORS_R_IO_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
    }
#endif /* XMLSEC_DL_LIBLTDL */

    return(0);
}

/**
 * xmlSecCryptoDLLoadLibrary:
 * @crypto:             the desired crypto library name ("openssl", "nss", ...). If NULL
 *                      then the default crypto engine will be used.
 *
 * Loads the xmlsec-$crypto library. This function is NOT thread safe,
 * application MUST NOT call #xmlSecCryptoDLLoadLibrary, #xmlSecCryptoDLGetLibraryFunctions,
 * and #xmlSecCryptoDLUnloadLibrary functions from multiple threads.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecCryptoDLLoadLibrary(const xmlChar* crypto) {
    xmlSecCryptoDLFunctionsPtr functions;
    int ret;

    /* if crypto is not specified, then used default */
    functions = xmlSecCryptoDLGetLibraryFunctions((crypto != NULL ) ? crypto : xmlSecGetDefaultCrypto());
    if(functions == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCryptoDLGetLibraryFunctions",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecCryptoDLSetFunctions(functions);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCryptoDLSetFunctions",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecCryptoDLGetLibraryFunctions:
 * @crypto:             the desired crypto library name ("openssl", "nss", ...).
 *
 * Loads the xmlsec-$crypto library and gets global crypto functions/transforms/keys data/keys store
 * table. This function is NOT thread safe, application MUST NOT call #xmlSecCryptoDLLoadLibrary,
 * #xmlSecCryptoDLGetLibraryFunctions, and #xmlSecCryptoDLUnloadLibrary functions from multiple threads.
 *
 * Returns: the table or NULL if an error occurs.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoDLGetLibraryFunctions(const xmlChar* crypto) {
    xmlSecCryptoDLLibraryPtr lib;
    int pos;
    int ret;

    xmlSecAssert2(crypto != NULL, NULL);

    pos = xmlSecCryptoDLLibrariesListFindByName(&gXmlSecCryptoDLLibraries, crypto);
    if(pos >= 0) {
        lib = (xmlSecCryptoDLLibraryPtr)xmlSecPtrListGetItem(&gXmlSecCryptoDLLibraries, pos);
        xmlSecAssert2(lib != NULL, NULL);
        xmlSecAssert2(lib->functions != NULL, NULL);

        return(lib->functions);
    }

    lib = xmlSecCryptoDLLibraryCreate(crypto);
    if(lib == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecCryptoDLLibraryCreate",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "crypto=%s",
                    xmlSecErrorsSafeString(crypto));
        return(NULL);
    }

    ret = xmlSecPtrListAdd(&gXmlSecCryptoDLLibraries, lib);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListAdd",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "crypto=%s",
                    xmlSecErrorsSafeString(crypto));
        xmlSecCryptoDLLibraryDestroy(lib);
        return(NULL);
    }

    return(lib->functions);
}

/**
 * xmlSecCryptoDLUnloadLibrary:
 * @crypto:             the desired crypto library name ("openssl", "nss", ...).
 *
 * Unloads the xmlsec-$crypto library. All pointers to this library
 * functions tables became invalid. This function is NOT thread safe,
 * application MUST NOT call #xmlSecCryptoDLLoadLibrary, #xmlSecCryptoDLGetLibraryFunctions,
 * and #xmlSecCryptoDLUnloadLibrary functions from multiple threads.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecCryptoDLUnloadLibrary(const xmlChar* crypto) {
    xmlSecCryptoDLLibraryPtr lib;
    int pos;
    int ret;

    xmlSecAssert2(crypto != NULL, -1);

    pos = xmlSecCryptoDLLibrariesListFindByName(&gXmlSecCryptoDLLibraries, crypto);
    if(pos < 0) {
        /* todo: is it an error? */
        return(0);
    }

    lib = (xmlSecCryptoDLLibraryPtr)xmlSecPtrListGetItem(&gXmlSecCryptoDLLibraries, pos);
    if((lib != NULL) && (lib->functions == gXmlSecCryptoDLFunctions)) {
        gXmlSecCryptoDLFunctions = NULL;
    }

    ret = xmlSecPtrListRemove(&gXmlSecCryptoDLLibraries, pos);
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecPtrListRemove",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    return(0);
}

/**
 * xmlSecCryptoDLSetFunctions:
 * @functions:          the new table
 *
 * Sets global crypto functions/transforms/keys data/keys store table.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecCryptoDLSetFunctions(xmlSecCryptoDLFunctionsPtr functions) {
    xmlSecAssert2(functions != NULL, -1);

    gXmlSecCryptoDLFunctions = functions;

    return(0);
}

/**
 * xmlSecCryptoDLGetFunctions:
 *
 * Gets global crypto functions/transforms/keys data/keys store table.
 *
 * Returns: the table.
 */
xmlSecCryptoDLFunctionsPtr
xmlSecCryptoDLGetFunctions(void) {
    return(gXmlSecCryptoDLFunctions);
}

#endif /* XMLSEC_NO_CRYPTO_DYNAMIC_LOADING */

/**
 * xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms:
 * @functions:          the functions table.
 *
 * Registers the key data and transforms klasses from @functions table in xmlsec.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecCryptoDLFunctionsRegisterKeyDataAndTransforms(struct _xmlSecCryptoDLFunctions* functions) {
    xmlSecAssert2(functions != NULL, -1);

    /****************************************************************************
     *
     * Register keys
     *
     ****************************************************************************/
    if((functions->keyDataAesGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataAesGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataAesGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataDesGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataDesGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataDesGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataDsaGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataDsaGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataDsaGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataEcdsaGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataEcdsaGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataEcdsaGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataGost2001GetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataGost2001GetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataGost2001GetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataGostR3410_2012_256GetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataGostR3410_2012_256GetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataGostR3410_2012_256GetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataGostR3410_2012_512GetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataGostR3410_2012_512GetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataGostR3410_2012_512GetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }    if((functions->keyDataHmacGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataHmacGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataHmacGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataRsaGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataRsaGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataRsaGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataX509GetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataX509GetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataX509GetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->keyDataRawX509CertGetKlass != NULL) && (xmlSecKeyDataIdsRegister(functions->keyDataRawX509CertGetKlass()) < 0)) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecKeyDataKlassGetName(functions->keyDataRawX509CertGetKlass())),
                    "xmlSecKeyDataIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }


    /****************************************************************************
     *
     * Register transforms
     *
     ****************************************************************************/
    if((functions->transformAes128CbcGetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformAes128CbcGetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformAes128CbcGetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformAes192CbcGetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformAes192CbcGetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformAes192CbcGetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformAes256CbcGetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformAes256CbcGetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformAes256CbcGetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformKWAes128GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformKWAes128GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformKWAes128GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformKWAes192GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformKWAes192GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformKWAes192GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformKWAes256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformKWAes256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformKWAes256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformDes3CbcGetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformDes3CbcGetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformDes3CbcGetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformKWDes3GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformKWDes3GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformKWDes3GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformGost2001GostR3411_94GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformGost2001GostR3411_94GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformGost2001GostR3411_94GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformGostR3410_2012GostR3411_2012_256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformGostR3410_2012GostR3411_2012_256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformGostR3410_2012GostR3411_2012_256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformGostR3410_2012GostR3411_2012_512GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformGostR3410_2012GostR3411_2012_512GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformGostR3410_2012GostR3411_2012_512GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformDsaSha1GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformDsaSha1GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformDsaSha1GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformDsaSha256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformDsaSha256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformDsaSha256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformEcdsaSha1GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformEcdsaSha1GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformEcdsaSha1GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformEcdsaSha224GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformEcdsaSha224GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformEcdsaSha224GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformEcdsaSha256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformEcdsaSha256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformEcdsaSha256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformEcdsaSha384GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformEcdsaSha384GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformEcdsaSha384GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformEcdsaSha512GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformEcdsaSha512GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformEcdsaSha512GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacMd5GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacMd5GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacMd5GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacRipemd160GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacRipemd160GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacRipemd160GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacSha1GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacSha1GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacSha1GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacSha224GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacSha224GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacSha224GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacSha256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacSha256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacSha256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacSha384GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacSha384GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacSha384GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformHmacSha512GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformHmacSha512GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformHmacSha512GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformMd5GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformMd5GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformMd5GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRipemd160GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRipemd160GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRipemd160GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaMd5GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaMd5GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaMd5GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaRipemd160GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaRipemd160GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaRipemd160GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaSha1GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaSha1GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaSha1GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaSha224GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaSha224GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaSha224GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaSha256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaSha256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaSha256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaSha384GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaSha384GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaSha384GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaSha512GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaSha512GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaSha512GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaPkcs1GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaPkcs1GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaPkcs1GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformRsaOaepGetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformRsaOaepGetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformRsaOaepGetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformGostR3411_94GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformGostR3411_94GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformGostR3411_94GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformGostR3411_2012_256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformGostR3411_2012_256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformGostR3411_2012_256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformGostR3411_2012_512GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformGostR3411_2012_512GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformGostR3411_2012_512GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    if((functions->transformSha1GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformSha1GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformSha1GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformSha224GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformSha224GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformSha224GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformSha256GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformSha256GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformSha256GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformSha384GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformSha384GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformSha384GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    if((functions->transformSha512GetKlass != NULL) && xmlSecTransformIdsRegister(functions->transformSha512GetKlass()) < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecTransformKlassGetName(functions->transformSha512GetKlass())),
                    "xmlSecTransformIdsRegister",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    /* done */
    return(0);
}



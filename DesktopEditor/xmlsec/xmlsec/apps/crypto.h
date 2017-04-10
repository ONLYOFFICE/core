/** 
 * XMLSec library
 *
 *
 * See Copyright for the status of this software.
 * 
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_APPS_CRYPTO_H__
#define __XMLSEC_APPS_CRYPTO_H__    

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */ 

#include <libxml/tree.h>
#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/crypto.h>

int     xmlSecAppCryptoInit                                     (const char* config);
int     xmlSecAppCryptoShutdown                                 (void);

xmlSecKeyPtr xmlSecAppCryptoKeyGenerate                         (const char* keyKlassAndSize,
                                                                 const char* name,
                                                                 xmlSecKeyDataType type);

/*****************************************************************************
 *
 * Simple keys manager
 *
 ****************************************************************************/
int     xmlSecAppCryptoSimpleKeysMngrInit                       (xmlSecKeysMngrPtr mngr);
int     xmlSecAppCryptoSimpleKeysMngrLoad                       (xmlSecKeysMngrPtr mngr, 
                                                                 const char *filename);
int     xmlSecAppCryptoSimpleKeysMngrSave                       (xmlSecKeysMngrPtr mngr, 
                                                                 const char *filename,
                                                                 xmlSecKeyDataType type);
int     xmlSecAppCryptoSimpleKeysMngrCertLoad                   (xmlSecKeysMngrPtr mngr, 
                                                                 const char *filename, 
                                                                 xmlSecKeyDataFormat format,
                                                                 xmlSecKeyDataType type);
int     xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad            (xmlSecKeysMngrPtr mngr, 
                                                                 const char *files, 
                                                                 const char* pwd, 
                                                                 const char* name,
                                                                 xmlSecKeyDataFormat format);
int     xmlSecAppCryptoSimpleKeysMngrPkcs12KeyLoad              (xmlSecKeysMngrPtr mngr, 
                                                                 const char *filename, 
                                                                 const char* pwd, 
                                                                 const char *name);
int     xmlSecAppCryptoSimpleKeysMngrBinaryKeyLoad              (xmlSecKeysMngrPtr mngr, 
                                                                 const char* keyKlass, 
                                                                 const char* filename, 
                                                                 const char *name);
int     xmlSecAppCryptoSimpleKeysMngrKeyGenerate                (xmlSecKeysMngrPtr mngr, 
                                                                 const char* keyKlassAndSize,
                                                                 const char* name);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_APPS_CRYPTO_H__ */




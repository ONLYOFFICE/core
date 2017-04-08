/**
 * XMLSec library
 *
 * MSCrypto keys store
 *
 * This is free software; see Copyright file in the source
 * distribution for precise wording.
 *
 * Copyright (C) 2003 Cordys R&D BV, All rights reserved.
 */
#ifndef __XMLSEC_MSCRYPTO_KEYSSTORE_H__
#define __XMLSEC_MSCRYPTO_KEYSSTORE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <xmlsec/xmlsec.h>

/****************************************************************************
 *
 * MSCrypto Keys Store
 *
 ***************************************************************************/
/**
 * xmlSecMSCryptoKeysStoreId:
 *
 * A MSCrypto keys store klass id.
 */
#define xmlSecMSCryptoKeysStoreId       xmlSecMSCryptoKeysStoreGetKlass()
XMLSEC_CRYPTO_EXPORT xmlSecKeyStoreId   xmlSecMSCryptoKeysStoreGetKlass (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeysStoreAdoptKey (xmlSecKeyStorePtr store,
                                                                         xmlSecKeyPtr key);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeysStoreLoad     (xmlSecKeyStorePtr store,
                                                                         const char *uri,
                                                                         xmlSecKeysMngrPtr keysMngr);
XMLSEC_CRYPTO_EXPORT int                xmlSecMSCryptoKeysStoreSave     (xmlSecKeyStorePtr store,
                                                                         const char *filename,
                                                                         xmlSecKeyDataType type);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_MSCRYPTO_KEYSSTORE_H__ */


/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (c) 2003 America Online, Inc.  All rights reserved.
 */
#ifndef __XMLSEC_NSS_PKIKEYS_H__
#define __XMLSEC_NSS_PKIKEYS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <nspr.h>
#include <nss.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>


XMLSEC_CRYPTO_EXPORT xmlSecKeyDataPtr   xmlSecNssPKIAdoptKey           (SECKEYPrivateKey *privkey,
                                                                        SECKEYPublicKey  *pubkey);

XMLSEC_CRYPTO_EXPORT SECKEYPublicKey*   xmlSecNssPKIKeyDataGetPubKey    (xmlSecKeyDataPtr data);

XMLSEC_CRYPTO_EXPORT SECKEYPrivateKey*  xmlSecNssPKIKeyDataGetPrivKey   (xmlSecKeyDataPtr data);

XMLSEC_CRYPTO_EXPORT KeyType            xmlSecNssPKIKeyDataGetKeyType   (xmlSecKeyDataPtr data);

XMLSEC_CRYPTO_EXPORT int                xmlSecNssPKIKeyDataDuplicate    (xmlSecKeyDataPtr dst,
                                                                         xmlSecKeyDataPtr src);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_NSS_PKIKEYS_H__ */



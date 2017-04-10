/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_SKELETON_CRYPTO_H__
#define __XMLSEC_SKELETON_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <xmlsec/xmlsec.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/dl.h>

XMLSEC_CRYPTO_EXPORT xmlSecCryptoDLFunctionsPtr xmlSecCryptoGetFunctions_skeleton(void);

/********************************************************************
 *
 * Init shutdown
 *
 ********************************************************************/
XMLSEC_CRYPTO_EXPORT int                xmlSecSkeletonInit              (void);
XMLSEC_CRYPTO_EXPORT int                xmlSecSkeletonShutdown          (void);

XMLSEC_CRYPTO_EXPORT int                xmlSecSkeletonKeysMngrInit      (xmlSecKeysMngrPtr mngr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_SKELETON_CRYPTO_H__ */

#define __XMLSEC_SKELETON_CRYPTO_H__

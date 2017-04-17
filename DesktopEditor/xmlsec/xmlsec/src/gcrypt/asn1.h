/*
 * XML Security Library
 *
 * gcrypt/asn1.h: internal header only used during the compilation
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GCRYPT_ASN1_H__
#define __XMLSEC_GCRYPT_ASN1_H__

#ifndef XMLSEC_PRIVATE
#error "gcrypt/asn1.h file contains private xmlsec-gcrypt definitions and should not be used outside xmlsec or xmlsec-$crypto libraries"
#endif /* XMLSEC_PRIVATE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum xmlSecGCryptDerKeyType {
    xmlSecGCryptDerKeyTypeAuto = 0,
    xmlSecGCryptDerKeyTypePublicDsa,
    xmlSecGCryptDerKeyTypePublicRsa,
    xmlSecGCryptDerKeyTypePrivateDsa,
    xmlSecGCryptDerKeyTypePrivateRsa
};

xmlSecKeyDataPtr        xmlSecGCryptParseDer            (const xmlSecByte * der,
                                                         xmlSecSize derlen,
                                                         enum xmlSecGCryptDerKeyType type);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__XMLSEC_GCRYPT_ASN1_H__ */

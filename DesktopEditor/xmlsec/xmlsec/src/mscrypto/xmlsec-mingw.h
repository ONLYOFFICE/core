/**
 * XMLSec library
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2007,2010 Roumen Petrov.
 */

#ifndef __XMLSEC_MSCRYPTO_XMLSEC_MINGW_H__
#define __XMLSEC_MSCRYPTO_XMLSEC_MINGW_H__

#ifndef XMLSEC_PRIVATE
#error "xmlsec-mingw.h file contains private xmlsec definitions for mingw build and should not be used outside xmlsec or xmlsec-$crypto libraries"
#endif /* XMLSEC_PRIVATE */


/*defines*/

#ifndef ALG_SID_HMAC
#  define ALG_SID_HMAC          9
#endif

#ifndef ALG_SID_SHA_256
#  define ALG_SID_SHA_256       12
#endif

#ifndef ALG_SID_SHA_384
#  define ALG_SID_SHA_384       13
#endif

#ifndef ALG_SID_SHA_512
#  define ALG_SID_SHA_512       14
#endif

#ifndef CALG_HMAC
#  define CALG_HMAC             (ALG_CLASS_HASH|ALG_TYPE_ANY|ALG_SID_HMAC)
#endif

#ifndef CALG_SHA_256
#  define CALG_SHA_256          (ALG_CLASS_HASH|ALG_TYPE_ANY|ALG_SID_SHA_256)
#endif

#ifndef CALG_SHA_384
#  define CALG_SHA_384          (ALG_CLASS_HASH|ALG_TYPE_ANY|ALG_SID_SHA_384)
#endif

#ifndef CALG_SHA_512
#  define CALG_SHA_512          (ALG_CLASS_HASH|ALG_TYPE_ANY|ALG_SID_SHA_512)
#endif


#ifndef KP_OAEP_PARAMS
#  define KP_OAEP_PARAMS        36
#endif


#ifndef CERT_CLOSE_STORE_FORCE_FLAG
#  define CERT_CLOSE_STORE_FORCE_FLAG           1
#endif

#ifndef CERT_CLOSE_STORE_CHECK_FLAG
#  define CERT_CLOSE_STORE_CHECK_FLAG           2
#endif


#ifndef CERT_PHYSICAL_STORE_ADD_ENABLE_FLAG
#  define CERT_PHYSICAL_STORE_ADD_ENABLE_FLAG   1
#endif


#ifndef CERT_STORE_ADD_ALWAYS
#  define CERT_STORE_ADD_ALWAYS                 4
#endif

#ifndef CERT_STORE_CREATE_NEW_FLAG
#  define CERT_STORE_CREATE_NEW_FLAG            (2<<12)
#endif

#ifndef CERT_STORE_SIGNATURE_FLAG
#  define CERT_STORE_SIGNATURE_FLAG             1
#endif

#ifndef CERT_STORE_TIME_VALIDITY_FLAG
#  define CERT_STORE_TIME_VALIDITY_FLAG         2
#endif

#ifndef CERT_STORE_REVOCATION_FLAG
#  define CERT_STORE_REVOCATION_FLAG            4
#endif

#ifndef CERT_STORE_NO_CRL_FLAG
#  define CERT_STORE_NO_CRL_FLAG                (1<<16)
#endif


#ifndef CERT_STORE_PROV_COLLECTION
#  define CERT_STORE_PROV_COLLECTION            ((LPCSTR) 11)
#endif

#ifndef CERT_STORE_PROV_MEMORY
#  define CERT_STORE_PROV_MEMORY                ((LPCSTR) 2)
#endif


#ifndef CERT_KEY_SPEC_PROP_ID
#  define CERT_KEY_SPEC_PROP_ID                 6
#endif

#ifndef CERT_FRIENDLY_NAME_PROP_ID
#  define CERT_FRIENDLY_NAME_PROP_ID            11
#endif

#ifndef CERT_KEY_IDENTIFIER_PROP_ID
#  define CERT_KEY_IDENTIFIER_PROP_ID           20
#endif


#ifndef CERT_NAME_ISSUER_FLAG
#  define CERT_NAME_ISSUER_FLAG                 1
#endif

#ifndef CERT_NAME_RDN_TYPE
#  define CERT_NAME_RDN_TYPE                    2
#endif

#ifndef CERT_NAME_STR_ENABLE_UTF8_UNICODE_FLAG
#  define CERT_NAME_STR_ENABLE_UTF8_UNICODE_FLAG        (4<<16)
#endif


#ifndef CERT_CHAIN_REVOCATION_CHECK_CHAIN
#  define CERT_CHAIN_REVOCATION_CHECK_CHAIN                     (2<<28)
#endif

#ifndef CERT_CHAIN_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT
#  define CERT_CHAIN_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT        (4<<28)
#endif


#ifndef szOID_SUBJECT_KEY_IDENTIFIER
#  define szOID_SUBJECT_KEY_IDENTIFIER          "2.5.29.14"
#endif


#ifndef CRYPT_ACQUIRE_COMPARE_KEY_FLAG
#  define CRYPT_ACQUIRE_COMPARE_KEY_FLAG        4
#endif


/*structures/types*/
typedef struct _PUBKEY {
        DWORD magic;
        DWORD bitlen;
} DSSPUBKEY;

typedef struct _DSSSEED {
        DWORD   counter;
        BYTE    seed[20];
} DSSSEED;


typedef struct _PROV_ENUMALGS_EX {
        ALG_ID aiAlgid;
        DWORD dwDefaultLen;
        DWORD dwMinLen;
        DWORD dwMaxLen;
        DWORD dwProtocols;
        DWORD dwNameLen;
        CHAR szName[20];
        DWORD dwLongNameLen;
        CHAR szLongName[40];
} PROV_ENUMALGS_EX;


/*methods(functions)*/
DWORD           WINAPI CertGetPublicKeyLength(DWORD,PCERT_PUBLIC_KEY_INFO);

BOOL            WINAPI CertStrToNameA(DWORD,LPCSTR,DWORD,void*,BYTE*,DWORD*,LPCSTR*);
BOOL            WINAPI CertStrToNameW(DWORD,LPCWSTR,DWORD,void*,BYTE*,DWORD*,LPCWSTR*);
#ifdef UNICODE
#define CertStrToName  CertStrToNameW
#else
#define CertStrToName  CertStrToNameA
#endif


BOOL            WINAPI CertCompareCertificateName(DWORD,PCERT_NAME_BLOB,PCERT_NAME_BLOB);

BOOL            WINAPI CertAddStoreToCollection(HCERTSTORE,HCERTSTORE,DWORD,DWORD);

PCCERT_CONTEXT  WINAPI CertCreateCertificateContext(DWORD,const BYTE*,DWORD);
BOOL            WINAPI CertGetCertificateContextProperty(PCCERT_CONTEXT,DWORD,void*,DWORD*);
BOOL            WINAPI CertVerifySubjectCertificateContext(PCCERT_CONTEXT,PCCERT_CONTEXT,DWORD*);

BOOL            WINAPI CertAddCRLContextToStore(HCERTSTORE,PCCRL_CONTEXT,DWORD,PCCRL_CONTEXT*);
PCCRL_CONTEXT   WINAPI CertDuplicateCRLContext(PCCRL_CONTEXT);
BOOL            WINAPI CertFreeCRLContext(PCCRL_CONTEXT);

BOOL            WINAPI CertFindCertificateInCRL(PCCERT_CONTEXT,PCCRL_CONTEXT,DWORD,void*,PCRL_ENTRY*);
PCCRL_CONTEXT   WINAPI CertEnumCRLsInStore(HCERTSTORE,PCCRL_CONTEXT);

PCCRL_CONTEXT   WINAPI CertCreateCRLContext(DWORD,const BYTE*,DWORD);

BOOL            WINAPI CryptAcquireCertificatePrivateKey(PCCERT_CONTEXT,DWORD,void*,HCRYPTPROV*,DWORD*,BOOL*);
BOOL            WINAPI CryptDuplicateKey(HCRYPTKEY,DWORD*,DWORD,HCRYPTKEY*);
BOOL            WINAPI CryptImportPublicKeyInfo(HCRYPTPROV,DWORD,PCERT_PUBLIC_KEY_INFO,HCRYPTKEY*);


#endif /*ndef __XMLSEC_MSCRYPTO_XMLSEC_MINGW_H__*/

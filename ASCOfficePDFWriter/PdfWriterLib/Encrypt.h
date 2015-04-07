//----------------------------------------------------------------------------------------
//
// The code implements MD5 message-digest algorithm.
// To compute the message digest of a chunk of bytes, declare an
// MD5Context structure, pass it to MD5Init, call MD5Update as
// needed on buffers full of bytes, and then call MD5Final, which
// will fill a supplied 16-byte array with the digest.
//
//----------------------------------------------------------------------------------------

#ifndef _ENCRYPT_H
#define _ENCRYPT_H

#pragma once

#include "Utils.h"
#include "MemoryManager.h"

static const BYTE c_sPaddingString[] = 
{
    0x28, 0xBF, 0x4E, 0x5E, 0x4E, 0x75, 0x8A, 0x41,
    0x64, 0x00, 0x4E, 0x56, 0xFF, 0xFA, 0x01, 0x08,
    0x2E, 0x2E, 0x00, 0xB6, 0xD0, 0x68, 0x3E, 0x80,
    0x2F, 0x0C, 0xA9, 0xFE, 0x64, 0x53, 0x69, 0x7A
};

//-------- EncryptDict -------------------------------------------------------------------

#define ID_LEN              16
#define PASSWD_LEN          32
#define ENCRYPT_KEY_MAX     16
#define MD5_KEY_LEN         16
#define PERMISSION_PAD      0xFFFFFFC0
#define ARC4_BUF_SIZE       256


typedef struct TMD5Context
{
    unsigned int anBuf[4];
    unsigned int anBits[2];
    BYTE         anIn[64];
} MD5Context;

typedef struct TARC4ContextRec
{
    BYTE    nIndex1;
    BYTE    nIndex2;
    BYTE    anState[ARC4_BUF_SIZE];
} ARC4ContextRec;

typedef struct EncryptRec* EncryptRecPtr;

struct EncryptRec
{
    EncryptMode    eMode;

    // nKeyLen должно быть кратно 8, и лежать в отрезке от 40 и до 128
    unsigned int   nKeyLen;

    // Owner-Password (не шифрованный)
    BYTE           anOwnerPassword[PASSWD_LEN];

    // User-Password (не шифрованный)
    BYTE           anUserPassword[PASSWD_LEN];

    // Owner-Password (шифрованный)
    BYTE           anOwnerKey[PASSWD_LEN];

    // User-Password (шифрованный)
    BYTE           anUserKey[PASSWD_LEN];

    int            nPermission;
    BYTE           anEncryptID[ID_LEN];
    BYTE           anEncryptionKey[MD5_KEY_LEN + 5];
    BYTE           anMD5EncryptionKey[MD5_KEY_LEN];
    ARC4ContextRec oARC4Context;
};

//-------- MD5 алгоритм - Вспомогательные функции ----------------------------------------

// Оптимизационные функции.

#define OptFunc1(x, y, z) (z ^ (x & (y ^ z)))
#define OptFunc2(x, y, z) OptFunc1(z, x, y)
#define OptFunc3(x, y, z) (x ^ y ^ z)
#define OptFunc4(x, y, z) (y ^ (x | ~z))


//-------- MD5 алгоритм - Основные функции -----------------------------------------------
void MD5Init  (MD5Context *pContext);
void MD5Update(MD5Context *pContext, const BYTE *pBuf, unsigned int nLen);
// Final wrapup - pad to 64-byte boundary with the bit pattern
// 1 0* (64-bit count of bits processed, MSB-first)
void MD5Final (BYTE anDigest[16], MD5Context *pContext);
//-------- EncryptRecPtr - Вспомогательные функции ---------------------------------------------
void ARC4Init     (ARC4ContextRec *pContext, const BYTE *pKey, unsigned int nKeyLen);
void ARC4CryptBuf (ARC4ContextRec *pContext, const BYTE *pIn, BYTE *pOut, unsigned int nLen );
//-------- EncryptRecPtr - Основные функции ----------------------------------------------------
void PadOrTrancatePassword      (const char *sPassword, BYTE *pNewPassword);
void EncryptInit                (EncryptRecPtr pAttr);
void EncryptCreateUserKey       (EncryptRecPtr pAttr);
void EncryptCreateOwnerKey      (EncryptRecPtr pAttr);
void EncryptCreateEncryptionKey (EncryptRecPtr pAttr);
void EncryptInitKey             (EncryptRecPtr pAttr, unsigned int nObjectId, unsigned short nGenNo );
void EncryptReset               (EncryptRecPtr pAttr);
void EncryptCryptBuf            (EncryptRecPtr pAttr, const BYTE *pSrc, BYTE *pDst, unsigned int nLen);
#endif /* _ENCRYPT_H */



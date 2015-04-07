#include "Encrypt.h"
#include "Objects.h"

// Основной шаг в алгоритме MD5.
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

// The core of the MD5 algorithm, this alters an existing MD5 hash to
// reflect the addition of 16 longwords of new data.  MD5Update blocks
// the data and converts bytes into longwords for this routine.

static void MD5ByteReverse(BYTE *pBuf, unsigned int nLongs)
{
    unsigned int nTemp = 0;
    do
    {
        nTemp = (unsigned int) ((unsigned int) pBuf[3] << 8 | pBuf[2]) << 16 |  ((unsigned int) pBuf[1] << 8 | pBuf[0]);
        *(unsigned int *) pBuf = nTemp;
        pBuf += 4;
    }
    while (--nLongs);
}
static void MD5Transform  (unsigned int anBuf[4], const unsigned int anIn[16])
{
    register unsigned int a, b, c, d;

    a = anBuf[0];
    b = anBuf[1];
    c = anBuf[2];
    d = anBuf[3];

    MD5STEP( OptFunc1, a, b, c, d, anIn[0]  + 0xd76aa478, 7);
    MD5STEP( OptFunc1, d, a, b, c, anIn[1]  + 0xe8c7b756, 12);
    MD5STEP( OptFunc1, c, d, a, b, anIn[2]  + 0x242070db, 17);
    MD5STEP( OptFunc1, b, c, d, a, anIn[3]  + 0xc1bdceee, 22);
    MD5STEP( OptFunc1, a, b, c, d, anIn[4]  + 0xf57c0faf, 7);
    MD5STEP( OptFunc1, d, a, b, c, anIn[5]  + 0x4787c62a, 12);
    MD5STEP( OptFunc1, c, d, a, b, anIn[6]  + 0xa8304613, 17);
    MD5STEP( OptFunc1, b, c, d, a, anIn[7]  + 0xfd469501, 22);
    MD5STEP( OptFunc1, a, b, c, d, anIn[8]  + 0x698098d8, 7);
    MD5STEP( OptFunc1, d, a, b, c, anIn[9]  + 0x8b44f7af, 12);
    MD5STEP( OptFunc1, c, d, a, b, anIn[10] + 0xffff5bb1, 17);
    MD5STEP( OptFunc1, b, c, d, a, anIn[11] + 0x895cd7be, 22);
    MD5STEP( OptFunc1, a, b, c, d, anIn[12] + 0x6b901122, 7);
    MD5STEP( OptFunc1, d, a, b, c, anIn[13] + 0xfd987193, 12);
    MD5STEP( OptFunc1, c, d, a, b, anIn[14] + 0xa679438e, 17);
    MD5STEP( OptFunc1, b, c, d, a, anIn[15] + 0x49b40821, 22);

    MD5STEP( OptFunc2, a, b, c, d, anIn[1]  + 0xf61e2562, 5);
    MD5STEP( OptFunc2, d, a, b, c, anIn[6]  + 0xc040b340, 9);
    MD5STEP( OptFunc2, c, d, a, b, anIn[11] + 0x265e5a51, 14);
    MD5STEP( OptFunc2, b, c, d, a, anIn[0]  + 0xe9b6c7aa, 20);
    MD5STEP( OptFunc2, a, b, c, d, anIn[5]  + 0xd62f105d, 5);
    MD5STEP( OptFunc2, d, a, b, c, anIn[10] + 0x02441453, 9);
    MD5STEP( OptFunc2, c, d, a, b, anIn[15] + 0xd8a1e681, 14);
    MD5STEP( OptFunc2, b, c, d, a, anIn[4]  + 0xe7d3fbc8, 20);
    MD5STEP( OptFunc2, a, b, c, d, anIn[9]  + 0x21e1cde6, 5);
    MD5STEP( OptFunc2, d, a, b, c, anIn[14] + 0xc33707d6, 9);
    MD5STEP( OptFunc2, c, d, a, b, anIn[3]  + 0xf4d50d87, 14);
    MD5STEP( OptFunc2, b, c, d, a, anIn[8]  + 0x455a14ed, 20);
    MD5STEP( OptFunc2, a, b, c, d, anIn[13] + 0xa9e3e905, 5);
    MD5STEP( OptFunc2, d, a, b, c, anIn[2]  + 0xfcefa3f8, 9);
    MD5STEP( OptFunc2, c, d, a, b, anIn[7]  + 0x676f02d9, 14);
    MD5STEP( OptFunc2, b, c, d, a, anIn[12] + 0x8d2a4c8a, 20);

    MD5STEP( OptFunc3, a, b, c, d, anIn[5]  + 0xfffa3942, 4);
    MD5STEP( OptFunc3, d, a, b, c, anIn[8]  + 0x8771f681, 11);
    MD5STEP( OptFunc3, c, d, a, b, anIn[11] + 0x6d9d6122, 16);
    MD5STEP( OptFunc3, b, c, d, a, anIn[14] + 0xfde5380c, 23);
    MD5STEP( OptFunc3, a, b, c, d, anIn[1]  + 0xa4beea44, 4);
    MD5STEP( OptFunc3, d, a, b, c, anIn[4]  + 0x4bdecfa9, 11);
    MD5STEP( OptFunc3, c, d, a, b, anIn[7]  + 0xf6bb4b60, 16);
    MD5STEP( OptFunc3, b, c, d, a, anIn[10] + 0xbebfbc70, 23);
    MD5STEP( OptFunc3, a, b, c, d, anIn[13] + 0x289b7ec6, 4);
    MD5STEP( OptFunc3, d, a, b, c, anIn[0]  + 0xeaa127fa, 11);
    MD5STEP( OptFunc3, c, d, a, b, anIn[3]  + 0xd4ef3085, 16);
    MD5STEP( OptFunc3, b, c, d, a, anIn[6]  + 0x04881d05, 23);
    MD5STEP( OptFunc3, a, b, c, d, anIn[9]  + 0xd9d4d039, 4);
    MD5STEP( OptFunc3, d, a, b, c, anIn[12] + 0xe6db99e5, 11);
    MD5STEP( OptFunc3, c, d, a, b, anIn[15] + 0x1fa27cf8, 16);
    MD5STEP( OptFunc3, b, c, d, a, anIn[2]  + 0xc4ac5665, 23);

    MD5STEP( OptFunc4, a, b, c, d, anIn[0]  + 0xf4292244, 6);
    MD5STEP( OptFunc4, d, a, b, c, anIn[7]  + 0x432aff97, 10);
    MD5STEP( OptFunc4, c, d, a, b, anIn[14] + 0xab9423a7, 15);
    MD5STEP( OptFunc4, b, c, d, a, anIn[5]  + 0xfc93a039, 21);
    MD5STEP( OptFunc4, a, b, c, d, anIn[12] + 0x655b59c3, 6);
    MD5STEP( OptFunc4, d, a, b, c, anIn[3]  + 0x8f0ccc92, 10);
    MD5STEP( OptFunc4, c, d, a, b, anIn[10] + 0xffeff47d, 15);
    MD5STEP( OptFunc4, b, c, d, a, anIn[1]  + 0x85845dd1, 21);
    MD5STEP( OptFunc4, a, b, c, d, anIn[8]  + 0x6fa87e4f, 6);
    MD5STEP( OptFunc4, d, a, b, c, anIn[15] + 0xfe2ce6e0, 10);
    MD5STEP( OptFunc4, c, d, a, b, anIn[6]  + 0xa3014314, 15);
    MD5STEP( OptFunc4, b, c, d, a, anIn[13] + 0x4e0811a1, 21);
    MD5STEP( OptFunc4, a, b, c, d, anIn[4]  + 0xf7537e82, 6);
    MD5STEP( OptFunc4, d, a, b, c, anIn[11] + 0xbd3af235, 10);
    MD5STEP( OptFunc4, c, d, a, b, anIn[2]  + 0x2ad7d2bb, 15);
    MD5STEP( OptFunc4, b, c, d, a, anIn[9]  + 0xeb86d391, 21);

    anBuf[0] += a;
    anBuf[1] += b;
    anBuf[2] += c;
    anBuf[3] += d;
}
void MD5Init  (MD5Context *pContext)
{
    pContext->anBuf[0] = 0x67452301;
    pContext->anBuf[1] = 0xefcdab89;
    pContext->anBuf[2] = 0x98badcfe;
    pContext->anBuf[3] = 0x10325476;

    pContext->anBits[0] = 0;
    pContext->anBits[1] = 0;
}
void MD5Update(MD5Context *pContext, const BYTE *pBuf, unsigned int nLen)
{
    // Update bitcount

	unsigned int nTempBit = pContext->anBits[0];
    
	if ( ( pContext->anBits[0] = nTempBit + ((unsigned int) nLen << 3) ) < nTempBit )
		pContext->anBits[1]++;     // Carry from low to high
	pContext->anBits[1] += nLen >> 29;

    nTempBit = (nTempBit >> 3) & 0x3f; // Bytes already in shsInfo->data

    // Handle any leading odd-sized chunks

    if ( nTempBit ) 
	{
		BYTE *pTemp = (BYTE *) pContext->anIn + nTempBit;

        nTempBit = 64 - nTempBit;
        if ( nLen < nTempBit && pBuf )
        {
            UtilsMemCpy( pTemp, pBuf, nLen);
            return;
        }
       if (pBuf ) UtilsMemCpy( pTemp, pBuf, nTempBit);
		MD5ByteReverse( pContext->anIn, 16);
		MD5Transform( pContext->anBuf, (unsigned int *) pContext->anIn);
        pBuf += nTempBit;
        nLen -= nTempBit;
    }
    // Process data in 64-byte chunks

    while ( nLen >= 64 ) 
	{
		UtilsMemCpy( pContext->anIn, pBuf, 64);
		MD5ByteReverse( pContext->anIn, 16);
		MD5Transform( pContext->anBuf, (unsigned int *) pContext->anIn);
        pBuf += 64;
        nLen -= 64;
    }

    // Handle any remaining bytes of data.

    UtilsMemCpy( pContext->anIn, pBuf, nLen);
}

// Final wrapup - pad to 64-byte boundary with the bit pattern
// 1 0* (64-bit count of bits processed, MSB-first)
void MD5Final (BYTE anDigest[16], MD5Context *pContext)
{
    // Вычисляем количество байтов по модулю 64
    unsigned int nCount = ( pContext->anBits[0] >> 3 ) & 0x3F;

    // Устанавливаем первый символ в дополнительной строке(padding) значением 0x80. 
	// Это безопасно, поскольку всегда как минимум один байт свободен.
	BYTE *pTemp = pContext->anIn + nCount;
    *pTemp++ = 0x80;

    // Bytes of padding needed to make 64 bytes 
    nCount = 64 - 1 - nCount;

    // Pad out to 56 mod 64
    if ( nCount < 8 ) 
	{
        // Two lots of padding:  Pad the first block to 64 bytes
        UtilsMemSet( pTemp, 0, nCount);
		MD5ByteReverse( pContext->anIn, 16);
		MD5Transform( pContext->anBuf, (unsigned int *) pContext->anIn);

        // Now fill the next block with 56 bytes
		UtilsMemSet( pContext->anIn, 0, 56);
    } 
	else 
	{
        // Pad block to 56 bytes
        UtilsMemSet( pTemp, 0, nCount - 8);
    }
	MD5ByteReverse( pContext->anIn, 14);

    // Append length in bits and transform
	((unsigned int *) pContext->anIn)[14] = pContext->anBits[0];
	((unsigned int *) pContext->anIn)[15] = pContext->anBits[1];

	MD5Transform( pContext->anBuf, (unsigned int *) pContext->anIn);
	MD5ByteReverse( (BYTE *)pContext->anBuf, 4);
	UtilsMemCpy( (BYTE *)anDigest, (BYTE *)pContext->anBuf, 16);
    UtilsMemSet( (BYTE *)pContext, 0, sizeof (pContext));   // In case it's sensitive
}

//-------- EncryptRecPtr - Вспомогательные функции ---------------------------------------------
void ARC4Init     (ARC4ContextRec *pContext, const BYTE *pKey, unsigned int nKeyLen)
{
    BYTE pTempArray[ARC4_BUF_SIZE];
    unsigned int nJ = 0;

    for ( unsigned int nIndex = 0; nIndex < ARC4_BUF_SIZE; nIndex++)
		pContext->anState[nIndex] = nIndex;

    for ( unsigned int nIndex = 0; nIndex < ARC4_BUF_SIZE; nIndex++)
        pTempArray[nIndex] = pKey[nIndex % nKeyLen];

    for ( unsigned int nIndex = 0; nIndex < ARC4_BUF_SIZE; nIndex++) 
	{
        nJ = ( nJ + pContext->anState[nIndex] + pTempArray[nIndex] ) % ARC4_BUF_SIZE;

        BYTE nTemp = pContext->anState[nIndex];
        pContext->anState[nIndex] = pContext->anState[nJ];
        pContext->anState[nJ] = nTemp;
    }

    pContext->nIndex1 = 0;
    pContext->nIndex2 = 0;
}
void ARC4CryptBuf (ARC4ContextRec *pContext, const BYTE *pIn, BYTE *pOut, unsigned int nLen )
{
	for ( unsigned int nIndex = 0; nIndex < nLen; nIndex++) 
	{
		pContext->nIndex1 = ( pContext->nIndex1 + 1 ) % 256;
		pContext->nIndex2 = ( pContext->nIndex2 + pContext->anState[ pContext->nIndex1 ] ) % 256;

		BYTE nTemp = pContext->anState[ pContext->nIndex1 ];
		pContext->anState[ pContext->nIndex1 ] = pContext->anState[ pContext->nIndex2 ];
		pContext->anState[ pContext->nIndex2 ] = nTemp;

		unsigned int nTempIndex = ( pContext->anState[ pContext->nIndex1 ] + pContext->anState[ pContext->nIndex2 ] ) % 256;
		BYTE nKoef = pContext->anState[nTempIndex];

        pOut[nIndex] = pIn[nIndex] ^ nKoef;
    }
}
//-------- EncryptRecPtr - Основные функции ----------------------------------------------------
void PadOrTrancatePassword      (const char *sPassword, BYTE *pNewPassword)
{
    unsigned int nLen = UtilsStrLen( sPassword, PASSWD_LEN + 1);

    UtilsMemSet( pNewPassword, 0x00, PASSWD_LEN);

    if ( nLen >= PASSWD_LEN )
        UtilsMemCpy( pNewPassword, (BYTE*)sPassword, PASSWD_LEN);
    else 
	{
        if ( nLen > 0 )
            UtilsMemCpy( pNewPassword, (BYTE*)sPassword, nLen);
        UtilsMemCpy( pNewPassword + nLen, c_sPaddingString, PASSWD_LEN - nLen);
    }
}
void EncryptInit                (EncryptRecPtr pAttr)
{
    UtilsMemSet( pAttr, 0, sizeof(EncryptRec) );
	pAttr->eMode = EncryptR2;
	pAttr->nKeyLen = 5;
	UtilsMemCpy( pAttr->anOwnerPassword, c_sPaddingString, PASSWD_LEN);
	UtilsMemCpy( pAttr->anUserPassword,  c_sPaddingString, PASSWD_LEN);
	pAttr->nPermission = ENABLE_PRINT | ENABLE_EDIT_ALL | ENABLE_COPY | ENABLE_EDIT | PERMISSION_PAD;
}
void EncryptCreateUserKey       (EncryptRecPtr pAttr)
{
    ARC4ContextRec oContext;

    // Algorithm 3.4/5 step1

    // Algorithm 3.4 step2
	ARC4Init( &oContext, pAttr->anEncryptionKey, pAttr->nKeyLen);
	ARC4CryptBuf( &oContext, c_sPaddingString, pAttr->anUserKey, PASSWD_LEN);

	if ( EncryptR3 == pAttr->eMode ) 
	{
        MD5Context oMD5Context;
        BYTE anDigest[MD5_KEY_LEN];
        BYTE anDigest2[MD5_KEY_LEN];
        // Algorithm 3.5 step2 (same as Algorithm3.2 step2)
        MD5Init( &oMD5Context);
        MD5Update( &oMD5Context, c_sPaddingString, PASSWD_LEN);

        // Algorithm 3.5 step3
		MD5Update( &oMD5Context, pAttr->anEncryptID, ID_LEN);
        MD5Final( anDigest, &oMD5Context);
        // Algorithm 3.5 step4
		ARC4Init( &oContext, pAttr->anEncryptionKey, pAttr->nKeyLen);
        ARC4CryptBuf( &oContext, anDigest, anDigest2, MD5_KEY_LEN);
        // Algorithm 3.5 step5 */
        for ( unsigned int nI = 1; nI <= 19; nI++ ) 
		{
            BYTE pNewKey[MD5_KEY_LEN];
			for ( unsigned int nJ = 0; nJ < pAttr->nKeyLen; nJ++ )
				pNewKey[nJ] = pAttr->anEncryptionKey[nJ] ^ nI;

            UtilsMemCpy( anDigest, anDigest2, MD5_KEY_LEN);

			ARC4Init( &oContext, pNewKey, pAttr->nKeyLen );
            ARC4CryptBuf( &oContext, anDigest, anDigest2, MD5_KEY_LEN );
        }

        // use the result of Algorithm 3.4 as 'arbitrary padding'
		UtilsMemSet( pAttr->anUserKey, 0, PASSWD_LEN);
		UtilsMemCpy( pAttr->anUserKey, anDigest2, MD5_KEY_LEN);
    }
}
void EncryptCreateOwnerKey      (EncryptRecPtr pAttr)
{
    ARC4ContextRec oARC4Context;
    MD5Context oMD5Context;
    BYTE anDigest[MD5_KEY_LEN];
    BYTE anTempPassword[PASSWD_LEN];

    // create md5-digest using the value of anOwnerPassword 

    // Algorithm 3.3 step 2
    MD5Init( &oMD5Context);
    MD5Update( &oMD5Context, pAttr->anOwnerPassword, PASSWD_LEN);

    MD5Final( anDigest, &oMD5Context);

    // Algorithm 3.3 step 3 (Revision 3 only)
    if ( EncryptR3 == pAttr->eMode ) 
	{
        for (unsigned int nIndex = 0; nIndex < 50; nIndex++) 
		{
            MD5Init( &oMD5Context);

            //MD5Update( &oMD5Context, anDigest, MD5_KEY_LEN);
            MD5Update ( &oMD5Context, anDigest, pAttr->nKeyLen );
            MD5Final( anDigest, &oMD5Context);
        }
    }

    // Algorithm 3.3 step 4   
    ARC4Init( &oARC4Context, anDigest, pAttr->nKeyLen );
    // Algorithm 3.3 step 6   
    ARC4CryptBuf( &oARC4Context, pAttr->anUserPassword, anTempPassword, PASSWD_LEN);
    // Algorithm 3.3 step 7   
    if ( EncryptR3 == pAttr->eMode )
	{
        BYTE anTempPassword2[PASSWD_LEN];

        for (unsigned int i = 1; i <= 19; i++) 
		{
            BYTE anNewKey[MD5_KEY_LEN];

            for (unsigned int j = 0; j < pAttr->nKeyLen; j++)
                anNewKey[j] = anDigest[j] ^ i;

            UtilsMemCpy( anTempPassword2, anTempPassword, PASSWD_LEN);
            ARC4Init( &oARC4Context, anNewKey, pAttr->nKeyLen);
            ARC4CryptBuf( &oARC4Context, anTempPassword2, anTempPassword, PASSWD_LEN);
        }
    }

    // Algorithm 3.3 step 8   
	UtilsMemCpy( pAttr->anOwnerKey, anTempPassword, PASSWD_LEN);
}

void EncryptCreateEncryptionKey (EncryptRecPtr pAttr)
{
    MD5Context oMD5Context;
    BYTE anTempFlag[4];

    // Algorithm3.2 step2   
    MD5Init( &oMD5Context);
    MD5Update( &oMD5Context, pAttr->anUserPassword, PASSWD_LEN);

    // Algorithm3.2 step3   
    MD5Update( &oMD5Context, pAttr->anOwnerKey, PASSWD_LEN);


    // Algorithm3.2 step4   
    anTempFlag[0] = pAttr->nPermission;
    anTempFlag[1] = (pAttr->nPermission >> 8);
    anTempFlag[2] = (pAttr->nPermission >> 16);
    anTempFlag[3] = (pAttr->nPermission >> 24);

    MD5Update( &oMD5Context, anTempFlag, 4);

    // Algorithm3.2 step5   
    MD5Update(&oMD5Context, pAttr->anEncryptID, ID_LEN);
    MD5Final( pAttr->anEncryptionKey, &oMD5Context);

    // Algorithm 3.2 step6 (Revision 3 only)   
    if ( EncryptR3 == pAttr->eMode ) 
	{
        for ( unsigned int nIndex = 0; nIndex < 50; nIndex++) 
		{
            MD5Init( &oMD5Context);
            MD5Update( &oMD5Context, pAttr->anEncryptionKey, pAttr->nKeyLen);
            MD5Final( pAttr->anEncryptionKey, &oMD5Context);
        }
    }
}
void EncryptInitKey             (EncryptRecPtr pAttr, unsigned int nObjectId, unsigned short nGenNo )
{
    MD5Context oContext;

    pAttr->anEncryptionKey[pAttr->nKeyLen] = nObjectId;
    pAttr->anEncryptionKey[pAttr->nKeyLen + 1] = (nObjectId >> 8);
    pAttr->anEncryptionKey[pAttr->nKeyLen + 2] = (nObjectId >> 16);
    pAttr->anEncryptionKey[pAttr->nKeyLen + 3] = nGenNo;
    pAttr->anEncryptionKey[pAttr->nKeyLen + 4] = (nGenNo >> 8);

    MD5Init( &oContext);
    MD5Update( &oContext, pAttr->anEncryptionKey, pAttr->nKeyLen + 5);
    MD5Final( pAttr->anMD5EncryptionKey, &oContext);

    unsigned int nKeyLen = ( pAttr->nKeyLen + 5 > ENCRYPT_KEY_MAX) ? ENCRYPT_KEY_MAX : pAttr->nKeyLen + 5;

	ARC4Init( &pAttr->oARC4Context, pAttr->anMD5EncryptionKey, nKeyLen );
}

void EncryptReset               (EncryptRecPtr pAttr)
{
    unsigned int nKeyLen = ( pAttr->nKeyLen + 5 > ENCRYPT_KEY_MAX) ? ENCRYPT_KEY_MAX : pAttr->nKeyLen + 5;
    ARC4Init( &pAttr->oARC4Context, pAttr->anMD5EncryptionKey, nKeyLen );
}
void EncryptCryptBuf            (EncryptRecPtr pAttr, const BYTE *pSrc, BYTE *pDst, unsigned int nLen)
{
    ARC4CryptBuf( &pAttr->oARC4Context, pSrc, pDst, nLen);
}

#include "Encrypt.h"
#include "Objects.h"

// Оптимизационные функции.
#define OptFunc1(x, y, z) (z ^ (x & (y ^ z)))
#define OptFunc2(x, y, z) OptFunc1(z, x, y)
#define OptFunc3(x, y, z) (x ^ y ^ z)
#define OptFunc4(x, y, z) (y ^ (x | ~z))

// Основной шаг в алгоритме MD5.
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

namespace PdfWriter
{
	static const BYTE c_sPaddingString[] =
	{
		0x28, 0xBF, 0x4E, 0x5E, 0x4E, 0x75, 0x8A, 0x41,
		0x64, 0x00, 0x4E, 0x56, 0xFF, 0xFA, 0x01, 0x08,
		0x2E, 0x2E, 0x00, 0xB6, 0xD0, 0x68, 0x3E, 0x80,
		0x2F, 0x0C, 0xA9, 0xFE, 0x64, 0x53, 0x69, 0x7A
	};

	void PadOrTrancatePassword(const char* sPassword, BYTE* pNewPassword)
	{
		unsigned int unLen = StrLen(sPassword, PASSWD_LEN + 1);
		MemSet(pNewPassword, 0x00, PASSWD_LEN);

		if (unLen >= PASSWD_LEN)
			MemCpy(pNewPassword, (BYTE*)sPassword, PASSWD_LEN);
		else
		{
			if (unLen > 0)
				MemCpy(pNewPassword, (BYTE*)sPassword, unLen);

			MemCpy(pNewPassword + unLen, c_sPaddingString, PASSWD_LEN - unLen);
		}
	}
	// The core of the MD5 algorithm, this alters an existing MD5 hash to
	// reflect the addition of 16 longwords of new data.  MD5Update blocks
	// the data and converts bytes into longwords for this routine.
	static void MD5ByteReverse(BYTE *pBuf, unsigned int nLongs)
	{
		unsigned int nTemp = 0;
		do
		{
			nTemp = (unsigned int)((unsigned int)pBuf[3] << 8 | pBuf[2]) << 16 | ((unsigned int)pBuf[1] << 8 | pBuf[0]);
			*(unsigned int *)pBuf = nTemp;
			pBuf += 4;
		} while (--nLongs);
	}
	static void MD5Transform(unsigned int anBuf[4], const unsigned int anIn[16])
	{
		register unsigned int a, b, c, d;

		a = anBuf[0];
		b = anBuf[1];
		c = anBuf[2];
		d = anBuf[3];

		MD5STEP(OptFunc1, a, b, c, d, anIn[0] + 0xd76aa478, 7);
		MD5STEP(OptFunc1, d, a, b, c, anIn[1] + 0xe8c7b756, 12);
		MD5STEP(OptFunc1, c, d, a, b, anIn[2] + 0x242070db, 17);
		MD5STEP(OptFunc1, b, c, d, a, anIn[3] + 0xc1bdceee, 22);
		MD5STEP(OptFunc1, a, b, c, d, anIn[4] + 0xf57c0faf, 7);
		MD5STEP(OptFunc1, d, a, b, c, anIn[5] + 0x4787c62a, 12);
		MD5STEP(OptFunc1, c, d, a, b, anIn[6] + 0xa8304613, 17);
		MD5STEP(OptFunc1, b, c, d, a, anIn[7] + 0xfd469501, 22);
		MD5STEP(OptFunc1, a, b, c, d, anIn[8] + 0x698098d8, 7);
		MD5STEP(OptFunc1, d, a, b, c, anIn[9] + 0x8b44f7af, 12);
		MD5STEP(OptFunc1, c, d, a, b, anIn[10] + 0xffff5bb1, 17);
		MD5STEP(OptFunc1, b, c, d, a, anIn[11] + 0x895cd7be, 22);
		MD5STEP(OptFunc1, a, b, c, d, anIn[12] + 0x6b901122, 7);
		MD5STEP(OptFunc1, d, a, b, c, anIn[13] + 0xfd987193, 12);
		MD5STEP(OptFunc1, c, d, a, b, anIn[14] + 0xa679438e, 17);
		MD5STEP(OptFunc1, b, c, d, a, anIn[15] + 0x49b40821, 22);

		MD5STEP(OptFunc2, a, b, c, d, anIn[1] + 0xf61e2562, 5);
		MD5STEP(OptFunc2, d, a, b, c, anIn[6] + 0xc040b340, 9);
		MD5STEP(OptFunc2, c, d, a, b, anIn[11] + 0x265e5a51, 14);
		MD5STEP(OptFunc2, b, c, d, a, anIn[0] + 0xe9b6c7aa, 20);
		MD5STEP(OptFunc2, a, b, c, d, anIn[5] + 0xd62f105d, 5);
		MD5STEP(OptFunc2, d, a, b, c, anIn[10] + 0x02441453, 9);
		MD5STEP(OptFunc2, c, d, a, b, anIn[15] + 0xd8a1e681, 14);
		MD5STEP(OptFunc2, b, c, d, a, anIn[4] + 0xe7d3fbc8, 20);
		MD5STEP(OptFunc2, a, b, c, d, anIn[9] + 0x21e1cde6, 5);
		MD5STEP(OptFunc2, d, a, b, c, anIn[14] + 0xc33707d6, 9);
		MD5STEP(OptFunc2, c, d, a, b, anIn[3] + 0xf4d50d87, 14);
		MD5STEP(OptFunc2, b, c, d, a, anIn[8] + 0x455a14ed, 20);
		MD5STEP(OptFunc2, a, b, c, d, anIn[13] + 0xa9e3e905, 5);
		MD5STEP(OptFunc2, d, a, b, c, anIn[2] + 0xfcefa3f8, 9);
		MD5STEP(OptFunc2, c, d, a, b, anIn[7] + 0x676f02d9, 14);
		MD5STEP(OptFunc2, b, c, d, a, anIn[12] + 0x8d2a4c8a, 20);

		MD5STEP(OptFunc3, a, b, c, d, anIn[5] + 0xfffa3942, 4);
		MD5STEP(OptFunc3, d, a, b, c, anIn[8] + 0x8771f681, 11);
		MD5STEP(OptFunc3, c, d, a, b, anIn[11] + 0x6d9d6122, 16);
		MD5STEP(OptFunc3, b, c, d, a, anIn[14] + 0xfde5380c, 23);
		MD5STEP(OptFunc3, a, b, c, d, anIn[1] + 0xa4beea44, 4);
		MD5STEP(OptFunc3, d, a, b, c, anIn[4] + 0x4bdecfa9, 11);
		MD5STEP(OptFunc3, c, d, a, b, anIn[7] + 0xf6bb4b60, 16);
		MD5STEP(OptFunc3, b, c, d, a, anIn[10] + 0xbebfbc70, 23);
		MD5STEP(OptFunc3, a, b, c, d, anIn[13] + 0x289b7ec6, 4);
		MD5STEP(OptFunc3, d, a, b, c, anIn[0] + 0xeaa127fa, 11);
		MD5STEP(OptFunc3, c, d, a, b, anIn[3] + 0xd4ef3085, 16);
		MD5STEP(OptFunc3, b, c, d, a, anIn[6] + 0x04881d05, 23);
		MD5STEP(OptFunc3, a, b, c, d, anIn[9] + 0xd9d4d039, 4);
		MD5STEP(OptFunc3, d, a, b, c, anIn[12] + 0xe6db99e5, 11);
		MD5STEP(OptFunc3, c, d, a, b, anIn[15] + 0x1fa27cf8, 16);
		MD5STEP(OptFunc3, b, c, d, a, anIn[2] + 0xc4ac5665, 23);

		MD5STEP(OptFunc4, a, b, c, d, anIn[0] + 0xf4292244, 6);
		MD5STEP(OptFunc4, d, a, b, c, anIn[7] + 0x432aff97, 10);
		MD5STEP(OptFunc4, c, d, a, b, anIn[14] + 0xab9423a7, 15);
		MD5STEP(OptFunc4, b, c, d, a, anIn[5] + 0xfc93a039, 21);
		MD5STEP(OptFunc4, a, b, c, d, anIn[12] + 0x655b59c3, 6);
		MD5STEP(OptFunc4, d, a, b, c, anIn[3] + 0x8f0ccc92, 10);
		MD5STEP(OptFunc4, c, d, a, b, anIn[10] + 0xffeff47d, 15);
		MD5STEP(OptFunc4, b, c, d, a, anIn[1] + 0x85845dd1, 21);
		MD5STEP(OptFunc4, a, b, c, d, anIn[8] + 0x6fa87e4f, 6);
		MD5STEP(OptFunc4, d, a, b, c, anIn[15] + 0xfe2ce6e0, 10);
		MD5STEP(OptFunc4, c, d, a, b, anIn[6] + 0xa3014314, 15);
		MD5STEP(OptFunc4, b, c, d, a, anIn[13] + 0x4e0811a1, 21);
		MD5STEP(OptFunc4, a, b, c, d, anIn[4] + 0xf7537e82, 6);
		MD5STEP(OptFunc4, d, a, b, c, anIn[11] + 0xbd3af235, 10);
		MD5STEP(OptFunc4, c, d, a, b, anIn[2] + 0x2ad7d2bb, 15);
		MD5STEP(OptFunc4, b, c, d, a, anIn[9] + 0xeb86d391, 21);

		anBuf[0] += a;
		anBuf[1] += b;
		anBuf[2] += c;
		anBuf[3] += d;
	}
	//----------------------------------------------------------------------------------------
	// CMd5
	//----------------------------------------------------------------------------------------
	CMd5::CMd5()
	{
		Init();
	};
	void CMd5::Init()
	{
		m_anBuf[0] = 0x67452301;
		m_anBuf[1] = 0xefcdab89;
		m_anBuf[2] = 0x98badcfe;
		m_anBuf[3] = 0x10325476;

		m_anBits[0] = 0;
		m_anBits[1] = 0;

		MemSet(m_anIn, 0x00, 64 * sizeof(BYTE));
	}
	void CMd5::Update(const BYTE* pBuffer, unsigned int unLen)
	{
		// Update bitcount
		unsigned int nTempBit = m_anBits[0];

		if ((m_anBits[0] = nTempBit + (unLen << 3)) < nTempBit)
			m_anBits[1]++;     // Carry from low to high

		m_anBits[1] += unLen >> 29;
		nTempBit = (nTempBit >> 3) & 0x3f; // Bytes already in shsInfo->data

		// Handle any leading odd-sized chunks
		if (nTempBit)
		{
			BYTE *pTemp = (BYTE *)m_anIn + nTempBit;

			nTempBit = 64 - nTempBit;
			if (unLen < nTempBit && pBuffer)
			{
				MemCpy(pTemp, pBuffer, unLen);
				return;
			}

			if (pBuffer)
				MemCpy(pTemp, pBuffer, nTempBit);

			MD5ByteReverse(m_anIn, 16);
			MD5Transform(m_anBuf, (unsigned int *)m_anIn);
			pBuffer += nTempBit;
			unLen -= nTempBit;
		}

		// Process data in 64-byte chunks
		while (unLen >= 64)
		{
			MemCpy(m_anIn, pBuffer, 64);
			MD5ByteReverse(m_anIn, 16);
			MD5Transform(m_anBuf, (unsigned int *)m_anIn);
			pBuffer += 64;
			unLen -= 64;
		}

		// Handle any remaining bytes of data.
		MemCpy(m_anIn, pBuffer, unLen);
	}
	// Final wrapup - pad to 64-byte boundary with the bit pattern
	// 1 0* (64-bit count of bits processed, MSB-first)
	void CMd5::Final(BYTE anDigest[16])
	{
		// Вычисляем количество байтов по модулю 64
		unsigned int nCount = (m_anBits[0] >> 3) & 0x3F;

		// Устанавливаем первый символ в дополнительной строке(padding) значением 0x80. 
		// Это безопасно, поскольку всегда как минимум один байт свободен.
		BYTE *pTemp = m_anIn + nCount;
		*pTemp++ = 0x80;

		// Bytes of padding needed to make 64 bytes 
		nCount = 64 - 1 - nCount;

		// Pad out to 56 mod 64
		if (nCount < 8)
		{
			// Two lots of padding:  Pad the first block to 64 bytes
			MemSet(pTemp, 0, nCount);
			MD5ByteReverse(m_anIn, 16);
			MD5Transform(m_anBuf, (unsigned int *)m_anIn);

			// Now fill the next block with 56 bytes
			MemSet(m_anIn, 0, 56);
		}
		else
		{
			// Pad block to 56 bytes
			MemSet(pTemp, 0, nCount - 8);
		}
		MD5ByteReverse(m_anIn, 14);

		// Append length in bits and transform
		((unsigned int *)m_anIn)[14] = m_anBits[0];
		((unsigned int *)m_anIn)[15] = m_anBits[1];

		MD5Transform(m_anBuf, (unsigned int *)m_anIn);
		MD5ByteReverse((BYTE *)m_anBuf, 4);
		MemCpy((BYTE *)anDigest, (BYTE *)m_anBuf, 16);
		Clear(); // In case it's sensitive
	}
	void CMd5::Clear()
	{
		MemSet(m_anBits, 0, 2 * sizeof(unsigned int));
		MemSet(m_anBuf, 0, 4 * sizeof(unsigned int));
		MemSet(m_anIn, 0x00, 64 * sizeof(BYTE));
	}
	//----------------------------------------------------------------------------------------
	// CArc4
	//----------------------------------------------------------------------------------------
	void CArc4::Init(const BYTE* pKey, unsigned int unKeyLen)
	{
		BYTE pTempArray[ARC4_BUF_SIZE];
		unsigned int nJ = 0;

		for (unsigned int nIndex = 0; nIndex < ARC4_BUF_SIZE; nIndex++)
			m_anState[nIndex] = nIndex;

		for (unsigned int nIndex = 0; nIndex < ARC4_BUF_SIZE; nIndex++)
			pTempArray[nIndex] = pKey[nIndex % unKeyLen];

		for (unsigned int nIndex = 0; nIndex < ARC4_BUF_SIZE; nIndex++)
		{
			nJ = (nJ + m_anState[nIndex] + pTempArray[nIndex]) % ARC4_BUF_SIZE;

			BYTE nTemp = m_anState[nIndex];
			m_anState[nIndex] = m_anState[nJ];
			m_anState[nJ] = nTemp;
		}

		m_nIndex1 = 0;
		m_nIndex2 = 0;
	}
	void CArc4::CryptBuf(const BYTE* pIn, BYTE* pOut, unsigned int unLen)
	{
		for (unsigned int nIndex = 0; nIndex < unLen; nIndex++)
		{
			m_nIndex1 = (m_nIndex1 + 1) % 256;
			m_nIndex2 = (m_nIndex2 + m_anState[m_nIndex1]) % 256;

			BYTE nTemp = m_anState[m_nIndex1];
			m_anState[m_nIndex1] = m_anState[m_nIndex2];
			m_anState[m_nIndex2] = nTemp;

			unsigned int nTempIndex = (m_anState[m_nIndex1] + m_anState[m_nIndex2]) % 256;
			BYTE nKoef = m_anState[nTempIndex];

			pOut[nIndex] = pIn[nIndex] ^ nKoef;
		}
	}
	//----------------------------------------------------------------------------------------
	// CEncrypt
	//----------------------------------------------------------------------------------------
	CEncrypt::CEncrypt()
	{
		Init();
	}
	void CEncrypt::Init()
	{
		m_eMode = encryptmode_R2;
		m_unKeyLen = 5;
		MemCpy(m_anOwnerPassword, c_sPaddingString, PASSWD_LEN);
		MemCpy(m_anUserPassword, c_sPaddingString, PASSWD_LEN);
		m_unPermission = ENABLE_PRINT | ENABLE_EDIT_ALL | ENABLE_COPY | ENABLE_EDIT | PERMISSION_PAD;

		MemSet(m_anOwnerKey, 0, PASSWD_LEN);
		MemSet(m_anUserKey, 0, PASSWD_LEN);
		MemSet(m_anEncryptID, 0, ID_LEN);
		MemSet(m_anEncryptionKey, 0, MD5_KEY_LEN + 5);
		MemSet(m_anMD5EncryptionKey, 0, MD5_KEY_LEN);
	}
	void CEncrypt::CreateUserKey()
	{
		CArc4 oContext;

		// Algorithm 3.4/5 step1
		// Algorithm 3.4 step2
		oContext.Init(m_anEncryptionKey, m_unKeyLen);
		oContext.CryptBuf(c_sPaddingString, m_anUserKey, PASSWD_LEN);

		if (encryptmode_R3 == m_eMode)
		{
			CMd5 oMmd5;
			BYTE anDigest[MD5_KEY_LEN];
			BYTE anDigest2[MD5_KEY_LEN];

			// Algorithm 3.5 step2 (same as Algorithm3.2 step2)
			oMmd5.Init();
			oMmd5.Update(c_sPaddingString, PASSWD_LEN);

			// Algorithm 3.5 step3
			oMmd5.Update(m_anEncryptID, ID_LEN);
			oMmd5.Final(anDigest);

			// Algorithm 3.5 step4
			oContext.Init(m_anEncryptionKey, m_unKeyLen);
			oContext.CryptBuf(anDigest, anDigest2, MD5_KEY_LEN);

			// Algorithm 3.5 step5
			for (unsigned int nI = 1; nI <= 19; nI++)
			{
				BYTE pNewKey[MD5_KEY_LEN];
				for (unsigned int nJ = 0; nJ < m_unKeyLen; nJ++)
					pNewKey[nJ] = m_anEncryptionKey[nJ] ^ nI;

				MemCpy(anDigest, anDigest2, MD5_KEY_LEN);

				oContext.Init(pNewKey, m_unKeyLen);
				oContext.CryptBuf(anDigest, anDigest2, MD5_KEY_LEN);
			}

			// use the result of Algorithm 3.4 as 'arbitrary padding'
			MemSet(m_anUserKey, 0, PASSWD_LEN);
			MemCpy(m_anUserKey, anDigest2, MD5_KEY_LEN);
		}
	}
	void CEncrypt::CreateOwnerKey()
	{
		CArc4 oArc4;
		CMd5 oMd5;

		BYTE anDigest[MD5_KEY_LEN];
		BYTE anTempPassword[PASSWD_LEN];

		// create md5-digest using the value of anOwnerPassword 
		// Algorithm 3.3 step 2
		oMd5.Init();
		oMd5.Update(m_anOwnerPassword, PASSWD_LEN);
		oMd5.Final(anDigest);

		// Algorithm 3.3 step 3 (Revision 3 only)
		if (encryptmode_R3 == m_eMode)
		{
			for (unsigned int nIndex = 0; nIndex < 50; nIndex++)
			{
				oMd5.Init();
				oMd5.Update(anDigest, m_unKeyLen);
				oMd5.Final(anDigest);
			}
		}

		// Algorithm 3.3 step 4   
		oArc4.Init(anDigest, m_unKeyLen);

		// Algorithm 3.3 step 6   
		oArc4.CryptBuf(m_anUserPassword, anTempPassword, PASSWD_LEN);

		// Algorithm 3.3 step 7   
		if (encryptmode_R3 == m_eMode)
		{
			BYTE anTempPassword2[PASSWD_LEN];

			for (unsigned int i = 1; i <= 19; i++)
			{
				BYTE anNewKey[MD5_KEY_LEN];

				for (unsigned int j = 0; j < m_unKeyLen; j++)
					anNewKey[j] = anDigest[j] ^ i;

				MemCpy(anTempPassword2, anTempPassword, PASSWD_LEN);
				oArc4.Init(anNewKey, m_unKeyLen);
				oArc4.CryptBuf(anTempPassword2, anTempPassword, PASSWD_LEN);
			}
		}

		// Algorithm 3.3 step 8   
		MemCpy(m_anOwnerKey, anTempPassword, PASSWD_LEN);
	}
	void CEncrypt::CreateEncryptionKey()
	{
		CMd5 oMd5;
		BYTE anTempFlag[4];

		// Algorithm3.2 step2   
		oMd5.Init();
		oMd5.Update(m_anUserPassword, PASSWD_LEN);

		// Algorithm3.2 step3   
		oMd5.Update(m_anOwnerKey, PASSWD_LEN);


		// Algorithm3.2 step4   
		anTempFlag[0] =  m_unPermission;
		anTempFlag[1] = (m_unPermission >> 8);
		anTempFlag[2] = (m_unPermission >> 16);
		anTempFlag[3] = (m_unPermission >> 24);

		oMd5.Update(anTempFlag, 4);

		// Algorithm3.2 step5   
		oMd5.Update(m_anEncryptID, ID_LEN);
		oMd5.Final(m_anEncryptionKey);

		// Algorithm 3.2 step6 (Revision 3 only)   
		if (encryptmode_R3 == m_eMode)
		{
			for (unsigned int nIndex = 0; nIndex < 50; nIndex++)
			{
				oMd5.Init();
				oMd5.Update(m_anEncryptionKey, m_unKeyLen);
				oMd5.Final(m_anEncryptionKey);
			}
		}
	}
	void CEncrypt::InitKey(unsigned int unObjectId, unsigned short unGenNo)
	{
		m_anEncryptionKey[m_unKeyLen + 0] = (BYTE) unObjectId;
		m_anEncryptionKey[m_unKeyLen + 1] = (BYTE)(unObjectId >> 8);
		m_anEncryptionKey[m_unKeyLen + 2] = (BYTE)(unObjectId >> 16);
		m_anEncryptionKey[m_unKeyLen + 3] = (BYTE) unGenNo;
		m_anEncryptionKey[m_unKeyLen + 4] = (BYTE)(unGenNo >> 8);

		CMd5 oMd5;
		oMd5.Init();
		oMd5.Update(m_anEncryptionKey, m_unKeyLen + 5);
		oMd5.Final(m_anMD5EncryptionKey);

		unsigned int unKeyLen = (m_unKeyLen + 5 > ENCRYPT_KEY_MAX) ? ENCRYPT_KEY_MAX : m_unKeyLen + 5;
		m_oArc4Context.Init(m_anMD5EncryptionKey, unKeyLen);
	}
	void CEncrypt::Reset()
	{
		unsigned int unKeyLen = (m_unKeyLen + 5 > ENCRYPT_KEY_MAX) ? ENCRYPT_KEY_MAX : m_unKeyLen + 5;
		m_oArc4Context.Init(m_anMD5EncryptionKey, unKeyLen);
	}
	void CEncrypt::CryptBuf(const BYTE* pSrc, BYTE* pDst, unsigned int unLen)
	{
		m_oArc4Context.CryptBuf(pSrc, pDst, unLen);
	}
	void CEncrypt::SetPermission(unsigned int unPermission)
	{
		m_unPermission = unPermission;
	}
	void CEncrypt::SetMode(EEncryptMode eMode, unsigned int unKeyLen)
	{
		if (encryptmode_R2 == eMode)
			m_unKeyLen = 5;
		else
		{
			if (unKeyLen >= 5 && unKeyLen <= 16)
				m_unKeyLen = unKeyLen;
			else
				m_unKeyLen = 16;
		}

		m_eMode = eMode;
	}
}
/*
 * (c) Copyright Ascensio System SIA 2010-2018
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include <string.h>
#include "MemoryUtils.h"
#include "Decrypt.h"

namespace PdfReader
{
	static void          RC4InitKey(unsigned char *sKey, int nKeyLen, unsigned char *sState);
	static unsigned char RC4DecryptByte(unsigned char *sState, unsigned char *pX, unsigned char *pY, unsigned char nChar);

	static void AESKeyExpansion(DecryptAESState *pState, unsigned char *sObjectKey, int nObjectKeyLen);
	static void AESDecryptBlock(DecryptAESState *pState, unsigned char *sIn, bool bLast);
	static void MD5(unsigned char *sMessage, int nMessageLen, unsigned char *sDigest);

	static unsigned char passwordPad[32] =
	{
		0x28, 0xbf, 0x4e, 0x5e, 0x4e, 0x75, 0x8a, 0x41,
		0x64, 0x00, 0x4e, 0x56, 0xff, 0xfa, 0x01, 0x08,
		0x2e, 0x2e, 0x00, 0xb6, 0xd0, 0x68, 0x3e, 0x80,
		0x2f, 0x0c, 0xa9, 0xfe, 0x64, 0x53, 0x69, 0x7a
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Decrypt
	//-------------------------------------------------------------------------------------------------------------------------------

	bool Decrypt::MakeFileKey(int nEncVersion, int nEncRevision, int nKeyLength, StringExt *seOwnerKey, StringExt *seUserKey, int nPermissions, StringExt *seFileID, StringExt *seOwnerPassword, StringExt *seUserPassword, unsigned char *sFileKey, bool bEncryptMetadata, bool *pbOwnerPasswordValid)
	{
		// Попытаемся, используя пароль владельца, сгенерировать пользовательский пароль
		*pbOwnerPasswordValid = false;
		if (seOwnerPassword)
		{
			int nLen = seOwnerPassword->GetLength();
			unsigned char arrOwnerPass[32];
			if (nLen < 32)
			{
				memcpy(arrOwnerPass, seOwnerPassword->GetBuffer(), nLen);
				memcpy(arrOwnerPass + nLen, passwordPad, 32 - nLen);
			}
			else
			{
				memcpy(arrOwnerPass, seOwnerPassword->GetBuffer(), 32);
			}
			MD5(arrOwnerPass, 32, arrOwnerPass);

			if (nEncRevision == 3)
			{
				for (int nIndex = 0; nIndex < 50; ++nIndex)
				{
					MD5(arrOwnerPass, 16, arrOwnerPass);
				}
			}

			unsigned char arrOwnerKey[32];
			unsigned char arrFState[256];
			if (nEncRevision == 2)
			{
				RC4InitKey(arrOwnerPass, nKeyLength, arrFState);
				unsigned char unFX = 0, unFY = 0;
				for (int nIndex = 0; nIndex < 32; ++nIndex)
				{
					arrOwnerKey[nIndex] = RC4DecryptByte(arrFState, &unFX, &unFY, seOwnerKey->GetAt(nIndex));
				}
			}
			else
			{
				memcpy(arrOwnerKey, seOwnerKey->GetBuffer(), 32);
				for (int nIndex = 19; nIndex >= 0; --nIndex)
				{
					unsigned char arrTempKey[16];
					for (int nJ = 0; nJ < nKeyLength; ++nJ)
					{
						arrTempKey[nJ] = arrOwnerPass[nJ] ^ nIndex;
					}
					RC4InitKey(arrTempKey, nKeyLength, arrFState);
					unsigned char unFX = 0, unFY = 0;
					for (int nJ = 0; nJ < 32; ++nJ)
					{
						arrOwnerKey[nJ] = RC4DecryptByte(arrFState, &unFX, &unFY, arrOwnerKey[nJ]);
					}
				}
			}
			StringExt *seUserPassword2 = new StringExt((char *)arrOwnerKey, 32);
			if (MakeFileKey2(nEncVersion, nEncRevision, nKeyLength, seOwnerKey, seUserKey, nPermissions, seFileID, seUserPassword2, sFileKey, bEncryptMetadata))
			{
				*pbOwnerPasswordValid = true;
				delete seUserPassword2;
				return true;
			}
			delete seUserPassword2;
		}

		// Попытаемся использовать пользовательский пароль
		return MakeFileKey2(nEncVersion, nEncRevision, nKeyLength, seOwnerKey, seUserKey, nPermissions, seFileID, seUserPassword, sFileKey, bEncryptMetadata);
	}

	bool Decrypt::MakeFileKey2(int nEncVersion, int nEncRevision, int nKeyLength, StringExt *seOwnerKey, StringExt *seUserKey, int nPermissions, StringExt *seFileID, StringExt *seUserPassword, unsigned char *sFileKey, bool bEncryptMetadata)
	{
		unsigned char sTest[32];
		unsigned char sFState[256];
		unsigned char sTempKey[16];
		unsigned char unFx, unFy;
		int nLen = 0;
		bool bResult = true;

		unsigned char *pBuffer = (unsigned char *)MemUtilsMalloc(72 + seFileID->GetLength());
		if (seUserPassword)
		{
			nLen = seUserPassword->GetLength();
			if (nLen < 32)
			{
				memcpy(pBuffer, seUserPassword->GetBuffer(), nLen);
				memcpy(pBuffer + nLen, passwordPad, 32 - nLen);
			}
			else
			{
				memcpy(pBuffer, seUserPassword->GetBuffer(), 32);
			}
		}
		else
		{
			memcpy(pBuffer, passwordPad, 32);
		}

		memcpy(pBuffer + 32, seOwnerKey->GetBuffer(), 32);
		pBuffer[64] =  nPermissions & 0xff;
		pBuffer[65] = (nPermissions >> 8) & 0xff;
		pBuffer[66] = (nPermissions >> 16) & 0xff;
		pBuffer[67] = (nPermissions >> 24) & 0xff;

		memcpy(pBuffer + 68, seFileID->GetBuffer(), seFileID->GetLength());
		nLen = 68 + seFileID->GetLength();
		if (!bEncryptMetadata)
		{
			pBuffer[nLen++] = 0xff;
			pBuffer[nLen++] = 0xff;
			pBuffer[nLen++] = 0xff;
			pBuffer[nLen++] = 0xff;
		}
		MD5(pBuffer, nLen, sFileKey);
		if (nEncRevision == 3)
		{
			for (int nIndex = 0; nIndex < 50; ++nIndex)
			{
				MD5(sFileKey, nKeyLength, sFileKey);
			}
		}

		if (nEncRevision == 2)
		{
			RC4InitKey(sFileKey, nKeyLength, sFState);
			unFx = unFy = 0;
			for (int nJ = 0; nJ < 32; ++nJ)
			{
				sTest[nJ] = RC4DecryptByte(sFState, &unFx, &unFy, seUserKey->GetAt(nJ));
			}
			bResult = (memcmp(sTest, passwordPad, 32) == 0);
		}
		else if (nEncRevision == 3)
		{
			memcpy(sTest, seUserKey->GetBuffer(), 32);
			for (int nIndex = 19; nIndex >= 0; --nIndex)
			{
				for (int nJ = 0; nJ < nKeyLength; ++nJ)
				{
					sTempKey[nJ] = sFileKey[nJ] ^ nIndex;
				}
				RC4InitKey(sTempKey, nKeyLength, sFState);
				unFx = unFy = 0;
				for (int nJ = 0; nJ < 32; ++nJ)
				{
					sTest[nJ] = RC4DecryptByte(sFState, &unFx, &unFy, sTest[nJ]);
				}
			}
			memcpy(pBuffer, passwordPad, 32);
			memcpy(pBuffer + 32, seFileID->GetBuffer(), seFileID->GetLength());
			MD5(pBuffer, 32 + seFileID->GetLength(), pBuffer);
			bResult = (memcmp(sTest, pBuffer, 16) == 0);
		}
		else
		{
			bResult = false;
		}

		MemUtilsFree(pBuffer);
		return bResult;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// DecryptStream
	//-------------------------------------------------------------------------------------------------------------------------------

	DecryptStream::DecryptStream(Stream *pStream, unsigned char *sFileKey, CryptAlgorithm eType, int nKeyLength, int nObjectNum, int nObjectGen) :
		FilterStream(pStream)
	{
		m_eCryptType = eType;

		for (int nIndex = 0; nIndex < nKeyLength; ++nIndex)
		{
			m_sObjectKey[nIndex] = sFileKey[nIndex];
		}

		m_sObjectKey[nKeyLength + 0] =  nObjectNum & 0xff;
		m_sObjectKey[nKeyLength + 1] = (nObjectNum >> 8) & 0xff;
		m_sObjectKey[nKeyLength + 2] = (nObjectNum >> 16) & 0xff;
		m_sObjectKey[nKeyLength + 3] =  nObjectGen & 0xff;
		m_sObjectKey[nKeyLength + 4] = (nObjectGen >> 8) & 0xff;

		int nLen = 0;
		if (m_eCryptType == cryptAES)
		{
			m_sObjectKey[nKeyLength + 5] = 0x73; // 's'
			m_sObjectKey[nKeyLength + 6] = 0x41; // 'A'
			m_sObjectKey[nKeyLength + 7] = 0x6c; // 'l'
			m_sObjectKey[nKeyLength + 8] = 0x54; // 'T'
			nLen = nKeyLength + 9;
		}
		else
		{
			nLen = nKeyLength + 5;
		}
		MD5(m_sObjectKey, nLen, m_sObjectKey);

		if ((m_nObjectKeyLength = nKeyLength + 5) > 16)
		{
			m_nObjectKeyLength = 16;
		}
	}

	DecryptStream::~DecryptStream()
	{
		delete m_pStream;
	}

	void DecryptStream::Reset()
	{
		m_pStream->Reset();
		switch (m_eCryptType)
		{
			case cryptRC4:
			m_oState.oRC4.unX = m_oState.oRC4.unY = 0;
			RC4InitKey(m_sObjectKey, m_nObjectKeyLength, m_oState.oRC4.sState);
			m_oState.oRC4.nBuffer = EOF;
			break;
			case cryptAES:
			AESKeyExpansion(&m_oState.oAES, m_sObjectKey, m_nObjectKeyLength);
			for (int nIndex = 0; nIndex < 16; ++nIndex)
			{
				m_oState.oAES.sCBC[nIndex] = m_pStream->GetChar();
			}
			m_oState.oAES.nBufferIndex = 16;
			break;
		}
	}

	int DecryptStream::GetChar()
	{
		unsigned char sIn[16];
		int nChar = EOF;

		switch (m_eCryptType)
		{
			case cryptRC4:
			if (m_oState.oRC4.nBuffer == EOF)
			{
				nChar = m_pStream->GetChar();
				if (nChar != EOF)
				{
					m_oState.oRC4.nBuffer = RC4DecryptByte(m_oState.oRC4.sState, &m_oState.oRC4.unX, &m_oState.oRC4.unY, (unsigned char)nChar);
				}
			}
			nChar = m_oState.oRC4.nBuffer;
			m_oState.oRC4.nBuffer = EOF;
			break;
			case cryptAES:
			if (m_oState.oAES.nBufferIndex == 16)
			{
				for (int nIndex = 0; nIndex < 16; ++nIndex)
				{
					if ((nChar = m_pStream->GetChar()) == EOF)
					{
						return EOF;
					}
					sIn[nIndex] = (unsigned char)nChar;
				}
				AESDecryptBlock(&m_oState.oAES, sIn, m_pStream->LookChar() == EOF);
			}
            if (m_oState.oAES.nBufferIndex >= 16)
			{
				nChar = EOF;
			}
			else
			{
				nChar = m_oState.oAES.sBuffer[m_oState.oAES.nBufferIndex++];
			}
			break;
		}
		return nChar;
	}

	int DecryptStream::LookChar()
	{
		unsigned char sIn[16];
		int nChar = EOF;
		switch (m_eCryptType)
		{
			case cryptRC4:
			if (m_oState.oRC4.nBuffer == EOF)
			{
				nChar = m_pStream->GetChar();
				if (nChar != EOF)
				{
					m_oState.oRC4.nBuffer = RC4DecryptByte(m_oState.oRC4.sState, &m_oState.oRC4.unX, &m_oState.oRC4.unY, (unsigned char)nChar);
				}
			}
			nChar = m_oState.oRC4.nBuffer;
			break;
			case cryptAES:
			if (m_oState.oAES.nBufferIndex == 16)
			{
				for (int nIndex = 0; nIndex < 16; ++nIndex)
				{
					if ((nChar = m_pStream->GetChar()) == EOF)
					{
						return EOF;
					}
					sIn[nIndex] = nChar;
				}
				AESDecryptBlock(&m_oState.oAES, sIn, m_pStream->LookChar() == EOF);
			}
			if (m_oState.oAES.nBufferIndex == 16)
			{
				nChar = EOF;
			}
			else
			{
				nChar = m_oState.oAES.sBuffer[m_oState.oAES.nBufferIndex];
			}
			break;
		}
		return nChar;
	}

	bool DecryptStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(bLast);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// RC4
	//-------------------------------------------------------------------------------------------------------------------------------

	static void RC4InitKey(unsigned char *sKey, int nKeyLen, unsigned char *sState)
	{
		for (int nIndex = 0; nIndex < 256; ++nIndex)
			sState[nIndex] = nIndex;

		//unsigned char unIndex1 = 0, unIndex2 = 0;
		//for ( int nIndex = 0; nIndex < 256; ++nIndex ) 
		//{
		//	unIndex2 = ( sKey[unIndex1] + sState[nIndex] + unIndex2 ) % 256;
		//	unsigned char unTemp = sState[nIndex];
		//	sState[nIndex] = sState[unIndex2];
		//	sState[unIndex2] = nIndex;
		//	unIndex1 = (unIndex1 + 1) % nKeyLen;
		//}

		for (int i = 0, j = 0; i < 256; i++)
		{
			j = (j + sKey[i % nKeyLen] + sState[i]) % 256;
			unsigned char unTemp = sState[i];
			sState[i] = sState[j];
			sState[j] = unTemp;
		}
	}

	static unsigned char RC4DecryptByte(unsigned char *sState, unsigned char *pX, unsigned char *pY, unsigned char nChar)
	{
		unsigned char unX1 = *pX = (*pX + 1) % 256;
		unsigned char unY1 = *pY = (sState[*pX] + *pY) % 256;
		unsigned char unTempX = sState[unX1];
		unsigned char unTempY = sState[unY1];
		sState[unX1] = unTempY;
		sState[unY1] = unTempX;
		return nChar ^ sState[(unTempX + unTempY) % 256];
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// AES
	//-------------------------------------------------------------------------------------------------------------------------------

	static unsigned char c_sSbox[256] =
	{
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};

	static unsigned char c_sInvSbox[256] =
	{
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
	};

	static unsigned int c_arrRCon[11] =
	{
		0x00000000, // не используется
		0x01000000,
		0x02000000,
		0x04000000,
		0x08000000,
		0x10000000,
		0x20000000,
		0x40000000,
		0x80000000,
		0x1b000000,
		0x36000000
	};

	static inline unsigned int SubWord(unsigned int unValue)
	{
		return (c_sSbox[unValue >> 24] << 24) | (c_sSbox[(unValue >> 16) & 0xff] << 16) | (c_sSbox[(unValue >> 8) & 0xff] << 8) | c_sSbox[unValue & 0xff];
	}

	static inline unsigned int RotWord(unsigned int unValue)
	{
		return ((unValue << 8) & 0xffffffff) | (unValue >> 24);
	}

	static inline void InvSubBytes(unsigned char *sState)
	{
		for (int nIndex = 0; nIndex < 16; ++nIndex)
		{
			sState[nIndex] = c_sInvSbox[sState[nIndex]];
		}
	}

	static inline void InvShiftRows(unsigned char *sState)
	{
		unsigned char unTemp = 0;

		unTemp    = sState[7];
		sState[7] = sState[6];
		sState[6] = sState[5];
		sState[5] = sState[4];
		sState[4] = unTemp;

		unTemp     = sState[8];
		sState[8]  = sState[10];
		sState[10] = unTemp;

		unTemp     = sState[9];
		sState[9]  = sState[11];
		sState[11] = unTemp;

		unTemp     = sState[12];
		sState[12] = sState[13];
		sState[13] = sState[14];
		sState[14] = sState[15];
		sState[15] = unTemp;
	}

	// 09 ... nChar
	static inline unsigned char Mult09(unsigned char nChar)
	{
		unsigned char nChar2 = (nChar & 0x80) ? ((nChar << 1) ^ 0x1b) : (nChar << 1);
		unsigned char nChar4 = (nChar2 & 0x80) ? ((nChar2 << 1) ^ 0x1b) : (nChar2 << 1);
		unsigned char nChar8 = (nChar4 & 0x80) ? ((nChar4 << 1) ^ 0x1b) : (nChar4 << 1);

		return nChar ^ nChar8;
	}

	// 0b ... nChar
	static inline unsigned char Mult0b(unsigned char nChar)
	{
		unsigned char nChar2 = (nChar & 0x80) ? ((nChar << 1) ^ 0x1b) : (nChar << 1);
		unsigned char nChar4 = (nChar2 & 0x80) ? ((nChar2 << 1) ^ 0x1b) : (nChar2 << 1);
		unsigned char nChar8 = (nChar4 & 0x80) ? ((nChar4 << 1) ^ 0x1b) : (nChar4 << 1);
		return nChar ^ nChar2 ^ nChar8;
	}

	// 0d ... nChar
	static inline unsigned char Mult0d(unsigned char nChar)
	{
		unsigned char nChar2 = (nChar & 0x80) ? ((nChar << 1) ^ 0x1b) : (nChar << 1);
		unsigned char nChar4 = (nChar2 & 0x80) ? ((nChar2 << 1) ^ 0x1b) : (nChar2 << 1);
		unsigned char nChar8 = (nChar4 & 0x80) ? ((nChar4 << 1) ^ 0x1b) : (nChar4 << 1);
		return nChar ^ nChar4 ^ nChar8;
	}

	// 0e ... nChar
	static inline unsigned char Mult0e(unsigned char nChar)
	{
		unsigned char nChar2 = (nChar & 0x80) ? ((nChar << 1) ^ 0x1b) : (nChar << 1);
		unsigned char nChar4 = (nChar2 & 0x80) ? ((nChar2 << 1) ^ 0x1b) : (nChar2 << 1);
		unsigned char nChar8 = (nChar4 & 0x80) ? ((nChar4 << 1) ^ 0x1b) : (nChar4 << 1);
		return nChar2 ^ nChar4 ^ nChar8;
	}

	static inline void InvMixColumns(unsigned char *sState)
	{
		for (int nChar = 0; nChar < 4; ++nChar)
		{
			unsigned char unS0  = sState[nChar + 0];
			unsigned char unS4  = sState[nChar + 4];
			unsigned char unS8  = sState[nChar + 8];
			unsigned char unS12 = sState[nChar + 12];
			sState[nChar + 0] = Mult0e(unS0) ^ Mult0b(unS4) ^ Mult0d(unS8) ^ Mult09(unS12);
			sState[nChar + 4] = Mult09(unS0) ^ Mult0e(unS4) ^ Mult0b(unS8) ^ Mult0d(unS12);
			sState[nChar + 8] = Mult0d(unS0) ^ Mult09(unS4) ^ Mult0e(unS8) ^ Mult0b(unS12);
			sState[nChar + 12] = Mult0b(unS0) ^ Mult0d(unS4) ^ Mult09(unS8) ^ Mult0e(unS12);
		}
	}

	static inline void InvMixColumnsW(unsigned int *pW)
	{

		for (int nChar = 0; nChar < 4; ++nChar)
		{
			unsigned char unS0 = pW[nChar] >> 24;
			unsigned char unS1 = pW[nChar] >> 16;
			unsigned char unS2 = pW[nChar] >> 8;
			unsigned char unS3 = pW[nChar];
			pW[nChar] =  ((Mult0e(unS0) ^ Mult0b(unS1) ^ Mult0d(unS2) ^ Mult09(unS3)) << 24)
				| ((Mult09(unS0) ^ Mult0e(unS1) ^ Mult0b(unS2) ^ Mult0d(unS3)) << 16)
				| ((Mult0d(unS0) ^ Mult09(unS1) ^ Mult0e(unS2) ^ Mult0b(unS3)) << 8)
				| (Mult0b(unS0) ^ Mult0d(unS1) ^ Mult09(unS2) ^ Mult0e(unS3));
		}
	}

	static inline void AddRoundKey(unsigned char *sState, unsigned int *pW)
	{
		for (int nChar = 0; nChar < 4; ++nChar)
		{
			sState[nChar + 0] ^= pW[nChar] >> 24;
			sState[nChar + 4] ^= pW[nChar] >> 16;
			sState[nChar + 8] ^= pW[nChar] >> 8;
			sState[nChar + 12] ^= pW[nChar];
		}
	}

	static void AESKeyExpansion(DecryptAESState *pState, unsigned char *sObjectKey, int nObjectKeyLen)
	{
		//Предполагается, что nObjectKeyLen == 16

		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			pState->arrW[nIndex] = (sObjectKey[4 * nIndex] << 24) + (sObjectKey[4 * nIndex + 1] << 16) + (sObjectKey[4 * nIndex + 2] << 8) + sObjectKey[4 * nIndex + 3];
		}
		for (int nIndex = 4; nIndex < 44; ++nIndex)
		{
			unsigned int unTemp = pState->arrW[nIndex - 1];
			if (!(nIndex & 3))
			{
				unTemp = SubWord(RotWord(unTemp)) ^ c_arrRCon[nIndex / 4];
			}
			pState->arrW[nIndex] = pState->arrW[nIndex - 4] ^ unTemp;
		}
		for (int nRound = 1; nRound <= 9; ++nRound)
		{
			InvMixColumnsW(&pState->arrW[nRound * 4]);
		}
	}

	static void AESDecryptBlock(DecryptAESState *pState, unsigned char *sIn, bool bLast)
	{
		// Начальное состояние
		for (int nChar = 0; nChar < 4; ++nChar)
		{
			pState->sState[nChar + 0] = sIn[4 * nChar + 0];
			pState->sState[nChar + 4] = sIn[4 * nChar + 1];
			pState->sState[nChar + 8] = sIn[4 * nChar + 2];
			pState->sState[nChar + 12] = sIn[4 * nChar + 3];
		}

		// Round 0
		AddRoundKey(pState->sState, &pState->arrW[10 * 4]);

		// Round 1-9
		for (int nRound = 9; nRound >= 1; --nRound)
		{
			InvSubBytes(pState->sState);
			InvShiftRows(pState->sState);
			InvMixColumns(pState->sState);
			AddRoundKey(pState->sState, &pState->arrW[nRound * 4]);
		}

		// Round 10
		InvSubBytes(pState->sState);
		InvShiftRows(pState->sState);
		AddRoundKey(pState->sState, &pState->arrW[0]);

		// CBC
		for (int nChar = 0; nChar < 4; ++nChar)
		{
			pState->sBuffer[4 * nChar + 0] = pState->sState[nChar + 0] ^ pState->sCBC[4 * nChar + 0];
			pState->sBuffer[4 * nChar + 1] = pState->sState[nChar + 4] ^ pState->sCBC[4 * nChar + 1];
			pState->sBuffer[4 * nChar + 2] = pState->sState[nChar + 8] ^ pState->sCBC[4 * nChar + 2];
			pState->sBuffer[4 * nChar + 3] = pState->sState[nChar + 12] ^ pState->sCBC[4 * nChar + 3];
		}

		// сохраняем блок с следующий CBC
		for (int nIndex = 0; nIndex < 16; ++nIndex)
		{
			pState->sCBC[nIndex] = sIn[nIndex];
		}

		// remove padding
		pState->nBufferIndex = 0;
		if (bLast)
		{
			int nLen = pState->sBuffer[15];
			for (int nIndex = 15; nIndex >= nLen; --nIndex)
			{
				pState->sBuffer[nIndex] = pState->sBuffer[nIndex - nLen];
			}
			pState->nBufferIndex = nLen;
		}
	}

	//------------------------------------------------------------------------
	// MD5 message digest
	//------------------------------------------------------------------------

	static inline unsigned long RotateLeft(unsigned long x, int r)
	{
		x &= 0xffffffff;
		return ((x << r) | (x >> (32 - r))) & 0xffffffff;
	}

	static inline unsigned long MD5Round1(unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long Xk, unsigned long s, unsigned long Ti)
	{
		return b + RotateLeft((a + ((b & c) | (~b & d)) + Xk + Ti), s);
	}

	static inline unsigned long MD5Round2(unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long Xk, unsigned long s, unsigned long Ti)
	{
		return b + RotateLeft((a + ((b & d) | (c & ~d)) + Xk + Ti), s);
	}

	static inline unsigned long MD5Round3(unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long Xk, unsigned long s, unsigned long Ti)
	{
		return b + RotateLeft((a + (b ^ c ^ d) + Xk + Ti), s);
	}

	static inline unsigned long MD5Round4(unsigned long a, unsigned long b, unsigned long c, unsigned long d, unsigned long Xk, unsigned long s, unsigned long Ti)
	{
		return b + RotateLeft((a + (c ^ (b | ~d)) + Xk + Ti), s);
	}

	static void MD5(unsigned char *sMessage, int nMessageLen, unsigned char *sDigest)
	{
		unsigned long x[16];
		unsigned long a, b, c, d, aa, bb, cc, dd;
		int i, j, k;

		// Вычислим количество блоков 64x64
		// ( nMessageLen + pad byte (0x80) + 8 bytes for length )
		int nBlocksCount = (nMessageLen + 1 + 8 + 63) / 64;

		// Инициализируем a, b, c, d
		a = 0x67452301;
		b = 0xefcdab89;
		c = 0x98badcfe;
		d = 0x10325476;

		k = 0;
		for (i = 0; i < nBlocksCount; ++i)
		{
			// Считываем один блок
			for (j = 0; j < 16 && k < nMessageLen - 3; ++j, k += 4)
				x[j] = (((((sMessage[k + 3] << 8) + sMessage[k + 2]) << 8) + sMessage[k + 1]) << 8) + sMessage[k];
			if (i == nBlocksCount - 1)
			{
				if (k == nMessageLen - 3)
					x[j] = 0x80000000 + (((sMessage[k + 2] << 8) + sMessage[k + 1]) << 8) + sMessage[k];
				else if (k == nMessageLen - 2)
					x[j] = 0x800000 + (sMessage[k + 1] << 8) + sMessage[k];
				else if (k == nMessageLen - 1)
					x[j] = 0x8000 + sMessage[k];
				else
					x[j] = 0x80;
				++j;
				while (j < 16)
					x[j++] = 0;
				x[14] = nMessageLen << 3;
			}

			// Сохраняем a, b, c, d
			aa = a;
			bb = b;
			cc = c;
			dd = d;

			// Round 1
			a = MD5Round1(a, b, c, d, x[0], 7, 0xd76aa478);
			d = MD5Round1(d, a, b, c, x[1], 12, 0xe8c7b756);
			c = MD5Round1(c, d, a, b, x[2], 17, 0x242070db);
			b = MD5Round1(b, c, d, a, x[3], 22, 0xc1bdceee);
			a = MD5Round1(a, b, c, d, x[4], 7, 0xf57c0faf);
			d = MD5Round1(d, a, b, c, x[5], 12, 0x4787c62a);
			c = MD5Round1(c, d, a, b, x[6], 17, 0xa8304613);
			b = MD5Round1(b, c, d, a, x[7], 22, 0xfd469501);
			a = MD5Round1(a, b, c, d, x[8], 7, 0x698098d8);
			d = MD5Round1(d, a, b, c, x[9], 12, 0x8b44f7af);
			c = MD5Round1(c, d, a, b, x[10], 17, 0xffff5bb1);
			b = MD5Round1(b, c, d, a, x[11], 22, 0x895cd7be);
			a = MD5Round1(a, b, c, d, x[12], 7, 0x6b901122);
			d = MD5Round1(d, a, b, c, x[13], 12, 0xfd987193);
			c = MD5Round1(c, d, a, b, x[14], 17, 0xa679438e);
			b = MD5Round1(b, c, d, a, x[15], 22, 0x49b40821);

			// Round 2
			a = MD5Round2(a, b, c, d, x[1], 5, 0xf61e2562);
			d = MD5Round2(d, a, b, c, x[6], 9, 0xc040b340);
			c = MD5Round2(c, d, a, b, x[11], 14, 0x265e5a51);
			b = MD5Round2(b, c, d, a, x[0], 20, 0xe9b6c7aa);
			a = MD5Round2(a, b, c, d, x[5], 5, 0xd62f105d);
			d = MD5Round2(d, a, b, c, x[10], 9, 0x02441453);
			c = MD5Round2(c, d, a, b, x[15], 14, 0xd8a1e681);
			b = MD5Round2(b, c, d, a, x[4], 20, 0xe7d3fbc8);
			a = MD5Round2(a, b, c, d, x[9], 5, 0x21e1cde6);
			d = MD5Round2(d, a, b, c, x[14], 9, 0xc33707d6);
			c = MD5Round2(c, d, a, b, x[3], 14, 0xf4d50d87);
			b = MD5Round2(b, c, d, a, x[8], 20, 0x455a14ed);
			a = MD5Round2(a, b, c, d, x[13], 5, 0xa9e3e905);
			d = MD5Round2(d, a, b, c, x[2], 9, 0xfcefa3f8);
			c = MD5Round2(c, d, a, b, x[7], 14, 0x676f02d9);
			b = MD5Round2(b, c, d, a, x[12], 20, 0x8d2a4c8a);

			// Round 3
			a = MD5Round3(a, b, c, d, x[5], 4, 0xfffa3942);
			d = MD5Round3(d, a, b, c, x[8], 11, 0x8771f681);
			c = MD5Round3(c, d, a, b, x[11], 16, 0x6d9d6122);
			b = MD5Round3(b, c, d, a, x[14], 23, 0xfde5380c);
			a = MD5Round3(a, b, c, d, x[1], 4, 0xa4beea44);
			d = MD5Round3(d, a, b, c, x[4], 11, 0x4bdecfa9);
			c = MD5Round3(c, d, a, b, x[7], 16, 0xf6bb4b60);
			b = MD5Round3(b, c, d, a, x[10], 23, 0xbebfbc70);
			a = MD5Round3(a, b, c, d, x[13], 4, 0x289b7ec6);
			d = MD5Round3(d, a, b, c, x[0], 11, 0xeaa127fa);
			c = MD5Round3(c, d, a, b, x[3], 16, 0xd4ef3085);
			b = MD5Round3(b, c, d, a, x[6], 23, 0x04881d05);
			a = MD5Round3(a, b, c, d, x[9], 4, 0xd9d4d039);
			d = MD5Round3(d, a, b, c, x[12], 11, 0xe6db99e5);
			c = MD5Round3(c, d, a, b, x[15], 16, 0x1fa27cf8);
			b = MD5Round3(b, c, d, a, x[2], 23, 0xc4ac5665);

			// Round 4
			a = MD5Round4(a, b, c, d, x[0], 6, 0xf4292244);
			d = MD5Round4(d, a, b, c, x[7], 10, 0x432aff97);
			c = MD5Round4(c, d, a, b, x[14], 15, 0xab9423a7);
			b = MD5Round4(b, c, d, a, x[5], 21, 0xfc93a039);
			a = MD5Round4(a, b, c, d, x[12], 6, 0x655b59c3);
			d = MD5Round4(d, a, b, c, x[3], 10, 0x8f0ccc92);
			c = MD5Round4(c, d, a, b, x[10], 15, 0xffeff47d);
			b = MD5Round4(b, c, d, a, x[1], 21, 0x85845dd1);
			a = MD5Round4(a, b, c, d, x[8], 6, 0x6fa87e4f);
			d = MD5Round4(d, a, b, c, x[15], 10, 0xfe2ce6e0);
			c = MD5Round4(c, d, a, b, x[6], 15, 0xa3014314);
			b = MD5Round4(b, c, d, a, x[13], 21, 0x4e0811a1);
			a = MD5Round4(a, b, c, d, x[4], 6, 0xf7537e82);
			d = MD5Round4(d, a, b, c, x[11], 10, 0xbd3af235);
			c = MD5Round4(c, d, a, b, x[2], 15, 0x2ad7d2bb);
			b = MD5Round4(b, c, d, a, x[9], 21, 0xeb86d391);

			// Увеличиваем a, b, c, d
			a += aa;
			b += bb;
			c += cc;
			d += dd;
		}

		// Разбиваем sDigest на байты
		sDigest[0]  = (unsigned char)(a & 0xff);
		sDigest[1]  = (unsigned char)((a >>= 8) & 0xff);
		sDigest[2]  = (unsigned char)((a >>= 8) & 0xff);
		sDigest[3]  = (unsigned char)((a >>= 8) & 0xff);
		sDigest[4]  = (unsigned char)(b & 0xff);
		sDigest[5]  = (unsigned char)((b >>= 8) & 0xff);
		sDigest[6]  = (unsigned char)((b >>= 8) & 0xff);
		sDigest[7]  = (unsigned char)((b >>= 8) & 0xff);
		sDigest[8]  = (unsigned char)(c & 0xff);
		sDigest[9]  = (unsigned char)((c >>= 8) & 0xff);
		sDigest[10] = (unsigned char)((c >>= 8) & 0xff);
		sDigest[11] = (unsigned char)((c >>= 8) & 0xff);
		sDigest[12] = (unsigned char)(d & 0xff);
		sDigest[13] = (unsigned char)((d >>= 8) & 0xff);
		sDigest[14] = (unsigned char)((d >>= 8) & 0xff);
		sDigest[15] = (unsigned char)((d >>= 8) & 0xff);
	}
}

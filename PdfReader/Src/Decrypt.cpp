/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "SecurityHandler.h"

#include "../../Common/3dParty/cryptopp/modes.h"
#include "../../Common/3dParty/cryptopp/aes.h"
#include "../../Common/3dParty/cryptopp/sha.h"
#include "../../Common/3dParty/cryptopp/md5.h"
#include "../../Common/3dParty/cryptopp/arc4.h"
#include "../../Common/3dParty/cryptopp/filters.h"

#include "../../DesktopEditor/common/File.h"
#include "../../UnicodeConverter/UnicodeConverter.h"
namespace PdfReader
{
	static void MD5(unsigned char *sMessage, int nMessageLen, unsigned char *sDigest)
	{
		CryptoPP::MD5 hash;
		
		hash.Update( sMessage, nMessageLen);

		CryptoPP::SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		memcpy(sDigest, buffer.BytePtr(), buffer.size());
		return;
	}
	static int SHA(int type, unsigned char *sMessage, int nMessageLen, unsigned char *sDigest)
	{
		int res = 0;
		switch(type)
		{
			case 0:
			case 256:
			{
				CryptoPP::SHA256 hash;			
				hash.Update( sMessage, nMessageLen > 0 ? nMessageLen : hash.DigestSize());

				CryptoPP::SecByteBlock buffer(res = hash.DigestSize());
				hash.Final(buffer);

				memcpy(sDigest, buffer.BytePtr(), buffer.size());
			}break;
			case 1:
			case 384:
			{
				CryptoPP::SHA384 hash;			
				hash.Update( sMessage, nMessageLen > 0 ? nMessageLen : hash.DigestSize());

				CryptoPP::SecByteBlock buffer(res = hash.DigestSize());
				hash.Final(buffer);

				memcpy(sDigest, buffer.BytePtr(), buffer.size());
			}break;
			case 2:
			case 512:
			{
				CryptoPP::SHA512 hash;			
				hash.Update( sMessage, nMessageLen > 0 ? nMessageLen : hash.DigestSize());

				CryptoPP::SecByteBlock buffer(res = hash.DigestSize());
				hash.Final(buffer);

				memcpy(sDigest, buffer.BytePtr(), buffer.size());
			}break;
		}
		return res;
	}
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

	bool Decrypt::MakeFileKey(StandardSecurityHandler *handler, const std::wstring &wsOwnerPassword, const std::wstring &wsUserPassword)
	{
		if (!handler) return false;
		handler->m_bOwnerPasswordValid = false;
		
		if ( handler->m_nEncryptRevision < 5 )
		{
			// Попытаемся, используя пароль владельца, сгенерировать пользовательский пароль
			if (false == wsOwnerPassword.empty())
			{				
				std::string sOwnerPassword = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsOwnerPassword);
				int nLen = sOwnerPassword.length();
				unsigned char arrOwnerPass[32];
				if (nLen < 32)
				{
					memcpy(arrOwnerPass, sOwnerPassword.c_str(), nLen);
					memcpy(arrOwnerPass + nLen, passwordPad, 32 - nLen);
				}
				else
				{
					memcpy(arrOwnerPass, sOwnerPassword.c_str(), 32);
				}
				MD5(arrOwnerPass, 32, arrOwnerPass);

				if (handler->m_nEncryptRevision >= 3)
				{
					for (int nIndex = 0; nIndex < 50; ++nIndex)
					{
						MD5(arrOwnerPass, 16, arrOwnerPass);
					}
				}

				unsigned char arrOwnerKey[32];
				unsigned char arrFState[256];
				if (handler->m_nEncryptRevision == 2)
				{
					CryptoPP::ARC4::Decryption rc4Decryption;
					rc4Decryption.SetKey(arrOwnerPass, handler->m_nFileKeyLength);

					rc4Decryption.ProcessData(arrOwnerKey, (unsigned char*)handler->m_seOwnerKey->GetBuffer(), 32);
				}
				else
				{
					memcpy(arrOwnerKey, handler->m_seOwnerKey->GetBuffer(), 32);
					for (int nIndex = 19; nIndex >= 0; --nIndex)
					{
						unsigned char arrTempKey[16];
						for (int nJ = 0; nJ < handler->m_nFileKeyLength; ++nJ)
						{
							arrTempKey[nJ] = arrOwnerPass[nJ] ^ nIndex;
						}
						CryptoPP::ARC4::Decryption rc4Decryption;

						rc4Decryption.SetKey(arrTempKey, handler->m_nFileKeyLength);
						rc4Decryption.ProcessData(arrOwnerKey, arrOwnerKey, 32);
					}
				}
				std::string sUserPassword2((char *)arrOwnerKey, 32);
				
				if (MakeFileKey2(handler, sUserPassword2))
				{
					handler->m_bOwnerPasswordValid = true;
					return true;
				}
			}
			// Попытаемся использовать пользовательский пароль
			std::string sUserPassword = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsUserPassword);
			return MakeFileKey2(handler, sUserPassword);
		}
		else
		{
			bool bValidate = false;
			unsigned char empty[16];

			NSUnicodeConverter::CUnicodeConverter conv;
			std::string sUserPassword = conv.SASLprepToUtf8(wsUserPassword);
			
			if (sUserPassword.length() > 127)
				sUserPassword = sUserPassword.substr(0, 127);
			
			CryptoPP::SHA256 hash;

			hash.Update( (unsigned char*) sUserPassword.c_str(), sUserPassword.length());
			hash.Update( handler->m_seUserKey->GetUBuffer() + 32, 8);

			CryptoPP::SecByteBlock buffer(hash.DigestSize());
			hash.Final(buffer);

			if ( handler->m_nEncryptRevision > 5 )
			{
				MakeFileKey3(sUserPassword, buffer.BytePtr(), buffer.size());
			}

			bValidate = (0 == memcmp(buffer.BytePtr(), handler->m_seUserKey->GetUBuffer(), 32));

			if (bValidate)
			{
				hash.Update( (unsigned char*) sUserPassword.c_str(), sUserPassword.length());
				hash.Update( handler->m_seUserKey->GetUBuffer() + 40, 8);

				CryptoPP::SecByteBlock buffer(hash.DigestSize());
				hash.Final(buffer);

				if ( handler->m_nEncryptRevision > 5 )
				{
					MakeFileKey3(sUserPassword, buffer.BytePtr(), buffer.size());
				}

				memset(empty, 0, 16);
				CryptoPP::AES::Decryption aesDecryption(buffer.BytePtr(), buffer.size());
				CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, empty );

				CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::ArraySink( (unsigned char*)handler->m_sFileKey, 32), CryptoPP::StreamTransformationFilter::NO_PADDING );
				stfDecryptor.Put( handler->m_seUserEncryptionKey->GetUBuffer(), 32);
				stfDecryptor.MessageEnd();
			}
			else
			{
				std::string sOwnerPassword = conv.SASLprepToUtf8(wsOwnerPassword);
				if (sOwnerPassword.length() > 127)
					sOwnerPassword = sOwnerPassword.substr(0, 127);
				
				hash.Update( (unsigned char*) sOwnerPassword.c_str(), sOwnerPassword.length());
				hash.Update( handler->m_seOwnerKey->GetUBuffer() + 32, 8);
				hash.Update( handler->m_seUserKey->GetUBuffer(), 48);

				CryptoPP::SecByteBlock buffer(hash.DigestSize());
				hash.Final(buffer);

				if ( handler->m_nEncryptRevision > 5 )
				{
					MakeFileKey3(sOwnerPassword, buffer.BytePtr(), buffer.size(), handler->m_seUserKey->GetUBuffer(), 48);
				}

				bValidate = (0 == memcmp(buffer.BytePtr(), handler->m_seOwnerKey->GetUBuffer(), 32));

				if (bValidate)
				{
					hash.Update( (unsigned char*) sOwnerPassword.c_str(), sOwnerPassword.length());
					hash.Update( handler->m_seOwnerKey->GetUBuffer() + 40, 8);
					hash.Update( handler->m_seUserKey->GetUBuffer(), 48);

					CryptoPP::SecByteBlock buffer(hash.DigestSize());
					hash.Final(buffer);

					if ( handler->m_nEncryptRevision > 5 )
					{
						MakeFileKey3(sOwnerPassword, buffer.BytePtr(), buffer.size(), handler->m_seUserKey->GetUBuffer(), 48);
					}
					memset(empty, 0, 16);					
					CryptoPP::AES::Decryption aesDecryption(buffer.BytePtr(), buffer.size());
					CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, empty );

					CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::ArraySink( (unsigned char*)handler->m_sFileKey, 32), CryptoPP::StreamTransformationFilter::NO_PADDING );
					stfDecryptor.Put(handler->m_seOwnerEncryptionKey->GetUBuffer(), 32);
					stfDecryptor.MessageEnd();
				}
			}
			handler->m_bOwnerPasswordValid = bValidate;
			return bValidate;
		}
	}

	bool Decrypt::MakeFileKey3(const std::string &sPassword, unsigned char *pHash, int nHashSize, unsigned char *pHash2, int nHashSize2)
	{
		if (!pHash) return false;
	
		int size = 64 * (sPassword.length() + 64 + nHashSize2); // max
				
		unsigned char K[64];	//max size sha
		unsigned char *K1 = new unsigned char[size];
		unsigned char *E = new unsigned char[size];

		int hash_size = nHashSize;
		memcpy(K, pHash, nHashSize);
				
		int iteration = 0;			
		
		while( (iteration < 64) || (iteration < E[size - 1] + 32))
		{
			CryptoPP::SecByteBlock key(K, 16), iv(K + 16, 16);

			size = 0;
			for (int i = 0; i < 64; i++)
			{
				memcpy(K1 + size, sPassword.c_str(), sPassword.length()); size += sPassword.length();
				memcpy(K1 + size, K, hash_size); size += hash_size;
				if (pHash2)
				{
					memcpy(K1 + size, pHash2, nHashSize2);	size += nHashSize2;
				}
			}
			CryptoPP::AES::Encryption aesEncryption(key, key.size());
			CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv);
			
			CryptoPP::StreamTransformationFilter stfEncryption(cbcEncryption, new CryptoPP::ArraySink( E, size), CryptoPP::StreamTransformationFilter::NO_PADDING);
		 
			stfEncryption.Put( K1, size);
			stfEncryption.MessageEnd();
//----------------------------------------------------------
			int E_mod_3 = 0;
            for (unsigned int i = 0; i < 16; ++i)
            {
                E_mod_3 += E[i];
            }
            E_mod_3 %= 3;

			hash_size = SHA(E_mod_3, E, size, K);

			iteration++;
		}

		delete []K1;
		delete []E;

		memcpy (pHash, K, 32); // pHash - from sha256
		return true;
	}

	bool Decrypt::MakeFileKey2(StandardSecurityHandler *handler, const std::string &sUserPassword)
	{
		if (!handler) return false;

		unsigned char sTest[32];
		unsigned char sFState[256];
		unsigned char sTempKey[16];
		unsigned char unFx, unFy;
		int nLen = 0;
		bool bResult = true;

		unsigned char *pBuffer = (unsigned char *)MemUtilsMalloc(72 + handler->m_seFileID->GetLength());
		
		if (false == sUserPassword.empty())
		{
			nLen = sUserPassword.length();
			if (nLen < 32)
			{
				memcpy(pBuffer, sUserPassword.c_str(), nLen);
				memcpy(pBuffer + nLen, passwordPad, 32 - nLen);
			}
			else
			{
				memcpy(pBuffer, sUserPassword.c_str(), 32);
			}
		}
		else
		{
			memcpy(pBuffer, passwordPad, 32);
		}

		memcpy(pBuffer + 32, handler->m_seOwnerKey->GetBuffer(), 32);
		
		pBuffer[64] =  handler->m_nPermissionFlags & 0xff;
		pBuffer[65] = (handler->m_nPermissionFlags >> 8) & 0xff;
		pBuffer[66] = (handler->m_nPermissionFlags >> 16) & 0xff;
		pBuffer[67] = (handler->m_nPermissionFlags >> 24) & 0xff;

		memcpy(pBuffer + 68, handler->m_seFileID->GetBuffer(), handler->m_seFileID->GetLength());
		nLen = 68 + handler->m_seFileID->GetLength();
		if (!handler->m_bEncryptMetadata)
		{
			pBuffer[nLen++] = 0xff;
			pBuffer[nLen++] = 0xff;
			pBuffer[nLen++] = 0xff;
			pBuffer[nLen++] = 0xff;
		}
		MD5(pBuffer, nLen, handler->m_sFileKey);
		if (handler->m_nEncryptRevision >= 3)
		{
			for (int nIndex = 0; nIndex < 50; ++nIndex)
			{
				MD5(handler->m_sFileKey, handler->m_nFileKeyLength, handler->m_sFileKey);
			}
		}

		if (handler->m_nEncryptRevision == 2)
		{
			CryptoPP::ARC4::Decryption rc4Decryption;
			rc4Decryption.SetKey(handler->m_sFileKey, handler->m_nFileKeyLength);

			rc4Decryption.ProcessData(sTest, (unsigned char*)handler->m_seUserKey->GetBuffer(), 32);
			
			bResult = (memcmp(sTest, passwordPad, 32) == 0);
		}
		else if (handler->m_nEncryptRevision >= 3)
		{
			memcpy(sTest, handler->m_seUserKey->GetBuffer(), 32);
			for (int nIndex = 19; nIndex >= 0; --nIndex)
			{
				for (int nJ = 0; nJ < handler->m_nFileKeyLength; ++nJ)
				{
					sTempKey[nJ] = handler->m_sFileKey[nJ] ^ nIndex;
				}
				CryptoPP::ARC4::Decryption rc4Decryption;

				rc4Decryption.SetKey(sTempKey, handler->m_nFileKeyLength);
				rc4Decryption.ProcessData(sTest, sTest, 32);
			}
			memcpy(pBuffer, passwordPad, 32);
			memcpy(pBuffer + 32, handler->m_seFileID->GetBuffer(), handler->m_seFileID->GetLength());
			
			MD5(pBuffer, 32 + handler->m_seFileID->GetLength(), pBuffer);
			
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
		FilterStream(pStream), m_pUncryptedData(NULL), m_pUncryptedDataSize(0), m_nObjectKeyLength(nKeyLength)
	{
		m_eCryptType = eType;
		memcpy(m_sObjectKey, sFileKey, nKeyLength);

		if (m_eCryptType != cryptAES256)
		{
			m_sObjectKey[nKeyLength + 0] =  nObjectNum & 0xff;
			m_sObjectKey[nKeyLength + 1] = (nObjectNum >> 8) & 0xff;
			m_sObjectKey[nKeyLength + 2] = (nObjectNum >> 16) & 0xff;
			m_sObjectKey[nKeyLength + 3] =  nObjectGen & 0xff;
			m_sObjectKey[nKeyLength + 4] = (nObjectGen >> 8) & 0xff;

			int nLen = 0;
			if (m_eCryptType == cryptAES128)
			{
				m_sObjectKey[nKeyLength + 5] = 0x73; // 's'
				m_sObjectKey[nKeyLength + 6] = 0x41; // 'A'
				m_sObjectKey[nKeyLength + 7] = 0x6c; // 'l'
				m_sObjectKey[nKeyLength + 8] = 0x54; // 'T'
				
				nLen = nKeyLength + 9;
			}
			else if (m_eCryptType == cryptRC4)
			{
				nLen = nKeyLength + 5;
			}
			MD5(m_sObjectKey, nLen, m_sObjectKey);

			if ((m_nObjectKeyLength = nKeyLength + 5) > 16)
			{
				m_nObjectKeyLength = 16;
			}		
		}
	}

	DecryptStream::~DecryptStream()
	{
		delete m_pStream;

		if (m_pUncryptedData)
			delete []m_pUncryptedData;
	}

	void DecryptStream::Reset()
	{
		m_pStream->Reset();

		MemoryStream* mem_stream = dynamic_cast<MemoryStream*>(m_pStream);
		
		unsigned int size = mem_stream->getCurrentLength();
		unsigned char* data = (unsigned char*)mem_stream->getCurrent();

		if (!m_pUncryptedData || m_pUncryptedDataSize < size)
		{
			if (m_pUncryptedData)
				delete []m_pUncryptedData;

			m_pUncryptedData = new unsigned char[size];
			m_pUncryptedDataSize = size;
		}
		m_pUncryptedDataPosition = 0;

		switch (m_eCryptType)
		{
			case cryptRC4:
			{
				CryptoPP::ARC4::Decryption rc4Decryption;
				rc4Decryption.SetKey(m_sObjectKey, m_nObjectKeyLength);

				rc4Decryption.ProcessData(m_pUncryptedData, data, size);
			}break;
			case cryptAES128:
			case cryptAES256:
			{
				CryptoPP::AES::Decryption aesDecryption(m_sObjectKey, m_nObjectKeyLength);
				CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, data );

				CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::ArraySink( m_pUncryptedData, size), CryptoPP::StreamTransformationFilter::NO_PADDING );
				stfDecryptor.Put( data + 16, size - 16);
				stfDecryptor.MessageEnd();

				m_pUncryptedDataSize = size - 16;
			}break;
		}
	}

	int DecryptStream::GetChar()
	{
		m_pStream->GetChar();
		return m_pUncryptedDataPosition < m_pUncryptedDataSize ? (m_pUncryptedData[m_pUncryptedDataPosition++] & 0xff) : EOF;
	}

	int DecryptStream::LookChar()
	{
		return m_pUncryptedDataPosition < m_pUncryptedDataSize ? (m_pUncryptedData[m_pUncryptedDataPosition] & 0xff) : EOF;

	}

	bool DecryptStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(bLast);
	}
}

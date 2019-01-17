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
#include "EncryptionHeader.h"

#define GETBIT(from, num) ((from & (1 << num)) != 0)

namespace DocFileFormat
{
	EncryptionHeader::EncryptionHeader( FileInformationBlock* fib, POLE::Stream* tableStream ) : bStandard(false), bXOR(false), bAES(false)
	{
		VirtualStreamReader tStream( tableStream, 0, fib->m_nWordVersion);

		if (fib->m_FibBase.fObfuscation || fib->m_nWordVersion > 0)
		{
			bXOR = true;
			
			crypt_data_xor.key  = fib->m_FibBase.lKey >> 16;
			crypt_data_xor.hash  = fib->m_FibBase.lKey - (crypt_data_xor.key << 16);
		}
		else
		{
			unsigned short vMajor = tStream.ReadUInt16();
			unsigned short vMinor = tStream.ReadUInt16();

			if(0x0001 == vMajor) // RC4 encryption header structure
			{// fib->m_FibBase.lKey == 52 
				bStandard = true;

				crypt_data_rc4.Salt.b1  = tStream.ReadUInt32();
				crypt_data_rc4.Salt.b2  = tStream.ReadUInt32();
				crypt_data_rc4.Salt.b3  = tStream.ReadUInt32();
				crypt_data_rc4.Salt.b4  = tStream.ReadUInt32();
				
				crypt_data_rc4.EncryptedVerifier.b1  = tStream.ReadUInt32();
				crypt_data_rc4.EncryptedVerifier.b2  = tStream.ReadUInt32();
				crypt_data_rc4.EncryptedVerifier.b3  = tStream.ReadUInt32();
				crypt_data_rc4.EncryptedVerifier.b4  = tStream.ReadUInt32();
				
				crypt_data_rc4.EncryptedVerifierHash.b1  = tStream.ReadUInt32();
				crypt_data_rc4.EncryptedVerifierHash.b2  = tStream.ReadUInt32();
				crypt_data_rc4.EncryptedVerifierHash.b3  = tStream.ReadUInt32();
				crypt_data_rc4.EncryptedVerifierHash.b4  = tStream.ReadUInt32();
			}
			else
			{
				bAES					= true;
				crypt_data_aes.bAgile	= false;

				unsigned short flags = tStream.ReadUInt32();
				unsigned char *pDataRead = NULL;

				bool fCryptoAPI				= GETBIT(flags, 1);
				crypt_data_aes.fDocProps	= GETBIT(flags, 2);
				bool fExternal				= GETBIT(flags, 3);
				bool fAES					= GETBIT(flags, 4);

			//EncryptionHeader
				int HeaderSize	= tStream.ReadUInt32();
				int Flags		= tStream.ReadUInt32();
				int SizeExtra	= tStream.ReadUInt32();
				int AlgID		= tStream.ReadUInt32();
				int AlgIDHash	= tStream.ReadUInt32();
				int KeySize		= tStream.ReadUInt32();
				int ProviderType= tStream.ReadUInt32();
				int Reserved1	= tStream.ReadUInt32();
				int Reserved2	= tStream.ReadUInt32();

				int pos		= tStream.GetPosition();
				int size	= tStream.GetSize();

				std::vector<char> dataCSPName;
				while(pos  < size - 1)
				{
					dataCSPName.push_back(tStream.ReadByte());
					dataCSPName.push_back(tStream.ReadByte());
					if (dataCSPName[dataCSPName.size() - 1] == 0 && dataCSPName[dataCSPName.size() - 2] == 0)
					{
						break;
					}
					pos+=2;//unicode null-terminate string
				}
			//EncryptionVerifier
				crypt_data_aes.saltSize = tStream.ReadUInt32();
				pDataRead = tStream.ReadBytes(crypt_data_aes.saltSize, true);
				if (pDataRead)
				{
					crypt_data_aes.saltValue = std::string((char*)pDataRead, crypt_data_aes.saltSize);	
					delete pDataRead;
				}
				
				pDataRead = tStream.ReadBytes(0x10, true);
				if (pDataRead)
				{
					crypt_data_aes.encryptedVerifierInput = std::string((char*)pDataRead, 0x10);
					delete pDataRead;
				}

				crypt_data_aes.hashSize = tStream.ReadUInt32();
						
				int szEncryptedVerifierHash = (ProviderType == 0x0001) ? 0x14 : 0x20;
				pDataRead = tStream.ReadBytes(szEncryptedVerifierHash, true);
				if (pDataRead)
				{
					crypt_data_aes.encryptedVerifierValue = std::string((char*)pDataRead, szEncryptedVerifierHash);
					delete pDataRead;
				}

				pos = tStream.GetPosition();
				
			//------------------------------------------------------------------------------------------
				switch(AlgIDHash)
				{
					case 0x8003: crypt_data_aes.hashAlgorithm = CRYPT_METHOD::MD5;	break;
					case 0x0000: 
					case 0x8004: crypt_data_aes.hashAlgorithm = CRYPT_METHOD::SHA1;	break;
				}
				crypt_data_aes.spinCount = 0;

				switch(AlgID)
				{
				case 0x0000:
					if (fAES)		crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
					if (fCryptoAPI)	crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::RC4;	
					crypt_data_aes.keySize = KeySize / 8;
				case 0x6801:	
					crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::RC4;		
					crypt_data_aes.keySize = KeySize / 8;
					
					if (crypt_data_aes.keySize == 0)	crypt_data_aes.keySize = 5; // 40 bit
					break;
				case 0x660E:	
					crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
					crypt_data_aes.keySize	= 128 /8;	
					break;
				case 0x660F:	
					crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
					crypt_data_aes.keySize	= 192 /8;	
					break;
				case 0x6610:	
					crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
					crypt_data_aes.keySize	= 256 /8;	
					break;
				}

				//switch(ProviderType)
				//{
				//	case 0x0001:	crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::RC4;		break;
				//	case 0x0018:	crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB; break;
				//}
			}
		}
	}
}

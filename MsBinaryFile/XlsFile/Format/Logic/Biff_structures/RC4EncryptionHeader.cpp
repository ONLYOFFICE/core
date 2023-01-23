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

#include "RC4EncryptionHeader.h"

namespace CRYPTO
{


XLS::BiffStructurePtr RC4EncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new RC4EncryptionHeader(*this));
}


void RC4EncryptionHeader::load(XLS::CFRecord& record)
{
	record >> EncryptionVersionInfo;
	
	if (bStandard)
	{
		record.loadAnyData(crypt_data_rc4.Salt);
		record.loadAnyData(crypt_data_rc4.EncryptedVerifier);
		record.loadAnyData(crypt_data_rc4.EncryptedVerifierHash);
	}
	else
	{
		crypt_data_aes.bAgile	= false;

		unsigned short flags;
		record >> flags;
		
		bool fCryptoAPI	= GETBIT(flags, 1);
		bool fDocProps	= GETBIT(flags, 2);
		bool fExternal	= GETBIT(flags, 3);
		bool fAES		= GETBIT(flags, 4);

		unsigned short Reserved3;
		record >> Reserved3;

		_UINT32 HeaderSize;		record >> HeaderSize;
		_UINT32 Flags;			record >> Flags;
		_UINT32 SizeExtra;		record >> SizeExtra;
		_UINT32 AlgID;			record >> AlgID;
		_UINT32 AlgIDHash;		record >> AlgIDHash;
		_UINT32 KeySize;		record >> KeySize;
		_UINT32 ProviderType;	record >> ProviderType;
		_UINT32 Reserved1;		record >> Reserved1;
		_UINT32 Reserved2;		record >> Reserved2;

		int pos		= record.getRdPtr();
		int size	= record.getDataSize();

		std::wstring providerName;
		record >> providerName;

	//EncryptionVerifier

		record >> crypt_data_aes.saltSize;
		
		unsigned char *pDataRead = new unsigned char[crypt_data_aes.saltSize];		
		memcpy(pDataRead, record.getCurData<unsigned char>(), crypt_data_aes.saltSize);
		record.skipNunBytes(crypt_data_aes.saltSize);

		if (pDataRead)
		{
			crypt_data_aes.saltValue = std::string((char*)pDataRead, crypt_data_aes.saltSize);	
			delete []pDataRead;
		}
		
		pDataRead = new unsigned char[0x10];
		memcpy(pDataRead, record.getCurData<unsigned char>(), 0x10);
		record.skipNunBytes(0x10);

		if (pDataRead)
		{
			crypt_data_aes.encryptedVerifierInput = std::string((char*)pDataRead, 0x10);
			delete pDataRead;
		}

		record >> crypt_data_aes.hashSize;
				
		int szEncryptedVerifierHash = (ProviderType == 0x0001) ? 0x14 : 0x20;
		
		pDataRead = new unsigned char[szEncryptedVerifierHash];
		memcpy(pDataRead, record.getCurData<unsigned char>(), szEncryptedVerifierHash);
		record.skipNunBytes(szEncryptedVerifierHash);

		if (pDataRead)
		{
			crypt_data_aes.encryptedVerifierValue = std::string((char*)pDataRead, szEncryptedVerifierHash);
			delete []pDataRead;
		}

		pos = record.getRdPtr();
		
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
			case 0x6801:	
				crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::RC4;		
				crypt_data_aes.keySize = KeySize / 8;
				
				if (crypt_data_aes.keySize == 0)crypt_data_aes.keySize = 5; // 40 bit
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
	}
}


} // namespace CRYPTO


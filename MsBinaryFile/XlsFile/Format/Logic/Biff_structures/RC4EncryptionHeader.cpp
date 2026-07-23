/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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


		std::wstring providerName;
		record >> providerName;

	//EncryptionVerifier

		record >> crypt_data_aes.saltSize;

		int pos		= record.getRdPtr();
		int size	= record.getDataSize();
		if(pos + crypt_data_aes.saltSize > size)
			return;
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


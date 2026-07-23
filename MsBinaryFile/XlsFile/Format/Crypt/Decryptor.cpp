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


#include "Decryptor.h"
#include "RC4Crypt.h"
#include "XORCrypt.h"
#include "../Logic/Biff_structures/RC4EncryptionHeader.h"

namespace CRYPT
{
	RC4Decryptor::RC4Decryptor(_rc4CryptData & header, std::wstring _password) :
													crypt(new RC4Crypt(header, _password))
	{
		crypt_data	= header;
	}

	void RC4Decryptor::Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size)
	{
		crypt->Decrypt(data, size, stream_pos, block_size);
	}
	void RC4Decryptor::Decrypt(char* data, const size_t size, const unsigned long block_index)
	{
		crypt->Decrypt(data, size, block_index);
	}
	bool RC4Decryptor::IsVerify()
	{
		return crypt->IsVerify();
	}

	bool RC4Decryptor::SetPassword(std::wstring password)
	{
		crypt.reset();
		crypt = CryptPtr(new RC4Crypt(crypt_data, password));
		
		if (crypt)	return crypt->IsVerify();
		else		return false;
	}
//----------------------------------------------------------------------------------------

	XORDecryptor::XORDecryptor(int type, unsigned short key, unsigned short hash, std::wstring password) :
													crypt(new XORCrypt(type, key, hash, password))
	{
		nKey	= key;
		nHash	= hash;
		nType	= type;
	}

	void XORDecryptor::Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size)
	{
		crypt->Decrypt(data, size, stream_pos, block_size);
	}
	void XORDecryptor::Decrypt(char* data, const size_t size, const unsigned long block_index)
	{
		crypt->Decrypt(data, size, block_index);
	}
	bool XORDecryptor::IsVerify()
	{
		return crypt->IsVerify();
	}

	bool XORDecryptor::SetPassword(std::wstring password)
	{
		crypt.reset();
		crypt = CryptPtr(new XORCrypt(nType, nKey, nHash, password));
		
		if (crypt)	return crypt->IsVerify();
		else		return false;
	}

	void XORDecryptor::Init(const unsigned long val)
	{
		crypt->Init(val);
	}


};



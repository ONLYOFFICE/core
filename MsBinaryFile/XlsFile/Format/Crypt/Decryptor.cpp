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



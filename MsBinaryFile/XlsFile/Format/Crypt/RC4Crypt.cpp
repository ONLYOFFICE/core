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


#include "RC4Crypt.h"

#include <boost/scoped_array.hpp>

namespace CRYPT
{

RC4Crypt::RC4Crypt(CRYPT::_rc4CryptData & data, std::wstring password)
{
	m_VerifyPassword = false;

	CopyDWORDs2Bytes(data.Salt.b1, data.Salt.b2, data.Salt.b3, data.Salt.b4, pnSalt);
	CopyDWORDs2Bytes(data.EncryptedVerifier.b1	, data.EncryptedVerifier.b2, data.EncryptedVerifier.b3, data.EncryptedVerifier.b4, pnVerifier);
	CopyDWORDs2Bytes(data.EncryptedVerifierHash.b1, data.EncryptedVerifierHash.b2, data.EncryptedVerifierHash.b3, data.EncryptedVerifierHash.b4, pnVerifierHash);

	mxDecoder.reset(new BiffDecoder_RCF(pnSalt, pnVerifier, pnVerifierHash));

	m_VerifyPassword = mxDecoder->verifyPassword(password);
}


bool RC4Crypt::IsVerify()
{
	return m_VerifyPassword;
}

void RC4Crypt::CopyDWORDs2Bytes(const unsigned int b1, const unsigned int b2, const unsigned int b3, const unsigned int b4, unsigned char* byte_array)
{
	byte_array[0] = static_cast<unsigned char>((b1 & 0x000000ff) >> 0);
	byte_array[1] = static_cast<unsigned char>((b1 & 0x0000ff00) >> 8);
	byte_array[2] = static_cast<unsigned char>((b1 & 0x00ff0000) >> 16);
	byte_array[3] = static_cast<unsigned char>((b1 & 0xff000000) >> 24);

	byte_array[4] = static_cast<unsigned char>((b2 & 0x000000ff) >> 0);
	byte_array[5] = static_cast<unsigned char>((b2 & 0x0000ff00) >> 8);
	byte_array[6] = static_cast<unsigned char>((b2 & 0x00ff0000) >> 16);
	byte_array[7] = static_cast<unsigned char>((b2 & 0xff000000) >> 24);

	byte_array[8] = static_cast<unsigned char>((b3 & 0x000000ff) >> 0);
	byte_array[9] = static_cast<unsigned char>((b3 & 0x0000ff00) >> 8);
	byte_array[10] = static_cast<unsigned char>((b3 & 0x00ff0000) >> 16);
	byte_array[11] = static_cast<unsigned char>((b3 & 0xff000000) >> 24);

	byte_array[12] = static_cast<unsigned char>((b4 & 0x000000ff) >> 0);
	byte_array[13] = static_cast<unsigned char>((b4 & 0x0000ff00) >> 8);
	byte_array[14] = static_cast<unsigned char>((b4 & 0x00ff0000) >> 16);
	byte_array[15] = static_cast<unsigned char>((b4 & 0xff000000) >> 24);
}

void RC4Crypt::Decrypt(char* data, const size_t size, const unsigned long block_index)
{
	if(size <= 256)
	{
		static unsigned char quick_buf[256];
		mxDecoder->decode( quick_buf, reinterpret_cast<unsigned char*>(data), size, block_index );
        memcpy(data, quick_buf, size);
	}
	else
	{
		boost::scoped_array<unsigned char> out_data(new unsigned char[size]);
		mxDecoder->decode( out_data.get(), reinterpret_cast<unsigned char*>(data), size, block_index );
        memcpy(data, out_data.get(), size);
	}
}
void RC4Crypt::Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size)
{
	if(size <= 256)
	{
		static unsigned char quick_buf[256];
		mxDecoder->decode( quick_buf, reinterpret_cast<unsigned char*>(data), size, stream_pos, block_size );
        memcpy(data, quick_buf, size);
	}
	else
	{
		boost::scoped_array<unsigned char> out_data(new unsigned char[size]);
		mxDecoder->decode( out_data.get(), reinterpret_cast<unsigned char*>(data), size, stream_pos, block_size );
        memcpy(data, out_data.get(), size);
	}
}

};



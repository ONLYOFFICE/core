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



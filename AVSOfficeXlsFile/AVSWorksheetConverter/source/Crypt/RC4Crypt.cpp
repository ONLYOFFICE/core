#include "stdafx.h"
#include "RC4Crypt.h"

#include <boost/scoped_array.hpp>

namespace CRYPT
{;

RC4Crypt::RC4Crypt(const CRYPTO::RC4EncryptionHeader& header)
{
	CopyDWORDs2BYTEs(header.Salt.b1, header.Salt.b2, header.Salt.b3, header.Salt.b4, pnSalt);
	CopyDWORDs2BYTEs(header.EncryptedVerifier.b1, header.EncryptedVerifier.b2, header.EncryptedVerifier.b3, header.EncryptedVerifier.b4, pnVerifier);
	CopyDWORDs2BYTEs(header.EncryptedVerifierHash.b1, header.EncryptedVerifierHash.b2, header.EncryptedVerifierHash.b3, header.EncryptedVerifierHash.b4, pnVerifierHash);
	mxDecoder.reset(new BiffDecoder_RCF(pnSalt, pnVerifier, pnVerifierHash));

	mxDecoder->verifyPassword(L"VelvetSweatshop");
}

void RC4Crypt::Encrypt(char* data, const size_t size)
{
	
}

void RC4Crypt::CopyDWORDs2BYTEs(const DWORD b1, const DWORD b2, const DWORD b3, const DWORD b4, BYTE* byte_array)
{
	byte_array[0] = static_cast<BYTE>((b1 & 0x000000ff) >> 0);
	byte_array[1] = static_cast<BYTE>((b1 & 0x0000ff00) >> 8);
	byte_array[2] = static_cast<BYTE>((b1 & 0x00ff0000) >> 16);
	byte_array[3] = static_cast<BYTE>((b1 & 0xff000000) >> 24);

	byte_array[4] = static_cast<BYTE>((b2 & 0x000000ff) >> 0);
	byte_array[5] = static_cast<BYTE>((b2 & 0x0000ff00) >> 8);
	byte_array[6] = static_cast<BYTE>((b2 & 0x00ff0000) >> 16);
	byte_array[7] = static_cast<BYTE>((b2 & 0xff000000) >> 24);

	byte_array[8] = static_cast<BYTE>((b3 & 0x000000ff) >> 0);
	byte_array[9] = static_cast<BYTE>((b3 & 0x0000ff00) >> 8);
	byte_array[10] = static_cast<BYTE>((b3 & 0x00ff0000) >> 16);
	byte_array[11] = static_cast<BYTE>((b3 & 0xff000000) >> 24);

	byte_array[12] = static_cast<BYTE>((b4 & 0x000000ff) >> 0);
	byte_array[13] = static_cast<BYTE>((b4 & 0x0000ff00) >> 8);
	byte_array[14] = static_cast<BYTE>((b4 & 0x00ff0000) >> 16);
	byte_array[15] = static_cast<BYTE>((b4 & 0xff000000) >> 24);
}

void RC4Crypt::Decrypt(char* data, const size_t size, const ULONGLONG stream_pos)
{
	static BYTE quick_buf[256];
	if(size <= 256)
	{
		mxDecoder->decode( quick_buf, reinterpret_cast<BYTE*>(data), stream_pos, size );
		memcpy_s(data, size, quick_buf, size);
	}
	else
	{
		boost::scoped_array<BYTE> out_data(new BYTE[size]);
		mxDecoder->decode( out_data.get(), reinterpret_cast<BYTE*>(data), stream_pos, size );
		memcpy_s(data, size, out_data.get(), size);
	}
}

};



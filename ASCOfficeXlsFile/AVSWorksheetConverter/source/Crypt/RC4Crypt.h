#pragma once

#include "Crypt.h"
#include <XLS_logic/Biff_structures/RC4EncryptionHeader.h>
#include "BiffDecoder_RCF.h"

namespace CRYPT
{;

class RC4Crypt : public Crypt
{
public:
	RC4Crypt(const CRYPTO::RC4EncryptionHeader& header);

	virtual void Encrypt(char* data, const size_t size);
	virtual void Decrypt(char* data, const size_t size, const ULONGLONG stream_pos);

private:
	void CopyDWORDs2BYTEs(const DWORD b1, const DWORD b2, const DWORD b3, const DWORD b4, BYTE* byte_array);

private:
	BYTE pnSalt[16];
	BYTE pnVerifier[16];
	BYTE pnVerifierHash[16];

	BiffDecoderRef mxDecoder;
};


} // namespace CRYPT

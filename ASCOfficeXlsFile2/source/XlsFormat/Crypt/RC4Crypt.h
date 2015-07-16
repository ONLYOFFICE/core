#pragma once

#include "Crypt.h"
#include <Logic/Biff_structures/RC4EncryptionHeader.h>
#include "BiffDecoder_RCF.h"

namespace CRYPT
{;

class RC4Crypt : public Crypt
{
public:
	RC4Crypt(const CRYPTO::RC4EncryptionHeader& header);

	virtual void Encrypt(char* data, const size_t size);
	virtual void Decrypt(char* data, const size_t size, const unsigned long stream_pos);

private:
	void CopyDWORDs2Bytes(const unsigned int b1, const unsigned int b2, const unsigned int b3, const unsigned int b4, unsigned char* byte_array);

private:
	unsigned char pnSalt[16];
	unsigned char pnVerifier[16];
	unsigned char pnVerifierHash[16];

	BiffDecoderRef mxDecoder;
};


} // namespace CRYPT

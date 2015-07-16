#pragma once

#include "Crypt.h"

namespace CRYPTO
{;
class RC4EncryptionHeader;
} // namespace CRYPTO

namespace CRYPT
{;

class Decryptor
{
public:
	Decryptor(const CRYPTO::RC4EncryptionHeader& header);

	void Decrypt(char* data, const size_t size, const unsigned long stream_pos);

private:
	CryptPtr crypt;
	Crypt::crypt_type type;
};

typedef boost::shared_ptr<Decryptor> DecryptorPtr;


} // namespace CRYPT

#pragma once

#include "Crypt.h"

namespace CRYPTO
{
	class RC4EncryptionHeader;
	typedef boost::shared_ptr<RC4EncryptionHeader> RC4EncryptionHeaderPtr;

} // namespace CRYPTO

namespace CRYPT
{

class Decryptor
{
public:
	Decryptor(CRYPTO::RC4EncryptionHeaderPtr & header, std::wstring password);

	void Decrypt(char* data, const size_t size, const unsigned long stream_pos);

	bool IsVerify();

	bool SetPassword(std::wstring password);

private:
	CryptPtr						crypt;
	Crypt::crypt_type				type;
	CRYPTO::RC4EncryptionHeaderPtr	crypt_header;
};

typedef boost::shared_ptr<Decryptor> DecryptorPtr;


} // namespace CRYPT

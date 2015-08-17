

#include "Decryptor.h"
#include "RC4Crypt.h"
#include <Logic/Biff_structures/RC4EncryptionHeader.h>
//#include <Logic/Biff_structures/RC4CryptoAPIEncryptionHeader.h>

namespace CRYPT
{

Decryptor::Decryptor(const CRYPTO::RC4EncryptionHeader& header)
:	crypt(new RC4Crypt(header)),
	type(Crypt::RC4)
{
}

void Decryptor::Decrypt(char* data, const size_t size, const unsigned long stream_pos)
{
	crypt->Decrypt(data, size, stream_pos);
}

};





#include "Decryptor.h"
#include "RC4Crypt.h"
#include <Logic/Biff_structures/RC4EncryptionHeader.h>
//#include <Logic/Biff_structures/RC4CryptoAPIEncryptionHeader.h>

namespace CRYPT
{

	Decryptor::Decryptor(CRYPTO::RC4EncryptionHeaderPtr & header, std::wstring password) :
							crypt	(new RC4Crypt(header, password)),
							type	(Crypt::RC4)
	{
		crypt_header = header;
	}

	void Decryptor::Decrypt(char* data, const size_t size, const unsigned long stream_pos)
	{
		crypt->Decrypt(data, size, stream_pos);
	}

	bool Decryptor::IsVerify()
	{
		return crypt->IsVerify();
	}

	bool Decryptor::SetPassword(std::wstring password)
	{
		crypt.reset();
		crypt = CryptPtr(new RC4Crypt(crypt_header, password));
		
		if (crypt)	return crypt->IsVerify();
		else		return false;
	}

};



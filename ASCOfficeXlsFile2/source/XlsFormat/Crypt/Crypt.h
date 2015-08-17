#pragma once

#include <boost/shared_ptr.hpp>

namespace CRYPT
{

class Crypt
{
public:

	virtual void Encrypt(char* data, const size_t size) = 0;
	virtual void Decrypt(char* data, const size_t size, const unsigned long stream_pos) = 0;

	typedef enum
	{
		RC4,
		RC4CryptoAPI,
		XOR
	} crypt_type;

};

typedef boost::shared_ptr<Crypt> CryptPtr;


} // namespace CRYPT

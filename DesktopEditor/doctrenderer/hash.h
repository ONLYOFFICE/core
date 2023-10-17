#ifndef NSHASH_H
#define NSHASH_H

#include <functional>
#include <stddef.h>
#include "./common.h"

// hide algs into lib
namespace NSHash
{
	void BUILDER_DECL hash_internal(const unsigned char* src, unsigned char* dst, size_t size, int alg);
}

class BUILDER_DECL CHash
{	
public:
	enum HashAlgs
	{
		haMD2		= 0,
		haMD4		= 1,
		haMD5		= 2,
		haRMD160	= 3,
		haSHA1		= 4,
		haSHA256	= 5,
		haSHA384	= 6,
		haSHA512	= 7,
		haWHIRLPOOL	= 8
	};

	static int getDigestLength(HashAlgs alg);

public:
	CHash(std::function<void*(size_t)> allocator);
	~CHash();

	unsigned char* hash(const unsigned char* data, int size, int alg);
	unsigned char* hash2(const char* password, const char* salt, int spinCount, int alg);

private:
	std::function<void*(size_t)> m_fAllocator;
};

#endif // NSHASH_H

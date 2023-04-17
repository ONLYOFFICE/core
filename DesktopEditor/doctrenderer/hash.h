#ifndef NSHASH_H
#define NSHASH_H

class NSHash
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
	NSHash();
	~NSHash();

	unsigned char* hash(const unsigned char* data, int size, int alg);
	unsigned char* hash2(const char* password, const char* salt, int spinCount, int alg);
};

#endif // NSHASH_H

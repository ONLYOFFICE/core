#include "hash.h"

#include "openssl/sha.h"
#include "openssl/md2.h"
#include "openssl/md4.h"
#include "openssl/md5.h"
#include "openssl/whrlpool.h"
#include "openssl/ripemd.h"

#include <memory>

void NSHash::hash_internal(const unsigned char* src, unsigned char* dst, size_t d, int alg)
{
	switch (alg)
	{
	case CHash::haMD2:
	{
		MD2(src, d, dst);
		break;
	}
	case CHash::haMD4:
	{
		MD4(src, d, dst);
		break;
	}
	case CHash::haMD5:
	{
		MD5(src, d, dst);
		break;
	}
	case CHash::haRMD160:
	{
		RIPEMD160(src, d, dst);
		break;
	}
	case CHash::haSHA1:
	{
		SHA1(src, d, dst);
		break;
	}
	case CHash::haSHA256:
	{
		SHA256(src, d, dst);
		break;
	}
	case CHash::haSHA384:
	{
		SHA384(src, d, dst);
		break;
	}
	case CHash::haSHA512:
	{
		SHA512(src, d, dst);
		break;
	}
	case CHash::haWHIRLPOOL:
	{
		WHIRLPOOL(src, d, dst);
		break;
	}
	default:
		break;
	}
}

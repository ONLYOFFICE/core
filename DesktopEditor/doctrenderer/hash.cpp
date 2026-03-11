#include "hash.h"

#ifndef OWN_BASE_64
#include "../common/Base64.h"
#endif

#include <openssl/evp.h>
#include <openssl/opensslv.h>

#if OPENSSL_VERSION_MAJOR < 3
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/whrlpool.h>
#include <openssl/ripemd.h>
#ifndef OPENSSL_NO_MD2
#include <openssl/md2.h>
#endif
#endif

#include <cstring>
#include <memory>
#include <stdlib.h>

/*
 * OpenSSL 3.x deprecates the low-level one-shot hash functions (MD4(), MD5(),
 * SHA1(), RIPEMD160(), WHIRLPOOL()) and removes MD2 entirely. Use EVP_Digest()
 * as the single entry point for all algorithms. On OpenSSL 1.x the legacy
 * one-shot functions are still available and used directly.
 */
#if OPENSSL_VERSION_MAJOR >= 3
static const EVP_MD* hash_alg_to_evp_md(int alg)
{
	switch (alg)
	{
	case CHash::haMD4:       return EVP_md4();
	case CHash::haMD5:       return EVP_md5();
	case CHash::haRMD160:    return EVP_ripemd160();
	case CHash::haSHA1:      return EVP_sha1();
	case CHash::haSHA256:    return EVP_sha256();
	case CHash::haSHA384:    return EVP_sha384();
	case CHash::haSHA512:    return EVP_sha512();
	/* MD2 and WHIRLPOOL are not available in the default OpenSSL 3.x provider.
	 * haMD2 falls through to default and returns NULL. */
	default:                 return NULL;
	}
}

static bool evp_digest(const unsigned char* data, size_t len,
                       unsigned char* out, int alg)
{
	const EVP_MD* md = hash_alg_to_evp_md(alg);
	if (!md)
		return false;
	unsigned int md_len = 0;
	return EVP_Digest(data, len, out, &md_len, md, NULL) == 1;
}
#endif

int CHash::getDigestLength(HashAlgs alg)
{
	static const int aDigestLengths[] = { 16, 16, 16, 20, 20, 32, 48, 64, 64 };
	return aDigestLengths[alg];
}

CHash::CHash(std::function<void*(size_t)> allocator) : m_fAllocator(allocator)
{
}

CHash::~CHash()
{
}

unsigned char* CHash::hash(const unsigned char* data, int size, int alg)
{
	unsigned int nBufLen = 0;
	unsigned char* pBufData = NULL;
	size_t d = (size_t)size;

	nBufLen = getDigestLength((HashAlgs)alg);
	if (0 == nBufLen)
		return NULL;

	pBufData = (unsigned char*)m_fAllocator(nBufLen);

#if OPENSSL_VERSION_MAJOR >= 3
	if (!evp_digest(data, d, pBufData, alg))
	{
		/* Algorithm not available (e.g. MD2 on OpenSSL 3.x). */
		return NULL;
	}
#else
	switch (alg)
	{
#ifndef OPENSSL_NO_MD2
	case haMD2:       MD2(data, d, pBufData);       break;
#endif
	case haMD4:       MD4(data, d, pBufData);       break;
	case haMD5:       MD5(data, d, pBufData);       break;
	case haRMD160:    RIPEMD160(data, d, pBufData);  break;
	case haSHA1:      SHA1(data, d, pBufData);       break;
	case haSHA256:    SHA256(data, d, pBufData);     break;
	case haSHA384:    SHA384(data, d, pBufData);     break;
	case haSHA512:    SHA512(data, d, pBufData);     break;
	case haWHIRLPOOL: WHIRLPOOL(data, d, pBufData); break;
	default:          return NULL;
	}
#endif

	return pBufData;
}

namespace NSUtfs
{
#define WriteUtf16_byte(code, p)							\
	*p++ = code & 0xFF;										\
	*p++ = (code >> 8) & 0xFF;

#define WriteUtf16(code, p)									\
	if (code < 0x10000)										\
	{														\
	WriteUtf16_byte(code, p)								\
	}														\
	else													\
	{														\
	code -= 0x10000;										\
	WriteUtf16_byte((0xD800 | ((code >> 10) & 0x03FF)), p);	\
	WriteUtf16_byte((0xDC00 | (code & 0x03FF)), p);			\
	}

	void Utf8_to_utf16le(const char* utf8, unsigned char*& utf16, size_t& utf16_len)
	{
		size_t utf8_count = std::strlen(utf8);
		utf16 = (unsigned char*)malloc(utf8_count * 2);
		unsigned char* utf16_cur = utf16;

		size_t index = 0;
		while (index < utf8_count)
		{
			unsigned char byteMain = utf8[index];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				WriteUtf16(byteMain, utf16_cur);
				++index;
			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = 0;
				if ((index + 1) < utf8_count)
				{
					val = (int)(((byteMain & 0x1F) << 6) |
								(utf8[index + 1] & 0x3F));
				}

				WriteUtf16(val, utf16_cur);
				index += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = 0;
				if ((index + 2) < utf8_count)
				{
					val = (int)(((byteMain & 0x0F) << 12) |
								((utf8[index + 1] & 0x3F) << 6) |
							(utf8[index + 2] & 0x3F));
				}

				WriteUtf16(val, utf16_cur);
				index += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = 0;
				if ((index + 3) < utf8_count)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((utf8[index + 1] & 0x3F) << 12) |
							((utf8[index + 2] & 0x3F) << 6) |
							(utf8[index + 3] & 0x3F));
				}

				WriteUtf16(val, utf16_cur);
				index += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = 0;
				if ((index + 3) < utf8_count)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((utf8[index + 1] & 0x3F) << 12) |
							((utf8[index + 2] & 0x3F) << 6) |
							(utf8[index + 3] & 0x3F));
				}

				WriteUtf16(val, utf16_cur);
				index += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = 0;
				if ((index + 4) < utf8_count)
				{
					val = (int)(((byteMain & 0x03) << 24) |
								((utf8[index + 1] & 0x3F) << 18) |
							((utf8[index + 2] & 0x3F) << 12) |
							((utf8[index + 3] & 0x3F) << 6) |
							(utf8[index + 4] & 0x3F));
				}

				WriteUtf16(val, utf16_cur);
				index += 5;
			}
			else
			{
				// 6 byte
				int val = 0;
				if ((index + 5) < utf8_count)
				{
					val = (int)(((byteMain & 0x01) << 30) |
								((utf8[index + 1] & 0x3F) << 24) |
							((utf8[index + 2] & 0x3F) << 18) |
							((utf8[index + 3] & 0x3F) << 12) |
							((utf8[index + 4] & 0x3F) << 6) |
							(utf8[index + 5] & 0x3F));
				}

				WriteUtf16(val, utf16_cur);
				index += 5;
			}
		}

		utf16_len = (int)(utf16_cur - utf16);
	}
}

void hash_iteration(unsigned char*& input, int iter, unsigned char*& tmp, int alg, size_t alg_size)
{
	input[alg_size]     = 0xFF & (iter >> 0);
	input[alg_size + 1] = 0xFF & (iter >> 8);
	input[alg_size + 2] = 0xFF & (iter >> 16);
	input[alg_size + 3] = 0xFF & (iter >> 24);

#if OPENSSL_VERSION_MAJOR >= 3
	evp_digest(input, alg_size + 4, tmp, alg);
#else
	switch (alg)
	{
#ifndef OPENSSL_NO_MD2
	case CHash::haMD2:       MD2(input, alg_size + 4, tmp);       break;
#endif
	case CHash::haMD4:       MD4(input, alg_size + 4, tmp);       break;
	case CHash::haMD5:       MD5(input, alg_size + 4, tmp);       break;
	case CHash::haRMD160:    RIPEMD160(input, alg_size + 4, tmp);  break;
	case CHash::haSHA1:      SHA1(input, alg_size + 4, tmp);       break;
	case CHash::haSHA256:    SHA256(input, alg_size + 4, tmp);     break;
	case CHash::haSHA384:    SHA384(input, alg_size + 4, tmp);     break;
	case CHash::haSHA512:    SHA512(input, alg_size + 4, tmp);     break;
	case CHash::haWHIRLPOOL: WHIRLPOOL(input, alg_size + 4, tmp); break;
	default: break;
	}
#endif

	unsigned char* mem = input;
	input = tmp;
	tmp = mem;
}

unsigned char* CHash::hash2(const char* password, const char* salt, int spinCount, int alg)
{
	int inputSaltLen = std::strlen(salt);
	unsigned char* passwordUtf16 = NULL;
	size_t passwordUtf16Len = 0;
	NSUtfs::Utf8_to_utf16le(password, passwordUtf16, passwordUtf16Len);

	int inputDataLen = inputSaltLen + passwordUtf16Len;
	unsigned char* inputData = (unsigned char*)malloc(inputDataLen);
	NSBase64::Base64Decode(salt, inputSaltLen, inputData, &inputDataLen);
	memcpy(inputData + inputDataLen, passwordUtf16, passwordUtf16Len);
	inputDataLen += passwordUtf16Len;

	free(passwordUtf16);

	size_t alg_size = getDigestLength((HashAlgs)alg);
	if (0 == alg_size)
	{
		free(inputData);
		return NULL;
	}

	unsigned char* pBuffer1 = (unsigned char*)m_fAllocator(alg_size + 4);

#if OPENSSL_VERSION_MAJOR >= 3
	if (!evp_digest(inputData, inputDataLen, pBuffer1, alg))
	{
		free(inputData);
		return NULL;
	}
#else
	switch (alg)
	{
#ifndef OPENSSL_NO_MD2
	case haMD2:       MD2(inputData, inputDataLen, pBuffer1);       break;
#endif
	case haMD4:       MD4(inputData, inputDataLen, pBuffer1);       break;
	case haMD5:       MD5(inputData, inputDataLen, pBuffer1);       break;
	case haRMD160:    RIPEMD160(inputData, inputDataLen, pBuffer1);  break;
	case haSHA1:      SHA1(inputData, inputDataLen, pBuffer1);       break;
	case haSHA256:    SHA256(inputData, inputDataLen, pBuffer1);     break;
	case haSHA384:    SHA384(inputData, inputDataLen, pBuffer1);     break;
	case haSHA512:    SHA512(inputData, inputDataLen, pBuffer1);     break;
	case haWHIRLPOOL: WHIRLPOOL(inputData, inputDataLen, pBuffer1); break;
	default:
		free(inputData);
		return NULL;
	}
#endif

	free(inputData);

	if (0 == spinCount)
		return pBuffer1;

	unsigned char* pBuffer2 = (unsigned char*)malloc(alg_size + 4);
	for (int i = 0; i < spinCount; ++i)
	{
		hash_iteration(pBuffer1, i, pBuffer2, alg, alg_size);
	}

	free(pBuffer2);
	return pBuffer1;
}

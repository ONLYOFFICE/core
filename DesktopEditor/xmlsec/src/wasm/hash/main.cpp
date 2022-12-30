#include <memory>
#include <string.h>
#define OPENSSL_MD2_ENABLED

#include "./../3rdParty/openssl/include/openssl/sha.h"
#include "./../3rdParty/openssl/include/openssl/md2.h"
#include "./../3rdParty/openssl/include/openssl/md4.h"
#include "./../3rdParty/openssl/include/openssl/md5.h"
#include "./../3rdParty/openssl/include/openssl/whrlpool.h"
#include "./../3rdParty/openssl/include/openssl/ripemd.h"

#ifdef OOXML_SUPPORT_SHA3_512
#include "./../3rdParty/openssl/include/openssl/evp.h"
#endif

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OOXML_HASH_ALG_MD2          0
#define OOXML_HASH_ALG_MD4          1
#define OOXML_HASH_ALG_MD5          2
#define OOXML_HASH_ALG_RIPEMD160    3
#define OOXML_HASH_ALG_SHA1         4
#define OOXML_HASH_ALG_SHA256       5
#define OOXML_HASH_ALG_SHA384       6
#define OOXML_HASH_ALG_SHA512       7
#define OOXML_HASH_ALG_WHIRLPOOL    8
#define OOXML_HASH_ALG_SHA3_512     9

// BASE64
namespace NSBase64
{
	int DecodeBase64Char(unsigned int ch)
	{
		// returns -1 if the character is invalid
		// or should be skipped
		// otherwise, returns the 6-bit code for the character
		// from the encoding table
		if (ch >= 'A' && ch <= 'Z')
			return ch - 'A' + 0;	// 0 range starts at 'A'
		if (ch >= 'a' && ch <= 'z')
			return ch - 'a' + 26;	// 26 range starts at 'a'
		if (ch >= '0' && ch <= '9')
			return ch - '0' + 52;	// 52 range starts at '0'
		if (ch == '+')
			return 62;
		if (ch == '/')
			return 63;
		return -1;
	}

	bool Base64Decode(const char* szSrc, size_t nSrcLen, unsigned char *pbDest, size_t *pnDestLen)
	{
		// walk the source buffer
		// each four character sequence is converted to 3 bytes
		// CRLFs and =, and any characters not in the encoding table
		// are skiped

		if (szSrc == NULL || pnDestLen == NULL)
			return false;

		const char* szSrcEnd = szSrc + nSrcLen;
		size_t nWritten = 0;

		bool bOverflow = (pbDest == NULL) ? true : false;

		while (szSrc < szSrcEnd &&(*szSrc) != 0)
		{
			unsigned int dwCurr = 0;
			int i;
			int nBits = 0;
			for (i=0; i<4; i++)
			{
				if (szSrc >= szSrcEnd)
					break;
				int nCh = DecodeBase64Char(*szSrc);
				szSrc++;
				if (nCh == -1)
				{
					// skip this char
					i--;
					continue;
				}
				dwCurr <<= 6;
				dwCurr |= nCh;
				nBits += 6;
			}

			if(!bOverflow && nWritten + (nBits/8) > (*pnDestLen))
				bOverflow = true;

			// dwCurr has the 3 bytes to write to the output buffer
			// left to right
			dwCurr <<= 24-nBits;
			for (i=0; i<nBits/8; i++)
			{
				if(!bOverflow)
				{
					*pbDest = (unsigned char) ((dwCurr & 0x00ff0000) >> 16);
					pbDest++;
				}
				dwCurr <<= 8;
				nWritten++;
			}

		}

		*pnDestLen = nWritten;

		if(bOverflow)
		{
			return false;
		}

		return true;
	}
}

namespace NSUtfs
{
#define WriteUtf16_byte(code, p)                            \
	*p++ = code & 0xFF;                                     \
	*p++ = (code >> 8) & 0xFF;

#define WriteUtf16(code, p)                                 \
	if (code < 0x10000)                                         \
	{                                                           \
	WriteUtf16_byte(code, p)                                \
}                                                           \
	else                                                        \
	{                                                           \
	code -= 0x10000;                                        \
	WriteUtf16_byte((0xD800 | ((code >> 10) & 0x03FF)), p);	\
	WriteUtf16_byte((0xDC00 | (code & 0x03FF)), p);			\
}

	size_t strlen(const char* s)
	{
		const char* _s = s;
		while (*_s != 0)
			++_s;
		return _s - s;
	}

	void Utf8_to_utf16le(char* utf8, unsigned char*& utf16, size_t& utf16_len)
	{
		size_t utf8_count = strlen(utf8);
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

WASM_EXPORT unsigned char* hash(unsigned char* data, int size, int alg)
{
	unsigned int nBufLen = 0;
	unsigned char* pBufData = NULL;
	size_t d = (size_t)size;

	switch (alg)
	{
	case OOXML_HASH_ALG_MD2:
	{
		nBufLen = 16;
		pBufData = (unsigned char*)malloc(nBufLen);
		MD2(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_MD4:
	{
		nBufLen = 16;
		pBufData = (unsigned char*)malloc(nBufLen);
		MD4(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_MD5:
	{
		nBufLen = 16;
		pBufData = (unsigned char*)malloc(nBufLen);
		MD5(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_RIPEMD160:
	{
		nBufLen = 20;
		pBufData = (unsigned char*)malloc(nBufLen);
		RIPEMD160(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_SHA1:
	{
		nBufLen = 20;
		pBufData = (unsigned char*)malloc(nBufLen);
		SHA1(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_SHA256:
	{
		nBufLen = 32;
		pBufData = (unsigned char*)malloc(nBufLen);
		SHA256(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_SHA384:
	{
		nBufLen = 48;
		pBufData = (unsigned char*)malloc(nBufLen);
		SHA384(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_SHA512:
	{
		nBufLen = 64;
		pBufData = (unsigned char*)malloc(nBufLen);
		SHA512(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_WHIRLPOOL:
	{
		nBufLen = 64;
		pBufData = (unsigned char*)malloc(nBufLen);
		WHIRLPOOL(data, d, pBufData);
		break;
	}
	case OOXML_HASH_ALG_SHA3_512:
	{
#ifdef OOXML_SUPPORT_SHA3_512
		nBufLen = 64;
		pBufData = (unsigned char*)malloc(nBufLen);
		const EVP_MD* algorithm = EVP_sha3_512();
		EVP_MD_CTX* context = EVP_MD_CTX_new();
		EVP_DigestInit_ex(context, algorithm, nullptr);
		EVP_DigestUpdate(context, data, d);
		EVP_DigestFinal_ex(context, pBufData, &nBufLen);
		EVP_MD_CTX_destroy(context);
#endif
		break;
	}
	default:
		break;
	}

	return pBufData;
}

void hash_iteration(unsigned char*& input, int iter, unsigned char*& tmp, int alg, size_t alg_size)
{
	input[alg_size]     = 0xFF & (iter >> 0);
	input[alg_size + 1] = 0xFF & (iter >> 8);
	input[alg_size + 2] = 0xFF & (iter >> 16);
	input[alg_size + 3] = 0xFF & (iter >> 24);

	switch (alg)
	{
	case OOXML_HASH_ALG_MD2:
	{
		MD2(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_MD4:
	{
		MD4(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_MD5:
	{
		MD5(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_RIPEMD160:
	{
		RIPEMD160(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_SHA1:
	{
		SHA1(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_SHA256:
	{
		SHA256(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_SHA384:
	{
		SHA384(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_SHA512:
	{
		SHA512(input, alg_size + 4, tmp);
		break;
	}
	case OOXML_HASH_ALG_WHIRLPOOL:
	{
		WHIRLPOOL(input, alg_size + 4, tmp);
		break;
	}
	default:
		break;
	}

	unsigned char* mem = input;
	input = tmp;
	tmp = mem;
}

WASM_EXPORT unsigned char* hash2(char* password, char* salt, int spinCount, int alg)
{
	size_t inputSaltLen = NSUtfs::strlen(salt);
	unsigned char* passwordUtf16 = NULL;
	size_t passwordUtf16Len = 0;
	NSUtfs::Utf8_to_utf16le(password, passwordUtf16, passwordUtf16Len);

	size_t inputDataLen = inputSaltLen + passwordUtf16Len;
	unsigned char* inputData = (unsigned char*)malloc(inputDataLen);
	NSBase64::Base64Decode(salt, inputSaltLen, inputData, &inputDataLen);
	memcpy(inputData + inputDataLen, passwordUtf16, passwordUtf16Len);
	inputDataLen += passwordUtf16Len;

	free(passwordUtf16);

	size_t alg_size = 0;
	unsigned char* pBuffer1 = NULL;
	switch (alg)
	{
	case OOXML_HASH_ALG_MD2:
	{
		alg_size = 16;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		MD2(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_MD4:
	{
		alg_size = 16;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		MD4(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_MD5:
	{
		alg_size = 16;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		MD5(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_RIPEMD160:
	{
		alg_size = 20;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		RIPEMD160(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_SHA1:
	{
		alg_size = 20;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		SHA1(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_SHA256:
	{
		alg_size = 32;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		SHA256(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_SHA384:
	{
		alg_size = 48;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		SHA384(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_SHA512:
	{
		alg_size = 64;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		SHA512(inputData, inputDataLen, pBuffer1);
		break;
	}
	case OOXML_HASH_ALG_WHIRLPOOL:
	{
		alg_size = 64;
		pBuffer1 = (unsigned char*)malloc(alg_size + 4);
		WHIRLPOOL(inputData, inputDataLen, pBuffer1);
		break;
	}
	default:
		break;
	}

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

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
int main()
{
	std::string data = "test";
	unsigned char* result = hash((unsigned char*)data.c_str(), (int)data.length(), OOXML_HASH_ALG_SHA256);

	std::string password = "oleg";
	std::string salt = "yrGivlyCImiWnryRee1OJw==";

	unsigned char* result2 = hash2((char*)password.c_str(), (char*)salt.c_str(), 100000, OOXML_HASH_ALG_SHA512);

	return 0;
}
#endif

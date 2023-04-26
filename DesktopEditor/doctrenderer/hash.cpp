#include "hash.h"

#ifndef OWN_BASE_64
#include "../common/Base64.h"
#endif

#include <cstring>
#include <memory>

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
	unsigned int nBufLen = (unsigned int)getDigestLength((HashAlgs)alg);
	unsigned char* pBufData = (unsigned char*)m_fAllocator(nBufLen);

	NSHash::hash_internal(data, pBufData, (size_t)size, alg);
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

	NSHash::hash_internal(input, tmp, alg_size + 4, alg);

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

	size_t alg_size = (size_t)getDigestLength((HashAlgs)alg);
	unsigned char* pBuffer1 = (unsigned char*)m_fAllocator(alg_size + 4);
	NSHash::hash_internal(inputData, pBuffer1, (size_t)inputDataLen, alg);

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

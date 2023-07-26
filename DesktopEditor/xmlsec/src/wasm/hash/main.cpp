#include <cstddef>

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

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

	bool Base64Decode(const char* szSrc, int nSrcLen, unsigned char *pbDest, int *pnDestLen)
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

#define OWN_BASE_64
#define DOCTRENDERER_NO_USE_DYNAMIC_LIBRARY
#include "../../../../doctrenderer/hash.cpp"

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT unsigned char* hash(const unsigned char* data, int size, int alg)
{
	return CHash(malloc).hash(data, size, alg);
}

WASM_EXPORT unsigned char* hash2(const char* password, const char* salt, int spinCount, int alg)
{
	return CHash(malloc).hash2(password, salt, spinCount, alg);
}

#ifdef __cplusplus
}
#endif


#ifdef TEST_AS_EXECUTABLE
#include <string>
#include <iostream>

int main()
{
	std::string data = "test";
	CHash::HashAlgs alg1 = CHash::haMD2;
	unsigned char* result1 = hash((unsigned char*)data.c_str(), (int)data.length(), alg1);

	for (int i = 0; i < CHash::getDigestLength(alg1); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(result1[i]);
	}
	std::cout << std::endl;

	std::string password = "oleg";
	std::string salt = "yrGivlyCImiWnryRee1OJw==";

	CHash::HashAlgs alg2 = CHash::haSHA512;
	unsigned char* result2 = hash2((char*)password.c_str(), (char*)salt.c_str(), 100000, alg2);
	for (int i = 0; i < CHash::getDigestLength(alg2); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(result2[i]);
	}
	std::cout << std::endl;

	return 0;
}
#endif

#pragma once

#include <stddef.h>
#include "../../../Common/DocxFormat/Source/Base/Types_32.h"

class CipherARCFOUR
{
public:
	typedef enum 
	{
		rtl_Cipher_DirectionBoth,
		rtl_Cipher_DirectionDecode,
		rtl_Cipher_DirectionEncode,
		rtl_Cipher_DirectionInvalid,
	} rtlCipherDirection;

	typedef enum
	{
		rtl_Cipher_E_None,
		rtl_Cipher_E_Argument,
		rtl_Cipher_E_Direction,
		rtl_Cipher_E_BufferSize,
	} rtlCipherError;

public:
	CipherARCFOUR();

	rtlCipherError Init(rtlCipherDirection Direction, const unsigned char *pKeyData, const size_t nKeyLen, const unsigned char *pArgData, const size_t nArgLen);
	rtlCipherError Encode(const void *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen);
	rtlCipherError Decode(const void *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen);


private:
	rtlCipherError Update(const unsigned char *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen);

private:
	static const _UINT32 CIPHER_CBLOCK_ARCFOUR = 256;
	typedef struct
	{
		_UINT32 m_S[CIPHER_CBLOCK_ARCFOUR];
		_UINT32 m_X, m_Y;
	} CipherContext;

	rtlCipherDirection  m_direction;
	CipherContext m_context;

};



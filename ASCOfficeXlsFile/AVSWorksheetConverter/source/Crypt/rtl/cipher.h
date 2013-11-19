#pragma once


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

	rtlCipherError Init(rtlCipherDirection Direction, const BYTE *pKeyData, const size_t nKeyLen, const BYTE *pArgData, const size_t nArgLen);
	rtlCipherError Encode(const void *pData, const size_t nDatLen, BYTE *pBuffer, const size_t nBufLen);
	rtlCipherError Decode(const void *pData, const size_t nDatLen, BYTE *pBuffer, const size_t nBufLen);


private:
	rtlCipherError Update(const BYTE *pData, const size_t nDatLen, BYTE *pBuffer, const size_t nBufLen);

private:
	static const unsigned int CIPHER_CBLOCK_ARCFOUR = 256;
	typedef struct
	{
		unsigned int m_S[CIPHER_CBLOCK_ARCFOUR];
		unsigned int m_X, m_Y;
	} CipherContext;

	rtlCipherDirection  m_direction;
	CipherContext m_context;

};



#pragma once

#include "../../../Common/DocxFormat/Source/Base/Types_32.h"

class DigestMD5
{
public:
	typedef enum 
	{
		rtl_Digest_E_None,
		rtl_Digest_E_Argument,
		rtl_Digest_E_BufferSize,
	} rtlDigestError;

public:
	DigestMD5();

	rtlDigestError Update(const void *pData, const unsigned int nDatLen);
	rtlDigestError Get(unsigned char *pBuffer, const unsigned int nBufLen);
	rtlDigestError Raw (unsigned char *pBuffer, const unsigned int nBufLen);

	static const unsigned int RTL_DIGEST_LENGTH_MD5 = 16;

private:
	void InitContext();
	void UpdateContext();
	void EndContext();

private:
	static const _UINT32 DIGEST_CBLOCK_MD5 = 64;
	static const _UINT32 DIGEST_LBLOCK_MD5 = 16;

	typedef struct digestMD5_context_st
	{
		_UINT32 m_nDatLen;
		_UINT32 m_pData[DIGEST_LBLOCK_MD5];
		_UINT32 m_nA, m_nB, m_nC, m_nD;
		_UINT32 m_nL, m_nH;
	} DigestContextMD5;

	DigestContextMD5 m_context;
};


#pragma once


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

	rtlDigestError Update(const void *pData, const DWORD nDatLen);
	rtlDigestError Get(BYTE *pBuffer, const DWORD nBufLen);
	rtlDigestError Raw (BYTE *pBuffer, const DWORD nBufLen);

	static const unsigned int RTL_DIGEST_LENGTH_MD5 = 16;

private:
	void InitContext();
	void UpdateContext();
	void EndContext();

private:
	static const unsigned int DIGEST_CBLOCK_MD5 = 64;
	static const unsigned int DIGEST_LBLOCK_MD5 = 16;

	typedef struct digestMD5_context_st
	{
		DWORD m_nDatLen;
		DWORD m_pData[DIGEST_LBLOCK_MD5];
		DWORD m_nA, m_nB, m_nC, m_nD;
		DWORD m_nL, m_nH;
	} DigestContextMD5;

	DigestContextMD5 m_context;
};


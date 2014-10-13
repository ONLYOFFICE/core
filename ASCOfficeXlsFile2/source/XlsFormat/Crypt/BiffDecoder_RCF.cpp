#include "precompiled_xls.h"

#include "BiffDecoder_RCF.h"

namespace CRYPT
{;


BiffDecoderBase::BiffDecoderBase() : mbValid(false)
{
}


BiffDecoderBase::~BiffDecoderBase()
{
}


bool BiffDecoderBase::verifyPassword(const std::wstring& rPassword)
{
	mbValid = implVerify(rPassword);
	return mbValid;
}


void BiffDecoderBase::decode(unsigned char* pnDestData, const unsigned char* pnSrcData, const __int64 nStreamPos, const unsigned __int16 nBytes)
{
	if(pnDestData && pnSrcData && (nBytes> 0))
	{
		if(mbValid)
			implDecode(pnDestData, pnSrcData, nStreamPos, nBytes);
		else
			memcpy(pnDestData, pnSrcData, nBytes);
	}
}


/** Returns the block index of the passed stream position for RCF decryption. */
int lclGetRcfBlock(__int64 nStreamPos)
{
	return static_cast<int>(nStreamPos / BIFF_RCF_BLOCKSIZE);
}

/** Returns the offset of the passed stream position in a block for RCF decryption. */
int lclGetRcfOffset(__int64 nStreamPos)
{
	return static_cast<int>(nStreamPos % BIFF_RCF_BLOCKSIZE);
}


BiffDecoder_RCF::BiffDecoder_RCF(unsigned char pnSalt[16], unsigned char pnVerifier[16], unsigned char pnVerifierHash[16])
:	maPassword(16, 0),
	maSalt(pnSalt, pnSalt + 16),
	maVerifier(pnVerifier, pnVerifier + 16),
	maVerifierHash(pnVerifierHash, pnVerifierHash + 16)
{
}


bool BiffDecoder_RCF::implVerify(const std::wstring& rPassword)
{
	int nLen = rPassword.length();
	if((0 <nLen) && (nLen <16))
	{
		// copy string to unsigned __int16 array
		maPassword.clear();
		maPassword.resize(16, 0);
		const wchar_t* pcChar = rPassword.c_str();
		const wchar_t* pcCharEnd = pcChar + nLen;
		std::vector<unsigned __int16>::iterator aIt = maPassword.begin();
		for(; pcChar <pcCharEnd; ++pcChar, ++aIt)
			*aIt = static_cast<unsigned __int16>(*pcChar);

		// init codec
		maCodec.initKey(&maPassword.front(), &maSalt.front());
		return maCodec.verifyKey(&maVerifier.front(), &maVerifierHash.front());
	}
	return false;
}

void BiffDecoder_RCF::implDecode(unsigned char* pnDestData, const unsigned char* pnSrcData, const __int64 nStreamPos, const unsigned __int16 nBytes)
{
	unsigned char* pnCurrDest = pnDestData;
	const unsigned char* pnCurrSrc = pnSrcData;
	__int64 nCurrPos = nStreamPos;
	unsigned __int16 nBytesLeft = nBytes;
	while(nBytesLeft> 0)
	{
		// initialize codec for current stream position
		maCodec.startBlock(lclGetRcfBlock(nCurrPos));
		maCodec.skip(lclGetRcfOffset(nCurrPos));

		// decode the block
		unsigned __int16 nBlockLeft = static_cast<unsigned __int16>(BIFF_RCF_BLOCKSIZE - lclGetRcfOffset(nCurrPos));
		unsigned __int16 nDecBytes = nBytesLeft < nBlockLeft ? nBytesLeft : nBlockLeft;
		maCodec.decode(pnCurrDest, pnCurrSrc, static_cast<int>(nDecBytes));

		// prepare for next block
		pnCurrDest += nDecBytes;
		pnCurrSrc += nDecBytes;
		nCurrPos += nDecBytes;
		nBytesLeft = nBytesLeft - nDecBytes;
	}
}


};



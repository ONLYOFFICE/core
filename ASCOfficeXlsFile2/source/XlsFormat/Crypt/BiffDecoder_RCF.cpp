#include "BiffDecoder_RCF.h"

namespace CRYPT
{


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


void BiffDecoderBase::decode(unsigned char* pnDestData, const unsigned char* pnSrcData, const long nStreamPos, const unsigned short nBytes)
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
int lclGetRcfBlock(long nStreamPos)
{
	return static_cast<int>(nStreamPos / BIFF_RCF_BLOCKSIZE);
}

/** Returns the offset of the passed stream position in a block for RCF decryption. */
int lclGetRcfOffset(long nStreamPos)
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
		// copy string to unsigned short array
		maPassword.clear();
		maPassword.resize(16, 0);
		const wchar_t* pcChar = rPassword.c_str();
		const wchar_t* pcCharEnd = pcChar + nLen;
		std::vector<unsigned short>::iterator aIt = maPassword.begin();
		for(; pcChar <pcCharEnd; ++pcChar, ++aIt)
			*aIt = static_cast<unsigned short>(*pcChar);

		// init codec
		maCodec.initKey(&maPassword.front(), &maSalt.front());
		return maCodec.verifyKey(&maVerifier.front(), &maVerifierHash.front());
	}
	return false;
}

void BiffDecoder_RCF::implDecode(unsigned char* pnDestData, const unsigned char* pnSrcData, const long nStreamPos, const unsigned short nBytes)
{
	unsigned char* pnCurrDest = pnDestData;
	const unsigned char* pnCurrSrc = pnSrcData;
	long nCurrPos = nStreamPos;
	unsigned short nBytesLeft = nBytes;
	while(nBytesLeft> 0)
	{
		// initialize codec for current stream position
		maCodec.startBlock(lclGetRcfBlock(nCurrPos));
		maCodec.skip(lclGetRcfOffset(nCurrPos));

		// decode the block
		unsigned short nBlockLeft = static_cast<unsigned short>(BIFF_RCF_BLOCKSIZE - lclGetRcfOffset(nCurrPos));
		unsigned short nDecBytes = nBytesLeft < nBlockLeft ? nBytesLeft : nBlockLeft;
		maCodec.decode(pnCurrDest, pnCurrSrc, static_cast<int>(nDecBytes));

		// prepare for next block
		pnCurrDest += nDecBytes;
		pnCurrSrc += nDecBytes;
		nCurrPos += nDecBytes;
		nBytesLeft = nBytesLeft - nDecBytes;
	}
}


};



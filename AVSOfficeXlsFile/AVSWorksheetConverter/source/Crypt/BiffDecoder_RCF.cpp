#include "stdafx.h"
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


void BiffDecoderBase::decode(BYTE* pnDestData, const BYTE* pnSrcData, const LONGLONG nStreamPos, const WORD nBytes)
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
int lclGetRcfBlock(LONGLONG nStreamPos)
{
	return static_cast<int>(nStreamPos / BIFF_RCF_BLOCKSIZE);
}

/** Returns the offset of the passed stream position in a block for RCF decryption. */
int lclGetRcfOffset(LONGLONG nStreamPos)
{
	return static_cast<int>(nStreamPos % BIFF_RCF_BLOCKSIZE);
}


BiffDecoder_RCF::BiffDecoder_RCF(BYTE pnSalt[16], BYTE pnVerifier[16], BYTE pnVerifierHash[16])
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
		// copy string to WORD array
		maPassword.clear();
		maPassword.resize(16, 0);
		const wchar_t* pcChar = rPassword.c_str();
		const wchar_t* pcCharEnd = pcChar + nLen;
		std::vector<WORD>::iterator aIt = maPassword.begin();
		for(; pcChar <pcCharEnd; ++pcChar, ++aIt)
			*aIt = static_cast<WORD>(*pcChar);

		// init codec
		maCodec.initKey(&maPassword.front(), &maSalt.front());
		return maCodec.verifyKey(&maVerifier.front(), &maVerifierHash.front());
	}
	return false;
}

void BiffDecoder_RCF::implDecode(BYTE* pnDestData, const BYTE* pnSrcData, const LONGLONG nStreamPos, const WORD nBytes)
{
	BYTE* pnCurrDest = pnDestData;
	const BYTE* pnCurrSrc = pnSrcData;
	LONGLONG nCurrPos = nStreamPos;
	WORD nBytesLeft = nBytes;
	while(nBytesLeft> 0)
	{
		// initialize codec for current stream position
		maCodec.startBlock(lclGetRcfBlock(nCurrPos));
		maCodec.skip(lclGetRcfOffset(nCurrPos));

		// decode the block
		WORD nBlockLeft = static_cast<WORD>(BIFF_RCF_BLOCKSIZE - lclGetRcfOffset(nCurrPos));
		WORD nDecBytes = min(nBytesLeft, nBlockLeft);
		maCodec.decode(pnCurrDest, pnCurrSrc, static_cast<int>(nDecBytes));

		// prepare for next block
		pnCurrDest += nDecBytes;
		pnCurrSrc += nDecBytes;
		nCurrPos += nDecBytes;
		nBytesLeft = nBytesLeft - nDecBytes;
	}
}


};



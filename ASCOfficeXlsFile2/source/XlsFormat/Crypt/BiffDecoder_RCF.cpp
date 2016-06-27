/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "BiffDecoder_RCF.h"

namespace CRYPT
{


BiffDecoderBase::BiffDecoderBase(int BLOCKSIZE) : mbValid(false)
{
	RCF_BLOCKSIZE = BLOCKSIZE;
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
int BiffDecoderBase::lclGetRcfBlock(long nStreamPos)
{
	return static_cast<int>(nStreamPos / RCF_BLOCKSIZE);
}

/** Returns the offset of the passed stream position in a block for RCF decryption. */
int BiffDecoderBase::lclGetRcfOffset(long nStreamPos)
{
	return static_cast<int>(nStreamPos % RCF_BLOCKSIZE);
}


BiffDecoder_RCF::BiffDecoder_RCF(unsigned char pnSalt[16], unsigned char pnVerifier[16], unsigned char pnVerifierHash[16], int BlockSize)
:	BiffDecoderBase(BlockSize),
	maPassword(16, 0),
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
		unsigned short nBlockLeft = static_cast<unsigned short>(get_BLOCKSIZE() - lclGetRcfOffset(nCurrPos));
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



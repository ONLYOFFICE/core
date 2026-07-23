/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
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
void BiffDecoderBase::decode(unsigned char* pnDestData, const unsigned char* pnSrcData, const unsigned short nBytes, const long block_index)
{
	if(pnDestData && pnSrcData && (nBytes> 0))
	{
		if(mbValid)
			implDecode(pnDestData, pnSrcData, nBytes, block_index);
		else
			memcpy(pnDestData, pnSrcData, nBytes);
	}}

void BiffDecoderBase::decode(unsigned char* pnDestData, const unsigned char* pnSrcData, const unsigned short nBytes, const long nStreamPos, const unsigned short block_size)
{
	if(pnDestData && pnSrcData && (nBytes> 0))
	{
		if(mbValid)
			implDecode(pnDestData, pnSrcData, nBytes, nStreamPos, block_size);
		else
			memcpy(pnDestData, pnSrcData, nBytes);
	}
}

BiffDecoder_RCF::BiffDecoder_RCF(unsigned char pnSalt[16], unsigned char pnVerifier[16], unsigned char pnVerifierHash[16])
:	
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
void BiffDecoder_RCF::implDecode(unsigned char* pnDestData, const unsigned char* pnSrcData, const unsigned short nBytes, const long block_index)
{
	maCodec.startBlock(block_index);
	maCodec.decode(pnDestData, pnSrcData, static_cast<int>(nBytes));
}
void BiffDecoder_RCF::implDecode(unsigned char* pnDestData, const unsigned char* pnSrcData, const unsigned short nBytes, const long nStreamPos, const unsigned short block_size)
{
	unsigned char*			pnCurrDest	= pnDestData;
	const unsigned char*	pnCurrSrc	= pnSrcData;
	long					nCurrPos	= nStreamPos;
	unsigned short			nBytesLeft	= nBytes;
	
	while(nBytesLeft > 0)
	{
		// initialize codec for current stream position
		maCodec.startBlock (nCurrPos / block_size);

		const long offset = nCurrPos % block_size;
		maCodec.skip (offset);

		// decode the block
		unsigned short nBlockLeft = static_cast<unsigned short>(block_size - offset);
		unsigned short nDecBytes = nBytesLeft < nBlockLeft ? nBytesLeft : nBlockLeft;
		
		maCodec.decode(pnCurrDest, pnCurrSrc, static_cast<int>(nDecBytes));

		// prepare for next block
		pnCurrDest	+= nDecBytes;
		pnCurrSrc	+= nDecBytes;
		nCurrPos	+= nDecBytes;

		nBytesLeft = nBytesLeft - nDecBytes;
	}
}


};



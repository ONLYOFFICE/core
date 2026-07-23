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

#pragma once

#include "Types.h"
#include "JpgEvent.h"
#include "Utils.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Bit input-output stream (BitIO)
	//-------------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------------
	// Helper functions
	//-------------------------------------------------------------------------------------------------------------------------------
	static int  BitIO_ByteOut(BitIO *pBIO)
	{
		pBIO->nBuffer = (pBIO->nBuffer << 8) & 0xffff;
		pBIO->nValidBitsCount = pBIO->nBuffer == 0xff00 ? 7 : 8;

		if (pBIO->pCurPos >= pBIO->pEnd)
		{
			return 1;
		}
		*pBIO->pCurPos++ = pBIO->nBuffer >> 8;
		return 0;
	}

	static int  BitIO_ByteIn(BitIO *pBIO)
	{
		pBIO->nBuffer = (pBIO->nBuffer << 8) & 0xffff;
		pBIO->nValidBitsCount = pBIO->nBuffer == 0xff00 ? 7 : 8;

		if (pBIO->pCurPos >= pBIO->pEnd)
		{
			return 1;
		}
		pBIO->nBuffer |= *pBIO->pCurPos++;
		return 0;
	}

	static void BitIO_PutBit(BitIO *pBIO, int nBit)
	{
		if (0 == pBIO->nValidBitsCount)
		{
			BitIO_ByteOut(pBIO);
		}
		pBIO->nValidBitsCount--;
		pBIO->nBuffer |= nBit << pBIO->nValidBitsCount;
	}

	static int  BitIO_GetBit(BitIO *pBIO)
	{
		if (0 == pBIO->nValidBitsCount)
		{
			BitIO_ByteIn(pBIO);
		}
		pBIO->nValidBitsCount--;
		return (pBIO->nBuffer >> pBIO->nValidBitsCount) & 1;
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	// Main functions
	//-------------------------------------------------------------------------------------------------------------------------------
	BitIO* BitIO_Create()
	{
		BitIO *pBIO = (BitIO*)Malloc(sizeof(BitIO));
		return pBIO;
	}

	void   BitIO_Destroy(BitIO *pBIO)
	{
		Free(pBIO);
	}

	int    BitIO_WrittenBytesCount(BitIO *pBIO)
	{
		return (pBIO->pCurPos - pBIO->pStart);
	}

	void   BitIO_InitEncoder(BitIO *pBIO, unsigned char *pBuffer, int nLen)
	{
		pBIO->pStart  = pBuffer;
		pBIO->pEnd    = pBuffer + nLen;
		pBIO->pCurPos = pBuffer;
		pBIO->nBuffer = 0;
		pBIO->nValidBitsCount = 8;
	}
	void   BitIO_InitDecoder(BitIO *pBIO, unsigned char *pBuffer, int nLen)
	{
		pBIO->pStart  = pBuffer;
		pBIO->pEnd    = pBuffer + nLen;
		pBIO->pCurPos = pBuffer;
		pBIO->nBuffer = 0;
		pBIO->nValidBitsCount = 0;
	}

	void   BitIO_Write(BitIO *pBIO, int nValue, int nLen)
	{
		for (int nIndex = nLen - 1; nIndex >= 0; nIndex--)
		{
			BitIO_PutBit(pBIO, (nValue >> nIndex) & 1);
		}
	}

	int    BitIO_Read(BitIO *pBIO, int nLen)
	{
		int nResult = 0;
		for (int nIndex = nLen - 1; nIndex >= 0; nIndex--)
		{
			nResult += BitIO_GetBit(pBIO) << nIndex;
		}
		return nResult;
	}

	int    BitIO_Flush(BitIO *pBIO)
	{
		pBIO->nValidBitsCount = 0;

		if (BitIO_ByteOut(pBIO))
		{
			return 1;
		}
		if (7 == pBIO->nValidBitsCount)
		{
			pBIO->nValidBitsCount = 0;
			if (BitIO_ByteOut(pBIO))
			{
				return 1;
			}
		}
		return 0;
	}
	int    BitIO_InAlign(BitIO *pBIO)
	{
		pBIO->nValidBitsCount = 0;

		if ((pBIO->nBuffer & 0xff) == 0xff)
		{
			if (BitIO_ByteIn(pBIO))
			{
				return 1;
			}
			pBIO->nValidBitsCount = 0;
		}
		return 0;
	}

}
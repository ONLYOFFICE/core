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
	// Вспомогательные функции
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
	// Основные функции
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
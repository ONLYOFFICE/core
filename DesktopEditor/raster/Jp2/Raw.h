#pragma once

#include "Types.h"
#include "Utils.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	Raw* Raw_Create()
	{
		Raw *pRaw = (Raw*)Malloc(sizeof(Raw));
		return pRaw;
	}

	void Raw_Destroy(Raw *pRaw)
	{
		Free(pRaw);
	}

	int  Raw_BytesCount(Raw *pRaw)
	{
		return pRaw->pBufferPointer - pRaw->pBufferStart;
	}

	void Raw_InitDecoder(Raw *pRaw, unsigned char *pBuffer, int nLen)
	{
		pRaw->pBufferStart   = pBuffer;
		pRaw->nMaxLen        = nLen;
		pRaw->nLen           = 0;
		pRaw->nBuffer        = 0;
		pRaw->nFreeBitsCount = 0;
	}

	int  Raw_Decode(Raw *pRaw)
	{
		if (pRaw->nFreeBitsCount == 0)
		{
			pRaw->nFreeBitsCount = 8;
			if (pRaw->nLen == pRaw->nMaxLen)
			{
				pRaw->nBuffer = 0xff;
			}
			else
			{
				if (pRaw->nBuffer == 0xff)
				{
					pRaw->nFreeBitsCount = 7;
				}
				pRaw->nBuffer = *(pRaw->pBufferStart + pRaw->nLen);
				pRaw->nLen++;
			}
		}
		pRaw->nFreeBitsCount--;
		int nResult = (pRaw->nBuffer >> pRaw->nFreeBitsCount) & 0x01;

		return nResult;
	}
}
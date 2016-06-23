#pragma once

#include "Types.h"
#include "Utils.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------

	// Данный массив содержит все возможные состояния (см. Annex C Table C-2)
	static MQCoderState aoMQCoderStates[47 * 2] =
	{
		{ 0x5601, 0, &aoMQCoderStates[2], &aoMQCoderStates[3] },
		{ 0x5601, 1, &aoMQCoderStates[3], &aoMQCoderStates[2] },
		{ 0x3401, 0, &aoMQCoderStates[4], &aoMQCoderStates[12] },
		{ 0x3401, 1, &aoMQCoderStates[5], &aoMQCoderStates[13] },
		{ 0x1801, 0, &aoMQCoderStates[6], &aoMQCoderStates[18] },
		{ 0x1801, 1, &aoMQCoderStates[7], &aoMQCoderStates[19] },
		{ 0x0ac1, 0, &aoMQCoderStates[8], &aoMQCoderStates[24] },
		{ 0x0ac1, 1, &aoMQCoderStates[9], &aoMQCoderStates[25] },
		{ 0x0521, 0, &aoMQCoderStates[10], &aoMQCoderStates[58] },
		{ 0x0521, 1, &aoMQCoderStates[11], &aoMQCoderStates[59] },
		{ 0x0221, 0, &aoMQCoderStates[76], &aoMQCoderStates[66] },
		{ 0x0221, 1, &aoMQCoderStates[77], &aoMQCoderStates[67] },
		{ 0x5601, 0, &aoMQCoderStates[14], &aoMQCoderStates[13] },
		{ 0x5601, 1, &aoMQCoderStates[15], &aoMQCoderStates[12] },
		{ 0x5401, 0, &aoMQCoderStates[16], &aoMQCoderStates[28] },
		{ 0x5401, 1, &aoMQCoderStates[17], &aoMQCoderStates[29] },
		{ 0x4801, 0, &aoMQCoderStates[18], &aoMQCoderStates[28] },
		{ 0x4801, 1, &aoMQCoderStates[19], &aoMQCoderStates[29] },
		{ 0x3801, 0, &aoMQCoderStates[20], &aoMQCoderStates[28] },
		{ 0x3801, 1, &aoMQCoderStates[21], &aoMQCoderStates[29] },
		{ 0x3001, 0, &aoMQCoderStates[22], &aoMQCoderStates[34] },
		{ 0x3001, 1, &aoMQCoderStates[23], &aoMQCoderStates[35] },
		{ 0x2401, 0, &aoMQCoderStates[24], &aoMQCoderStates[36] },
		{ 0x2401, 1, &aoMQCoderStates[25], &aoMQCoderStates[37] },
		{ 0x1c01, 0, &aoMQCoderStates[26], &aoMQCoderStates[40] },
		{ 0x1c01, 1, &aoMQCoderStates[27], &aoMQCoderStates[41] },
		{ 0x1601, 0, &aoMQCoderStates[58], &aoMQCoderStates[42] },
		{ 0x1601, 1, &aoMQCoderStates[59], &aoMQCoderStates[43] },
		{ 0x5601, 0, &aoMQCoderStates[30], &aoMQCoderStates[29] },
		{ 0x5601, 1, &aoMQCoderStates[31], &aoMQCoderStates[28] },
		{ 0x5401, 0, &aoMQCoderStates[32], &aoMQCoderStates[28] },
		{ 0x5401, 1, &aoMQCoderStates[33], &aoMQCoderStates[29] },
		{ 0x5101, 0, &aoMQCoderStates[34], &aoMQCoderStates[30] },
		{ 0x5101, 1, &aoMQCoderStates[35], &aoMQCoderStates[31] },
		{ 0x4801, 0, &aoMQCoderStates[36], &aoMQCoderStates[32] },
		{ 0x4801, 1, &aoMQCoderStates[37], &aoMQCoderStates[33] },
		{ 0x3801, 0, &aoMQCoderStates[38], &aoMQCoderStates[34] },
		{ 0x3801, 1, &aoMQCoderStates[39], &aoMQCoderStates[35] },
		{ 0x3401, 0, &aoMQCoderStates[40], &aoMQCoderStates[36] },
		{ 0x3401, 1, &aoMQCoderStates[41], &aoMQCoderStates[37] },
		{ 0x3001, 0, &aoMQCoderStates[42], &aoMQCoderStates[38] },
		{ 0x3001, 1, &aoMQCoderStates[43], &aoMQCoderStates[39] },
		{ 0x2801, 0, &aoMQCoderStates[44], &aoMQCoderStates[38] },
		{ 0x2801, 1, &aoMQCoderStates[45], &aoMQCoderStates[39] },
		{ 0x2401, 0, &aoMQCoderStates[46], &aoMQCoderStates[40] },
		{ 0x2401, 1, &aoMQCoderStates[47], &aoMQCoderStates[41] },
		{ 0x2201, 0, &aoMQCoderStates[48], &aoMQCoderStates[42] },
		{ 0x2201, 1, &aoMQCoderStates[49], &aoMQCoderStates[43] },
		{ 0x1c01, 0, &aoMQCoderStates[50], &aoMQCoderStates[44] },
		{ 0x1c01, 1, &aoMQCoderStates[51], &aoMQCoderStates[45] },
		{ 0x1801, 0, &aoMQCoderStates[52], &aoMQCoderStates[46] },
		{ 0x1801, 1, &aoMQCoderStates[53], &aoMQCoderStates[47] },
		{ 0x1601, 0, &aoMQCoderStates[54], &aoMQCoderStates[48] },
		{ 0x1601, 1, &aoMQCoderStates[55], &aoMQCoderStates[49] },
		{ 0x1401, 0, &aoMQCoderStates[56], &aoMQCoderStates[50] },
		{ 0x1401, 1, &aoMQCoderStates[57], &aoMQCoderStates[51] },
		{ 0x1201, 0, &aoMQCoderStates[58], &aoMQCoderStates[52] },
		{ 0x1201, 1, &aoMQCoderStates[59], &aoMQCoderStates[53] },
		{ 0x1101, 0, &aoMQCoderStates[60], &aoMQCoderStates[54] },
		{ 0x1101, 1, &aoMQCoderStates[61], &aoMQCoderStates[55] },
		{ 0x0ac1, 0, &aoMQCoderStates[62], &aoMQCoderStates[56] },
		{ 0x0ac1, 1, &aoMQCoderStates[63], &aoMQCoderStates[57] },
		{ 0x09c1, 0, &aoMQCoderStates[64], &aoMQCoderStates[58] },
		{ 0x09c1, 1, &aoMQCoderStates[65], &aoMQCoderStates[59] },
		{ 0x08a1, 0, &aoMQCoderStates[66], &aoMQCoderStates[60] },
		{ 0x08a1, 1, &aoMQCoderStates[67], &aoMQCoderStates[61] },
		{ 0x0521, 0, &aoMQCoderStates[68], &aoMQCoderStates[62] },
		{ 0x0521, 1, &aoMQCoderStates[69], &aoMQCoderStates[63] },
		{ 0x0441, 0, &aoMQCoderStates[70], &aoMQCoderStates[64] },
		{ 0x0441, 1, &aoMQCoderStates[71], &aoMQCoderStates[65] },
		{ 0x02a1, 0, &aoMQCoderStates[72], &aoMQCoderStates[66] },
		{ 0x02a1, 1, &aoMQCoderStates[73], &aoMQCoderStates[67] },
		{ 0x0221, 0, &aoMQCoderStates[74], &aoMQCoderStates[68] },
		{ 0x0221, 1, &aoMQCoderStates[75], &aoMQCoderStates[69] },
		{ 0x0141, 0, &aoMQCoderStates[76], &aoMQCoderStates[70] },
		{ 0x0141, 1, &aoMQCoderStates[77], &aoMQCoderStates[71] },
		{ 0x0111, 0, &aoMQCoderStates[78], &aoMQCoderStates[72] },
		{ 0x0111, 1, &aoMQCoderStates[79], &aoMQCoderStates[73] },
		{ 0x0085, 0, &aoMQCoderStates[80], &aoMQCoderStates[74] },
		{ 0x0085, 1, &aoMQCoderStates[81], &aoMQCoderStates[75] },
		{ 0x0049, 0, &aoMQCoderStates[82], &aoMQCoderStates[76] },
		{ 0x0049, 1, &aoMQCoderStates[83], &aoMQCoderStates[77] },
		{ 0x0025, 0, &aoMQCoderStates[84], &aoMQCoderStates[78] },
		{ 0x0025, 1, &aoMQCoderStates[85], &aoMQCoderStates[79] },
		{ 0x0015, 0, &aoMQCoderStates[86], &aoMQCoderStates[80] },
		{ 0x0015, 1, &aoMQCoderStates[87], &aoMQCoderStates[81] },
		{ 0x0009, 0, &aoMQCoderStates[88], &aoMQCoderStates[82] },
		{ 0x0009, 1, &aoMQCoderStates[89], &aoMQCoderStates[83] },
		{ 0x0005, 0, &aoMQCoderStates[90], &aoMQCoderStates[84] },
		{ 0x0005, 1, &aoMQCoderStates[91], &aoMQCoderStates[85] },
		{ 0x0001, 0, &aoMQCoderStates[90], &aoMQCoderStates[86] },
		{ 0x0001, 1, &aoMQCoderStates[91], &aoMQCoderStates[87] },
		{ 0x5601, 0, &aoMQCoderStates[92], &aoMQCoderStates[92] },
		{ 0x5601, 1, &aoMQCoderStates[93], &aoMQCoderStates[93] },
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	static void MQC_ByteOut(MQCoder *pMQCoder)
	{
		if (*pMQCoder->pBufferPointer == 0xff)
		{
			pMQCoder->pBufferPointer++;
			*pMQCoder->pBufferPointer = pMQCoder->nCregister >> 20;
			pMQCoder->nCregister &= 0xfffff;
			pMQCoder->nCt = 7;
		}
		else
		{
			if ((pMQCoder->nCregister & 0x8000000) == 0) // CHANGE
			{
				pMQCoder->pBufferPointer++;
				*pMQCoder->pBufferPointer = pMQCoder->nCregister >> 19;
				pMQCoder->nCregister &= 0x7ffff;
				pMQCoder->nCt = 8;
			}
			else
			{
				(*pMQCoder->pBufferPointer)++;
				if (*pMQCoder->pBufferPointer == 0xff)
				{
					pMQCoder->nCregister &= 0x7ffffff;
					pMQCoder->pBufferPointer++;
					*pMQCoder->pBufferPointer = pMQCoder->nCregister >> 20;
					pMQCoder->nCregister &= 0xfffff;
					pMQCoder->nCt = 7;
				}
				else
				{
					pMQCoder->pBufferPointer++;
					*pMQCoder->pBufferPointer = pMQCoder->nCregister >> 19;
					pMQCoder->nCregister &= 0x7ffff;
					pMQCoder->nCt = 8;
				}
			}
		}
	}

	static void MQC_RenormE(MQCoder *pMQCoder)
	{
		do
		{
			pMQCoder->nAregister <<= 1;
			pMQCoder->nCregister <<= 1;
			pMQCoder->nCt--;

			if (0 == pMQCoder->nCt)
			{
				MQC_ByteOut(pMQCoder);
			}
		} while ((pMQCoder->nAregister & 0x8000) == 0);
	}

	static void MQC_CodeMPS(MQCoder *pMQCoder)
	{
		pMQCoder->nAregister -= (*pMQCoder->ppCurContext)->nLPSValue;
		if ((pMQCoder->nAregister & 0x8000) == 0)
		{
			if (pMQCoder->nAregister < (*pMQCoder->ppCurContext)->nLPSValue)
			{
				pMQCoder->nAregister = (*pMQCoder->ppCurContext)->nLPSValue;
			}
			else
			{
				pMQCoder->nCregister += (*pMQCoder->ppCurContext)->nLPSValue;
			}
			*pMQCoder->ppCurContext = (*pMQCoder->ppCurContext)->pNextMPS;
			MQC_RenormE(pMQCoder);
		}
		else
		{
			pMQCoder->nCregister += (*pMQCoder->ppCurContext)->nLPSValue;
		}
	}

	static void MQC_CodeLPS(MQCoder *pMQCoder)
	{
		pMQCoder->nAregister -= (*pMQCoder->ppCurContext)->nLPSValue;
		if (pMQCoder->nAregister < (*pMQCoder->ppCurContext)->nLPSValue)
		{
			pMQCoder->nCregister += (*pMQCoder->ppCurContext)->nLPSValue;
		}
		else
		{
			pMQCoder->nAregister = (*pMQCoder->ppCurContext)->nLPSValue;
		}
		*pMQCoder->ppCurContext = (*pMQCoder->ppCurContext)->pNextLPS;
		MQC_RenormE(pMQCoder);
	}

	static void MQC_SetBits(MQCoder *pMQCoder)
	{
		unsigned int nTempCode = pMQCoder->nCregister + pMQCoder->nAregister;
		pMQCoder->nCregister |= 0xffff;
		if (pMQCoder->nCregister >= nTempCode)
		{
			pMQCoder->nCregister -= 0x8000;
		}
	}

	static int  MQC_MPSExchange(MQCoder *pMQCoder)
	{
		int nResult = 0;
		if (pMQCoder->nAregister < (*pMQCoder->ppCurContext)->nLPSValue)
		{
			nResult = 1 - (*pMQCoder->ppCurContext)->nMPS;
			*pMQCoder->ppCurContext = (*pMQCoder->ppCurContext)->pNextLPS;
		}
		else
		{
			nResult = (*pMQCoder->ppCurContext)->nMPS;
			*pMQCoder->ppCurContext = (*pMQCoder->ppCurContext)->pNextMPS;
		}

		return nResult;
	}

	static int  MQC_LPSExchange(MQCoder *pMQCoder)
	{
		int nResult = 0;
		if (pMQCoder->nAregister < (*pMQCoder->ppCurContext)->nLPSValue)
		{
			pMQCoder->nAregister = (*pMQCoder->ppCurContext)->nLPSValue;
			nResult = (*pMQCoder->ppCurContext)->nMPS;
			*pMQCoder->ppCurContext = (*pMQCoder->ppCurContext)->pNextMPS;
		}
		else
		{
			pMQCoder->nAregister = (*pMQCoder->ppCurContext)->nLPSValue;
			nResult = 1 - (*pMQCoder->ppCurContext)->nMPS;
			*pMQCoder->ppCurContext = (*pMQCoder->ppCurContext)->pNextLPS;
		}

		return nResult;
	}

	static void MQC_ByteIn(MQCoder *pMQCoder)
	{
		if (pMQCoder->pBufferPointer != pMQCoder->pEnd)
		{
			unsigned int nCode = 0;
			if (pMQCoder->pBufferPointer + 1 != pMQCoder->pEnd)
			{
				nCode = *(pMQCoder->pBufferPointer + 1);
			}
			else
			{
				nCode = 0xff;
			}
			if (*pMQCoder->pBufferPointer == 0xff)
			{
				if (nCode > 0x8f)
				{
					pMQCoder->nCregister += 0xff00;
					pMQCoder->nCt = 8;
				}
				else
				{
					pMQCoder->pBufferPointer++;
					pMQCoder->nCregister += nCode << 9;
					pMQCoder->nCt = 7;
				}
			}
			else
			{
				pMQCoder->pBufferPointer++;
				pMQCoder->nCregister += nCode << 8;
				pMQCoder->nCt = 8;
			}
		}
		else
		{
			pMQCoder->nCregister += 0xff00;
			pMQCoder->nCt = 8;
		}
	}

	static void MQC_RenormD(MQCoder *pMQCoder)
	{
		do
		{
			if (pMQCoder->nCt == 0)
			{
				MQC_ByteIn(pMQCoder);
			}
			pMQCoder->nAregister <<= 1;
			pMQCoder->nCregister <<= 1;
			pMQCoder->nCt--;
		} while (pMQCoder->nAregister < 0x8000);
	}



	//-------------------------------------------------------------------------------------------------------------------------------
	// Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	MQCoder* MQC_Create()
	{
		MQCoder *pCoder = (MQCoder*)Malloc(sizeof(MQCoder));
		return pCoder;
	}
	void     MQC_Destroy(MQCoder *pMQCoder)
	{
		Free(pMQCoder);
	}

	int      MQC_BytesCount(MQCoder *pMQCoder)
	{
		return pMQCoder->pBufferPointer - pMQCoder->pStart;
	}

	void     MQC_ResetStates(MQCoder *pMQCoder)
	{
		for (int nIndex = 0; nIndex < MQC_NUMCTXS; nIndex++)
		{
			pMQCoder->pContexts[nIndex] = aoMQCoderStates;
		}
	}

	void     MQC_SetState(MQCoder *pMQCoder, int nContextIndex, int nMSB, int nProb)
	{
		pMQCoder->pContexts[nContextIndex] = &aoMQCoderStates[nMSB + (nProb << 1)];
	}
	void     MQC_SetCurContext(MQCoder *pMQCoder, int nContextIndex)
	{
		pMQCoder->ppCurContext = &pMQCoder->pContexts[nContextIndex];
	}

	void     MQC_InitEncoder(MQCoder *pMQCoder, unsigned char *pBufferPointer)
	{
		MQC_SetCurContext(pMQCoder, 0);

		pMQCoder->nAregister     = 0x8000;
		pMQCoder->nCregister     = 0;
		pMQCoder->pBufferPointer = pBufferPointer - 1;
		pMQCoder->nCt            = 12;

		if (*pMQCoder->pBufferPointer == 0xff)
		{
			pMQCoder->nCt = 13;
		}
		pMQCoder->pStart = pBufferPointer;
	}
	void     MQC_Encode(MQCoder *pMQCoder, int nValue)
	{
		if ((*pMQCoder->ppCurContext)->nMPS == nValue)
		{
			MQC_CodeMPS(pMQCoder);
		}
		else
		{
			MQC_CodeLPS(pMQCoder);
		}
	}

	void     MQC_Flush(MQCoder *pMQCoder)
	{
		MQC_SetBits(pMQCoder);

		pMQCoder->nCregister <<= pMQCoder->nCt;
		MQC_ByteOut(pMQCoder);

		pMQCoder->nCregister <<= pMQCoder->nCt;
		MQC_ByteOut(pMQCoder);

		if (*pMQCoder->pBufferPointer != 0xff)
		{
			pMQCoder->pBufferPointer++;
		}
	}
	void     MQC_BypassInitEncoder(MQCoder *pMQCoder)
	{
		// TO DO: Данная функция не польностью протестирована
		pMQCoder->nCregister = 0;
		pMQCoder->nCt = 8;

		//if (pMQCoder->pBufferPointer == 0xff)
		//{
		//	pMQCoder->nCt = 7;
		//}
	}
	void     MQC_BypassEncoder(MQCoder *pMQCoder, int nValue)
	{
		// TO DO: Данная функция не польностью протестирована
		pMQCoder->nCt--;
		pMQCoder->nCregister = pMQCoder->nCregister + (nValue << pMQCoder->nCt);

		if (pMQCoder->nCt == 0)
		{
			pMQCoder->pBufferPointer++;
			*pMQCoder->pBufferPointer = pMQCoder->nCregister;
			pMQCoder->nCt = 8;

			if (*pMQCoder->pBufferPointer == 0xff)
			{
				pMQCoder->nCt = 7;
			}
			pMQCoder->nCregister = 0;
		}
	}

	int      MQC_BypassFlushEncoder(MQCoder *pMQCoder)
	{
		// TO DO: Данная функция не польностью протестирована
		unsigned char nBitPadding = 0;

		if (pMQCoder->nCt != 0)
		{
			while (pMQCoder->nCt > 0)
			{
				pMQCoder->nCt--;
				pMQCoder->nCregister += nBitPadding << pMQCoder->nCt;
				nBitPadding = (nBitPadding + 1) & 0x01;
			}
			pMQCoder->pBufferPointer++;
			*pMQCoder->pBufferPointer = pMQCoder->nCregister;
			pMQCoder->nCt = 8;
			pMQCoder->nCregister = 0;
		}

		return 1;
	}

	void     MQC_ResetEncoder(MQCoder *pMQCoder)
	{
		MQC_ResetStates(pMQCoder);
		MQC_SetState(pMQCoder, 18, 0, 46);
		MQC_SetState(pMQCoder, 0, 0, 3);
		MQC_SetState(pMQCoder, 1, 0, 4);
	}

	int      MQC_RestartEncoder(MQCoder *pMQCoder)
	{
		int nCorrection = 1;

		int nLen = 27 - 15 - pMQCoder->nCt;
		pMQCoder->nCregister <<= pMQCoder->nCt;
		while (nLen > 0)
		{
			MQC_ByteOut(pMQCoder);
			nLen -= pMQCoder->nCt;
			pMQCoder->nCregister <<= pMQCoder->nCt;
		}
		MQC_ByteOut(pMQCoder);

		return nCorrection;
	}

	void     MQC_RestartInitEncoder(MQCoder *pMQCoder)
	{
		MQC_SetCurContext(pMQCoder, 0);

		pMQCoder->nAregister = 0x8000;
		pMQCoder->nCregister = 0;
		pMQCoder->nCt = 12;
		pMQCoder->pBufferPointer--;

		if (*pMQCoder->pBufferPointer == 0xff)
		{
			pMQCoder->nCt = 13;
		}
	}

	void     MQC_ErtermEncoder(MQCoder *pMQCoder)
	{
		int nK = 11 - pMQCoder->nCt + 1;

		while (nK > 0)
		{
			pMQCoder->nCregister <<= pMQCoder->nCt;
			pMQCoder->nCt = 0;
			MQC_ByteOut(pMQCoder);
			nK -= pMQCoder->nCt;
		}

		if (*pMQCoder->pBufferPointer != 0xff)
		{
			MQC_ByteOut(pMQCoder);
		}
	}
	void     MQC_SegMarkEncoder(MQCoder *pMQCoder)
	{
		MQC_SetCurContext(pMQCoder, 18);

		for (int nIndex = 1; nIndex < 5; nIndex++)
		{
			MQC_Encode(pMQCoder, nIndex % 2);
		}
	}

	void     MQC_InitDecoder(MQCoder *pMQCoder, unsigned char *nBufferPointer, int nLen)
	{
		MQC_SetCurContext(pMQCoder, 0);

		pMQCoder->pStart         = nBufferPointer;
		pMQCoder->pEnd           = nBufferPointer + nLen;
		pMQCoder->pBufferPointer = nBufferPointer;

		if (0 == nLen)
			pMQCoder->nCregister = 0xff << 16;
		else
			pMQCoder->nCregister = *pMQCoder->pBufferPointer << 16;

		MQC_ByteIn(pMQCoder);

		pMQCoder->nCregister <<= 7;
		pMQCoder->nCt         -= 7;
		pMQCoder->nAregister   = 0x8000;
	}

	int      MQC_Decode(MQCoder *pMQCoder)
	{
		int nResult = 0;
		pMQCoder->nAregister -= (*pMQCoder->ppCurContext)->nLPSValue;
		if ((pMQCoder->nCregister >> 16) < (*pMQCoder->ppCurContext)->nLPSValue)
		{
			nResult = MQC_LPSExchange(pMQCoder);
			MQC_RenormD(pMQCoder);
		}
		else
		{
			pMQCoder->nCregister -= (*pMQCoder->ppCurContext)->nLPSValue << 16;
			if ((pMQCoder->nAregister & 0x8000) == 0)
			{
				nResult = MQC_MPSExchange(pMQCoder);
				MQC_RenormD(pMQCoder);
			}
			else
			{
				nResult = (*pMQCoder->ppCurContext)->nMPS;
			}
		}

		return nResult;
	}
}
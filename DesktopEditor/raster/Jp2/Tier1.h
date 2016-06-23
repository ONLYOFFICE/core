#pragma once

#include "DWT.h"
#include "ArithmeticCoder.h"
#include "Raw.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------

	static int    Tier1_GetContextIndexZC(Tier1 *pTier1, int nF, int nOrient)
	{
		return pTier1->lut_ctxno_zc[(nOrient << 8) | (nF & T1_SIG_OTH)];
	}

	static int    Tier1_GetContextIndexSC(Tier1 *pTier1, int nF)
	{
		return pTier1->lut_ctxno_sc[(nF & (T1_SIG_PRIM | T1_SGN)) >> 4];
	}

	static int    Tier1_GetContextIndexMag(Tier1 *pTier1, int nF)
	{
		return pTier1->lut_ctxno_mag[(nF & T1_SIG_OTH) | (((nF & T1_REFINE) != 0) << 11)];
	}

	static int    Tier1_GetSPB(Tier1 *pTier1, int nF)
	{
		return pTier1->lut_spb[(nF & (T1_SIG_PRIM | T1_SGN)) >> 4];
	}

	static int    Tier1_GetNMSEDecSig(Tier1 *pTier1, int nValue, int nBitPos)
	{
		if (nBitPos > T1_NMSEDEC_FRACBITS)
		{
			return pTier1->lut_nmsedec_sig[(nValue >> (nBitPos - T1_NMSEDEC_FRACBITS)) & ((1 << T1_NMSEDEC_BITS) - 1)];
		}

		return pTier1->lut_nmsedec_sig0[nValue & ((1 << T1_NMSEDEC_BITS) - 1)];
	}

	static int    Tier1_GetNMSEDecRef(Tier1 *pTier1, int nValue, int nBitPos)
	{
		if (nBitPos > T1_NMSEDEC_FRACBITS)
		{
			return pTier1->lut_nmsedec_ref[(nValue >> (nBitPos - T1_NMSEDEC_FRACBITS)) & ((1 << T1_NMSEDEC_BITS) - 1)];
		}

		return pTier1->lut_nmsedec_ref0[nValue & ((1 << T1_NMSEDEC_BITS) - 1)];
	}

	static void   Tier1_UpdateFlags(int *pFlags, int nSign)
	{
		int *pSouth = pFlags - (T1_MAXCBLKW + 2);
		int *pNorth = pFlags + (T1_MAXCBLKW + 2);

		pSouth[-1] |= T1_SIG_SE;
		pSouth[1] |= T1_SIG_SW;

		pNorth[-1] |= T1_SIG_NE;
		pNorth[1] |= T1_SIG_NW;

		*pSouth |= T1_SIG_S;
		*pNorth |= T1_SIG_N;

		pFlags[-1] |= T1_SIG_E;
		pFlags[1] |= T1_SIG_W;

		if (nSign)
		{
			*pSouth |= T1_SGN_S;
			*pNorth |= T1_SGN_N;
			pFlags[-1] |= T1_SGN_E;
			pFlags[1] |= T1_SGN_W;
		}
	}

	static void   Tier1_EncoderSigPassStep(Tier1 *pTier1, int *pFlags, int *pData, int nOrient, int nBufferPos, int nOne, int *pNMSEDec, char nType, int nVSC)
	{
		MQCoder *pMQCoder = pTier1->pMQCoder;

		int nFlag = nVSC ? ((*pFlags) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*pFlags);
		if ((nFlag & T1_SIG_OTH) && !(nFlag & (T1_SIG | T1_VISIT)))
		{
			int nValue = abs(*pData) & nOne ? 1 : 0;
			if (nType == T1_TYPE_RAW) // BYPASS/LAZY MODE
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexZC(pTier1, nFlag, nOrient)); // ESSAI
				MQC_BypassEncoder(pMQCoder, nValue);
			}
			else
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexZC(pTier1, nFlag, nOrient));
				MQC_Encode(pMQCoder, nValue);
			}
			if (nValue)
			{
				nValue = *pData < 0 ? 1 : 0;
				*pNMSEDec += Tier1_GetNMSEDecSig(pTier1, abs(*pData), nBufferPos + T1_NMSEDEC_FRACBITS);

				if (nType == T1_TYPE_RAW) // BYPASS/LAZY MODE 
				{
					MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexSC(pTier1, nFlag)); // ESSAI
					MQC_BypassEncoder(pMQCoder, nValue);
				}
				else
				{
					MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexSC(pTier1, nFlag));
					MQC_Encode(pMQCoder, nValue ^ Tier1_GetSPB(pTier1, nFlag));
				}
				Tier1_UpdateFlags(pFlags, nValue);
				*pFlags |= T1_SIG;
			}
			*pFlags |= T1_VISIT;
		}
	}

	static void   Tier1_DecoderSigPassStep(Tier1 *pTier1, int *pFlags, int *pData, int nOrient, int nOnePlusHalf, char nType, int nVSC)
	{
		Raw *pRaw = pTier1->pRaw;
		MQCoder *pMQCoder = pTier1->pMQCoder;
		int nValue = 0;
		int nFlag = nVSC ? ((*pFlags) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*pFlags);
		if ((nFlag & T1_SIG_OTH) && !(nFlag & (T1_SIG | T1_VISIT)))
		{
			if (nType == T1_TYPE_RAW)
			{
				if (Raw_Decode(pRaw))
				{
					nValue = Raw_Decode(pRaw); // ESSAI
					*pData = nValue ? -nOnePlusHalf : nOnePlusHalf;
					Tier1_UpdateFlags(pFlags, nValue);
					*pFlags |= T1_SIG;
				}
			}
			else
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexZC(pTier1, nFlag, nOrient));
				if (MQC_Decode(pMQCoder))
				{
					MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexSC(pTier1, nFlag));
					nValue = MQC_Decode(pMQCoder) ^ Tier1_GetSPB(pTier1, nFlag);
					*pData = nValue ? -nOnePlusHalf : nOnePlusHalf;
					Tier1_UpdateFlags(pFlags, nValue);
					*pFlags |= T1_SIG;
				}
			}
			*pFlags |= T1_VISIT;
		}
	}

	static void   Tier1_EncoderSigPass(Tier1 *pTier1, int nWidth, int nHeight, int nBufferPos, int nOrient, int *pNMSEDec, char nType, int nCodeBlockStyle)
	{
		*pNMSEDec = 0;
		int nOne = 1 << (nBufferPos + T1_NMSEDEC_FRACBITS);
		for (int nY = 0; nY < nHeight; nY += 4)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				for (int nYY = nY; nYY < nY + 4 && nYY < nHeight; nYY++)
				{
					int nVSC = ((nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC) && (nYY == nY + 3 || nYY == nHeight - 1)) ? 1 : 0;
					Tier1_EncoderSigPassStep(pTier1, &pTier1->aFlags[1 + nYY][1 + nX], &pTier1->aData[nYY][nX], nOrient, nBufferPos, nOne, pNMSEDec, nType, nVSC);
				}
			}
		}
	}

	static void   Tier1_DecoderSigPass(Tier1 *pTier1, int nWidth, int nHeight, int nBufferPos, int nOrient, char nType, int nCodeBlockStyle)
	{
		int nOne  = 1 << nBufferPos;
		int nHalf = nOne >> 1;
		int nOnePlusHalf = nOne | nHalf;

		for (int nY = 0; nY < nHeight; nY += 4)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				for (int nYY = nY; nYY < nY + 4 && nYY < nHeight; nYY++)
				{
					int nVSC = ((nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC) && (nYY == nY + 3 || nYY == nHeight - 1)) ? 1 : 0;
					Tier1_DecoderSigPassStep(pTier1, &pTier1->aFlags[1 + nYY][1 + nX], &pTier1->aData[nYY][nX], nOrient, nOnePlusHalf, nType, nVSC);
				}
			}
		}
	}

	static void   Tier1_EnocderRefPassStep(Tier1 *pTier1, int *pFlags, int *pData, int nBufferPos, int nOne, int *pNMSEDec, char nType, int nVSC)
	{
		MQCoder *pMQCoder = pTier1->pMQCoder;
		int nFlag = nVSC ? ((*pFlags) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*pFlags);

		if ((nFlag & (T1_SIG | T1_VISIT)) == T1_SIG)
		{
			*pNMSEDec += Tier1_GetNMSEDecRef(pTier1, abs(*pData), nBufferPos + T1_NMSEDEC_FRACBITS);
			int nValue = abs(*pData) & nOne ? 1 : 0;
			if (nType == T1_TYPE_RAW)
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexMag(pTier1, nFlag)); // ESSAI
				MQC_BypassEncoder(pMQCoder, nValue);
			}
			else
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexMag(pTier1, nFlag));
				MQC_Encode(pMQCoder, nValue);
			}
			*pFlags |= T1_REFINE;
		}
	}

	static void   Tier1_DecoderRefPassStep(Tier1 *pTier1, int *pFlags, int *pData, int nPosHalf, int nNegHalf, char nType, int nVSC)
	{
		MQCoder *pMQCoder = pTier1->pMQCoder;
		Raw *pRaw = pTier1->pRaw;

		int nFlag = nVSC ? ((*pFlags) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*pFlags);
		if ((nFlag & (T1_SIG | T1_VISIT)) == T1_SIG)
		{
			int nValue = 0;
			if (nType == T1_TYPE_RAW)
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexMag(pTier1, nFlag)); // ESSAI
				nValue = Raw_Decode(pRaw);
			}
			else
			{
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexMag(pTier1, nFlag));
				nValue = MQC_Decode(pMQCoder);
			}
			int nTemp = nValue ? nPosHalf : nNegHalf;
			*pData += *pData < 0 ? -nTemp : nTemp;
			*pFlags |= T1_REFINE;
		}
	}

	static void   Tier1_EncoderRefPass(Tier1 *pTier1, int nWidth, int nHeight, int nBufferPos, int *pNMSEDec, char nType, int nCodeBlockStyle)
	{
		*pNMSEDec = 0;
		int nOne = 1 << (nBufferPos + T1_NMSEDEC_FRACBITS);
		for (int nY = 0; nY < nHeight; nY += 4)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				for (int nYY = nY; nYY < nY + 4 && nYY < nHeight; nYY++)
				{
					int nVSC = ((nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC) && (nYY == nY + 3 || nYY == nHeight - 1)) ? 1 : 0;
					Tier1_EnocderRefPassStep(pTier1, &pTier1->aFlags[1 + nYY][1 + nX], &pTier1->aData[nYY][nX], nBufferPos, nOne, pNMSEDec, nType, nVSC);
				}
			}
		}
	}

	static void   Tier1_DecoderRefPass(Tier1 *pTier1, int nWidth, int nHeight, int nBufferPos, char nType, int nCodeBlockStyle)
	{
		int nOne = 1 << nBufferPos;
		int nPosHalf = nOne >> 1;
		int nNegHalf = nBufferPos > 0 ? -nPosHalf : -1;
		for (int nY = 0; nY < nHeight; nY += 4)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				for (int nYY = nY; nYY < nY + 4 && nYY < nHeight; nYY++)
				{
					int nVSC = ((nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC) && (nYY == nY + 3 || nYY == nHeight - 1)) ? 1 : 0;
					Tier1_DecoderRefPassStep(pTier1, &pTier1->aFlags[1 + nYY][1 + nX], &pTier1->aData[nYY][nX], nPosHalf, nNegHalf, nType, nVSC);
				}
			}
		}
	}

	static void   Tier1_EncoderClnPassStep(Tier1 *pTier1, int *pFlags, int *pData, int nOrient, int nBufferPos, int nOne, int *pNMSEDec, int nPartial, int nVSC)
	{
		MQCoder *pMQCoder = pTier1->pMQCoder;
		int nFlag = nVSC ? ((*pFlags) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*pFlags);
		int nValue = 0;
		if (nPartial)
		{
			goto LABEL_PARTIAL;
		}
		if (!(*pFlags & (T1_SIG | T1_VISIT)))
		{
			MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexZC(pTier1, nFlag, nOrient));
			nValue = abs(*pData) & nOne ? 1 : 0;
			MQC_Encode(pMQCoder, nValue);
			if (nValue)
			{
			LABEL_PARTIAL:
				*pNMSEDec += Tier1_GetNMSEDecSig(pTier1, abs(*pData), nBufferPos + T1_NMSEDEC_FRACBITS);
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexSC(pTier1, nFlag));
				nValue = *pData < 0 ? 1 : 0;
				MQC_Encode(pMQCoder, nValue ^ Tier1_GetSPB(pTier1, nFlag));
				Tier1_UpdateFlags(pFlags, nValue);
				*pFlags |= T1_SIG;
			}
		}
		*pFlags &= ~T1_VISIT;
	}
	static void   Tier1_DecoderClnPassStep(Tier1 *pTier1, int *pFlags, int *pData, int nOrient, int nOnePlusHalf, int nPartial, int nVSC)
	{
		MQCoder *pMQCoder = pTier1->pMQCoder;
		int nFlag = nVSC ? ((*pFlags) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*pFlags);
		int nValue = 0;
		if (nPartial)
		{
			goto LABEL_PARTIAL;
		}
		if (!(nFlag & (T1_SIG | T1_VISIT)))
		{
			MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexZC(pTier1, nFlag, nOrient));
			if (MQC_Decode(pMQCoder))
			{
			LABEL_PARTIAL:
				MQC_SetCurContext(pMQCoder, Tier1_GetContextIndexSC(pTier1, nFlag));
				nValue = MQC_Decode(pMQCoder) ^ Tier1_GetSPB(pTier1, nFlag);
				*pData = nValue ? -nOnePlusHalf : nOnePlusHalf;
				Tier1_UpdateFlags(pFlags, nValue);
				*pFlags |= T1_SIG;
			}
		}
		*pFlags &= ~T1_VISIT;
	}

	static void   Tier1_EncoderClnPass(Tier1 *pTier1, int nWidth, int nHeight, int nBufferPos, int nOrient, int *pNMSEDec, int nCodeBlockStyle)
	{
		MQCoder *pMQCoder = pTier1->pMQCoder;
		*pNMSEDec = 0;
		int nOne = 1 << (nBufferPos + T1_NMSEDEC_FRACBITS);

		for (int nY = 0; nY < nHeight; nY += 4)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				int nAgg = 0, nRunLen = 0;
				if (nY + 3 < nHeight)
				{
					if (nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC)
					{
						nAgg = !(pTier1->aFlags[1 + nY][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || pTier1->aFlags[1 + nY + 1][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH)
								 || pTier1->aFlags[1 + nY + 2][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || (pTier1->aFlags[1 + nY + 3][1 + nX] & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) & (T1_SIG | T1_VISIT | T1_SIG_OTH));
					}
					else
					{
						nAgg = !(pTier1->aFlags[1 + nY][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || pTier1->aFlags[1 + nY + 1][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH)
								 || pTier1->aFlags[1 + nY + 2][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || pTier1->aFlags[1 + nY + 3][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH));
					}
				}
				else
				{
					nAgg = 0;
				}
				if (nAgg)
				{
					for (nRunLen = 0; nRunLen < 4; nRunLen++)
					{
						if (abs(pTier1->aData[nY + nRunLen][nX]) & nOne)
							break;
					}
					MQC_SetCurContext(pMQCoder, T1_CTXNO_AGG);
					MQC_Encode(pMQCoder, nRunLen != 4);
					if (nRunLen == 4)
					{
						continue;
					}
					MQC_SetCurContext(pMQCoder, T1_CTXNO_UNI);
					MQC_Encode(pMQCoder, nRunLen >> 1);
					MQC_Encode(pMQCoder, nRunLen & 1);
				}
				else
				{
					nRunLen = 0;
				}
				for (int nYY = nY + nRunLen; nYY < nY + 4 && nYY < nHeight; nYY++)
				{
					int nVSC = ((nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC) && (nYY == nY + 3 || nYY == nHeight - 1)) ? 1 : 0;
					Tier1_EncoderClnPassStep(pTier1, &pTier1->aFlags[1 + nYY][1 + nX], &pTier1->aData[nYY][nX], nOrient, nBufferPos, nOne, pNMSEDec, nAgg && (nYY == nY + nRunLen), nVSC);
				}
			}
		}
	}

	static void   Tier1_DecoderClnPass(Tier1 *pTier1, int nWidth, int nHeight, int nBufferPos, int nOrient, int nCodeBlockStyle)
	{
		int nSegSym = nCodeBlockStyle & J2K_CCP_CBLKSTY_SEGSYM;

		MQCoder *pMQCoder = pTier1->pMQCoder;
		int nOne  = 1 << nBufferPos;
		int nHalf = nOne >> 1;
		int nOnePlusHalf = nOne | nHalf;

		for (int nY = 0; nY < nHeight; nY += 4)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
				int nAgg = 0, nRunLen = 0;
				if (nY + 3 < nHeight)
				{
					if (nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC)
					{
						nAgg = !(pTier1->aFlags[1 + nY][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || pTier1->aFlags[1 + nY + 1][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH)
								 || pTier1->aFlags[1 + nY + 2][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || (pTier1->aFlags[1 + nY + 3][1 + nX] & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) & (T1_SIG | T1_VISIT | T1_SIG_OTH));
					}
					else
					{
						nAgg = !(pTier1->aFlags[1 + nY][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || pTier1->aFlags[1 + nY + 1][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH)
								 || pTier1->aFlags[1 + nY + 2][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH) || pTier1->aFlags[1 + nY + 3][1 + nX] & (T1_SIG | T1_VISIT | T1_SIG_OTH));
					}
				}
				else
				{
					nAgg = 0;
				}
				if (nAgg)
				{
					MQC_SetCurContext(pMQCoder, T1_CTXNO_AGG);
					if (!MQC_Decode(pMQCoder))
					{
						continue;
					}
					MQC_SetCurContext(pMQCoder, T1_CTXNO_UNI);
					nRunLen = MQC_Decode(pMQCoder);
					nRunLen = (nRunLen << 1) | MQC_Decode(pMQCoder);
				}
				else
				{
					nRunLen = 0;
				}
				for (int nYY = nY + nRunLen; nYY < nY + 4 && nYY < nHeight; nYY++)
				{
					int nVSC = ((nCodeBlockStyle & J2K_CCP_CBLKSTY_VSC) && (nYY == nY + 3 || nYY == nHeight - 1)) ? 1 : 0;
					Tier1_DecoderClnPassStep(pTier1, &pTier1->aFlags[1 + nYY][1 + nX], &pTier1->aData[nYY][nX], nOrient, nOnePlusHalf, nAgg && (nYY == nY + nRunLen), nVSC);
				}
			}
		}
		if (nSegSym)
		{
			MQC_SetCurContext(pMQCoder, T1_CTXNO_UNI);
			int nValue = MQC_Decode(pMQCoder);
			nValue = (nValue << 1) | MQC_Decode(pMQCoder);
			nValue = (nValue << 1) | MQC_Decode(pMQCoder);
			nValue = (nValue << 1) | MQC_Decode(pMQCoder);

			// if ( 0xa != nValue ) 
			//{
			//	Event_Message( pTier1->pCodecInfo, EVT_WARNING, "Bad segmentation symbol %x\n", nValue );
			//} 
		}
	}


	// Mode fixed_quality
	static double Tier1_GetWMSEDec(int nNMSEDec, int nComponentIndex, int nLevel, int nOrient, int nBufferPos, int nQFMBID, double dStepSize, int nComponentsCount)
	{
		double dWidth1, dWidth2;
		if (1 == nQFMBID)
		{
			dWidth1 = (nComponentsCount > 1) ? GetNormRCT(nComponentIndex) : 1;
			dWidth2 = DWT_GetNormRev(nLevel, nOrient);
		}
		else
		{
			dWidth1 = (nComponentsCount > 1) ? GetNormICT(nComponentIndex) : 1;
			dWidth2 = DWT_GetNormIrr(nLevel, nOrient);
		}
		double dWMSEDec = dWidth1 * dWidth2 * dStepSize * (1 << nBufferPos);
		dWMSEDec *= dWMSEDec * nNMSEDec / 8192.0;

		return dWMSEDec;
	}

	static void   Tier1_EncodeCodeBlock(Tier1 *pTier1, CodeBlock *pCodeBlock, int nOrient, int nComponentIndex, int nLevel, int nQFMBID, double dStepSize, int nCodeBlockStyle, int nComponentsCount, Tile *pTile)
	{
		int nNMSEDec = 0;
		double dCumWMSEDec = 0;
		char nType = T1_TYPE_MQ;

		MQCoder *pMQCoder = pTier1->pMQCoder;

		int nWidth  = pCodeBlock->nX1 - pCodeBlock->nX0;
		int nHeight = pCodeBlock->nY1 - pCodeBlock->nY0;

		int nMax = 0;
		for (int nY = 0; nY < nHeight; nY++)
		{
			for (int nX = 0; nX < nWidth; nX++)
			{
                nMax = (std::max)(nMax, abs(pTier1->aData[nY][nX]));
			}
		}

		pCodeBlock->nBPSCount = nMax ? (FloorLog2(nMax) + 1) - T1_NMSEDEC_FRACBITS : 0;

		for (int nX = 0; nX <= nWidth; nX++)
		{
			for (int nY = 0; nY <= nHeight; nY++)
			{
				pTier1->aFlags[nY][nX] = 0;
			}
		}

		int nBufferPos = pCodeBlock->nBPSCount - 1;
		int nPassType = 2;

		MQC_ResetStates(pMQCoder);
		MQC_SetState(pMQCoder, T1_CTXNO_UNI, 0, 46);
		MQC_SetState(pMQCoder, T1_CTXNO_AGG, 0, 3);
		MQC_SetState(pMQCoder, T1_CTXNO_ZC, 0, 4);
		MQC_InitEncoder(pMQCoder, pCodeBlock->aData);

		int nPassIndex;
		for (nPassIndex = 0; nBufferPos >= 0; nPassIndex++)
		{
			TCDPass *pPass = &pCodeBlock->aPasses[nPassIndex];
			int nCorrection = 3;
			nType = ((nBufferPos < (pCodeBlock->nBPSCount - 4)) && (nPassType < 2) && (nCodeBlockStyle & J2K_CCP_CBLKSTY_LAZY)) ? T1_TYPE_RAW : T1_TYPE_MQ;

			switch (nPassType)
			{
				case 0:
				Tier1_EncoderSigPass(pTier1, nWidth, nHeight, nBufferPos, nOrient, &nNMSEDec, nType, nCodeBlockStyle);
				break;
				case 1:
				Tier1_EncoderRefPass(pTier1, nWidth, nHeight, nBufferPos, &nNMSEDec, nType, nCodeBlockStyle);
				break;
				case 2:
				Tier1_EncoderClnPass(pTier1, nWidth, nHeight, nBufferPos, nOrient, &nNMSEDec, nCodeBlockStyle);
				// Code switch SEGMARK (SEGSYM)
				if (nCodeBlockStyle & J2K_CCP_CBLKSTY_SEGSYM)
					MQC_SegMarkEncoder(pMQCoder);
				break;
			}

			// fixed_quality
			dCumWMSEDec += Tier1_GetWMSEDec(nNMSEDec, nComponentIndex, nLevel, nOrient, nBufferPos, nQFMBID, dStepSize, nComponentsCount);
			pTile->dDistoTile += Tier1_GetWMSEDec(nNMSEDec, nComponentIndex, nLevel, nOrient, nBufferPos, nQFMBID, dStepSize, nComponentsCount);

			// Code switch "RESTART" (TERMALL)
			if ((nCodeBlockStyle & J2K_CCP_CBLKSTY_TERMALL) && !((nPassType == 2) && (nBufferPos - 1 < 0)))
			{
				if (nType == T1_TYPE_RAW)
				{
					MQC_Flush(pMQCoder);
					nCorrection = 1;
					//nCorrection = MQC_BypassFlushEncoder();
				}
				else
				{
					//nCorrection = MQC_RestartEncoder();
					MQC_Flush(pMQCoder);
					nCorrection = 1;
				}
				pPass->nTerm = 1;
			}
			else
			{
				if (((nBufferPos < (pCodeBlock->nBPSCount - 4) && (nPassType > 0)) || ((nBufferPos == (pCodeBlock->nBPSCount - 4)) && (nPassType == 2))) && (nCodeBlockStyle & J2K_CCP_CBLKSTY_LAZY))
				{
					if (nType == T1_TYPE_RAW)
					{
						MQC_Flush(pMQCoder);
						nCorrection = 1;
						//nCorrection = MQC_BypassFlushEncoder();
					}
					else
					{
						//nCorrection = MQC_RestartEncoder();
						MQC_Flush(pMQCoder);
						nCorrection = 1;
					}
					pPass->nTerm = 1;
				}
				else
				{
					pPass->nTerm = 0;
				}
			}

			if (++nPassType == 3)
			{
				nPassType = 0;
				nBufferPos--;
			}

			if (pPass->nTerm && nBufferPos > 0)
			{
				nType = ((nBufferPos < (pCodeBlock->nBPSCount - 4)) && (nPassType < 2) && (nCodeBlockStyle & J2K_CCP_CBLKSTY_LAZY)) ? T1_TYPE_RAW : T1_TYPE_MQ;
				if (nType == T1_TYPE_RAW)
					MQC_BypassInitEncoder(pMQCoder);
				else
					MQC_RestartInitEncoder(pMQCoder);
			}

			pPass->dDistortionDec = dCumWMSEDec;
			pPass->nRate = MQC_BytesCount(pMQCoder) + nCorrection;
			pPass->nLen = pPass->nRate - (nPassIndex == 0 ? 0 : pCodeBlock->aPasses[nPassIndex - 1].nRate);

			// Code-switch "RESET"
			if (nCodeBlockStyle & J2K_CCP_CBLKSTY_RESET)
				MQC_ResetEncoder(pMQCoder);
		}

		// Code switch "ERTERM" (PTERM)
		if (nCodeBlockStyle & J2K_CCP_CBLKSTY_PTERM)
			MQC_ErtermEncoder(pMQCoder);
		else // Default coding
			if (!(nCodeBlockStyle & J2K_CCP_CBLKSTY_LAZY))
				MQC_Flush(pMQCoder);

		pCodeBlock->nTotalPassesCount = nPassIndex;
	}

	static void   Tier1_DecodeCodeBlock(Tier1 *pTier1, CodeBlock *pCodeBlock, int nOrient, int nROIShift, int nCodeBlockStyle)
	{
		char nType = T1_TYPE_MQ;

		Raw *pRaw = pTier1->pRaw;
		MQCoder *pMQCoder = pTier1->pMQCoder;

		int nWidth  = pCodeBlock->nX1 - pCodeBlock->nX0;
		int nHeight = pCodeBlock->nY1 - pCodeBlock->nY0;

		for (int nY = 0; nY <= nHeight; nY++)
		{
			for (int nX = 0; nX <= nWidth; nX++)
			{
				pTier1->aFlags[nY][nX] = 0;
			}
		}

		for (int nX = 0; nX < nWidth; nX++)
		{
			for (int nY = 0; nY < nHeight; nY++)
			{
				pTier1->aData[nY][nX] = 0;
			}
		}

		int nBufferPos = nROIShift + pCodeBlock->nBPSCount - 1;
		int nPassType = 2;

		MQC_ResetStates(pMQCoder);
		MQC_SetState(pMQCoder, T1_CTXNO_UNI, 0, 46);
		MQC_SetState(pMQCoder, T1_CTXNO_AGG, 0, 3);
		MQC_SetState(pMQCoder, T1_CTXNO_ZC, 0, 4);

		for (int nSegIndex = 0; nSegIndex < pCodeBlock->nSegmentsCount; nSegIndex++)
		{
			TCDSegment *pSegment = &pCodeBlock->aSegments[nSegIndex];

			nType = ((nBufferPos <= (pCodeBlock->nBPSCount - 1) - 4) && (nPassType < 2) && (nCodeBlockStyle & J2K_CCP_CBLKSTY_LAZY)) ? T1_TYPE_RAW : T1_TYPE_MQ;
			if (nType == T1_TYPE_RAW)
			{
				Raw_InitDecoder(pRaw, pSegment->pData, pSegment->nLength);
			}
			else
			{
				MQC_InitDecoder(pMQCoder, pSegment->pData, pSegment->nLength);
			}

			for (int nPassIndex = 0; nPassIndex < pSegment->nPassesCount; nPassIndex++)
			{
				switch (nPassType)
				{
					case 0:
					Tier1_DecoderSigPass(pTier1, nWidth, nHeight, nBufferPos + 1, nOrient, nType, nCodeBlockStyle);
					break;
					case 1:
					Tier1_DecoderRefPass(pTier1, nWidth, nHeight, nBufferPos + 1, nType, nCodeBlockStyle);
					break;
					case 2:
					Tier1_DecoderClnPass(pTier1, nWidth, nHeight, nBufferPos + 1, nOrient, nCodeBlockStyle);
					break;
				}

				if ((nCodeBlockStyle & J2K_CCP_CBLKSTY_RESET) && nType == T1_TYPE_MQ)
				{
					MQC_ResetStates(pMQCoder);
					MQC_SetState(pMQCoder, T1_CTXNO_UNI, 0, 46);
					MQC_SetState(pMQCoder, T1_CTXNO_AGG, 0, 3);
					MQC_SetState(pMQCoder, T1_CTXNO_ZC, 0, 4);
				}
				if (++nPassType == 3)
				{
					nPassType = 0;
					nBufferPos--;
				}
			}
		}
	}

	static int    Tier1_InitContextIndexZC(int nF, int nOrient)
	{
		int nHor = ((nF & T1_SIG_W) != 0) + ((nF & T1_SIG_E) != 0);
		int nVer = ((nF & T1_SIG_N) != 0) + ((nF & T1_SIG_S) != 0);
		int nDia = ((nF & T1_SIG_NW) != 0) + ((nF & T1_SIG_NE) != 0) + ((nF & T1_SIG_SE) != 0) + ((nF & T1_SIG_SW) != 0);

		int nHorVer, nTemp;
		int nLen = 0;

		switch (nOrient)
		{
			case 2:
			nTemp = nHor;
			nHor = nVer;
			nVer = nTemp;
			case 0:
			case 1:
			if (!nHor)
			{
				if (!nVer)
				{
					if (!nDia)
						nLen = 0;
					else if (1 == nDia)
						nLen = 1;
					else
						nLen = 2;
				}
				else if (1 == nVer)
				{
					nLen = 3;
				}
				else
				{
					nLen = 4;
				}
			}
			else if (1 == nHor)
			{
				if (!nVer)
				{
					if (!nDia)
						nLen = 5;
					else
						nLen = 6;
				}
				else
				{
					nLen = 7;
				}
			}
			else
				nLen = 8;
			break;
			case 3:
			nHorVer = nHor + nVer;
			if (!nDia)
			{
				if (!nHorVer)
				{
					nLen = 0;
				}
				else if (1 == nHorVer)
				{
					nLen = 1;
				}
				else
				{
					nLen = 2;
				}
			}
			else if (1 == nDia)
			{
				if (!nHorVer)
				{
					nLen = 3;
				}
				else if (1 == nHorVer)
				{
					nLen = 4;
				}
				else
				{
					nLen = 5;
				}
			}
			else if (2 == nDia)
			{
				if (!nHorVer)
				{
					nLen = 6;
				}
				else
				{
					nLen = 7;
				}
			}
			else
			{
				nLen = 8;
			}
			break;
		}

		return (T1_CTXNO_ZC + nLen);
	}

	static int    Tier1_InitContextIndexSC(int nF)
	{
		int nLen = 0;

        int nHorC = (std::min)(((nF & (T1_SIG_E | T1_SGN_E)) == T1_SIG_E) + ((nF & (T1_SIG_W | T1_SGN_W)) == T1_SIG_W), 1) - (std::min)(((nF & (T1_SIG_E | T1_SGN_E)) == (T1_SIG_E | T1_SGN_E)) + ((nF & (T1_SIG_W | T1_SGN_W)) == (T1_SIG_W | T1_SGN_W)), 1);
        int nVerC = (std::min)(((nF & (T1_SIG_N | T1_SGN_N)) == T1_SIG_N) + ((nF & (T1_SIG_S | T1_SGN_S)) == T1_SIG_S), 1) - (std::min)(((nF & (T1_SIG_N | T1_SGN_N)) == (T1_SIG_N | T1_SGN_N)) + ((nF & (T1_SIG_S | T1_SGN_S)) == (T1_SIG_S | T1_SGN_S)), 1);

		if (nHorC < 0)
		{
			nHorC = -nHorC;
			nVerC = -nVerC;
		}
		if (!nHorC)
		{
			if (1 == nVerC)
				nLen = 1;
			else if (!nVerC)
				nLen = 0;
			else
				nLen = 1;
		}
		else if (1 == nHorC)
		{
			if (-1 == nVerC)
				nLen = 2;
			else if (!nVerC)
				nLen = 3;
			else
				nLen = 4;
		}

		return (T1_CTXNO_SC + nLen);
	}

	static int    Tier1_InitContextIndexMag(int nF)
	{
		int nLen = 0;

		if (!(nF & T1_REFINE))
			nLen = (nF & (T1_SIG_OTH)) ? 1 : 0;
		else
			nLen = 2;

		return (T1_CTXNO_MAG + nLen);
	}

	static int    Tier1_InitSPB(int nF)
	{
		int nLen = 0;

        int nHorC = (std::min)(((nF & (T1_SIG_E | T1_SGN_E)) == T1_SIG_E) + ((nF & (T1_SIG_W | T1_SGN_W)) == T1_SIG_W), 1) - (std::min)(((nF & (T1_SIG_E | T1_SGN_E)) == (T1_SIG_E | T1_SGN_E)) + ((nF & (T1_SIG_W | T1_SGN_W)) == (T1_SIG_W | T1_SGN_W)), 1);
        int nVerC = (std::min)(((nF & (T1_SIG_N | T1_SGN_N)) == T1_SIG_N) + ((nF & (T1_SIG_S | T1_SGN_S)) == T1_SIG_S), 1) - (std::min)(((nF & (T1_SIG_N | T1_SGN_N)) == (T1_SIG_N | T1_SGN_N)) + ((nF & (T1_SIG_S | T1_SGN_S)) == (T1_SIG_S | T1_SGN_S)), 1);

		if (!nHorC && !nVerC)
			nLen = 0;
		else
			nLen = (!(nHorC > 0 || (!nHorC && nVerC > 0)));

		return nLen;
	}

	static void   Tier1_InitLuts(Tier1 *pTier1)
	{
		for (int nJ = 0; nJ < 4; nJ++)
		{
			for (int nI = 0; nI < 256; ++nI)
			{
				pTier1->lut_ctxno_zc[(nJ << 8) | nI] = Tier1_InitContextIndexZC(nI, nJ);
			}
		}
		for (int nI = 0; nI < 256; nI++)
		{
			pTier1->lut_ctxno_sc[nI] = Tier1_InitContextIndexSC(nI << 4);
		}
		for (int nJ = 0; nJ < 2; nJ++)
		{
			for (int nI = 0; nI < 2048; ++nI)
			{
				pTier1->lut_ctxno_mag[(nJ << 11) + nI] = Tier1_InitContextIndexMag((nJ ? T1_REFINE : 0) | nI);
			}
		}
		for (int nI = 0; nI < 256; ++nI)
		{
			pTier1->lut_spb[nI] = Tier1_InitSPB(nI << 4);
		}

		for (int nI = 0; nI < (1 << T1_NMSEDEC_BITS); nI++)
		{
			double dT = nI / pow(double(2), T1_NMSEDEC_FRACBITS);
			double dU = dT;
			double dV = dT - 1.5;
            pTier1->lut_nmsedec_sig[nI]  = (std::max)(0, (int)(floor((dU * dU - dV * dV) * pow(double(2), T1_NMSEDEC_FRACBITS) + 0.5) / pow(double(2), T1_NMSEDEC_FRACBITS) * 8192.0));
            pTier1->lut_nmsedec_sig0[nI] = (std::max)(0, (int)(floor((dU * dU)           * pow(double(2), T1_NMSEDEC_FRACBITS) + 0.5) / pow(double(2), T1_NMSEDEC_FRACBITS) * 8192.0));
			dU = dT - 1.0;
			if (nI & (1 << (T1_NMSEDEC_BITS - 1)))
			{
				dV = dT - 1.5;
			}
			else
			{
				dV = dT - 0.5;
			}
            pTier1->lut_nmsedec_ref[nI]  = (std::max)(0, (int)(floor((dU * dU - dV * dV) * pow(double(2), T1_NMSEDEC_FRACBITS) + 0.5) / pow(double(2), T1_NMSEDEC_FRACBITS) * 8192.0));
            pTier1->lut_nmsedec_ref0[nI] = (std::max)(0, (int)(floor((dU * dU)           * pow(double(2), T1_NMSEDEC_FRACBITS) + 0.5) / pow(double(2), T1_NMSEDEC_FRACBITS) * 8192.0));
		}
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	// Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	Tier1* Tier1_Create(PCommon pCodecInfo)
	{
		Tier1 *pTier1 = (Tier1*)Malloc(sizeof(Tier1));
		if (!pTier1)
		{
			pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return NULL;
		}
		else
		{
			pTier1->pCodecInfo = pCodecInfo;
			pTier1->pMQCoder   = MQC_Create();
			if (!pTier1->pMQCoder)
			{
				pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				Free(pTier1);
				return NULL;
			}
			pTier1->pRaw       = Raw_Create();
			if (!pTier1->pRaw)
			{
				pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				Free(pTier1->pMQCoder);
				Free(pTier1);
				return NULL;
			}

			// Инициализируем таблицы 
			Tier1_InitLuts(pTier1);
		}
		return pTier1;
	}

	void   Tier1_Destroy(Tier1 *pTier1)
	{
		if (pTier1)
		{
			MQC_Destroy(pTier1->pMQCoder);
			Raw_Destroy(pTier1->pRaw);
			Free(pTier1);
		}
	}

	void   Tier1_EncodeCodeBlocks(Tier1 *pTier1, Tile *pTile, TileCodingParams *pTCP)
	{
		pTile->dDistoTile = 0; // Fixed quality

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];

			for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
			{
				Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

				for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
				{
					Band *pBand = &pResolution->aBands[nBandIndex];

					for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
					{
						Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

						for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
						{
							CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];

							int nX = pCodeBlock->nX0 - pBand->nX0;
							int nY = pCodeBlock->nY0 - pBand->nY0;
							if (pBand->nBandIndex & 1)
							{
								Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex - 1];
								nX += pResolution->nX1 - pResolution->nX0;
							}
							if (pBand->nBandIndex & 2)
							{
								Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex - 1];
								nY += pResolution->nY1 - pResolution->nY0;
							}

							int nWidth = pTileComp->nX1 - pTileComp->nX0;
							if (pTCP->pTCCP[nComponentIndex].nTransformID == 1)
							{
								for (int nYIndex = 0; nYIndex < pCodeBlock->nY1 - pCodeBlock->nY0; nYIndex++)
								{
									for (int nXIndex = 0; nXIndex < pCodeBlock->nX1 - pCodeBlock->nX0; nXIndex++)
									{
										pTier1->aData[nYIndex][nXIndex] = pTileComp->pData[(nX + nXIndex) + (nY + nYIndex) * nWidth] << T1_NMSEDEC_FRACBITS;
									}
								}
							}
							else
							{
								// if ( pTCP->pTCCPs[nComponentIndex].qmfbid == 0 )
								for (int nYIndex = 0; nYIndex < pCodeBlock->nY1 - pCodeBlock->nY0; nYIndex++)
								{
									for (int nXIndex = 0; nXIndex < pCodeBlock->nX1 - pCodeBlock->nX0; nXIndex++)
									{
										pTier1->aData[nYIndex][nXIndex] = FixedMult(pTileComp->pData[nX + nXIndex + (nY + nYIndex) * nWidth], 8192 * 8192 / ((int)floor(pBand->fStepSize * 8192))) >> (13 - T1_NMSEDEC_FRACBITS);
									}
								}
							}

							int nOrient = pBand->nBandIndex;
							if (2 == nOrient)
							{
								nOrient = 1;
							}
							else if (1 == nOrient)
							{
								nOrient = 2;
							}

							Tier1_EncodeCodeBlock(pTier1, pCodeBlock, nOrient, nComponentIndex, pTileComp->nResolutionsCount - 1 - nResolutionIndex, pTCP->pTCCP[nComponentIndex].nTransformID, pBand->fStepSize, pTCP->pTCCP[nComponentIndex].nCodeBlockStyle, pTile->nComponentsCount, pTile);
						}
					}
				}
			}
		}
	}

	void   Tier1_DecodeCodeBlocks(Tier1 *pTier1, Tile *pTile, TileCodingParams *pTCP)
	{
		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];

			for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
			{
				Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

				for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
				{
					Band *pBand = &pResolution->aBands[nBandIndex];

					for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
					{
						Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

						for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
						{
							CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];

							int nOrient = pBand->nBandIndex;
							if (2 == nOrient)
							{
								nOrient = 1;
							}
							else if (1 == nOrient)
							{
								nOrient = 2;
							}

							try
							{
								Tier1_DecodeCodeBlock(pTier1, pCodeBlock, nOrient, pTCP->pTCCP[nComponentIndex].nROIShift, pTCP->pTCCP[nComponentIndex].nCodeBlockStyle);
							}
							catch (...)
							{
								break;
							}
							int nX = pCodeBlock->nX0 - pBand->nX0;
							int nY = pCodeBlock->nY0 - pBand->nY0;

							if (pBand->nBandIndex & 1)
							{
								Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex - 1];
								nX += pResolution->nX1 - pResolution->nX0;
							}
							if (pBand->nBandIndex & 2)
							{
								Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex - 1];
								nY += pResolution->nY1 - pResolution->nY0;
							}

							if (pTCP->pTCCP[nComponentIndex].nROIShift)
							{
								int nTreshold = 1 << pTCP->pTCCP[nComponentIndex].nROIShift;
								for (int nYIndex = 0; nYIndex < pCodeBlock->nY1 - pCodeBlock->nY0; nYIndex++)
								{
									for (int nXIndex = 0; nXIndex < pCodeBlock->nX1 - pCodeBlock->nX0; nXIndex++)
									{
										int nValue = pTier1->aData[nYIndex][nXIndex];
										int nMagnitude = abs(nValue);
										if (nMagnitude >= nTreshold)
										{
											nMagnitude >>= pTCP->pTCCP[nComponentIndex].nROIShift;
											pTier1->aData[nYIndex][nXIndex] = nValue < 0 ? -nMagnitude : nMagnitude;
										}
									}
								}
							}

							int nWidth = pTileComp->nX1 - pTileComp->nX0;
							if (pTCP->pTCCP[nComponentIndex].nTransformID == 1)
							{
								for (int nYIndex = 0; nYIndex < pCodeBlock->nY1 - pCodeBlock->nY0; nYIndex++)
								{
									for (int nXIndex = 0; nXIndex < pCodeBlock->nX1 - pCodeBlock->nX0; nXIndex++)
									{
										pTileComp->pData[nX + nXIndex + (nY + nYIndex) * nWidth] = pTier1->aData[nYIndex][nXIndex] / 2;
									}
								}
							}
							else
							{
								// if ( pTCP->pTCCPs[nComponentIndex].qmfbid == 0 )
								for (int nYIndex = 0; nYIndex < pCodeBlock->nY1 - pCodeBlock->nY0; nYIndex++)
								{
									for (int nXIndex = 0; nXIndex < pCodeBlock->nX1 - pCodeBlock->nX0; nXIndex++)
									{
										if (pTier1->aData[nYIndex][nXIndex] >> 1 == 0)
										{
											pTileComp->pData[nX + nXIndex + (nY + nYIndex) * nWidth] = 0;
										}
										else
										{
											double dTemp  = (double)((pTier1->aData[nYIndex][nXIndex] << 12) * pBand->fStepSize);
											int    nTemp2 = ((int)(floor(fabs(dTemp)))) + ((int)floor(fabs(dTemp * 2)) % 2);
											pTileComp->pData[nX + nXIndex + (nY + nYIndex) * nWidth] = ((dTemp < 0) ? -nTemp2 : nTemp2);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

#pragma once

#include "Reader.h"
#include "PacketIterator.h"
#include "TagTree.h"
#include "Stream.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	static void Tier2_PutCommaCode(BitIO *pBIO, int nLen)
	{
		while (--nLen >= 0)
		{
			BitIO_Write(pBIO, 1, 1);
		}
		BitIO_Write(pBIO, 0, 1);
	}

	static int  Tier2_GetCommaCode(BitIO *pBIO)
	{
		int nLen;
		for (nLen = 0; BitIO_Read(pBIO, 1); nLen++);

		return nLen;
	}

	static void Tier2_PutPassesCount(BitIO *pBIO, int nLen)
	{
		if (1 == nLen)
		{
			BitIO_Write(pBIO, 0, 1);
		}
		else if (2 == nLen)
		{
			BitIO_Write(pBIO, 2, 2);
		}
		else if (nLen <= 5)
		{
			BitIO_Write(pBIO, 0xc | (nLen - 3), 4);
		}
		else if (nLen <= 36)
		{
			BitIO_Write(pBIO, 0x1e0 | (nLen - 6), 9);
		}
		else if (nLen <= 164)
		{
			BitIO_Write(pBIO, 0xff80 | (nLen - 37), 16);
		}
	}

	static int  Tier2_GetPassesCount(BitIO *pBitStream)
	{
		int nLen = 0;
		if (!BitIO_Read(pBitStream, 1))
			return 1;
		if (!BitIO_Read(pBitStream, 1))
			return 2;
		if ((nLen = BitIO_Read(pBitStream, 2)) != 3)
			return (3 + nLen);
		if ((nLen = BitIO_Read(pBitStream, 5)) != 31)
			return (6 + nLen);
		return (37 + BitIO_Read(pBitStream, 7));
	}

	static int  Tier2_EncodePacket(Tile *pTile, TileCodingParams *pTCP, PacketIterator *pPI, unsigned char *pDst, int nLength, ImageInfo * pImageInfo, int nTileIndex)
	{
		unsigned char *pDstPointer = pDst;

		int nComponentIndex  = pPI->nIndexComponent;
		int nResolutionIndex = pPI->nIndexResolution;
		int nPrecinctIndex   = pPI->nIndexPrecinct;
		int nLayerIndex      = pPI->nIndexLayer;

		TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
		Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

		// <SOP 0xff91>
		if (pTCP->nCodingStyle & J2K_CP_CSTY_SOP)
		{
			unsigned char *pSOP = (unsigned char *)Malloc(6 * sizeof(unsigned char));
			pSOP[0] = 255; // ff
			pSOP[1] = 145; // 91
			pSOP[2] = 0;   // Длина всегда 4 байта
			pSOP[3] = 4;   // 
			pSOP[4] = (pImageInfo->nPacketCount % 65536) / 256;
			pSOP[5] = (pImageInfo->nPacketCount % 65536) % 256;
			memcpy(pDstPointer, pSOP, 6);
			Free(pSOP);
			pDstPointer += 6;
		}
		// </SOP>

		if (!nLayerIndex)
		{
			for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
			{
				Band *pBand = &pResolution->aBands[nBandIndex];
				Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
				TGT_Reset(pPrecinct->pInclTree);
				TGT_Reset(pPrecinct->pIMSBTree);
				for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
				{
					CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
					pCodeBlock->nPassesCount = 0;
					TGT_SetValue(pPrecinct->pIMSBTree, nCodeBlockIndex, pBand->nBPSCount - pCodeBlock->nBPSCount);
				}
			}
		}

		BitIO *pBitStream = BitIO_Create();
		if (!pBitStream)
			return -999;
		BitIO_InitEncoder(pBitStream, pDstPointer, nLength);
		BitIO_Write(pBitStream, 1, 1); // Empty header bit

		// Writing Packet header
		for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
		{
			Band *pBand = &pResolution->aBands[nBandIndex];
			Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
			for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
			{
				CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
				Layer *pLayer = &pCodeBlock->aLayers[nLayerIndex];
				if (!pCodeBlock->nPassesCount && pLayer->nPassesCount)
				{
					TGT_SetValue(pPrecinct->pInclTree, nCodeBlockIndex, nLayerIndex);
				}
			}
			for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
			{
				CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
				Layer *pLayer = &pCodeBlock->aLayers[nLayerIndex];
				int nIncrement = 0;
				int nPassesCount = 0;
				int nLen = 0;

				// CodeBlock Inclusion bits
				if (!pCodeBlock->nPassesCount)
				{
					TGT_Encode(pBitStream, pPrecinct->pInclTree, nCodeBlockIndex, nLayerIndex + 1);
				}
				else
				{
					BitIO_Write(pBitStream, pLayer->nPassesCount != 0, 1);
				}

				// Если CodeBlock не включен, тогда переходми к следующему CodeBlock
				if (!pLayer->nPassesCount)
				{
					continue;
				}

				// Если это первое появление CodeBlock --> тогда обрабытываем информацию о Zero bit-planes
				if (!pCodeBlock->nPassesCount)
				{
					pCodeBlock->nLenBitsCount = 3;
					TGT_Encode(pBitStream, pPrecinct->pIMSBTree, nCodeBlockIndex, 999);
				}

				Tier2_PutPassesCount(pBitStream, pLayer->nPassesCount);

				for (int nPassIndex = pCodeBlock->nPassesCount; nPassIndex < pCodeBlock->nPassesCount + pLayer->nPassesCount; nPassIndex++)
				{
					TCDPass *pPass = &pCodeBlock->aPasses[nPassIndex];
					nPassesCount++;
					nLen += pPass->nLen;
					if (pPass->nTerm || nPassIndex == (pCodeBlock->nPassesCount + pLayer->nPassesCount) - 1)
					{
                        nIncrement = (std::max)(nIncrement, FloorLog2(nLen) + 1 - (pCodeBlock->nLenBitsCount + FloorLog2(nPassesCount)));
						nLen = 0;
						nPassesCount = 0;
					}
				}
				Tier2_PutCommaCode(pBitStream, nIncrement);

				pCodeBlock->nLenBitsCount += nIncrement;

				// Codeword segment lenght
				for (int nPassIndex = pCodeBlock->nPassesCount; nPassIndex < pCodeBlock->nPassesCount + pLayer->nPassesCount; nPassIndex++)
				{
					TCDPass *pPass = &pCodeBlock->aPasses[nPassIndex];
					nPassesCount++;
					nLen += pPass->nLen;
					if (pPass->nTerm || nPassIndex == (pCodeBlock->nPassesCount + pLayer->nPassesCount) - 1)
					{
						BitIO_Write(pBitStream, nLen, pCodeBlock->nLenBitsCount + FloorLog2(nPassesCount));
						nLen = 0;
						nPassesCount = 0;
					}
				}
			}
		}

		if (BitIO_Flush(pBitStream))
		{
			return -999; //  to eliminate longjmp!!
		}

		pDstPointer += BitIO_WrittenBytesCount(pBitStream);

		BitIO_Destroy(pBitStream);

		// <EPH 0xff92>
		if (pTCP->nCodingStyle & J2K_CP_CSTY_EPH)
		{
			unsigned char *pEPH = (unsigned char *)Malloc(2 * sizeof(unsigned char));
			pEPH[0] = 255;
			pEPH[1] = 146;
			memcpy(pDstPointer, pEPH, 2);
			Free(pEPH);
			pDstPointer += 2;
		}
		// </EPH>

		// Writing the packet body

		for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
		{
			Band *pBand = &pResolution->aBands[nBandIndex];
			Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
			for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
			{
				CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
				Layer *pLayer = &pCodeBlock->aLayers[nLayerIndex];
				if (!pLayer->nPassesCount)
				{
					continue;
				}
				if (pDstPointer + pLayer->nLen > pDst + nLength)
				{
					return -999;
				}

				memcpy(pDstPointer, pLayer->pData, pLayer->nLen);
				pCodeBlock->nPassesCount += pLayer->nPassesCount;
				pDstPointer += pLayer->nLen;

				//
				if (pImageInfo && pImageInfo->nIndexWrite && pImageInfo->nIndexOn)
				{
					TileInfo *pTileInfo = &pImageInfo->pTile[nTileIndex];
					PacketInfo *pPacketInfo = &pTileInfo->pPacket[pImageInfo->nPacketCount];
					pPacketInfo->dDisto += pLayer->dDisto;
					if (pImageInfo->dDistoMax < pPacketInfo->dDisto)
					{
						pImageInfo->dDistoMax = pPacketInfo->dDisto;
					}
				}
				//
			}
		}

		return (pDstPointer - pDst);
	}

	static void Tier2_InitSegment(TCDSegment *pSegment, int nCodeBlockStyle, int nFirst)
	{
		pSegment->nPassesCount = 0;
		pSegment->nLength      = 0;

		if (nCodeBlockStyle & J2K_CCP_CBLKSTY_TERMALL)
		{
			pSegment->nMaxPasses = 1;
		}
		else if (nCodeBlockStyle & J2K_CCP_CBLKSTY_LAZY)
		{
			if (nFirst)
			{
				pSegment->nMaxPasses = 10;
			}
			else
			{
				pSegment->nMaxPasses = (((pSegment - 1)->nMaxPasses == 1) || ((pSegment - 1)->nMaxPasses == 10)) ? 2 : 1;
			}
		}
		else
		{
			pSegment->nMaxPasses = 109;
		}
	}

	static int  Tier2_DecodePacket(Tier2* pTier2, unsigned char *pSrc, int nLen, Tile *pTile, TileCodingParams *pTCP, PacketIterator *pPI)
	{
		unsigned char *pSrcPointer = pSrc;

		CodingParams *pCodingParams = pTier2->pCodingParams;

		int nComponentIndex  = pPI->nIndexComponent;
		int nResolutionIndex = pPI->nIndexResolution;
		int nPrecinctIndex   = pPI->nIndexPrecinct;
		int nLayerIndex      = pPI->nIndexLayer;

		TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
		Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

		unsigned char *pBuffer = NULL;

		if (0 == nLayerIndex)
		{
			for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
			{
				Band *pBand = &pResolution->aBands[nBandIndex];
				Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

				if ((pBand->nX1 - pBand->nX0 == 0) || (pBand->nY1 - pBand->nY0 == 0))
					continue;

				TGT_Reset(pPrecinct->pInclTree);
				TGT_Reset(pPrecinct->pIMSBTree);
				for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
				{
					CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
					pCodeBlock->nSegmentsCount = 0;
				}
			}
		}

		// SOP маркер
		if (pTCP->nCodingStyle & J2K_CP_CSTY_SOP)
		{
			if ((*pSrcPointer) != 0xff || (*(pSrcPointer + 1) != 0x91))
			{
				Event_Message(EVT_WARNING, "Expected SOP marker\n");
			}
			else
			{
				pSrcPointer += 6;
			}

			// TO DO : Добавить проверку значения Nsop
		}

		// Когда используются пакеты PPT/PPM, Packet header хранится в маркерах PPT/PPM.

		BitIO *pBitStream = BitIO_Create();
		if (!pBitStream)
			return -999;

		if (1 == pCodingParams->nPPM) // PPM
		{
			pBuffer = pCodingParams->pPPMData;
			BitIO_InitDecoder(pBitStream, pBuffer, pCodingParams->nPPMLength);
		}
		else if (1 == pTCP->nPPT) // PPT
		{
			pBuffer = pTCP->pPPTData;
			BitIO_InitDecoder(pBitStream, pBuffer, pTCP->nPPTLength);
		}
		else // Обычный случай
		{
			pBuffer = pSrcPointer;
			BitIO_InitDecoder(pBitStream, pBuffer, pSrc + nLen - pBuffer);
		}

		int nPresent = BitIO_Read(pBitStream, 1);

		if (!nPresent)
		{
			BitIO_InAlign(pBitStream);
			pBuffer += BitIO_WrittenBytesCount(pBitStream);
			BitIO_Destroy(pBitStream);

			// EPH маркер
			if (pTCP->nCodingStyle & J2K_CP_CSTY_EPH)
			{
				if ((*pBuffer) != 0xff || (*(pBuffer + 1) != 0x92))
				{
					// TO DO: Error: expected EPH marker
				}
				else
				{
					pBuffer += 2;
				}
			}

			if (1 == pCodingParams->nPPM) // PPM
			{
				pCodingParams->nPPMLength += pCodingParams->pPPMData - pBuffer;
				pCodingParams->pPPMData    = pBuffer;
				return (pSrcPointer - pSrc);
			}
			if (1 == pTCP->nPPT) // PPT
			{
				pTCP->nPPTLength += pTCP->pPPTData - pBuffer;
				pTCP->pPPTData    = pBuffer;
				return (pSrcPointer - pSrc);
			}

			return (pBuffer - pSrc);
		}

		for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
		{
			Band *pBand = &pResolution->aBands[nBandIndex];
			Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

			if ((pBand->nX1 - pBand->nX0 == 0) || (pBand->nY1 - pBand->nY0 == 0))
				continue;

			for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
			{
				int nIncluded = 0;
				CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
				TCDSegment *pSegment = NULL;

				// Если CodeBlock не был включен ранее --> TagTree
				if (!pCodeBlock->nSegmentsCount)
				{
					nIncluded = TGT_Decode(pBitStream, pPrecinct->pInclTree, nCodeBlockIndex, nLayerIndex + 1);
				}
				else
				{
					nIncluded = BitIO_Read(pBitStream, 1);
				}

				if (!nIncluded)
				{
					pCodeBlock->nNewPassesCount = 0;
					continue;
				}

				// Если CodeBlock не был включен ранее --> zero-bitplane tagtree
				if (!pCodeBlock->nSegmentsCount)
				{
					int nIndex;
					for (nIndex = 0; !TGT_Decode(pBitStream, pPrecinct->pIMSBTree, nCodeBlockIndex, nIndex); nIndex++);
					int nImsbsCount = nIndex - 1;

					pCodeBlock->nBPSCount = pBand->nBPSCount - nImsbsCount;
					pCodeBlock->nLenBitsCount = 3;
				}

				pCodeBlock->nNewPassesCount = Tier2_GetPassesCount(pBitStream);
				int nIncrement = Tier2_GetCommaCode(pBitStream);
				pCodeBlock->nLenBitsCount += nIncrement;

				if (!pCodeBlock->nSegmentsCount)
				{
					pSegment = &pCodeBlock->aSegments[0];
					Tier2_InitSegment(pSegment, pTCP->pTCCP[nComponentIndex].nCodeBlockStyle, 1);
				}
				else
				{
					pSegment = &pCodeBlock->aSegments[pCodeBlock->nSegmentsCount - 1];
					if (pSegment->nPassesCount == pSegment->nMaxPasses)
					{
						Tier2_InitSegment(++pSegment, pTCP->pTCCP[nComponentIndex].nCodeBlockStyle, 0);
					}
				}
				int nCount = pCodeBlock->nNewPassesCount;

				do
				{
                    pSegment->nNewPassesCount = (std::min)(pSegment->nMaxPasses - pSegment->nPassesCount, nCount);
					pSegment->nNewLength   = BitIO_Read(pBitStream, pCodeBlock->nLenBitsCount + FloorLog2(pSegment->nNewPassesCount));
					nCount -= pSegment->nNewPassesCount;
					if (nCount > 0)
					{
						Tier2_InitSegment(++pSegment, pTCP->pTCCP[nComponentIndex].nCodeBlockStyle, 0);
					}
				} while (nCount > 0);
			}
		}

		if (BitIO_InAlign(pBitStream))
		{
			BitIO_Destroy(pBitStream);
			return -999;
		}

		pBuffer += BitIO_WrittenBytesCount(pBitStream);
		BitIO_Destroy(pBitStream);

		// EPH маркер
		if (pTCP->nCodingStyle & J2K_CP_CSTY_EPH)
		{
			if ((*pBuffer) != 0xff || (*(pBuffer + 1) != 0x92))
			{
				Event_Message(EVT_ERROR, "Expected EPH marker\n");
			}
			else
			{
				pBuffer += 2;
			}
		}

		if (1 == pCodingParams->nPPM) // PPM
		{
			pCodingParams->nPPMLength += pCodingParams->pPPMData - pBuffer;
			pCodingParams->pPPMData    = pBuffer;
		}
		else if (1 == pTCP->nPPT) // PPT
		{
			pTCP->nPPTLength += pTCP->pPPTData - pBuffer;
			pTCP->pPPTData    = pBuffer;
		}
		else
		{
			pSrcPointer = pBuffer;
		}

		for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
		{
			Band *pBand = &pResolution->aBands[nBandIndex];
			Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

			if ((pBand->nX1 - pBand->nX0 == 0) || (pBand->nY1 - pBand->nY0 == 0))
				continue;

			for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
			{
				CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];

				if (pCodeBlock->nSegmentsCount < 0)break;

				TCDSegment *pSegment = NULL;
				if (!pCodeBlock->nNewPassesCount)
					continue;
				if (!pCodeBlock->nSegmentsCount)
				{
					pSegment = &pCodeBlock->aSegments[0];
					pCodeBlock->nSegmentsCount++;
					pCodeBlock->nLen = 0;
				}
				else
				{
					pSegment = &pCodeBlock->aSegments[pCodeBlock->nSegmentsCount - 1];
					if (pSegment->nPassesCount == pSegment->nMaxPasses)
					{
						pSegment++;
						pCodeBlock->nSegmentsCount++;
					}
				}

				do
				{
					if (pSrcPointer + pSegment->nNewLength > pSrc + nLen)
					{
						return -999;
					}

					memcpy(pCodeBlock->aData + pCodeBlock->nLen, pSrcPointer, pSegment->nNewLength);
					if (pSegment->nPassesCount == 0)
					{
						pSegment->pData = pCodeBlock->aData + pCodeBlock->nLen;
					}
					pSrcPointer += pSegment->nNewLength;
					pCodeBlock->nLen += pSegment->nNewLength;
					pSegment->nLength += pSegment->nNewLength;
					pSegment->nPassesCount += pSegment->nNewPassesCount;
					pCodeBlock->nNewPassesCount -= pSegment->nNewPassesCount;
					if (pCodeBlock->nNewPassesCount > 0)
					{
						pSegment++;
						pCodeBlock->nSegmentsCount++;
					}
				} while (pCodeBlock->nNewPassesCount > 0);
			}
		}

		return (pSrcPointer - pSrc);
	}



	//-------------------------------------------------------------------------------------------------------------------------------
	// Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	int    Tier2_EncodePackets(Tier2 *pTier2, int nTileIndex, Tile *pTile, int nMaxLayers, unsigned char *pDst, int nLen, ImageInfo *pImageInfo)
	{
		unsigned char *pDstPointer = pDst;
		int nShift = 0;

		Image *pImage = pTier2->pImage;
		CodingParams *pCodingParams = pTier2->pCodingParams;
		PacketIterator *pPI = PI_Create(pImage, pCodingParams, nTileIndex);
		if (!pPI)
		{
			// TO DO: Throw an error
			return -999;
		}

		if (pImageInfo)
		{
			pImageInfo->nPacketCount = 0;
		}

		for (int nIndexPI = 0; nIndexPI <= pCodingParams->pTCP[nTileIndex].nPOCsCount; nIndexPI++)
		{
			while (PI_Next(&pPI[nIndexPI]))
			{
				if (pPI[nIndexPI].nIndexLayer < nMaxLayers)
				{
					nShift = Tier2_EncodePacket(pTile, &pCodingParams->pTCP[nTileIndex], &pPI[nIndexPI], pDstPointer, pDst + nLen - pDstPointer, pImageInfo, nTileIndex);
					if (nShift == -999)
					{
						break;
					}
					else
					{
						pDstPointer += nShift;
					}

					// Индексация
					if (pImageInfo && pImageInfo->nIndexOn)
					{
						if (pImageInfo->nIndexWrite)
						{
							TileInfo *pTileInfo = &pImageInfo->pTile[nTileIndex];
							PacketInfo *pPacketInfo = &pTileInfo->pPacket[pImageInfo->nPacketCount];
							if (!pImageInfo->nPacketCount)
							{
								pPacketInfo->nStartPos = pTileInfo->nEndHeader + 1;
							}
							else
							{
								pPacketInfo->nStartPos = pTileInfo->pPacket[pImageInfo->nPacketCount - 1].nEndPos + 1;
							}
							pPacketInfo->nEndPos = pPacketInfo->nStartPos + nShift - 1;
						}

						pImageInfo->nPacketCount++;
					}
					//
				}
			}
		}

		PI_Destroy(pPI, pCodingParams, nTileIndex);

		if (nShift == -999)
		{
			return nShift;
		}

		return (pDstPointer - pDst);
	}

	int    Tier2_DecodePackets(Tier2 *pTier2, unsigned char *pSrc, int nLen, int nTileIndex, Tile *pTile)
	{
		unsigned char *pSrcPointer = pSrc;
		int nShift = 0;
		int nCount = 0;

		Image *pImage = pTier2->pImage;
		CodingParams *pCodingParams = pTier2->pCodingParams;
		PacketIterator *pPI = PI_Create(pImage, pCodingParams, nTileIndex);

		if (!pPI)
		{
			// TO DO: Throw an error
			return -999;
		}

		for (int nIndexPI = 0; nIndexPI <= pCodingParams->pTCP[nTileIndex].nPOCsCount; nIndexPI++)
		{
			while (PI_Next(&pPI[nIndexPI]))
			{
				if ((pCodingParams->nLayerFactor == 0) || (pCodingParams->nLayerFactor >= ((pPI[nIndexPI].nIndexLayer) + 1)))
				{
					nShift = Tier2_DecodePacket(pTier2, pSrcPointer, pSrc + nLen - pSrcPointer, pTile, &pCodingParams->pTCP[nTileIndex], &pPI[nIndexPI]);
				}
				else
				{
					nShift = 0;
				}

                pImage->pComponents[pPI[nIndexPI].nIndexComponent].nDecodedResCount = (nShift > 0) ? (std::max)(pPI[nIndexPI].nIndexResolution, pImage->pComponents[pPI[nIndexPI].nIndexComponent].nDecodedResCount) : pImage->pComponents[pPI[nIndexPI].nIndexComponent].nDecodedResCount;
				nCount++;

				if (nShift == -999)
				{
					break;
				}
				else
				{
					pSrcPointer += nShift;
				}
			}
		}

		PI_Destroy(pPI, pCodingParams, nTileIndex);

		if (nShift == -999)
		{
			return nShift;
		}

		return (pSrcPointer - pSrc);
	}

	Tier2* Tier2_Create(PCommon pCodecInfo, Image *pImage, CodingParams *pCodingParams)
	{
		Tier2 *pTier2 = (Tier2*)Malloc(sizeof(Tier2));
		if (!pTier2)
			return NULL;

		pTier2->pCodecInfo    = pCodecInfo;
		pTier2->pImage        = pImage;
		pTier2->pCodingParams = pCodingParams;

		return pTier2;
	}
	void   Tier2_Destroy(Tier2 *pTier2)
	{
		Free(pTier2);
	}
}

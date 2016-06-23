#pragma once

#include "DWT.h"
#include "Tier1.h"
#include "Tier2.h"
#include "float.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------

	void TCD_Dump(FILE *pFile, TCD *pTCD, TCDImage *pTCDImage)
	{
		fprintf(pFile, "<Image ");
		fprintf(pFile, "TilesCountW='%d' TilesCountH='%d' X0='%d' X1='%d' Y0='%d' Y1='%d'>", pTCDImage->nTilesCountW, pTCDImage->nTilesCountH, pTCD->pImage->nXOsiz, pTCD->pImage->nXsiz, pTCD->pImage->nYOsiz, pTCD->pImage->nYsiz);

		for (int nTileIndex = 0; nTileIndex < 1; nTileIndex++)
		{
			Tile *pTile = &pTCD->pTCDImage->pTiles[nTileIndex];
			fprintf(pFile, "<Tile Index ");
			fprintf(pFile, "X0='%d' Y0='%d' X1='%d' Y1='%d' ComponentsCount='%d'>\n", pTile->nX0, pTile->nY0, pTile->nX1, pTile->nY1, pTile->nComponentsCount);
			for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
			{
				TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
				fprintf(pFile, "<TileComp Index='%d' ", nComponentIndex);
				fprintf(pFile, "X0='%d' Y0='%d' X1='%d' Y1='%d' ResolutionsCount='%d'>\n", pTileComp->nX0, pTileComp->nY0, pTileComp->nX1, pTileComp->nY1, pTileComp->nResolutionsCount);
				for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
				{
					Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];
					fprintf(pFile, "<Resolution Index='%d' ", nResolutionIndex);
					fprintf(pFile, "X0='%d' Y0='%d' X1='%d' Y1='%d' PrecCountW='%d' PrecCountH='%d' BandsCount='%d'>\n", pResolution->nX0, pResolution->nY0, pResolution->nX1, pResolution->nY1, pResolution->nPrecCountW, pResolution->nPrecCountH, pResolution->nBandsCount);
					for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
					{
						Band *pBand = &pResolution->aBands[nBandIndex];
						fprintf(pFile, "<Band Index='%d' ", nBandIndex);
						fprintf(pFile, "X0='%d' Y0='%d' X1='%d' Y1='%d' StepSize='%f' BPSCount='%d'>\n", pBand->nX0, pBand->nY0, pBand->nX1, pBand->nY1, pBand->fStepSize, pBand->nBPSCount);
						for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
						{
							Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
							fprintf(pFile, "<Precinct Index='%d' ", nPrecinctIndex);
							fprintf(pFile, "X0='%d' Y0='%d' X1='%d' Y1='%d' CodeBlockCountW='%d' CodeBlockCountH='%d'>\n", pPrecinct->nX0, pPrecinct->nY0, pPrecinct->nX1, pPrecinct->nY1, pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);
							for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
							{
								CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
								fprintf(pFile, "<CodeBlock Index='%d' ", nCodeBlockIndex);
								fprintf(pFile, "X0='%d' Y0='%d' X1='%d' Y1='%d'/>\n", pCodeBlock->nX0, pCodeBlock->nY0, pCodeBlock->nX1, pCodeBlock->nY1);
							}
							fprintf(pFile, "</Precinct>\n");
						}
						fprintf(pFile, "</Band>\n");
					}
					fprintf(pFile, "</Resolution>\n");
				}
				fprintf(pFile, "</TileComp>\n");
			}
			fprintf(pFile, "</Tile>\n");
		}
		fprintf(pFile, "</Image>\n");
	}

	TCD* TCD_Create(PCommon pCodecInfo)
	{
		TCD *pTCD = (TCD*)Malloc(sizeof(TCD));

		if (!pTCD)
			return NULL;

		pTCD->pCodecInfo = pCodecInfo;
		pTCD->pTCDImage = (TCDImage*)Malloc(sizeof(TCDImage));

		if (!pTCD->pTCDImage)
		{
			Free(pTCD);
			return NULL;
		}

		return pTCD;
	}

	void TCD_Destroy(TCD *pTCD)
	{
		if (pTCD)
		{
			Free(pTCD->pTCDImage);
			Free(pTCD);
		}
	}

	void TCD_FreeEncode(TCD *pTCD)
	{
		for (int nTileIndex = 0; nTileIndex < 1; nTileIndex++)
		{
			Tile *pTile = pTCD->pTCDImage->pTiles;
			if (pTile)
			{
				for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
				{
					TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
					if (pTileComp)
					{
						for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
						{
							Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];
							if (pResolution)
							{
								for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
								{
									Band *pBand = &pResolution->aBands[nBandIndex];
									if (pBand)
									{
										for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
										{
											Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
											if (pPrecinct)
											{
												TGT_Destroy(pPrecinct->pInclTree);
												TGT_Destroy(pPrecinct->pIMSBTree);

												Free(pPrecinct->pCodeBlocks);
											}
										}
										Free(pBand->pPrecincts);
									}
								}
							}
						}
						Free(pTileComp->pResolutions);
					}
				}
				if (pTile->pComponents != NULL)
					Free(pTile->pComponents);
				pTile->pComponents = NULL;
			}
		}
		if (pTCD->pTCDImage->pTiles != NULL)
			Free(pTCD->pTCDImage->pTiles);
		pTCD->pTCDImage->pTiles = NULL;
	}

	void TCD_MallocEncode(TCD *pTCD, Image *pImage, CodingParams *pCodingParams, int nCurTileIndex)
	{
		pTCD->pImage                  = pImage;
		pTCD->pCodingParams           = pCodingParams;
		pTCD->pTCDImage->nTilesCountW = pCodingParams->nXTilesCount;
		pTCD->pTCDImage->nTilesCountH = pCodingParams->nYTilesCount;
		pTCD->pTCDImage->pTiles       = (Tile *)Malloc(sizeof(Tile));
		if (!pTCD->pTCDImage->pTiles)
		{
			TCD_FreeEncode(pTCD);
			pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		for (int nTileIndex = 0; nTileIndex < 1; nTileIndex++)
		{
			TileCodingParams *pTCP = &pCodingParams->pTCP[nCurTileIndex];

			// См. стр 59 ISO/IEC FDIS15444-1
			int nP = nCurTileIndex % pCodingParams->nXTilesCount;
			int nQ = nCurTileIndex / pCodingParams->nXTilesCount;

			Tile *pTile = pTCD->pTCDImage->pTiles;

			// Границы тайла (маштабированные относительно размеров изображения, если необходимо)
            pTile->nX0 = (std::max)(pCodingParams->nXTOsiz + nP * pCodingParams->nXTsiz, pImage->nXOsiz);
            pTile->nY0 = (std::max)(pCodingParams->nYTOsiz + nQ * pCodingParams->nYTsiz, pImage->nYOsiz);
            pTile->nX1 = (std::min)(pCodingParams->nXTOsiz + (nP + 1) * pCodingParams->nXTsiz, pImage->nXsiz);
            pTile->nY1 = (std::min)(pCodingParams->nYTOsiz + (nQ + 1) * pCodingParams->nYTsiz, pImage->nYsiz);
			pTile->nComponentsCount = pImage->nCsiz;

			// RATE >>
			for (int nIndex = 0; nIndex < pTCP->nLayersCount; nIndex++)
			{
				pTCP->afRates[nIndex] = pTCP->afRates[nIndex] ? ((float)(pTile->nComponentsCount * (pTile->nX1 - pTile->nX0) * (pTile->nY1 - pTile->nY0) * pImage->pComponents[0].nPrecision)) / (pTCP->afRates[nIndex] * 8 * pImage->pComponents[0].nXRsiz * pImage->pComponents[0].nYRsiz) : 0;

				if (pTCP->afRates[nIndex])
				{
					if (nIndex && pTCP->afRates[nIndex] < pTCP->afRates[nIndex - 1] + 10)
					{
						pTCP->afRates[nIndex] = pTCP->afRates[nIndex - 1] + 20;
					}
					else
					{
						if (!nIndex && pTCP->afRates[nIndex] < 30)
							pTCP->afRates[nIndex] = 30;
					}
				}
			}
			// << RATE

			pTile->pComponents = (TileComp *)Malloc(pImage->nCsiz * sizeof(TileComp));
			if (!pTile->pComponents)
			{
				TCD_FreeEncode(pTCD);
				pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}

			for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
			{
				TileCompCodingParams *pTCCP = &pTCP->pTCCP[nComponentIndex];
				TileComp *pTileComp = &pTile->pComponents[nComponentIndex];

				// Границы каждой тайл-компоненты (глобальные)
				pTileComp->nX0 = CeilDiv(pTile->nX0, pImage->pComponents[nComponentIndex].nXRsiz);
				pTileComp->nY0 = CeilDiv(pTile->nY0, pImage->pComponents[nComponentIndex].nYRsiz);
				pTileComp->nX1 = CeilDiv(pTile->nX1, pImage->pComponents[nComponentIndex].nXRsiz);
				pTileComp->nY1 = CeilDiv(pTile->nY1, pImage->pComponents[nComponentIndex].nYRsiz);

				pTileComp->pData = (int *)Malloc((pTileComp->nX1 - pTileComp->nX0) * (pTileComp->nY1 - pTileComp->nY0) * sizeof(int));
				if (!pTileComp->pData)
				{
					TCD_FreeEncode(pTCD);
					pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}
				pTileComp->nResolutionsCount = pTCCP->nResolutionsCount;

				pTileComp->pResolutions = (Resolution *)Malloc(pTileComp->nResolutionsCount * sizeof(Resolution));
				if (!pTileComp->pResolutions)
				{
					TCD_FreeEncode(pTCD);
					pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}

				for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
				{
					int nLevel = pTileComp->nResolutionsCount - 1 - nResolutionIndex;

					Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

					// Границы для каждого уровня (глобальные)
					pResolution->nX0 = CeilDivPow2(pTileComp->nX0, nLevel);
					pResolution->nY0 = CeilDivPow2(pTileComp->nY0, nLevel);
					pResolution->nX1 = CeilDivPow2(pTileComp->nX1, nLevel);
					pResolution->nY1 = CeilDivPow2(pTileComp->nY1, nLevel);

					pResolution->nBandsCount = (nResolutionIndex == 0 ? 1 : 3);

					// См. стр. 35, таблица A-23 ISO/IEC FDIS154444-1
					int nPPx, nPPy;
					if (pTCCP->nCodingStyle & J2K_CCP_CSTY_PRT)
					{
						nPPx = pTCCP->anPrecinctWidth[nResolutionIndex];
						nPPy = pTCCP->anPrecinctHeight[nResolutionIndex];
					}
					else
					{
						nPPx = 15;
						nPPy = 15;
					}

					// См. стр. 64, Annex-B.6, ISO/IEC FDIS15444-1
					int nFloorTRX0 = FloorDivPow2(pResolution->nX0, nPPx) << nPPx;
					int nFloorTRY0 = FloorDivPow2(pResolution->nY0, nPPy) << nPPy;
					int nCeilTRX1 = CeilDivPow2(pResolution->nX1, nPPx) << nPPx;
					int nCeilTRY1 = CeilDivPow2(pResolution->nY1, nPPy) << nPPy;

					pResolution->nPrecCountW = (nCeilTRX1 - nFloorTRX0) >> nPPx;
					pResolution->nPrecCountH = (nCeilTRY1 - nFloorTRY0) >> nPPy;

					int nCBWidthExpn, nCBHeightExpn;
					int nTLCodeBlockGlobalStartX, nTLCodeBlockGlobalStartY, nBRCodeBlockGlobalEndX, nBRCodeBlockGlobalEndY;
					if (0 == nResolutionIndex)
					{
						nTLCodeBlockGlobalStartX = nFloorTRX0;
						nTLCodeBlockGlobalStartY = nFloorTRY0;
						nBRCodeBlockGlobalEndX = nCeilTRX1;
						nBRCodeBlockGlobalEndY = nCeilTRY1;

						nCBWidthExpn  = nPPx;
						nCBHeightExpn = nPPy;
					}
					else
					{
						nTLCodeBlockGlobalStartX = CeilDivPow2(nFloorTRX0, 1);
						nTLCodeBlockGlobalStartY = CeilDivPow2(nFloorTRY0, 1);
						nBRCodeBlockGlobalEndX = CeilDivPow2(nCeilTRX1, 1);
						nBRCodeBlockGlobalEndY = CeilDivPow2(nCeilTRY1, 1);

						nCBWidthExpn  = nPPx - 1;
						nCBHeightExpn = nPPy - 1;
					}

                    int nXCBa = (std::min)(pTCCP->nCodeBlockWidth, nCBWidthExpn);
                    int nYCBa = (std::min)(pTCCP->nCodeBlockHeight, nCBHeightExpn);

					for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
					{
						Band *pBand = &pResolution->aBands[nBandIndex];
						pBand->nBandIndex = (nResolutionIndex == 0 ? 0 : nBandIndex + 1);

						int nBandX0 = (pBand->nBandIndex == 1) || (pBand->nBandIndex == 3) ? 1 : 0;
						int nBandY0 = (pBand->nBandIndex == 2) || (pBand->nBandIndex == 3) ? 1 : 0;

						if (pBand->nBandIndex == 0)
						{
							pBand->nX0 = CeilDivPow2(pTileComp->nX0, nLevel);
							pBand->nY0 = CeilDivPow2(pTileComp->nY0, nLevel);
							pBand->nX1 = CeilDivPow2(pTileComp->nX1, nLevel);
							pBand->nY1 = CeilDivPow2(pTileComp->nY1, nLevel);
						}
						else
						{
							pBand->nX0 = CeilDivPow2(pTileComp->nX0 - (1 << nLevel) * nBandX0, nLevel + 1);
							pBand->nY0 = CeilDivPow2(pTileComp->nY0 - (1 << nLevel) * nBandY0, nLevel + 1);
							pBand->nX1 = CeilDivPow2(pTileComp->nX1 - (1 << nLevel) * nBandX0, nLevel + 1);
							pBand->nY1 = CeilDivPow2(pTileComp->nY1 - (1 << nLevel) * nBandY0, nLevel + 1);
						}

						QuantStepSize *pStepSize = &pTCCP->aoStepSizes[nResolutionIndex == 0 ? 0 : 3 * (nResolutionIndex - 1) + nBandIndex + 1];
						int nGain = pTCCP->nTransformID == 0 ? DWT_GetGainIrr(pBand->nBandIndex) : DWT_GetGainRev(pBand->nBandIndex);
						int nBPSCount = pImage->pComponents[nComponentIndex].nPrecision + nGain;

						pBand->fStepSize = (float)((1.0 + pStepSize->nMantissa / 2048.0) * pow(2.0, nBPSCount - pStepSize->nExponent));
						pBand->nBPSCount = pStepSize->nExponent + pTCCP->nGuardBitsCount - 1;

						pBand->pPrecincts = (Precinct *)Malloc(3 * pResolution->nPrecCountW * pResolution->nPrecCountH * sizeof(Precinct));
						if (!pBand->pPrecincts)
						{
							pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
							return;
						}

						for (int nIndex = 0; nIndex < pResolution->nPrecCountW * pResolution->nPrecCountH * 3; nIndex++)
						{
							pBand->pPrecincts[nIndex].pIMSBTree = NULL;
							pBand->pPrecincts[nIndex].pInclTree = NULL;
						}

						for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
						{
							int nGlobalCodeBlockStartX = nTLCodeBlockGlobalStartX + (nPrecinctIndex % pResolution->nPrecCountW) * (1 << nCBWidthExpn);
							int nGlobalCodeBlockStartY = nTLCodeBlockGlobalStartY + (nPrecinctIndex / pResolution->nPrecCountW) * (1 << nCBHeightExpn);
							int nGlobalCodeBlockEndX = nGlobalCodeBlockStartX + (1 << nCBWidthExpn);
							int nGlobalCodeBlockEndY = nGlobalCodeBlockStartY + (1 << nCBHeightExpn);

							Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

							// Размеры области (Precinct)
                            pPrecinct->nX0 = (std::max)(nGlobalCodeBlockStartX, pBand->nX0);
                            pPrecinct->nY0 = (std::max)(nGlobalCodeBlockStartY, pBand->nY0);
                            pPrecinct->nX1 = (std::min)(nGlobalCodeBlockEndX, pBand->nX1);
                            pPrecinct->nY1 = (std::min)(nGlobalCodeBlockEndY, pBand->nY1);

							int nTLCodeBlockStartX = FloorDivPow2(pPrecinct->nX0, nXCBa) << nXCBa;
							int nTLCodeBlockStartY = FloorDivPow2(pPrecinct->nY0, nYCBa) << nYCBa;
							int nBRCodeBlockEndX = CeilDivPow2(pPrecinct->nX1, nXCBa) << nXCBa;
							int nBRCodeBlockEndY = CeilDivPow2(pPrecinct->nY1, nYCBa) << nYCBa;
							pPrecinct->nCountInWidth  = (nBRCodeBlockEndX - nTLCodeBlockStartX) >> nXCBa;
							pPrecinct->nCountInHeight = (nBRCodeBlockEndY - nTLCodeBlockStartY) >> nYCBa;

							pPrecinct->pCodeBlocks = (CodeBlock *)Malloc((pPrecinct->nCountInWidth * pPrecinct->nCountInHeight) * sizeof(CodeBlock));
							if (!pPrecinct->pCodeBlocks)
							{
								TCD_FreeEncode(pTCD);
								pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
								return;
							}

							pPrecinct->pInclTree = TGT_Create(pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);
							pPrecinct->pIMSBTree = TGT_Create(pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);

							for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
							{
								int nCodeBlockStartX = nTLCodeBlockStartX + (nCodeBlockIndex % pPrecinct->nCountInWidth) * (1 << nXCBa);
								int nCodeBlockStartY = nTLCodeBlockStartY + (nCodeBlockIndex / pPrecinct->nCountInWidth) * (1 << nYCBa);
								int nCodeBlockEndX = nCodeBlockStartX + (1 << nXCBa);
								int nCodeBlockEndY = nCodeBlockStartY + (1 << nYCBa);

								CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];

								// Границы CodeBlock (глобальные)
                                pCodeBlock->nX0 = (std::max)(nCodeBlockStartX, pPrecinct->nX0);
                                pCodeBlock->nY0 = (std::max)(nCodeBlockStartY, pPrecinct->nY0);
                                pCodeBlock->nX1 = (std::min)(nCodeBlockEndX, pPrecinct->nX1);
                                pCodeBlock->nY1 = (std::min)(nCodeBlockEndY, pPrecinct->nY1);
							}
						}
					}
				}
			}
		}
		//#ifdef _DEBUG
		//	FILE *pFile;
		//	pFile = fopen( "E:\\Jpeg2000DumpFile.xml", "wb" );
		//	if ( pFile )
		//	{
		//		TCD_Dump( pFile, pTCD, pTCD->pTCDImage );
		//		fclose( pFile );
		//	}
		//#endif
	}

	void TCD_InitEncode(TCD *pTCD, Image *pImage, CodingParams *pCodingParams, int nCurTileIndex)
	{
		for (int nTileIndex = 0; nTileIndex < 1; nTileIndex++)
		{
			TileCodingParams *pTCP = &pCodingParams->pTCP[nCurTileIndex];

			// См. стр 59 ISO/IEC FDIS15444-1
			int nP = nCurTileIndex % pCodingParams->nXTilesCount;
			int nQ = nCurTileIndex / pCodingParams->nXTilesCount;

			Tile *pTile = pTCD->pTCDImage->pTiles;

			// Границы тайла (маштабированные относительно размеров изображения, если необходимо)
            pTile->nX0 = (std::max)(pCodingParams->nXTOsiz + nP * pCodingParams->nXTsiz, pImage->nXOsiz);
            pTile->nY0 = (std::max)(pCodingParams->nYTOsiz + nQ * pCodingParams->nYTsiz, pImage->nYOsiz);
            pTile->nX1 = (std::min)(pCodingParams->nXTOsiz + (nP + 1) * pCodingParams->nXTsiz, pImage->nXsiz);
            pTile->nY1 = (std::min)(pCodingParams->nYTOsiz + (nQ + 1) * pCodingParams->nYTsiz, pImage->nYsiz);

			pTile->nComponentsCount = pImage->nCsiz;

			// RATE >>
			for (int nIndex = 0; nIndex < pTCP->nLayersCount; nIndex++)
			{
				pTCP->afRates[nIndex] = pTCP->afRates[nIndex] ? ((float)(pTile->nComponentsCount * (pTile->nX1 - pTile->nX0) * (pTile->nY1 - pTile->nY0) * pImage->pComponents[0].nPrecision)) / (pTCP->afRates[nIndex] * 8 * pImage->pComponents[0].nXRsiz * pImage->pComponents[0].nYRsiz) : 0;

				if (pTCP->afRates[nIndex])
				{
					if (nIndex && pTCP->afRates[nIndex] < pTCP->afRates[nIndex - 1] + 10)
					{
						pTCP->afRates[nIndex] = pTCP->afRates[nIndex - 1] + 20;
					}
					else
					{
						if (!nIndex && pTCP->afRates[nIndex] < 30)
							pTCP->afRates[nIndex] = 30;
					}
				}
			}
			// << RATE

			for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
			{
				TileCompCodingParams *pTCCP = &pTCP->pTCCP[nComponentIndex];
				TileComp *pTileComp = &pTile->pComponents[nComponentIndex];

				// Границы каждой тайл-компоненты (глобальные)
				pTileComp->nX0 = CeilDiv(pTile->nX0, pImage->pComponents[nComponentIndex].nXRsiz);
				pTileComp->nY0 = CeilDiv(pTile->nY0, pImage->pComponents[nComponentIndex].nYRsiz);
				pTileComp->nX1 = CeilDiv(pTile->nX1, pImage->pComponents[nComponentIndex].nXRsiz);
				pTileComp->nY1 = CeilDiv(pTile->nY1, pImage->pComponents[nComponentIndex].nYRsiz);

				pTileComp->pData = (int *)Malloc((pTileComp->nX1 - pTileComp->nX0) * (pTileComp->nY1 - pTileComp->nY0) * sizeof(int));
				if (!pTileComp->pData)
				{
					TCD_FreeEncode(pTCD);
					pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}

				pTileComp->nResolutionsCount = pTCCP->nResolutionsCount;

				for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
				{
					int nLevel = pTileComp->nResolutionsCount - 1 - nResolutionIndex;

					Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

					// Границы для каждого уровня (глобальные)
					pResolution->nX0 = CeilDivPow2(pTileComp->nX0, nLevel);
					pResolution->nY0 = CeilDivPow2(pTileComp->nY0, nLevel);
					pResolution->nX1 = CeilDivPow2(pTileComp->nX1, nLevel);
					pResolution->nY1 = CeilDivPow2(pTileComp->nY1, nLevel);
					pResolution->nBandsCount = (nResolutionIndex == 0 ? 1 : 3);

					// См. стр. 35, таблица A-23 ISO/IEC FDIS154444-1
					int nPPx, nPPy;
					if (pTCCP->nCodingStyle & J2K_CCP_CSTY_PRT)
					{
						nPPx = pTCCP->anPrecinctWidth[nResolutionIndex];
						nPPy = pTCCP->anPrecinctHeight[nResolutionIndex];
					}
					else
					{
						nPPx = 15;
						nPPy = 15;
					}

					// См. стр. 64, Annex-B.6, ISO/IEC FDIS15444-1
					int nFloorTRX0 = FloorDivPow2(pResolution->nX0, nPPx) << nPPx;
					int nFloorTRY0 = FloorDivPow2(pResolution->nY0, nPPy) << nPPy;
					int nCeilTRX1 = CeilDivPow2(pResolution->nX1, nPPx) << nPPx;
					int nCeilTRY1 = CeilDivPow2(pResolution->nY1, nPPy) << nPPy;

					pResolution->nPrecCountW = (nCeilTRX1 - nFloorTRX0) >> nPPx;
					pResolution->nPrecCountH = (nCeilTRY1 - nFloorTRY0) >> nPPy;

					int nCBWidthExpn, nCBHeightExpn;
					int nTLCodeBlockGlobalStartX, nTLCodeBlockGlobalStartY, nBRCodeBlockGlobalEndX, nBRCodeBlockGlobalEndY;
					if (0 == nResolutionIndex)
					{
						nTLCodeBlockGlobalStartX = nFloorTRX0;
						nTLCodeBlockGlobalStartY = nFloorTRY0;
						nBRCodeBlockGlobalEndX = nCeilTRX1;
						nBRCodeBlockGlobalEndY = nCeilTRY1;

						nCBWidthExpn  = nPPx;
						nCBHeightExpn = nPPy;
					}
					else
					{
						nTLCodeBlockGlobalStartX = CeilDivPow2(nFloorTRX0, 1);
						nTLCodeBlockGlobalStartY = CeilDivPow2(nFloorTRY0, 1);
						nBRCodeBlockGlobalEndX = CeilDivPow2(nCeilTRX1, 1);
						nBRCodeBlockGlobalEndY = CeilDivPow2(nCeilTRY1, 1);

						nCBWidthExpn  = nPPx - 1;
						nCBHeightExpn = nPPy - 1;
					}

                    int nXCBa = (std::min)(pTCCP->nCodeBlockWidth, nCBWidthExpn);
                    int nYCBa = (std::min)(pTCCP->nCodeBlockHeight, nCBHeightExpn);

					for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
					{
						Band *pBand = &pResolution->aBands[nBandIndex];
						pBand->nBandIndex = (nResolutionIndex == 0 ? 0 : nBandIndex + 1);

						int nBandX0 = (pBand->nBandIndex == 1) || (pBand->nBandIndex == 3) ? 1 : 0;
						int nBandY0 = (pBand->nBandIndex == 2) || (pBand->nBandIndex == 3) ? 1 : 0;

						if (0 == pBand->nBandIndex)
						{
							pBand->nX0 = CeilDivPow2(pTileComp->nX0, nLevel);
							pBand->nY0 = CeilDivPow2(pTileComp->nY0, nLevel);
							pBand->nX1 = CeilDivPow2(pTileComp->nX1, nLevel);
							pBand->nY1 = CeilDivPow2(pTileComp->nY1, nLevel);
						}
						else
						{
							pBand->nX0 = CeilDivPow2(pTileComp->nX0 - (1 << nLevel) * nBandX0, nLevel + 1);
							pBand->nY0 = CeilDivPow2(pTileComp->nY0 - (1 << nLevel) * nBandY0, nLevel + 1);
							pBand->nX1 = CeilDivPow2(pTileComp->nX1 - (1 << nLevel) * nBandX0, nLevel + 1);
							pBand->nY1 = CeilDivPow2(pTileComp->nY1 - (1 << nLevel) * nBandY0, nLevel + 1);
						}

						QuantStepSize *pStepSize = &pTCCP->aoStepSizes[nResolutionIndex == 0 ? 0 : 3 * (nResolutionIndex - 1) + nBandIndex + 1];
						int nGain = pTCCP->nTransformID == 0 ? DWT_GetGainIrr(pBand->nBandIndex) : DWT_GetGainRev(pBand->nBandIndex);
						int nBPSCount = pImage->pComponents[nComponentIndex].nPrecision + nGain;

						pBand->fStepSize = (float)((1.0 + pStepSize->nMantissa / 2048.0) * pow(2.0, nBPSCount - pStepSize->nExponent));
						pBand->nBPSCount = pStepSize->nExponent + pTCCP->nGuardBitsCount - 1;

						for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
						{
							int nGlobalCodeBlockStartX = nTLCodeBlockGlobalStartX + (nPrecinctIndex % pResolution->nPrecCountW) * (1 << nCBWidthExpn);
							int nGlobalCodeBlockStartY = nTLCodeBlockGlobalStartY + (nPrecinctIndex / pResolution->nPrecCountW) * (1 << nCBHeightExpn);
							int nGlobalCodeBlockEndX = nGlobalCodeBlockStartX + (1 << nCBWidthExpn);
							int nGlobalCodeBlockEndY = nGlobalCodeBlockStartY + (1 << nCBHeightExpn);

							Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];

							// Размеры области (Precinct)
                            pPrecinct->nX0 = (std::max)(nGlobalCodeBlockStartX, pBand->nX0);
                            pPrecinct->nY0 = (std::max)(nGlobalCodeBlockStartY, pBand->nY0);
                            pPrecinct->nX1 = (std::min)(nGlobalCodeBlockEndX, pBand->nX1);
                            pPrecinct->nY1 = (std::min)(nGlobalCodeBlockEndY, pBand->nY1);

							int nTLCodeBlockStartX = FloorDivPow2(pPrecinct->nX0, nXCBa) << nXCBa;
							int nTLCodeBlockStartY = FloorDivPow2(pPrecinct->nY0, nYCBa) << nYCBa;
							int nBRCodeBlockEndX = CeilDivPow2(pPrecinct->nX1, nXCBa) << nXCBa;
							int nBRCodeBlockEndY = CeilDivPow2(pPrecinct->nY1, nYCBa) << nYCBa;
							pPrecinct->nCountInWidth  = (nBRCodeBlockEndX - nTLCodeBlockStartX) >> nXCBa;
							pPrecinct->nCountInHeight = (nBRCodeBlockEndY - nTLCodeBlockStartY) >> nYCBa;

							Free(pPrecinct->pCodeBlocks);
							pPrecinct->pCodeBlocks = (CodeBlock *)Malloc(pPrecinct->nCountInWidth * pPrecinct->nCountInHeight * sizeof(CodeBlock));
							if (!pPrecinct->pCodeBlocks)
							{
								TCD_FreeEncode(pTCD);
								pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
								return;
							}

							if (pPrecinct->pInclTree != NULL)
							{
								TGT_Destroy(pPrecinct->pInclTree);
							}
							if (pPrecinct->pIMSBTree != NULL)
							{
								TGT_Destroy(pPrecinct->pIMSBTree);
							}

							pPrecinct->pInclTree = TGT_Create(pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);
							pPrecinct->pIMSBTree = TGT_Create(pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);

							for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
							{
								int nCodeBlockStartX = nTLCodeBlockStartX + (nCodeBlockIndex % pPrecinct->nCountInWidth) * (1 << nXCBa);
								int nCodeBlockStartY = nTLCodeBlockStartY + (nCodeBlockIndex / pPrecinct->nCountInWidth) * (1 << nYCBa);
								int nCodeBlockEndX = nCodeBlockStartX + (1 << nXCBa);
								int nCodeBlockEndY = nCodeBlockStartY + (1 << nYCBa);

								CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];

								// Границы CodeBlock (глобальные)
                                pCodeBlock->nX0 = (std::max)(nCodeBlockStartX, pPrecinct->nX0);
                                pCodeBlock->nY0 = (std::max)(nCodeBlockStartY, pPrecinct->nY0);
                                pCodeBlock->nX1 = (std::min)(nCodeBlockEndX, pPrecinct->nX1);
                                pCodeBlock->nY1 = (std::min)(nCodeBlockEndY, pPrecinct->nY1);
							}
						}
					}
				}
			}
		}

		//#ifdef _DEBUG
		//	FILE *pFile;
		//	pFile = fopen( "E:\\Jpeg2000DumpFile.xml", "wb" );
		//	if ( pFile )
		//	{
		//		TCD_Dump( pFile, pTCD, pTCD->pTCDImage );
		//		fclose( pFile );
		//	}
		//#endif
	}

	void TCD_FreeDecode(TCD *pTCD)
	{
		TCDImage *pTCDImage = pTCD->pTCDImage;
		if (pTCDImage != NULL)
		{
			for (int nTileIndex = 0; nTileIndex < pTCDImage->nTilesCountW * pTCDImage->nTilesCountH; nTileIndex++)
			{
				Tile *pTile = &pTCDImage->pTiles[nTileIndex];
				if (pTile != NULL)
				{
					for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
					{
						TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
						if (pTileComp != NULL)
						{
							for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
							{
								Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];
								if (pResolution != NULL)
								{
									for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
									{
										Band *pBand = &pResolution->aBands[nBandIndex];
										if (pBand != NULL)
										{
											for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountH * pResolution->nPrecCountW; nPrecinctIndex++)
											{
												if (pBand->pPrecincts == NULL)continue;

												Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
												if (pPrecinct != NULL)
												{
													Free(pPrecinct->pCodeBlocks);

													if (pPrecinct->pIMSBTree != NULL)
														TGT_Destroy(pPrecinct->pIMSBTree);

													if (pPrecinct->pInclTree != NULL)
														TGT_Destroy(pPrecinct->pInclTree);
												}
											}
											Free(pBand->pPrecincts);
										}
									}
								}
							}
							Free(pTileComp->pResolutions);
							Free(pTileComp->pData);
						}
					}
					Free(pTile->pComponents);
				}
			}
			if (pTCDImage->pTiles != NULL)
				Free(pTCDImage->pTiles);
		}
	}


	void TCD_MallocDecode(TCD *pTCD, Image *pImage, CodingParams *pCodingParams)
	{
		pTCD->pImage                  = pImage;
		pTCD->pCodingParams           = pCodingParams;
		pTCD->pTCDImage->nTilesCountW = pCodingParams->nXTilesCount;
		pTCD->pTCDImage->nTilesCountH = pCodingParams->nYTilesCount;
		pTCD->pTCDImage->pTiles       = (Tile *)Malloc(pCodingParams->nXTilesCount * pCodingParams->nYTilesCount * sizeof(Tile));
		if (!pTCD->pTCDImage->pTiles)
		{
			pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		for (int nIndex = 0; nIndex < pCodingParams->nTileIndexSize; nIndex++)
		{
			TileCodingParams *pTCP = &(pCodingParams->pTCP[pCodingParams->pTileIndex[nIndex]]);
			Tile *pTile = &(pTCD->pTCDImage->pTiles[pCodingParams->pTileIndex[nIndex]]);

			// См. стр 59 ISO/IEC FDIS15444-1
			int nTileIndex = pCodingParams->pTileIndex[nIndex];
			int nP = nTileIndex % pCodingParams->nXTilesCount;
			int nQ = nTileIndex / pCodingParams->nXTilesCount;

			// Границы тайла (маштабированные относительно размеров изображения, если необходимо)
            pTile->nX0 = (std::max)(pCodingParams->nXTOsiz + nP * pCodingParams->nXTsiz, pImage->nXOsiz);
            pTile->nY0 = (std::max)(pCodingParams->nYTOsiz + nQ * pCodingParams->nYTsiz, pImage->nYOsiz);
            pTile->nX1 = (std::min)(pCodingParams->nXTOsiz + (nP + 1) * pCodingParams->nXTsiz, pImage->nXsiz);
            pTile->nY1 = (std::min)(pCodingParams->nYTOsiz + (nQ + 1) * pCodingParams->nYTsiz, pImage->nYsiz);

			pTile->nComponentsCount = pImage->nCsiz;
			pTile->pComponents = (TileComp *)Malloc(pImage->nCsiz * sizeof(TileComp));
			if (!pTile->pComponents)
			{
				TCD_FreeDecode(pTCD);
				pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}

			for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
			{
				TileCompCodingParams *pTCCP = &pTCP->pTCCP[nComponentIndex];
				TileComp *pTileComp = &pTile->pComponents[nComponentIndex];

				// Границы каждой тайл-компоненты (глобальные)
				pTileComp->nX0 = CeilDiv(pTile->nX0, pImage->pComponents[nComponentIndex].nXRsiz);
				pTileComp->nY0 = CeilDiv(pTile->nY0, pImage->pComponents[nComponentIndex].nYRsiz);
				pTileComp->nX1 = CeilDiv(pTile->nX1, pImage->pComponents[nComponentIndex].nXRsiz);
				pTileComp->nY1 = CeilDiv(pTile->nY1, pImage->pComponents[nComponentIndex].nYRsiz);

				long sz_alloc = (pTileComp->nX1 - pTileComp->nX0) * (pTileComp->nY1 - pTileComp->nY0) * sizeof(int);
				pTileComp->pData = (int *)Malloc(sz_alloc);
				if (!pTileComp->pData)
				{
					TCD_FreeDecode(pTCD);
					pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}
				pTileComp->nResolutionsCount = pTCCP->nResolutionsCount;
				pTileComp->pResolutions = (Resolution *)Malloc(pTileComp->nResolutionsCount * sizeof(Resolution));
				if (!pTileComp->pResolutions)
				{
					TCD_FreeDecode(pTCD);
					pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
					return;
				}

				for (int nResolutionIndex = 0; nResolutionIndex < pTileComp->nResolutionsCount; nResolutionIndex++)
				{
					int nLevel = pTileComp->nResolutionsCount - 1 - nResolutionIndex;

					Resolution *pResolution = &pTileComp->pResolutions[nResolutionIndex];

					// Границы для каждого уровня (глобальные)
					pResolution->nX0 = CeilDivPow2(pTileComp->nX0, nLevel);
					pResolution->nY0 = CeilDivPow2(pTileComp->nY0, nLevel);
					pResolution->nX1 = CeilDivPow2(pTileComp->nX1, nLevel);
					pResolution->nY1 = CeilDivPow2(pTileComp->nY1, nLevel);
					pResolution->nBandsCount = (nResolutionIndex == 0 ? 1 : 3);

					// См. стр. 35, таблица A-23 ISO/IEC FDIS154444-1
					int nPPx, nPPy;
					if (pTCCP->nCodingStyle & J2K_CCP_CSTY_PRT)
					{
						nPPx = pTCCP->anPrecinctWidth[nResolutionIndex];
						nPPy = pTCCP->anPrecinctHeight[nResolutionIndex];
					}
					else
					{
						nPPx = 15;
						nPPy = 15;
					}

					// См. стр. 64, Annex-B.6, ISO/IEC FDIS15444-1
					int nFlootTRX0 = FloorDivPow2(pResolution->nX0, nPPx) << nPPx;
					int nFloorTRY0 = FloorDivPow2(pResolution->nY0, nPPy) << nPPy;
					int nCeilTRX1 = CeilDivPow2(pResolution->nX1, nPPx) << nPPx;
					int nCeilTRY1 = CeilDivPow2(pResolution->nY1, nPPy) << nPPy;

					pResolution->nPrecCountW = (pResolution->nX0 == pResolution->nX1) ? 0 : ((nCeilTRX1 - nFlootTRX0) >> nPPx);
					pResolution->nPrecCountH = (pResolution->nY0 == pResolution->nY1) ? 0 : ((nCeilTRY1 - nFloorTRY0) >> nPPy);

					int nCBWidthExpn, nCBHeightExpn;
					int nTLCodeBlockGlobalStartX, nTLCodeBlockGlobalStartY, nBRCodeBlockGlobalEndX, nBRCodeBlockGlobalEndY;
					if (0 == nResolutionIndex)
					{
						nTLCodeBlockGlobalStartX = nFlootTRX0;
						nTLCodeBlockGlobalStartY = nFloorTRY0;
						nBRCodeBlockGlobalEndX = nCeilTRX1;
						nBRCodeBlockGlobalEndY = nCeilTRY1;

						nCBWidthExpn  = nPPx;
						nCBHeightExpn = nPPy;
					}
					else
					{
						nTLCodeBlockGlobalStartX = CeilDivPow2(nFlootTRX0, 1);
						nTLCodeBlockGlobalStartY = CeilDivPow2(nFloorTRY0, 1);
						nBRCodeBlockGlobalEndX = CeilDivPow2(nCeilTRX1, 1);
						nBRCodeBlockGlobalEndY = CeilDivPow2(nCeilTRY1, 1);

						nCBWidthExpn  = nPPx - 1;
						nCBHeightExpn = nPPy - 1;
					}

                    int nXCBa = (std::min)(pTCCP->nCodeBlockWidth, nCBWidthExpn);
                    int nYCBa = (std::min)(pTCCP->nCodeBlockHeight, nCBHeightExpn);

					for (int nBandIndex = 0; nBandIndex < pResolution->nBandsCount; nBandIndex++)
					{
						Band *pBand = &pResolution->aBands[nBandIndex];
						pBand->nBandIndex = (nResolutionIndex == 0 ? 0 : nBandIndex + 1);

						int nBandX0 = (pBand->nBandIndex == 1) || (pBand->nBandIndex == 3) ? 1 : 0;
						int nBandY0 = (pBand->nBandIndex == 2) || (pBand->nBandIndex == 3) ? 1 : 0;

						if (0 == pBand->nBandIndex)
						{
							pBand->nX0 = CeilDivPow2(pTileComp->nX0, nLevel);
							pBand->nY0 = CeilDivPow2(pTileComp->nY0, nLevel);
							pBand->nX1 = CeilDivPow2(pTileComp->nX1, nLevel);
							pBand->nY1 = CeilDivPow2(pTileComp->nY1, nLevel);
						}
						else
						{
							pBand->nX0 = CeilDivPow2(pTileComp->nX0 - (1 << nLevel) * nBandX0, nLevel + 1);
							pBand->nY0 = CeilDivPow2(pTileComp->nY0 - (1 << nLevel) * nBandY0, nLevel + 1);
							pBand->nX1 = CeilDivPow2(pTileComp->nX1 - (1 << nLevel) * nBandX0, nLevel + 1);
							pBand->nY1 = CeilDivPow2(pTileComp->nY1 - (1 << nLevel) * nBandY0, nLevel + 1);
						}

						QuantStepSize *pStepSize = &pTCCP->aoStepSizes[nResolutionIndex == 0 ? 0 : 3 * (nResolutionIndex - 1) + nBandIndex + 1];
						int nGain = pTCCP->nTransformID == 0 ? DWT_GetGainIrr(pBand->nBandIndex) : DWT_GetGainRev(pBand->nBandIndex);
						int nBPSCount = pImage->pComponents[nComponentIndex].nPrecision + nGain;
						pBand->fStepSize = (float)((1.0 + pStepSize->nMantissa / 2048.0) * pow(2.0, nBPSCount - pStepSize->nExponent));
						pBand->nBPSCount = pStepSize->nExponent + pTCCP->nGuardBitsCount - 1;

						pBand->pPrecincts = (Precinct *)Malloc(pResolution->nPrecCountW * pResolution->nPrecCountH * sizeof(Precinct));
						if (!pBand->pPrecincts)
						{
							TCD_FreeDecode(pTCD);
							pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
							return;
						}

						for (int nPrecinctIndex = 0; nPrecinctIndex < pResolution->nPrecCountW * pResolution->nPrecCountH; nPrecinctIndex++)
						{
							int nGlobalCodeBlockStartX = nTLCodeBlockGlobalStartX + (nPrecinctIndex % pResolution->nPrecCountW) * (1 << nCBWidthExpn);
							int nGlobalCodeBlockStartY = nTLCodeBlockGlobalStartY + (nPrecinctIndex / pResolution->nPrecCountW) * (1 << nCBHeightExpn);
							int nGlobalCodeBlockEndX = nGlobalCodeBlockStartX + (1 << nCBWidthExpn);
							int nGlobalCodeBlockEndY = nGlobalCodeBlockStartY + (1 << nCBHeightExpn);

							Precinct *pPrecinct = &pBand->pPrecincts[nPrecinctIndex];
							// Размеры области (Precinct)
                            pPrecinct->nX0 = (std::max)(nGlobalCodeBlockStartX, pBand->nX0);
                            pPrecinct->nY0 = (std::max)(nGlobalCodeBlockStartY, pBand->nY0);
                            pPrecinct->nX1 = (std::min)(nGlobalCodeBlockEndX, pBand->nX1);
                            pPrecinct->nY1 = (std::min)(nGlobalCodeBlockEndY, pBand->nY1);

							int nTLCodeBlockStartX = FloorDivPow2(pPrecinct->nX0, nXCBa) << nXCBa;
							int nTLCodeBlockStartY = FloorDivPow2(pPrecinct->nY0, nYCBa) << nYCBa;
							int nBRCodeBlockEndX = CeilDivPow2(pPrecinct->nX1, nXCBa) << nXCBa;
							int nBRCodeBlockEndY = CeilDivPow2(pPrecinct->nY1, nYCBa) << nYCBa;
							pPrecinct->nCountInWidth  = (nBRCodeBlockEndX - nTLCodeBlockStartX) >> nXCBa;
							pPrecinct->nCountInHeight = (nBRCodeBlockEndY - nTLCodeBlockStartY) >> nYCBa;

							pPrecinct->pCodeBlocks = (CodeBlock *)Malloc(pPrecinct->nCountInWidth * pPrecinct->nCountInHeight * sizeof(CodeBlock));
							if (!pPrecinct->pCodeBlocks)
							{
								TCD_FreeDecode(pTCD);
								pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
								return;
							}

							pPrecinct->pInclTree = TGT_Create(pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);
							pPrecinct->pIMSBTree = TGT_Create(pPrecinct->nCountInWidth, pPrecinct->nCountInHeight);

							for (int nCodeBlockIndex = 0; nCodeBlockIndex < pPrecinct->nCountInWidth * pPrecinct->nCountInHeight; nCodeBlockIndex++)
							{
								int nCodeBlockStartX = nTLCodeBlockStartX + (nCodeBlockIndex % pPrecinct->nCountInWidth) * (1 << nXCBa);
								int nCodeBlockStartY = nTLCodeBlockStartY + (nCodeBlockIndex / pPrecinct->nCountInWidth) * (1 << nYCBa);
								int nCodeBlockEndX = nCodeBlockStartX + (1 << nXCBa);
								int nCodeBlockEndY = nCodeBlockStartY + (1 << nYCBa);

								// Границы CodeBlock (глобальные)
								CodeBlock *pCodeBlock = &pPrecinct->pCodeBlocks[nCodeBlockIndex];
                                pCodeBlock->nX0 = (std::max)(nCodeBlockStartX, pPrecinct->nX0);
                                pCodeBlock->nY0 = (std::max)(nCodeBlockStartY, pPrecinct->nY0);
                                pCodeBlock->nX1 = (std::min)(nCodeBlockEndX, pPrecinct->nX1);
                                pCodeBlock->nY1 = (std::min)(nCodeBlockEndY, pPrecinct->nY1);
							}
						}
					}
				}
			}
		}
		//
		//#ifdef _DEBUG
		//	FILE *pFile;
		//	pFile = fopen( "E:\\Jpeg2000DumpFile.xml", "wb" );
		//	if ( pFile )
		//	{
		//		TCD_Dump( pFile, pTCD, pTCD->pTCDImage );
		//		fclose( pFile );
		//	}
		//#endif

		// Выделяем место для хранения закодированных данных ( = окончательное изображение ).

		unsigned int nX0 = 0, nY0 = 0, nX1 = 0, nY1 = 0, nWidth, nHeight;

		for (int nComponentIndex = 0; nComponentIndex < pImage->nCsiz; nComponentIndex++)
		{
			for (int nIndex = 0; nIndex < pCodingParams->nTileIndexSize; nIndex++)
			{
				int nTileIndex = pCodingParams->pTileIndex[nIndex];
                nX0 = (nIndex == 0 ? pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nX0 : (std::min)(nX0, (unsigned int)pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nX0));
                nY0 = (nIndex == 0 ? pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nY0 : (std::min)(nY0, (unsigned int)pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nY0));
                nX1 = (nIndex == 0 ? pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nX1 : (std::max)(nX1, (unsigned int)pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nX1));
                nY1 = (nIndex == 0 ? pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nY1 : (std::max)(nY1, (unsigned int)pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].nY1));
			}

			nWidth  = nX1 - nX0;
			nHeight = nY1 - nY0;

			pImage->pComponents[nComponentIndex].pData   = (int *)Malloc(nWidth * nHeight * sizeof(int));
			if (!pImage->pComponents[nComponentIndex].pData)
			{
				TCD_FreeDecode(pTCD);
				for (int nI = 0; nI < nComponentIndex; nI++)
					Free(pImage->pComponents[nI].pData);
				pTCD->pCodecInfo->nErrorCode = JP2_ERROR_NOT_ENOUGH_MEMORY;
				return;
			}
			pImage->pComponents[nComponentIndex].nWidth  = nWidth;
			pImage->pComponents[nComponentIndex].nHeight = nHeight;
			pImage->pComponents[nComponentIndex].nXOsiz  = nX0;
			pImage->pComponents[nComponentIndex].nYOsiz  = nY0;
		}
	}

	void TCD_MakeLayerFixed(TCD *pTCD, int nLayerIndex, int nFinal)
	{
		CodingParams *pCodingParams = pTCD->pCodingParams;
		Tile *pTile = pTCD->pTCDTile;
		TileCodingParams *pTCP = pTCD->pTCP;

		int aMatrix[10][10][3];
		int nValue = 0; // aMatrix[pTCP->nLayersCount][pTile->pComponents[0].nResolutionsCount][3]

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			for (int nI = 0; nI < pTCP->nLayersCount; nI++)
			{
				for (int nJ = 0; nJ < pTileComp->nResolutionsCount; nJ++)
				{
					for (int nK = 0; nK < 3; nK++)
					{
						aMatrix[nI][nJ][nK] = (int)(pCodingParams->pMatrix[nI * pTileComp->nResolutionsCount * 3 + nJ * 3 + nK] * (float)(pTCD->pImage->pComponents[nComponentIndex].nPrecision / 16.0));
					}
				}
			}

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
							Layer *pLayer = &pCodeBlock->aLayers[nLayerIndex];

							int nIMSB = pTCD->pImage->pComponents[nComponentIndex].nPrecision - pCodeBlock->nBPSCount; // Количество bit-plan равных 0

							if (0 == nLayerIndex)
							{
								nValue = aMatrix[nLayerIndex][nResolutionIndex][nBandIndex];
								if (nIMSB >= nValue)
								{
									nValue = 0;
								}
								else
								{
									nValue -= nIMSB;
								}
							}
							else
							{
								nValue = aMatrix[nLayerIndex][nResolutionIndex][nBandIndex] - aMatrix[nLayerIndex - 1][nResolutionIndex][nBandIndex];
								if (nIMSB >= aMatrix[nLayerIndex - 1][nResolutionIndex][nBandIndex])
								{
									nValue -= (nIMSB - aMatrix[nLayerIndex - 1][nResolutionIndex][nBandIndex]);
									if (nValue < 0)
									{
										nValue = 0;
									}
								}
							}

							if (0 == nLayerIndex)
							{
								pCodeBlock->nPassesCountInLayers = 0;
							}

							int nLayersPasses = pCodeBlock->nPassesCountInLayers;
							if (0 == pCodeBlock->nPassesCountInLayers)
							{
								if (nValue != 0)
								{
									nLayersPasses = 3 * nValue - 2 + pCodeBlock->nPassesCountInLayers;
								}
								else
								{
									nLayersPasses = pCodeBlock->nPassesCountInLayers;
								}
							}
							else
							{
								nLayersPasses = 3 * nValue + pCodeBlock->nPassesCountInLayers;
							}

							pLayer->nPassesCount = nLayersPasses - pCodeBlock->nPassesCountInLayers;

							if (!pLayer->nPassesCount)
								continue;

							if (0 == pCodeBlock->nPassesCountInLayers)
							{
								pLayer->nLen = pCodeBlock->aPasses[nLayersPasses - 1].nRate;
								pLayer->pData = pCodeBlock->aData;
							}
							else
							{
								pLayer->nLen = pCodeBlock->aPasses[nLayersPasses - 1].nRate - pCodeBlock->aPasses[pCodeBlock->nPassesCountInLayers - 1].nRate;
								pLayer->pData = pCodeBlock->aData + pCodeBlock->aPasses[pCodeBlock->nPassesCountInLayers - 1].nRate;
							}
							if (nFinal)
								pCodeBlock->nPassesCountInLayers = nLayersPasses;
						}
					}
				}
			}
		}
	}

	void TCD_RateAllocateFixed(TCD *pTCD)
	{
		for (int nLayerIndex = 0; nLayerIndex < pTCD->pTCP->nLayersCount; nLayerIndex++)
		{
			TCD_MakeLayerFixed(pTCD, nLayerIndex, 1);
		}
	}
	void TCD_MakeLayer(TCD *pTCD, int nLayerIndex, double dTreshold, int nFinal)
	{
		Tile *pTile = pTCD->pTCDTile;
		pTile->adDistoLayer[nLayerIndex] = 0;

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
							Layer *pLayer = &pCodeBlock->aLayers[nLayerIndex];

							if (0 == nLayerIndex)
							{
								pCodeBlock->nPassesCountInLayers = 0;
							}
							int nLayersPasses = pCodeBlock->nPassesCountInLayers;

							for (int nPassIndex = pCodeBlock->nPassesCountInLayers; nPassIndex < pCodeBlock->nTotalPassesCount; nPassIndex++)
							{
								TCDPass *pPass = &pCodeBlock->aPasses[nPassIndex];

								int nRate = 0;
								double dDisto = 0;
								if (0 == nLayersPasses)
								{
									nRate  = pPass->nRate;
									dDisto = pPass->dDistortionDec;
								}
								else
								{
									nRate  = pPass->nRate - pCodeBlock->aPasses[nLayersPasses - 1].nRate;
									dDisto = pPass->dDistortionDec - pCodeBlock->aPasses[nLayersPasses - 1].dDistortionDec;
								}
								if (!nRate)
								{
									if (dDisto != 0)
										nLayersPasses = nPassIndex + 1;
									continue;
								}
								if (dDisto / nRate >= dTreshold)
									nLayersPasses = nPassIndex + 1;
							}
							pLayer->nPassesCount = nLayersPasses - pCodeBlock->nPassesCountInLayers;

							if (!pLayer->nPassesCount)
							{
								pLayer->dDisto = 0;
								continue;
							}

							if (0 == pCodeBlock->nPassesCountInLayers)
							{
								pLayer->nLen   = pCodeBlock->aPasses[nLayersPasses - 1].nRate;
								pLayer->pData  = pCodeBlock->aData;
								pLayer->dDisto = pCodeBlock->aPasses[nLayersPasses - 1].dDistortionDec;
							}
							else
							{
								pLayer->nLen   = pCodeBlock->aPasses[nLayersPasses - 1].nRate - pCodeBlock->aPasses[pCodeBlock->nPassesCountInLayers - 1].nRate;
								pLayer->pData  = pCodeBlock->aData + pCodeBlock->aPasses[pCodeBlock->nPassesCountInLayers - 1].nRate;
								pLayer->dDisto = pCodeBlock->aPasses[nLayersPasses - 1].dDistortionDec - pCodeBlock->aPasses[pCodeBlock->nPassesCountInLayers - 1].dDistortionDec;
							}

							pTile->adDistoLayer[nLayerIndex] += pLayer->dDisto;

							if (nFinal)
								pCodeBlock->nPassesCountInLayers = nLayersPasses;
						}
					}
				}
			}
		}
	}

	bool TCD_RateAllocate(TCD *pTCD, unsigned char *pDst, int nLen, ImageInfo *pImageInfo)
	{
		CodingParams *pCodingParams = pTCD->pCodingParams;
		Tile *pTile = pTCD->pTCDTile;
		TileCodingParams *pTCP = pTCD->pTCP;

		double dMin = DBL_MAX;
		double dMax = 0;
		double dMaxSE = 0;

		pTile->nNBPix = 0;

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			pTileComp->nNBPix = 0;

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

							for (int nPassIndex = 0; nPassIndex < pCodeBlock->nTotalPassesCount; nPassIndex++)
							{
								TCDPass *pPass = &pCodeBlock->aPasses[nPassIndex];

								int nRate = 1;
								double dDisto = 0;
								if (0 == nPassIndex)
								{
									nRate = pPass->nRate;
									dDisto = pPass->dDistortionDec;
								}
								else
								{
									nRate = pPass->nRate - pCodeBlock->aPasses[nPassIndex - 1].nRate;
									dDisto = pPass->dDistortionDec - pCodeBlock->aPasses[nPassIndex - 1].dDistortionDec;
								}
								if (0 == nRate)
								{
									continue;
								}

								int dRateDistoSlope = dDisto / nRate;
								if (dRateDistoSlope < dMin)
								{
									dMin = dRateDistoSlope;
								}
								if (dRateDistoSlope > dMax)
								{
									dMax = dRateDistoSlope;
								}
							}

							pTile->nNBPix     += ((pCodeBlock->nX1 - pCodeBlock->nX0) * (pCodeBlock->nY1 - pCodeBlock->nY0));
							pTileComp->nNBPix += ((pCodeBlock->nX1 - pCodeBlock->nX0) * (pCodeBlock->nY1 - pCodeBlock->nY0));
						}
					}
				}
			}

			dMaxSE += (((double)(1 << pTCD->pImage->pComponents[nComponentIndex].nPrecision) - 1.0) * ((double)(1 << pTCD->pImage->pComponents[nComponentIndex].nPrecision) - 1.0)) * ((double)(pTileComp->nNBPix));
		}

		// INDEX >>
		if (pImageInfo && pImageInfo->nIndexOn)
		{
			TileInfo *pTileInfo = &pImageInfo->pTile[pTCD->nTCDTileIndex];
			pTileInfo->nNBPix      = pTile->nNBPix;
			pTileInfo->dDistoTile  = pTile->dDistoTile;
			pTileInfo->pdThreshold = (double *)Malloc(pTCP->nLayersCount * sizeof(double));
		}
		// << INDEX

		double adCumDisto[100];  // Fixed quality
		const double c_dK = 1;   // Fixed quality

		for (int nLayerIndex = 0; nLayerIndex < pTCP->nLayersCount; nLayerIndex++)
		{
			double dLow  = dMin;
			double dHigh = dMax;

			int nSuccess = 0;

			// TO DO: Убрать nMaxLen
            int nMaxLen = pTCP->afRates[nLayerIndex] ? (std::min)(((int)ceil(pTCP->afRates[nLayerIndex])), nLen) : nLen;
			double dGoodThreshold = 0;
			double dDistoTarget = pTile->dDistoTile - ((c_dK * dMaxSE) / pow((float)10, pTCP->afDistoRatio[nLayerIndex] / 10));

			if ((pTCP->afRates[nLayerIndex]) || (pCodingParams->nDistoAlloc == 0))
			{
				Tier2 *pTier2 = Tier2_Create(pTCD->pCodecInfo, pTCD->pImage, pCodingParams);

				for (int nIndex = 0; nIndex < 32; nIndex++)
				{
					double dThreshold = (dLow + dHigh) / 2;
					double dDistoAchieved = 0;

					TCD_MakeLayer(pTCD, nLayerIndex, dThreshold, 0);

					if (pCodingParams->nFixedQuality)
					{
						dDistoAchieved = (nLayerIndex == 0) ? pTile->adDistoLayer[0] : (adCumDisto[nLayerIndex - 1] + pTile->adDistoLayer[nLayerIndex]);
						if (dDistoAchieved < dDistoTarget)
						{
							dHigh = dThreshold;
							continue;
						}
						dLow = dThreshold;
					}
					else
					{
						int nL = Tier2_EncodePackets(pTier2, pTCD->nTCDTileIndex, pTile, nLayerIndex + 1, pDst, nMaxLen, pImageInfo);

						if (-999 == nL)
						{
							dLow = dThreshold;
							continue;
						}
						dHigh = dThreshold;
					}

					nSuccess = 1;
					dGoodThreshold = dThreshold;
				}
				Tier2_Destroy(pTier2);
			}
			else
			{
				nSuccess = 1;
				dGoodThreshold = dMin;
			}

			if (!nSuccess)
			{
				return false;
			}

			if (pImageInfo && pImageInfo->nIndexOn)
			{
				pImageInfo->pTile[pTCD->nTCDTileIndex].pdThreshold[nLayerIndex] = dGoodThreshold;
			}
			TCD_MakeLayer(pTCD, nLayerIndex, dGoodThreshold, 1);

			adCumDisto[nLayerIndex] = (nLayerIndex == 0) ? pTile->adDistoLayer[0] : (adCumDisto[nLayerIndex - 1] + pTile->adDistoLayer[nLayerIndex]);
		}

		return true;
	}

	int  TCD_EncodeTile(TCD *pTCD, int nTileIndex, unsigned char *pDst, int nLen, ImageInfo *pImageInfo)
	{
		TileCodingParams *pTCP = &pTCD->pCodingParams->pTCP[0];
		TileCompCodingParams *pTCCP = &pTCP->pTCCP[0];
		Image *pImage = pTCD->pImage;

		pTCD->nTCDTileIndex = nTileIndex;
		pTCD->pTCDTile      = pTCD->pTCDImage->pTiles;
		pTCD->pTCP          = &pTCD->pCodingParams->pTCP[nTileIndex];

		Tile *pTile = pTCD->pTCDTile;
		TileCodingParams *pTCD_TCP = pTCD->pTCP;
		CodingParams *pCodingParams = pTCD->pCodingParams;

		// INDEX >>
		if (pImageInfo && pImageInfo->nIndexOn)
		{
			int nPrecCount = 0;
			TileComp *pTileCompIndex = &pTile->pComponents[0];	/* based on component 0 */
			for (int nIndex = 0; nIndex < pTileCompIndex->nResolutionsCount; nIndex++)
			{
				Resolution *pResolutionIndex = &pTileCompIndex->pResolutions[nIndex];

				pImageInfo->pTile[nTileIndex].anXPrecinctCount[nIndex] = pResolutionIndex->nPrecCountW;
				pImageInfo->pTile[nTileIndex].anYPrecinctCount[nIndex] = pResolutionIndex->nPrecCountH;

				nPrecCount += pResolutionIndex->nPrecCountW * pResolutionIndex->nPrecCountH;

				pImageInfo->pTile[nTileIndex].anPrecinctWidth[nIndex]  = pTCCP->anPrecinctWidth[nIndex];
				pImageInfo->pTile[nTileIndex].anPrecinctHeight[nIndex] = pTCCP->anPrecinctHeight[nIndex];
			}
			pImageInfo->pTile[nTileIndex].pPacket = (PacketInfo *)Malloc(pImageInfo->nCompCount * pImageInfo->nLayersCount * nPrecCount * sizeof(PacketInfo));
		}
		// << INDEX

		//---------------TILE-DATA--------------
		double dEncodingTime = Clock(); // Засекаем сколько потребовалось времени для кодирования тайла

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			int nAdjust = pImage->pComponents[nComponentIndex].nSigned ? 0 : 1 << (pImage->pComponents[nComponentIndex].nPrecision - 1);
			int nOffsetX = CeilDiv(pImage->nXOsiz, pImage->pComponents[nComponentIndex].nXRsiz);
			int nOffsety = CeilDiv(pImage->nYOsiz, pImage->pComponents[nComponentIndex].nYRsiz);

			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			int nTileWidth = pTileComp->nX1 - pTileComp->nX0;
			int nWidth = CeilDiv(pImage->nXsiz - pImage->nXOsiz, pImage->pComponents[nComponentIndex].nXRsiz);

			// Извлекаем данные тайла

			if (pTCD_TCP->pTCCP[nComponentIndex].nTransformID == 1)
			{
				for (int nY = pTileComp->nY0; nY < pTileComp->nY1; nY++)
				{
					// Иходные данные
					int *pData = &pImage->pComponents[nComponentIndex].pData[(pTileComp->nX0 - nOffsetX) + (nY - nOffsety) * nWidth];
					// Результат
					int *pTileData = &pTileComp->pData[(nY - pTileComp->nY0) * nTileWidth];
					for (int nX = pTileComp->nX0; nX < pTileComp->nX1; nX++)
					{
						*pTileData++ = *pData++ - nAdjust;
					}
				}
			}
			else if (pTCD_TCP->pTCCP[nComponentIndex].nTransformID == 0)
			{
				for (int nY = pTileComp->nY0; nY < pTileComp->nY1; nY++)
				{
					// Иходные данные
					int *pData = &pImage->pComponents[nComponentIndex].pData[(pTileComp->nX0 - nOffsetX) + (nY - nOffsety) * nWidth];
					// Результат
					int *pTileData = &pTileComp->pData[(nY - pTileComp->nY0) * nTileWidth];
					for (int nX = pTileComp->nX0; nX < pTileComp->nX1; nX++)
					{
						*pTileData++ = (*pData++ - nAdjust) << 13;
					}
				}
			}
		}

		//----------------MCT-------------------
		if (pTCD_TCP->nMCT)
		{
			int nSamplesCount = (pTile->pComponents[0].nX1 - pTile->pComponents[0].nX0) * (pTile->pComponents[0].nY1 - pTile->pComponents[0].nY0);
			if (pTCD_TCP->pTCCP[0].nTransformID == 0)
			{
				ForwardICT(pTile->pComponents[0].pData, pTile->pComponents[1].pData, pTile->pComponents[2].pData, nSamplesCount);
			}
			else
			{
				ForwardRCT(pTile->pComponents[0].pData, pTile->pComponents[1].pData, pTile->pComponents[2].pData, nSamplesCount);
			}
		}

		//----------------DWT---------------------

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			if (pTCD_TCP->pTCCP[nComponentIndex].nTransformID == 1)
			{
				DWT_EncodeRev(pTileComp);
			}
			else if (pTCD_TCP->pTCCP[nComponentIndex].nTransformID == 0)
			{
				DWT_EncodeIrr(pTileComp);
			}
		}
		//------------------TIER1-----------------
		Tier1 *pTier1 = Tier1_Create(pTCD->pCodecInfo);
		Tier1_EncodeCodeBlocks(pTier1, pTile, pTCD_TCP);
		Tier1_Destroy(pTier1);

		//-----------RATE-ALLOCATE----------------

		// INDEX 
		if (pImageInfo)
		{
			pImageInfo->nIndexWrite = 0;
		}
		if (pCodingParams->nDistoAlloc || pCodingParams->nFixedQuality)
		{
			// Normal Rate/Distortion allocation
			TCD_RateAllocate(pTCD, pDst, nLen, pImageInfo);
		}
		else
		{
			// Fixed layer allocation
			TCD_RateAllocateFixed(pTCD);
		}

		//--------------TIER2---------------------

		// INDEX
		if (pImageInfo)
		{
			pImageInfo->nIndexWrite = 1;
		}

		Tier2 *pTier2 = Tier2_Create(pTCD->pCodecInfo, pImage, pCodingParams);
		int nRes = Tier2_EncodePackets(pTier2, nTileIndex, pTile, pTCD_TCP->nLayersCount, pDst, nLen, pImageInfo);
		Tier2_Destroy(pTier2);

		//---------------CLEAN--------------------

		dEncodingTime = Clock() - dEncodingTime;

		Event_Message(EVT_INFO, "- tile encoded in %f s\n", dEncodingTime);

		// Освобождаем память
		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			Free(pTileComp->pData);
		}

		return nRes;
	}

	bool TCD_DecodeTile(TCD *pTCD, unsigned char *pSrc, int nLen, int nTileIndex)
	{
		int nEOF = 0;

		pTCD->nTCDTileIndex = nTileIndex;
		pTCD->pTCDTile      = &(pTCD->pTCDImage->pTiles[nTileIndex]);
		pTCD->pTCP          = &(pTCD->pCodingParams->pTCP[nTileIndex]);
		Tile *pTile = pTCD->pTCDTile;

		double dDecodeTime = Clock(); // Время, затраченное на декодирование тайла
		Event_Message(EVT_INFO, "tile %d of %d\n", nTileIndex + 1, pTCD->pCodingParams->nXTilesCount * pTCD->pCodingParams->nYTilesCount);

		//--------------TIER2---------------------

		Tier2 *pTier2 = Tier2_Create(pTCD->pCodecInfo, pTCD->pImage, pTCD->pCodingParams);
		int nRes = Tier2_DecodePackets(pTier2, pSrc, nLen, nTileIndex, pTile);
		Tier2_Destroy(pTier2);

		if (nRes == -999)
		{
			nEOF = 1;
			Event_Message(EVT_ERROR, "tcd_decode: incomplete bistream\n");
		}

		//------------------TIER1-----------------

		double dTier1Time = Clock();
		Tier1 *pTier1 = Tier1_Create(pTCD->pCodecInfo);
		Tier1_DecodeCodeBlocks(pTier1, pTile, pTCD->pTCP);
		Tier1_Destroy(pTier1);
		dTier1Time = Clock() - dTier1Time;
		Event_Message(EVT_INFO, "- tiers-1 took %f s\n", dTier1Time);

		//----------------DWT---------------------

		double dDWTTime = Clock();
		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			if (pTCD->pCodingParams->nReduceFactor != 0)
			{
				pTCD->pImage->pComponents[nComponentIndex].nDecodedResCount = pTile->pComponents[nComponentIndex].nResolutionsCount - pTCD->pCodingParams->nReduceFactor - 1;
			}

			if (pTCD->pTCP->pTCCP[nComponentIndex].nTransformID == 1)
			{
				DWT_DecodeRev(pTileComp, pTileComp->nResolutionsCount - 1 - pTCD->pImage->pComponents[nComponentIndex].nDecodedResCount);
			}
			else
			{
				DWT_DecodeIrr(pTileComp, pTileComp->nResolutionsCount - 1 - pTCD->pImage->pComponents[nComponentIndex].nDecodedResCount);
			}

			if (pTile->pComponents[nComponentIndex].nResolutionsCount > 0)
			{
				pTCD->pImage->pComponents[nComponentIndex].nFactorDiv2 = pTile->pComponents[nComponentIndex].nResolutionsCount - (pTCD->pImage->pComponents[nComponentIndex].nDecodedResCount + 1);
			}
		}
		dDWTTime = Clock() - dDWTTime;
		Event_Message(EVT_INFO, "- dwt took %f s\n", dDWTTime);

		//----------------MCT-------------------

		if (pTCD->pTCP->nMCT)
		{
			if (pTCD->pTCP->pTCCP[0].nTransformID == 1)
			{
				InverseRCT(pTile->pComponents[0].pData, pTile->pComponents[1].pData, pTile->pComponents[2].pData, (pTile->pComponents[0].nX1 - pTile->pComponents[0].nX0) * (pTile->pComponents[0].nY1 - pTile->pComponents[0].nY0));
			}
			else
			{
				InverseICT(pTile->pComponents[0].pData, pTile->pComponents[1].pData, pTile->pComponents[2].pData, (pTile->pComponents[0].nX1 - pTile->pComponents[0].nX0) * (pTile->pComponents[0].nY1 - pTile->pComponents[0].nY0));
			}
		}

		//---------------TILE-DATA--------------

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			TileComp *pTileComp = &pTile->pComponents[nComponentIndex];
			Resolution *pResolution = &pTileComp->pResolutions[pTCD->pImage->pComponents[nComponentIndex].nDecodedResCount];

			int nAdjust = pTCD->pImage->pComponents[nComponentIndex].nSigned ? 0 : 1 << (pTCD->pImage->pComponents[nComponentIndex].nPrecision - 1);
			int nMin    = pTCD->pImage->pComponents[nComponentIndex].nSigned ? -(1 << (pTCD->pImage->pComponents[nComponentIndex].nPrecision - 1)) : 0;
			int nMax    = pTCD->pImage->pComponents[nComponentIndex].nSigned ? (1 << (pTCD->pImage->pComponents[nComponentIndex].nPrecision - 1)) - 1 : (1 << pTCD->pImage->pComponents[nComponentIndex].nPrecision) - 1;

			int nTileWidth = pTileComp->nX1 - pTileComp->nX0;
			int nWidth = pTCD->pImage->pComponents[nComponentIndex].nWidth;

			int offset_x = CeilDivPow2(pTCD->pImage->pComponents[nComponentIndex].nXOsiz, pTCD->pImage->pComponents[nComponentIndex].nFactorDiv2);
			int offset_y = CeilDivPow2(pTCD->pImage->pComponents[nComponentIndex].nYOsiz, pTCD->pImage->pComponents[nComponentIndex].nFactorDiv2);

			for (int nY = pResolution->nY0; nY < pResolution->nY1; nY++)
			{
				for (int nX = pResolution->nX0; nX < pResolution->nX1; nX++)
				{
					int nValue = 0;
					float fTemp = (float)((pTileComp->pData[nX - pResolution->nX0 + (nY - pResolution->nY0) * nTileWidth]) / 8192.0);

					if (pTCD->pTCP->pTCCP[nComponentIndex].nTransformID == 1)
					{
						nValue = pTileComp->pData[nX - pResolution->nX0 + (nY - pResolution->nY0) * nTileWidth];
					}
					else
					{
						int nTemp2 = ((int)(floor(fabs(fTemp)))) + ((int)floor(fabs(fTemp * 2)) % 2);
						nValue = ((fTemp < 0) ? -nTemp2 : nTemp2);
					}
					nValue += nAdjust;

					pTCD->pImage->pComponents[nComponentIndex].pData[(nX - offset_x) + (nY - offset_y) * nWidth] = Clamp(nValue, nMin, nMax);
				}
			}
		}

		dDecodeTime = Clock() - dDecodeTime;
		Event_Message(EVT_INFO, "- tile decoded in %f s\n", dDecodeTime);

		for (int nComponentIndex = 0; nComponentIndex < pTile->nComponentsCount; nComponentIndex++)
		{
			Free(pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].pData);
			pTCD->pTCDImage->pTiles[nTileIndex].pComponents[nComponentIndex].pData = NULL;
		}

		if (nEOF)
		{
			return false;
		}

		return true;
	}
}

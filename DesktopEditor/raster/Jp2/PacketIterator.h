#pragma once

#include "Types.h"
#include "Utils.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	static bool PI_NextLRCP(PacketIterator *pPI)
	{
		PacketComponent  *pComponent  = NULL;
		PacketResolution *pResolution = NULL;
		long nIndex = 0;

		if (!pPI->nFirst)
		{
			pComponent  = &pPI->pComponents[pPI->nIndexComponent];
			pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
			goto LABEL_SKIP;
		}
		else
		{
			pPI->nFirst = 0;
		}

		for (pPI->nIndexLayer = 0; pPI->nIndexLayer < pPI->oPOC.nLYEpoc; pPI->nIndexLayer++)
		{
			for (pPI->nIndexResolution = pPI->oPOC.nRSpoc; pPI->nIndexResolution < pPI->oPOC.nREpoc; pPI->nIndexResolution++)
			{
				for (pPI->nIndexComponent = pPI->oPOC.nCSpoc; pPI->nIndexComponent < pPI->oPOC.nCEpoc; pPI->nIndexComponent++)
				{
					pComponent = &pPI->pComponents[pPI->nIndexComponent];
					if (pPI->nIndexResolution >= pComponent->nResolutionsCount)
					{
						continue;
					}
					pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
					for (pPI->nIndexPrecinct = 0; pPI->nIndexPrecinct < pResolution->nWidth * pResolution->nHeight; pPI->nIndexPrecinct++)
					{
						nIndex = pPI->nIndexLayer * pPI->nStepLayer + pPI->nIndexResolution * pPI->nStepResolution + pPI->nIndexComponent * pPI->nStepComponent + pPI->nIndexPrecinct * pPI->nStepPrecinct;
						if (!pPI->pInclude[nIndex])
						{
							pPI->pInclude[nIndex] = 1;
							return true;
						}
					LABEL_SKIP:;
					}
				}
			}
		}

		return false;
	}
	static bool PI_NextRLCP(PacketIterator *pPI)
	{
		PacketComponent  *pComponent  = NULL;
		PacketResolution *pResolution = NULL;
		long nIndex = 0;

		if (!pPI->nFirst)
		{
			pComponent  = &pPI->pComponents[pPI->nIndexComponent];
			pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
			goto LABEL_SKIP;
		}
		else
		{
			pPI->nFirst = 0;
		}

		for (pPI->nIndexResolution = pPI->oPOC.nRSpoc; pPI->nIndexResolution < pPI->oPOC.nREpoc; pPI->nIndexResolution++)
		{
			for (pPI->nIndexLayer = 0; pPI->nIndexLayer < pPI->oPOC.nLYEpoc; pPI->nIndexLayer++)
			{
				for (pPI->nIndexComponent = pPI->oPOC.nCSpoc; pPI->nIndexComponent < pPI->oPOC.nCEpoc; pPI->nIndexComponent++)
				{
					pComponent = &pPI->pComponents[pPI->nIndexComponent];
					if (pPI->nIndexResolution >= pComponent->nResolutionsCount)
					{
						continue;
					}
					pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
					for (pPI->nIndexPrecinct = 0; pPI->nIndexPrecinct < pResolution->nWidth * pResolution->nHeight; pPI->nIndexPrecinct++)
					{
						nIndex = pPI->nIndexLayer * pPI->nStepLayer + pPI->nIndexResolution * pPI->nStepResolution + pPI->nIndexComponent * pPI->nStepComponent + pPI->nIndexPrecinct * pPI->nStepPrecinct;
						if (!pPI->pInclude[nIndex])
						{
							pPI->pInclude[nIndex] = 1;
							return true;
						}
					LABEL_SKIP:;
					}
				}
			}
		}

		return false;
	}
	static bool PI_NextRPCL(PacketIterator *pPI)
	{
		PacketComponent  *pComponent  = NULL;
		PacketResolution *pResolution = NULL;
		long nIndex = 0;
		int nLevel, nTileResX0, nTileResY0, nTileResX1, nTileResY1, nResPX, nResPY, nPCR_i, nPCR_j;

		if (!pPI->nFirst)
		{
			goto LABEL_SKIP;
		}
		else
		{
			pPI->nFirst = 0;
			pPI->nDx    = 0;
			pPI->nDy    = 0;

			for (int nComponentIndex = 0; nComponentIndex < pPI->nComponentsCount; nComponentIndex++)
			{
				pComponent = &pPI->pComponents[nComponentIndex];
				for (int nResolutionIndex = 0; nResolutionIndex < pComponent->nResolutionsCount; nResolutionIndex++)
				{
					pResolution = &pComponent->pResolutions[nResolutionIndex];
					int nDx = pComponent->nDx * (1 << (pResolution->nDx + pComponent->nResolutionsCount - 1 - nResolutionIndex));
					int nDy = pComponent->nDy * (1 << (pResolution->nDy + pComponent->nResolutionsCount - 1 - nResolutionIndex));
                    pPI->nDx = !pPI->nDx ? nDx : (std::min)(pPI->nDx, nDx);
                    pPI->nDy = !pPI->nDy ? nDy : (std::min)(pPI->nDy, nDy);
				}
			}
		}

		for (pPI->nIndexResolution = pPI->oPOC.nRSpoc; pPI->nIndexResolution < pPI->oPOC.nREpoc; pPI->nIndexResolution++)
		{
			for (pPI->nY = pPI->nTileY0; pPI->nY < pPI->nTileY1; pPI->nY += pPI->nDy - (pPI->nY % pPI->nDy))
			{
				for (pPI->nX = pPI->nTileX0; pPI->nX < pPI->nTileX1; pPI->nX += pPI->nDx - (pPI->nX % pPI->nDx))
				{
					for (pPI->nIndexComponent = pPI->oPOC.nCSpoc; pPI->nIndexComponent < pPI->oPOC.nCEpoc; pPI->nIndexComponent++)
					{
						pComponent = &pPI->pComponents[pPI->nIndexComponent];
						if (pPI->nIndexResolution >= pComponent->nResolutionsCount)
						{
							continue;
						}
						pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
						nLevel = pComponent->nResolutionsCount - 1 - pPI->nIndexResolution;

						nTileResX0 = CeilDiv(pPI->nTileX0, pComponent->nDx << nLevel);
						nTileResY0 = CeilDiv(pPI->nTileY0, pComponent->nDy << nLevel);
						nTileResX1 = CeilDiv(pPI->nTileX1, pComponent->nDx << nLevel);
						nTileResY1 = CeilDiv(pPI->nTileY1, pComponent->nDy << nLevel);

						nResPX = pResolution->nDx + nLevel;
						nResPY = pResolution->nDy + nLevel;

						if ((!(pPI->nX % (pComponent->nDx << nResPX) == 0) || (pPI->nX == pPI->nTileX0 && (nTileResX0 << nLevel) % (1 << nResPX))))
						{
							continue;
						}
						// TO DO: Проверить здесь последнее деление на (1 << nResPX)
						if ((!(pPI->nY % (pComponent->nDy << nResPY) == 0) || (pPI->nY == pPI->nTileY0 && (nTileResY0 << nLevel) % (1 << nResPX))))
						{
							continue;
						}

						// TO DO: ???
						if ((pResolution->nWidth == 0) || (pResolution->nWidth == 0))
							continue;

						if ((nTileResX0 == nTileResX1) || (nTileResY0 == nTileResY1))
							continue;

						nPCR_i = FloorDivPow2(CeilDiv(pPI->nX, pComponent->nDx << nLevel), pResolution->nDx) - FloorDivPow2(nTileResX0, pResolution->nDx);
						nPCR_j = FloorDivPow2(CeilDiv(pPI->nY, pComponent->nDy << nLevel), pResolution->nDy) - FloorDivPow2(nTileResY0, pResolution->nDy);
						pPI->nIndexPrecinct = nPCR_i + nPCR_j * pResolution->nWidth;

						for (pPI->nIndexLayer = 0; pPI->nIndexLayer < pPI->oPOC.nLYEpoc; pPI->nIndexLayer++)
						{
							nIndex = pPI->nIndexLayer * pPI->nStepLayer + pPI->nIndexResolution * pPI->nStepResolution + pPI->nIndexComponent * pPI->nStepComponent + pPI->nIndexPrecinct * pPI->nStepPrecinct;
							if (!pPI->pInclude[nIndex])
							{
								pPI->pInclude[nIndex] = 1;
								return true;
							}
						LABEL_SKIP:;
						}
					}
				}
			}
		}

		return false;
	}
	static bool PI_NextPCRL(PacketIterator *pPI)
	{
		PacketComponent  *pComponent  = NULL;
		PacketResolution *pResolution = NULL;
		long nIndex = 0;
		int nLevel, nTileResX0, nTileResY0, nTileResX1, nTileResY1, nResPX, nResPY, nPRC_i, nPRC_j;

		if (!pPI->nFirst)
		{
			pComponent = &pPI->pComponents[pPI->nIndexComponent];
			goto LABEL_SKIP;
		}
		else
		{
			pPI->nFirst = 0;
			pPI->nDx    = 0;
			pPI->nDy    = 0;

			for (int nComponentIndex = 0; nComponentIndex < pPI->nComponentsCount; nComponentIndex++)
			{
				pComponent = &pPI->pComponents[nComponentIndex];
				for (int nResolutionIndex = 0; nResolutionIndex < pComponent->nResolutionsCount; nResolutionIndex++)
				{
					pResolution = &pComponent->pResolutions[nResolutionIndex];
					int nDx = pComponent->nDx * (1 << (pResolution->nDx + pComponent->nResolutionsCount - 1 - nResolutionIndex));
					int nDy = pComponent->nDy * (1 << (pResolution->nDy + pComponent->nResolutionsCount - 1 - nResolutionIndex));
                    pPI->nDx = !pPI->nDx ? nDx : (std::min)(pPI->nDx, nDx);
                    pPI->nDy = !pPI->nDy ? nDy : (std::min)(pPI->nDy, nDy);
				}
			}
		}

		for (pPI->nY = pPI->nTileY0; pPI->nY < pPI->nTileY1; pPI->nY += pPI->nDy - (pPI->nY % pPI->nDy))
		{
			for (pPI->nX = pPI->nTileX0; pPI->nX < pPI->nTileX1; pPI->nX += pPI->nDx - (pPI->nX % pPI->nDx))
			{
				for (pPI->nIndexComponent = pPI->oPOC.nCSpoc; pPI->nIndexComponent < pPI->oPOC.nCEpoc; pPI->nIndexComponent++)
				{
					pComponent = &pPI->pComponents[pPI->nIndexComponent];
                    for (pPI->nIndexResolution = pPI->oPOC.nRSpoc; pPI->nIndexResolution < (std::min)(pPI->oPOC.nREpoc, pComponent->nResolutionsCount); pPI->nIndexResolution++)
					{
						pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
						nLevel = pComponent->nResolutionsCount - 1 - pPI->nIndexResolution;

						nTileResX0 = CeilDiv(pPI->nTileX0, pComponent->nDx << nLevel);
						nTileResY0 = CeilDiv(pPI->nTileY0, pComponent->nDy << nLevel);
						nTileResX1 = CeilDiv(pPI->nTileX1, pComponent->nDx << nLevel);
						nTileResY1 = CeilDiv(pPI->nTileY1, pComponent->nDy << nLevel);

						nResPX = pResolution->nDx + nLevel;
						nResPY = pResolution->nDy + nLevel;

						if ((!(pPI->nX % (pComponent->nDx << nResPX) == 0) || (pPI->nX == pPI->nTileX0 && (nTileResX0 << nLevel) % (1 << nResPX))))
						{
							continue;
						}
						// TO DO: Проверить здесь последнее деление на (1 << nResPX)
						if ((!(pPI->nY % (pComponent->nDy << nResPY) == 0) || (pPI->nY == pPI->nTileY0 && (nTileResY0 << nLevel) % (1 << nResPX))))
						{
							continue;
						}

						// TO DO: ???
						if ((pResolution->nWidth == 0) || (pResolution->nWidth == 0))
							continue;

						if ((nTileResX0 == nTileResX1) || (nTileResY0 == nTileResY1))
							continue;

						nPRC_i = FloorDivPow2(CeilDiv(pPI->nX, pComponent->nDx << nLevel), pResolution->nDx) - FloorDivPow2(nTileResX0, pResolution->nDx);
						nPRC_j = FloorDivPow2(CeilDiv(pPI->nY, pComponent->nDy << nLevel), pResolution->nDy) - FloorDivPow2(nTileResY0, pResolution->nDy);
						pPI->nIndexPrecinct = nPRC_i + nPRC_j * pResolution->nWidth;

						for (pPI->nIndexLayer = 0; pPI->nIndexLayer < pPI->oPOC.nLYEpoc; pPI->nIndexLayer++)
						{
							nIndex = pPI->nIndexLayer * pPI->nStepLayer + pPI->nIndexResolution * pPI->nStepResolution + pPI->nIndexComponent * pPI->nStepComponent + pPI->nIndexPrecinct * pPI->nStepPrecinct;
							if (!pPI->pInclude[nIndex])
							{
								pPI->pInclude[nIndex] = 1;
								return true;
							}
						LABEL_SKIP:;
						}
					}
				}
			}
		}

		return false;
	}
	static bool PI_NextCPRL(PacketIterator *pPI)
	{
		PacketComponent  *pComponent  = NULL;
		PacketResolution *pResolution = NULL;
		long nIndex = 0;
		int nLevel, nTileResX0, nTileResY0, nTileResX1, nTileResY1, nResPX, nResPY, nPRC_i, nPRC_j;

		if (!pPI->nFirst)
		{
			pComponent = &pPI->pComponents[pPI->nIndexComponent];
			goto LABEL_SKIP;
		}
		else
		{
			pPI->nFirst = 0;
		}

		for (pPI->nIndexComponent = pPI->oPOC.nCSpoc; pPI->nIndexComponent < pPI->oPOC.nCEpoc; pPI->nIndexComponent++)
		{
			pComponent = &pPI->pComponents[pPI->nIndexComponent];
			pPI->nDx = 0;
			pPI->nDy = 0;

			for (int nResolutionIndex = 0; nResolutionIndex < pComponent->nResolutionsCount; nResolutionIndex++)
			{
				pResolution = &pComponent->pResolutions[nResolutionIndex];
				int nDx = pComponent->nDx * (1 << (pResolution->nDx + pComponent->nResolutionsCount - 1 - nResolutionIndex));
				int nDy = pComponent->nDy * (1 << (pResolution->nDy + pComponent->nResolutionsCount - 1 - nResolutionIndex));
                pPI->nDx = !pPI->nDx ? nDx : (std::min)(pPI->nDx, nDx);
                pPI->nDy = !pPI->nDy ? nDy : (std::min)(pPI->nDy, nDy);
			}
			for (pPI->nY = pPI->nTileY0; pPI->nY < pPI->nTileY1; pPI->nY += pPI->nDy - (pPI->nY % pPI->nDy))
			{
				for (pPI->nX = pPI->nTileX0; pPI->nX < pPI->nTileX1; pPI->nX += pPI->nDx - (pPI->nX % pPI->nDx))
				{
                    for (pPI->nIndexResolution = pPI->oPOC.nRSpoc; pPI->nIndexResolution < (std::min)(pPI->oPOC.nREpoc, pComponent->nResolutionsCount); pPI->nIndexResolution++)
					{
						pResolution = &pComponent->pResolutions[pPI->nIndexResolution];
						nLevel = pComponent->nResolutionsCount - 1 - pPI->nIndexResolution;

						nTileResX0 = CeilDiv(pPI->nTileX0, pComponent->nDx << nLevel);
						nTileResY0 = CeilDiv(pPI->nTileY0, pComponent->nDy << nLevel);
						nTileResX1 = CeilDiv(pPI->nTileX1, pComponent->nDx << nLevel);
						nTileResY1 = CeilDiv(pPI->nTileY1, pComponent->nDy << nLevel);
						nResPX = pResolution->nDx + nLevel;
						nResPY = pResolution->nDy + nLevel;

						if ((!(pPI->nX % (pComponent->nDx << nResPX) == 0) || (pPI->nX == pPI->nTileX0 && (nTileResX0 << nLevel) % (1 << nResPX))))
						{
							continue;
						}
						// TO DO: Проверить здесь последнее деление на (1 << nResPX)
						if ((!(pPI->nY % (pComponent->nDy << nResPY) == 0) || (pPI->nY == pPI->nTileY0 && (nTileResY0 << nLevel) % (1 << nResPX))))
						{
							continue;
						}

						// TO DO: ???
						if ((pResolution->nWidth == 0) || (pResolution->nWidth == 0))
							continue;

						if ((nTileResX0 == nTileResX1) || (nTileResY0 == nTileResY1))
							continue;

						nPRC_i = FloorDivPow2(CeilDiv(pPI->nX, pComponent->nDx << nLevel), pResolution->nDx) - FloorDivPow2(nTileResX0, pResolution->nDx);
						nPRC_j = FloorDivPow2(CeilDiv(pPI->nY, pComponent->nDy << nLevel), pResolution->nDy) - FloorDivPow2(nTileResY0, pResolution->nDy);
						pPI->nIndexPrecinct = nPRC_i + nPRC_j * pResolution->nWidth;

						for (pPI->nIndexLayer = 0; pPI->nIndexLayer < pPI->oPOC.nLYEpoc; pPI->nIndexLayer++)
						{
							nIndex = pPI->nIndexLayer * pPI->nStepLayer + pPI->nIndexResolution * pPI->nStepResolution + pPI->nIndexComponent * pPI->nStepComponent + pPI->nIndexPrecinct * pPI->nStepPrecinct;
							if (!pPI->pInclude[nIndex])
							{
								pPI->pInclude[nIndex] = 1;
								return true;
							}
						LABEL_SKIP:;
						}
					}
				}
			}
		}

		return false;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	void	PI_Destroy(PacketIterator *pPI, CodingParams *pCodingParams, int nTileIndex)
	{
		TileCodingParams *tcp = &pCodingParams->pTCP[nTileIndex];
		if (pPI)
		{
			for (int nIndexPI = 0; nIndexPI < tcp->nPOCsCount + 1; nIndexPI++)
			{
				if (pPI[nIndexPI].pComponents)
				{
					for (int nComponentIndex = 0; nComponentIndex < pPI->nComponentsCount; nComponentIndex++)
					{
						PacketComponent *pComponent = &pPI[nIndexPI].pComponents[nComponentIndex];
						Free(pComponent->pResolutions);
					}
					Free(pPI[nIndexPI].pComponents);
				}
			}
			Free(pPI->pInclude);
			Free(pPI);
		}
	}
	PacketIterator *PI_Create(Image *pImage, CodingParams *pCodingParams, int nTileIndex)
	{
		TileCodingParams *pTCP = &pCodingParams->pTCP[nTileIndex];
		TileCompCodingParams *pTCCP = NULL;
		size_t nArraySize = (pTCP->nPOCsCount + 1) * sizeof(PacketIterator);
		PacketIterator *pPI = (PacketIterator *)Malloc(nArraySize);
		if (!pPI)
		{
			// TO DO: Error
			return NULL;
		}

		for (int nIndexPI = 0; nIndexPI < pTCP->nPOCsCount + 1; nIndexPI++)
		{
			int nMaxResolution = 0;
			int nMaxPrecinct   = 0;
			int nP = nTileIndex % pCodingParams->nXTilesCount;
			int nQ = nTileIndex / pCodingParams->nXTilesCount;

            pPI[nIndexPI].nTileX0 = (std::max)(pCodingParams->nXTOsiz + nP * pCodingParams->nXTsiz, pImage->nXOsiz);
            pPI[nIndexPI].nTileY0 = (std::max)(pCodingParams->nYTOsiz + nQ * pCodingParams->nYTsiz, pImage->nYOsiz);
            pPI[nIndexPI].nTileX1 = (std::min)(pCodingParams->nXTOsiz + (nP + 1) * pCodingParams->nXTsiz, pImage->nXsiz);
            pPI[nIndexPI].nTileY1 = (std::min)(pCodingParams->nYTOsiz + (nQ + 1) * pCodingParams->nYTsiz, pImage->nYsiz);
			pPI[nIndexPI].nComponentsCount = pImage->nCsiz;

			nArraySize = pImage->nCsiz * sizeof(PacketComponent);
			pPI[nIndexPI].pComponents = (PacketComponent *)Malloc(nArraySize);
			if (!pPI[nIndexPI].pComponents)
			{
				// TO DO: Error
				PI_Destroy(pPI, pCodingParams, nTileIndex);
				return NULL;
			}
			memset(pPI[nIndexPI].pComponents, 0, nArraySize);

			for (int nComponentIndex = 0; nComponentIndex < pPI->nComponentsCount; nComponentIndex++)
			{
				PacketComponent *pComponent = &pPI[nIndexPI].pComponents[nComponentIndex];
				pTCCP = &pTCP->pTCCP[nComponentIndex];

				pComponent->nDx = pImage->pComponents[nComponentIndex].nXRsiz;
				pComponent->nDy = pImage->pComponents[nComponentIndex].nYRsiz;
				pComponent->nResolutionsCount = pTCCP->nResolutionsCount;

				nArraySize = pComponent->nResolutionsCount * sizeof(PacketResolution);
				pComponent->pResolutions = (PacketResolution *)Malloc(nArraySize);
				if (!pComponent->pResolutions)
				{
					// TO DO: Error
					PI_Destroy(pPI, pCodingParams, nTileIndex);
					return NULL;
				}

				int nTileCompX0 = CeilDiv(pPI->nTileX0, pComponent->nDx);
				int nTileCompY0 = CeilDiv(pPI->nTileY0, pComponent->nDy);
				int nTileCompX1 = CeilDiv(pPI->nTileX1, pComponent->nDx);
				int nTileCompY1 = CeilDiv(pPI->nTileY1, pComponent->nDy);

				if (pComponent->nResolutionsCount > nMaxResolution)
				{
					nMaxResolution = pComponent->nResolutionsCount;
				}

				for (int nResolutionIndex = 0; nResolutionIndex < pComponent->nResolutionsCount; nResolutionIndex++)
				{
					PacketResolution *pResolution = &pComponent->pResolutions[nResolutionIndex];
					if (pTCCP->nCodingStyle & J2K_CCP_CSTY_PRT)
					{
						pResolution->nDx = pTCCP->anPrecinctWidth[nResolutionIndex];
						pResolution->nDy = pTCCP->anPrecinctHeight[nResolutionIndex];
					}
					else
					{
						pResolution->nDx = 15;
						pResolution->nDy = 15;
					}
					int nLevel = pComponent->nResolutionsCount - 1 - nResolutionIndex;

					int nResX0 = CeilDivPow2(nTileCompX0, nLevel);
					int nResY0 = CeilDivPow2(nTileCompY0, nLevel);
					int nResX1 = CeilDivPow2(nTileCompX1, nLevel);
					int nResY1 = CeilDivPow2(nTileCompY1, nLevel);

					int nPrecX0 = FloorDivPow2(nResX0, pResolution->nDx) << pResolution->nDx;
					int nPrecY0 = FloorDivPow2(nResY0, pResolution->nDy) << pResolution->nDy;
					int nPrecX1 = CeilDivPow2(nResX1, pResolution->nDx) << pResolution->nDx;
					int nPrecY1 = CeilDivPow2(nResY1, pResolution->nDy) << pResolution->nDy;

					pResolution->nWidth  = (nResX0 == nResX1) ? 0 : ((nPrecX1 - nPrecX0) >> pResolution->nDx);
					pResolution->nHeight = (nResY0 == nResY1) ? 0 : ((nPrecY1 - nPrecY0) >> pResolution->nDy);

					if (pResolution->nWidth * pResolution->nHeight > nMaxPrecinct)
					{
						nMaxPrecinct = pResolution->nWidth * pResolution->nHeight;
					}

				}
			}

			pTCCP = &pTCP->pTCCP[0];
			pPI[nIndexPI].nStepPrecinct   = 1;
			pPI[nIndexPI].nStepComponent  = nMaxPrecinct * pPI[nIndexPI].nStepPrecinct;
			pPI[nIndexPI].nStepResolution = pImage->nCsiz * pPI[nIndexPI].nStepComponent;
			pPI[nIndexPI].nStepLayer      = nMaxResolution * pPI[nIndexPI].nStepResolution;

			if (nIndexPI == 0)
			{
				nArraySize = pImage->nCsiz * nMaxResolution * pTCP->nLayersCount * nMaxPrecinct * sizeof(short int);
				pPI[nIndexPI].pInclude = (short int *)Malloc(nArraySize);
				if (!pPI[nIndexPI].pInclude)
				{
					// TO DO: Error
					PI_Destroy(pPI, pCodingParams, nTileIndex);
					return NULL;
				}
			}
			else
			{
				pPI[nIndexPI].pInclude = pPI[nIndexPI - 1].pInclude;
			}

			if (pTCP->nUsePOC == 0)
			{
				pPI[nIndexPI].nFirst       = 1;
				pPI[nIndexPI].oPOC.nRSpoc  = 0;
				pPI[nIndexPI].oPOC.nCSpoc  = 0;
				pPI[nIndexPI].oPOC.nLYEpoc = pTCP->nLayersCount;
				pPI[nIndexPI].oPOC.nREpoc  = nMaxResolution;
				pPI[nIndexPI].oPOC.nCEpoc  = pImage->nCsiz;
				pPI[nIndexPI].oPOC.ePpoc   = pTCP->eProgOrder;
			}
			else
			{
				pPI[nIndexPI].nFirst       = 1;
				pPI[nIndexPI].oPOC.nRSpoc  = pTCP->aoPOC[nIndexPI].nRSpoc;
				pPI[nIndexPI].oPOC.nCSpoc  = pTCP->aoPOC[nIndexPI].nCSpoc;
				pPI[nIndexPI].oPOC.nLYEpoc = pTCP->aoPOC[nIndexPI].nLYEpoc;
				pPI[nIndexPI].oPOC.nREpoc  = pTCP->aoPOC[nIndexPI].nREpoc;
				pPI[nIndexPI].oPOC.nCEpoc  = pTCP->aoPOC[nIndexPI].nCEpoc;
				pPI[nIndexPI].oPOC.ePpoc   = pTCP->aoPOC[nIndexPI].ePpoc;
			}
		}

		return pPI;
	}


	bool            PI_Next(PacketIterator *pPI)
	{
		switch (pPI->oPOC.ePpoc)
		{
			case poLRCP:
			return PI_NextLRCP(pPI);
			case poRLCP:
			return PI_NextRLCP(pPI);
			case poRPCL:
			return PI_NextRPCL(pPI);
			case poPCRL:
			return PI_NextPCRL(pPI);
			case poCPRL:
			return PI_NextCPRL(pPI);
			case poUnknown:
			return false;
		}

		return false;
	}
}

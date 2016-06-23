#pragma once

#include "Types.h"
#include "Utils.h"
#include <math.h>

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
#define S(i) pA[(i) * 2]
#define D(i) pA[(1 + (i) * 2)]
#define S_(i) ( (i) < 0 ? S(0) : ( (i) >= nSn ? S( nSn - 1 ) : S(i) ) )
#define D_(i) ( (i) < 0 ? D(0) : ( (i) >= nDn ? D( nDn - 1 ) : D(i) ) )
	/* new */
#define SS_(i) ( (i) < 0 ? S(0) : ( (i) >= nDn ? S( nDn - 1 ) : S(i) ) )
#define DD_(i) ( (i) < 0 ? D(0) : ( (i) >= nSn ? D( nSn - 1 ) : D(i) ) )

	//-------------------------------------------------------------------------------------------------------------------------------

	// Таблицы норм для обратимого 5-3 веэвлет-преобразования
	static const double c_aDWT_NormsRev[4][10] =
	{
		{ 1.000, 1.500, 2.750, 5.375, 10.68, 21.34, 42.67, 85.33, 170.7, 341.3 },
		{ 1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9 },
		{ 1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9 },
		{ .7186, .9218, 1.586, 3.043, 6.019, 12.01, 24.00, 47.97, 95.93 }
	};

	// Таблицы норм для необратимого 9-7 веэвлет-преобразования
	static const double c_aDWT_NormsIrr[4][10] =
	{
		{ 1.000, 1.965, 4.177, 8.403, 16.90, 33.84, 67.69, 135.3, 270.6, 540.9 },
		{ 2.022, 3.989, 8.355, 17.04, 34.27, 68.63, 137.3, 274.6, 549.0 },
		{ 2.022, 3.989, 8.355, 17.04, 34.27, 68.63, 137.3, 274.6, 549.0 },
		{ 2.080, 3.865, 8.307, 17.18, 34.71, 69.59, 139.3, 278.6, 557.2 }
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	//  Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------

	static void DWT_DeinterleaveHor(int *pA, int *pB, int nDn, int nSn, int nCase)
	{
		for (int nIndex = 0; nIndex < nSn; nIndex++)
			pB[nIndex] = pA[2 * nIndex + nCase];
		for (int nIndex = 0; nIndex < nDn; nIndex++)
			pB[nSn + nIndex] = pA[(2 * nIndex + 1 - nCase)];
	}
	static void DWT_DeinterleaveVer(int *pA, int *pB, int nDn, int nSn, int nW, int nCase)
	{
		for (int nIndex = 0; nIndex < nSn; nIndex++)
			pB[nIndex * nW] = pA[2 * nIndex + nCase];
		for (int nIndex = 0; nIndex < nDn; nIndex++)
			pB[(nSn + nIndex) * nW] = pA[(2 * nIndex + 1 - nCase)];
	}

	static void DWT_InterleaveHor(int *pA, int *pB, int nDn, int nSn, int nCase)
	{
		int *pAi = pA;
		int *pBi = pB + nCase;
		for (int nIndex = 0; nIndex < nSn; nIndex++)
		{
			*pBi = *pAi;
			pBi += 2;
			pAi++;
		}
		pAi = pA + nSn;
		pBi = pB + 1 - nCase;
		for (int nIndex = 0; nIndex < nDn; nIndex++)
		{
			*pBi = *pAi;
			pBi += 2;
			pAi++;
		}
	}

	static void DWT_InterleaveVer(int *pA, int *pB, int nDn, int nSn, int nW, int nCase)
	{
		int *pAi = pA;
		int *pBi = pB + nCase;
		for (int nIndex = 0; nIndex < nSn; nIndex++)
		{
			*pBi = *pAi;
			pBi += 2;
			pAi += nW;
		}
		pAi = pA + (nSn * nW);
		pBi = pB + 1 - nCase;
		for (int nIndex = 0; nIndex < nDn; nIndex++)
		{
			*pBi = *pAi;
			pBi += 2;
			pAi += nW;
		}
	}


	static void DWT_ForwardRev1D(int *pA, int nDn, int nSn, int nCase)
	{
		if (!nCase)
		{
			if ((nDn > 0) || (nSn > 1))
			{
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) -= (S_(nIndex) + S_(nIndex + 1)) >> 1;
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) += (D_(nIndex - 1) + D_(nIndex) + 2) >> 2;
			}
		}
		else
		{
			if (!nSn && nDn == 1)
				S(0) *= 2;
			else
			{
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) -= (DD_(nIndex) + DD_(nIndex - 1)) >> 1;
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) += (SS_(nIndex) + SS_(nIndex + 1) + 2) >> 2;
			}
		}
	}

	static void DWT_InverseRev1D(int *pA, int nDn, int nSn, int nCase)
	{
		if (!nCase)
		{
			if ((nDn > 0) || (nSn > 1))
			{
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) -= (D_(nIndex - 1) + D_(nIndex) + 2) >> 2;
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) += (S_(nIndex) + S_(nIndex + 1)) >> 1;
			}
		}
		else
		{
			if (!nSn && nDn == 1)
				S(0) /= 2;
			else
			{
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) -= (SS_(nIndex) + SS_(nIndex + 1) + 2) >> 2;
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) += (DD_(nIndex) + DD_(nIndex - 1)) >> 1;
			}
		}
	}

	static void DWT_ForwardIrr1D(int *pA, int nDn, int nSn, int nCase)
	{
		if (!nCase)
		{
			if ((nDn > 0) || (nSn > 1))
			{
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) -= FixedMult(S_(nIndex) + S_(nIndex + 1), 12993);
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) -= FixedMult(D_(nIndex - 1) + D_(nIndex), 434);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) += FixedMult(S_(nIndex) + S_(nIndex + 1), 7233);
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) += FixedMult(D_(nIndex - 1) + D_(nIndex), 3633);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) = FixedMult(D(nIndex), 5038); // 5038
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) = FixedMult(S(nIndex), 6659); // 6660
			}
		}
		else
		{
			if ((nSn > 0) || (nDn > 1))
			{
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) -= FixedMult(DD_(nIndex) + DD_(nIndex - 1), 12993);
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) -= FixedMult(SS_(nIndex) + SS_(nIndex + 1), 434);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) += FixedMult(DD_(nIndex) + DD_(nIndex - 1), 7233);
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) += FixedMult(SS_(nIndex) + SS_(nIndex + 1), 3633);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) = FixedMult(S(nIndex), 5038); // 5038
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) = FixedMult(D(nIndex), 6659); // 6660
			}
		}
	}

	static void DWT_InverseIrr1D(int *pA, int nDn, int nSn, int nCase)
	{
		if (!nCase)
		{
			if ((nDn > 0) || (nSn > 1))
			{
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) = FixedMult(S(nIndex), 10078); // 10076
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) = FixedMult(D(nIndex), 13318); // 13320
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) -= FixedMult(D_(nIndex - 1) + D_(nIndex), 3633);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) -= FixedMult(S_(nIndex) + S_(nIndex + 1), 7233);
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					S(nIndex) += FixedMult(D_(nIndex - 1) + D_(nIndex), 434);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					D(nIndex) += FixedMult(S_(nIndex) + S_(nIndex + 1), 12994); // 12993
			}
		}
		else
		{
			if ((nSn > 0) || (nDn > 1))
			{
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) = FixedMult(D(nIndex), 10078); // 10076
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) = FixedMult(S(nIndex), 13318); // 13320
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) -= FixedMult(SS_(nIndex) + SS_(nIndex + 1), 3633);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) -= FixedMult(DD_(nIndex) + DD_(nIndex - 1), 7233);
				for (int nIndex = 0; nIndex < nSn; nIndex++)
					D(nIndex) += FixedMult(SS_(nIndex) + SS_(nIndex + 1), 434);
				for (int nIndex = 0; nIndex < nDn; nIndex++)
					S(nIndex) += FixedMult(DD_(nIndex) + DD_(nIndex - 1), 12994); // 12993
			}
		}
	}

	static void DWT_EncodeStepsize(int nStepsize, int nBPsCount, QuantStepSize *pBandIndexStepsize)
	{
		int nP = FloorLog2(nStepsize) - 13;
		int nN = 11 - FloorLog2(nStepsize);
		pBandIndexStepsize->nMantissa = (nN < 0 ? nStepsize >> -nN : nStepsize << nN) & 0x7ff;
		pBandIndexStepsize->nExponent = nBPsCount - nP;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	//  Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	void   DWT_EncodeRev(TileComp *pTileComponent)
	{
		int *pA = NULL;
		int *pB = NULL;

		int nTileWidth = pTileComponent->nX1 - pTileComponent->nX0;
		int nLevel     = pTileComponent->nResolutionsCount - 1;
		int *pData     = pTileComponent->pData;

		for (int nIndex = 0; nIndex < nLevel; nIndex++)
		{
			int nResW;      // Ширина для данного уровня разрешения
			int nResH;      // Высота для данного уровня разрешения
			int nResW1;     // Ширина для уровня разрешения на 1 меньше, чем данный
			int nResH1;     // Высота для уровня разрешения на 1 меньше, чем данный
			int nCaseCol;   // 0 = non inversion on horizontal filtering, 1 = inversion between low-pass and high-pass filtering
			int nCaseRow;   // 0 = non inversion on vertical filtering, 1 = inversion between low-pass and high-pass filtering

			nResW  = pTileComponent->pResolutions[nLevel - nIndex].nX1 - pTileComponent->pResolutions[nLevel - nIndex].nX0;
			nResH  = pTileComponent->pResolutions[nLevel - nIndex].nY1 - pTileComponent->pResolutions[nLevel - nIndex].nY0;
			nResW1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nX1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nX0;
			nResH1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nY1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nY0;

			nCaseRow = pTileComponent->pResolutions[nLevel - nIndex].nX0 % 2;
			nCaseCol = pTileComponent->pResolutions[nLevel - nIndex].nY0 % 2;

			int nSn = nResH1;
			int nDn = nResH - nResH1;
			pB = (int*)Malloc(nResH * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResW; nJ++)
			{
				pA = pData + nJ;
				for (int nK = 0; nK < nResH; nK++)
					pB[nK] = pA[nK * nTileWidth];
				DWT_ForwardRev1D(pB, nDn, nSn, nCaseCol);
				DWT_DeinterleaveVer(pB, pA, nDn, nSn, nTileWidth, nCaseCol);
			}
			Free(pB);

			nSn = nResW1;
			nDn = nResW - nResW1;
			pB = (int*)Malloc(nResW * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResH; nJ++)
			{
				pA = pData + nJ * nTileWidth;
				for (int nK = 0; nK < nResW; nK++)
					pB[nK] = pA[nK];
				DWT_ForwardRev1D(pB, nDn, nSn, nCaseRow);
				DWT_DeinterleaveHor(pB, pA, nDn, nSn, nCaseRow);
			}
			Free(pB);
		}
	}

	void   DWT_DecodeRev(TileComp *pTileComponent, int nStop)
	{
		int *pA = NULL;
		int *pB = NULL;

		int nTileWidth = pTileComponent->nX1 - pTileComponent->nX0;
		int nLevel     = pTileComponent->nResolutionsCount - 1;
		int *pData     = pTileComponent->pData;

		for (int nIndex = nLevel - 1; nIndex >= nStop; nIndex--)
		{
			int nResW;      // Ширина для данного уровня разрешения
			int nResH;      // Высота для данного уровня разрешения
			int nResW1;     // Ширина для уровня разрешения на 1 меньше, чем данный
			int nResH1;     // Высота для уровня разрешения на 1 меньше, чем данный
			int nCaseCol;   // 0 = non inversion on horizontal filtering, 1 = inversion between low-pass and high-pass filtering
			int nCaseRow;   // 0 = non inversion on vertical filtering, 1 = inversion between low-pass and high-pass filtering

			nResW  = pTileComponent->pResolutions[nLevel - nIndex].nX1 - pTileComponent->pResolutions[nLevel - nIndex].nX0;
			nResH  = pTileComponent->pResolutions[nLevel - nIndex].nY1 - pTileComponent->pResolutions[nLevel - nIndex].nY0;
			nResW1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nX1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nX0;
			nResH1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nY1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nY0;

			nCaseRow = pTileComponent->pResolutions[nLevel - nIndex].nX0 % 2;
			nCaseCol = pTileComponent->pResolutions[nLevel - nIndex].nY0 % 2;

			int nSn = nResW1;
			int nDn = nResW - nResW1;
			pB = (int*)Malloc(nResW * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResH; nJ++)
			{
				pA = pData + nJ * nTileWidth;
				DWT_InterleaveHor(pA, pB, nDn, nSn, nCaseRow);
				DWT_InverseRev1D(pB, nDn, nSn, nCaseRow);
				for (int nK = 0; nK < nResW; nK++)
					pA[nK] = pB[nK];
			}
			Free(pB);

			nSn = nResH1;
			nDn = nResH - nResH1;
			pB = (int*)Malloc(nResH * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResW; nJ++)
			{
				pA = pData + nJ;
				DWT_InterleaveVer(pA, pB, nDn, nSn, nTileWidth, nCaseCol);
				DWT_InverseRev1D(pB, nDn, nSn, nCaseCol);
				for (int nK = 0; nK < nResH; nK++)
					pA[nK * nTileWidth] = pB[nK];
			}
			Free(pB);
		}
	}

	int    DWT_GetGainRev(int nOrient)
	{
		if (nOrient == 0)
			return 0;
		if (nOrient == 1 || nOrient == 2)
			return 1;
		return 2;
	}

	double DWT_GetNormRev(int nLevel, int nOrient)
	{
		return c_aDWT_NormsRev[nOrient][nLevel];
	}

	void   DWT_EncodeIrr(TileComp *pTileComponent)
	{
		int *pA = NULL;
		int *pB = NULL;

		int nTileWidth = pTileComponent->nX1 - pTileComponent->nX0;
		int nLevel     = pTileComponent->nResolutionsCount - 1;
		int *pData     = pTileComponent->pData;

		for (int nIndex = 0; nIndex < nLevel; nIndex++)
		{
			int nResW;      // Ширина для данного уровня разрешения
			int nResH;      // Высота для данного уровня разрешения
			int nResW1;     // Ширина для уровня разрешения на 1 меньше, чем данный
			int nResH1;     // Высота для уровня разрешения на 1 меньше, чем данный
			int nCaseCol;   // 0 = non inversion on horizontal filtering, 1 = inversion between low-pass and high-pass filtering
			int nCaseRow;   // 0 = non inversion on vertical filtering, 1 = inversion between low-pass and high-pass filtering

			nResW  = pTileComponent->pResolutions[nLevel - nIndex].nX1 - pTileComponent->pResolutions[nLevel - nIndex].nX0;
			nResH  = pTileComponent->pResolutions[nLevel - nIndex].nY1 - pTileComponent->pResolutions[nLevel - nIndex].nY0;
			nResW1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nX1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nX0;
			nResH1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nY1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nY0;

			nCaseRow = pTileComponent->pResolutions[nLevel - nIndex].nX0 % 2;
			nCaseCol = pTileComponent->pResolutions[nLevel - nIndex].nY0 % 2;

			int nSn = nResH1;
			int nDn = nResH - nResH1;
			pB = (int*)Malloc(nResH * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResW; nJ++)
			{
				pA = pData + nJ;
				for (int nK = 0; nK < nResH; nK++)
					pB[nK] = pA[nK * nTileWidth];
				DWT_ForwardIrr1D(pB, nDn, nSn, nCaseCol);
				DWT_DeinterleaveVer(pB, pA, nDn, nSn, nTileWidth, nCaseCol);
			}
			Free(pB);

			nSn = nResW1;
			nDn = nResW - nResW1;
			pB = (int*)Malloc(nResW * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResH; nJ++)
			{
				pA = pData + nJ * nTileWidth;
				for (int nK = 0; nK < nResW; nK++)
					pB[nK] = pA[nK];
				DWT_ForwardIrr1D(pB, nDn, nSn, nCaseRow);
				DWT_DeinterleaveHor(pB, pA, nDn, nSn, nCaseRow);
			}
			Free(pB);
		}
	}

	void   DWT_DecodeIrr(TileComp *pTileComponent, int nStop)
	{
		int *pA = NULL;
		int *pB = NULL;

		int nTileWidth = pTileComponent->nX1 - pTileComponent->nX0;
		int nLevel     = pTileComponent->nResolutionsCount - 1;
		int *pData     = pTileComponent->pData;

		for (int nIndex = nLevel - 1; nIndex >= nStop; nIndex--)
		{
			int nResW;      // Ширина для данного уровня разрешения
			int nResH;      // Высота для данного уровня разрешения
			int nResW1;     // Ширина для уровня разрешения на 1 меньше, чем данный
			int nResH1;     // Высота для уровня разрешения на 1 меньше, чем данный
			int nCaseCol;   // 0 = non inversion on horizontal filtering, 1 = inversion between low-pass and high-pass filtering
			int nCaseRow;   // 0 = non inversion on vertical filtering, 1 = inversion between low-pass and high-pass filtering

			nResW  = pTileComponent->pResolutions[nLevel - nIndex].nX1 - pTileComponent->pResolutions[nLevel - nIndex].nX0;
			nResH  = pTileComponent->pResolutions[nLevel - nIndex].nY1 - pTileComponent->pResolutions[nLevel - nIndex].nY0;
			nResW1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nX1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nX0;
			nResH1 = pTileComponent->pResolutions[nLevel - nIndex - 1].nY1 - pTileComponent->pResolutions[nLevel - nIndex - 1].nY0;

			nCaseCol = pTileComponent->pResolutions[nLevel - nIndex].nX0 % 2;
			nCaseRow = pTileComponent->pResolutions[nLevel - nIndex].nY0 % 2;

			int nSn = nResW1;
			int nDn = nResW - nResW1;
			pB = (int*)Malloc(nResW * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResH; nJ++)
			{
				pA = pData + nJ * nTileWidth;
				DWT_InterleaveHor(pA, pB, nDn, nSn, nCaseCol);
				DWT_InverseIrr1D(pB, nDn, nSn, nCaseCol);
				for (int nK = 0; nK < nResW; nK++)
					pA[nK] = pB[nK];
			}
			Free(pB);

			nSn = nResH1;
			nDn = nResH - nResH1;
			pB = (int*)Malloc(nResH * sizeof(int));
			if (!pB)
			{
				// TODO: print error "Memory!!!\n"
				return;
			}

			for (int nJ = 0; nJ < nResW; nJ++)
			{
				pA = pData + nJ;
				DWT_InterleaveVer(pA, pB, nDn, nSn, nTileWidth, nCaseRow);
				DWT_InverseIrr1D(pB, nDn, nSn, nCaseRow);
				for (int nK = 0; nK < nResH; nK++)
					pA[nK * nTileWidth] = pB[nK];
			}
			Free(pB);
		}
	}

	int    DWT_GetGainIrr(int nOrient)
	{
		(void)nOrient;
		return 0;
	}

	double DWT_GetNormIrr(int nLevel, int nOrient)
	{
		return c_aDWT_NormsIrr[nOrient][nLevel];
	}

	void   DWT_CalcExplicitStepsizes(TileCompCodingParams *pTCCP, int nPrecision)
	{
		int nBandsCount = 3 * pTCCP->nResolutionsCount - 2;
		for (int nBandIndex = 0; nBandIndex < nBandsCount; nBandIndex++)
		{
			double dStepsize = 0;

			int nResIndex = (nBandIndex == 0) ? 0 : ((nBandIndex - 1) / 3 + 1);
			int nOrient   = (nBandIndex == 0) ? 0 : ((nBandIndex - 1) % 3 + 1);
			int nLevel = pTCCP->nResolutionsCount - 1 - nResIndex;
			int nGain  = (pTCCP->nTransformID == 0) ? 0 : ((nOrient == 0) ? 0 : (((nOrient == 1) || (nOrient == 2)) ? 1 : 2));
			if (pTCCP->nQuantStyle == J2K_CCP_QNTSTY_NOQNT)
			{
				dStepsize = 1.0;
			}
			else
			{
				double dNorm = c_aDWT_NormsIrr[nOrient][nLevel];
				dStepsize = (1 << (nGain)) / dNorm;
			}
			DWT_EncodeStepsize((int)floor(dStepsize * 8192.0), nPrecision + nGain, &pTCCP->aoStepSizes[nBandIndex]);
		}
	}
}
#pragma once

namespace Jpeg2000
{
	static double Clock()
	{
		return 0;
		//   LARGE_INTEGER oFreq, oTime;

		//// oFreq - скорость часов у CPU
		//   QueryPerformanceFrequency( &oFreq );
		//   // oTime - текущее время(в смысле oFreq)
		//   QueryPerformanceCounter ( &oTime ) ;
		//   return ( oTime.QuadPart /(double) oFreq.QuadPart ) ;
	}



	static __inline int FixedMult(int nA, int nB)
	{
		__int64 nTemp = (__int64)nA * (__int64)nB >> 12;
		return (int)((nTemp >> 1) + (nTemp & 1));
	}
	static __inline int Clamp(int nValue, int nMin, int nMax)
	{
		if (nValue < nMin)
			return nMin;
		if (nValue > nMax)
			return nMax;
		return nValue;
	}

	// Делим nA на nB и округляем в большую сторону
	static __inline int CeilDiv(int nA, int nB)
	{
		return (nA + nB - 1) / nB;
	}

	// Делим nA на 2^nB и округляем в большую сторону
	static __inline int CeilDivPow2(int nA, int nB)
	{
		return (nA + (1 << nB) - 1) >> nB;
	}

	// Делим nA на 2^nB и округляем в меньшую сторону
	static __inline int FloorDivPow2(int nA, int nB)
	{
		return nA >> nB;
	}

	// Берем логарифм по основанию 2 от числа nA и округляем в меньшую сторону
	static __inline int FloorLog2(int nA)
	{
		int nL;
		for (nL = 0; nA > 1; nL++)
		{
			nA >>= 1;
		}
		return nL;
	}



	//-------------------------------------------------------------------------------------------------------------------------------
	//    Multi-Component Transform
	//-------------------------------------------------------------------------------------------------------------------------------
	static const double c_adNormRCT[3] ={ 1.732, .8292, .8292 };
	static const double c_adNormICT[3] ={ 1.732, 1.805, 1.573 };

	// Применяем обратимое много-компонентное преобразование к изображению (Annex G.2.1)
	static void   ForwardRCT(int *pRed, int *pGreen, int *pBlue, int nSamplesCount)
	{
		for (int nIndex = 0; nIndex < nSamplesCount; nIndex++)
		{
			int nR = pRed[nIndex];
			int nG = pGreen[nIndex];
			int nB = pBlue[nIndex];
			// RGB -> YUV
			int nY = (nR + (nG << 1) + nB) >> 2;
			int nU = nB - nG;
			int nV = nR - nG;
			pRed[nIndex]   = nY;
			pGreen[nIndex] = nU;
			pBlue[nIndex]  = nV;
		}
	}

	// Применяем обратимое много-компонентное обратное преобазование к изображению (Annex G.2.2)
	static void   InverseRCT(int *pRed, int *pGreen, int *pBlue, int nSamplesCount)
	{
		for (int nIndex = 0; nIndex < nSamplesCount; nIndex++)
		{
			int nY = pRed[nIndex];
			int nU = pGreen[nIndex];
			int nV = pBlue[nIndex];

			int nG = nY - ((nU + nV) >> 2);
			int nR = nV + nG;
			int nB = nU + nG;

			pRed[nIndex]   = nR;
			pGreen[nIndex] = nG;
			pBlue[nIndex]  = nB;
		}
	}

	static double GetNormRCT(int nComponentIndex)
	{
		return c_adNormRCT[nComponentIndex];
	}

	// Применяем необратимое много-компонентное преобразование к изображению (Annex G.3.1)
	static void   ForwardICT(int *pRed, int *pGreen, int *pBlue, int nSamplesCount)
	{
		for (int nIndex = 0; nIndex < nSamplesCount; nIndex++)
		{
			int nR = pRed[nIndex];
			int nG = pGreen[nIndex];
			int nB = pBlue[nIndex];

			int nY =  FixedMult(nR, 2449) + FixedMult(nG, 4809) + FixedMult(nB, 934);
			int nU = -FixedMult(nR, 1382) - FixedMult(nG, 2714) + FixedMult(nB, 4096);
			int nV =  FixedMult(nR, 4096) - FixedMult(nG, 3430) - FixedMult(nB, 666);
			pRed[nIndex]   = nY;
			pGreen[nIndex] = nU;
			pBlue[nIndex]  = nV;
		}
	}

	// Применяем необратимое много-компонентное обратное преобазование к изображению (Annex G.3.2)
	static void   InverseICT(int *pRed, int *pGreen, int *pBlue, int nSamplesCount)
	{
		for (int nIndex = 0; nIndex < nSamplesCount; nIndex++)
		{
			int nY = pRed[nIndex];
			int nU = pGreen[nIndex];
			int nV = pBlue[nIndex];

			int nR = nY + FixedMult(nV, 11485);
			int nG = nY - FixedMult(nU, 2819) - FixedMult(nV, 5850);
			int nB = nY + FixedMult(nU, 14516);

			pRed[nIndex]   = nR;
			pGreen[nIndex] = nG;
			pBlue[nIndex]  = nB;
		}
	}

	static double GetNormICT(int nComponentIndex)
	{
		return c_adNormICT[nComponentIndex];
	}
}
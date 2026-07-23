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

namespace Jpeg2000
{
	static double Clock()
	{
		return 0;
		//   LARGE_INTEGER oFreq, oTime;

		//// oFreq - CPU clock speed
		//   QueryPerformanceFrequency( &oFreq );
		//   // oTime - current time (in terms of oFreq)
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

	// Divide nA by nB and round up
	static __inline int CeilDiv(int nA, int nB)
	{
		return (nA + nB - 1) / nB;
	}

	// Divide nA by 2^nB and round up
	static __inline int CeilDivPow2(int nA, int nB)
	{
		return (nA + (1 << nB) - 1) >> nB;
	}

	// Divide nA by 2^nB and round down
	static __inline int FloorDivPow2(int nA, int nB)
	{
		return nA >> nB;
	}

	// Take base-2 logarithm of nA and round down
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

	// Apply reversible multi-component transform to image (Annex G.2.1)
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

	// Apply reversible multi-component inverse transform to image (Annex G.2.2)
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

	// Apply irreversible multi-component transform to image (Annex G.3.1)
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

	// Apply irreversible multi-component inverse transform to image (Annex G.3.2)
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
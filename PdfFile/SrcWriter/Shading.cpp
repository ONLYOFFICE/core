/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Shading.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CLinearFuntion
	//----------------------------------------------------------------------------------------
	class CLinearFuntion : public CDictObject
	{
	public:
		CLinearFuntion(CXref* pXref, double* pC0Values, double* pC1Values, int nCount)
		{
			if (!nCount || !pC0Values || !pC1Values)
				return;

			pXref->Add(this);

			Add("FunctionType", 2);

			CArrayObject* pDomain = new CArrayObject();
			if (!pDomain)
				return;

			pDomain->Add(0.0);
			pDomain->Add(1.0);

			Add("Domain", pDomain);

			CArrayObject* pC0Array = new CArrayObject();
			if (!pC0Array)
				return;

			Add("C0", pC0Array);

			CArrayObject* pC1Array = new CArrayObject();
			if (!pC1Array)
				return;

			Add("C1", pC1Array);

			Add("N", 1);

			for (int nIndex = 0; nIndex < nCount; nIndex++)
			{
				pC0Array->Add(pC0Values[nIndex]);
				pC1Array->Add(pC1Values[nIndex]);
			}

		}	
	};
	//----------------------------------------------------------------------------------------
	// CLineSegmentFuntion
	//----------------------------------------------------------------------------------------
	class CLineSegmentFuntion : public CDictObject
	{
	public:
		CLineSegmentFuntion(CXref* pXref, double** ppValues, double* pPoints, int nFunctionsCount, int nOutputDimension)
		{
			pXref->Add(this);

			Add("FunctionType", 3);

			CArrayObject* pDomain = new CArrayObject();
			if (!pDomain)
				return;
			pDomain->Add(0.0);
			pDomain->Add(1.0);
			Add("Domain", pDomain);

			CArrayObject* pFunctions = new CArrayObject();
			if (!pFunctions)
				return;

			Add("Functions", pFunctions);

			CArrayObject* pBounds = new CArrayObject();
			if (!pBounds)
				return;

			Add("Bounds", pBounds);

			CArrayObject* pEncode = new CArrayObject();
			if (!pEncode)
				return;

			Add("Encode", pEncode);

			for (int nFunctionIndex = 0; nFunctionIndex < nFunctionsCount; nFunctionIndex++)
			{
				CLinearFuntion* pFunc = new CLinearFuntion(pXref, ppValues[2 * nFunctionIndex], ppValues[2 * nFunctionIndex + 1], nOutputDimension);
				pFunctions->Add(pFunc);
				pEncode->Add(0.0);
				pEncode->Add(1.0);

				if (nFunctionIndex != nFunctionsCount - 1)
					pBounds->Add(pPoints[nFunctionIndex]);
			}
		}
	};
	//----------------------------------------------------------------------------------------
	// CShading
	//----------------------------------------------------------------------------------------
	CShading::CShading(CXref* pXref)
	{
		m_pXref = pXref;
		pXref->Add(this);

		m_bRgb          = true;
		m_pColors       = NULL;
		m_pColorsPoints = NULL;
		m_nColorsCount  = 0;
		m_bBeginExtend  = false;
		m_bEndExtend    = false;
	}
	CShading::~CShading()
	{
		if (m_pColors)
			delete[] m_pColors;

		if (m_pColorsPoints)
			delete[] m_pColorsPoints;
	}
	void CShading::SetRgbColors(unsigned char* pColors, double* pPoints, int nCount)
	{
		if (!pColors || !pPoints || nCount <= 1)
			return;

		m_bRgb          = true;
		m_pColors       = new unsigned char[3 * nCount];
		m_pColorsPoints = new double[nCount];
		m_nColorsCount  = nCount;

		if (!m_pColors || !m_pColorsPoints)
			return;

		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			m_pColors[3 * nIndex + 0] = pColors[3 * nIndex + 0];
			m_pColors[3 * nIndex + 1] = pColors[3 * nIndex + 1];
			m_pColors[3 * nIndex + 2] = pColors[3 * nIndex + 2];
			m_pColorsPoints[nIndex]   = pPoints[nIndex];
		}

		Add("ColorSpace", "DeviceRGB");

		CArrayObject* pDomain = new CArrayObject();
		if (!pDomain)
			return;

		pDomain->Add(0.0);
		pDomain->Add(1.0);
		Add("Domain", pDomain);

		if (nCount <= 2)
		{
			double pC0Values[] ={
				pColors[0] / 255.0, 
				pColors[1] / 255.0, 
				pColors[2] / 255.0
			};

			double pC1Values[] ={
				pColors[3] / 255.0,
				pColors[4] / 255.0,
				pColors[5] / 255.0
			};

			CLinearFuntion* pFunction = new CLinearFuntion(m_pXref, pC0Values, pC1Values, 3);
			Add("Function", pFunction);
		}
		else
		{
			double** ppValues = new double*[(nCount - 1) * 2];
			for (int nIndex = 0; nIndex < nCount - 1; nIndex++)
			{
				ppValues[2 * nIndex + 0] = new double[3];
				ppValues[2 * nIndex + 1] = new double[3];

				ppValues[2 * nIndex + 0][0] = pColors[3 * nIndex + 0] / 255.0;
				ppValues[2 * nIndex + 0][1] = pColors[3 * nIndex + 1] / 255.0;
				ppValues[2 * nIndex + 0][2] = pColors[3 * nIndex + 2] / 255.0;
				ppValues[2 * nIndex + 1][0] = pColors[3 * (nIndex + 1) + 0] / 255.0;
				ppValues[2 * nIndex + 1][1] = pColors[3 * (nIndex + 1) + 1] / 255.0;
				ppValues[2 * nIndex + 1][2] = pColors[3 * (nIndex + 1) + 2] / 255.0;
			}

			CLineSegmentFuntion* pFunction = new CLineSegmentFuntion(m_pXref, ppValues, pPoints + 1, nCount - 1, 3);
			Add("Function", pFunction);

			for (int nIndex = 0; nIndex < nCount - 1; nIndex++)
			{
				delete[] ppValues[2 * nIndex + 0];
				delete[] ppValues[2 * nIndex + 1];
			}
			delete[] ppValues;
		}
	}
	void CShading::SetGrayColors(unsigned char* pColors, double* pPoints, int nCount)
	{
		if (!pColors || !pPoints || nCount <= 1)
			return;

		m_bRgb          = false;
		m_pColors       = new unsigned char[nCount];
		m_pColorsPoints = new double[nCount];
		m_nColorsCount  = nCount;

		if (!m_pColors || !m_pColorsPoints)
			return;

		memcpy(m_pColors, pColors, nCount);

		Add("ColorSpace", "DeviceGray");

		CArrayObject* pDomain = new CArrayObject();
		if (!pDomain)
			return;

		pDomain->Add(0.0);
		pDomain->Add(1.0);
		Add("Domain", pDomain);

		if (nCount <= 2)
		{
			double pC0Values = pColors[0] / 255.0;
			double pC1Values = pColors[1] / 255.0;

			CLinearFuntion* pFunction = new CLinearFuntion(m_pXref, &pC0Values, &pC1Values, 1);
			Add("Function", pFunction);
		}
		else
		{
			double* pValues = new double[(nCount - 1) * 2];
			if (pValues)
			{
				for (int nIndex = 0; nIndex < nCount - 1; nIndex++)
				{
					pValues[2 * nIndex + 0] = pColors[nIndex] / 255.0;
					pValues[2 * nIndex + 1] = pColors[nIndex + 1] / 255.0;
				}

				CLineSegmentFuntion* pFunction = new CLineSegmentFuntion(m_pXref, &pValues, pPoints + 1, nCount - 1, 1);
				Add("Function", pFunction);
				delete[] pValues;
			}
		}
	}
	void CShading::SetExtend(bool bBegin, bool bEnd)
	{
		CArrayObject* pExtend = new CArrayObject();
		if (!pExtend)
			return;

		pExtend->Add(bBegin);
		pExtend->Add(bEnd);
		Add("Extend", pExtend);

		m_bBeginExtend = bBegin;
		m_bEndExtend   = bEnd;
	}
	bool CShading::CompareColors(unsigned char* pColors, double* pPoints, int nCount, bool bRgb)
	{
		if (nCount != m_nColorsCount
			|| (pColors && !m_pColors)
			|| (pPoints && !m_pColorsPoints)
			|| (!pColors && m_pColors)
			|| (!pPoints && m_pColorsPoints)
			|| bRgb != m_bRgb)
			return false;

		if (m_bRgb)
		{
			for (int nIndex = 0; nIndex < nCount; nIndex++)
			{
				if (pColors[3 * nIndex + 0] != m_pColors[3 * nIndex + 0]
					|| pColors[3 * nIndex + 1] != m_pColors[3 * nIndex + 1]
					|| pColors[3 * nIndex + 2] != m_pColors[3 * nIndex + 2]
					|| fabs(pPoints[nIndex] - m_pColorsPoints[nIndex]) > 0.01)
					return false;
			}
		}
		else
		{
			for (int nIndex = 0; nIndex < nCount; nIndex++)
			{
				if (pColors[nIndex] != m_pColors[nIndex]
					|| fabs(pPoints[nIndex] - m_pColorsPoints[nIndex]) > 0.01)
					return false;
			}
		}

		return true;
	}
	bool CShading::CompareExtend(bool bBegin, bool bEnd)
	{
		if (bBegin != m_bBeginExtend || bEnd != m_bEndExtend)
			return false;

		return true;
	}
	//----------------------------------------------------------------------------------------
	// CAxialShading
	//----------------------------------------------------------------------------------------
	CAxialShading::CAxialShading(CXref* pXref, double dX0, double dY0, double dX1, double dY1) : CShading(pXref)
	{
		Add("ShadingType", 2);

		CArrayObject* pCoords = new CArrayObject();
		if (!pCoords)
			return;

		pCoords->Add(dX0);
		pCoords->Add(dY0);
		pCoords->Add(dX1);
		pCoords->Add(dY1);
		Add("Coords", pCoords);

		m_dX0 = dX0;
		m_dY0 = dY0;
		m_dX1 = dX1;
		m_dY1 = dY1;
	}
	bool CAxialShading::Compare(double dX0, double dY0, double dX1, double dY1)
	{
		if (fabs(dX0 - m_dX0) > 0.01
			|| fabs(dY0 - m_dY0) > 0.01
			|| fabs(dX1 - m_dX1) > 0.01
			|| fabs(dY1 - m_dY1) > 0.01)
			return false;

		return true;
	}
	//----------------------------------------------------------------------------------------
	// CRadialShading
	//----------------------------------------------------------------------------------------
	CRadialShading::CRadialShading(CXref* pXref, double dX0, double dY0, double dR0, double dX1, double dY1, double dR1) : CShading(pXref)
	{
		Add("ShadingType", 3);

		CArrayObject* pCoords = new CArrayObject();
		if (!pCoords)
			return;

		pCoords->Add(dX0);
		pCoords->Add(dY0);
		pCoords->Add(dR0);
		pCoords->Add(dX1);
		pCoords->Add(dY1);
		pCoords->Add(dR1);
		Add("Coords", pCoords);

		m_dX0 = dX0;
		m_dY0 = dY0;
		m_dR0 = dR0;
		m_dX1 = dX1;
		m_dY1 = dY1;
		m_dR1 = dR1;
	}
	bool CRadialShading::Compare(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1)
	{
		if (fabs(dX0 - m_dX0) > 0.01
			|| fabs(dY0 - m_dY0) > 0.01
			|| fabs(dR0 - m_dR0) > 0.01
			|| fabs(dX1 - m_dX1) > 0.01
			|| fabs(dY1 - m_dY1) > 0.01
			|| fabs(dR1 - m_dR1) > 0.01)
			return false;

		return true;
	}
}
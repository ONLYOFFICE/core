/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#pragma once

#include "PptFormula.h"

namespace NSGuidesVML
{
	CFormula::CFormula()
	{
		m_eFormulaType = ftSum;
		m_lIndex = 0;
		m_lParam1 = 0;	m_eType1 = ptValue;
		m_lParam2 = 0;	m_eType2 = ptValue;
		m_lParam3 = 0;	m_eType3 = ptValue;

		m_lCountRecurs = 0;
	}
	CFormula::CFormula(int nIndex)
	{
		m_eFormulaType = ftSum;
		m_lIndex = nIndex;
		m_lParam1 = 0;	m_eType1 = ptValue;
		m_lParam2 = 0;	m_eType2 = ptValue;
		m_lParam3 = 0;	m_eType3 = ptValue;

		m_lCountRecurs = 0;
	}
	CFormula& CFormula::operator =(const CFormula& oSrc)
	{
		m_eFormulaType	= oSrc.m_eFormulaType;
		m_lIndex		= oSrc.m_lIndex;

		m_lParam1		= oSrc.m_lParam1;
		m_eType1		= oSrc.m_eType1;

		m_lParam2		= oSrc.m_lParam2;
		m_eType2		= oSrc.m_eType2;

		m_lParam3		= oSrc.m_lParam3;
		m_eType3		= oSrc.m_eType3;

		m_lCountRecurs = 0;
		return (*this);
	}
	void CFormula::FromString(std::wstring strFormula, long lShapeWidth, long lShapeHeight)
	{
		std::vector<std::wstring> oArrayParams;
		boost::algorithm::split(oArrayParams, strFormula, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

		int nCount = (int)oArrayParams.size();
		if (0 >= nCount)
			return;

		bool bRes = true;
		m_eFormulaType = GetFormula(oArrayParams[0], bRes);

		ParamType ptType = ptValue;
		if (1 < nCount)
		{
			m_lParam1 = GetValue(oArrayParams[1], ptType, bRes, lShapeWidth, lShapeHeight);
			m_eType1 = ptType;
		}
		if (2 < nCount)
		{
			m_lParam2 = GetValue(oArrayParams[2], ptType, bRes, lShapeWidth, lShapeHeight);
			m_eType2 = ptType;
		}
		if (3 < nCount)
		{
			m_lParam3 = GetValue(oArrayParams[3], ptType, bRes, lShapeWidth, lShapeHeight);
			m_eType3 = ptType;
		}
	}
	LONG CFormula::Calculate(NSGuidesVML::CFormulasManager* pManager)
	{
		if ((0 > m_lIndex) || (m_lIndex >= (int)pManager->m_arResults.size()))
			return 0;
		if (0xFFFFFFFF != pManager->m_arResults[m_lIndex])
		{
			return pManager->m_arResults[m_lIndex];
		}

		LONG lResult = 0;

		LONG lGuidesCount	= (LONG)pManager->m_arFormulas.size();
		LONG lAdjCount		= (LONG)pManager->m_pAdjustments->size();

		LONG a1 = m_lParam1;
		if (ptFormula == m_eType1 && !pManager->m_bCalc)
		{
			pManager->m_bCalc = true;
			a1 = (m_lParam1 >= lGuidesCount) ? 0 : pManager->m_arFormulas[m_lParam1].Calculate(pManager);
		}
		else if (ptAdjust == m_eType1)
		{
			a1 = (m_lParam1 >= lAdjCount) ? 0 : (*(pManager->m_pAdjustments))[m_lParam1];
		}
		pManager->m_bCalc = false;

		LONG b1 = m_lParam2;
		if (ptFormula == m_eType2 && !pManager->m_bCalc)
		{
			pManager->m_bCalc = true;
			b1 = (m_lParam2 >= lGuidesCount) ? 0 : pManager->m_arFormulas[m_lParam2].Calculate(pManager);
		}
		else if (ptAdjust == m_eType2)
		{
			b1 = (m_lParam2 >= lAdjCount) ? 0 : (*(pManager->m_pAdjustments))[m_lParam2];
		}
		pManager->m_bCalc = false;

		LONG c1 = m_lParam3;
		if (ptFormula == m_eType3 && !pManager->m_bCalc)
		{
			pManager->m_bCalc = true;
			c1 = (m_lParam3 >= lGuidesCount) ? 0 : pManager->m_arFormulas[m_lParam3].Calculate(pManager);
		}
		else if (ptAdjust == m_eType3)
		{
			c1 = (m_lParam3 >= lAdjCount) ? 0 : (*(pManager->m_pAdjustments))[m_lParam3];
		}
		pManager->m_bCalc = false;

		double a = (double)a1;
		double b = (double)b1;
		double c = (double)c1;

		double dRes = 0.0;

		try
		{
			// теперь нужно просто посчитать
			switch (m_eFormulaType)
			{
			case ftSum:			{ dRes = a + b - c;				break; }
			case ftProduct:		{
									if (0 == c)
										c = 1;

									dRes = a * b / c;
									break;
								}
			case ftMid:			{ dRes = (a + b) / 2.0;			break; }
			case ftAbsolute:	{ dRes = abs(a);				break; }

			case ftMin:			{ dRes = (std::min)(a, b);				break; }
			case ftMax:			{ dRes = (std::max)(a, b);				break; }
			case ftIf:			{ dRes = (a > 0) ? b : c;		break; }
			case ftSqrt:		{ dRes = sqrt(a);				break; }
			case ftMod:			{ dRes = sqrt(a*a + b*b + c*c); break; }

			case ftSin:			{
									//dRes = a * sin(b);
									//dRes = a * sin(b / pow2_16);
									dRes = a * sin(M_PI * b / (pow2_16 * 180));
									break;
								}
			case ftCos:			{
									//dRes = a * cos(b);
									//dRes = a * cos(b / pow2_16);
									dRes = a * cos(M_PI * b / (pow2_16 * 180));
									break;
								}
			case ftTan:			{
									//dRes = a * tan(b);
									dRes = a * tan(M_PI * b / (pow2_16 * 180));
									break;
								}
			case ftAtan2:		{
									dRes =  180 * pow2_16 * atan2(b,a) / M_PI;
									break;
								}

			case ftSinatan2:	{ dRes = a * sin(atan2(c,b));	break; }
			case ftCosatan2:	{ dRes = a * cos(atan2(c,b));	break; }

			case ftSumangle:	{
									//dRes = a  + b - c;
									dRes = a + b * pow2_16 - c * pow2_16;

									/*while (23592960 < dRes)
									{
										dRes -= 23592960;
									}
									while (-23592960 > dRes)
									{
										dRes += 23592960;
									}*/

									break;
								}
			case ftEllipse:		{
									if (0 == b)
										b = 1;
									dRes = c * sqrt(1-(a*a/(b*b)));
									break;
								}
			case ftVal:			{ dRes = a;						break; }
			default: break;
			};
		}
		catch (...)
		{
			dRes = 0;
		}

		lResult = (LONG)dRes;
		pManager->m_arResults[m_lIndex] = lResult;
		return lResult;
	}

	CFormulasManager::CFormulasManager() : m_bCalc(false)
	{
		m_pAdjustments	= NULL;
		m_lShapeWidth	= ShapeSizeVML;
		m_lShapeHeight	= ShapeSizeVML;
	}
	CFormulasManager& CFormulasManager::operator =(const CFormulasManager& oSrc)
	{
		m_bCalc			= oSrc.m_bCalc;
		m_pAdjustments	= oSrc.m_pAdjustments;
		m_lShapeWidth	= oSrc.m_lShapeWidth;
		m_lShapeHeight	= oSrc.m_lShapeHeight;

		m_arResults.clear();
		for (size_t nIndex = 0; nIndex < oSrc.m_arResults.size(); ++nIndex)
		{
			m_arResults.push_back(oSrc.m_arResults[nIndex]);
		}
		m_arFormulas.clear();
		for (size_t nIndex = 0; nIndex < oSrc.m_arFormulas.size(); ++nIndex)
		{
			m_arFormulas.push_back(oSrc.m_arFormulas[nIndex]);
		}

		return (*this);
	}
	void CFormulasManager::Clear()
	{
		m_bCalc			= false;
		m_pAdjustments	= NULL;
		m_lShapeWidth	= ShapeSizeVML;
		m_lShapeHeight	= ShapeSizeVML;

		m_arFormulas.clear();
		m_arResults.clear();
	}
	void CFormulasManager::Clear(std::vector<long>* pAdjusts)
	{
		m_pAdjustments = pAdjusts;

		//m_arFormulas.clear();
		//m_arResults.clear();
		for (size_t nIndex = 0; nIndex < m_arResults.size(); ++nIndex)
		{
			m_arResults[nIndex] = 0xFFFFFFFF;
		}
	}
	void CFormulasManager::AddFormula(std::wstring strFormula)
	{
		CFormula oFormula((int)m_arFormulas.size());

		oFormula.FromString(strFormula, m_lShapeWidth, m_lShapeHeight);
		m_arFormulas.push_back(oFormula);
		m_arResults.push_back(0xFFFFFFFF);
	}
	void CFormulasManager::AddFormula(CFormula oFormula)
	{
		oFormula.m_lIndex = (int)m_arFormulas.size();

		m_arFormulas.push_back(oFormula);
		m_arResults.push_back(0xFFFFFFFF);
	}
	void CFormulasManager::CalculateResults()
	{
		for (size_t index = 0; index < m_arFormulas.size(); ++index)
		{
			LONG lResult = m_arFormulas[index].Calculate(this);
		}

		//m_pAdjustments = NULL;
		//m_arFormulas.clear();
	}
}

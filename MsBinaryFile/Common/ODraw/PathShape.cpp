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

#include "PathShape.h"

namespace NSCustomShapesConvert
{
	CSlice::CSlice(RulesType eType /*, LONG x = 0, LONG y = 0*/)
	{
		m_eRuler = eType;
		m_nCountElementsPoint = 0;
		//m_lX = x;
		//m_lY = y;
	}
	void CSlice::AddParam(LONG lParam)
	{
		long lPoint = m_nCountElementsPoint % 2;
		if (0 == lPoint)
		{
			Aggplus::POINT point;
			point.x = lParam/* - m_lX*/;
			point.y = 0;
			m_arPoints.push_back(point);
		}
		else
		{
			m_arPoints[m_arPoints.size() - 1].y = lParam/* - m_lY*/;
		}
		++m_nCountElementsPoint;
	}
	CSlice& CSlice::operator =(const CSlice& oSrc)
	{
		m_eRuler = oSrc.m_eRuler;
		m_arPoints.clear();

		for (size_t nIndex = 0; nIndex < oSrc.m_arPoints.size(); ++nIndex)
		{
			m_arPoints.push_back(oSrc.m_arPoints[nIndex]);
		}
		return (*this);
	}
	double CSlice::GetAngle(double fCentreX, double fCentreY, double fX, double fY)
	{
		//  -   + (..  )
		double dX = fX - fCentreX;
		double dY = fY - fCentreY;

		double modDX = abs(dX);
		double modDY = abs(dY);

		if ((modDX < 0.01) && (modDY < 0.01))
		{
			return 0;
		}
		if ((modDX < 0.01) && (dY < 0))
		{
			return -90;
		}
		else if (modDX < 0.01)
		{
			return 90;
		}
		if ((modDY < 0.01) && (dX < 0))
		{
			return 180;
		}
		else if (modDY < 0.01)
		{
			return 0;
		}

		double fAngle = atan(dY / dX);
		fAngle *= (180 / M_PI);
		if (dX > 0 && dY > 0)
		{
			return fAngle;
		}
		else if (dX > 0 && dY < 0)
		{
			return fAngle;
		}
		else if (dX < 0 && dY > 0)
		{
			//return fAngle + 180;
			return 180 + fAngle;
		}
		else
		{
			//return fAngle + 180;
			return fAngle - 180;
		}
	}
	double CSlice::GetSweepAngle(const double& angleStart, const double& angleEnd)
	{
		if (angleStart >= angleEnd)
			return angleEnd - angleStart;
		else
			return angleEnd - angleStart - 360;
	}
	void CSlice::ApplyElliptical(bool& bIsX, double& angleStart, double& angleSweet,
		double& Left, double& Top, double& Width, double& Height, const CGeomShapeInfo::CPointD& pointCur)
	{
		//   (x - y - x...)
		if (bIsX)
		{
			angleStart = -90;
			angleSweet = 90;

			if ((Width < 0) && (Height < 0))
			{
				angleStart = 90;
				Width *= -1;
				Height *= -1;
				Left = pointCur.dX - Width / 2;
				Top = pointCur.dY - Height;
			}
			else if ((Width < 0) && (Height > 0))
			{
				angleStart = -90;
				angleSweet = -90;
				Width *= -1;
				Left = pointCur.dX - Width / 2;
				Top = pointCur.dY;
			}
			else if ((Width > 0) && (Height < 0))
			{
				angleStart = 90;
				angleSweet = -90;
				Height *= -1;
				Left = pointCur.dX - Width / 2;
				Top = pointCur.dY - Height;
			}
			else
			{
				Left = pointCur.dX - Width / 2;
				Top = pointCur.dY;
			}
		}
		else
		{
			angleStart = 180;
			angleSweet = -90;

			if ((Width < 0) && (Height < 0))
			{
				angleStart = 0;
				Width *= -1;
				Height *= -1;
				Left = pointCur.dX - Width;
				Top = pointCur.dY - Height / 2;
			}
			else if ((Width < 0) && (Height > 0))
			{
				angleStart = 0;
				angleSweet = 90;
				Width *= -1;
				Left = pointCur.dX - Width;
				Top = pointCur.dY - Height / 2;
			}
			else if ((Width > 0) && (Height < 0))
			{
				angleStart = 180;
				angleSweet = 90;
				Height *= -1;
				Left = pointCur.dX;
				Top = pointCur.dY - Height / 2;
			}
			else
			{
				Left = pointCur.dX;
				Top = pointCur.dY - Height / 2;
			}
		}
		bIsX = !bIsX;
	}
	void CSlice::ApplyLimo(CGeomShapeInfo& pGeomInfo, double& lX, double& lY)
	{
		if ((0 == pGeomInfo.m_dLimoX) || (0 == pGeomInfo.m_dLimoY))
			return;

		double dAspect = (double)pGeomInfo.m_dLimoX / pGeomInfo.m_dLimoY;
		double lWidth  = (dAspect * pGeomInfo.m_dHeight);

		if (lWidth < pGeomInfo.m_dWidth)
		{
			// LimoX
			double lXc = pGeomInfo.m_dLeft + pGeomInfo.m_dWidth / 2;
			if ((lX > lXc) || ((lX == lXc) && (pGeomInfo.m_oCurPoint.dX >= lXc)))
			{
				double lXNew = pGeomInfo.m_dLeft + ((lWidth / pGeomInfo.m_dWidth) * (lX - pGeomInfo.m_dLeft));
				lXNew += (pGeomInfo.m_dWidth - lWidth);
				lX = lXNew;
			}
			//if (lX >= lXc)
			//{
			//	LONG lXNew = pGeomInfo->m_lLeft + (LONG)(((double)lWidth / pGeomInfo->m_lWidth) * (lX - pGeomInfo->m_lLeft));
			//	if (pGeomInfo->m_oCurPoint.x >= lXc)
			//	{
			//		lXNew += (pGeomInfo->m_lWidth - lWidth);
			//	}
			//	lX = lXNew;
			//}
		}
		else if (lWidth != pGeomInfo.m_dWidth)
		{
			// LimoY
			double lHeight = (pGeomInfo.m_dWidth / dAspect);
			double lYc = pGeomInfo.m_dTop + pGeomInfo.m_dHeight / 2;
			if ((lY > lYc) || ((lY == lYc) && (pGeomInfo.m_oCurPoint.dY >= lYc)))
			{
				double lYNew = pGeomInfo.m_dTop + ((lHeight / pGeomInfo.m_dHeight) * (lY - pGeomInfo.m_dTop));
				lYNew += (pGeomInfo.m_dHeight - lHeight);
				lY = lYNew;
			}
		}
	}
	void CSlice::Bez2_3(std::vector<CGeomShapeInfo::CPointD>& oArray, RulesType& eType)
	{
		if (rtQuadrBesier == eType)
		{
			eType = rtCurveTo;
		}
		else if (rtOOXMLQuadBezTo == eType)
		{
			eType = rtOOXMLCubicBezTo;
		}
		else
		{
			return;
		}

		std::vector<CGeomShapeInfo::CPointD> arOld;
		arOld.insert(arOld.end(),oArray.begin(), oArray.end());

		oArray.clear();

		size_t nStart	= 0;
		size_t nEnd	= 2;

		size_t nCount = arOld.size();
		while (nStart < (nCount - 1))
		{
			if (2 >= (nCount - nStart))
			{
				// по идее такого быть не может
				for (size_t i = nStart; i < nCount; ++i)
				{
					oArray.push_back(arOld[i]);
				}

				nStart = nCount;
				break;
			}

			if (4 == (nCount - nStart))
			{
				// ничего не поделаешь... делаем кривую третьего порядка
				oArray.push_back(arOld[nStart]);
				oArray.push_back(arOld[nStart + 1]);
				oArray.push_back(arOld[nStart + 2]);
				oArray.push_back(arOld[nStart + 3]);

				nStart += 4;
				break;
			}

			// значит есть еще
			CGeomShapeInfo::CPointD mem1;
			mem1.dX = (arOld[nStart].dX + 2 * arOld[nStart + 1].dX) / 3.0;
			mem1.dY = (arOld[nStart].dY + 2 * arOld[nStart + 1].dY) / 3.0;

			CGeomShapeInfo::CPointD mem2;
			mem2.dX = (2 * arOld[nStart + 1].dX + arOld[nStart + 2].dX) / 3.0;
			mem2.dY = (2 * arOld[nStart + 1].dY + arOld[nStart + 2].dY) / 3.0;

			oArray.push_back(mem1);
			oArray.push_back(mem2);
			oArray.push_back(arOld[nStart + 2]);

			nStart += 2;
		}
	}

	CPartPath::CPartPath() : m_arSlices()
	{
		m_bFill		= true;
		m_bStroke	= true;

		width		= ShapeSizeVML;
		height		= ShapeSizeVML; //43200?
	}
	void CPartPath::FromXML(std::wstring strPath, NSCustomShapesConvert::CFormulasManager& pManager)
	{
		NSStringUtils::CheckPathOn_Fill_Stroke(strPath, m_bFill, m_bStroke);
		std::vector<std::wstring> oArray;

		NSStringUtils::ParsePath2(strPath, &oArray);

		ParamType eParamType = ptValue;
		RulesType eRuler = rtEnd;
		LONG lValue;
		bool bRes = true;

		for (size_t nIndex = 0; nIndex < oArray.size(); ++nIndex)
		{
			lValue = GetValue(oArray[nIndex], eParamType, bRes);
			if (bRes)
			{
				switch (eParamType)
				{
				case ptFormula: { lValue = pManager.m_arResults[lValue]; break; }
				case ptAdjust:  { lValue = (*(pManager.m_pAdjustments))[lValue]; break; }
				default: break;
				};
				if (0 != m_arSlices.size())
				{
					m_arSlices[m_arSlices.size() - 1].AddParam(lValue);
				}
			}
			else
			{
				eRuler = GetRuler(oArray[nIndex], bRes);
				if (bRes)
				{
					if (rtNoFill == eRuler)
					{
						m_bFill = false;
					}
					else if (rtNoStroke == eRuler)
					{
						m_bStroke = false;
					}
					else
					{
						CSlice oSlice(eRuler);
						m_arSlices.push_back(oSlice);
					}
				}
			}
		}
	}
	CPartPath& CPartPath::operator =(const CPartPath& oSrc)
	{
		m_bFill = oSrc.m_bFill;
		m_bStroke = oSrc.m_bStroke;

		width = oSrc.width;
		height = oSrc.height;

		m_arSlices.clear();

		for (size_t nIndex = 0; nIndex < oSrc.m_arSlices.size(); ++nIndex)
		{
			m_arSlices.push_back(oSrc.m_arSlices[nIndex]);
		}
		return (*this);
	}

	void CPath::FromXML(std::wstring strPath, NSCustomShapesConvert::CFormulasManager& pManager)
	{
		m_arParts.clear();
		std::vector<std::wstring> oArray;

		NSStringUtils::ParseString(_T("e"), strPath, oArray);

		for (size_t nIndex = 0; nIndex < oArray.size(); ++nIndex)
		{
			CPartPath oPath;
			m_arParts.push_back(oPath);
			m_arParts.back().FromXML(oArray[nIndex], pManager);
		}
	}
	CPath& CPath::operator =(const CPath& oSrc)
	{
		m_arParts.clear();

		for (size_t nIndex = 0; nIndex < oSrc.m_arParts.size(); ++nIndex)
		{
			m_arParts.push_back(oSrc.m_arParts[nIndex]);
		}
		return (*this);
	}
	void CPath::SetCoordsize(LONG lWidth, LONG lHeight)
	{
		for (size_t nIndex = 0; nIndex < m_arParts.size(); ++nIndex)
		{
			m_arParts[nIndex].width		= lWidth;
			m_arParts[nIndex].height	= lHeight;
		}
	}
}

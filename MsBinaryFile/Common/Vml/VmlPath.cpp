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

#include "VmlPath.h"

namespace ODRAW
{    
	CSlice::CSlice(RulesType eType, LONG x, LONG y)
	{
		m_eRuler = eType;
		m_nCountElementsPoint = 0;

		m_lX = x;
		m_lY = y;
	}
	void CSlice::AddParam(LONG lParam)
	{
		long lPoint = m_nCountElementsPoint % 2;
		if (0 == lPoint)
		{
			Aggplus::POINT point;
			point.x = lParam;
			if (m_eRuler !=  rtRMoveTo && m_eRuler !=  rtRLineTo && m_eRuler !=  rtRCurveTo)
			{
				point.x -= m_lX;
			}
			point.y = 0;
			m_arPoints.push_back(point);
		}
		else
		{
			m_arPoints.back().y = lParam;
			if (m_eRuler !=  rtRMoveTo && m_eRuler !=  rtRLineTo && m_eRuler !=  rtRCurveTo)
			{
				m_arPoints.back().y -= m_lY;
			}
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
	void CSlice::FromXML(XmlUtils::CXmlNode& Node, NSGuidesOOXML::CFormulaManager& pManager, double WidthKoef, double HeightKoef)
	{
		m_arPoints.clear();
		bool bRes = true;
		m_eRuler = GetRuler(Node.GetName(), bRes);
		if((m_eRuler != rtOOXMLClose) && (m_eRuler != rtOOXMLEnd))
		{
			if(m_eRuler != rtOOXMLArcTo)
			{
				std::vector<XmlUtils::CXmlNode> list;
				Node.GetNodes(_T("pt"), list);
				for(long i = 0; i < GetCountPoints(m_eRuler); i++)
				{
					Aggplus::POINT lpoint;
					XmlUtils::CXmlNode & pt = list[i];

					lpoint.x = (long)(pManager.GetValue(pt.GetAttribute(_T("x")))*WidthKoef);
					lpoint.y = (long)(pManager.GetValue(pt.GetAttribute(_T("y")))*HeightKoef);
					m_arPoints.push_back(lpoint);
				}
			}
			else
			{
				Aggplus::POINT size;
				size.x = (long)(pManager.GetValue(Node.GetAttribute(_T("wR")))*WidthKoef);
				size.y = (long)(pManager.GetValue(Node.GetAttribute(_T("hR")))*HeightKoef);
				m_arPoints.push_back(size);
				double stAng = pManager.GetValue(Node.GetAttribute(_T("stAng")));
				double swAng = pManager.GetValue(Node.GetAttribute(_T("swAng")));
				double stAng2 = atan2(HeightKoef * sin(stAng * RadKoef), WidthKoef * cos(stAng * RadKoef));
				double swAng2 = atan2(HeightKoef * sin((stAng + swAng) * RadKoef), WidthKoef * cos((stAng + swAng) * RadKoef)) - stAng2;
				Aggplus::POINT angle;
				angle.x = (long)(stAng2/RadKoef);//pManager.GetValue(Node.GetAttribute(_T("stAng")));
				angle.y = (long)(swAng2/RadKoef);//pManager.GetValue(Node.GetAttribute(_T("swAng")));
				if((angle.y > 0) && (swAng < 0)) angle.y = angle.y - 21600000;
				if((angle.y < 0) && (swAng > 0)) angle.y = angle.y + 21600000;
				if(angle.y == 0) angle.y = 21600000;
				//angle.x = angle.x / 60000;
				//angle.y = angle.y / 60000;
				m_arPoints.push_back(angle);
			}
		}
	}
	void CSlice::ToRenderer(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, long w, long h, NSBaseShape::ClassType ClassType)
	{
		switch(ClassType)
		{
		case NSBaseShape::pptx:
		{
			if (rtOOXMLClose == m_eRuler)
			{
				pRenderer->AddRuler(rtClose);
				return;
			}
			if (rtOOXMLEnd == m_eRuler)
			{
				pRenderer->AddRuler(rtEnd);
				return;
			}

			double dLeft	= pGeomInfo.m_dLeft;
			double dTop		= pGeomInfo.m_dTop;

			double dKoefX = (std::max)(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight) / ShapeSize;
			double dKoefY = (std::max)(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight) / ShapeSize;

			switch (m_eRuler)
			{
			case rtOOXMLMoveTo:
			{
				pRenderer->AddRuler(rtMoveTo);

				pGeomInfo.m_oCurPoint.dX = 0;
				pGeomInfo.m_oCurPoint.dY = 0;
			}
			case rtOOXMLLineTo:
			case rtOOXMLCubicBezTo:
			{
				if (rtOOXMLLineTo == m_eRuler)
					pRenderer->AddRuler(rtLineTo);
				else if (rtOOXMLCubicBezTo == m_eRuler)
					pRenderer->AddRuler(rtCurveTo);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
					double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

					pRenderer->AddPoint(lX, lY);

					pGeomInfo.m_oCurPoint.dX = lX;
					pGeomInfo.m_oCurPoint.dY = lY;
				}
				break;
			}
			case rtOOXMLQuadBezTo:
			{
				std::vector<CGeomShapeInfo::CPointD> arPoints;
				arPoints.push_back(pGeomInfo.m_oCurPoint);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					CGeomShapeInfo::CPointD oPoint;

					oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
					oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

					arPoints.push_back(oPoint);
				}

				Bez2_3(arPoints, m_eRuler);

				pRenderer->AddRuler(rtCurveTo);

				size_t nCountNew = arPoints.size();
				if (0 < nCountNew)
				{
					pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
				}

				for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
				{
					pRenderer->AddPoint(arPoints[nIndex].dX, arPoints[nIndex].dY);
				}
				break;
			}
			case rtOOXMLArcTo:
			{
				pRenderer->AddRuler(rtAngleEllipseTo);

				double lX = pGeomInfo.m_oCurPoint.dX;
				double lY = pGeomInfo.m_oCurPoint.dY;
				double dAngleP = atan2(m_arPoints[0].x * sin(RadKoef * m_arPoints[1].x), m_arPoints[0].y * cos(RadKoef * m_arPoints[1].x));
				lX += (m_arPoints[0].x * dKoefX) * (-cos(dAngleP));//(-cos(RadKoef * m_arPoints[1].x));
				lY += (m_arPoints[0].y * dKoefY) * (-sin(dAngleP));//(-sin(RadKoef * m_arPoints[1].x));

				pRenderer->AddPoint(lX, lY);

				double cx = lX;
				double cy = lY;

				lX = (2 * dKoefX * m_arPoints[0].x);
				lY = (2 * dKoefY * m_arPoints[0].y);

				pRenderer->AddPoint(lX, lY);

				double dAngle = (m_arPoints[1].y + m_arPoints[1].x)* RadKoef;
				dAngleP = atan2(/*dKoefX */ m_arPoints[0].x * sin(dAngle), /*dKoefY */ m_arPoints[0].y * cos(dAngle));

				lX = (double)m_arPoints[1].x/60000.0;
				lY = (double)m_arPoints[1].y/60000.0;

				pGeomInfo.m_oCurPoint.dX = cx + (m_arPoints[0].x * dKoefX) * cos(dAngleP);
				pGeomInfo.m_oCurPoint.dY = cy + (m_arPoints[0].y * dKoefY) * sin(dAngleP);

				pRenderer->AddPoint(lX, lY);
				break;
			}
			default:
				break;
			};
			break;
		}
		case NSBaseShape::ppt:
		{
			if ((rtClose == m_eRuler) || (rtNoFill == m_eRuler) ||
					(rtNoStroke == m_eRuler) || (rtEnd == m_eRuler))
			{
				pRenderer->AddRuler(m_eRuler);
				return;
			}

			double dLeft	= pGeomInfo.m_dLeft;
			double dTop		= pGeomInfo.m_dTop;

			double dKoefX	= pGeomInfo.m_dWidth / w;
			double dKoefY	= pGeomInfo.m_dHeight / h;

			switch (m_eRuler)
			{
			case rtMoveTo:
			case rtLineTo:
			case rtCurveTo:
			case rtEllipticalQuadrX:
			case rtEllipticalQuadrY:
			{
				pRenderer->AddRuler(m_eRuler);
				if (rtMoveTo == m_eRuler)
				{
					pGeomInfo.m_oCurPoint.dX = 0;
					pGeomInfo.m_oCurPoint.dY = 0;
				}
				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
					double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

					pRenderer->AddPoint(lX, lY);

					pGeomInfo.m_oCurPoint.dX = lX;
					pGeomInfo.m_oCurPoint.dY = lY;
				}
				break;
			}
			case rtQuadrBesier:
			{
				std::vector<CGeomShapeInfo::CPointD> arPoints;
				arPoints.push_back(pGeomInfo.m_oCurPoint);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					CGeomShapeInfo::CPointD oPoint;

					oPoint.dX = dLeft + (dKoefX * m_arPoints[nIndex].x);
					oPoint.dY = dTop  + (dKoefY * m_arPoints[nIndex].y);

					arPoints.push_back(oPoint);
				}

				Bez2_3(arPoints, m_eRuler);

				pRenderer->AddRuler(rtCurveTo);

				size_t nCountNew = arPoints.size();
				if (0 < nCountNew)
				{
					pGeomInfo.m_oCurPoint = arPoints[nCountNew - 1];
				}

				for (size_t nIndex = 0; nIndex < nCountNew; ++nIndex)
				{
					pRenderer->AddPoint(arPoints[nIndex].dX, arPoints[nIndex].dY);
				}
				break;
			}
			case rtArcTo:
			case rtClockwiseArcTo:
			{
				pRenderer->AddRuler(m_eRuler);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
					double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

					pRenderer->AddPoint(lX, lY);

					if (nIndex % 4 != 1)
					{
						pGeomInfo.m_oCurPoint.dX = lX;
						pGeomInfo.m_oCurPoint.dY = lY;
					}
				}
				break;
			}
			case rtArc:
			case rtClockwiseArc:
			{
				pRenderer->AddRuler(m_eRuler);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					double lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
					double lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

					pRenderer->AddPoint(lX, lY);

					if (nIndex % 4 > 1)
					{
						pGeomInfo.m_oCurPoint.dX = lX;
						pGeomInfo.m_oCurPoint.dY = lY;
					}
				}
				break;
			}
			case rtRMoveTo:
			case rtRLineTo:
			case rtRCurveTo:
			{
				pRenderer->AddRuler(m_eRuler);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					double lX = pGeomInfo.m_oCurPoint.dX + (dKoefX * m_arPoints[nIndex].x);
					double lY = pGeomInfo.m_oCurPoint.dY + (dKoefY * m_arPoints[nIndex].y);


					lX -= pGeomInfo.m_oCurPoint.dX;
					lY -= pGeomInfo.m_oCurPoint.dY;

					pRenderer->AddPoint(lX, lY);

					pGeomInfo.m_oCurPoint.dX += lX;
					pGeomInfo.m_oCurPoint.dY += lY;
				}
				break;
			}
			case rtAngleEllipseTo:
			case rtAngleEllipse:
			{
				pRenderer->AddRuler(m_eRuler);

				for (size_t nIndex = 0; nIndex < m_arPoints.size(); ++nIndex)
				{
					double lX = 0;
					double lY = 0;
					if (nIndex % 3 == 0)
					{
						lX = dLeft + (dKoefX * m_arPoints[nIndex].x);
						lY = dTop  + (dKoefY * m_arPoints[nIndex].y);

						pRenderer->AddPoint(lX, lY);
						continue;
					}
					else if (nIndex % 3 == 1)
					{
						lX = (2 * dKoefX * m_arPoints[nIndex].x);
						lY = (2 * dKoefY * m_arPoints[nIndex].y);

						pRenderer->AddPoint(lX, lY);
						continue;
					}
					else
					{
						double dCx = dLeft + (dKoefX * m_arPoints[nIndex - 2].x);
						double dCy = dTop  + (dKoefY * m_arPoints[nIndex - 2].y);

						double dRadX = dKoefX * m_arPoints[nIndex - 1].x;
						double dRadY = dKoefY * m_arPoints[nIndex - 1].y;
						double dAng = (m_arPoints[nIndex].x + m_arPoints[nIndex].y) / pow2_16;
						dAng *= (M_PI / pow2_16);

						double dcos = cos(dAng);
						double dsin = sin(dAng);
						double rad = 1 / sqrt(dRadX * dRadX * dcos * dcos + dRadY * dRadY * dsin * dsin);

						pGeomInfo.m_oCurPoint.dX = (dCx + rad * dcos);
						pGeomInfo.m_oCurPoint.dY = (dCy + rad * dsin);

						lX = m_arPoints[nIndex].x / pow2_16;
						lY = m_arPoints[nIndex].y / pow2_16;
					}

					pRenderer->AddPoint(-lX, -lY/*GetSweepAngle(lX, lY)*/);
				}
				break;
			}
			default:
				break;
			};
			break;
		}
		};

	}

	CPartPath::CPartPath() : m_arSlices()
	{
		m_bFill = true;
		m_bStroke = true;
  
		//width = height = 43200;

		// fix shape scaling xls
		width		= ShapeSizeVML;
		height		= ShapeSizeVML;

		x = y = 0;

	}
	void CPartPath::FromXML(std::wstring strPath, NSGuidesVML::CFormulasManager& pManager) //vml
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
				case ptFormula:
				{
					if (0 <= lValue && lValue < (LONG)pManager.m_arResults.size())
					{
						lValue = pManager.m_arResults[lValue];
					}
					else
					{
						lValue = 0;
					}
					break;
				}
				case ptAdjust:
				{
					if (0 <= lValue && lValue < (LONG)pManager.m_pAdjustments->size())
					{
						lValue = (*(pManager.m_pAdjustments))[lValue];
					}
					else
					{
						lValue = 0;
					}
					break;
				}
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
						CSlice oSlice(eRuler, x, y);
						m_arSlices.push_back(oSlice);
					}
				}
			}
		}
	}
	CPartPath& CPartPath::operator=(const CPartPath& oSrc)
	{
		m_bFill = oSrc.m_bFill;
		m_bStroke = oSrc.m_bStroke;

		width = oSrc.width;
		height = oSrc.height;

		x = oSrc.x;
		y = oSrc.y;

		m_arSlices.clear();
		for (size_t nIndex = 0; nIndex < oSrc.m_arSlices.size(); ++nIndex)
		{
			m_arSlices.push_back(oSrc.m_arSlices[nIndex]);
		}
		return (*this);
	}
	void CPartPath::FromXML(XmlUtils::CXmlNode& PathNode, NSGuidesOOXML::CFormulaManager& pManager)// oox
	{
		m_bFill = PathNode.GetAttribute(_T("fill"), _T("norm")) != _T("none");
		std::wstring stroke = PathNode.GetAttribute(_T("stroke"), _T("true"));
		m_bStroke = (stroke == _T("true")) || (stroke == _T("1"));

		width = (long)XmlUtils::GetInteger(PathNode.GetAttribute(_T("w"), _T("0")));
		height = (long)XmlUtils::GetInteger(PathNode.GetAttribute(_T("h"), _T("0")));

		if(width == 0) width = (long)pManager.GetWidth();
		if(height == 0) height = (long)pManager.GetHeight();

		std::vector<XmlUtils::CXmlNode> list;
		PathNode.GetNodes(_T("*"), list);

		for(size_t i = 0; i < list.size(); i++)
		{
			CSlice slice;
			XmlUtils::CXmlNode & node = list[i];

			slice.FromXML(node, pManager, pManager.GetWidth()/width, pManager.GetHeight()/height);

			m_arSlices.push_back(slice);
		}

		//CSlice EndSlice;
		//EndSlice.m_eRuler = rtEnd;
		//m_arSlices.push_back(EndSlice);
	}
	std::wstring CPartPath::ToXml(CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType)
	{
		return _T("");
	}
	void CPartPath::ToRenderer(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType)
	{
		pRenderer->m_bStroke	= m_bStroke;
		pRenderer->m_bFill		= m_bFill;

		pRenderer->Pen			= pPen;
		pRenderer->Brush		= pFore;

		//pRenderer->SetCommandParams(pGeomInfo.m_dRotate, pGeomInfo.m_dLeft, pGeomInfo.m_dTop,
		//	pGeomInfo.m_dWidth, pGeomInfo.m_dHeight, pGeomInfo.GetFlags());

		//pRenderer->BeginCommand(c_nPathType);

		for (size_t nIndex = 0; nIndex < m_arSlices.size(); ++nIndex)
		{
			m_arSlices[nIndex].ToRenderer(pRenderer, pGeomInfo, width, height, ClassType);
		}

		//pRenderer->PathCommandEnd();
		//pRenderer->EndCommand(c_nPathType);
		//pRenderer->SetCommandParams(0, -1, -1, -1, -1, 0);
	}
	void CPartPath::ToRendererOOX(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, NSBaseShape::ClassType ClassType)
	{
		pRenderer->m_bStroke	= m_bStroke;
		pRenderer->m_bFill		= m_bFill;
  
		for (size_t nIndex = 0; nIndex < m_arSlices.size(); ++nIndex)
		{
			m_arSlices[nIndex].ToRenderer(pRenderer, pGeomInfo, width, height, ClassType);
		}
	}

	CPath::CPath() : m_lX(0), m_lY(0) {}
	void CPath::FromXML(std::wstring strPath, NSGuidesVML::CFormulasManager& pManager)
	{
		m_arParts.clear();
		std::vector<std::wstring> oArray;
		boost::algorithm::split(oArray, strPath, boost::algorithm::is_any_of(L"e"), boost::algorithm::token_compress_on);

		for (size_t nIndex = 0; nIndex < oArray.size(); ++nIndex)
		{
			if (oArray[nIndex].empty()) continue;

			CPartPath part;

			part.x = m_lX;
			part.y = m_lY;

			m_arParts.push_back(part);
			m_arParts.back().FromXML(oArray[nIndex], pManager);
		}
	}
	CPath& CPath::operator=(const CPath& oSrc)
	{
		m_arParts.clear();
		for (size_t nIndex = 0; nIndex < oSrc.m_arParts.size(); ++nIndex)
		{
			m_arParts.push_back(oSrc.m_arParts[nIndex]);
		}
		m_lX = oSrc.m_lX;
		m_lY = oSrc.m_lY;

		return (*this);
	}
	void CPath::FromXML(std::vector<XmlUtils::CXmlNode>& list, NSGuidesOOXML::CFormulaManager& pManager)
	{
		m_arParts.clear();
		for(size_t i = 0; i < list.size(); i++)
		{
			XmlUtils::CXmlNode & path = list[i];

			CPartPath part;

			part.x = m_lX;
			part.y = m_lY;

			m_arParts.push_back(part);
			m_arParts.back().FromXML(path, pManager);
	   }
	}
	std::wstring CPath::ToXml(CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType)
	{
		std::wstring strResult = _T("");
		for (size_t nIndex = 0; nIndex < m_arParts.size(); ++nIndex)
		{
			strResult += m_arParts[nIndex].ToXml(pGeomInfo, dStartTime, dEndTime, pPen, pFore, ClassType);
		}
		return strResult;
	}
	void CPath::ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime,
					double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType)
	{
		CGraphicPath oPath;
		oPath.Pen	= pPen;
		oPath.Brush	= pFore;

		for (size_t nIndex = 0; nIndex < m_arParts.size(); ++nIndex)
		{
			oPath.Clear();
			m_arParts[nIndex].ToRenderer(&oPath, pGeomInfo, dStartTime, dEndTime, pPen, pFore, ClassType);

			oPath.Draw(pRenderer);
		}
	}
	void CPath::SetCoordpos(LONG lX, LONG lY)
	{
		m_lX = lX;
		m_lY = lY;
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

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

#include "GraphicsPath.h"

namespace NSCustomShapesConvert
{
	CDoubleRect::CDoubleRect()
	{
		left	= 0;
		top		= 0;
		right	= 0;
		bottom	= 0;
	}
	CDoubleRect& CDoubleRect::operator=(const CDoubleRect& oSrc)
	{
		left	= oSrc.left;
		top		= oSrc.top;
		right	= oSrc.right;
		bottom	= oSrc.bottom;

		return *this;
	}
	CDoubleRect::CDoubleRect(const CDoubleRect& oSrc)
	{
		*this = oSrc;
	}

	CGeomShapeInfo::CPointD::CPointD()
	{
		dX = 0;
		dY = 0;
	}
	CGeomShapeInfo::CPointD& CGeomShapeInfo::CPointD::operator= (const CPointD& oSrc)
	{
		dX = oSrc.dX;
		dY = oSrc.dY;

		return *this;
	}
	CGeomShapeInfo::CPointD::CPointD(const CPointD& oSrc)
	{
		*this = oSrc;
	}

	CGeomShapeInfo::CGeomShapeInfo()
	{
		m_dLeft = 0;
		m_dTop = 0;
		m_dWidth = 720;
		m_dHeight = 576;

		m_dLimoX = 0;
		m_dLimoY = 0;

		m_oCurPoint.dX = 0;
		m_oCurPoint.dY = 0;

		m_dRotate = 0.0;
		m_bFlipH = false;
		m_bFlipV = false;

		m_lOriginalWidth	= 0;
		m_lOriginalHeight	= 0;
	}
	CGeomShapeInfo::~CGeomShapeInfo()
	{
	}
	CGeomShapeInfo& CGeomShapeInfo::operator =(const CGeomShapeInfo& oSrc)
	{
		m_dLeft		= oSrc.m_dLeft;
		m_dTop		= oSrc.m_dTop;
		m_dWidth	= oSrc.m_dWidth;
		m_dHeight	= oSrc.m_dHeight;

		m_dLimoX	= oSrc.m_dLimoX;
		m_dLimoY	= oSrc.m_dLimoY;

		m_oCurPoint = oSrc.m_oCurPoint;

		m_dRotate	= oSrc.m_dRotate;
		m_bFlipH	= oSrc.m_bFlipH;
		m_bFlipV	= oSrc.m_bFlipV;

		m_lOriginalWidth	= oSrc.m_lOriginalWidth;
		m_lOriginalHeight	= oSrc.m_lOriginalHeight;

		return (*this);
	}

	CDoublePoint::CDoublePoint()
	{
		dX = 0;
		dY = 0;
	}
	CDoublePoint& CDoublePoint::operator= (const CDoublePoint& oSrc)
	{
		dX = oSrc.dX;
		dY = oSrc.dY;

		return *this;
	}
	CDoublePoint::CDoublePoint(const CDoublePoint& oSrc)
	{
		*this = oSrc;
	}

	void CGraphicPath::InternalClear()
	{
		m_lFlags	= 0;
	}
	CGraphicPath::CGraphicPath()
	{
		InternalClear();
	}

	CGraphicPath::CPart::CPart() : m_eType(rtMoveTo), m_arPoints()
	{
	}
	CGraphicPath::CPart& CGraphicPath::CPart::operator=(const CPart& oSrc)
	{
		m_eType	= oSrc.m_eType;
		this->m_arPoints.clear();
		for (size_t nIndex = 0; nIndex < oSrc.m_arPoints.size(); ++nIndex)
		{
			this->m_arPoints.push_back(oSrc.m_arPoints[nIndex]);
		}
		return (*this);
	}
	CGraphicPath::CPart::~CPart()
	{
		this->m_arPoints.clear();
	}

	void CGraphicPath::CPart::ParseString(std::wstring strDelimeters, std::wstring strSource,
		std::vector<std::wstring>& pArrayResults, bool bIsCleared)
	{
		if (bIsCleared)
			pArrayResults.clear();

		std::wstring resToken;
		int curPos= 0;

		boost::algorithm::split(pArrayResults, strSource, boost::algorithm::is_any_of(strDelimeters), boost::algorithm::token_compress_on);

	}

	void CGraphicPath::CPart::CheckLastPoint(IRenderer* pRenderer, CDoublePoint& pointCur)
	{
		if (NULL == pRenderer)
			return;

		pRenderer->PathCommandGetCurrentPoint(&pointCur.dX, &pointCur.dY);
	}
	double CGraphicPath::CPart::GetAngle(double fCentreX, double fCentreY, double fX, double fY)
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
		fAngle *= double(180 / M_PI);
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

	void CGraphicPath::CPart::ApplyElliptical(bool& bIsX, double& angleStart, double& angleSweet,
		double& Left, double& Top, double& Width, double& Height, const CDoublePoint& pointCur)
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
				Left	= pointCur.dX - Width / 2;
				Top		= pointCur.dY - Height;
			}
			else if ((Width < 0) && (Height > 0))
			{
				angleStart = -90;
				angleSweet = -90;
				Width *= -1;
				Left	= pointCur.dX - Width / 2;
				Top		= pointCur.dY;
			}
			else if ((Width > 0) && (Height < 0))
			{
				angleStart = 90;
				angleSweet = -90;
				Height *= -1;
				Left	= pointCur.dX - Width / 2;
				Top		= pointCur.dY - Height;
			}
			else
			{
				Left	= pointCur.dX - Width / 2;
				Top		= pointCur.dY;
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
				Left	= pointCur.dX - Width;
				Top		= pointCur.dY - Height / 2;
			}
			else if ((Width < 0) && (Height > 0))
			{
				angleStart = 0;
				angleSweet = 90;
				Width *= -1;
				Left	= pointCur.dX - Width;
				Top		= pointCur.dY - Height / 2;
			}
			else if ((Width > 0) && (Height < 0))
			{
				angleStart = 180;
				angleSweet = 90;
				Height *= -1;
				Left	= pointCur.dX;
				Top		= pointCur.dY - Height / 2;
			}
			else
			{
				Left	= pointCur.dX;
				Top		= pointCur.dY - Height / 2;
			}
		}
		bIsX = !bIsX;
	}

	void CGraphicPath::CPart::GetSafearrayPoints(IRenderer* pRenderer, double** ppArray, size_t& nCountOut, CDoublePoint& pointCur, bool bR)
	{
		if (NULL == ppArray)
			return;

		size_t nCount = this->m_arPoints.size();

		nCountOut = 2 * (nCount + 1);

		double* pArray = new double [nCountOut];
		double* pBuffer = pArray;

		memset (pBuffer, 0, nCountOut * sizeof(double));

		*pBuffer = pointCur.dX; ++pBuffer;
		*pBuffer = pointCur.dY; ++pBuffer;

		if (bR)
		{
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				*pBuffer = (this->m_arPoints[nIndex].dX + pointCur.dX); ++pBuffer;
				*pBuffer = (this->m_arPoints[nIndex].dY + pointCur.dY); ++pBuffer;

				if (nIndex == (nCount - 1))
				{
					pointCur.dX += this->m_arPoints[nIndex].dX;
					pointCur.dY += this->m_arPoints[nIndex].dY;
				}
			}
		}
		else
		{
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				*pBuffer = this->m_arPoints[nIndex].dX; ++pBuffer;
				*pBuffer = this->m_arPoints[nIndex].dY; ++pBuffer;

				if (nIndex == (nCount - 1))
				{
					pointCur.dX = this->m_arPoints[nIndex].dX;
					pointCur.dY = this->m_arPoints[nIndex].dY;
				}
			}
		}
		*ppArray = pArray;

	}

	void CGraphicPath::CPart::Draw(IRenderer* pRenderer, CDoublePoint& pointCur)
	{
		switch (m_eType)
		{
		case rtMoveTo:
			{
				if (0 < this->m_arPoints.size())
				{
					pointCur.dX = this->m_arPoints[0].dX;
					pointCur.dY = this->m_arPoints[0].dY;
					pRenderer->PathCommandMoveTo(this->m_arPoints[0].dX, this->m_arPoints[0].dY);
				}
				break;
			}
		case rtLineTo:
			{
				double* pArray = NULL;
				size_t nCount = 0;
				GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur);

				if (NULL != pArray)
				{
					pRenderer->PathCommandLinesTo(pArray, (int)nCount /*this->m_arPoints.size()*/);
				}

				break;
			}
		case rtCurveTo:
			{
				double* pArray = NULL;
				size_t nCount = 0;
				GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur);

				if (NULL != pArray)
				{
					pRenderer->PathCommandCurvesTo (pArray, (int)nCount/*this->m_arPoints.size()*/);
				}

				break;
			}
		case rtClose:
			{
				pRenderer->PathCommandClose();
				break;
			}
		case rtRMoveTo:
			{
				if (0 < this->m_arPoints.size())
				{
					pointCur.dX = this->m_arPoints[0].dX + pointCur.dX;
					pointCur.dY = this->m_arPoints[0].dY + pointCur.dY;
					pRenderer->PathCommandMoveTo(pointCur.dX, pointCur.dY);
				}
				break;
			}
		case rtRLineTo:
			{
				double* pArray = NULL;
				size_t nCount = 0;
				GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur, TRUE);

				if (NULL != pArray)
				{
					pRenderer->PathCommandLinesTo(pArray, (int)nCount/*this->m_arPoints.size()*/);
				}

				break;
			}
		case rtRCurveTo:
			{
				double* pArray = NULL;
				size_t nCount = 0;
				GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur, TRUE);

				if (NULL != pArray)
				{
					pRenderer->PathCommandCurvesTo(pArray, (int)nCount/*this->m_arPoints.size()*/);
				}
				break;
			}
		case rtAngleEllipseTo:
			{
				int nFigure = 0;
				while ((nFigure + 3) <= (int)this->m_arPoints.size())
				{
					double nLeft	= this->m_arPoints[nFigure].dX - this->m_arPoints[nFigure + 1].dX / 2;
					double nTop		= this->m_arPoints[nFigure].dY - this->m_arPoints[nFigure + 1].dY / 2;

					pRenderer->PathCommandArcTo(nLeft, nTop,
						this->m_arPoints[nFigure + 1].dX, this->m_arPoints[nFigure + 1].dY,
						this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

					nFigure += 3;
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		case rtAngleEllipse:
			{
				pRenderer->PathCommandStart();
				int nFigure = 0;
				while ((nFigure + 3) <= (int)this->m_arPoints.size())
				{
					double nLeft	= this->m_arPoints[nFigure].dX - this->m_arPoints[nFigure + 1].dX / 2;
					double nTop		= this->m_arPoints[nFigure].dY - this->m_arPoints[nFigure + 1].dY / 2;

					pRenderer->PathCommandArcTo(nLeft, nTop,
						this->m_arPoints[nFigure + 1].dX, this->m_arPoints[nFigure + 1].dY,
						this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

					nFigure += 3;
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		case rtArc:
			{
				pRenderer->PathCommandStart();
				int nFigure = 0;
				while ((nFigure + 4) <= (int)this->m_arPoints.size())
				{
					double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
					double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

					double angleStart = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

					double angleEnd = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

					pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
						this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
						this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
						angleStart, GetSweepAngle(angleStart, angleEnd));

					nFigure += 4;
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		case rtArcTo:
			{
				int nFigure = 0;
				while ((nFigure + 4) <= (int)this->m_arPoints.size())
				{
					double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
					double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

					double angleStart = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

					double angleEnd = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

					pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
						this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
						this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
						angleStart, GetSweepAngle(angleStart, angleEnd));

					nFigure += 4;
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		case rtClockwiseArcTo:
			{
				int nFigure = 0;
				while ((nFigure + 4) <= (int)this->m_arPoints.size())
				{
					double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
					double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

					double angleStart = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

					double angleEnd = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

					pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
						this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
						this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
						angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

					nFigure += 4;
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		case rtClockwiseArc:
			{
				pRenderer->PathCommandStart();
				int nFigure = 0;
				while ((nFigure + 4) <= (int)this->m_arPoints.size())
				{
					double nCentreX = (this->m_arPoints[nFigure].dX + this->m_arPoints[nFigure + 1].dX) / 2;
					double nCentreY = (this->m_arPoints[nFigure].dY + this->m_arPoints[nFigure + 1].dY) / 2;

					double angleStart = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 2].dX, this->m_arPoints[nFigure + 2].dY);

					double angleEnd = GetAngle(nCentreX, nCentreY,
						this->m_arPoints[nFigure + 3].dX, this->m_arPoints[nFigure + 3].dY);

					pRenderer->PathCommandArcTo(this->m_arPoints[nFigure].dX, this->m_arPoints[nFigure].dY,
						this->m_arPoints[nFigure + 1].dX - this->m_arPoints[nFigure].dX,
						this->m_arPoints[nFigure + 1].dY - this->m_arPoints[nFigure].dY,
						angleStart, 360 + GetSweepAngle(angleStart, angleEnd));

					nFigure += 4;
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		case rtEllipticalQuadrX:
			{
				bool bIsX = true;
				CheckLastPoint(pRenderer, pointCur);

				size_t nCount = this->m_arPoints.size();
				for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
				{
					double x1 = pointCur.dX;
					double y1 = pointCur.dY;

					double x2 = this->m_arPoints[nIndex].dX;
					double y2 = this->m_arPoints[nIndex].dY;

					double dRadX = fabs(x1 - x2);
					double dRadY = fabs(y1 - y2);

					AddEllipticalQuadr(pRenderer, bIsX, x1, y1, x2, y2, dRadX, dRadY);

					pointCur.dX = x2;
					pointCur.dY = y2;
				}

				break;
			}
		case rtEllipticalQuadrY:
			{
				bool bIsX = false;
				CheckLastPoint(pRenderer, pointCur);

				size_t nCount = this->m_arPoints.size();
				for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
				{
					double x1 = pointCur.dX;
					double y1 = pointCur.dY;

					double x2 = this->m_arPoints[nIndex].dX;
					double y2 = this->m_arPoints[nIndex].dY;

					double dRadX = fabs(x1 - x2);
					double dRadY = fabs(y1 - y2);

					AddEllipticalQuadr(pRenderer, bIsX, x1, y1, x2, y2, dRadX, dRadY);

					pointCur.dX = x2;
					pointCur.dY = y2;
				}

				break;
			}
		case rtQuadrBesier:
			{
				double* pArray = NULL;
				size_t nCount = 0;
				GetSafearrayPoints(pRenderer, &pArray, nCount, pointCur, TRUE);

				if (NULL != pArray)
				{
					pRenderer->PathCommandLinesTo(pArray, (int)nCount/*this->m_arPoints.size()*/);
				}

				CheckLastPoint(pRenderer, pointCur);
				break;
			}
		default: break;
		};
	}

	void CGraphicPath::CPart::AddEllipticalQuadr(IRenderer*& pRenderer, bool& bIsX, double& x1, double& y1, double& x2, double& y2, double& dRadX, double& dRadY)
			{
				if (bIsX)
				{
					if ((x2 >= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1, 2 * dRadX, 2 * dRadY, -90, 90);
					else if ((x2 >= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1 - 2 * dRadY, 2 * dRadX, 2 * dRadY, 90, -90);
					else if ((x2 <= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1, 2 * dRadX, 2 * dRadY, -90, -90);
					else if ((x2 <= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1 - dRadX, y1 - 2 * dRadY, 2 * dRadX, 2 * dRadY, 90, 90);
				}
				else
				{
					if ((x2 >= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1, y1 - dRadY, 2 * dRadX, 2 * dRadY, 180, -90);
					else if ((x2 >= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1, y1 - dRadY, 2 * dRadX, 2 * dRadY, 180, 90);
					else if ((x2 <= x1) && (y2 >= y1))
						pRenderer->PathCommandArcTo(x1 - 2 * dRadX, y1 - dRadY, 2 * dRadX, 2 * dRadY, 0, 90);
					else if ((x2 <= x1) && (y2 <= y1))
						pRenderer->PathCommandArcTo(x1 - 2 * dRadX, y1 - dRadY, 2 * dRadX, 2 * dRadY, 0, -90);
				}
				bIsX = !bIsX;
			}


	void CGraphicPath::AddRuler(const RulesType& eType)
	{
		size_t lCount = m_arParts.size();

		CPart oPart;
		oPart.m_eType = eType;

		m_arParts.push_back(oPart);
	}
	void CGraphicPath::AddPoint(const double& x, const double& y)
	{
		size_t lCount = m_arParts.size();
		if (0 != lCount)
		{
			CDoublePoint point;
			point.dX = x;
			point.dY = y;
			m_arParts[lCount - 1].m_arPoints.push_back(point);
		}
	}

	void CGraphicPath::Clear()
	{
		m_arParts.clear();
	}
}

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
#pragma once

#include <string>
#include <boost/algorithm/string.hpp>

#include "../../../DesktopEditor/graphics/GraphicsPath.h"

#define _USE_MATH_DEFINES
#include <math.h>//M_PI
#include <vector>

namespace NSCustomShapesConvert
{
	const double c_dMasterUnitsToInchKoef		= 1.0 / 576;
	const double c_dInchToMillimetreKoef		= 25.4;
	const double c_dMasterUnitsToMillimetreKoef = c_dMasterUnitsToInchKoef * c_dInchToMillimetreKoef;

	class CDoubleRect
	{
	public:
		double left;
		double top;
		double right;
		double bottom;

	public:
		CDoubleRect();
		CDoubleRect& operator=(const CDoubleRect& oSrc);
		CDoubleRect(const CDoubleRect& oSrc);

		inline bool IsEqual(const CDoubleRect& oSrc, double dEps = 0.01)
		{
			return ((fabs(left - oSrc.left) < dEps) && (fabs(top - oSrc.top) < dEps) && 
				(fabs(right - oSrc.right) < dEps) && (fabs(bottom - oSrc.bottom) < dEps));
		}
		inline double GetWidth() const
		{
			return right - left;
		}
		inline double GetHeight() const
		{
			return bottom - top;
		}
		inline void Scale(const double& dKoef)
		{
			left	*= dKoef;
			top		*= dKoef;
			right	*= dKoef;
			bottom	*= dKoef;
		}
	};

	class CGeomShapeInfo
	{
	public:
		class CPointD
		{
		public:
			double dX;
			double dY;

		public:
			CPointD();
			CPointD& operator= (const CPointD& oSrc);
			CPointD(const CPointD& oSrc);
		};

	public:
		double m_dLeft;
		double m_dTop;
		double m_dWidth;
		double m_dHeight;

		double m_dLimoX;
		double m_dLimoY;

		// нужен для регулировки по аспекту (limo)
		CPointD m_oCurPoint;

		double m_dRotate;
		bool m_bFlipH;
		bool m_bFlipV;

		LONG m_lOriginalWidth;
		LONG m_lOriginalHeight;

	public:
		CGeomShapeInfo();
		~CGeomShapeInfo();
		CGeomShapeInfo& operator =(const CGeomShapeInfo& oSrc);

		inline void SetBounds(const CDoubleRect& oRect)
		{
			m_dLeft		= oRect.left;
			m_dTop		= oRect.top;
			m_dWidth	= oRect.GetWidth();	
			m_dHeight	= oRect.GetHeight();
		}

		inline LONG GetFlags()
		{
			LONG lFlags = 0;
			if (m_bFlipH)
				lFlags |= 0x0001;
			if (m_bFlipV)
				lFlags |= 0x0002;

			return lFlags;
		}
	};
	class CDoublePoint
	{
	public:
		double dX;
		double dY;

	public:
		CDoublePoint();
		CDoublePoint& operator= (const CDoublePoint& oSrc);
		CDoublePoint(const CDoublePoint& oSrc);
	};

	enum RulesType
	{
// VML
		rtLineTo			= 0,	// 2*
		rtCurveTo			= 1,	// 6*
		rtMoveTo			= 2,	// 2
		
		rtClose				= 3,	// 0
		rtEnd				= 4,	// 0
		
		rtRMoveTo			= 5,	// 2*
		rtRLineTo			= 6,	// 2*
		rtRCurveTo			= 7,	// 6*
		
		rtNoFill			= 8,	// 0
		rtNoStroke			= 9,	// 0
		
		rtAngleEllipseTo	= 10,	// 6*
		rtAngleEllipse		= 11,	// 6*
		
		rtArc				= 12,	// 8* 
		rtArcTo				= 13,	// 8*
		
		rtClockwiseArcTo	= 14,	// 8*
		rtClockwiseArc		= 15,	// 8*
		
		rtEllipticalQuadrX	= 16,	// 2*
		rtEllipticalQuadrY	= 17,	// 2*

		rtQuadrBesier		= 18,	// 2 + 2*

		rtFillColor			= 20, 
		rtLineColor			= 21,

		// OOXML
		rtOOXMLMoveTo		= 0 + 100,	// 2
		rtOOXMLLineTo		= 1 + 100,	// 2*
		rtOOXMLCubicBezTo	= 2 + 100,	// 6*
		rtOOXMLArcTo		= 3 + 100,	// 8*	
		rtOOXMLQuadBezTo	= 4 + 100,	// 2 + 2*
		rtOOXMLClose		= 5 + 100,	// 0
		rtOOXMLEnd			= 6	+ 100	// 0
	};

	
	class CGraphicPath
	{
	public:
		virtual void InternalClear();

		CGraphicPath();

		class CPart
		{
		public:
			RulesType					m_eType;
            std::vector<CDoublePoint>	m_arPoints;

		public:
			CPart();
			CPart& operator=(const CPart& oSrc);
			~CPart();

			//void FromXmlNode(XmlUtils::CXmlNode& oNode)
			//{
			//	std::wstring strName = oNode.GetAttribute(_T("name"));
			//	if (_T("moveto")				== strName) m_eType = rtMoveTo;
			//	else if (_T("lineto")			== strName) m_eType = rtLineTo;
			//	else if (_T("curveto")			== strName) m_eType = rtCurveTo;
			//	else if (_T("rmoveto")			== strName) m_eType = rtRMoveTo;
			//	else if (_T("rlineto")			== strName) m_eType = rtRLineTo;
			//	else if (_T("rcurveto")			== strName) m_eType = rtRCurveTo;
			//	else if (_T("ellipseto")		== strName) m_eType = rtAngleEllipseTo;
			//	else if (_T("ellipse")			== strName) m_eType = rtAngleEllipse;
			//	else if (_T("arc")				== strName) m_eType = rtArc;
			//	else if (_T("arcto")			== strName) m_eType = rtArcTo;
			//	else if (_T("clockwisearcto")	== strName) m_eType = rtClockwiseArcTo;
			//	else if (_T("clockwisearc")		== strName) m_eType = rtClockwiseArc;
			//	else if (_T("ellipticalx")		== strName) m_eType = rtEllipticalQuadrX;
			//	else if (_T("ellipticaly")		== strName) m_eType = rtEllipticalQuadrY;
			//	else if (_T("qbesier")			== strName) m_eType = rtQuadrBesier;
			//	else m_eType = rtClose;

			//	std::wstring strPath = oNode.GetAttribute(_T("path"));
			//	if (_T("") == strPath)
			//		return;

   //             std::vector<std::wstring> arStrNums;
			//	ParseString(_T(" "), strPath, &arStrNums);

			//	bool bIsX = true;
			//	int nCurPoint = 0;
   //             for (int nIndex = 0; nIndex < arStrNums.size(); ++nIndex)
			//	{
			//		if (bIsX)
			//		{
			//			++nCurPoint;
			//			CDoublePoint point;
   //                     this->m_arPoints.push_back(point);
   //                     this->m_arPoints[nCurPoint - 1].dX = XmlUtils::GetDouble(arStrNums[nIndex]);
			//		}
			//		else
			//		{
   //                     this->m_arPoints[nCurPoint - 1].dY = XmlUtils::GetDouble(arStrNums[nIndex]);
			//		}
			//		bIsX = !bIsX;
			//	}
			//}
			void ParseString(std::wstring strDelimeters, std::wstring strSource, 
				std::vector<std::wstring>& pArrayResults, bool bIsCleared = true);

			void CheckLastPoint(IRenderer* pRenderer, CDoublePoint& pointCur);
			double GetAngle(double fCentreX, double fCentreY, double fX, double fY);

			inline double GetSweepAngle(const double& angleStart, const double& angleEnd)
			{
				if (angleStart >= angleEnd)
					return angleEnd - angleStart;
				else
					return angleEnd - angleStart - 360;
			}

			void ApplyElliptical(bool& bIsX, double& angleStart, double& angleSweet, 
				double& Left, double& Top, double& Width, double& Height, const CDoublePoint& pointCur);

			void GetSafearrayPoints(IRenderer* pRenderer, double** ppArray, size_t& nCountOut, CDoublePoint& pointCur, bool bR = false);

			void Draw(IRenderer* pRenderer, CDoublePoint& pointCur);

			void AddEllipticalQuadr(IRenderer*& pRenderer, bool& bIsX, double& x1, double& y1, double& x2, double& y2, double& dRadX, double& dRadY);
		};

		void AddRuler(const RulesType& eType);
		void AddPoint(const double& x, const double& y);

		void Clear();

        std::vector<CPart> m_arParts;
		int m_lFlags;
	};
}

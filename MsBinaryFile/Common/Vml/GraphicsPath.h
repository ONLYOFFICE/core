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

#include <boost/algorithm/string.hpp>

#include "../../../DesktopEditor/graphics/GraphicsPath.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"

#include "../../PptFile/Drawing/Metric.h"
#include "../../PptFile/Drawing/Attributes.h"

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include <math.h>//M_PI
#include <vector>

namespace ODRAW
{
    class CDoublePoint
    {
    public:
        double dX;
        double dY;

		CDoublePoint();

		CDoublePoint& operator= (const CDoublePoint& oSrc);

		CDoublePoint(const CDoublePoint& oSrc);
    };

    namespace NSBaseShape
    {
		enum ClassType {unknown, pptx, ppt, odp};// Перечисление можно расширять
    }

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
        rtUnknown	  		= 22,

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
		virtual void InternalFromXmlNode(XmlUtils::CXmlNode& oXmlNode);
		virtual void InternalClear();
		CGraphicPath();
		virtual void Draw(IRenderer* pRenderer);
		void ConvertVector(IRenderer* pRenderer);

        class CPart
        {
        public:
            RulesType					m_eType;
            std::vector<CDoublePoint>	m_arPoints;

			CPart();
			CPart& operator=(const CPart& oSrc);
			~CPart();

			void FromXmlNode(XmlUtils::CXmlNode& oNode);

			void CheckLastPoint(IRenderer* pRenderer, CDoublePoint& pointCur);
			double GetAngle(double fCentreX, double fCentreY, double fX, double fY);

			double GetSweepAngle(const double& angleStart, const double& angleEnd);

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
        int Metric;
        bool m_bFill;
        bool m_bStroke;

        double m_dAngle;
        CDoubleRect m_oBounds;
        LONG m_lFlags;

        CPen	Pen;
        CBrush	Brush;
    };
}

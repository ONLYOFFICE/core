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
#include "GraphicsPath.h"

#include "PPTXShape/PptxFormula.h"
#include "PPTShape/PptFormula.h"

using namespace NSGuidesVML;

namespace ODRAW
{
    static std::wstring GetRulerVML(RulesType eRuler)
    {
        switch (eRuler)
        {
        case rtMoveTo:				{ return _T("m"); }
        case rtLineTo:				{ return _T("l"); }
        case rtCurveTo:				{ return _T("c"); }

        case rtClose:				{ return _T("x"); }
        case rtEnd:					{ return _T("e"); }

        case rtRMoveTo:				{ return _T("t"); }
        case rtRLineTo:				{ return _T("r"); }
        case rtRCurveTo:			{ return _T("v"); }

        case rtNoFill:				{ return _T("nf"); }
        case rtUnknown:             { return _T(""); }
        case rtNoStroke:			{ return _T("ns"); }

        case rtAngleEllipseTo:		{ return _T("ae"); }
        case rtAngleEllipse:		{ return _T("al"); }

        case rtArc:					{ return _T("at"); }
        case rtArcTo:				{ return _T("ar"); }

        case rtClockwiseArcTo:		{ return _T("wa"); }
        case rtClockwiseArc:		{ return _T("wr"); }

        case rtEllipticalQuadrX:	{ return _T("qx"); }
        case rtEllipticalQuadrY:	{ return _T("qy"); }

        case rtQuadrBesier:			{ return _T("qb"); }
        default: break;
        };
        return _T("");
    }

    static RulesType GetRuler(const std::wstring& strName, bool& bRes)
    {
        bRes = true;
        if		(_T("moveTo")		== strName)	return rtOOXMLMoveTo;
        else if (_T("lnTo")			== strName)	return rtOOXMLLineTo;
        else if (_T("cubicBezTo")	== strName)	return rtOOXMLCubicBezTo;
        else if (_T("close")		== strName)	return rtOOXMLClose;
        else if (_T("end")			== strName)	return rtOOXMLEnd;
        else if (_T("arcTo")		== strName)	return rtOOXMLArcTo;
        else if (_T("quadBezTo")	== strName)	return rtOOXMLQuadBezTo;

        //bRes = true;
        else if	((_T("m") == strName) || (_T("M") == strName))		return rtMoveTo;
        else if ((_T("l") == strName) || (_T("L") == strName))		return rtLineTo;
        else if ((_T("c") == strName) || (_T("C") == strName))		return rtCurveTo;
        else if ((_T("x") == strName) || (_T("Z") == strName))		return rtClose;
        else if ((_T("e") == strName) || (_T("N") == strName))		return rtEnd;
        else if (_T("t") == strName)								return rtRMoveTo;
        else if (_T("r") == strName)								return rtRLineTo;
        else if (_T("v") == strName)								return rtRCurveTo;
        else if ((_T("nf") == strName) || (_T("F") == strName))		return rtNoFill;
        else if ((_T("ns") == strName) || (_T("S") == strName))		return rtNoStroke;
        else if ((_T("ae") == strName) || (_T("T") == strName))		return rtAngleEllipseTo;
        else if ((_T("al") == strName) || (_T("U") == strName))		return rtAngleEllipse;
        else if ((_T("at") == strName) || (_T("A") == strName))		return rtArcTo;
        else if ((_T("ar") == strName) || (_T("B") == strName))		return rtArc;
        else if ((_T("wa") == strName) || (_T("W") == strName))		return rtClockwiseArcTo;
        else if ((_T("wr") == strName) || (_T("V") == strName))		return rtClockwiseArc;
        else if ((_T("qx") == strName) || (_T("X") == strName))		return rtEllipticalQuadrX;
        else if ((_T("qy") == strName) || (_T("Y") == strName))		return rtEllipticalQuadrY;
        else if ((_T("qb") == strName) || (_T("Q") == strName))		return rtQuadrBesier;
        else bRes = false;

        return rtEnd;
    }

    static double GetSweepAngle(const double& angleStart, const double& angleEnd)
    {
        if (angleStart >= angleEnd)
            return angleEnd - angleStart;
        else
            return angleEnd - angleStart - 360;
    }

    static std::wstring GetRulerName(RulesType eRuler)
    {
        switch (eRuler)
        {
        case rtOOXMLMoveTo:		{ return _T("moveto"); }
        case rtOOXMLLineTo:		{ return _T("lineto"); }
        case rtOOXMLCubicBezTo:	{ return _T("curveto"); }
        case rtOOXMLArcTo:		{ return _T("ellipseto"); }
        case rtOOXMLQuadBezTo:	{ return _T("qbesier"); }
        case rtOOXMLClose:		{ return _T("close"); }
        case rtOOXMLEnd:		{ return _T("end"); }

        case rtMoveTo:				{ return _T("moveto"); }
        case rtLineTo:				{ return _T("lineto"); }
        case rtCurveTo:				{ return _T("curveto"); }

        case rtClose:				{ return _T("close"); }
        case rtEnd:					{ return _T("end"); }

        case rtRMoveTo:				{ return _T("rmoveto"); }
        case rtRLineTo:				{ return _T("rlineto"); }
        case rtRCurveTo:			{ return _T("rcurveto"); }

        case rtNoFill:				{ return _T("nofill"); }
        case rtNoStroke:			{ return _T("nostroke"); }

        case rtAngleEllipseTo:		{ return _T("ellipseto"); }
        case rtAngleEllipse:		{ return _T("ellipse"); }

        case rtArc:					{ return _T("arc"); }
        case rtArcTo:				{ return _T("arcto"); }

        case rtClockwiseArcTo:		{ return _T("clockwisearcto"); }
        case rtClockwiseArc:		{ return _T("clockwisearc"); }

        case rtEllipticalQuadrX:	{ return _T("ellipticalx"); }
        case rtEllipticalQuadrY:	{ return _T("ellipticaly"); }

        case rtQuadrBesier:			{ return _T("qbesier"); }
        default: break;
        };
        return _T("none");
    }

    static long GetCountPoints(RulesType eRuler)
    {
        switch (eRuler)
        {
        case rtOOXMLMoveTo:     return 1;
        case rtOOXMLLineTo:	return 1;
        case rtOOXMLQuadBezTo:	return 2;
        case rtOOXMLCubicBezTo:	return 3;
        case rtOOXMLArcTo:	return 3;//???
        case rtOOXMLClose:	return 0;
        case rtOOXMLEnd:	return 0;
        default: return 0;
        };
        return 0;
    }

    static long GetCountPoints2(RulesType eRuler, LONG lRepeatCount)
    {
        switch (eRuler)
        {
        case rtMoveTo:
        case rtRMoveTo:
        { return 1; }

        case rtLineTo:
        case rtRLineTo:
        { return lRepeatCount; }

        case rtCurveTo:
        case rtRCurveTo:
        { return 3 * lRepeatCount; }

        case rtNoFill:
        case rtNoStroke:
        case rtClose:
        case rtEnd:
        { return 0; }

        case rtAngleEllipseTo:
        case rtAngleEllipse:
        { return lRepeatCount; }

        case rtArc:
        case rtArcTo:

        case rtClockwiseArcTo:
        case rtClockwiseArc:
        { return lRepeatCount; }

        case rtEllipticalQuadrX:
        case rtEllipticalQuadrY:
        { return 1 * lRepeatCount; }

        case rtQuadrBesier:
        { return /*2 * */lRepeatCount; }
        case rtFillColor:
        case rtLineColor:
        {
            return 1;
        }
        default: return 3 * lRepeatCount;
        };

        return 0;
    }

    class CSlice
    {
    private:
        int m_nCountElementsPoint;

    public:
        RulesType m_eRuler;
        std::vector<Aggplus::POINT> m_arPoints;

		LONG						m_lX;
		LONG						m_lY;

		CSlice(RulesType eType = rtMoveTo, LONG x = 0, LONG y = 0);

		void AddParam(LONG lParam);
		void FromXML(XmlUtils::CXmlNode& Node, NSGuidesOOXML::CFormulaManager& pManager, double WidthKoef, double HeightKoef);
		void ToRenderer(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, long w, long h, NSBaseShape::ClassType ClassType);
		CSlice& operator =(const CSlice& oSrc);

    protected:
		double GetAngle(double fCentreX, double fCentreY, double fX, double fY);
		double GetSweepAngle(const double& angleStart, const double& angleEnd);

        void ApplyElliptical(bool& bIsX, double& angleStart, double& angleSweet,
							 double& Left, double& Top, double& Width, double& Height, const CGeomShapeInfo::CPointD& pointCur);

		void ApplyLimo(CGeomShapeInfo& pGeomInfo, double& lX, double& lY);

		void Bez2_3(std::vector<CGeomShapeInfo::CPointD>& oArray, RulesType& eType);
	};

    class CPartPath
    {
    public:
        bool m_bFill;
        bool m_bStroke;
        long width;
        long height;
		long x;
        long y;
		std::vector<CSlice> m_arSlices;

		CPartPath();

		void FromXML(XmlUtils::CXmlNode& PathNode, NSGuidesOOXML::CFormulaManager& pManager); // oox
		void FromXML(std::wstring strPath, NSGuidesVML::CFormulasManager& pManager); //vml

		std::wstring ToXml(CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType);

		void ToRenderer(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType);

		void ToRendererOOX(CGraphicPath* pRenderer, CGeomShapeInfo& pGeomInfo, NSBaseShape::ClassType ClassType);

		CPartPath& operator =(const CPartPath& oSrc);
    };

    class CPath
    {
    public:
        std::vector<CPartPath> m_arParts;
		LONG m_lX;
		LONG m_lY;

		CPath();

		void FromXML(std::vector<XmlUtils::CXmlNode>& list, NSGuidesOOXML::CFormulaManager& pManager);
		void FromXML(std::wstring strPath, NSGuidesVML::CFormulasManager& pManager);

		std::wstring ToXml(CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType);

        void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime,
						double dEndTime, CPen& pPen, CBrush& pFore, NSBaseShape::ClassType ClassType);

		CPath& operator =(const CPath& oSrc);
        
		void SetCoordpos(LONG lX, LONG lY);
		void SetCoordsize(LONG lWidth, LONG lHeight);
    };
}

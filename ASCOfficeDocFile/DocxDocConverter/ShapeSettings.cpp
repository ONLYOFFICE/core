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

#include "ShapeSettings.h"

#include "../Common/FormatUtils.h"

#include "OfficeArt/OfficeArtFOPTE.h"
#include "OfficeArt/OfficeArtFOPTEOPID.h"
#include "OfficeArt/OfficeArtFOPTE.h"
#include "OfficeArt/Common.h"

namespace ASCDocFileFormat
{
	CStyle::CStyle()
	{

	}

	void CStyle::AppendEntry (int nID, int nValue, bool bBid, bool bComplex)
	{
		OfficeArtFOPTEOPID id (nID, bBid, bComplex);
		OfficeArtFOPTE oStyle (id, nValue);

		m_arTable.push_back(ComplexOfficeArtProperty(oStyle));
	}
	void CStyle::AppendEntry2 (int nID, int nValue, bool bBid, bool bComplex)
	{
		OfficeArtFOPTEOPID id (nID, bBid, bComplex);
		OfficeArtFOPTE oStyle (id, nValue);

		m_arTable2.push_back(ComplexOfficeArtProperty(oStyle));
	}

	template<class T> void CStyle::AppenComplexEntry (int nID, bool bBid, T& oArray)
	{
		OfficeArtFOPTEOPID id (nID, bBid, true);
		OfficeArtFOPTE oStyle (id, oArray.GetSize());

		//if(oArray.GetSize())
		m_arTable.push_back (ComplexOfficeArtProperty(oStyle, oArray.Get()));
		//else
		//	m_arTable.push_back(ComplexOfficeArtProperty(oStyle));
	}
}

namespace ASCDocFileFormat
{
	CGroupShapeStyle::CGroupShapeStyle()
	{
		m_posh		=	Enumerations::msophAbs;
		m_posv		=	Enumerations::msopvAbs;
		m_posrelh	=	Enumerations::msoprhText;
		m_posrelv	=	Enumerations::msoprvText;

		m_pctHoriz	=	0;
		m_pctVert	=	0;
		m_sizerelh	=	0;
		m_sizerelv	=	0;
	}

	bool CGroupShapeStyle::SetVisible (const std::string& strVisible)
	{
		if (std::string("hidden") == strVisible)
		{
			m_groupShapeBooleanProperties.fHidden		=	true;
			m_groupShapeBooleanProperties.fUsefHidden	=	true;
		}

		return TRUE;
	}

	void CGroupShapeStyle::SetUseBehind	(bool bUse)
	{
		m_groupShapeBooleanProperties.fBehindDocument			=	bUse;
		m_groupShapeBooleanProperties.fUsefBehindDocument		=	bUse;
	}

	void CGroupShapeStyle::SetPositioning (const std::string& posh, const std::string& posrelh, const std::string& posv, const std::string& posrelv)
	{
		if (std::string("center") == posh)			
			m_posh		=	Enumerations::msophCenter;
		else if (std::string("inside") == posh)			
			m_posh		=	Enumerations::msophInside;
		else if (std::string("left") == posh)			
			m_posh		=	Enumerations::msophLeft;
		else if (std::string("right") == posh)			
			m_posh		=	Enumerations::msophRight;
		else if (std::string("outside") == posh)			
			m_posh		=	Enumerations::msophOutside;

		if (std::string("center") == posv)
			m_posv		=	Enumerations::msopvCenter;
		else if (std::string("inside") == posv)
			m_posv		=	Enumerations::msopvInside;
		else if (std::string("top") == posv)
			m_posv		=	Enumerations::msopvTop;
		else if (std::string("bottom") == posv)
			m_posv		=	Enumerations::msopvBottom;
		else if (std::string("outside") == posv)
			m_posv		=	Enumerations::msopvOutside;

		if (std::string("margin") == posrelh)
			m_posrelh	=	Enumerations::msoprhMargin - 1;
		else if (std::string("page") == posrelh)
			m_posrelh	=	Enumerations::msoprhPage - 1;
		else if (std::string("text") == posrelh)
			m_posrelh	=	Enumerations::msoprhText - 1;
		else if (std::string("character") == posrelh)
			m_posrelh	=	Enumerations::msoprhChar - 1;

		if (std::string("margin") == posrelv)
			m_posrelv	=	Enumerations::msoprhMargin - 1;
		else if (std::string("page") == posrelv)
			m_posrelv	=	Enumerations::msoprhPage - 1;
		else if (std::string("text") == posrelv)
			m_posrelv	=	Enumerations::msoprvText - 1;
		else if (std::string("line") == posrelv)
			m_posrelv	=	Enumerations::msoprvLine - 1;


		//if (std::string("margin") == posrelh)
		//	m_posrelh	=	Enumerations::msoprhMargin;
		//else if (std::string("page") == posrelh)
		//	m_posrelh	=	Enumerations::msoprhPage;
		//else if (std::string("character") == posrelh)
		//	m_posrelh	=	Enumerations::msoprhChar;
		//else if (std::string("column") == posrelh)
		//	m_posrelh	=	Enumerations::msoprhText;
		//else if (std::string("leftMargin") == posrelh || std::string("left-margin-area") == posrelh)
		//	m_posrelh	=	4;
		//else if (std::string("rightMargin") == posrelh || std::string("right-margin-area") == posrelh)
		//	m_posrelh	=	5;
		//else if (std::string("insideMargin") == posrelh || std::string("inside-margin-area") == posrelh)
		//	m_posrelh	=	6;	
		//else if (std::string("outsideMargin") == posrelh || std::string("outside-margin-area") == posrelh)
		//	m_posrelh	=	7;			

		//if ( std::string("margin") == posrelv)
		//	m_posrelv	=	Enumerations::msoprhMargin;
		//else if (std::string("page") == posrelv)
		//	m_posrelv	=	Enumerations::msoprhPage;
		//else if (std::string("paragraph") == posrelv)
		//	m_posrelv	=	Enumerations::msoprvText;
		//else if (std::string("line") == posrelv)
		//	m_posrelv	=	Enumerations::msoprvLine;
		//else if (std::string("topMargin") == posrelv || std::string("top-margin-area") == posrelv)
		//	m_posrelv	=	4;
		//else if (std::string("bottomMargin") == posrelv || std::string("bottom-margin-area") == posrelv)
		//	m_posrelv	=	5;
		//else if (std::string("insideMargin") == posrelv || std::string("inside-margin-area") == posrelv)
		//	m_posrelv	=	6;			
		//else if (std::string("outsideMargin") == posrelv || std::string("outside-margin-area") == posrelv)
		//	m_posrelv	=	7;
	}

	void CGroupShapeStyle::SetPercentageSettings (const std::string& strPctHoriz, const std::string& strPctVert, const std::string& strSizeRelh, const std::string& strSizeRelv)
	{
		if (strPctHoriz.length())
		{
			m_pctHoriz	=	(unsigned int)atof(strPctHoriz.c_str());
			if (m_pctHoriz)
			{
				if (std::string("margin") == strSizeRelh)
					m_sizerelh	=	0;
				else if (std::string("page") == strSizeRelh)
					m_sizerelh	=	1;
				else if (std::string("leftMargin") == strSizeRelh || std::string("left-margin-area") == strSizeRelh)
					m_sizerelh	=	2;
				else if (std::string("rightMargin") == strSizeRelh || std::string("right-margin-area") == strSizeRelh)
					m_sizerelh	=	3;
				else if (std::string("insideMargin") == strSizeRelh || std::string("inside-margin-area") == strSizeRelh)
					m_sizerelh	=	4;	
				else if (std::string("outsideMargin") == strSizeRelh || std::string("outside-margin-area") == strSizeRelh)
					m_sizerelh	=	5;	
			}
		}

		if (strPctVert.length())
		{
			m_pctVert	=	(unsigned int)atof(strPctVert.c_str());
			if (m_pctVert)
			{
				if (std::string("margin") == strSizeRelv)
					m_sizerelv	=	0;
				else if (std::string("page") == strSizeRelv)
					m_sizerelv	=	1;
				else if (std::string("topMargin") == strSizeRelv || std::string("top-margin-area") == strSizeRelv)
					m_sizerelv	=	2;
				else if (std::string("bottomMargin") == strSizeRelv || std::string("bottom-margin-area") == strSizeRelv)
					m_sizerelv	=	3;
				else if (std::string("insideMargin") == strSizeRelv || std::string("inside-margin-area") == strSizeRelv)
					m_sizerelv	=	4;			
				else if (std::string("outsideMargin") == strSizeRelv || std::string("outside-margin-area") == strSizeRelv)
					m_sizerelv	=	5;
			}
		}
	}

	bool CGroupShapeStyle::Append(OfficeArtRGFOPTE& oTable, bool bLayout)
	{
		m_arTable.clear();

		if (m_posh)
			AppendEntry(Enumerations::posh, m_posh);
		if (m_posrelh)
			AppendEntry(Enumerations::posrelh, m_posrelh);
		if (m_posv)
			AppendEntry(Enumerations::posv, m_posv);
		if (m_posrelv)
			AppendEntry(Enumerations::posrelv, m_posrelv);

		AppendEntry (OfficeArt::Enumerations::groupShapeBooleanProperties, m_groupShapeBooleanProperties.Get());

		//AppendEntry (OfficeArt::Enumerations::diagramBooleans, 0x00010000);

		//if (m_pctHoriz)
		//	AppendEntry (Enumerations::pctHoriz, m_pctHoriz);
		//if (m_pctVert)
		//	AppendEntry (Enumerations::pctVert, m_pctVert);

		// POS...

		//if (m_sizerelh)
		//	AppendEntry (Enumerations::sizerelh, m_sizerelh);
		//if (m_sizerelv)
		//	AppendEntry (Enumerations::sizerelv, m_sizerelv);

		if (m_arTable.size())
		{
			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}
}

namespace ASCDocFileFormat
{
	CGeometryStyle::CGeometryStyle()
	{
		m_oGeometryBooleans.fLineOK			=	true;
		m_oGeometryBooleans.fUsefLineOK		=	true;

		m_oGeometryBooleans.fFillOK			=	true;
		m_oGeometryBooleans.fUsefFillOK		=	true;

		m_nGeoLeft							=	0;
		m_nGeoTop							=	0;
		m_nGeoRight							=	21600;
		m_nGeoBottom						=	21600;

		m_bAdjustArc						=	false;
	}

	bool CGeometryStyle::Read()
	{

		return TRUE;
	}

	bool CGeometryStyle::SetPath(const std::string& strPath, const std::string& strAdj, const std::string& strFormulas)
	{
		return m_oShapePath.Build (strPath, strAdj, strFormulas);
	}

	bool CGeometryStyle::SetRightBottom(const std::string& strCoordSize)
	{
		DOCX::CFPoint oFPoint (strCoordSize, 21600, 21600);

		m_nGeoRight		=	(int)oFPoint.X();
		m_nGeoBottom	=	(int)oFPoint.Y();

		return TRUE;
	}

	bool CGeometryStyle::SetConnection(const std::string& strType)	
	{
		if (std::string("custom") == strType)
			m_nCxk	=	Enumerations::msocxkCustom;

		return TRUE;
	}

	bool CGeometryStyle::SetSites(const std::string& strSites)
	{
		m_oConnectionSites.Add (CMSOArray<CPoint32>::GetPoints(strSites));

		return TRUE;
	}

	bool CGeometryStyle::SetSitesDir(const std::string& strSitesDir)
	{
		m_oConnectionSitesDir.Add(CMSOArray<unsigned int>::GetValues(strSitesDir));

		return TRUE;
	}

	bool CGeometryStyle::SetAdjustValues(const std::string& strAdjust, bool bAdjustArc)
	{
		m_arAdjustValues.clear();

		if (bAdjustArc)
		{
			m_bAdjustArc	=	true;
			m_arAdjustValues.push_back(DOCXDOCUTILS::GetArcValue(strAdjust));

			return TRUE;
		}

		m_arAdjustValues = DOCXDOCUTILS::GetValues (strAdjust);

		return TRUE;
	}

	bool CGeometryStyle::SetInscribe(const std::string& inscribe)
	{
		m_pInscribe.Add(CMSOArray<OfficeArt::RECT>::GetRectangles(inscribe));

		return TRUE;
	}

	bool CGeometryStyle::Append(OfficeArtRGFOPTE& oTable)
	{
		if (m_oShapePath.IsValid())
		{		
			AppendEntry (Enumerations::geoTop, m_nGeoTop);
			AppendEntry (Enumerations::geoLeft, m_nGeoLeft);
			AppendEntry (Enumerations::geoRight, m_nGeoRight);
			AppendEntry (Enumerations::geoBottom, m_nGeoBottom);

			AppendEntry (Enumerations::shapePath, 1);

			AppenComplexEntry (Enumerations::pVertices,		true,	m_oShapePath.GetPoints());
			AppenComplexEntry (Enumerations::pSegmentInfo,	true,	m_oShapePath.GetSegments());

			if (m_pInscribe.GetSize())
			{
				AppenComplexEntry (Enumerations::pInscribe,	true,	m_pInscribe);
			}

			if (Enumerations::msocxkCustom == m_nCxk)
			{				
				//AppenComplexEntry (Enumerations::pAdjustHandles,		true,	m_oAdjustHandles);
				//AppenComplexEntry (Enumerations::pGuides,				true,	m_oGuides);

				//AppenComplexEntry (Enumerations::pConnectionSites,	true,	m_oConnectionSites);
				//AppenComplexEntry (Enumerations::pConnectionSitesDir,	true,	m_oConnectionSitesDir);

				//AppendEntry (Enumerations::cxk, m_nCxk);
			}
		}

		if (m_bAdjustArc)
		{
			if (m_arAdjustValues.size())
			{
				AppendEntry (Enumerations::adjustValue, m_arAdjustValues[0]);
			}
		}
		else
		{
			if (m_arAdjustValues.size())
			{
				for (size_t i = 0; i < m_arAdjustValues.size(); ++i)
				{
					if (m_arAdjustValues[i])
						AppendEntry (Enumerations::adjustValue + i, m_arAdjustValues[i]);
				}
			}
		}

		AppendEntry (Enumerations::geometryBooleanProperties,	m_oGeometryBooleans.Get());

		if (m_arTable.size())
		{
			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}
}

namespace ASCDocFileFormat
{
	CFillStyle::CFillStyle ()
	{
		m_nType									=	0;
		m_nColor								=	0x00FFFFFF;
		m_nOpacity								=	0x00010000;
		m_nBlip									=	0;
	}

	bool CFillStyle::Read (const OOX::Logic::FillStyle& oXml)
	{
		m_arTable.clear();
		m_arTable2.clear();

		if (oXml.fillcolor.is_init())
		{
			DOCX::CSColor color;
			if (color.Init(oXml.fillcolor))
			{
				m_nColor						=	color.Color();
			}
		}

		if (oXml.opacity.is_init())
		{
			m_nOpacity							=	DOCXDOCUTILS::GetFixePointValue(oXml.opacity);
		}

		if (oXml.filled.is_init())
		{
			if (std::string ("f") == oXml.filled)
			{
				m_oFillBooleans.fFilled			=	false;
				m_oFillBooleans.fUsefFilled		=	true;
			}
		}

		// Внимание - порядок добавления свойств очень важен ( добавляем по возрастанию ID )

		if (0 != m_nType)
			AppendEntry(Enumerations::fillType,				m_nType);

		if (0x00FFFFFF != m_nColor)
			AppendEntry(Enumerations::fillColor,			m_nColor);

		if (0x10000 != m_nOpacity)
			AppendEntry(Enumerations::fillOpacity,			m_nOpacity);

		if (0 != m_nBlip)
			AppendEntry(Enumerations::fillBlip,				m_nBlip, true, false);

		AppendEntry2(Enumerations::fillStyleBooleanProperties,	0x00600000);//m_oFillBooleans.Get());

		return TRUE;
	}

	bool CFillStyle::SetType(int nType)
	{
		m_nType									=	nType;

		return Rebuild();
	}

	bool CFillStyle::SetBlip(int nBlip)
	{
		m_nBlip									=	nBlip;

		return Rebuild();
	}

	bool CFillStyle::SetUseShapeAnchor(bool value)
	{
		m_oFillBooleans.fUsefUseShapeAnchor		=	value;

		return Rebuild();
	}

	bool CFillStyle::Append (OfficeArtRGFOPTE& oTable)
	{
		if (m_arTable.size())
		{
			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}

	bool CFillStyle::Append2(OfficeArtRGFOPTE& oTable)
	{
		if (m_arTable2.size())
		{
			for (size_t i = 0; i < m_arTable2.size(); ++i)
				oTable.PushComplexProperty(m_arTable2[i]);

			return TRUE;
		}

		return FALSE;
	}

	bool CFillStyle::Rebuild()
	{
		m_arTable.clear();

		// Внимание - порядок добавления свойств очень важен ( добавляем по возрастанию ID )

		if (0 != m_nType)
			AppendEntry (Enumerations::fillType,				m_nType);

		if (0x00FFFFFF != m_nColor)
			AppendEntry (Enumerations::fillColor,				m_nColor);

		if (0x10000 != m_nOpacity)
			AppendEntry (Enumerations::fillOpacity,				m_nOpacity);

		if (0 != m_nBlip)
			AppendEntry (Enumerations::fillBlip,				m_nBlip, true, false);

		AppendEntry (Enumerations::fillStyleBooleanProperties,	m_oFillBooleans.Get());

		return TRUE;
	}
}

namespace ASCDocFileFormat
{
	CLineStyle::CLineStyle ()
	{
		m_nColor									=	0x00000000;
		m_nOpacity									=	0x00010000;
		m_nType										=	0;
		m_nWidth									=	0x00002535;
		m_nStyle									=	0;
		m_nDashing									=	0;
		m_nStartArrow								=	Enumerations::msolineNoEnd;
		m_nEndArrow									=	Enumerations::msolineNoEnd;
		m_nStartArrowWidth							=	Enumerations::msolineMediumWidthArrow;
		m_nStartArrowLength							=	Enumerations::msolineMediumLenArrow;
		m_nEndArrowWidth							=	Enumerations::msolineMediumWidthArrow;
		m_nEndArrowLength							=	Enumerations::msolineMediumLenArrow;
		m_nEndCapStyle								=	Enumerations::msolineEndCapFlat;
	}

	bool CLineStyle::Read (const OOX::Logic::LineStyle& oXml)
	{
		m_arTable.clear();

		if (oXml.strokeColor.is_init())
		{
			DOCX::CSColor color;
			if (color.Init(oXml.strokeColor))
			{
				m_nColor						=	color.Color();
			}
		}

		if (oXml.opacity.is_init())
		{
			m_nOpacity							=	DOCXDOCUTILS::GetFixePointValue (oXml.opacity);
		}

		if (oXml.lineStyle.is_init())
		{
			m_nStyle							=	GetLineStyle(oXml.lineStyle);
		}

		if (oXml.endcap.is_init())
		{
			m_nEndCapStyle						=	GetEndCapStyle(oXml.endcap);
		}

		if (oXml.startarrow.is_init())
		{
			m_nStartArrow						=	GetLineArrowHead(oXml.startarrow);
		}

		if (oXml.endarrow.is_init())
		{
			m_nEndArrow							=	GetLineArrowHead(oXml.endarrow);
		}

		if (oXml.startarrowwidth.is_init())
		{
			m_nStartArrowWidth					=	GetLineArrowWidth(oXml.startarrowwidth);
		}

		if (oXml.startarrowlength.is_init())
		{
			m_nStartArrowLength					=	GetLineArrowLength(oXml.startarrowlength);
		}

		if (oXml.endarrowwidth.is_init())
		{
			m_nEndArrowWidth					=	GetLineArrowWidth(oXml.endarrowwidth);
		}

		if (oXml.endarrowlength.is_init())
		{
			m_nEndArrowLength					=	GetLineArrowLength(oXml.endarrowlength);
		}

		if (oXml.dashStyle.is_init())
		{
			m_nDashing							=	GetLineDash(oXml.dashStyle);
			if (-1 == m_nDashing)
			{
				m_nDashing						=	0;

				// TODO : need add CUSTOM dash pen
			}

			m_oLineBooleans.fNoLineDrawDash		=	true;
			m_oLineBooleans.fUsefNoLineDrawDash	=	true;
		}

		if (oXml.strokeWeight.is_init())
		{
			DOCX::CEmu oEmu (oXml.strokeWeight);
			m_nWidth	=	oEmu.Get();
		}

		if (oXml.stroked.is_init())
		{
			if (std::string ("f") == oXml.stroked)
			{
				m_oLineBooleans.fLine			=	false;
				m_oLineBooleans.fUsefLine		=	true;
			}
		}

		// Внимание - порядок добавления свойств очень важен ( добавляем по возрастанию ID )

		AppendEntry (Enumerations::lineColor,						m_nColor);

		if (0x10000 != m_nOpacity)
			AppendEntry (Enumerations::lineOpacity,					m_nOpacity);

		if (0 != m_nType)
			AppendEntry (Enumerations::lineType,					m_nType);

		AppendEntry (Enumerations::lineWidth,						m_nWidth);

		if (0 != m_nStyle)
			AppendEntry (Enumerations::lineStyle,					m_nStyle);

		if (0 != m_nDashing)
			AppendEntry (Enumerations::lineDashing,					m_nDashing);

		if (Enumerations::msolineNoEnd != m_nStartArrow)
			AppendEntry (Enumerations::lineStartArrowhead,			m_nStartArrow);

		if (Enumerations::msolineNoEnd != m_nEndArrow)
			AppendEntry (Enumerations::lineEndArrowhead,			m_nEndArrow);

		if (Enumerations::msolineMediumWidthArrow != m_nStartArrowWidth)
			AppendEntry (Enumerations::lineStartArrowWidth,			m_nStartArrowWidth);

		if (Enumerations::msolineMediumLenArrow != m_nStartArrowLength)
			AppendEntry (Enumerations::lineStartArrowLength,		m_nStartArrowLength);

		if (Enumerations::msolineMediumWidthArrow != m_nEndArrowWidth)
			AppendEntry (Enumerations::lineEndArrowWidth,			m_nEndArrowWidth);

		if (Enumerations::msolineMediumLenArrow != m_nEndArrowLength)
			AppendEntry (Enumerations::lineEndArrowLength,			m_nEndArrowLength);

		if (Enumerations::msolineEndCapFlat != m_nEndCapStyle)
			AppendEntry (Enumerations::lineEndCapStyle,				m_nEndCapStyle);

		if (0 != m_oLineBooleans.Get())
			AppendEntry (Enumerations::lineStyleBooleanProperties,	m_oLineBooleans.Get());

		return TRUE;
	}

	bool CLineStyle::Append (OfficeArtRGFOPTE& oTable)
	{
		if (m_arTable.size())
		{
			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}
	// READ
	int CLineStyle::GetLineStyle (const std::string& lineType)
	{
		std::string type = lineType; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("single"))
			return OfficeArt::Enumerations::msolineSimple;

		if (type == std::string("thickbetweenthin"))
			return OfficeArt::Enumerations::msolineTriple;

		if (type == std::string("thickthin"))
			return OfficeArt::Enumerations::msolineThickThin;

		if (type == std::string("thinthick"))
			return OfficeArt::Enumerations::msolineThinThick;

		if (type == std::string("thinthin"))
			return OfficeArt::Enumerations::msolineDouble;

		return OfficeArt::Enumerations::msolineSimple;
	}

	int CLineStyle::GetLineDash (const std::string& dashType)
	{
		std::string type = dashType; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("solid"))
			return OfficeArt::Enumerations::msolineSolid;
		if (type == std::string("shortdash"))
			return OfficeArt::Enumerations::msolineDashSys;
		if (type == std::string("shortdot"))
			return OfficeArt::Enumerations::msolineDotSys;
		if (type == std::string("shortdashdot"))
			return OfficeArt::Enumerations::msolineDashDotSys;
		if (type == std::string("shortdashdotdot"))
			return OfficeArt::Enumerations::msolineDashDotDotSys;
		if (type == std::string("dot"))
			return OfficeArt::Enumerations::msolineDotGEL;
		if (type == std::string("dash"))
			return OfficeArt::Enumerations::msolineDashGEL;
		if (type == std::string("dashdot"))
			return OfficeArt::Enumerations::msolineDashDotGEL;
		if (type == std::string("longdash"))
			return OfficeArt::Enumerations::msolineLongDashGEL;
		if (type == std::string("longdashdot"))
			return OfficeArt::Enumerations::msolineLongDashDotGEL;
		if (type == std::string("longdashdotdot"))
			return OfficeArt::Enumerations::msolineLongDashDotDotGEL;
		if (type == std::string("1 1"))
			return OfficeArt::Enumerations::msolineDotSys;
		if (type == std::string("3 1"))
			return OfficeArt::Enumerations::msolineDashSys;

		return OfficeArt::Enumerations::msolineSolid;	//	CUSTOM
	}

	int CLineStyle::GetEndCapStyle (const std::string& capStyle)
	{
		std::string type = capStyle; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("round"))
			return OfficeArt::Enumerations::msolineEndCapRound;
		if (type == std::string("square"))
			return OfficeArt::Enumerations::msolineEndCapSquare;

		return OfficeArt::Enumerations::msolineEndCapFlat;	
	}

	int CLineStyle::GetLineArrowHead (const std::string& strStyle)
	{
		std::string type = strStyle; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("none"))
			return OfficeArt::Enumerations::msolineNoEnd;
		if (type == std::string("diamond"))
			return OfficeArt::Enumerations::msolineArrowDiamondEnd;
		if (type == std::string("open"))
			return OfficeArt::Enumerations::msolineArrowOpenEnd;
		if (type == std::string("oval"))
			return OfficeArt::Enumerations::msolineArrowOvalEnd;
		if (type == std::string("block"))
			return OfficeArt::Enumerations::msolineArrowEnd;
		if (type == std::string("classic"))
			return OfficeArt::Enumerations::msolineArrowStealthEnd;

		return OfficeArt::Enumerations::msolineNoEnd;			
	}

	int CLineStyle::GetLineArrowWidth (const std::string& strStyle)
	{
		std::string type = strStyle; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("narrow"))
			return OfficeArt::Enumerations::msolineNarrowArrow;
		if (type == std::string("medium"))
			return OfficeArt::Enumerations::msolineMediumWidthArrow;
		if (type == std::string("wide"))
			return OfficeArt::Enumerations::msolineWideArrow;

		return OfficeArt::Enumerations::msolineMediumWidthArrow;			
	}

	int CLineStyle::GetLineArrowLength (const std::string& strStyle)
	{
		std::string type = strStyle; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("long"))
			return OfficeArt::Enumerations::msolineLongArrow;
		if (type == std::string("medium"))
			return OfficeArt::Enumerations::msolineMediumLenArrow;
		if (type == std::string("short"))
			return OfficeArt::Enumerations::msolineShortArrow;

		return OfficeArt::Enumerations::msolineMediumLenArrow;	
	}
}

namespace ASCDocFileFormat
{
	CShadowStyle::CShadowStyle ()
	{
		m_bOK										=	false;
		m_nType										=	2;		//	OfficeArt::Enumerations::msoshadowRich
	}

	bool CShadowStyle::Read (const OOX::Logic::Shadow& oXml)
	{
		m_arTable.clear();

		m_bOK	=	false;

		if (!oXml.On.is_init())
			return FALSE;		

		if (std::string("t") != oXml.On)			
			return FALSE;

		m_bOK	=	true;

		if (oXml.Type.is_init())
		{
			AppendEntry (Enumerations::shadowType, m_nType);
		}

		if (oXml.Color.is_init())
		{
			DOCX::CSColor dcColor;
			if (dcColor.Init(oXml.Color))
			{
				m_nColor	=	dcColor.Color();

				AppendEntry (Enumerations::shadowColor, m_nColor);
			}
		}

		if (oXml.Opacity.is_init())
		{
			m_nOpacity		=	DOCXDOCUTILS::GetFixePointValue (oXml.Opacity);

			AppendEntry (Enumerations::shadowOpacity, m_nOpacity);
		}

		if (oXml.Offset.is_init())
		{
			m_oOffset		=	DOCX::CPointF (oXml.Offset);

			AppendEntry (Enumerations::shadowOffsetX, m_oOffset.X());
			AppendEntry (Enumerations::shadowOffsetY, m_oOffset.Y());
		}

		if (oXml.Matrix.is_init())			// данные формируются не по спецификации
		{
			m_oTransform	=	DOCX::CMatrixF (oXml.Matrix);

			if (0.0 != m_oTransform.XToX())
				AppendEntry (Enumerations::shadowScaleXToX, m_oTransform.XToX());
			if (0.0 != m_oTransform.XToY())
				AppendEntry (Enumerations::shadowScaleXToY, m_oTransform.XToY());
			if (0.0 != m_oTransform.YToX())
				AppendEntry (Enumerations::shadowScaleYToX, m_oTransform.YToX());
			if (0.0 != m_oTransform.YToY())
				AppendEntry (Enumerations::shadowScaleYToY, m_oTransform.YToY());
			if (0.0 != m_oTransform.PX())
				AppendEntry (Enumerations::shadowPerspectiveX, m_oTransform.PX());
			if (0.0 != m_oTransform.PY())
				AppendEntry (Enumerations::shadowPerspectiveY, m_oTransform.PY());
		}

		if (oXml.Origin.is_init())
		{
			m_oOrigin		=	DOCX::CPointF (oXml.Origin);

			if (0.0 != m_oOrigin.X())
				AppendEntry (Enumerations::shadowOriginX, m_oOrigin.X());
			if (0.0 != m_oOrigin.Y())
				AppendEntry (Enumerations::shadowOriginY, m_oOrigin.Y());
		}

		return TRUE;
	}

	bool CShadowStyle::Append (OfficeArtRGFOPTE& oTable, bool bFilled)
	{
		if (m_bOK)
		{
			m_oShadowBooleans.fUsefShadow			=	true;
			m_oShadowBooleans.fShadow				=	true;

			m_oShadowBooleans.fUsefShadowObscured	=	bFilled;
			m_oShadowBooleans.fShadowObscured		=	bFilled;

			AppendEntry (Enumerations::shadowStyleBooleanProperties,	m_oShadowBooleans.Get());

			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}
}

namespace ASCDocFileFormat
{
	CTransformStyle::CTransformStyle ()
	{
		m_bAngleBoundFlip	=	FALSE;
		m_nRotate			=	0;
	}

	bool CTransformStyle::SetRotate (const std::string& strRotate, bool bPicture)
	{
		if (strRotate.length())
		{
			double dAngle	=	0.0;

			if (bPicture)			
			{
				if (strRotate.length())
					dAngle	=	atof(strRotate.c_str()) / 60000.0;
			}
			else
			{
				dAngle		=	DOCXDOCUTILS::GetAngleValue(strRotate);
			}

			if (dAngle < -360.0)
				dAngle += 360.0;

			if ((dAngle >= 45.0 && dAngle <= 135.0) || (dAngle >= 225.0 && dAngle <= 315.0) || 
				(dAngle <= -45.0 && dAngle >= -135.0) || (dAngle <= -225.0 && dAngle >= -315.0))
			{
				m_bAngleBoundFlip	=	TRUE;
			}

			m_nRotate		=	(unsigned int)(dAngle * 65536.0);

			if (m_nRotate)
			{
				AppendEntry (Enumerations::rotation, m_nRotate);

				return TRUE;
			}
		}

		return FALSE;
	}

	bool CTransformStyle::Append (OfficeArtRGFOPTE& oTable)
	{
		if (m_arTable.size())
		{
			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}
}

namespace ASCDocFileFormat
{
	CTextStyle::CTextStyle ()
	{
		m_nLTxID			=	0;

		m_nWrapMode			=	Enumerations::msowrapSquare;
		m_nAnchorText		=	Enumerations::msoanchorTop;
	}

	bool CTextStyle::SetAnchor (const std::string& textAnchor)
	{
		m_nAnchorText		=	Enumerations::msoanchorTop;

		std::string type = textAnchor; 
		std::transform(type.begin(), type.end(), type.begin(), ::tolower);	

		if (type == std::string("top"))
			m_nAnchorText	=	Enumerations::msoanchorTop;
		else if (type == std::string("middle"))
			m_nAnchorText	=	Enumerations::msoanchorMiddle;
		else if (type == std::string("bottom"))
			m_nAnchorText	=	Enumerations::msoanchorBottom;
		else if (type == std::string("top-center"))		
			m_nAnchorText	=	Enumerations::msoanchorTopCentered;
		else if (type == std::string("middle-center"))	
			m_nAnchorText	=	Enumerations::msoanchorMiddleCentered;
		else if (type == std::string("bottom-center"))	
			m_nAnchorText	=	Enumerations::msoanchorBottomCentered;
		else if (type == std::string("top-baseline"))		
			m_nAnchorText	=	Enumerations::msoanchorTopBaseline;
		else if (type == std::string("bottom-baseline"))	
			m_nAnchorText	=	Enumerations::msoanchorBottomBaseline;
		else if (type == std::string("top-center-baseline"))	
			m_nAnchorText	=	Enumerations::msoanchorTopCenteredBaseline;
		else if (type == std::string("bottom-center-baseline"))	
			m_nAnchorText	=	Enumerations::msoanchorBottomCenteredBaseline;
		else
			m_nAnchorText	=	Enumerations::msoanchorTop;

		return TRUE;
	}

	bool CTextStyle::Append (OfficeArtRGFOPTE& oTable)
	{
		if (m_nLTxID)
			AppendEntry (Enumerations::lTxid, m_nLTxID);

		if (Enumerations::msowrapSquare != m_nWrapMode)
			AppendEntry (Enumerations::anchorText, m_nWrapMode);

		if (Enumerations::msoanchorTop != m_nAnchorText)
			AppendEntry (Enumerations::anchorText, m_nAnchorText);

		if (m_arTable.size())
		{
			AppendEntry (Enumerations::textBooleanProperties, m_oTextBooleans.Get());

			for (size_t i = 0; i < m_arTable.size(); ++i)
				oTable.PushComplexProperty(m_arTable[i]);

			return TRUE;
		}

		return FALSE;
	}
}

namespace ASCDocFileFormat
{
	CBlipStyle::CBlipStyle ()
	{
		m_nPib				=	0;
	}

	bool CBlipStyle::SetPib (int nPib)
	{
		m_nPib				=	nPib;

		return TRUE;
	}

	bool CBlipStyle::Append (OfficeArtRGFOPTE& oTable)
	{
		AppendEntry (Enumerations::pib,						m_nPib, true, false);
		AppendEntry (Enumerations::blipBooleanProperties,	m_oBlipBooleans.Get());

		for (size_t i = 0; i < m_arTable.size(); ++i)
			oTable.PushComplexProperty(m_arTable[i]);

		return TRUE;
	}
}

namespace ASCDocFileFormat
{
	CShapeSettings::CShapeSettings		()
	{
		m_nDXWrapDistLeft					=	0x0001BE7C;
		m_nDYWrapDistTop					=	0x00000000;	
		m_nDXWrapDistRight					=	0x0001BE7C;	
		m_nDYWrapDistBottom					=	0x00000000;	
	}
	//
	void CShapeSettings::SetUseBehind	(bool bUse)
	{
		m_oGroupShapeStyle.SetUseBehind (bUse);
	}

	//
	void CShapeSettings::SetUseBehind	(const OOX::Logic::ShapeStyle& refStyle)		//	объект располагается за текстом
	{
		if (refStyle.ZIndex.is_init())
		{
			double fStatus									=	atof(refStyle.ZIndex->c_str());
			if (fStatus < 0.0)
			{
				m_oGroupShapeStyle.SetUseBehind (true);
			}
		}
	}


	void CShapeSettings::SetWrapDist	(const OOX::Logic::ShapeStyle& refStyle)
	{
		if (refStyle.distanceLeftTop.is_init())
		{
			Unit<double, Dx> nLeft		=	*refStyle.distanceLeftTop->X;
			m_nDXWrapDistLeft			=	(int) ( (double)nLeft * 635.0 );

			Unit<double, Dx> nTop		=	*refStyle.distanceLeftTop->Y;
			m_nDYWrapDistTop			=	(int) ( (double)nTop * 635.0 );	
		}

		if (refStyle.distanceRightBottom.is_init())
		{
			Unit<double, Dx> nRight		=	*refStyle.distanceRightBottom->X;
			m_nDXWrapDistRight			=	(int) ( (double)nRight * 635.0 );	

			Unit<double, Dx> nBottom	=	*refStyle.distanceRightBottom->Y;
			m_nDYWrapDistBottom			=	(int) ( (double)nBottom * 635.0 );	
		}
	}

	//
	void CShapeSettings::AddEntry (int nID, int nValue, bool bBid, bool bComplex)
	{
		OfficeArt::OfficeArtFOPTEOPID id (nID, bBid, bComplex);
		OfficeArt::OfficeArtFOPTE oStyle (id, nValue);

		m_oPrimary.PushComplexProperty(OfficeArt::ComplexOfficeArtProperty(oStyle));
	}

	bool CShapeSettings::SetupSecondaryOptions()
	{
		if (m_oFill.Append2(m_oSecondary) && m_oGroupShapeStyle.Append(m_oSecondary))
			return TRUE;

		return FALSE;
	}

	OfficeArt::OfficeArtFOPT CShapeSettings::GetPrimaryOptions()
	{
		// Внимание - порядок добавления свойств очень важен

		m_oText.Append(m_oPrimary);
		m_oTransform.Append(m_oPrimary);
		m_oGeometry.Append(m_oPrimary);
		m_oFill.Append(m_oPrimary);
		m_oLine.Append(m_oPrimary);
		m_oShadow.Append(m_oPrimary, m_oFill.GetFill().fFilled);

		// GROUP

		if ((m_nDXWrapDistLeft != 0x0001BE7C) || (m_nDYWrapDistTop > 0) || (m_nDXWrapDistRight != 0x0001BE7C) || (m_nDYWrapDistBottom > 0))
		{
			AddEntry(OfficeArt::Enumerations::dxWrapDistLeft,	m_nDXWrapDistLeft);
			AddEntry(OfficeArt::Enumerations::dyWrapDistTop,	m_nDYWrapDistTop);
			AddEntry(OfficeArt::Enumerations::dxWrapDistRight,	m_nDXWrapDistRight);
			AddEntry(OfficeArt::Enumerations::dyWrapDistBottom,	m_nDYWrapDistBottom);
		}

		return OfficeArt::OfficeArtFOPT(m_oPrimary);
	}

	OfficeArt::OfficeArtSecondaryFOPT CShapeSettings::GetSecondaryOptions()
	{
		return OfficeArt::OfficeArtSecondaryFOPT(m_oSecondary);
	}

	void CShapeSettings::AddOptionalEntry (OfficeArtRGFOPTE& oTable, int nID, int nValue, bool bBid, bool bComplex)
	{
		OfficeArtFOPTEOPID id (nID, bBid, bComplex);
		OfficeArtFOPTE oStyle (id, nValue);

		oTable.PushComplexProperty(ComplexOfficeArtProperty(oStyle));
	}
}

namespace ASCDocFileFormat
{
	CImageSettings::CImageSettings () : CShapeSettings()
	{
		m_oFillStyleBooleans.fFilled		=	false;
		m_oFillStyleBooleans.fUsefFilled	=	true;

		m_oLineStyleBooleans.fLine			=	false;
		m_oLineStyleBooleans.fUsefLine		=	true;
	}

	void CImageSettings::SetWrapDist (const OOX::Logic::Inline& oInline)
	{
		if (oInline.distanceLeftTop.is_init())
		{
			Unit<int, Dx> nLeft		=	*oInline.distanceLeftTop->X;
			m_nDXWrapDistLeft		=	nLeft;

			Unit<int, Dx> nTop		=	*oInline.distanceLeftTop->Y;
			m_nDYWrapDistTop		=	nTop;	
		}

		if (oInline.distanceRightBottom.is_init())
		{
			Unit<int, Dx> nRight	=	*oInline.distanceRightBottom->X;
			m_nDXWrapDistRight		=	nRight;	

			Unit<int, Dx> nBottom	=	*oInline.distanceRightBottom->Y;
			m_nDYWrapDistBottom		=	nBottom;	
		}
	}


	bool CImageSettings::SetupSecondaryOptions()
	{
		if (m_oGroupShapeStyle.Append(m_oSecondary))
			return TRUE;

		return FALSE;
	}

	OfficeArt::OfficeArtFOPT CImageSettings::GetPrimaryOptions()
	{
		// Внимание - порядок добавления свойств очень важен

		m_oTransform.Append(m_oPrimary);
		m_oBlipStyle.Append(m_oPrimary);
		m_oFill.Append(m_oPrimary);
		m_oLine.Append(m_oPrimary);

		// GROUP

		if ( (m_nDXWrapDistLeft != 0x0001BE7C) || m_nDYWrapDistTop > 0 || (m_nDXWrapDistRight != 0x0001BE7C) || m_nDYWrapDistBottom > 0 )
		{
			AddEntry (OfficeArt::Enumerations::dxWrapDistLeft,				m_nDXWrapDistLeft);
			AddEntry (OfficeArt::Enumerations::dyWrapDistTop,				m_nDYWrapDistTop);
			AddEntry (OfficeArt::Enumerations::dxWrapDistRight,				m_nDXWrapDistRight);
			AddEntry (OfficeArt::Enumerations::dyWrapDistBottom,			m_nDYWrapDistBottom);
		}

		m_oGroupShapeStyle.Append(m_oPrimary);

		return OfficeArt::OfficeArtFOPT(m_oPrimary);
	}
	OfficeArt::OfficeArtSecondaryFOPT CImageSettings::GetSecondaryOptions()
	{
		return OfficeArt::OfficeArtSecondaryFOPT(m_oSecondary);
	}
}
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

#include "CtrlShapeTextArt.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrlShapeTextArt::CCtrlShapeTextArt()
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
		default: break;
	}
}

void CCtrlShapeTextArt::ReadFromHWPX(CXMLReader &oReader)
{
	m_sText = oReader.GetAttribute("text");

	#define READ_POINT(variable_point)\
	{\
		START_READ_ATTRIBUTES(oReader)\
		{\
			if ("x" == sAttributeName)\
				variable_point.m_nX = oReader.GetInt();\
			else if ("y" == sAttributeName)\
				variable_point.m_nY = oReader.GetInt();\
		}\
		END_READ_ATTRIBUTES(oReader)\
	}

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:pt0" == sNodeName)
			READ_POINT(m_oPt0)
		else if ("hp:pt1" == sNodeName)
			READ_POINT(m_oPt1)
		else if ("hp:pt2" == sNodeName)
			READ_POINT(m_oPt2)
		else if ("hp:pt3" == sNodeName)
			READ_POINT(m_oPt3)
		else if ("hp:textartPr" == sNodeName)
			ReadTextArtShape(oReader, EHanType::HWPX);
		else if ("hp:outline" == sNodeName)
			ReadOutlineData(oReader, EHanType::HWPX);
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeTextArt::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("Text" == sAttributeName)
			m_sText = oReader.GetText();
		else if ("X0" == sAttributeName)
			m_oPt0.m_nX = oReader.GetInt();
		else if ("Y0" == sAttributeName)
			m_oPt0.m_nY = oReader.GetInt();
		else if ("X1" == sAttributeName)
			m_oPt1.m_nX = oReader.GetInt();
		else if ("Y1" == sAttributeName)
			m_oPt1.m_nY = oReader.GetInt();
		else if ("X2" == sAttributeName)
			m_oPt2.m_nX = oReader.GetInt();
		else if ("Y2" == sAttributeName)
			m_oPt2.m_nY = oReader.GetInt();
		else if ("X3" == sAttributeName)
			m_oPt3.m_nX = oReader.GetInt();
		else if ("Y3" == sAttributeName)
			m_oPt3.m_nY = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("TEXTARTSHAPE" == sNodeName)
			ReadTextArtShape(oReader, EHanType::HWPML);
		else if ("OUTLINEDATA" == sNodeName)
			ReadOutlineData(oReader, EHanType::HWPML);
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeTextArt::ReadTextArtShape(CXMLReader &oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::FontName, eType) == sAttributeName)
			m_sFontName = oReader.GetText();
		else if (GetAttributeName(EAttribute::FontStyle, eType)== sAttributeName)
			m_sFontStyle = oReader.GetText();
		else if (GetAttributeName(EAttribute::FontType, eType) == sAttributeName)
			m_sFontType = oReader.GetText();
		else if (GetAttributeName(EAttribute::TextShape, eType) == sAttributeName)
			m_sTextShape = oReader.GetText();
		else if (GetAttributeName(EAttribute::Align, eType) == sAttributeName)
			m_sAlign = oReader.GetText();
		else if (GetAttributeName(EAttribute::LineSpacing, eType) == sAttributeName)
			m_shLineSpacing = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Spacing, eType) == sAttributeName)
			m_shSpacing = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	//TODO:: Implement shadows
}

void CCtrlShapeTextArt::ReadOutlineData(CXMLReader &oReader, EHanType eType)
{
	TPoint oPoint{0, 0};

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::Point, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::X, eType) == sAttributeName)
				oPoint.m_nX = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Y, eType) == sAttributeName)
				oPoint.m_nY = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)

		m_arOutline.push_back(oPoint);

		oPoint.m_nX = 0;
		oPoint.m_nY = 0;
	}
	END_WHILE
}

EShapeType CCtrlShapeTextArt::GetShapeType() const
{
	return EShapeType::TextArt;
}

int CCtrlShapeTextArt::ParseElement(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	// TODO:: Verify

	// [HWP ambiguous] following 120bytes are unknown.
	// Document doesn't mention about this at all.

	oBuffer.Skip(nSize);
	return nSize;
}

int CCtrlShapeTextArt::ParseCtrl(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}
}

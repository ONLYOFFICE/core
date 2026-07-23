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

#include "CtrlShapeConnectLine.h"

namespace HWP
{
EConnectLineType GetConnectLineType(int nValue)
{
	switch(static_cast<EConnectLineType>(nValue))
	{
		case EConnectLineType::STRAIGHT_NOARROW:
		case EConnectLineType::STRAIGHT_ONEWAY:
		case EConnectLineType::STRAIGHT_BOTH:
		case EConnectLineType::STROKE_NOARROW:
		case EConnectLineType::STROKE_ONEWAY:
		case EConnectLineType::STROKE_BOTH:
		case EConnectLineType::ARC_NOARROW:
		case EConnectLineType::ARC_ONEWAY:
		case EConnectLineType::ARC_BOTH:
			return static_cast<EConnectLineType>(nValue);
		default:
			return EConnectLineType::null;
	}
}

CCtrlShapeConnectLine::CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeConnectLine::CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
		default: break;
	}
}

void CCtrlShapeConnectLine::ReadFromHWPX(CXMLReader &oReader)
{
	m_eType = GetConnectLineType(oReader.GetAttributeInt("type"));

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:startPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_oStartPt.m_nX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_oStartPt.m_nY = oReader.GetInt();
				else if ("subjectIDRef" == sAttributeName)
					m_oStartPt.m_shSubjectIDRef = oReader.GetInt();
				else if ("subjectIdx" == sAttributeName)
					m_oStartPt.m_shSubjectIdx = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:endPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_oEndPt.m_nX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_oEndPt.m_nY = oReader.GetInt();
				else if ("subjectIDRef" == sAttributeName)
					m_oEndPt.m_shSubjectIDRef = oReader.GetInt();
				else if ("subjectIdx" == sAttributeName)
					m_oEndPt.m_shSubjectIdx = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeConnectLine::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("Type" == sAttributeName)
			// TODO:: Implement when encountered
			m_eType = EConnectLineType::STRAIGHT_NOARROW;
		else if ("StartX" == sAttributeName)
			m_oStartPt.m_nX = oReader.GetInt();
		else if ("StartY" == sAttributeName)
			m_oStartPt.m_nY = oReader.GetInt();
		else if ("EndX" == sAttributeName)
			m_oEndPt.m_nX = oReader.GetInt();
		else if ("EndY" == sAttributeName)
			m_oEndPt.m_nY = oReader.GetInt();
		else if ("StartSubjectID" == sAttributeName)
			m_oStartPt.m_shSubjectIDRef = oReader.GetInt();
		else if ("StartSubjectIndex" == sAttributeName)
			m_oStartPt.m_shSubjectIdx = oReader.GetInt();
		else if ("EndSubjectID" == sAttributeName)
			m_oEndPt.m_shSubjectIDRef = oReader.GetInt();
		else if ("EndSubjectIndex" == sAttributeName)
			m_oEndPt.m_shSubjectIdx = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPML);
	END_WHILE
}

EShapeType CCtrlShapeConnectLine::GetShapeType() const
{
	return EShapeType::ConnectLine;
}
}

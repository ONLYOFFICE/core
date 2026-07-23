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

#include "CtrlShapeLine.h"

namespace HWP
{
CCtrlShapeLine::CCtrlShapeLine()
{}

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeLine::CCtrlShapeLine(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
		default: break;
	}
}

void CCtrlShapeLine::ReadFromHWPX(CXMLReader &oReader)
{
	m_shAttr = (short)oReader.GetAttributeBool("isReverseHV");

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hc:startPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nStartX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nStartY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hc:endPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nEndX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nEndY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeLine::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("StartX" == sAttributeName)
			m_nStartX = oReader.GetInt();
		else if ("StartY" == sAttributeName)
			m_nStartY = oReader.GetInt();
		else if ("EndX" == sAttributeName)
			m_nEndX = oReader.GetInt();
		else if ("EndY" == sAttributeName)
			m_nEndY = oReader.GetInt();
		else if ("IsReverseHV" == sAttributeName)
			m_shAttr = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPML);
	END_WHILE
}

EShapeType CCtrlShapeLine::GetShapeType() const
{
	return EShapeType::Line;
}

int HWP::CCtrlShapeLine::GetStartX() const
{
	return m_nStartX;
}

int HWP::CCtrlShapeLine::GetStartY() const
{
	return m_nStartY;
}

int HWP::CCtrlShapeLine::GetEndX() const
{
	return m_nEndX;
}

int HWP::CCtrlShapeLine::GetEndY() const
{
	return m_nEndY;
}

void CCtrlShapeLine::ParseElement(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	if (L"loc$" == oObj.GetID())
		oBuffer.Skip(4);

	oBuffer.ReadInt(oObj.m_nStartX);
	oBuffer.ReadInt(oObj.m_nStartY);
	oBuffer.ReadInt(oObj.m_nEndX);
	oBuffer.ReadInt(oObj.m_nEndY);

	if (nSize == oBuffer.GetDistanceToLastPos())
	{
		oBuffer.RemoveLastSavedPos();
		return;
	}

	oBuffer.ReadShort(oObj.m_shAttr);
	oBuffer.Skip(2);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}

void CCtrlShapeLine::ParseCtrl(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}

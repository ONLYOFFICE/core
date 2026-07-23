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

#include "CtrlShapePolygon.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrlShapePolygon::CCtrlShapePolygon()
{}

CCtrlShapePolygon::CCtrlShapePolygon(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapePolygon::CCtrlShapePolygon(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapePolygon::CCtrlShapePolygon(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapePolygon::CCtrlShapePolygon(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	TPoint oPoint{0, 0};

	WHILE_READ_NEXT_NODE(oReader)
	{
		if (GetNodeName(ENode::Point, eType) == oReader.GetName())
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::X, eType) == sAttributeName)
					oPoint.m_nX = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Y, eType) == sAttributeName)
					oPoint.m_nY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)

			m_arPoints.push_back(oPoint);

			oPoint.m_nX = 0;
			oPoint.m_nY = 0;
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, eType);
	}
	END_WHILE

	m_nPoints = m_arPoints.size();
}

EShapeType CCtrlShapePolygon::GetShapeType() const
{
	return EShapeType::Polygon;
}

std::vector<TPoint> CCtrlShapePolygon::GetPoints() const
{
	return m_arPoints;
}

int CCtrlShapePolygon::ParseElement(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nPoints);

	if (0 < oObj.m_nPoints)
	{
		TPoint oPoint;
		for (unsigned int unIndex = 0; unIndex < oObj.m_nPoints; ++unIndex)
		{
			oBuffer.ReadInt(oPoint.m_nX);
			oBuffer.ReadInt(oPoint.m_nY);
			oObj.m_arPoints.push_back(oPoint);
		}
	}

	if (4 == (nSize - oBuffer.GetDistanceToLastPos()))
		oBuffer.Skip(4);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlShapePolygon::ParseCtrl(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

int CCtrlShapePolygon::ParseListHeaderAppend(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);

	oBuffer.ReadShort(oObj.m_shLeftSpace);
	oBuffer.ReadShort(oObj.m_shRightSpace);
	oBuffer.ReadShort(oObj.m_shTopSpace);
	oBuffer.ReadShort(oObj.m_shBottomSpace);
	oBuffer.ReadInt(oObj.m_nMaxTxtWidth);

	oBuffer.Skip(13);

	if (nSize > oBuffer.GetDistanceToLastPos())
	{
		oBuffer.Skip(10);
		HWP_STRING sFieldName;
		oBuffer.ReadString(sFieldName, EStringCharacter::UTF16);

		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());
	}

	return oBuffer.GetDistanceToLastPos(true);
}


}

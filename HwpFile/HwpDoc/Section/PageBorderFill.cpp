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

#include "PageBorderFill.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CPageBorderFill::CPageBorderFill()
{}

CPageBorderFill::CPageBorderFill(CXMLReader& oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::BorderFillId, eType) == sAttributeName)
			m_shBorderFill = oReader.GetInt();
		else if (GetAttributeName(EAttribute::TextBorder, eType) == sAttributeName)
		{
			if (EHanType::HWPX == eType)
				m_bTextBorder = "PAPER" == oReader.GetTextA();
			else
				m_bTextBorder = oReader.GetBool();
		}
		else if (GetAttributeName(EAttribute::HeaderInside, eType) == sAttributeName)
			m_bHeaderInside = oReader.GetBool();
		else if (GetAttributeName(EAttribute::FooterInside, eType) == sAttributeName)
			m_bFooterInside = oReader.GetBool();
		else if (GetAttributeName(EAttribute::FillArea, eType) == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if (GetValueName(EValue::Paper, eType) == sType)
				m_chFillArea = 0;
			else if (GetValueName(EValue::Page, eType) == sType)
				m_chFillArea = 1;
			else if (GetValueName(EValue::Border, eType) == sType)
				m_chFillArea = 2;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::PageOffset, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Left, eType) == sAttributeName)
				m_shOffsetLeft = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Right, eType) == sAttributeName)
				m_shOffsetRight = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Top, eType) == sAttributeName)
				m_shOffsetTop = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Bottom, eType) == sAttributeName)
				m_shOffsetBottom = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
}

CPageBorderFill* CPageBorderFill::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CPageBorderFill* pBorderFill = new CPageBorderFill();

	if (nullptr == pBorderFill)
		return nullptr;

	oBuffer.SavePosition();

	int nAttr;
	oBuffer.ReadInt(nAttr);

	pBorderFill->m_bTextBorder = CHECK_FLAG(nAttr, 0x01);
	pBorderFill->m_bHeaderInside = CHECK_FLAG(nAttr, 0x02);
	pBorderFill->m_bHeaderInside = CHECK_FLAG(nAttr, 0x04);
	pBorderFill->m_chFillArea = (HWP_BYTE)((nAttr >> 3) & 0x03);

	oBuffer.ReadShort(pBorderFill->m_shOffsetLeft);
	oBuffer.ReadShort(pBorderFill->m_shOffsetRight);
	oBuffer.ReadShort(pBorderFill->m_shOffsetTop);
	oBuffer.ReadShort(pBorderFill->m_shOffsetBottom);
	oBuffer.ReadShort(pBorderFill->m_shBorderFill);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return pBorderFill;
}
}

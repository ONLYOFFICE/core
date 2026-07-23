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

#include "HwpRecordTabDef.h"

#include "../Common/NodeNames.h"

namespace HWP
{
TTab::TTab()
{}

TTab::TTab(CXMLReader& oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("pos" == sAttributeName)
			m_nPos = oReader.GetInt();
		else if ("type" == sAttributeName)
			SetType(oReader.GetInt());
		else if ("leader" == sAttributeName)
			m_eLeader = GetLineStyle2(oReader.GetTextA(), EHanType::HWPX);
	}
	END_READ_ATTRIBUTES(oReader)
}

void TTab::SetType(int nValue)
{
	SWITCH(TTab::EType, nValue)
	{
		case EType::LEFT: default: m_eType = EType::LEFT; break;
		case EType::RIGHT: m_eType = EType::RIGHT; break;
		case EType::CENTER: m_eType = EType::CENTER; break;
		case EType::DECIMAL: m_eType = EType::DECIMAL; break;
	}
}

CHwpRecordTabDef::CHwpRecordTabDef(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr), m_nAttr(0)
{}

CHwpRecordTabDef::CHwpRecordTabDef(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nAttr);

	int nCount = 0;
	oBuffer.ReadInt(nCount);

	if (nSize - oBuffer.GetDistanceToLastPos() != nCount * 8)
		return; // TODO:: Error

	TTab *pTab = nullptr;
	for (unsigned int unIndex = 0; unIndex < nCount; ++unIndex)
	{
		pTab = new TTab();

		if (nullptr == pTab)
		{
			oBuffer.Skip(8);
			continue;
		}

		oBuffer.ReadInt(pTab->m_nPos);
		pTab->SetType(oBuffer.ReadByte());
		pTab->m_eLeader = GetLineStyle2(oBuffer.ReadByte());
		oBuffer.Skip(2); // reserved 2 bytes for align

		m_arTabs.push_back(pTab);
	}

	oBuffer.RemoveLastSavedPos();
}

CHwpRecordTabDef::CHwpRecordTabDef(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_TAB_DEF, 0, 0), m_pParent(&oDocInfo), m_nAttr(0)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::AutoTabLeft, eType) == sAttributeName)
		{
			if (oReader.GetBool())
				m_nAttr |= 0x00000001;
			else
				m_nAttr &= 0xFFFFFFFE;
		}
		else if (GetAttributeName(EAttribute::AutoTabRight, eType) == sAttributeName)
		{
			if (oReader.GetBool())
				m_nAttr |= 0x00000002;
			else
				m_nAttr &= 0xFFFFFFFD;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	// The following structure is occurs only in HWPX format
	if (EHanType::HWPX != eType)
		return;
	
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:switch")
		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "hp:default")
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, TabChild, "hh:tabItem")
				m_arTabs.push_back(new TTab(oReader));
			END_WHILE
		END_WHILE
	END_WHILE
}

int CHwpRecordTabDef::GetCount() const
{
	return m_arTabs.size();
}

const TTab* CHwpRecordTabDef::GetTab(unsigned int unIndex) const
{
	if (unIndex >= m_arTabs.size())
		return nullptr;

	return dynamic_cast<const TTab*>(m_arTabs.at(unIndex));
}
}

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

#include "Page.h"
#include "../Common/NodeNames.h"

namespace HWP
{
HWP::CPage::CPage()
{}

CPage::CPage(CXMLReader& oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Landscape, eType) == sAttributeName)
		{
			if (EHanType::HWPX == eType)
				m_bLandscape = "NARROWLY" == oReader.GetTextA();
			else
				m_bLandscape = oReader.GetBool();
		}
		else if (GetAttributeName(EAttribute::Width, eType) == sAttributeName)
			m_nWidth = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Height, eType) == sAttributeName)
			m_nHeight = oReader.GetInt();
		else if (GetAttributeName(EAttribute::GutterType, eType) == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if (GetValueName(EValue::LeftOnly, eType) == sType)
				m_chGutterType = 0;
			else if (GetValueName(EValue::LeftRight, eType) == sType)
				m_chGutterType = 1;
			else if (GetValueName(EValue::TopBottom, eType) == sType)
				m_chGutterType = 2;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::PageMargin, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Left, eType) == sAttributeName)
				m_nMarginLeft = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Right, eType) == sAttributeName)
				m_nMarginRight = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Top, eType) == sAttributeName)
				m_nMarginTop = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Bottom, eType) == sAttributeName)
				m_nMarginBottom = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Header, eType) == sAttributeName)
				m_nMarginHeader = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Footer, eType) == sAttributeName)
				m_nMarginFooter = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Gutter, eType) == sAttributeName)
				m_nMarginGutter = oReader.GetInt();

		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
}

int CPage::GetWidth() const
{
	return m_nWidth;
}

int CPage::GetHeight() const
{
	return m_nHeight;
}

int CPage::GetMarginLeft() const
{
	return m_nMarginLeft;
}

int CPage::GetMarginRight() const
{
	return m_nMarginRight;
}

int CPage::GetMarginTop() const
{
	return m_nMarginTop;
}

int CPage::GetMarginBottom() const
{
	return m_nMarginBottom;
}

int CPage::GetMarginHeader() const
{
	return m_nMarginHeader;
}

int CPage::GetMarginFooter() const
{
	return m_nMarginFooter;
}

int CPage::GetMarginGutter() const
{
	return m_nMarginGutter;
}

CPage* CPage::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	CPage *pPage = new CPage();

	if (nullptr == pPage)
	{
		oBuffer.Skip(nSize);
		oBuffer.RemoveLastSavedPos();
		return nullptr;
	}

	oBuffer.ReadInt(pPage->m_nWidth);
	oBuffer.ReadInt(pPage->m_nHeight);
	oBuffer.ReadInt(pPage->m_nMarginLeft);
	oBuffer.ReadInt(pPage->m_nMarginRight);
	oBuffer.ReadInt(pPage->m_nMarginTop);
	oBuffer.ReadInt(pPage->m_nMarginBottom);
	oBuffer.ReadInt(pPage->m_nMarginHeader);
	oBuffer.ReadInt(pPage->m_nMarginFooter);
	oBuffer.ReadInt(pPage->m_nMarginGutter);

	int nAttr;
	oBuffer.ReadInt(nAttr);

	pPage->m_bLandscape = CHECK_FLAG(nAttr, 0x01);
	pPage->m_chGutterType = (HWP_BYTE)((nAttr >> 1) & 0x03);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return pPage;
}
}

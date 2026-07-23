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

#include "ParagraphStyleManager.h"

namespace NSDocxRenderer
{
	CParagraphStyleManager::CParagraphStyleManager()
	{
		// standard styles
		CParagraphStyle oNormal(L"Normal", L"Normal");
		CParagraphStyle oHeading1(L"Heading1", L"Heading 1");
		CParagraphStyle oHeading2(L"Heading2", L"Heading 2");

		oNormal.bIsDefault = true;

		oHeading1.wsBasedOn = oNormal.wsStyleId;
		oHeading1.nUiPriority = 9;

		oHeading2.wsBasedOn = oNormal.wsStyleId;
		oHeading2.nUiPriority = 9;

		m_arDefaultParagraphStyles.push_back(oNormal);
		m_arDefaultParagraphStyles.push_back(oHeading1);
		m_arDefaultParagraphStyles.push_back(oHeading2);

	}
	CParagraphStyleManager::~CParagraphStyleManager()
	{
		m_arDefaultParagraphStyles.clear();
	}

	std::wstring CParagraphStyleManager::GetDefaultParagraphStyleId(const CParagraph& oParagraph) const noexcept
	{
		if (oParagraph.m_arTextLines.size() > 1) return L"Normal";

		bool isHeading = true;
		for (auto& val : oParagraph.m_arTextLines[0]->m_arConts)
			if (val && val->m_pFontStyle->dFontSize <= m_dAvgFontSize + 1 && !val->m_pFontStyle->bBold)
				isHeading = false;

		return isHeading ? L"Heading1" : L"Normal";
	}
	double CParagraphStyleManager::GetAvgFontSize() const noexcept
	{
		return m_dAvgFontSize;
	}
	void CParagraphStyleManager::UpdateAvgFontSize(double dFontSize)
	{
		m_dAvgFontSize = (m_dAvgFontSize / (m_nN + 1)) * m_nN + (dFontSize / (m_nN + 1));
		m_nN++;
	}

	void CParagraphStyleManager::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		for(auto& val : m_arDefaultParagraphStyles)
			val.ToXml(oWriter);
	}
}


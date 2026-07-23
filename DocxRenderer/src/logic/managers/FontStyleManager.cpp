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

#include "FontStyleManager.h"

#include <utility>

namespace NSDocxRenderer
{
	CFontStyleManager::CFontStyleManager()
	{
	}

	CFontStyleManager::~CFontStyleManager()
	{
		Clear();
	}

	void CFontStyleManager::Clear()
	{
		m_arFontStyles.clear();
	}

	void CFontStyleManager::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		for(auto& val : m_arFontStyles)
			val->ToXml(oWriter);
	}

	std::shared_ptr<CFontStyle> CFontStyleManager::GetOrAddFontStyle(const CFontStyle& oFontStyle)
	{
		return GetOrAddFontStyle(
		            oFontStyle.oBrush,
		            oFontStyle.wsFontName,
		            oFontStyle.dFontSize,
		            oFontStyle.bItalic,
		            oFontStyle.bBold);
	}
	std::shared_ptr<CFontStyle> CFontStyleManager::GetOrAddFontStyle(
	        const NSStructures::CBrush& oBrush,
	        const std::wstring& wsFontName,
	        double dFontSize,
	        bool bItalic,
	        bool bBold)
	{
		for(auto it = m_arFontStyles.begin(); it != m_arFontStyles.end(); ++it)
		{
			if (oBrush.Type == (*it)->oBrush.Type &&
			        oBrush.Color1 == (*it)->oBrush.Color1 &&
			        oBrush.Color2 == (*it)->oBrush.Color2 &&
			        oBrush.Alpha1 == (*it)->oBrush.Alpha1 &&
			        oBrush.Alpha2 == (*it)->oBrush.Alpha2 &&
			        oBrush.LinearAngle == (*it)->oBrush.LinearAngle &&
			        dFontSize == (*it)->dFontSize &&
			        wsFontName == (*it)->wsFontName &&
			        (bItalic == (*it)->bItalic) && (bBold == (*it)->bBold))
			{
				auto val = *it;

				// to the beginning of the list
				if (it != m_arFontStyles.begin())
				{
					m_arFontStyles.erase(it);
					m_arFontStyles.push_front(val);

				}
				return val;
			}
		}
		auto pFontStyle = std::make_shared<CFontStyle>();
		pFontStyle->oBrush = oBrush;
		pFontStyle->wsFontName = wsFontName;
		pFontStyle->dFontSize = dFontSize;
		pFontStyle->bItalic = bItalic;
		pFontStyle->bBold = bBold;

		m_arFontStyles.push_front(pFontStyle);
		return pFontStyle;
	}
}

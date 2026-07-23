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

#pragma once
#include <string>
#include <map>

class ColorTable
{
public:
	ColorTable ()
	{
		InitClrTable ();
	}

	inline std::wstring ConverColorToString(const unsigned int& sKey)
	{
		auto iter = m_Table.find(sKey);
		if (iter == m_Table.end())
		{
			//note if standard color not found, return what we have
			return L"none";
		}
		else
		{
			return iter->second;
		}
	}

	inline bool IsStandardColor(const unsigned int& sKey)
	{
		auto iter = m_Table.find(sKey);
		return iter == m_Table.end() ? false : true;
	}

private:
	std::map<unsigned int, std::wstring> m_Table;

private:
	void InitClrTable()
	{
		if (m_Table.size())
			return;

		//ECMA-376-1:2016 17.18.40 ST_HighlightColor (Text Highlight Colors)
		m_Table.insert({0x000000, L"black" });
		m_Table.insert({0x0000FF, L"blue" });
		m_Table.insert({0x00FFFF, L"cyan" });
		m_Table.insert({0x00008B, L"darkBlue" });
		m_Table.insert({0x008B8B, L"darkCyan" });
		m_Table.insert({0xA9A9A9, L"darkGray" });
		m_Table.insert({0x006400, L"darkGreen" });
		m_Table.insert({0x800080, L"darkMagenta" });
		m_Table.insert({0x8B0000, L"darkRed" });
		m_Table.insert({0x808000, L"darkYellow" });
		m_Table.insert({0x00FF00, L"green" });
		m_Table.insert({0xD3D3D3, L"lightGray" });
		m_Table.insert({0xFF00FF, L"magenta" });
		m_Table.insert({0xFF0000, L"red" });
		m_Table.insert({0xFFFFFF, L"white" });
		m_Table.insert({0xFFFF00, L"yellow" });

		//note More colors here
		//core\Common\3dParty\html\css\src\ConstValues.h
		//core\DesktopEditor\agg-2.4\svg\agg_svg_color_parser.cpp
	}
};

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
#include <algorithm>

enum class eSimpleLineType
{
	sltUnknown,
	sltHDot, //Horizontal
	sltVDot, //Vertical
	sltHDash,
	sltVDash,
	sltHLongDash,
	sltVLongDash,
	sltHWave,
	sltVWave
};

enum class eLineType
{
	ltUnknown,
	ltSingle,
	ltDouble,
	ltThick,
	ltDotted,
	ltDottedHeavy,
	ltDash,
	ltDashedHeavy,
	ltDashLong,
	ltDashLongHeavy,
	ltDotDash,
	ltDashDotHeavy,
	ltDotDotDash,
	ltDashDotDotHeavy,
	ltWave,
	ltWavyHeavy,
	ltWavyDouble,
	ltWords,
	ltNone
};

class LinesTable
{
public:
	LinesTable()
	{
		InitLinesTable();
	}

	inline std::wstring ConvertLineToString(const eLineType& sKey)
	{
		auto iter = m_Table.find(sKey);
		return iter == m_Table.end() ? L"\"none\"" : iter->second;
	}

	inline std::wstring ConvertLineToStringPptx(const eLineType& sKey)
	{
		auto iter = m_TablePptx.find(sKey);
		return iter == m_TablePptx.end() ? L"\"none\"" : iter->second;
	}

private:
	std::map<eLineType, std::wstring> m_Table;
	std::map<eLineType, std::wstring> m_TablePptx;

private:
	void InitLinesTable()
	{
		if (m_Table.size())
			return;

		//ECMA-376 Part 1 17.18.99 ST_Underline (Underline Patterns)
		m_Table.insert({eLineType::ltSingle,          L"\"single\""});
		m_Table.insert({eLineType::ltDouble,          L"\"double\"" });
		m_Table.insert({eLineType::ltThick,           L"\"thick\"" });
		m_Table.insert({eLineType::ltDotted,          L"\"dotted\"" });
		m_Table.insert({eLineType::ltDottedHeavy,     L"\"dottedHeavy\"" });
		m_Table.insert({eLineType::ltDash,            L"\"dash\"" });
		m_Table.insert({eLineType::ltDashedHeavy,     L"\"dashedHeavy\"" });
		m_Table.insert({eLineType::ltDashLong,        L"\"dashLong\"" });
		m_Table.insert({eLineType::ltDashLongHeavy,   L"\"dashLongHeavy\"" });
		m_Table.insert({eLineType::ltDotDash,         L"\"dotDash\"" });
		m_Table.insert({eLineType::ltDashDotHeavy,    L"\"dashDotHeavy\"" });
		m_Table.insert({eLineType::ltDotDotDash,      L"\"dotDotDash\"" });
		m_Table.insert({eLineType::ltDashDotDotHeavy, L"\"dashDotDotHeavy\"" });
		m_Table.insert({eLineType::ltWave,            L"\"wave\"" });
		m_Table.insert({eLineType::ltWavyHeavy,       L"\"wavyHeavy\"" });
		m_Table.insert({eLineType::ltWavyDouble,      L"\"wavyDouble\"" });
		m_Table.insert({eLineType::ltWords,           L"\"words\"" });
		m_Table.insert({eLineType::ltNone,            L"\"none\"" });

		m_TablePptx.insert({eLineType::ltSingle,          L"\"sng\""});
		m_TablePptx.insert({eLineType::ltDouble,          L"\"dbl\"" });
		m_TablePptx.insert({eLineType::ltThick,           L"\"heavy\"" });
		m_TablePptx.insert({eLineType::ltDotted,          L"\"dotted\"" });
		m_TablePptx.insert({eLineType::ltDottedHeavy,     L"\"dottedHeavy\"" });
		m_TablePptx.insert({eLineType::ltDash,            L"\"dash\"" });
		m_TablePptx.insert({eLineType::ltDashedHeavy,     L"\"dashHeavy\"" });
		m_TablePptx.insert({eLineType::ltDashLong,        L"\"dashLong\"" });
		m_TablePptx.insert({eLineType::ltDashLongHeavy,   L"\"dashLongHeavy\"" });
		m_TablePptx.insert({eLineType::ltDotDash,         L"\"dotDash\"" });
		m_TablePptx.insert({eLineType::ltDashDotHeavy,    L"\"dotDashHeavy\"" });
		m_TablePptx.insert({eLineType::ltDotDotDash,      L"\"dotDotDash\"" });
		m_TablePptx.insert({eLineType::ltDashDotDotHeavy, L"\"dotDotDashHeavy\"" });
		m_TablePptx.insert({eLineType::ltWave,            L"\"wavy\"" });
		m_TablePptx.insert({eLineType::ltWavyHeavy,       L"\"wavyHeavy\"" });
		m_TablePptx.insert({eLineType::ltWavyDouble,      L"\"wavyDbl\"" });
		m_TablePptx.insert({eLineType::ltWords,           L"\"words\"" });
		m_TablePptx.insert({eLineType::ltNone,            L"\"none\"" });
	}
};


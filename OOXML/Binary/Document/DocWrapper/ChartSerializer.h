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
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>

namespace OOX
{
	namespace Spreadsheet
	{
		class CXlsx;
		class CT_Chart;
		class CWorksheet;
		class CCell;
		class CXfs;
		class CT_StrRef;
		class CT_NumRef;
		class CT_MultiLvlStrRef;
		class CT_AxDataSource;
		class CT_NumDataSource;
		class CT_Title;
	}
}

namespace BinXlsxRW {

	class ChartWriter
	{
	public:
        std::map<std::wstring, std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>*>	m_mapSheets;
        boost::unordered_map<std::wstring, unsigned int>									m_mapFormats;

        std::vector<OOX::Spreadsheet::CXfs*>    m_aXfs;
        std::vector<std::wstring>               m_aTableNames;

        int m_nRow1;
		int m_nCol1;
		int m_nRow2;
		int m_nCol2;

        ChartWriter();
		~ChartWriter();
		void toXlsx(OOX::Spreadsheet::CXlsx& oXlsx);
		void parseChart(const OOX::Spreadsheet::CT_Chart* pChart);

    private:
		OOX::Spreadsheet::CWorksheet* toXlsxGetSheet(std::vector<OOX::Spreadsheet::CWorksheet*>& arWorksheets, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets, const std::wstring& sName);
		void toXlsxSheetdata(OOX::Spreadsheet::CWorksheet* pWorksheet, const std::map<int, std::map<int, OOX::Spreadsheet::CCell*>*>& rows, std::vector<std::wstring>& aSharedStrings);
		
		void parseCell(const std::wstring& sheet, const int& nRow, const int& nCol, const std::wstring& val, std::wstring format, bool bAlwaysSharedString = false);
		OOX::Spreadsheet::CCell* parseCreateCell(const int& nRow, const int& nCol, const std::wstring& val, std::wstring format);

		void parseStrRef(const OOX::Spreadsheet::CT_StrRef* pStrRef, bool bUpdateRange, const wchar_t* cRangeName);
		void parseNumRef(const OOX::Spreadsheet::CT_NumRef* pNumRef, bool bUpdateRange, const wchar_t* cRangeName);
		void parseMultiLvlStrRef(const OOX::Spreadsheet::CT_MultiLvlStrRef* pMultiLvlStrRef, bool bUpdateRange, const wchar_t* cRangeName);
		void parseAxDataSource(const OOX::Spreadsheet::CT_AxDataSource* pAxDataSource, bool bUpdateRange, const wchar_t* cRangeName);
		void parseNumDataSource(const OOX::Spreadsheet::CT_NumDataSource* pNumDataSource, bool bUpdateRange, const wchar_t* cRangeName);
	};
}
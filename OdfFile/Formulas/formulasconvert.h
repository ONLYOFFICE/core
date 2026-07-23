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
#include "../Common/CPScopedPtr.h"

namespace cpdoccore {

namespace formulasconvert {


// Working with OpenOffice formulas, conversion.
// See examples in ut_formulasconvert.cpp test
class odf2oox_converter
{
public:
	odf2oox_converter();
    ~odf2oox_converter();

    // of:=SUM([.DDA1:.BA3]) -> SUM(DDA1:BA3)
    std::wstring convert(std::wstring const & expr);
    
    // $Sheet1.$A$1 -> Sheet1!$A$1
	std::wstring convert_named_ref(std::wstring const & expr, bool withTableName = true, std::wstring separator = L" ", bool bAbsoluteAlways = false);
	std::wstring get_table_name();

	std::wstring convert_list_values(std::wstring const & expr);

	//a-la convert without check formula
    std::wstring convert_named_expr(std::wstring const & expr, bool withTableName = true, bool bAbsoluteAlways = false);
	
	//Sheet2.C3:Sheet2.C19 -> Sheet2!C3:C19
    std::wstring convert_chart_distance(std::wstring const & expr);

	void split_distance_by(const std::wstring& expr, const std::wstring& by, std::vector<std::wstring>& out);
    
    std::wstring convert_ref(std::wstring const & expr);

	std::wstring convert_spacechar(std::wstring expr);

    // =[.A1]+[.B1] -> table = ""; ref = "A1"
    // of:=['Sheet2 A'.B2] -> table= "Sheet2 A"; ref = "B2"
    bool find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref);

	//Table!.$A$1:$A2 -> ref $A$1 -> ref $A$2
	bool find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last);

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

class oox2odf_converter
{
public:
    oox2odf_converter();
    ~oox2odf_converter();

    // SUM(DDA1:BA3) -> of:=SUM([.DDA1:.BA3]) 
    std::wstring convert(std::wstring const & expr);
	std::wstring convert_formula(std::wstring const & expr);
 	
	std::wstring convert_conditional_formula(std::wstring const & expr);
  
    // Sheet1!$A$1 -> $Sheet1.$A$1
    std::wstring convert_named_ref(std::wstring const & expr);
	std::wstring convert_named_formula(std::wstring const & expr);
	bool is_simple_ref(std::wstring const & expr);

	std::wstring get_table_name();
    void set_table_name(std::wstring const& val);

	//Sheet2!C3:C19 -> Sheet2.C3:Sheet2.C19 
    std::wstring convert_ref_distances(std::wstring const & expr, std::wstring const& separator_in, std::wstring const& separator_out);
    
    std::wstring convert_ref(std::wstring const & expr);

	std::wstring convert_spacechar(std::wstring expr);

	int get_count_value_points(std::wstring expr);

 //   // =[.A1]+[.B1] -> table = ""; ref = "A1"
 //   // of:=['Sheet2 A'.B2] -> table= "Sheet2 A"; ref = "B2"
 //   bool find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref);

	////ref $A$1 -> ref $A$2 -> Table!.$A$1:$A2 
	//bool find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last);

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

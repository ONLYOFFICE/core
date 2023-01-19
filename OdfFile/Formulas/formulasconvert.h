/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include <string>
#include <vector>
#include "../Common/CPScopedPtr.h"

namespace cpdoccore {

namespace formulasconvert {


// Работа с форумулами OpenOffice, преобразование.
// Примеры см. в тесте ut_formulasconvert.cpp
class odf2oox_converter
{
public:
	odf2oox_converter();
    ~odf2oox_converter();

    // of:=SUM([.DDA1:.BA3]) -> SUM(DDA1:BA3)
    std::wstring convert(std::wstring const & expr);
    
    // $Лист1.$A$1 -> Лист1!$A$1
	std::wstring convert_named_ref(std::wstring const & expr, bool withTableName = true, std::wstring separator = L" ", bool bAbsoluteAlways = false);
	std::wstring get_table_name();

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
  
    // Лист1!$A$1 -> $Лист1.$A$1 
    std::wstring convert_named_ref(std::wstring const & expr);
	std::wstring convert_named_formula(std::wstring const & expr);
	bool is_simple_ref(std::wstring const & expr);

	std::wstring get_table_name();

	//Sheet2!C3:C19 -> Sheet2.C3:Sheet2.C19 
    std::wstring convert_chart_distance(std::wstring const & expr);
    
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

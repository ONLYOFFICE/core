#pragma once

#include <string>
#include <cpdoccore/CPScopedPtr.h>

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
    std::wstring convert_named_ref(std::wstring const & expr);

	//Sheet2.C3:Sheet2.C19 -> Sheet2!C3:C19
    std::wstring convert_chart_distance(std::wstring const & expr);
    
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

	std::wstring find_base_cell(std::wstring const & expr);

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

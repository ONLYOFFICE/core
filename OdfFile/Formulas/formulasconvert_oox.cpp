/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "formulasconvert.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include"../../OOXML/Base/Unit.h"
#include "boost/lexical_cast.hpp"
//#include <random>

namespace cpdoccore {
namespace formulasconvert {

	static std::wstring forbidden_formulas1[] =
	{
		L"NULLFORMULA()"
		/*
		L"BETADIST",
		L"CEILING",
		L"FLOOR",
		L"RANK",
		L"ROUND",
		L"ROUNDDOWN",
		L"ROUNDUP",
		L"SUBTOTAL",
		L"FORMULA",
		L"ISREF"*/
	};

class oox2odf_converter::Impl
{
public:

    std::wstring convert(const std::wstring& expr);
	std::wstring convert_formula(const std::wstring& expr);
	std::wstring convert_conditional_formula(const std::wstring& expr);

	std::wstring convert_ref_distances(std::wstring const& expr, std::wstring const& separator_in, std::wstring const& separator_out);
    static void replace_cells_range(std::wstring& expr, bool bSelect = true);
  
	static void replace_semicolons(std::wstring& expr);
    static void replace_vertical(std::wstring& expr);
    static void replace_space(std::wstring& expr);

	static std::wstring replace_cells_range_formater1(boost::wsmatch const & what);
	static std::wstring replace_cells_range_formater2(boost::wsmatch const & what);
	static std::wstring replace_arguments(boost::wsmatch const & what);
	static std::wstring convert_scobci(boost::wsmatch const & what);
	
	static std::vector<std::map<std::wstring, std::wstring>> mapReplacements;

	static std::wstring replace_tilda_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
			return L";";
		else if (what[2].matched)
			return what[2].str();    
		else if (what[3].matched)
			return what[3].str();
		//else if (what[4].matched)
		//    return what[4].str();
		else
			return L"";
	}
	static std::wstring replace_semicolons_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
			return L";";
		else if (what[2].matched)
			return what[2].str();    
		else if (what[3].matched)
			return what[3].str();
		//else if (what[4].matched)
		//    return what[4].str();
		else
			return L"";
	}
	static std::wstring replace_vertical_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
		{
			std::wstring inner = what[1].str();
			XmlUtils::replace_all( inner, L";", L"|");
			return L"{" + inner + L"}";
		}    
		else if (what[2].matched)
			return what[2].str();
		else if (what[3].matched)
			return what[3].str();
		
		return L"";
	}
	static std::wstring replace_space_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
		{
			std::wstring inner = what[1].str();
			XmlUtils::replace_all( inner, L",", L" ");
			return inner;
		}    
		else if (what[2].matched)
			return what[2].str();
		else if (what[3].matched)
			return what[3].str();

		return L"";
	}
	static void oox_replace_tmp_back(std::wstring& expr)
	{
		for (auto key : mapReplacements.back())
		{
			XmlUtils::replace_all(expr, key.first, key.second);
		}

		XmlUtils::replace_all(expr, L"KVADRATIN", L"[");
		XmlUtils::replace_all(expr, L"KVADRATOUT", L"]");

		return;
	}

	static void oox_replace_tmp(std::wstring &expr)
	{
		//std::random_device genSource;
		//std::uniform_int_distribution<> generator(0, 23);
		//for (int index = 0; index < 5; index++)
		//{
		//	key += wchar_t(L'a' + generator(genSource));
		//}

		std::wstring key = L"aaaaaaaaaaaaaaaaaaaaaaaa";
		for (unsigned i = 0; i < 23; ++i)
		{
			unsigned j = rand() % (i + 1);
			key[i] = key[j];
			key[j] = wchar_t(L'a' + i);
		}
		mapReplacements.back().insert(std::make_pair(key, expr));
		expr = key;
	}

	static bool is_forbidden1(const std::wstring & formula)
	{
		for (size_t i = 0; i < 1; i++)
		 {
			if (boost::algorithm::contains(formula, forbidden_formulas1[i]))
				return true;
		}
		return false;
	}
	void replace_named_ref(std::wstring & expr);
    void replace_named_formula(std::wstring & expr);
	bool is_simple_ref(std::wstring const & expr);
	
	static bool isFindBaseCell_;

	static std::wstring table_name_;
};

bool			oox2odf_converter::Impl::isFindBaseCell_ = false;
std::wstring	oox2odf_converter::Impl::table_name_ = L"";
std::vector<std::map<std::wstring, std::wstring>> oox2odf_converter::Impl::mapReplacements;

void oox2odf_converter::Impl::replace_cells_range(std::wstring& expr, bool bSelect)
{
	boost::wregex re(L"(([:$!])+)|(\\S+\\d+)");

	boost::wsmatch result;
	bool b = boost::regex_search(expr, result, re);

	if (b)
	{
		boost::wregex re1(L"(\\$?[^\\']+\\!)?([a-zA-Z$]+\\d*)(\\:[a-zA-Z$]+\\d*)?");
//                          $   Sheet2   ! $ A1                 :  $ B5    
//                          $   Sheet2   ! $ A                  :  $ A    
//                          $   Sheet2   ! $ 1                  :  $ 1    
		std::wstring workstr = expr;
		
		std::wstring res = boost::regex_replace(
			workstr,	
			re1,
			bSelect ? &replace_cells_range_formater1 : &replace_cells_range_formater2,
			boost::match_default | boost::format_all);
	     
		 expr = res;
	}
	else
	{

	}

	return;
}

size_t getColAddressInv(const std::wstring & a_)
{
    std::wstring a = a_;
    ::boost::algorithm::to_upper(a);
    static const size_t r = (L'Z' - L'A' + 1);
    size_t mul = 1;
    bool f = true;
    size_t res = 0;

	for (int i = a.length() - 1; i >= 0; i--)
    {
        size_t v = a[i] - L'A';
        if (f)
            f = false;
        else
            v += 1;
        res += v * mul;
        mul *= r;
    }
    return res;
}

size_t getRowAdderssInv(const std::wstring & a_)
{
	int sz = a_.length();
	if (a_.length()>0)
	{
	   return boost::lexical_cast<size_t>(a_)-1;
	}
	else
		return 0;
}
void splitCellAddress(const std::wstring & a_, std::wstring & col, std::wstring & row)
{   
	std::wstring a = a_;

	std::reverse(a.begin(), a.end());
    ::XmlUtils::replace_all( a, L"$", L"");
    //::XmlUtils::replace_all( a, L"'", L"");
	::boost::algorithm::to_upper(a);

	for (size_t i = 0; i < a.length(); i++)
    {
		if (a[i] >= L'0' && a[i] <= L'9')
			row += a[i];
		else
			col += a[i];
    }
	std::reverse(col.begin(), col.end());
	std::reverse(row.begin(), row.end());
}

void getCellAddressInv(const std::wstring & a_, int & col, int & row)
{
    std::wstring colStr=L"", rowStr=L"";
    splitCellAddress(a_, colStr, rowStr);
    
    col = getColAddressInv(colStr);
	row = getRowAdderssInv(rowStr);

	if (col > 16384) col= -1;
}

bool IsRefPresent(const std::wstring& ref_test)
{
	int col = -1, row = -1;
	getCellAddressInv(ref_test, col, row);

	if (col >= 0 && row >=0) return true;
	return false;
}
std::wstring oox2odf_converter::Impl::replace_cells_range_formater1(boost::wsmatch const & what)
{
    const size_t sz = what.size();
	
	if (sz > 3)
    {
		std::wstring s;
		std::wstring sheet = what[1].matched ? what[1].str() : L"";

        std::wstring c1 = what[2].str(); 
        std::wstring c2 = what[3].str(); 

		if (!c1.empty() || !c2.empty() || !sheet.empty())
		{      
			XmlUtils::replace_all( sheet, L"!", L"");

			if (isFindBaseCell_ && table_name_.empty() && !sheet.empty())
			{
				table_name_ = L"$" + sheet + L".$A$1";
			}	
			if (!sheet.empty()  && (std::wstring::npos != c1.find(L"$"))) sheet = L"$"  + sheet;
			if (!c2.empty() && c2.substr(0, 1) == L":") 
				c2 = c2.substr(1);

			bool bRefPresent = true;
			
			if (sheet.empty() && c2.empty())
			{
				bRefPresent = IsRefPresent(c1);
			}
			
			if (bRefPresent)
			{
				s =  L"["  + sheet + L"." + c1 + (c2.empty() ? L"" : (L":" + sheet  + L"." + c2)) + std::wstring(L"]");
			}
			else
			{
				s = c1;
			}
		}
        return s;
	}
	else
	{
        const std::wstring c1 = what[1].str(); 
        const std::wstring c2 = what[2].str(); 

		const std::wstring s =  std::wstring(L"[.") +  c1 + (c2.empty() ? L"" : (L":." + c2) ) + std::wstring(L"]");

		return s; 
	}
	return L"";
}	

std::wstring oox2odf_converter::Impl::replace_cells_range_formater2(boost::wsmatch const & what)
{
    const size_t sz = what.size();
	
	if (sz > 3)
    {
		std::wstring s;
		std::wstring sheet = what[1].matched ? what[1].str() : L"";

        std::wstring c1 = what[2].str(); 
        std::wstring c2 = what[3].str(); 

		if (!c1.empty() || !c2.empty() || !sheet.empty())
		{      
			XmlUtils::replace_all( sheet, L"!", L"");

			if (isFindBaseCell_ && table_name_.empty() && !sheet.empty())
			{
				table_name_ = L"$" + sheet + L".$A$1";
			}	
			if (!sheet.empty()  && (std::wstring::npos != c1.find(L"$"))) sheet = L"$"  + sheet;
			
			if (!c2.empty() && c2.substr(0, 1) == L":") 
				c2 = c2.substr(1);

			s = (sheet.empty() ? L"" : (sheet + L".")) + c1 + (c2.empty() ? L"" : (L":" + (sheet.empty() ? L"" : (sheet + L".")) + c2)) + std::wstring(L"");
		}
        return s;
	}
	else
	{
        const std::wstring c1 = what[1].str(); 
        const std::wstring c2 = what[2].str(); 

		const std::wstring s =  std::wstring(L".") +  c1 + (c2.empty() ? L"" : (L":." + c2) );

		return s; 
	}
	return L"";
}
		
void oox2odf_converter::Impl::replace_named_formula(std::wstring & expr)
{
	table_name_.clear();
	
	isFindBaseCell_ = true;
	expr = convert_formula(expr);
	isFindBaseCell_ = false;
}
bool oox2odf_converter::Impl::is_simple_ref(std::wstring const & expr)
{
	if (expr.find(L"(") != std::wstring::npos) return false;
	if (expr.find(L" ") != std::wstring::npos) return false;
	if (expr.find(L";") != std::wstring::npos) return false;

	boost::wsmatch match;
	if (boost::regex_search(expr, match, boost::wregex(L"([\\w]+\\!)?\\$?[a-zA-Z]+\\$?\\d+(\\:\\$?[a-zA-Z]+\\$?\\d+)?")))
	{
		return true;
	}
	return false;
}

void oox2odf_converter::Impl::replace_named_ref(std::wstring & expr)
{
	table_name_.clear();
	
	isFindBaseCell_ = true;
	std::wstring workstr = expr, out;
	
	mapReplacements.emplace_back();

	replace_vertical(workstr);
	replace_semicolons(workstr);		
	
	std::wstring res1 = boost::regex_replace(
        workstr,
		boost::wregex(L"('.*?')|(\".*?\")"),
		&oox2odf_converter::Impl::convert_scobci, boost::match_default | boost::format_all);

	std::vector<std::wstring> distance;

	boost::algorithm::split(distance, res1, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

	for (size_t i = 0; i < distance.size(); i++)
	{		
		std::wstring &d = distance[i];

		replace_cells_range(d, false);

		oox_replace_tmp_back(d);
			
		out = out + d + std::wstring(L";");
	}

	if (!out.empty()) expr = out.substr(0, out.length() - 1);

	isFindBaseCell_ = false;

	if (table_name_.empty() == false)
	{
		oox_replace_tmp_back(table_name_);
	}
	mapReplacements.pop_back();
}



// TODO
// заменить запятые на точки с запятой во всех вхождениях кроме находящихся в кавычках --*и в фигурных скобках*--
// TODO: проверить как сохраняются кавычки в строке
void oox2odf_converter::Impl::replace_semicolons(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        //boost::wregex(L"(;)|(?:\".*?\")|(?:'.*?')"),
        boost::wregex(L"(,)|(\".*?\")|('.*?')"),
        &replace_semicolons_formater,
        boost::match_default | boost::format_all);
     expr = res;
}


//void oox2odf_converter::Impl::split_(std::wstring& expr)
//{
//     const std::wstring res = boost::regex_split(
//        expr,
//        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
//        &replace_vertical_formater,
//        boost::match_default | boost::format_all);
//     expr = res;
//}

// заменить вертикальную черту во всех вхождениях в фигурных скобках, но не внутри строк
void oox2odf_converter::Impl::replace_vertical(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
        &replace_vertical_formater,
        boost::match_default | boost::format_all);
     expr = res;
}
// заменить запятую во всех вхождениях на пробел 
void oox2odf_converter::Impl::replace_space(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
        &replace_space_formater,
        boost::match_default | boost::format_all);
     expr = res;
}

std::wstring oox2odf_converter::Impl::convert_scobci(boost::wsmatch const & what)
{
    if (what[1].matched)
    {
        std::wstring inner = what[1].str();
		oox_replace_tmp(inner);
		return inner;
	}
    else if (what[2].matched)
    {
        std::wstring inner = what[2].str();
        oox_replace_tmp(inner);
		return inner;
	}
    else if (what[3].matched)
        return what[3].str();

	return L"";
}

std::wstring  oox2odf_converter::Impl::replace_arguments(boost::wsmatch const & what)
{
	std::wstring out;

	size_t sz = what.size();

    if (what[1].matched)
    {
        out = what[1].str();
	}
	else if (what[2].matched)
	{
        out = what[2].str();
	}
    if (!out.empty())
    {
		std::vector<std::wstring> distance;

		boost::algorithm::split(distance, out, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

		out = L"";
		for (size_t i = 0; i < distance.size(); i++)
		{				
			replace_cells_range(distance[i]);

			out = out + distance[i] + std::wstring(L";");
		}

		if (!out.empty()) out = out.substr(0, out.length() - 1);
	
    }    
    else if (what[3].matched)
        out = what[3].str();
	return out ;
}
std::wstring oox2odf_converter::Impl::convert(const std::wstring& expr)
{
    if (is_forbidden1(expr))
        return L"NULLFORMULA";

	std::wstring workstr = expr;
    replace_cells_range(workstr);
    replace_vertical(workstr);
    replace_semicolons(workstr);
    return workstr;
}
// (Formula) -> of:=(Formula) 

std::wstring oox2odf_converter::Impl::convert_formula(const std::wstring & expr)
{	
    std::wstring workstr = expr;

	mapReplacements.emplace_back();
	std::wstring res1 = boost::regex_replace(
        workstr,    
		boost::wregex(L"('.*?')|(\".*?\")"),
		&oox2odf_converter::Impl::convert_scobci, boost::match_default | boost::format_all);
	
	std::wstring res = boost::regex_replace(
		res1,
		boost::wregex(L"(?!([[:Unicode:]\\w^0-9]+\\d*\\())(((\[[0-9]+\])?[[[:Unicode:]\\w^0-9]+\\!)?\\$?[\\w^0-9]*\\$?\\d*(\\:\\$?[\\w^0-9]*\\$?\\d*){0,1})"),
		&oox2odf_converter::Impl::replace_arguments, boost::match_default | boost::format_all);

	//SUBTOTAL(109,Expense31[Amount])
	XmlUtils::replace_all( res, L"[", L"KVADRATIN");
	XmlUtils::replace_all( res, L"]", L"KVADRATOUT");

	if (res1 == res)
	{
		//IMCONJUGATE_emb.xlsx
	
		res = boost::regex_replace(
			res1,	
			boost::wregex(L"(\\$?\\w+\\!)?([a-zA-Z$]+\\d{1,})\\:?([a-zA-Z$]+\\d{1,})?"),
			&replace_cells_range_formater1,
			boost::match_default | boost::format_all);
	}

	oox_replace_tmp_back(res);

	replace_vertical(res);   
	replace_semicolons(res);

	if (table_name_.empty() == false)
	{
		oox_replace_tmp_back(table_name_);
	}

	mapReplacements.pop_back();

    return std::wstring(L"of:=") + res;

}
std::wstring oox2odf_converter::Impl::convert_conditional_formula(const std::wstring & expr)
{
    std::wstring workstr = expr;

	mapReplacements.emplace_back();

	std::wstring res1 = boost::regex_replace(
        workstr,
		boost::wregex(L"('.*?')|(\".*?\")"),
		&oox2odf_converter::Impl::convert_scobci, boost::match_default | boost::format_all);
	
	std::wstring res = boost::regex_replace(
		res1,
		boost::wregex(L"(?:(?=[()])(.*?)(?=[)]))"),
		&oox2odf_converter::Impl::replace_arguments, boost::match_default | boost::format_all);

	if (res1 == res)
	{
		res = boost::regex_replace(res1,	
			boost::wregex(L"(\\$?\\w+\\!)?([a-zA-Z$]+\\d{1,})\\:?([a-zA-Z$]+\\d{1,})?"),
			&replace_cells_range_formater1,
			boost::match_default | boost::format_all);
	     
	}

	//XmlUtils::replace_all(res, L"&", L"&amp;");
    oox_replace_tmp_back( res);

	replace_vertical(res);
	replace_semicolons(res);
    
	mapReplacements.pop_back();
	//XmlUtils::replace_all( res, L"PROBEL", L" ");
   
	return res;

}
//Sheet2!C3:C19,Sheet2!L27:L34
//в
//Sheet2.C3:Sheet2.C19 Sheet2.L29:Sheet2.L36
//todooo
std::wstring oox2odf_converter::Impl::convert_ref_distances(std::wstring const& expr1, std::wstring const& separator_in, std::wstring const& separator_out)
{
	std::wstring expr = expr1;
	size_t res = expr.find(L"(");
	if (res == 0) expr = expr.substr(res + 1, expr.size()-1);

	res= expr.rfind(L")");
	if (res ==expr.size()-2) expr = expr.substr(0, res);

	//распарсить по диапазонам - пробел -> separator

	std::vector<std::wstring> distance_inp;
	std::vector<std::wstring> distance_out;
	
	boost::algorithm::split(distance_inp,expr, boost::algorithm::is_any_of(separator_in), boost::algorithm::token_compress_on);

	for (size_t i = 0; i < distance_inp.size(); i++)
	{
		std::wstring sheet;
		std::vector<std::wstring> range;
		std::vector<std::wstring> cells;

		boost::algorithm::split(range, distance_inp[i], boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

		for (size_t j = 0 ; j < range.size(); j++)
		{
			int pos = range[j].find('!'); 
			if (0 <= pos)
			{
				if (sheet.empty())
					sheet = range[j].substr(0, pos);		
			}
			cells.push_back(range[j].substr(pos + 1));

		}
		std::wstring cells_out;
		for (size_t c = 0; c < cells.size(); c++)
		{
			if (!sheet.empty())
				cells_out += sheet + L".";
			cells_out += cells[c];
			cells_out += L":";
		}
		int res1 = sheet.find(L"-");
		int res2 = sheet.find(L"'");
		
		if (res1 >= 0 && !(res2 == 0))
		{
			sheet = L"'" + sheet + L"'";
		}

		distance_out.push_back(cells_out.substr(0, cells_out.size()-1));
	}
	std::wstring result;

	for (size_t i = 0 ; i < distance_out.size(); i++)
	{
		result += distance_out[i];
		result += separator_out;
	}
	return result.substr(0, result.size()-1);
}
oox2odf_converter::oox2odf_converter(): impl_(new oox2odf_converter::Impl)
{
}

oox2odf_converter::~oox2odf_converter()
{
}

std::wstring oox2odf_converter::convert(const std::wstring& expr)
{
    return impl_->convert(expr);
}
std::wstring oox2odf_converter::convert_conditional_formula(const std::wstring& expr)
{
    return impl_->convert_conditional_formula(expr);
}
std::wstring oox2odf_converter::convert_formula(const std::wstring& expr)
{
    return impl_->convert_formula(expr);
}
std::wstring oox2odf_converter::convert_ref_distances(std::wstring const& expr, std::wstring const& separator_in, std::wstring const& separator_out)
{
    return impl_->convert_ref_distances(expr, separator_in, separator_out);
}
std::wstring oox2odf_converter::convert_named_ref(const std::wstring& expr)
{
    std::wstring workstr = expr;
    impl_->replace_named_ref(workstr);
    return workstr;
}
std::wstring oox2odf_converter::convert_named_formula(const std::wstring& expr)
{
    std::wstring workstr = expr;
    impl_->replace_named_formula(workstr);
    return workstr;
}
bool oox2odf_converter::is_simple_ref(std::wstring const & expr)
{
	return impl_->is_simple_ref(expr);
}
void oox2odf_converter::set_table_name(std::wstring const& val)
{
	impl_->table_name_ = val;
}
std::wstring oox2odf_converter::get_table_name()
{
    return impl_->table_name_;
}
std::wstring oox2odf_converter::convert_ref(std::wstring const & expr)
{
    std::wstring workstr = expr;
    impl_->replace_cells_range(workstr);
    return workstr;
}
std::wstring oox2odf_converter::convert_spacechar(std::wstring expr)
{
	while(true)
	{
		int pos = expr.find(L"%20");
		if (pos <0)break;

		expr.replace(pos,3,L" ");
	}
	return expr; 
}
int oox2odf_converter::get_count_value_points(std::wstring expr)
{
	int count =0;
	std::vector< std::wstring > splitted;
    
	XmlUtils::replace_all( expr, L"(", L"");
	XmlUtils::replace_all( expr, L")", L"");
	boost::algorithm::split(splitted, expr, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

	for (size_t i=0; i < splitted.size(); i++)
	{
		int res = splitted[i].find(L"!");
		if (res > 0) splitted[i] = splitted[i].substr(res+1, splitted[i].size()-res);

		std::vector< std::wstring >cells;
		boost::algorithm::split(cells, splitted[i], boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
		if (cells.size() >1)
		{
			int col_1, row_1, col_2, row_2;
			getCellAddressInv(cells[0],col_1,row_1);
			getCellAddressInv(cells[1],col_2,row_2);

			count += (std::max)(col_2 - col_1 + 1, row_2 - row_1 + 1);

		}
		else count ++; 
	}
	return count;
}


}
}

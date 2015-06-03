#include "formulasconvert.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

namespace cpdoccore {
namespace formulasconvert {

class odf2oox_converter::Impl
{
public:
    std::wstring convert(const std::wstring& expr);
	std::wstring convert_chart_distance(const std::wstring& expr);
    void replace_cells_range(std::wstring& expr);
    bool check_formula(std::wstring& expr);
    void replace_semicolons(std::wstring& expr);
    void replace_vertical(std::wstring& expr);
    void replace_space(std::wstring& expr);

    void replace_named_ref(std::wstring & expr);
    bool find_first_ref(std::wstring const & expr, std::wstring & table,  std::wstring & ref);
	bool find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last);
};
bool odf2oox_converter::Impl::find_first_last_ref(std::wstring const & expr, std::wstring & table,std::wstring & ref_first,std::wstring & ref_last)
{	
	std::vector< std::wstring > splitted;
    
	boost::algorithm::split(splitted, expr, boost::algorithm::is_any_of(L".:"), boost::algorithm::token_compress_on);
    
	if (splitted.size()==3)
	{
		table	= splitted[0];	
		ref_first = splitted[1];
		ref_last = splitted[2];
		return true;
	}
	if (splitted.size()==4)
	{
		table	= splitted[0];	
		ref_first = splitted[1];
		ref_last = splitted[3];
		return true;
	}
	return false;
}

bool odf2oox_converter::Impl::find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref)
{
    boost::wregex re(L"\\[(?:\\$)?([^\\.]+?){0,1}\\.([a-zA-Z\\$]+\\d+)(?::\\.([a-zA-Z]+\\d+)){0,1}\\]");
    boost::wsmatch result;
    bool b = boost::regex_search(expr, result, re);

    size_t sz = result.size();
    if (sz == 4 && !result[1].matched)
    {
        table = L"";
        ref =  result[2].str();        
        return true;
    }
    else if (sz == 4 && result[1].matched)
    {
        table = result[1].str();
        boost::algorithm::replace_all(table, L"$", L"");
        boost::algorithm::replace_all(table, L"'", L"");
        ref =  result[2].str();                
        return true;
    }
    return false;
}

namespace 
{

std::wstring replace_cell_range_formater(boost::wsmatch const & what)
{
    const size_t sz = what.size();
    if (sz == 4 && !what[1].matched)
    {
        const std::wstring c1 = what[2].str(); 
        const std::wstring c2 = what[3].str(); 
        const std::wstring s =  c1 + (c2.empty() ? L"" : (L":" + c2) );
        return s;
    }
    else if (sz == 4 && what[1].matched)
    {
        std::wstring sheet1 = what[1].str();
        boost::algorithm::replace_all(sheet1, L"$", L"");

        const std::wstring c1 = what[2].str(); 
        const std::wstring c2 = what[3].str(); 
        const std::wstring s =  sheet1 + L"!" + c1 + (c2.empty() ? L"" : (L":" + c2) );
        return s;
    }
    return L"";
}

}


// замен€ем формат адресации €чеек
// [.A1] -> A1
// [.A1:.B5] -> A1:B5
// [Sheet2.A1:B5] -> Sheet2!A1:B5
// [Sheet2.A1] -> Sheet2!A1
// [$'Sheet2 A'.$B2] -> 'Sheet2 A'!$B2
void odf2oox_converter::Impl::replace_cells_range(std::wstring& expr)
{
    //boost::wregex simpleRef(L"\\[\\.([a-zA-Z]+\\d+)(?::\\.([a-zA-Z]+\\d+)){0,1}\\]");
    boost::wregex complexRef(L"\\[(?:\\$)?([^\\.]+?){0,1}\\.([a-zA-Z\\$]+\\d+)(?::\\.([a-zA-Z]+\\d+)){0,1}\\]");
    /*
                                 [     $   Sheet2          . A1                  :  . B5                    ]

    */

    const std::wstring res = boost::regex_replace(
        expr,
        complexRef,
        &replace_cell_range_formater,
        boost::match_default | boost::format_all);
    expr = res;
}

namespace {

std::wstring replace_named_ref_formater(boost::wsmatch const & what)
{
    const size_t sz = what.size();

    if (sz == 4 && !what[1].matched)
    {
        const std::wstring c1 = what[2].str(); 
        const std::wstring c2 = what[3].str(); 
        const std::wstring s =  c1 + (c2.empty() ? L"" : (L":" + c2) );
        return s;
    }
    else if (sz == 4 && what[1].matched)
    {
        std::wstring sheet1 = what[1].str();
        boost::algorithm::replace_all(sheet1, L"$", L"");

        const std::wstring c1 = what[2].str(); 
        const std::wstring c2 = what[3].str(); 
        const std::wstring s =  sheet1 + L"!" + c1 + (c2.empty() ? L"" : (L":" + c2) );
        return s;
    }
    return L"";
}

}
void odf2oox_converter::Impl::replace_named_ref(std::wstring & expr)
{
    boost::wregex complexRef(L"\\$([^\\.]+?){0,1}\\.(\\$[a-zA-Z]+\\$\\d+)(?::\\.(\\$[a-zA-Z]+\\$\\d+)){0,1}");

    const std::wstring res = boost::regex_replace(
        expr,
        complexRef,
        &replace_named_ref_formater,
        boost::match_default | boost::format_all);
    expr = res;    
}

// распознаем и замен€ем формат формулы
// of:=(Formula) -> (Formula)
bool odf2oox_converter::Impl::check_formula(std::wstring& expr)
{
    boost::match_results<std::wstring::const_iterator> res;
    if (boost::regex_search(expr, res, boost::wregex(L"(?:[\\w]+:)?=(.+)"), boost::match_default))
    {
        expr = res[1].str();
        return true;
    }
    else   
        return false;
}

namespace 
{

std::wstring replace_semicolons_formater(boost::wsmatch const & what)
{
    if (what[1].matched)
        return L",";
    else if (what[2].matched)
        return what[2].str();    
    else if (what[3].matched)
        return what[3].str();
    //else if (what[4].matched)
    //    return what[4].str();
	else
		return L"";
}

}

// TODO
// заменить точки с зап€той во всех вхождени€х кроме наход€щихс€ в кавычках --*и в фигурных скобках*--
// TODO: проверить как сохран€ютс€ кавычки в строке
void odf2oox_converter::Impl::replace_semicolons(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        //boost::wregex(L"(;)|(?:\".*?\")|(?:'.*?')"),
        boost::wregex(L"(;)|(\".*?\")|('.*?')"),
        &replace_semicolons_formater,
        boost::match_default | boost::format_all);
     expr = res;
}

namespace 
{

std::wstring replace_vertical_formater(boost::wsmatch const & what)
{
    if (what[1].matched)
    {
        std::wstring inner = what[1].str();
        boost::algorithm::replace_all(inner, L"|", L";");
        return L"{" + inner + L"}";
    }    
    else if (what[2].matched)
        return what[2].str();
    else if (what[3].matched)
        return what[3].str();
}
std::wstring replace_space_formater(boost::wsmatch const & what)
{
    if (what[1].matched)
    {
        std::wstring inner = what[1].str();
        boost::algorithm::replace_all(inner, L" ", L",");
        return inner;
    }    
    else if (what[2].matched)
        return what[2].str();
    else if (what[3].matched)
        return what[3].str();
}
}

std::wstring forbidden_formulas[] =
{
    L"NULLFORMULA"
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

bool is_forbidden(const std::wstring & formula)
{
    BOOST_FOREACH(const std::wstring & s, forbidden_formulas)
    {
        if (boost::algorithm::contains(formula, s))
            return true;
    }
    return false;
}

// заменить вертикальную черту во всех вхождени€х в фигурных скобках, но не внутри строк
void odf2oox_converter::Impl::replace_vertical(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
        &replace_vertical_formater,
        boost::match_default | boost::format_all);
     expr = res;
}
// заменить пробел во всех вхождени€х на зап€тую
void odf2oox_converter::Impl::replace_space(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
        &replace_space_formater,
        boost::match_default | boost::format_all);
     expr = res;
}

std::wstring odf2oox_converter::Impl::convert(const std::wstring& expr)
{
    if (is_forbidden(expr))
        return L"NULLFORMULA()";

    std::wstring workstr = expr;
    check_formula(workstr);
    replace_cells_range(workstr);
    replace_semicolons(workstr);
    replace_vertical(workstr);
    return workstr;
}


//Sheet2.C3:Sheet2.C19 Sheet2.L29:Sheet2.L36
//в
//Sheet2!C3:C19,Sheet2!L27:L34
std::wstring odf2oox_converter::Impl::convert_chart_distance(const std::wstring& expr)
{
    if (is_forbidden(expr))
        return L"NULLFORMULA()";

    //std::wstring workstr = expr;
    //replace_space(workstr);
    //return workstr;

	//распарсить по диапазонам - одф-пробел, ик-эль-зап€та€

	std::vector<std::wstring> distance_inp;
	std::vector<std::wstring> distance_out;

	boost::algorithm::split(distance_inp,expr, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

	BOOST_FOREACH(std::wstring &d,distance_inp)
	{
		std::wstring sheet;
		std::vector<std::wstring> range;
		std::vector<std::wstring> cells;

		boost::algorithm::split(range,d, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

		BOOST_FOREACH(std::wstring &c,range)
		{
			const ::std::string::size_type colon = c.find('.');
			cells.push_back(c.substr(colon+1));
			if (sheet.size()<1)
				sheet=c.substr(0, colon);
		}
		std::wstring cells_out;
		BOOST_FOREACH(std::wstring &c,cells)
		{
			cells_out.append(c);
			cells_out.append(L":");
		}
		int res1 = sheet.find(L"-");
		int res2 = sheet.find(L"'");
		
		if (res1>=0 && !(res2==0))
		{
			sheet = L"'" + sheet + L"'";
		}

		distance_out.push_back(sheet+L"!"+cells_out.substr(0, cells_out.size()-1));
	}
	std::wstring result;

	BOOST_FOREACH(std::wstring &d,distance_out)
	{
		result.append(d);
		result.append(L",");
	}
	return result.substr(0, result.size()-1);
}
odf2oox_converter::odf2oox_converter(): impl_(new odf2oox_converter::Impl)
{
}

odf2oox_converter::~odf2oox_converter()
{
}

std::wstring odf2oox_converter::convert(const std::wstring& expr)
{
    return impl_->convert(expr);
}
std::wstring odf2oox_converter::convert_chart_distance(const std::wstring& expr)
{
    return impl_->convert_chart_distance(expr);
}
std::wstring odf2oox_converter::convert_named_ref(const std::wstring& expr)
{
    std::wstring workstr = expr;
    impl_->replace_named_ref(workstr);
    return workstr;
}


std::wstring odf2oox_converter::convert_ref(std::wstring const & expr)
{
    std::wstring workstr = L"[" + expr + L"]";
    impl_->replace_cells_range(workstr);
    return workstr;
}
std::wstring odf2oox_converter::convert_spacechar(std::wstring expr)
{
	while(true)
	{
		int pos = expr.find(L"%20");
		if (pos <0)break;

		expr.replace(pos,3,L" ");
	}
	return expr; 
}

bool odf2oox_converter::find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref)
{
    return impl_->find_first_ref(expr, table, ref);
}
bool odf2oox_converter::find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last)
{
    return impl_->find_first_last_ref(expr,  table, ref_first,ref_last);
}

}
}

#include <cpdoccore/formulasconvert.h>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
namespace cpdoccore {
namespace formulasconvert {

class oox2odf_converter::Impl
{
public:
    std::wstring convert(const std::wstring& expr);
	std::wstring convert_formula(const std::wstring& expr);

	std::wstring convert_chart_distance(const std::wstring& expr);
    static void replace_cells_range(std::wstring& expr);
  
	static void replace_semicolons(std::wstring& expr);
    static void replace_vertical(std::wstring& expr);
    static void replace_space(std::wstring& expr);

	static std::wstring replace_cells_range_formater1(boost::wsmatch const & what);
	static std::wstring replace_cells_range_formater2(boost::wsmatch const & what);
	static std::wstring replace_arguments(boost::wsmatch const & what);
	std::wstring  replace_arguments1(std::wstring & workstr);

    void replace_named_ref(std::wstring & expr);
 //   bool find_first_ref(std::wstring const & expr, std::wstring & table,  std::wstring & ref);
	//bool find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last);
};
////Table!.$A$1:$A2 -> ref $A$1 -> ref $A$2
//bool oox2odf_converter::Impl::find_first_last_ref(std::wstring const & expr, std::wstring & table,std::wstring & ref_first,std::wstring & ref_last)
//{	
//	std::vector< std::wstring > splitted;
//    
//	boost::algorithm::split(splitted, expr, boost::algorithm::is_any_of(L".:"), boost::algorithm::token_compress_on);
//    
//	if (splitted.size()>2)
//	{
//		table	= splitted[0];	
//		ref_first = splitted[1];
//		ref_last = splitted[2];
//		return true;
//	}
//	return false;
//}
//   // =[.A1]+[.B1] -> table = ""; ref = "A1"
//    // of:=['Sheet2 A'.B2] -> table= "Sheet2 A"; ref = "B2"
//
//bool oox2odf_converter::Impl::find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref)
//{
//    boost::wregex re(L"\\[(?:\\$)?([^\\.]+?){0,1}\\.([a-zA-Z\\$]+\\d+)(?::\\.([a-zA-Z]+\\d+)){0,1}\\]");
//    boost::wsmatch result;
//    bool b = boost::regex_search(expr, result, re);
//
//    size_t sz = result.size();
//    if (sz == 4 && !result[1].matched)
//    {
//        table = L"";
//        ref =  result[2].str();        
//        return true;
//    }
//    else if (sz == 4 && result[1].matched)
//    {
//        table = result[1].str();
//        boost::algorithm::replace_all(table, L"$", L"");
//        boost::algorithm::replace_all(table, L"'", L"");
//        ref =  result[2].str();                
//        return true;
//    }
//    return false;
//}
//
//namespace 
//{
//
//std::wstring replace_cell_range_formater(boost::wsmatch const & what)
//{
//    const size_t sz = what.size();
//    if (sz == 4 && !what[1].matched)
//    {
//        const std::wstring c1 = what[2].str(); 
//        const std::wstring c2 = what[3].str(); 
//        const std::wstring s =  c1 + (c2.empty() ? L"" : (L":" + c2) );
//        return s;
//    }
//    else if (sz == 4 && what[1].matched)
//    {
//        std::wstring sheet1 = what[1].str();
//        boost::algorithm::replace_all(sheet1, L"$", L"");
//
//        const std::wstring c1 = what[2].str(); 
//        const std::wstring c2 = what[3].str(); 
//        const std::wstring s =  sheet1 + L"!" + c1 + (c2.empty() ? L"" : (L":" + c2) );
//        return s;
//    }
//    return L"";
//}
//
//}
//

// замен€ем формат адресации €чеек ЌјќЅќ–ќ“
// [.A1] -> A1
// [.A1:.B5] -> A1:B5
// [Sheet2.A1:B5] -> Sheet2!A1:B5
// [Sheet2.A1] -> Sheet2!A1
// [$'Sheet2 A'.$B2] -> 'Sheet2 A'!$B2
void oox2odf_converter::Impl::replace_cells_range(std::wstring& expr)
{
	boost::wregex re1(L"(\\$?\\w+)?\\!?([a-zA-Z$]+\\d{1,2})\\:?([a-zA-Z$]+\\d{1,2})?");
	boost::wregex re2(L"([a-zA-Z$]+\\d{1,2})\\:?([a-zA-Z$]+\\d{1,2})?");
//                          $   Sheet2   ! $ A1                  :  $ B5          
	//boost::wsmatch result;
	//bool b = boost::regex_search(expr, result, re);
    std::wstring workstr = expr;

     std::wstring res = boost::regex_replace(
        workstr,	
		re1,
        &replace_cells_range_formater1,
        boost::match_default | boost::format_all);
     
	 //workstr = res;
	 //res = boost::regex_replace(
		//workstr,	
		//re2,
		//&replace_cells_range_formater2,
		//boost::match_default | boost::format_all);
	 expr = res;

	return;
}
std::wstring oox2odf_converter::Impl::replace_cells_range_formater1(boost::wsmatch const & what)
{
    const size_t sz = what.size();
	
	if (sz>3)
    {
		std::wstring sheet1 = what[1].str();
        boost::algorithm::replace_all(sheet1, L"$", L"");

        std::wstring c1 = what[2].str(); 
        std::wstring c2 = what[3].str(); 
        const std::wstring s =  std::wstring(L"[") + sheet1 + L"." + c1 + (c2.empty() ? L"" : (L":" + sheet1  + L"." + c2) ) + std::wstring(L"]");
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
	
	if (sz>2)
	{
        const std::wstring c1 = what[1].str(); 
        const std::wstring c2 = what[2].str(); 

		const std::wstring s =  std::wstring(L"[.") +  c1 + (c2.empty() ? L"" : (L":." + c2) ) + std::wstring(L"]");

		return s; 
	}

    return L"";

}

//namespace {
//
//std::wstring replace_named_ref_formater(boost::wsmatch const & what)
//{
//    const size_t sz = what.size();
//
//    if (sz == 4 && !what[1].matched)
//    {
//        const std::wstring c1 = what[2].str(); 
//        const std::wstring c2 = what[3].str(); 
//        const std::wstring s =  c1 + (c2.empty() ? L"" : (L":" + c2) );
//        return s;
//    }
//    else if (sz == 4 && what[1].matched)
//    {
//        std::wstring sheet1 = what[1].str();
//        boost::algorithm::replace_all(sheet1, L"$", L"");
//
//        const std::wstring c1 = what[2].str(); 
//        const std::wstring c2 = what[3].str(); 
//        const std::wstring s =  sheet1 + L"!" + c1 + (c2.empty() ? L"" : (L":" + c2) );
//        return s;
//    }
//    return L"";
//}
//
//}

  // Ћист1!$A$1 -> $Ћист1.$A$1 

void oox2odf_converter::Impl::replace_named_ref(std::wstring & expr)
{
	std::vector< std::wstring > splitted;
    
	boost::algorithm::split(splitted, expr, boost::algorithm::is_any_of(L"."), boost::algorithm::token_compress_on);

	std::wstring table;
	std::wstring ref;
	if (splitted.size()>1)
	{
		table= splitted[0];
		ref = splitted[1];
		boost::algorithm::replace_all(table, L"$", L"");	
		expr = table +std::wstring(L"!") + ref;
	}
	else if (splitted.size()==1)
	{
		ref = splitted[0];
		expr = ref;
	}

  
}



namespace 
{

std::wstring replace_semicolons_formater(boost::wsmatch const & what)
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

}

// TODO
// заменить зап€тые на точки с зап€той во всех вхождени€х кроме наход€щихс€ в кавычках --*и в фигурных скобках*--
// TODO: проверить как сохран€ютс€ кавычки в строке
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

namespace 
{

std::wstring replace_vertical_formater(boost::wsmatch const & what)
{
    if (what[1].matched)
    {
        std::wstring inner = what[1].str();
        boost::algorithm::replace_all(inner, L";", L"|");
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
        boost::algorithm::replace_all(inner, L",", L" ");
        return inner;
    }    
    else if (what[2].matched)
        return what[2].str();
    else if (what[3].matched)
        return what[3].str();
}

}

std::wstring forbidden_formulas1[] =
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

bool is_forbidden1(const std::wstring & formula)
{
    BOOST_FOREACH(const std::wstring & s, forbidden_formulas1)
    {
        if (boost::algorithm::contains(formula, s))
            return true;
    }
    return false;
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
// заменить вертикальную черту во всех вхождени€х в фигурных скобках, но не внутри строк
void oox2odf_converter::Impl::replace_vertical(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
        &replace_vertical_formater,
        boost::match_default | boost::format_all);
     expr = res;
}
// заменить зап€тую во всех вхождени€х на пробел 
void oox2odf_converter::Impl::replace_space(std::wstring& expr)
{
     const std::wstring res = boost::regex_replace(
        expr,
        boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
        &replace_space_formater,
        boost::match_default | boost::format_all);
     expr = res;
}

std::wstring replace_(boost::wsmatch const & what)
{
    if (what[1].matched)
    {
        std::wstring inner = what[1].str();
        boost::algorithm::replace_all(inner, L",", L" ");
        return inner;
    }    
    else if (what[2].matched)
        return what[2].str();
    else if (what[3].matched)
        return what[3].str();
}
std::wstring  oox2odf_converter::Impl::replace_arguments1(std::wstring & workstr1)
{
	std::wstring out;

	std::wstring workstr = workstr1;
		
	replace_vertical(workstr);
	replace_semicolons(workstr);		
	
	std::vector<std::wstring> distance;

	boost::algorithm::split(distance,workstr, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

	BOOST_FOREACH(std::wstring &d, distance)
	{		
	
		replace_cells_range(d);

		out = out + d + std::wstring(L";");
	}

	if (out.length()>0) out = out.substr(0,out.length()-1);
	        

	return out ;
}
std::wstring  oox2odf_converter::Impl::replace_arguments(boost::wsmatch const & what)
{
	std::wstring out;

	int sz = what.size();

	std::wstring c1= what[1].str();
	std::wstring c2= what[2].str();
	std::wstring c3= what[3].str();

    if (what[1].matched)
    {
        std::wstring workstr = what[1].str();
		replace_vertical(workstr);
		replace_semicolons(workstr);		
		
		std::vector<std::wstring> distance;

		boost::algorithm::split(distance,workstr, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

		BOOST_FOREACH(std::wstring &d, distance)
		{		
		
			replace_cells_range(d);

			out = out + d + std::wstring(L";");
		}

		if (out.length()>0) out = out.substr(0,out.length()-1);
	
    }    
    else if (what[2].matched)
        out = what[2].str();
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
// of:=(Formula) -> (Formula)
std::wstring oox2odf_converter::Impl::convert_formula(const std::wstring& expr)
{
    if (is_forbidden1(expr))
        return L"NULLFORMULA";

    std::wstring workstr = expr;

     const std::wstring res = boost::regex_replace(
        workstr,
		boost::wregex(L"(?:(?=[()])(.*?)(?=[)]))|(\".*?\")|('.*?')"),
		&oox2odf_converter::Impl::replace_arguments,
        boost::match_default | boost::format_all);

    return std::wstring(L"of:=") + res;
}
//Sheet2!C3:C19,Sheet2!L27:L34
//в
//Sheet2.C3:Sheet2.C19 Sheet2.L29:Sheet2.L36
//todooo
std::wstring oox2odf_converter::Impl::convert_chart_distance(const std::wstring& expr1)
{
	std::wstring expr = expr1;
	int res = expr.find(L"(");
	if (res >=0) expr = expr.substr(res + 1, expr.size()-1);

	res= expr.rfind(L")");
	if (res >=0) expr = expr.substr(0, res);

	//распарсить по диапазонам - одф-пробел, ик-эль-зап€та€

	std::vector<std::wstring> distance_inp;
	std::vector<std::wstring> distance_out;

	boost::algorithm::split(distance_inp,expr, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

	BOOST_FOREACH(std::wstring &d,distance_inp)
	{
		std::wstring sheet;
		std::vector<std::wstring> range;
		std::vector<std::wstring> cells;

		boost::algorithm::split(range,d, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

		BOOST_FOREACH(std::wstring &c,range)
		{
			const ::std::string::size_type colon = c.find('!');
			cells.push_back(c.substr(colon+1));
			if (sheet.size()<1)
				sheet=c.substr(0, colon);
		}
		std::wstring cells_out;
		BOOST_FOREACH(std::wstring &c,cells)
		{
			cells_out.append(sheet+L".");
			cells_out.append(c);
			cells_out.append(L":");
		}
		int res1 = sheet.find(L"-");
		int res2 = sheet.find(L"'");
		
		if (res1>=0 && !(res2==0))
		{
			sheet = L"'" + sheet + L"'";
		}

		distance_out.push_back(cells_out.substr(0, cells_out.size()-1));
	}
	std::wstring result;

	BOOST_FOREACH(std::wstring &d,distance_out)
	{
		result.append(d);
		result.append(L" ");
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
std::wstring oox2odf_converter::convert_formula(const std::wstring& expr)
{
    return impl_->convert_formula(expr);
}
std::wstring oox2odf_converter::convert_chart_distance(const std::wstring& expr)
{
    return impl_->convert_chart_distance(expr);
}
std::wstring oox2odf_converter::convert_named_ref(const std::wstring& expr)
{
    std::wstring workstr = expr;
    impl_->replace_named_ref(workstr);
    return workstr;
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
size_t getColAddressInv(const std::wstring & a_)
{
    std::wstring a = a_;
    ::boost::algorithm::to_upper(a);
    static const size_t r = (L'Z' - L'A' + 1);
    size_t mul = 1;
    bool f = true;
    size_t res = 0;
    BOOST_REVERSE_FOREACH(const wchar_t c, a)
    {
        size_t v = c - L'A';
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
    ::boost::algorithm::replace_all(a, L"$", L"");
    //::boost::algorithm::replace_all(a, L"'", L"");
	::boost::algorithm::to_upper(a);
	

	BOOST_FOREACH(wchar_t c, a)
    {
		if (c >= L'0' && c <= L'9')
			row +=c;
		else
			col += c;
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
}

int oox2odf_converter::get_count_value_points(std::wstring expr)
{
	int count =0;
	std::vector< std::wstring > splitted;
    
	boost::algorithm::replace_all(expr, L"(", L"");
	boost::algorithm::replace_all(expr, L")", L"");
	boost::algorithm::split(splitted, expr, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

	for (long i=0; i < splitted.size(); i++)
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

			count += std::max(col_2-col_1+1, row_2-row_1+1);

		}
		else count ++; 
	}
	return count;
}

//bool oox2odf_converter::find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref)
//{
//    return impl_->find_first_ref(expr, table, ref);
//}
//bool oox2odf_converter::find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last)
//{
//    return impl_->find_first_last_ref(expr,  table, ref_first,ref_last);
//}

}
}

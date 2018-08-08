/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include"../../Common/DocxFormat/Source/XML/Utils.h"

namespace cpdoccore {
namespace formulasconvert {

	class odf2oox_converter::Impl
	{
	public:

		std::wstring convert(const std::wstring& expr);
		std::wstring convert_chart_distance(const std::wstring& expr);

		void split_distance_by(const std::wstring& expr, const std::wstring& by, std::vector<std::wstring>& out);
		
		void replace_cells_range(std::wstring& expr, bool withTableName);
		bool check_formula(std::wstring& expr);
		void replace_semicolons(std::wstring& expr);
		void replace_tilda(std::wstring& expr);
		void replace_vertical(std::wstring& expr);
		void replace_space(std::wstring& expr);

		static std::wstring replace_named_ref_formater(boost::wsmatch const & what);
		static std::wstring replace_named_ref_formater1(boost::wsmatch const & what);
		static std::wstring replace_cell_range_formater(boost::wsmatch const & what);
		
		void replace_named_formula(std::wstring & expr, bool w = true);
		void replace_named_ref(std::wstring & expr, bool w = true);
		bool find_first_ref(std::wstring const & expr, std::wstring & table,  std::wstring & ref);
		bool find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last);
	
		static bool			convert_with_TableName;
		static std::wstring table_name_;
	};
	bool			odf2oox_converter::Impl::convert_with_TableName = true;
	std::wstring	odf2oox_converter::Impl::table_name_			= L"";

	bool odf2oox_converter::Impl::find_first_last_ref(std::wstring const & expr, std::wstring & table,std::wstring & ref_first,std::wstring & ref_last)
	{	
		std::vector< std::wstring > splitted;
	    
		boost::algorithm::split(splitted, expr, boost::algorithm::is_any_of(L".:"), boost::algorithm::token_compress_on);
	    
		if (splitted.size()==3)
		{
			table		= splitted[0];	
			ref_first	= splitted[1];
			ref_last	= splitted[2];
			return true;
		}
		if (splitted.size()==4)
		{
			table		= splitted[0];	
			ref_first	= splitted[1];
			ref_last	= splitted[3];
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
			XmlUtils::replace_all( table, L"$", L"");
			XmlUtils::replace_all( table, L"'", L"");
			ref =  result[2].str();                
			return true;
		}
		return false;
	}

	
	std::wstring odf2oox_converter::Impl::replace_cell_range_formater(boost::wsmatch const & what)
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
			XmlUtils::replace_all( sheet1, L"$", L"");

			const std::wstring c1 = what[2].str(); 
			std::wstring c2 = what[3].str(); 
			if (c2.empty()) c2 = what[4].str(); 
			const std::wstring s =  sheet1 + L"!" + c1 + (c2.empty() ? L"" : (L":" + c2) );
			return s;
		}
		return L"";
	}

	std::wstring odf2oox_converter::Impl::replace_named_ref_formater(boost::wsmatch const & what)
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
			XmlUtils::replace_all( sheet1, L"$", L"");

			table_name_ = sheet1;

			const std::wstring c1 = what[2].str(); 
			const std::wstring c2 = what[3].str(); 

			if (convert_with_TableName)
			{
				if (std::wstring::npos != sheet1.find(L" "))
				{
					if (sheet1[0] != L'\'')
					{
						sheet1 = L"'" + sheet1 + L"'";
					}
				}
				return (sheet1 + L"!") + c1 + (c2.empty() ? L"" : (L":" + c2) );
			}
			else
			{
				return c1 + (c2.empty() ? L"" : (L":" + c2) );
			}
		}
		else if (sz == 5 && what[1].matched)
		{
			std::wstring sheet1 = what[1].str();
			XmlUtils::replace_all( sheet1, L"$", L"");

			const std::wstring c1 = what[2].str(); 
			const std::wstring c2 = what[3].str(); //sheet name 2
			const std::wstring c3 = what[4].str(); 

			table_name_ = sheet1;

			if (convert_with_TableName)
			{
				return (sheet1 + L"!") + c1 + (c3.empty() ? L"" : (L":" + c3) );
			}
			else
			{
				return c1 + (c3.empty() ? L"" : (L":" + c3) );
			}
		}
		else if (sz == 5 && !what[1].matched)
		{
			const std::wstring c1 = what[2].str(); 
			const std::wstring c2 = what[3].str(); //sheet name 2
			const std::wstring c3 = what[4].str(); 

			return c1 + (c3.empty() ? L"" : (L":" + c3) );
		}
		return L"";
	}
	std::wstring odf2oox_converter::Impl::replace_named_ref_formater1(boost::wsmatch const & what)
	{
		boost::wregex complexRef(L"\\${0,1}([^\\.]+?){0,1}\\.(\\${0,1}[a-zA-Z]+\\${0,1}\\d+)(?::\\.(\\${0,1}[a-zA-Z]+\\${0,1}\\d+)){0,1}");

		std::wstring expr = what[1].str();
		const std::wstring res = boost::regex_replace(
			expr,
			complexRef,
			&replace_named_ref_formater,
			boost::match_default | boost::format_all);
		expr = res; 

		return expr;
	}


	// заменяем формат адресации ячеек
	// [.A1] -> A1
	// [.A1:.B5] -> A1:B5
	// [Sheet2.A1:B5] -> Sheet2!A1:B5
	// [Sheet2.A1] -> Sheet2!A1
	// [$'Sheet2 A'.$B2] -> 'Sheet2 A'!$B2
	void odf2oox_converter::Impl::replace_cells_range(std::wstring& expr, bool withTableName)
	{
		convert_with_TableName = withTableName;

		boost::wregex complexRef(L"\\[(?:\$)?([^\\.]+?){0,1}\\.(\\${0,1}[a-zA-Z]*\\${0,1}\\d*)(?::(\\${0,1}[^\\.]+?){0,1}\\.(\\${0,1}[a-zA-Z]*\\${0,1}\\d*)){0,1}\\]");
		/*
									 [     $   Sheet2          . A1								 : ( $   Sheet2)? . B5                    ]
		*/

		const std::wstring res = boost::regex_replace(
			expr,
			complexRef,
			&replace_named_ref_formater,
			boost::match_default | boost::format_all);
		expr = res;
	}
	void odf2oox_converter::Impl::replace_named_ref(std::wstring & expr, bool withTableName)
	{
		convert_with_TableName = withTableName;
		
		//boost::wregex complexRef(L"\\${0,1}([^\\.]+?){0,1}\\.(\\${0,1}[a-zA-Z]+\\${0,1}\\d+)(?::\\.(\\${0,1}[a-zA-Z]+\\${0,1}\\d+)){0,1}");
		boost::wregex complexRef(L"\\${0,1}([^\\.\\s]+?){0,1}\\.(\\${0,1}[a-zA-Z]*\\${0,1}\\d*)(?::\\${0,1}([^\\.\\s]+?){0,1}\\.(\\${0,1}[a-zA-Z]*\\${0,1}\\d*)){0,1}");
	
		const std::wstring res = boost::regex_replace(
			expr,
			complexRef,
			&replace_named_ref_formater,
			boost::match_default | boost::format_all);
		expr = res;    
	}

	// распознаем и заменяем формат формулы
	// of:=(Formula) -> (Formula)
	bool odf2oox_converter::Impl::check_formula(std::wstring& expr)
	{
		boost::match_results<std::wstring::const_iterator> res;
		if (boost::regex_search(expr, res, boost::wregex(L"^(?:[\\w]+:)?=(.+)"), boost::match_default))
		{
			expr = res[1].str();
			while (expr.find(L"=") == 0)
			{
				expr.erase(expr.begin(), expr.begin() + 1);
			}
			return true;
		}
		else   
			return false;
	}


	std::wstring replace_semicolons_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
			return L",";
		else if (what[2].matched)
			return what[2].str();    
		else if (what[3].matched)
			return what[3].str();
		else
			return L"";
	}
	std::wstring replace_tilda_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
			return L";";
		else if (what[2].matched)
			return what[2].str();    
		else if (what[3].matched)
			return what[3].str();
		else
			return L"";
	}
	// TODO
	// заменить точки с запятой во всех вхождениях кроме находящихся в кавычках --*и в фигурных скобках*--
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
	void odf2oox_converter::Impl::replace_tilda(std::wstring& expr)
	{
		 const std::wstring res = boost::regex_replace(
			expr,
			//boost::wregex(L"(;)|(?:\".*?\")|(?:'.*?')"),
			boost::wregex(L"(~)|(\".*?\")|('.*?')"),
			&replace_semicolons_formater,
			boost::match_default | boost::format_all);

		 expr = res;
	}
	std::wstring replace_vertical_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
		{
			std::wstring inner = what[1].str();
			XmlUtils::replace_all( inner, L"|", L";");
			return L"{" + inner + L"}";
		}    
		else if (what[2].matched)
			return what[2].str();
		else if (what[3].matched)
			return what[3].str();
	
		return L"";
	}

	std::wstring replace_point_space(boost::wsmatch const & what)
	{
		if (what[1].matched)
		{
			std::wstring inner = what[1].str();
			XmlUtils::replace_all( inner, L" ", L"PROBEL");
			return inner;
		}    
		else if (what[2].matched)
			return what[2].str();
		else if (what[3].matched)
			return what[3].str();
	
		return L"";
	}
	std::wstring convert_scobci(boost::wsmatch const & what)
	{
		if (what[1].matched)
		{
			std::wstring inner = what[1].str();
			XmlUtils::replace_all( inner, L".", L"ТОСHKA");
			XmlUtils::replace_all( inner, L"(", L"SCOBCAIN");
			XmlUtils::replace_all( inner, L")", L"SCOBCAOUT");

			XmlUtils::replace_all( inner, L"[", L"KVADRATIN");
			XmlUtils::replace_all( inner, L"]", L"KVADRATOUT");
			
			XmlUtils::replace_all( inner, L" ", L"PROBEL");
			XmlUtils::replace_all( inner, L"'", L"APOSTROF");
			return inner;
		}
		else if (what[2].matched)
		{
			std::wstring inner = what[2].str();
			XmlUtils::replace_all( inner, L".", L"ТОСHKA");

			XmlUtils::replace_all( inner, L"(", L"SCOBCAIN");
			XmlUtils::replace_all( inner, L")", L"SCOBCAOUT");

 			XmlUtils::replace_all( inner, L"[", L"KVADRATIN");
			XmlUtils::replace_all( inner, L"]", L"KVADRATOUT");
			
			XmlUtils::replace_all( inner, L" ", L"PROBEL");
			XmlUtils::replace_all( inner, L"\"", L"KAVYCHKA");
			return inner;
		}
		else if (what[3].matched)
			return what[3].str();
	
		return L"";
	}
	std::wstring replace_space_formater(boost::wsmatch const & what)
	{
		if (what[1].matched)
		{
			std::wstring inner = what[1].str();
			XmlUtils::replace_all( inner, L" ", L",");
			return inner;
		}    
		else if (what[2].matched)
			return what[2].str();
		else if (what[3].matched)
			return what[3].str();

		return L"";
	}


	//std::wstring forbidden_formulas[] =
	//{
	//	L"NULLFORMULA"	
	//	//L"BETADIST",
	//	//L"CEILING",
	//	//L"FLOOR",
	//	//L"RANK",
	//	//L"ROUND",
	//	//L"ROUNDDOWN",
	//	//L"ROUNDUP",
	//	//L"SUBTOTAL",
	//	//L"FORMULA",
	//	//L"ISREF"
	//};

	std::wstring is_forbidden(const std::wstring & formula)
	{
		std::wstring result = formula;
        std::map<std::wstring, std::wstring> forbidden_formulas;

		forbidden_formulas.insert(std::make_pair(L"FORMULA", L"_xlfn.FORMULATEXT"));
		
        for (std::map<std::wstring, std::wstring>::iterator it = forbidden_formulas.begin(); it != forbidden_formulas.end(); ++it)
		{
			if (boost::algorithm::contains(formula, it->first))
			{

				XmlUtils::replace_all(result, it->first, it->second);
			}
		}
		return result;
	}

	// заменить вертикальную черту во всех вхождениях в фигурных скобках, но не внутри строк
	void odf2oox_converter::Impl::replace_vertical(std::wstring& expr)
	{
		 const std::wstring res = boost::regex_replace(
			expr,
			boost::wregex(L"(?:(?:\\{)([^\\}]*?)(?:\\}))|(\".*?\")|('.*?')"),
			&replace_vertical_formater,
			boost::match_default | boost::format_all);
		 expr = res;
	}
	// заменить пробел во всех вхождениях на запятую
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
		std::wstring workstr = is_forbidden(expr);
		//boost::wregex complexRef(L"('(?!\\s\\'){0,1}.*?')");// Better_Donut.ods- cell(c27)
		//std::wstring workstr = boost::regex_replace(
		//	expr,
		//	complexRef,
		//	&replace_point_space,
		//	boost::match_default | boost::format_all);	

		bool isFormula = check_formula(workstr);

		boost::regex_replace(
			workstr,
			boost::wregex(L"('.*?')|(\".*?\")"),
			&convert_scobci, boost::match_default | boost::format_all);

		replace_cells_range	(workstr, true);
		replace_semicolons	(workstr);
		replace_tilda		(workstr);
		replace_vertical	(workstr);
		
		if (isFormula)
		{
			XmlUtils::replace_all( workstr, L"FDIST(", L"_xlfn.F.DIST(");
			// ROUNDUP( - тут в oox 2 параметра - разрядность нужно - ,0) - EV Requirements v2.2.3.ods
			
			int res_find=0;
			if ((res_find = workstr.find(L"CONCATINATE")) > 0)
			{
				//могут быть частично заданы диапазоны
				//todooo
			}	
			//todooo INDEX((A1:C6~A8:C11),2,2,2) - ???? - INDEX_emb.ods
		}
		else
		{
			size_t sz_workstr = workstr.length();
            if (workstr.substr(0, (std::min)((size_t)3, sz_workstr)) == L"of:")//sample_02neu_crashes.ods
				workstr =  workstr.substr(3);
		}


	//-----------------------------------------------------------
 		XmlUtils::replace_all( workstr, L"PROBEL"	, L" ");
		XmlUtils::replace_all( workstr, L"APOSTROF"	, L"'");
		XmlUtils::replace_all( workstr, L"ТОСHKA"	, L".");

		XmlUtils::replace_all( workstr, L"SCOBCAIN"	, L"(");
		XmlUtils::replace_all( workstr, L"SCOBCAOUT"	, L")");

		XmlUtils::replace_all( workstr, L"KVADRATIN"	, L"[");
		XmlUtils::replace_all( workstr, L"KVADRATOUT", L"]");

		XmlUtils::replace_all( workstr, L"PROBEL"	, L" ");
		XmlUtils::replace_all( workstr, L"KAVYCHKA"	, L"\"");

		return workstr;
	}

	void odf2oox_converter::Impl::split_distance_by(const std::wstring& expr, const std::wstring& by, std::vector<std::wstring>& out)
	{
		std::wstring workstr = expr;
		boost::wregex complexRef(L"('(?!\\s\\'){0,1}.*?')");// поиск того что в апострофах и замена там

		workstr = boost::regex_replace(
			expr,
			complexRef,
			&replace_point_space,
			boost::match_default | boost::format_all);
	    
		boost::algorithm::split(out, workstr, boost::algorithm::is_any_of(by), boost::algorithm::token_compress_on);
		
		for (size_t i = 0; i < out.size(); i++)
		{
 			XmlUtils::replace_all( out[i], L"PROBEL", L" ");
			XmlUtils::replace_all( out[i], L"TOCHKA", L".");
		}
	}

	//Sheet2.C3:Sheet2.C19 Sheet2.L29:Sheet2.L36
	//в
	//Sheet2!C3:C19,Sheet2!L27:L34

	std::wstring odf2oox_converter::Impl::convert_chart_distance(const std::wstring& expr)
	{
		std::wstring workstr = is_forbidden(expr);

		boost::wregex complexRef(L"('(?!\\s\\'){0,1}.*?')");// поиск того что в апострофах и замена там

		workstr = boost::regex_replace(
			expr,
			complexRef,
			&replace_point_space,
			boost::match_default | boost::format_all);
	    
		//распарсить по диапазонам - одф-пробел, ик-эль-запятая

		std::vector<std::wstring> distance_inp;
		std::vector<std::wstring> distance_out;

		boost::algorithm::split(distance_inp, workstr, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

		for (size_t i = 0; i < distance_inp.size(); i++)
		{
			std::wstring sheet;
			std::vector<std::wstring> range;
			std::vector<std::wstring> cells;

			boost::algorithm::split(range, distance_inp[i], boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

			for (size_t j = 0; j < range.size(); j++)
			{
				const std::string::size_type colon = range[j].find('.');
				cells.push_back(range[j].substr(colon + 1));
				if (sheet.size() < 1)
				{
					sheet = range[j].substr(0, colon);
				}
			}
			std::wstring cells_out;
			for (size_t j = 0; j < cells.size(); j++)
			{
				cells_out.append(cells[j]);
				cells_out.append(L":");
			}
			int res1 = sheet.find(L"-");
			int res2 = sheet.find(L"'");
			
			if (res1 >= 0 && !(res2 == 0))
			{
				sheet = L"'" + sheet + L"'";
			}

			distance_out.push_back(sheet + L"!" + cells_out.substr(0, cells_out.size()-1));
		}
		std::wstring result;

		for (size_t i = 0; i < distance_out.size(); i++)
		{
			result.append(distance_out[i]);
			result.append(L",");
		}
 		XmlUtils::replace_all( result, L"PROBEL" , L" ");
		XmlUtils::replace_all( result, L"TOCHKA", L".");
		
		return result.substr(0, result.size() - 1);// минус последняя лишняя запятая
	}
	odf2oox_converter::odf2oox_converter(): impl_(new odf2oox_converter::Impl)
	{
	}

	odf2oox_converter::~odf2oox_converter()
	{
	}

	std::wstring odf2oox_converter::get_table_name()
	{
		return impl_->table_name_;
	}

	std::wstring odf2oox_converter::convert(const std::wstring& expr)
	{
		return impl_->convert(expr);
	}
	std::wstring odf2oox_converter::convert_chart_distance(const std::wstring& expr)
	{
		return impl_->convert_chart_distance(expr);
	}
	void odf2oox_converter::split_distance_by(const std::wstring& expr, const std::wstring& by, std::vector<std::wstring>& out)
	{
		return impl_->split_distance_by(expr, by, out);
	}
	std::wstring odf2oox_converter::convert_named_ref(const std::wstring& expr, bool withTableName, std::wstring separator)
	{
		boost::wregex complexRef(L"('(?!\\s\\'){0,1}.*?')");// поиск того что в апострофах и замена там

		std::wstring workstr = boost::regex_replace(
			expr,
			complexRef,
			&replace_point_space,
			boost::match_default | boost::format_all);	
		
		XmlUtils::replace_all( workstr, L"'", L"APOSTROF");
		//XmlUtils::replace_all( workstr, L"", L"APOSTROF");

		impl_->replace_named_ref(workstr, withTableName);

		if (separator != L" ")
		{
			XmlUtils::replace_all( workstr, L" "	, separator);
		}

		XmlUtils::replace_all( workstr, L"PROBEL"	, L" ");
		XmlUtils::replace_all( workstr, L"APOSTROF"	, L"'");
		XmlUtils::replace_all( workstr, L"TOCHKA"	, L".");

		if (impl_->table_name_.empty() == false)
		{
			XmlUtils::replace_all( impl_->table_name_, L"PROBEL"	, L" ");
			XmlUtils::replace_all( impl_->table_name_, L"APOSTROF"	, L"'");
			XmlUtils::replace_all( impl_->table_name_, L"TOCHKA"	, L".");
		}
		return workstr;
	}
	std::wstring odf2oox_converter::convert_named_expr(const std::wstring& expr, bool withTableName)
	{
		std::wstring workstr = expr;

		bool isFormula = impl_->check_formula(workstr);

		if (isFormula)
		{
			workstr = impl_->convert(expr);
		}
		else
		{

			boost::wregex complexRef(L"('(?!\\s\\'){0,1}.*?')");// поиск того что в апострофах и замена там

			workstr = boost::regex_replace(
				workstr,
				complexRef,
				&replace_point_space,
				boost::match_default | boost::format_all);	

			XmlUtils::replace_all( workstr, L"'", L"APOSTROF");
		   
			impl_->replace_cells_range(workstr, withTableName);
			impl_->replace_semicolons(workstr);
			impl_->replace_vertical(workstr);

			int res_find=0;
			if ((res_find = workstr.find(L"CONCATINATE")) > 0)
			{
				//могут быть частично заданы диапазоны
				//todooo

			}
 			XmlUtils::replace_all( workstr, L"PROBEL"	, L" ");
			XmlUtils::replace_all( workstr, L"APOSTROF"	, L"'");
			XmlUtils::replace_all( workstr, L"TOCHKA"	, L".");

			if (impl_->table_name_.empty() == false)
			{
 				XmlUtils::replace_all( impl_->table_name_, L"PROBEL"	, L" ");
				XmlUtils::replace_all( impl_->table_name_, L"APOSTROF"	, L"'");
				XmlUtils::replace_all( impl_->table_name_, L"TOCHKA"	, L".");
			}
		}
		return workstr;
	}

	std::wstring odf2oox_converter::convert_ref(std::wstring const & expr)
	{
		std::wstring workstr = L"[" + expr + L"]";
		impl_->replace_cells_range(workstr, true);
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
		return impl_->find_first_last_ref(expr,  table, ref_first, ref_last);
	}

}
}

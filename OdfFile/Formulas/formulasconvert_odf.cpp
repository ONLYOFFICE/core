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
#include "formulasconvert.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include"../../OOXML/Base/Unit.h"
#include "../Reader/Converter/xlsxconversioncontext.h"
#include "../Reader/Converter/xlsx_utils.h"

namespace cpdoccore {
namespace formulasconvert {

	class odf2oox_converter::Impl
	{
	public:
		Impl() {}
		
		static std::unordered_map<std::wstring, int> & mapExternalLink_;

		std::wstring convert(const std::wstring& expr);
		std::wstring convert_chart_distance(const std::wstring& expr);

		void split_distance_by(const std::wstring& expr, const std::wstring& by, std::vector<std::wstring>& out);
		
		void replace_cells_range(std::wstring& expr, bool withTableName, bool bAbsoluteAlways = false);
		bool check_formula(std::wstring& expr);
		void replace_semicolons(std::wstring& expr);
		void replace_tilda(std::wstring& expr);
		void replace_vertical(std::wstring& expr);
		void replace_space(std::wstring& expr);
		void replace_apersand(std::wstring& expr);
	
		std::wstring convert_named_ref(const std::wstring& expr, bool withTableName, std::wstring separator, bool bAbsoluteAlways);
		std::wstring convert_named_expr(const std::wstring& expr, bool withTableName, bool bAbsoluteAlways);
		
		static std::wstring replace_named_ref_formater(boost::wsmatch const & what);
		static std::wstring replace_named_ref_formater1(boost::wsmatch const & what);
		//static std::wstring replace_cell_range_formater(boost::wsmatch const & what);
		
		void replace_named_formula(std::wstring & expr, bool w = true);
		void replace_named_ref(std::wstring & expr, bool withTable = true, bool bAbsoluteAlways = false);
		
		bool find_first_ref(std::wstring const & expr, std::wstring & table,  std::wstring & ref);
		bool find_first_last_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref_first,std::wstring & ref_last);
	
		static bool				convert_with_absolute;
		static bool				convert_with_TableName;
		static std::wstring		table_name_;

//-------------------------------------------------------------------------------------------------------------
		static std::wstring replace_apersand_formater(boost::wsmatch const & what)
		{
			if (what[1].matched)
				return L"&amp;";
			else if (what[2].matched)
				return what[2].str();
			else if (what[3].matched)
				return what[3].str();
			else
				return L"";
		}
		static std::wstring replace_semicolons_formater(boost::wsmatch const & what)
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
		static std::wstring replace_tilda_formater(boost::wsmatch const & what)
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
		static std::wstring replace_vertical_formater(boost::wsmatch const & what)
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

		static void replace_tmp_back(std::wstring &expr)
		{
			XmlUtils::replace_all( expr, L"ТОСHKA", L".");
			XmlUtils::replace_all( expr, L"VOSKL", L"!");

			XmlUtils::replace_all( expr, L"SCOBCAIN", L"(");
			XmlUtils::replace_all( expr, L"SCOBCAOUT", L")");

			//XmlUtils::replace_all( expr, L"KVADRATIN", L"[");
			//XmlUtils::replace_all( expr, L"KVADRATOUT", L"]");
			
			XmlUtils::replace_all( expr, L"PROBEL", L" ");
			//XmlUtils::replace_all( expr, L"APOSTROF", L"'");	
			//XmlUtils::replace_all( expr, L"KAVYCHKA", L"\"");
			XmlUtils::replace_all(expr, L"APERSAND", L"&");
		}
		static void replace_tmp(std::wstring &expr)
		{
		//	XmlUtils::replace_all( expr, L".", L"ТОСHKA");
		//	XmlUtils::replace_all( expr, L"!", L"VOSKL");

		//	XmlUtils::replace_all( expr, L"(", L"SCOBCAIN");
		//	XmlUtils::replace_all( expr, L")", L"SCOBCAOUT");

		//	//XmlUtils::replace_all( expr, L"[", L"KVADRATIN");
		//	//XmlUtils::replace_all( expr, L"]", L"KVADRATOUT");
		//	
		//	XmlUtils::replace_all( expr, L" ", L"PROBEL");
		////	XmlUtils::replace_all( expr, L"'", L"APOSTROF");	
		////	XmlUtils::replace_all( expr, L"\"", L"KAVYCHKA");

			std::wstring result;

			size_t pos = 0, size = expr.length();

			while(pos < size)
			{
				switch(expr[pos])
				{
					case '.':
					{	
						result += L"ТОСHKA";
					}break;
					case '!':
					{
						result += L"VOSKL";
					}break;
					case '(':
					{
						result += L"SCOBCAIN";
					}break;
					case ')':
					{
						result += L"SCOBCAOUT";
					}break;
					case ' ':
					{
						result += L"PROBEL";
					}break;
					case '&':
					{
						result += L"APERSAND";
					}break;
					default:
					{
						result += expr[pos];
					}break;
				}
				pos++;
			}
		expr = result;
		}
		static std::wstring convert_scobci(boost::wsmatch const & what)
		{
			if (what[1].matched)
			{
				std::wstring inner = what[1].str();

				replace_tmp(inner);

				return inner;
			}
			else if (what[2].matched)
			{
				std::wstring inner = what[2].str();
				
				replace_tmp(inner);

				return inner;
			}
			else if (what[3].matched)
				return what[3].str();
		
			return L"";
		}
		static std::wstring replace_space_formater(boost::wsmatch const & what)
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

		static std::wstring is_forbidden(const std::wstring & formula)
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


	};
	bool			odf2oox_converter::Impl::convert_with_TableName = true;
	std::wstring	odf2oox_converter::Impl::table_name_			= L"";
	bool			odf2oox_converter::Impl::convert_with_absolute	= false;
	
	std::unordered_map<std::wstring, int> &odf2oox_converter::Impl::mapExternalLink_ = oox::xlsx_conversion_context::mapExternalLink_;

	bool odf2oox_converter::Impl::find_first_last_ref(std::wstring const & expr, std::wstring & table,std::wstring & ref_first,std::wstring & ref_last)
	{	
		std::wstring workstr = expr;
		workstr = boost::regex_replace(
			workstr,
			boost::wregex(L"('.*?')|(\".*?\")"),
			&convert_scobci, boost::match_default | boost::format_all);

		std::vector< std::wstring > splitted;
	    
		boost::algorithm::split(splitted, workstr, boost::algorithm::is_any_of(L".:"), boost::algorithm::token_compress_on);
	  
		bool res = false;
		if (splitted.size() == 2)
		{
			table		= splitted[0];	
			ref_first	= splitted[1];
			ref_last	= splitted[1];
			res = true;
		}
		if (splitted.size() == 3)
		{
			table		= splitted[0];	
			ref_first	= splitted[1];
			ref_last	= splitted[2];
			res = true;
		}
		if (splitted.size() == 4)
		{
			table		= splitted[0];	
			ref_first	= splitted[1];
			ref_last	= splitted[3];
			res = true;
		}
		replace_tmp_back( table );

		return res;
	}

	bool odf2oox_converter::Impl::find_first_ref(std::wstring const & expr, std::wstring & table, std::wstring & ref)
	{
                boost::wregex re(L"\\[(?:\\$)?([^\\.]+?){0,1}\\.([\\w^0-9\\$]+\\d+)(?::\\.([\\w^0-9]+\\d+)){0,1}\\]");
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

	
	//std::wstring odf2oox_converter::Impl::replace_cell_range_formater(boost::wsmatch const & what)
	//{
	//	const size_t sz = what.size();
	//	if (sz == 4 && !what[1].matched)
	//	{
	//		const std::wstring c1 = what[2].str(); 
	//		const std::wstring c2 = what[3].str(); 
	//		const std::wstring s =  c1 + (c2.empty() ? L"" : (L":" + c2) );
	//		return s;
	//	}
	//	else if (sz == 4 && what[1].matched)
	//	{
	//		std::wstring sheet1 = what[1].str();
	//		XmlUtils::replace_all( sheet1, L"$", L"");

	//		const std::wstring c1 = what[2].str(); 
	//		std::wstring c2 = what[3].str(); 
	//		if (c2.empty()) c2 = what[4].str(); 
	//		const std::wstring s =  sheet1 + L"!" + c1 + (c2.empty() ? L"" : (L":" + c2) );
	//		return s;
	//	}
	//	return L"";
	//}

	std::wstring odf2oox_converter::Impl::replace_named_ref_formater(boost::wsmatch const & what)
	{
		const size_t sz = what.size();

		if (sz < 6) return what[0].str();

		std::wstring c0			= what[0].str(); 
		std::wstring external	= sz == 7 ? what[1].matched ? what[1].str() : what[2].str() : what[1].str();
		std::wstring sheet1		= sz == 7 ? what[3].str() : what[2].str();
		std::wstring ref1		= sz == 7 ? what[4].str() : what[3].str();
		std::wstring sheet2		= sz == 7 ? what[5].str() : what[4].str();
		std::wstring ref2		= sz == 7 ? what[6].str() : what[5].str();
		
		XmlUtils::replace_all( sheet1, L"$", L"");

		std::wstring result;

		if (false == external.empty())
		{
			replace_tmp_back(external);

			int id = -1;//add_external_link(external);
			std::unordered_map<std::wstring, int>::iterator pFind = mapExternalLink_.find(external);
			if ( pFind == mapExternalLink_.end())
			{
				id = (int)mapExternalLink_.size() + 1;
				mapExternalLink_.insert(std::make_pair(external, id));
			}
			else
			{
				id = pFind->second;
			}
			if (sheet1[0] == L'\'')
			{
				sheet1 = sheet1.substr(1, sheet1.length() - 2);
			}
			sheet1 = L"'[" + std::to_wstring(id) + L"]" + sheet1 + L"'";
		}
		else if (std::wstring::npos != sheet1.find(L" "))
		{
			if (sheet1[0] != L'\'')
			{
				sheet1 = L"'" + sheet1 + L"'";
			}
		}
		
		table_name_ = sheet1;

		if (convert_with_absolute)
		{
			size_t col = 0, row = 0;
			oox::getCellAddressInv(ref1, col, row);

			ref1 = oox::getCellAddress(col, row, true);

			if (false == ref2.empty())
			{
				oox::getCellAddressInv(ref2, col, row);
				ref2 = oox::getCellAddress(col, row, true);
			}
		}
		
		if (convert_with_TableName)
		{
			return (sheet1.empty() ? L"" : (sheet1 + L"!")) + ref1 + (ref2.empty() ? L"" : (L":" + ref2) );
		}
		else
		{
			return ref1 + (ref2.empty() ? L"" : (L":" + ref2) );
		}
	}
	std::wstring odf2oox_converter::Impl::replace_named_ref_formater1(boost::wsmatch const & what)
	{
		boost::wregex complexRef(L"(?:\'([^\']*)\'#){0,1}\\${0,1}([^\\.]+?){0,1}\\.(\\${0,1}[\\w^0-9]+\\${0,1}\\d+)(?::\\.(\\${0,1}[\\w^0-9]+\\${0,1}\\d+)){0,1}");
//									  'external'#  $   Sheet2         . A1								 : ( $   Sheet2)? . B5   
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
	void odf2oox_converter::Impl::replace_cells_range(std::wstring& expr, bool withTableName, bool bAbsoluteAlways)
	{
		XmlUtils::replace_all( expr, L"#REF !", L"#REF!");
		XmlUtils::replace_all( expr, L"#REF!#REF!", L"#REF!");
		XmlUtils::replace_all( expr, L"$#REF!$#REF!", L"#REF!");

		convert_with_TableName = withTableName;
		convert_with_absolute = bAbsoluteAlways;

		//boost::wregex complexRef(L"\\[(?:\'([^\']*)\'#){0,1}\\[{0,1}(?:\\$){0,1}([^\\.]+?){0,1}\\.(\\${0,1}[\\w^0-9]*\\${0,1}\\d*)(?::(\\${0,1}[^\\.]+?){0,1}\\.(\\${0,1}[\\w^0-9]*\\${0,1}\\d*)){0,1}\\]{0,1}");
		boost::wregex complexRef(L"(?:(?:(?:(?:\\[\'([^\']*)\'#)|(?:\'([^\']*)\'#\\[)))|(?:\\[))\
(?:\\$){0,1}([^\\.]+?){0,1}\\.(\\${0,1}[\\w^0-9]*\\${0,1}\\d*)(?::(\\${0,1}[^\\.]+?){0,1}\\.(\\${0,1}[\\w^0-9]*\\${0,1}\\d*)){0,1}\\]");
//									 [ 'external'#  [  $   Sheet2         . A1								 : ( $   Sheet2)? . B5                    ]

		expr = boost::regex_replace(
  			expr,
			complexRef,
			&replace_named_ref_formater,
			boost::match_default | boost::format_all);
	}
	void odf2oox_converter::Impl::replace_named_ref(std::wstring & expr, bool withTableName, bool bAbsoluteAlways)
	{
		XmlUtils::replace_all( expr, L"#REF !", L"#REF!");
		XmlUtils::replace_all( expr, L"#REF!#REF!", L"#REF!");
		XmlUtils::replace_all( expr, L"$#REF!$#REF!", L"#REF!");

		convert_with_TableName = withTableName;
		convert_with_absolute = bAbsoluteAlways;
		
		//boost::wregex complexRef(L"\\${0,1}([^\\.]+?){0,1}\\.(\\${0,1}[a-zA-Z]+\\${0,1}\\d+)(?::\\.(\\${0,1}[a-zA-Z]+\\${0,1}\\d+)){0,1}");
		boost::wregex complexRef(L"\\[{0,1}(?:\'([^\']*)\'#){0,1}\\${0,1}([^\\.\\s]+?){0,1}\\.(\\${0,1}[\\w^0-9]*\\${0,1}\\d*)(?::\\${0,1}([^\\.\\s]+?){0,1}\\.(\\${0,1}[\\w^0-9]*\\${0,1}\\d*)){0,1}\\]{0,1}");
//									  'external'#  $   Sheet2         . A1								 : ( $   Sheet2)? . B5   
	
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
	void odf2oox_converter::Impl::replace_apersand(std::wstring& expr)
	{
		const std::wstring res = boost::regex_replace(
			expr,
			//boost::wregex(L"(;)|(?:\".*?\")|(?:'.*?')"),
			boost::wregex(L"(&)|(\".*?\")|('.*?')"),
			&replace_apersand_formater,
			boost::match_default | boost::format_all);

		expr = res;
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

		bool isFormula = check_formula(workstr);

//экранирование
		workstr = boost::regex_replace(workstr,			
			boost::wregex(L"('.*?')|(\".*?\")"),
			&convert_scobci, boost::match_default | boost::format_all);

		replace_cells_range	(workstr, true);
		replace_semicolons	(workstr);
		replace_tilda		(workstr);
		replace_vertical	(workstr);
		//replace_apersand	(workstr);

		if (isFormula)
		{
			XmlUtils::replace_all( workstr, L"FDIST(", L"_xlfn.F.DIST(");
			// ROUNDUP( - тут в oox 2 параметра - разрядность нужно - ,0) - EV Requirements v2.2.3.ods
			
			if (std::wstring::npos != workstr.find(L"CONCATINATE"))
			{
				bool l = true;
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
		replace_tmp_back(workstr);

		return workstr;
	}

	void odf2oox_converter::Impl::split_distance_by(const std::wstring& expr, const std::wstring& by, std::vector<std::wstring>& out)
	{
		std::wstring workstr = boost::regex_replace(
			expr,
			boost::wregex(L"('.*?')|(\".*?\")"),
			&convert_scobci, boost::match_default | boost::format_all);
	    
		boost::algorithm::split(out, workstr, boost::algorithm::is_any_of(by), boost::algorithm::token_compress_on);
		
		for (size_t i = 0; i < out.size(); i++)
		{
			replace_tmp_back(out[i]);
		}
	}

	//Sheet2.C3:Sheet2.C19 Sheet2.L29:Sheet2.L36
	//в
	//Sheet2!C3:C19,Sheet2!L27:L34

	std::wstring odf2oox_converter::Impl::convert_chart_distance(const std::wstring& expr)
	{
		std::wstring workstr = boost::regex_replace(
			is_forbidden(expr),
			boost::wregex(L"('.*?')|(\".*?\")"),
			&convert_scobci, boost::match_default | boost::format_all);
	    
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

				if (colon != std::wstring::npos)
				{
					cells.push_back(range[j].substr(colon + 1));

					if (sheet.empty())
					{
						sheet = range[j].substr(0, colon);
					}
				}
				else
					cells.push_back(range[j]);
			}
			std::wstring cells_out;
			for (size_t j = 0; j < cells.size(); j++)
			{
				cells_out.append(cells[j]);
				cells_out.append(L":");
			}
			
			if (sheet.find(L"-") != std::wstring::npos && sheet.find(L"'") == std::wstring::npos && sheet.find(L"\"") == std::wstring::npos)
			{
				sheet = L"'" + sheet + L"'";
			}

			distance_out.push_back((sheet.empty() ? L"" : (sheet + L"!")) + cells_out.substr(0, cells_out.size() - 1));
		}
		std::wstring result;

		for (size_t i = 0; i < distance_out.size(); i++)
		{
			result.append(distance_out[i]);
			result.append(L",");
		}
 		replace_tmp_back( result );
		
		return result.substr(0, result.size() - 1);// минус последняя лишняя запятая
	}
	std::wstring odf2oox_converter::Impl::convert_named_ref(const std::wstring& expr, bool withTableName, std::wstring separator, bool bAbsoluteAlways)
	{
		boost::wregex complexRef(L"('(?!\\s\\'){0,1}.*?')");// поиск того что в апострофах и замена там

		std::wstring workstr = expr;

		workstr = boost::regex_replace(
			workstr,
			boost::wregex(L"('.*?')|(\".*?\")"),
			&convert_scobci, boost::match_default | boost::format_all);

		replace_named_ref(workstr, withTableName, bAbsoluteAlways);

		if (separator != L" ")
		{
			XmlUtils::replace_all( workstr, L" ", separator);
		}

		replace_tmp_back( workstr );

		if (table_name_.empty() == false)
		{
			replace_tmp_back( table_name_ );
		}
		return workstr;
	}
	std::wstring odf2oox_converter::Impl::convert_named_expr(const std::wstring& expr, bool withTableName, bool bAbsoluteAlways)
	{
		std::wstring workstr = expr;

		bool isFormula = check_formula(workstr);

		if (isFormula)
		{
			workstr = convert(expr);
		}
		else
		{

			workstr = boost::regex_replace(
				workstr,
				boost::wregex(L"('.*?')|(\".*?\")"),
				&convert_scobci, boost::match_default | boost::format_all);

		   
			replace_cells_range(workstr, withTableName, bAbsoluteAlways);
			replace_semicolons(workstr);
			replace_vertical(workstr);

			size_t res_find = 0;
			if ((res_find = workstr.find(L"CONCATINATE")) != std::wstring::npos)
			{
				//могут быть частично заданы диапазоны
				//todooo

			}
			replace_tmp_back(workstr);

			if (table_name_.empty() == false)
			{
				replace_tmp_back(table_name_);
 			}
		}
		return workstr;
	}

//------------------------------------------------------------------------------------------------------------
	odf2oox_converter::odf2oox_converter() : impl_(new odf2oox_converter::Impl())
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
	std::wstring odf2oox_converter::convert_named_ref(const std::wstring& expr, bool withTableName, std::wstring separator, bool bAbsoluteAlways)
	{
		return impl_->convert_named_ref(expr, withTableName, separator, bAbsoluteAlways);
	}
	std::wstring odf2oox_converter::convert_named_expr(const std::wstring& expr, bool withTableName, bool bAbsoluteAlways)
	{
		return impl_->convert_named_expr(expr, withTableName, bAbsoluteAlways);
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
			size_t pos = expr.find(L"%20");
			if (pos == std::wstring::npos)break;

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

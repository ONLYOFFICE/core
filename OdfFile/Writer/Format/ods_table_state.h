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
#pragma once

#include <string>
#include <vector>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "../../../OOXML/Base/Unit.h"

#include "odf_drawing_context.h"
#include "odf_controls_context.h"

#include "office_elements_create.h"
#include "style_graphic_properties.h"

#include "../../DataTypes/officevaluetype.h"


namespace cpdoccore {

struct oox_table_position
{
	_INT32 col;
    double col_off;
	_INT32 row;
    double row_off;
};

namespace odf_types
{
	class color;
}

namespace odf_writer {

class ods_conversion_context;
class odf_text_context;

class table_table;
class style;

namespace utils 
{
	static std::wstring getColAddress(size_t col)
	{
		static const size_t r = (L'Z' - L'A' + 1);
		std::wstring res;
		size_t r0 = col / r;

		if (r0 > 0)
		{
			const std::wstring rest = getColAddress(col - r * r0);
			const std::wstring res = getColAddress(r0 - 1) + rest;
			return res;
		}
		else
			return std::wstring(1, (wchar_t)(L'A' + col));
	}
    static _INT32 getColAddressInv(const std::wstring & a_)
	{
		std::wstring a = a_;
        XmlUtils::GetUpper(a);
		static const size_t r = (L'Z' - L'A' + 1);
		size_t mul = 1;
		bool f = true;
        int res = 0;

        for (int i = (int)a.length() - 1; i >= 0; i--)
		{
            size_t v = a[i] - L'A';
			if (f)
				f = false;
			else
				v += 1;
			res += (int)(v * mul);
			mul *= r;
		}
		return res;
	}
    static _INT32 getRowAdderssInv(const std::wstring & a_)
	{
		if (!a_.empty())
		{
           return boost::lexical_cast<_INT32>(a_) - 1;
		}
		else
			return 0;
	}
	static void splitCellAddress(const std::wstring & a_, std::wstring & col, std::wstring & row)
	{   
		std::wstring a = a_;

		std::reverse(a.begin(), a.end());
		XmlUtils::replace_all( a, L"$", L"");
		XmlUtils::replace_all( a, L"[", L"");
		XmlUtils::replace_all( a, L"]", L"");
		XmlUtils::GetUpper(a);
		
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
    static void parsing_ref (std::wstring ref, _INT32& col, _INT32& row)
	{
		if (std::wstring::npos != ref.find(L" "))
			return;

		size_t pos = ref.rfind(L"!");//oox table name
		if (pos != std::wstring::npos)
			ref = ref.substr(pos + 1);
		else
		{
			pos = ref.rfind(L".");//odf table name
			if (pos != std::wstring::npos)
				ref = ref.substr(pos + 1);
		}
		
		std::wstring strCol, strRow;
		splitCellAddress(ref, strCol, strRow);

		if (strCol.empty() || strRow.empty()) 
			return;
		
		if (strCol.size() > 3 || strRow.size() > 7)// 1048576 & 16384(xfd)
			return;
			
		col = getColAddressInv(strCol) + 1;
		row = getRowAdderssInv(strRow) + 1;

	}
}
struct ods_element_state
{
    ods_element_state(){}
    ods_element_state(office_element_ptr &elm_, _UINT32 repeated_, const std::wstring &style_name_, office_element_ptr &style_elm_, size_t level_)
        : elm(elm_), repeated(repeated_), style_name(style_name_), style_elm(style_elm_), level(level_)
    {}

    office_element_ptr elm;
	
	_UINT32 repeated = 1;
	std::wstring style_name;
	office_element_ptr style_elm;

    _UINT32 level = 1;
};
struct ods_column_state : ods_element_state
{
	ods_column_state(office_element_ptr& elm_, _UINT32 repeated_, const std::wstring& style_name_, office_element_ptr& style_elm_, double size_, size_t level_)
	{
		elm = elm_;
		repeated = repeated_;
		style_name = style_name_;
		style_elm = style_elm_;
		size = size_;
		level = level_;
	}
	double size = 0;
	std::wstring cell_style_name;
};
typedef ods_column_state ods_row_state;

struct ods_cell_state : ods_element_state
{
    _INT32 col = -1;
	_INT32 row = -1;

	_INT32 hyperlink_idx = -1;
	_INT32 comment_idx = -1;

	_INT32 data_validation_idx = -1;
	
    bool empty = true;
};

struct ods_hyperlink_state
{
	std::wstring ref;
	_INT32 col = -1;
	_INT32 row = -1;
	
	std::wstring link;
	bool bLocation = false;
};
struct ods_comment_state
{
	_INT32 col = -1;
	_INT32 row = -1;
	std::wstring author;

	office_element_ptr elm;

	graphic_format_properties *graphic_properties = NULL;

	bool used = false;
};
struct ods_shared_formula_state
{
	_UINT32 index;
	std::wstring formula;
	std::wstring ref;

	_INT32	base_column;
	_INT32	base_row;

    int moving_type; //1 - col, 2 - row
};

struct pilot_table_state
{
	office_element_ptr elm;
};

struct table_part_state
{
	std::wstring name;
	std::wstring ref;

	_INT32 col_start = 0;
	_INT32 row_start = 0;

	_INT32 col_end = 0;
	_INT32 row_end = 0;

	bool in_ref(_INT32 col, _INT32 row)
	{
		return (col >= col_start && col <= col_end && row >= row_start && row <= row_end);
	}

	std::vector<std::pair<std::wstring, std::wstring>> columns; //name, odf_ref
};
struct data_validation_state 
{
	std::wstring name;
	int type = 0;
	int operator_ = -1;

	office_element_ptr elm;

	struct _ref
	{
		std::wstring ref;
		_INT32 col_start = 0;
		_INT32 row_start = 0;

		_INT32 col_end = 0;
		_INT32 row_end = 0;
	};
	std::vector<_ref> refs;

	std::wstring condition;

	bool in_ref(_INT32 col, _INT32 row, _UINT32 repeate_col, _ref & ref)
	{
		for (size_t i = 0; i < refs.size(); i++)
		{
			if (row < refs[i].row_start || row > refs[i].row_end) continue;

			if (col + (_INT32)repeate_col <= refs[i].col_start ||  col > refs[i].col_end) continue;
			
			ref = refs[i];
			return true;
		}
		return false;
	}
	bool in_row(_INT32 row, _UINT32 repeate_row, _ref & ref)
	{
		for (size_t i = 0; i < refs.size(); i++)
		{
			if (row + (_INT32)repeate_row <= refs[i].row_start || row > refs[i].row_end) continue;

			ref = refs[i];
			return true;
		}
		return false;
	}
};
struct ods_array_formula_state
{
	std::wstring formula;
	std::wstring ref;
};

class ods_table_state
{
public:
	ods_table_state(odf_conversion_context * Context, office_element_ptr & elm);
		void set_table_name(std::wstring name);
		void set_table_style(office_element_ptr & _style);
		void set_table_hidden(bool Val);
		void set_table_master_page(std::wstring name);
		void set_table_rtl(bool Val);
		void set_table_tab_color(_CP_OPT(odf_types::color) & _color);
        void set_table_dimension(_INT32 col, _INT32 row);
		void set_table_print_ranges(const std::wstring &ranges);
		
		void set_table_protection(bool Val);
		void set_table_protection_insert_columns(bool Val);
		void set_table_protection_insert_rows(bool Val);
		void set_table_protection_delete_columns(bool Val);
		void set_table_protection_delete_rows(bool Val);
		void set_table_protection_unprotected_cells(bool Val);
		void set_table_protection_protected_cells(bool Val);

    void add_column(office_element_ptr & elm, _UINT32 repeated, office_element_ptr & style);
		void set_column_width_sym(double width);
		void set_column_width(double width);
		void set_column_optimal_width(bool val);
		void set_column_hidden(bool val);
		void set_column_default_cell_style(std::wstring & style_name);
		
        std::wstring get_column_default_cell_style(_INT32 column);

	void add_row_break(_INT32 val);
	void add_column_break(_INT32 val);

	void start_group(office_element_ptr & elm);
	void end_group();
		
	void start_rows(office_element_ptr& elm);
	void end_rows();

	_INT32 current_level() {return (_INT32)current_level_.size() - 1;}

	void start_headers(office_element_ptr & elm);
	void end_headers();

    void add_row(office_element_ptr & elm, _UINT32 repeated , office_element_ptr & style);//const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
		void set_row_hidden(bool Val);
		void set_row_optimal_height(bool val);
		void set_row_height(double height);
		void set_row_default_cell_style(std::wstring & style_name);
		void add_row_repeated();

	bool isSpannedCell(_INT32 col, _INT32 row, _INT32 &spanned_cols, _INT32 &spanned_rows);
	bool isCoveredCell(_INT32 col, _INT32 repeated_cols = 1);

	void start_cell(office_element_ptr & elm ,office_element_ptr & style);
	void end_cell();
    void add_default_cell(_INT32 repeated);

	void check_spanned_cells();

	void set_cell_format_value(odf_types::office_value_type::type value_type);
    void set_cell_type(int type);
    void set_cell_value(const std::wstring & value, bool need_cash = false);
	void set_cell_text(odf_text_context *text_context, bool cash_value = false);
	void set_cell_formula(std::wstring &formula);
	void set_cell_array_formula(std::wstring & formula, std::wstring ref);
	void set_cell_spanned(_INT32 spanned_cols, _INT32 spanned_rows);
    void add_or_find_cell_shared_formula(std::wstring & formula, std::wstring ref, _INT32 ind);

	void start_cell_text();
	void end_cell_text();
	
	static std::wstring replace_cell_row(boost::wsmatch const & what);
	static std::wstring replace_cell_column(boost::wsmatch const & what);
	
	void add_child_element( const office_element_ptr & child_element);

	void start_conditional_formats();
		void start_conditional_format(const std::wstring& ref);
            void start_conditional_rule(int rule_type, _CP_OPT(unsigned int) rank, _CP_OPT(bool) bottom, _CP_OPT(bool) percent, _CP_OPT(bool) above, _CP_OPT(bool) equal, _CP_OPT(int) stdDev);
				void set_conditional_formula(const std::wstring& formula);
                void set_conditional_value(int type, const std::wstring& value );
                
				void set_conditional_iconset(int type_iconset);
				
				void set_conditional_show_value(bool value);

				void add_conditional_colorscale(int index, _CP_OPT(odf_types::color) & color);
				
				void set_conditional_databar_color(_CP_OPT(odf_types::color) & color);
				void set_conditional_databar_negative_color(_CP_OPT(odf_types::color) & color);
				void set_conditional_databar_axis_color(_CP_OPT(odf_types::color) & color);
				void set_conditional_databar_axis_position(const std::wstring& value);
				void set_conditional_databar_gradient(bool val);
				void set_conditional_databar_max(unsigned int val);
				void set_conditional_databar_min(unsigned int val);

				void set_conditional_style_name(const std::wstring &style_name);
                void set_conditional_operator(int _operator);

				void set_conditional_text(const std::wstring &text);
				
				void set_conditional_time(int period);
			void end_conditional_rule();
		void end_conditional_format();
	void end_conditional_formats();

	void start_pilot_table(office_element_ptr & elm);
	void end_pilot_table();

	void start_sparkline_groups();
		void start_sparkline_group();
		void set_sparkline_id(const std::wstring& val);
		void set_sparkline_type(int type);
		void set_sparkline_manual_max(double val);
		void set_sparkline_manual_min(double val);
		void set_sparkline_line_weight(double val);
		void set_sparkline_minAxisType(int val);
		void set_sparkline_maxAxisType(int val);
		void set_sparkline_emptyCellsAs(int val);
		void set_sparkline_markers(bool val);
		void set_sparkline_date_axis(bool val);
		void set_sparkline_high(bool val);
		void set_sparkline_low(bool val);
		void set_sparkline_first(bool val);
		void set_sparkline_last(bool val);
		void set_sparkline_negative(bool val);
		void set_sparkline_display_xAxis(bool val);
		void set_sparkline_display_hidden(bool val);
		void set_sparkline_rtl(bool val);

		void set_sparkline_color_series(_CP_OPT(odf_types::color) &color);
		void set_sparkline_color_negative(_CP_OPT(odf_types::color)& color);
		void set_sparkline_color_axis(_CP_OPT(odf_types::color)& color);
		void set_sparkline_color_markers(_CP_OPT(odf_types::color)& color);
		void set_sparkline_color_first(_CP_OPT(odf_types::color)& color);
		void set_sparkline_color_last(_CP_OPT(odf_types::color)& color);
		void set_sparkline_color_high(_CP_OPT(odf_types::color)& color);
		void set_sparkline_color_low(_CP_OPT(odf_types::color)& color);

			void start_sparklines();
				void start_sparkline();
					void set_sparkline_range(const std::wstring& ref);
					void set_sparkline_cell(const std::wstring& ref);
				void end_sparkline();
			void end_sparklines();
		void end_sparkline_group();
	void end_sparkline_groups();
///////////////////////////////
    void add_hyperlink(const std::wstring & ref, _INT32 col, _INT32 row, const std::wstring & link, const std::wstring & location);
	
	void add_definded_expression(office_element_ptr & elm);

    void start_comment(_INT32 col, _INT32 row, std::wstring & author);
		void set_comment_rect(double l, double t, double w, double h);
		void set_comment_visible(bool val);
		void set_comment_color(const std::wstring & color);
	void end_comment(odf_text_context *text_context);
	
    void set_merge_cells(_INT32 start_col, _INT32 start_row, _INT32 end_col, _INT32 end_row);

	office_element_ptr & current_row_element();
	office_element_ptr & current_cell_element();

	bool is_cell_data_validation();
	bool is_cell_hyperlink();
	bool is_cell_comment();

	_INT32 is_cell_hyperlink(_INT32 col, _INT32 row);
	_INT32 is_cell_comment(_INT32 col, _INT32 row, _UINT32 repeate_col = 1);
	_INT32 is_row_comment(_INT32 row, _INT32 repeate_row = 1);
	_INT32 is_row_validation(_INT32 row, _INT32& repeate_row);
	_INT32 is_cell_data_validation(_INT32 col, _INT32 row, _UINT32 repeate_col, data_validation_state::_ref & ref);

	_UINT32 get_last_row_repeated ();

	ods_hyperlink_state & current_hyperlink();

	std::vector<table_part_state> & table_parts() 
	{
		return table_parts_;
	}

	_INT32 current_column() const;
	_INT32 current_row() const;

	_INT32 dimension_columns;
	_INT32 dimension_row;

	double defaut_column_width_sym_ = 0;
	double defaut_column_width_;
	double defaut_row_height_;

	std::vector<_INT32> column_breaks_;
	std::vector<_INT32> row_breaks_;

	void convert_position(oox_table_position & oox_pos, double & x, double & y);

	odf_drawing_context*	drawing_context(){return  drawing_context_.get();}
	odf_controls_context*	controls_context(){return  &controls_context_;}

	std::wstring office_table_name_;
	std::vector<ods_comment_state> comments_;

	std::map<std::wstring, office_element_ptr> mapHeaderFooterImages;

private:

	struct _spanned_info
	{
		_INT32 spanned_cols = 0;
		_INT32 spanned_rows = 0;
		_INT32 state = 0;
	};
	struct _covered_info
	{
		_INT32 start_col = 0;
		_INT32 count_cols = 0;

		_INT32 count_rows = 0; // от текущей строки
	};
	std::vector<_covered_info> current_covered_rows_;
	_INT32 current_covered_cols_;

    odf_conversion_context *context_;   
	
	office_element_ptr	office_table_;
	style*				office_table_style_ = NULL;//??? может хранить как office_element_ptr ???
	office_element_ptr	table_defined_expressions_;
	
	std::wstring row_default_cell_style_name_;

	static _INT32 current_table_column_;
    static _INT32 current_table_row_;

    static _INT32 tmp_column_;
    static _INT32 tmp_row_;

	std::vector<ods_column_state> columns_;
	std::vector<ods_row_state> rows_;

//            row          column
	std::map<_INT32, std::map<_INT32, _spanned_info>> map_merged_cells;
	
	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней ("0-й элемент - сама таблица)
	
	std::vector<ods_cell_state>	cells_;
	
	std::vector<ods_hyperlink_state> hyperlinks_;
	std::map<_UINT32, ods_shared_formula_state> shared_formulas_;

	std::map<std::wstring, std::wstring> mapTabled; // for formula used ... perhaps

	std::vector<table_part_state> table_parts_;

	std::vector<data_validation_state> data_validations_;

	pilot_table_state pilot_table_state_;

	odf_drawing_context_ptr		drawing_context_;	
	odf_controls_context		controls_context_;	

	friend class ods_table_context;
};
typedef shared_ptr<ods_table_state>::Type ods_table_state_ptr;

}
}


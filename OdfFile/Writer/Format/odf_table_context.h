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

#include <CPOptional.h>

#include "office_elements_create.h"

namespace cpdoccore {
namespace odf_writer
{

class odf_conversion_context;
class odf_style_context;

class odf_table_context
{
public:
	odf_table_context(odf_conversion_context *odf_context);
    ~odf_table_context();

	void set_styles_context(odf_style_context*  styles_context);
	
	void set_table_styled(bool val);
	bool is_styled();
	bool is_styled(int row, int col);

	void set_table_size(size_t cols, size_t rows);

	bool empty();

	int current_column ();
	int current_row ();
	int count_columns ();
	int count_rows ();

	void start_table(office_element_ptr &elm, bool styled = false);
		void set_default_column_width(double val);
		void set_optimal_column_width(bool val);
		void change_current_column_width(double val);
		void set_default_row_height(double val);
	void end_table();
	
	void set_first_row_cell_properties(const std::wstring &style_name);
	void set_first_col_cell_properties(const std::wstring &style_name);
	void set_band_row_cell_properties(const std::wstring &style_name);
	void set_band_col_cell_properties(const std::wstring &style_name);
	void set_last_row_cell_properties(const std::wstring &style_name);
	void set_last_col_cell_properties(const std::wstring &style_name);
	
	void set_default_cell_properties(const std::wstring &style_name);
	std::wstring get_default_cell_properties();
	std::wstring get_column_cell_properties();
	
	_CP_OPT(double) get_table_width();
	_CP_OPT(double) get_table_height();

	void set_table_inside_v(_CP_OPT(std::wstring) border);
	void set_table_inside_h(_CP_OPT(std::wstring) border);
	
	_CP_OPT(std::wstring) get_table_inside_v();
	_CP_OPT(std::wstring) get_table_inside_h();

	void start_row(office_element_ptr &elm, bool styled = false);
		void set_row_height(double val);
	void end_row();

	void start_cell(office_element_ptr &elm, bool styled = false);
		void set_cell_column_span(int spanned = 0);
		void set_cell_column_span_restart();	
		void set_cell_row_span(int spanned = 0);
		void set_cell_row_span_restart();	
	void end_cell();

	void add_column(office_element_ptr &elm, bool styled = false);
		void set_column_width(double width);
		void set_column_optimal(bool val);



private:

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}
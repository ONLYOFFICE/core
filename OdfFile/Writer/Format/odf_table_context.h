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

	void set_default_cell_paddings(
		_CP_OPT(odf_types::length) left,
		_CP_OPT(odf_types::length) right,
		_CP_OPT(odf_types::length) top,
		_CP_OPT(odf_types::length) bottom);
	void get_default_cell_paddings(
		_CP_OPT(odf_types::length)& left,
		_CP_OPT(odf_types::length)& right,
		_CP_OPT(odf_types::length)& top,
		_CP_OPT(odf_types::length)& bottom);
	
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
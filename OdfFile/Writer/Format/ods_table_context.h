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
#include "ods_table_state.h"

namespace cpdoccore {
namespace odf_writer {

class ods_conversion_context;
class ods_text_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

struct table_additional_elements_state
{
	office_element_ptr				root;
	std::vector<office_element_ptr> elements;
};

class ods_table_context
{
public:
    ods_table_context(ods_conversion_context & Context/*, ods_text_context & textCotnext*/);

    void start_table(office_element_ptr & elm);
    void end_table();

    unsigned int columns_count();

    ods_table_state_ptr & state();
	
	void start_defined_expressions(office_element_ptr & root_elm);

    void add_defined_range(const std::wstring & name, const  std::wstring & cell_range, int sheet_id, bool printable = false);
    void add_defined_expression(const std::wstring & name, const  std::wstring & value, int sheet_id, bool printable = false);

	void add_autofilter(std::wstring ref);
	
	void start_table_part(const std::wstring &name, std::wstring ref);
		void set_table_part_autofilter(bool val);
		void add_table_part_column(std::wstring name);
	void end_table_part();

	bool start_data_validation(const std::wstring &ref, int type);
		void set_data_validation_operator(int val);
		void set_data_validation_content(std::wstring val1, std::wstring val2);
		void set_data_validation_allow_empty(bool val);
		void set_data_validation_error(const std::wstring &title, const std::wstring &content, bool display, int type);
		void set_data_validation_promt(const std::wstring &title, const std::wstring &content, bool display);
	void end_data_validation();

	void start_pivot_table(const std::wstring &name);
	void end_pivot_table();
private:

    ods_conversion_context & context_;

	std::vector<ods_table_state_ptr> table_state_list_;
	
	table_additional_elements_state	table_defined_expressions_;
	table_additional_elements_state	table_database_ranges_;
	table_additional_elements_state	table_content_validations_;
	table_additional_elements_state	table_pivots_;

	size_t count_validations_;

	friend class ods_conversion_context;

};


}
}

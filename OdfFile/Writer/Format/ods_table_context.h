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

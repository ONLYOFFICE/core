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
#include <boost/unordered_map.hpp>

#include "office_elements_create.h"

#include "../../DataTypes/officevaluetype.h"

namespace cpdoccore {
namespace odf_writer {

class ods_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

struct number_format_state
{
	int oox_num_fmt;//дефолтные (по документации - номера 0-163, за исключением некоторых)

	odf_types::office_value_type::type ods_type;

	std::vector<std::wstring> format_code;

	std::wstring style_name;

	unsigned int language_code;
	std::wstring currency_str;
};

class odf_number_styles_context
{
public:
    odf_number_styles_context();
	void set_odf_context(odf_conversion_context * Context);

	number_format_state		&	add_or_find(int oox_num_fmt, std::wstring formatCode = L"");
	void process_styles(office_element_ptr root );

private:
	std::vector<number_format_state> number_format_array_;
	boost::unordered_map<int, int> named_link_map_;

	void create(int oox_num_fmt, std::wstring formatCode = L""); 	
	void create_default(int oox_num_fmt, std::wstring formatCode = L"");
	void create_style(number_format_state & state);	

	void detect_format(number_format_state & state);

////////////////
	odf_conversion_context			*odf_context_;
	std::vector<office_element_ptr>	styles_elments;

//////////////////
	void create_number_style	(number_format_state & state, office_element_ptr & root_elm);
	void create_currency_style	(number_format_state & state, office_element_ptr & root_elm);
	void create_date_style		(number_format_state & state, office_element_ptr & root_elm);
	void create_time_style		(number_format_state & state, office_element_ptr & root_elm);
	void create_boolean_style	(number_format_state & state, office_element_ptr & root_elm);
	void create_text_style		(number_format_state & state, office_element_ptr & root_elm);
	void create_percentage_style(number_format_state & state, office_element_ptr & root_elm);

	void create_numbers(number_format_state & state, office_element_ptr & elm, office_element_ptr & root_elm);


};
}
}

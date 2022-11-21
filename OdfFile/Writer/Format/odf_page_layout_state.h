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

#include "office_elements_create.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/stylenumformat.h"

namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;
class odf_style_context;
class odf_page_layout_context;

class style_page_layout_properties;
class style_header_footer_properties;


struct 	odf_element_state
{
	odf_element_state() {}
	
	odf_element_state(office_element_ptr _elm, const std::wstring & _style_name = L"", office_element_ptr _style_elm = office_element_ptr(), size_t _level = 0, size_t _type = 0)
		: elm(_elm), style_name(_style_name), style_elm(_style_elm), level(_level), type(_type)
	{
	}
	office_element_ptr	elm;
	std::wstring		style_name;

	office_element_ptr	style_elm;
	
	size_t level = 0;
	size_t type = 0;
};

class odf_master_state
{
public:
	odf_master_state(office_element_ptr & master);

	void add_child(office_element_ptr & child_elm, office_element_ptr  style_elm, std::wstring style_name );

	void			set_name(std::wstring & name);
	std::wstring	get_name();

	void add_footer (office_element_ptr & elm);
	void add_header (office_element_ptr & elm);
	
    void set_layout_style_name (std::wstring name);
	void set_display_name (std::wstring & name);
	void set_class(const std::wstring &value);

	office_element_ptr & get_root();
	office_element_ptr & get_last_element();
	
private:
	std::wstring	style_oox_name_;

	std::vector<office_element_ptr>	current_level_;
	std::vector<odf_element_state>	elements_;

	friend class odf_page_layout_context;
};

class odf_layout_state
{
public:
	odf_layout_state(office_element_ptr & elm);

	void add_child(office_element_ptr & child_elm, office_element_ptr  style_elm, std::wstring style_name );

	void			set_name(std::wstring & name);
	std::wstring	get_name();	

	style_page_layout_properties	*get_properties();//пока не красиво ... лучше хранить прямую ссыль нежели искать
	style_header_footer_properties	*get_header_properties();//-''-
	style_header_footer_properties	*get_footer_properties();//-''-

	void set_pages_mirrored(bool val);

	_CP_OPT(odf_types::length) header_size_;
	_CP_OPT(odf_types::length) footer_size_;
	
	_CP_OPT(odf_types::length) header_min_size_;
	_CP_OPT(odf_types::length) footer_min_size_;

	_CP_OPT(odf_types::style_numformat) page_number_format;

private:
	std::wstring	style_oox_name_;

	std::vector<office_element_ptr>	current_level_;
	std::vector<odf_element_state>	elements_;

	friend class odf_page_layout_context;
};

}
}


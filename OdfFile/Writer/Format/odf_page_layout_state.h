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

	style_page_layout_properties	*get_properties();//not pretty for now ... better to store direct reference rather than search
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


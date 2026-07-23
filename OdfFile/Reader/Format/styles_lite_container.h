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

#include <CPScopedPtr.h>
#include <CPOptional.h>
#include <CPSharedPtr.h>

namespace cpdoccore { 
namespace odf_reader {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class styles_lite_container
{
public:
    styles_lite_container();
    ~styles_lite_container();

	void add(const std::wstring & style_name, office_element_ptr content);
    office_element_ptr find_by_style_name(const std::wstring & style_name);
    
private:
    struct Impl;
    _CP_SCOPED_PTR(Impl) impl_;

};

class doc_props_container
{
public:
	doc_props_container();
	~doc_props_container();

	void add_user_defined(const std::wstring & name, const std::wstring & value);
	std::wstring get_user_defined(const std::wstring & name);
	std::wstring dump_user_defined();
	
	std::wstring dc_creator_;
	std::wstring dc_date_;
	std::wstring dc_description_;
	std::wstring dc_language_;
	std::wstring dc_subject_;
	std::wstring dc_title_;
	std::wstring application_;
	std::wstring creation_date_;
	std::wstring keyword_;
	std::wstring template_;
	_CP_OPT(int) revision_;

	_CP_OPT(int) page_count_;
	_CP_OPT(int) paragraph_count_;
	_CP_OPT(int) word_count_;
	_CP_OPT(int) character_count_;
	_CP_OPT(int) non_whitespace_character_count_; 
private:
	class Impl;
	_CP_SCOPED_PTR(Impl) impl_;
};
class settings_container
{
public:
	settings_container();
	~settings_container();

	_CP_OPT(std::wstring) find_by_name				(const std::wstring & name);
    _CP_OPT(std::wstring) find_view_by_name			(const std::wstring & name, int index = -1); //"-1" - common
    
	std::pair<std::wstring, std::wstring> get_table_view (int index_view, const std::wstring & table_name, int index);

	int	get_views_count		();
	int get_table_view_count(int ind, std::wstring name);

	void add (const std::wstring & name, const std::wstring & value);

	void start_view	();
	void end_view ();

	void start_table_view (const std::wstring & name);
	void end_table_view ();
	
	void add_view (const std::wstring & name, const std::wstring & value);

	void set_tab_distance(double pt);
	_CP_OPT(double) get_tab_distance();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

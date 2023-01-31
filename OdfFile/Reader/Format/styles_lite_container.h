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

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

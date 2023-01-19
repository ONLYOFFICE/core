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

#include "odf_style_state.h"
#include "odf_number_styles_context.h"
#include "odf_table_styles_context.h"
#include "odf_lists_styles_context.h"


namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_style_context;
typedef shared_ptr<odf_style_context>::Type odf_style_context_ptr;

class style;
class paragraph_format_properties;
class text_format_properties_content;

class odf_style_context
{
public:
    odf_style_context();
	void set_odf_context(odf_conversion_context * Context);

	void create_style(std::wstring name, odf_types::style_family::type family, bool automatic = false, bool root = false, int oox_id = -1);
	void create_default_style(odf_types::style_family::type family);

	void add_style(office_element_ptr elm, bool automatic = false, bool root = false, odf_types::style_family::type family=odf_types::style_family::None);

	office_element_ptr & add_or_find(std::wstring name, odf_types::style_family::type family, bool automatic = false, bool root = false, int id = -1);

	void process_automatic_for_styles	(office_element_ptr root );
	void process_automatic_styles		(office_element_ptr root );
	void process_office_styles			(office_element_ptr root );

	void reset_defaults();	
    void add_default(const odf_style_state_ptr & state)	{current_default_styles_.push_back(state);}
	
	std::wstring			find_odf_style_name_default	(odf_types::style_family::type family);
	office_element_ptr		find_odf_style_default		(odf_types::style_family::type family);

	std::wstring 			find_conditional_style_name	(int oox_id_style, odf_types::style_family::type family);
	office_element_ptr		find_conditional_style		(int oox_id_style, odf_types::style_family::type family);
	
	bool find_odf_default_style_state(odf_types::style_family::type family, odf_style_state_ptr & state);
	
	bool find_odf_style(std::wstring style_name, odf_types::style_family::type family, style *& style_);
	bool find_odf_style_state(std::wstring style_name, odf_types::style_family::type family, odf_style_state_ptr & state); //имена в odf_writer сделаны полностью уникальными

	odf_style_state_ptr	last_state(odf_types::style_family::type family = odf_types::style_family::None);

	odf_number_styles_context & numbers_styles() {return number_styles_context_;}
	odf_table_styles_context & table_styles() {return table_styles_context_;}
	odf_lists_styles_context & lists_styles() {return lists_styles_context_;}
	
	std::wstring find_free_name(odf_types::style_family::type  family);

//////////////////////////////////////////////////////////////////////
	void calc_paragraph_properties(std::wstring style_name, odf_types::style_family::type family, paragraph_format_properties * result);
	void calc_text_properties(std::wstring style_name, odf_types::style_family::type family, text_format_properties_content * result);
//////////////////////////////////////////////////////////////////////
	std::wstring find_odf_style_name (int oox_id_style, odf_types::style_family::type family, bool root, bool automatic);//xlsx only
	bool		 find_odf_style_state(int oox_id_style, odf_types::style_family::type family, odf_style_state_ptr & state, bool root, bool automatic);//xlsx only

private:
	odf_table_styles_context	table_styles_context_;
	odf_number_styles_context	number_styles_context_;
	odf_lists_styles_context	lists_styles_context_;
   
    
	std::vector<odf_style_state_ptr>		style_state_list_;	
	std::vector<odf_style_state_ptr>		current_default_styles_;//для внутренней работы .. переодически очищаемый частично или полностью
   
	std::wstring get_name_family(odf_types::style_family::type family);

	odf_conversion_context *	odf_context_;

};


}
}

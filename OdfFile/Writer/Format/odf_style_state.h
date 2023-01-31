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

#include "../../DataTypes/stylefamily.h"

namespace cpdoccore {
namespace odf_writer {

class style;
class odf_conversion_context;
class odf_style_context;

class graphic_format_properties;

class style_text_properties;
class style_paragraph_properties;
class style_table_properties;
class style_section_properties;
class style_table_cell_properties;
class style_table_row_properties;
class style_table_column_properties;
class style_chart_properties;
class style_drawing_page_properties;

class odf_style_state;
typedef shared_ptr<odf_style_state>::Type odf_style_state_ptr;

class odf_style_state
{
public:
	odf_style_state(office_element_ptr & elm, const odf_types::style_family::type family );

	void set_automatic(bool val);
	void set_root(bool val);
	void set_default(bool val);
	void set_conditional(bool val);

	office_element_ptr & get_office_element();

	void apply_from(style* other_style);

	void set_dont_write(bool val);

	void add_child(office_element_ptr & child);
/////////////////////////////////////////////////////////////////////////////////////////
    void			set_name(const std::wstring &name);
	std::wstring 	get_name();

	void	set_class(const std::wstring &value);

	odf_types::style_family::type	get_family_type	();
	void							set_family_type	(odf_types::style_family::type type);

	void	set_number_format(int id)	{num_fmt_id_ = id;}
	int		get_number_format()			{return num_fmt_id_;}

    void	set_parent_style_name(const std::wstring &name) ;
    void	set_data_style_name(const std::wstring &name);
    void	set_list_style_name(const std::wstring &name);

    void	set_display_name(const std::wstring &name);
//////////////////////////////////////////////////////////////////////////////////////////
    graphic_format_properties *		get_graphic_properties() ;
    
	style_text_properties *			get_text_properties();
    style_paragraph_properties *	get_paragraph_properties() ;
    style_table_properties *		get_table_properties();
    style_section_properties *		get_section_properties();
    style_table_cell_properties *	get_table_cell_properties();
    style_table_row_properties *	get_table_row_properties() ;
    style_table_column_properties * get_table_column_properties() ;
    style_chart_properties *		get_chart_properties() ;
	style_drawing_page_properties*	get_drawing_page_properties();

	bool	get_list_style_exist();
	int		get_list_style_level();
	int		get_list_style_id();

	void	set_list_style_exist(bool val);
	void	set_list_style_level(int val);
	void	set_list_style_id(int val);

private:
	std::wstring	odf_style_name_;
	std::wstring	oox_style_name_;

//инфа для описания ячеек
	int				style_oox_id_;
	int				num_fmt_id_; //default =0 (general) 
///////	
	bool automatic_;
	bool root_;
	bool default_;		// это для тех стилей что в root
	bool conditional_;	//стили накладываемые по условию

	bool writable_;

	odf_types::style_family::type style_family_;
	
	office_element_ptr	odf_style_;

	friend class odf_style_context;

	bool	list_style_exist;
	int		list_style_level;
	int		list_style_id;
};


}
}


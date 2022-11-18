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

#include "logging.h"

#include "odf_style_state.h"
#include "odf_conversion_context.h"

#include "styles.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

	odf_style_state::odf_style_state(office_element_ptr & elm, style_family::type family )
{        
	odf_style_ = elm;
	
	automatic_		= false;
	default_		= false;
	conditional_	= false;

	num_fmt_id_		= 0;
	style_family_	= family;

	writable_		= true;

	style* style_ = dynamic_cast<style*>(elm.get());
	if (style_)
	{
		style_->style_family_ = style_family(style_family_);
		style_->content_.style_family_ = style_family_;
	}
	default_style* default_style_ = dynamic_cast<default_style*>(elm.get());

	if (default_style_)
	{
		default_style_->style_family_ = style_family(style_family_);
		default_style_->content_.style_family_ = style_family_;
	}

	list_style_exist = false;
	list_style_level = -1;
	list_style_id	 = -1;
}

office_element_ptr & odf_style_state::get_office_element()
{
	return odf_style_;
}

void odf_style_state::add_child(office_element_ptr & child)
{
	odf_style_->add_child_element(child);
}

void odf_style_state::set_name(const std::wstring &name)
{
	odf_style_name_ = name;

	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_name_ = name;
}
void odf_style_state::set_display_name(const std::wstring &name)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_display_name_ = name;
}
void odf_style_state::set_class(const std::wstring &value)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;

	style_->style_class_ = value;
}
std::wstring odf_style_state::get_name()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());	
	if (!style_) return odf_style_name_;

	return style_->style_name_;
}
style_family::type odf_style_state::get_family_type()
{
	return style_family_;
}
void odf_style_state::set_family_type (odf_types::style_family::type type)
{
	 style_family_ = type;
}

void odf_style_state::set_parent_style_name(const std::wstring &name)
{
	if (name.length() < 1) return;

	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	if (name == style_->style_name_ || name.length() <1)
		return;
	style_->style_parent_style_name_ = name;
}
void odf_style_state::set_list_style_name(const std::wstring &name)
{
	if (name.length() < 1) return;

	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	if (name == style_->style_name_ || name.length() <1)
		return;
	style_->style_list_style_name_ = name;
}
void odf_style_state::set_dont_write(bool Val)
{
	writable_ = !Val;
}
void odf_style_state::set_data_style_name(const std::wstring &name)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_data_style_name_ = name;
}
//////////////////////////////////////////////////////////////////////
void odf_style_state::set_automatic(bool val)
{
	automatic_ = val;
}
void odf_style_state::set_root(bool val)//метка того что в файле styles, а не content
{
	root_ = val;
}
void odf_style_state::set_default(bool val)
{
	default_ = val;
}
void odf_style_state::set_conditional(bool val)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());	
	if (!style_) return;

	conditional_ = val;
	if (val)
		style_->style_name_ = L"dx_" + style_->style_name_;
}
//------------------------------------------------------------------------------------
bool odf_style_state::get_list_style_exist()
{
	return list_style_exist;
}
int	odf_style_state::get_list_style_level()
{
	return list_style_level;
}
int odf_style_state::get_list_style_id()
{
	return list_style_id;
}

void odf_style_state::set_list_style_exist(bool val)
{
	list_style_exist = val;
}
void odf_style_state::set_list_style_level(int val)
{
	list_style_level = val;
}
void odf_style_state::set_list_style_id(int val)
{
	list_style_id = val;
}
//------------------------------------------------------------------------------------
void odf_style_state::apply_from(style* other_style)
{
	if (other_style == NULL)return;
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;

	//style_->style_parent_style_name_ = other_style->style_parent_style_name_;

	//style_paragraph_properties * props = style_->content_.get_style_text_properties();
	//if (props)props->apply_from(other_style->content_.get_style_paragraph_properties());
	
}
//////////////////////////////////////////////////////////////////
style_text_properties * odf_style_state::get_text_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_)	return style_->content_.get_style_text_properties();   

	default_style* style_d_ = dynamic_cast<default_style*>(odf_style_.get());
	if (style_d_)return style_d_->content_.get_style_text_properties(); 
		
	return NULL;
}

style_paragraph_properties * odf_style_state::get_paragraph_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_paragraph_properties(); 
	
	default_style* style_d = dynamic_cast<default_style*>(odf_style_.get());
	if (style_d) return style_d->content_.get_style_paragraph_properties(); 
	
	return NULL;
}

graphic_format_properties * odf_style_state::get_graphic_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_)return style_->content_.get_graphic_properties();  
	
	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_)return default_style_->content_.get_graphic_properties();  

	return NULL;
}

style_table_properties * odf_style_state::get_table_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_table_properties(); 

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->content_.get_style_table_properties(); 

	return NULL;
}

style_section_properties * odf_style_state::get_section_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_section_properties();   

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->content_.get_style_section_properties();   

	return NULL;
}

style_table_cell_properties * odf_style_state::get_table_cell_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_table_cell_properties();

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->content_.get_style_table_cell_properties();

	return NULL;
}

style_table_row_properties * odf_style_state::get_table_row_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_table_row_properties(); 
	
	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->content_.get_style_table_row_properties(); 

	return NULL;
}

style_table_column_properties * odf_style_state::get_table_column_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_table_column_properties();

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->content_.get_style_table_column_properties();

	return NULL;
}

style_chart_properties * odf_style_state::get_chart_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->content_.get_style_chart_properties(); 

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->content_.get_style_chart_properties(); 

	return NULL;
}

style_drawing_page_properties * odf_style_state::get_drawing_page_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->content_.get_style_drawing_page_properties();
}

///////////////////////

}
}

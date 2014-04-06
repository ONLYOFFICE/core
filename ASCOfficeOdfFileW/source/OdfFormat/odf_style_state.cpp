#include "precompiled_cpodf.h"
#include "logging.h"

#include "odf_style_state.h"
#include "odf_conversion_context.h"

#include "styles.h"

namespace cpdoccore {
namespace odf {

odf_style_state::odf_style_state(office_element_ptr & elm, const style_family family )
{        
	odf_style_ = elm;
	
	automatic_= false;
	default_ = false;

	num_fmt_id_ =0;
	style_family_ = family;

	style* style_ = dynamic_cast<style*>(elm.get());
	if (style_)
	{
		style_->style_family_ = style_family_;
	}
	default_style* default_style_ = dynamic_cast<default_style*>(elm.get());

	if (default_style_)
	{
		default_style_->style_family_ = style_family_;
	}
}

office_element_ptr & odf_style_state::get_office_element()
{
	return odf_style_;
}

void odf_style_state::add_child(office_element_ptr & child)
{
	odf_style_->add_child_element(child);
}

void odf_style_state::set_name(std::wstring & name)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_name_ = name;
}
std::wstring & odf_style_state::get_name()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return std::wstring();

	return style_->style_name_;
}
void odf_style_state::set_parent_style_name(std::wstring & name)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	if (name == style_->style_name_)
		return;
	style_->style_parent_style_name_ = name;
}
void odf_style_state::set_data_style_name(std::wstring & name)
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
//////////////////////////////////////////////////////////////////
style_text_properties * odf_style_state::get_text_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_text_properties();   
}

style_paragraph_properties * odf_style_state::get_paragraph_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_paragraph_properties(); 
}

style_graphic_properties * odf_style_state::get_graphic_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_graphic_properties();  
}
style_table_properties * odf_style_state::get_table_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_table_properties(); 
}

style_section_properties * odf_style_state::get_section_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_section_properties();   
}

style_table_cell_properties * odf_style_state::get_table_cell_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_table_cell_properties();
}

style_table_row_properties * odf_style_state::get_table_row_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_table_row_properties(); 
}

style_table_column_properties * odf_style_state::get_table_column_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_table_column_properties();
}

style_chart_properties * odf_style_state::get_chart_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return NULL;

	return style_->style_content_.get_style_chart_properties(); 
}
//style_drawing_page_properties * odf_style_state::get_drawing_page_properties()
//{
//	style* style_ = dynamic_cast<style*>(odf_style_.get());
//	if (!style_)return NULL;
//
//	return style_->style_content_.get_style_drawing_page_properties();
//}

///////////////////////

}
}

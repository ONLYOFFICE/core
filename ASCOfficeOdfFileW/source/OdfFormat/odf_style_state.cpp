#include "precompiled_cpodf.h"
#include "logging.h"

#include "odf_style_state.h"
#include "odf_conversion_context.h"

#include "styles.h"

namespace cpdoccore {
namespace odf {

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
	}
	default_style* default_style_ = dynamic_cast<default_style*>(elm.get());

	if (default_style_)
	{
		default_style_->style_family_ = style_family(style_family_);
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
	odf_style_name_ = name;

	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_name_ = name;
}

void odf_style_state::set_display_name(std::wstring & name)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_display_name_ = name;

}
std::wstring odf_style_state::get_name()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	
	if (!style_)return odf_style_name_;

	return style_->style_name_;
}
style_family::type odf_style_state::get_family_type()
{
	return style_family_;
}
void odf_style_state::set_parent_style_name(std::wstring & name)
{
	if (name.length() < 1) return;

	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	if (name == style_->style_name_ || name.length() <1)
		return;
	style_->style_parent_style_name_ = name;
}
void odf_style_state::set_list_style_name(std::wstring & name)
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
void odf_style_state::set_conditional(bool val)
{
	conditional_ = val;
}

void odf_style_state::apply_from(style* other_style)
{
	if (other_style == NULL)return;
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;

	//style_->style_parent_style_name_ = other_style->style_parent_style_name_;

	//style_paragraph_properties * props = style_->style_content_.get_style_text_properties();
	//if (props)props->apply_from(other_style->style_content_.get_style_paragraph_properties());
	
}
//////////////////////////////////////////////////////////////////
style_text_properties * odf_style_state::get_text_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_)	return style_->style_content_.get_style_text_properties();   

	default_style* style_d_ = dynamic_cast<default_style*>(odf_style_.get());
	if (style_d_)return style_d_->style_content_.get_style_text_properties(); 
		
	return NULL;
}

style_paragraph_properties * odf_style_state::get_paragraph_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_paragraph_properties(); 
	
	default_style* style_d = dynamic_cast<default_style*>(odf_style_.get());
	if (style_d) return style_d->style_content_.get_style_paragraph_properties(); 
	
	return NULL;
}

style_graphic_properties * odf_style_state::get_graphic_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_)return style_->style_content_.get_style_graphic_properties();  
	
	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_)return default_style_->style_content_.get_style_graphic_properties();  

	return NULL;
}
style_table_properties * odf_style_state::get_table_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_table_properties(); 

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->style_content_.get_style_table_properties(); 

	return NULL;
}

style_section_properties * odf_style_state::get_section_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_section_properties();   

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->style_content_.get_style_section_properties();   

	return NULL;
}

style_table_cell_properties * odf_style_state::get_table_cell_properties()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_table_cell_properties();

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->style_content_.get_style_table_cell_properties();

	return NULL;
}

style_table_row_properties * odf_style_state::get_table_row_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_table_row_properties(); 
	
	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->style_content_.get_style_table_row_properties(); 

	return NULL;
}

style_table_column_properties * odf_style_state::get_table_column_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_table_column_properties();

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->style_content_.get_style_table_column_properties();

	return NULL;
}

style_chart_properties * odf_style_state::get_chart_properties() 
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (style_) return style_->style_content_.get_style_chart_properties(); 

	default_style* default_style_ = dynamic_cast<default_style*>(odf_style_.get());
	if (default_style_) return default_style_->style_content_.get_style_chart_properties(); 

	return NULL;
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

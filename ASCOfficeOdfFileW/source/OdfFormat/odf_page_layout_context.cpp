#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "odf_page_layout_context.h" 

#include "odf_style_context.h"
#include "ods_conversion_context.h"

#include "styles.h"
#include "style_page_layout_properties.h" 


namespace cpdoccore {
namespace odf {

odf_page_layout_context::odf_page_layout_context(odf_conversion_context * Context)
{
	odf_context_	= Context;
	style_context_	= Context->styles_context();   

	local_style_context_ =  boost::shared_ptr<odf_style_context>(new odf_style_context());
	local_style_context_->set_odf_context(odf_context_);


	even_and_left_headers_ = true;

	current_page_width_ = 0;
}

odf_page_layout_context::~odf_page_layout_context()
{
}


odf_layout_state & odf_page_layout_context::last_layout()
{
	if (layout_state_list_.size() >0)
		return layout_state_list_.back();
	else
		throw;
}
odf_master_state & odf_page_layout_context::last_master()
{
	if (master_state_list_.size() >0)
		return master_state_list_.back();
	else
		throw;
}

void odf_page_layout_context::start_master_page(std::wstring page_name)
{
	office_element_ptr elm;
	create_element(L"style", L"master-page", elm, odf_context_);

	if (!elm) return;

	master_state_list_.push_back( odf_master_state(elm) ); 
///////////////////////////////////////
	if (page_name.length() <1)page_name =L"MasterPage" + boost::lexical_cast<std::wstring>(master_state_list_.size());
	master_state_list_.back().set_name(page_name);
/////////////////////////

	//default layout
	create_layout_page();
	master_state_list_.back().set_layout_name(layout_state_list_.back().get_name());
}
void odf_page_layout_context::end_master_page()
{
	if (master_state_list_.size() < 1)return;

	bool header=false, f_header = false, l_header = false;
	bool footer=false, f_footer = false, l_footer = false;

	for (long i = 0; i < master_state_list_.back().elements_.size(); i++)
	{
		if (!master_state_list_.back().elements_[i].elm)continue;
		if (master_state_list_.back().elements_[i].elm->get_type() == typeStyleHeader)header = true;
		if (master_state_list_.back().elements_[i].elm->get_type() == typeStyleFooter)footer = true;

		if (master_state_list_.back().elements_[i].elm->get_type() == typeStyleHeaderFirst)f_header = true;
		if (master_state_list_.back().elements_[i].elm->get_type() == typeStyleFooterFirst)f_footer = true;

		if (master_state_list_.back().elements_[i].elm->get_type() == typeStyleHeaderLeft)l_header = true;
		if (master_state_list_.back().elements_[i].elm->get_type() == typeStyleFooterLeft)l_footer = true;
	}
	if (f_header && !f_footer && footer)
	{
		add_footer(2);
		office_element_ptr blank_p_elm;
		create_element(L"text", L"p", blank_p_elm, odf_context_);

		master_state_list_.back().elements_.back().elm->add_child_element(blank_p_elm);
		
		f_footer = true;
	}
	if (!f_header && f_footer && header)
	{
		add_header(2);
		office_element_ptr blank_p_elm;
		create_element(L"text", L"p", blank_p_elm, odf_context_);

		master_state_list_.back().elements_.back().elm->add_child_element(blank_p_elm);

		f_header = true;
	}	
	//Так как лажовый Libra и Apach Оо не воспринимают бланковые колонтитулы только первых страниц - городим велосипед на остальные страницы
	if (!header && (f_header || l_header))
	{
		add_header(0);
		office_element_ptr blank_p_elm;
		create_element(L"text", L"p", blank_p_elm, odf_context_);

		master_state_list_.back().elements_.back().elm->add_child_element(blank_p_elm);
		header = true;
	}
	if (!footer && (f_footer || l_footer))
	{
		add_footer(0);
		office_element_ptr blank_p_elm;
		create_element(L"text", L"p", blank_p_elm, odf_context_);

		master_state_list_.back().elements_.back().elm->add_child_element(blank_p_elm);

		footer = true;
	}


}

void odf_page_layout_context::process_master_styles(office_element_ptr root )
{
	for (long i =0; i < master_state_list_.size(); i++)
	{
		try
		{
			root->add_child_element(master_state_list_[i].get_root());
		}
		catch(...)
		{
			//почему то нет страницы
		}
	}
}

void odf_page_layout_context::process_automatic_for_styles(office_element_ptr root )
{
	//local_style_context_->process_automatic_for_styles(root);
	local_style_context_->process_automatic_styles(root);
}
void odf_page_layout_context::process_office_styles(office_element_ptr root )
{
	local_style_context_->process_office_styles(root);
}

void odf_page_layout_context::set_current_master_page_base()
{
	master_state_list_.insert(master_state_list_.begin(), master_state_list_.back());
	master_state_list_.pop_back();
}

void odf_page_layout_context::set_styles_context(odf_style_context * Context)
{
	style_context_	= Context;       
}

void odf_page_layout_context::create_layout_page()
{
	office_element_ptr elm;
	create_element(L"style", L"page-layout", elm, odf_context_);

	std::wstring odf_name = local_style_context_->find_free_name(style_family::PageLayout);
	if (!elm) return;

	local_style_context_->add_style(elm, true, false, style_family::PageLayout);
	
	layout_state_list_.push_back( odf_layout_state(elm) ); 
///////////////////////////////////////

	layout_state_list_.back().set_name(odf_name);
}

void odf_page_layout_context::set_page_margin(_CP_OPT(double) top, _CP_OPT(double) left, _CP_OPT(double) bottom, _CP_OPT(double) right, _CP_OPT(double) header, _CP_OPT(double) footer)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (top)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = 
												length(length(*top,length::inch).get_value_unit(length::cm),length::cm);
	if (bottom)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_= 
												length(length(*bottom,length::inch).get_value_unit(length::cm),length::cm);
	if (left)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_ = 
												length(length(*left,length::inch).get_value_unit(length::cm),length::cm);
	if (right)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_ = 
												length(length(*right,length::inch).get_value_unit(length::cm),length::cm);
}
void odf_page_layout_context::set_page_margin(_CP_OPT(length) top, _CP_OPT(length) left, _CP_OPT(length) bottom, _CP_OPT(length) right)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (top)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = 
												length(top->get_value_unit(length::cm),length::cm);
	if (bottom)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_= 
												length(bottom->get_value_unit(length::cm),length::cm);
	if (left)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_ = 
												length(left->get_value_unit(length::cm),length::cm);
	if (right)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_ = 
												length(right->get_value_unit(length::cm),length::cm);
}
void odf_page_layout_context::set_page_gutter(_CP_OPT(length) length_)
{
	if (!length_) return;
	
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	//if (props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_)
	//{
	//	props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_= 
	//		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_->get_length() + length(length_->get_value_unit(length::cm),length::cm);
	//}
	//else
	//	props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_= length(length_->get_value_unit(length::cm),length::cm);

}
void odf_page_layout_context::set_footer_size(_CP_OPT(length) length_)//тут собственно не footer а размер после колонтитула
{
	if (layout_state_list_.size() < 1) return;

	layout_state_list_.back().footer_size_ = length_;
	//собственно в layout встроим позднее - по факту наличия хоть одного колонтитула
	return;
}
void odf_page_layout_context::set_header_size(_CP_OPT(length) length_)
{
	if (layout_state_list_.size() < 1) return;

	layout_state_list_.back().header_size_ = length_;

	return;
}

void odf_page_layout_context::set_background(_CP_OPT(color) & color, int type)
{
	if (!color) return;

	if (type == 1)
	{
		style_page_layout_properties * props = get_properties();
		if (!props)return;

		props->style_page_layout_properties_attlist_.common_background_color_attlist_.fo_background_color_ = color;
	}
	if (type == 2)
	{
		style_header_footer_properties * props = get_header_properties();
		if (!props)return;

		props->style_header_footer_properties_attlist_.common_background_color_attlist_.fo_background_color_ = color;
	}
	if (type == 3)
	{
		style_header_footer_properties * props = get_footer_properties();
		if (!props)return;

		props->style_header_footer_properties_attlist_.common_background_color_attlist_.fo_background_color_ = color;
	}
}

///////////////////////////////////////////////////////////////
bool odf_page_layout_context::add_footer(int type)
{
	office_element_ptr elm;
	
	if (type == 1) 
	{
		if (even_and_left_headers_)create_element(L"style", L"footer-left", elm, odf_context_);
	}
	else if (type == 2)
		create_element(L"style", L"footer-first", elm, odf_context_);
	else
		create_element(L"style", L"footer", elm, odf_context_);

	if (!elm) return false;
	
	master_state_list_.back().add_footer(elm);

/////////////////////////////////////////////////////////////////////
//настраить нужно 1 раз
	if (!layout_state_list_.back().footer_size_) return true;

	style_header_footer_properties * footer_props = get_footer_properties();
	if (!footer_props)return true;
	style_page_layout_properties * props = get_properties();
	if (!props)return true;

	length length_ = length(layout_state_list_.back().footer_size_->get_value_unit(length::cm),length::cm);

	_CP_OPT(length) bottom_;
	
	if (props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_)
		bottom_= props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_->get_length();

	if (bottom_)
	{
		double length_cm = bottom_->get_value_unit(length::cm) - length_.get_value_unit(length::cm);
	
		if (length_cm > 0.01)
		{
			props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_ = length_;
			footer_props->style_header_footer_properties_attlist_.svg_height_ = length(abs(length_cm),length::cm);//fo_min_height_
			footer_props->style_header_footer_properties_attlist_.fo_min_height_ = length(abs(length_cm),length::cm);
		}
		else if (-length_cm >0.01)
		{
			footer_props->style_header_footer_properties_attlist_.svg_height_ = length(-length_cm,length::cm);//fo_min_height_
		}
	}else
	{
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_ = length_;
	}
	layout_state_list_.back().footer_size_ = boost::none;

	return true;
}
bool odf_page_layout_context::add_header(int type)
{
	office_element_ptr elm;

	if (type == 1)
	{
		if (even_and_left_headers_)create_element(L"style", L"header-left", elm, odf_context_);
	}
	else if (type == 2)
		create_element(L"style", L"header-first", elm, odf_context_);
	else
		create_element(L"style", L"header", elm, odf_context_);

	if (!elm)return false;
	
	master_state_list_.back().add_header(elm);
////////////////////////////////////////////////////////////////////////
//настроить нужно один раз
	if (!layout_state_list_.back().header_size_) return true;
	
	style_header_footer_properties * header_props = get_header_properties();
	if (!header_props)return true;
	style_page_layout_properties * props = get_properties();
	if (!props)return true;

	length length_ = length(layout_state_list_.back().header_size_->get_value_unit(length::cm),length::cm);

	_CP_OPT(length) top_;

	if (props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_)
		top_ = props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_->get_length();;

	if (top_)
	{
		double length_cm = top_->get_value_unit(length::cm) - length_.get_value_unit(length::cm);
		if (length_cm > 0.01)
		{
			props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = length_;
			header_props->style_header_footer_properties_attlist_.svg_height_ = length(abs(length_cm),length::cm);
			header_props->style_header_footer_properties_attlist_.fo_min_height_ = length(abs(length_cm),length::cm);//fo_min_height_
		}
		else if (-length_cm >0.01)
		{
			header_props->style_header_footer_properties_attlist_.svg_height_ = length(-length_cm,length::cm);
			header_props->style_header_footer_properties_attlist_.fo_min_height_ = length(-length_cm,length::cm);//fo_min_height_
		}	
	}
	else
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = length_;
	
	layout_state_list_.back().header_size_ = boost::none;
	return true;
}

void odf_page_layout_context::set_page_border_padding_bottom(int offset_type, double length_pt)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	length length_ = length(length(length_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (offset_type == 2 && props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_)
	{
		length new_margin = length_;
		length_ = props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_->get_length() - length_;
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_ = new_margin;
	}

	props->style_page_layout_properties_attlist_.common_padding_attlist_.fo_padding_bottom_ = length_;
}


void odf_page_layout_context::set_page_border_padding_top(int offset_type, double length_pt)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	length length_ = length(length(length_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (offset_type == 2 && props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_)
	{
		length new_margin = length_;
		length_ = props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_->get_length() - length_;
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = new_margin;
	}

	props->style_page_layout_properties_attlist_.common_padding_attlist_.fo_padding_top_ = length_;
}


void odf_page_layout_context::set_page_border_padding_left(int offset_type, double length_pt)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;
	
	length length_ = length(length(length_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (offset_type == 2 && props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_)
	{
		length new_margin = length_;
		length_ = props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_->get_length() - length_;
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_ = new_margin;
	}

	props->style_page_layout_properties_attlist_.common_padding_attlist_.fo_padding_left_ = length_;
}


void odf_page_layout_context::set_page_border_padding_right(int offset_type, double length_pt)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;
	
	length length_ = length(length(length_pt,length::pt).get_value_unit(length::cm),length::cm);

	if (offset_type == 2 && props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_)
	{
		length new_margin = length_;
		length_ = props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_->get_length() - length_;
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_ = new_margin;
	}

	props->style_page_layout_properties_attlist_.common_padding_attlist_.fo_padding_right_ = length_;
}

void odf_page_layout_context::set_page_border_shadow(bool val)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	props->style_page_layout_properties_attlist_.common_shadow_attlist_.style_shadow_ = L"#000000 0.159cm 0.159cm";
}
void odf_page_layout_context::set_page_border(std::wstring top, std::wstring left, std::wstring bottom, std::wstring right)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_ = left;
	}
	else
	{
		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_bottom_	= 
		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_top_		= 
		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_left_		= 
		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_right_	= props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_;

		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() >0 )props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0 )	props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_right_	= right;
		
	}
}
void odf_page_layout_context::set_page_size(_CP_OPT(length) width, _CP_OPT(length) height)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (width)
	{
		props->style_page_layout_properties_attlist_.fo_page_width_ = 
												length(width->get_value_unit(length::cm),length::cm);
	}
	if (height)
		props->style_page_layout_properties_attlist_.fo_page_height_ = 
												length(height->get_value_unit(length::cm),length::cm);
}

void odf_page_layout_context::set_pages_mirrored(bool val)
{
	//for all

	for (long i=0; i < layout_state_list_.size(); i++)
	{
		layout_state_list_[i].set_pages_mirrored(val);
	}	
}

void odf_page_layout_context::set_even_and_left_headers(bool val)
{
	even_and_left_headers_ = val;
}

void odf_page_layout_context::set_title_page_enable(bool val)
{
}

style_page_layout_properties * odf_page_layout_context::get_properties()
{
	if (layout_state_list_.size() < 1) return NULL;
	style_page_layout_properties * props = layout_state_list_.back().get_properties();
	if (props == NULL)
	{
		office_element_ptr elm;
		create_element(L"style", L"page-layout-properties", elm, odf_context_);
		layout_state_list_.back().add_child(elm, office_element_ptr(),L"");
		props = layout_state_list_.back().get_properties();
	}
	return props;
}
style_header_footer_properties *odf_page_layout_context::get_header_properties()
{
	if (layout_state_list_.size() < 1) return NULL;
	style_header_footer_properties *props = layout_state_list_.back().get_header_properties();
	if (props == NULL)
	{
		office_element_ptr elm;
		create_element(L"style", L"header-style", elm, odf_context_);
		layout_state_list_.back().add_child(elm, office_element_ptr(),L"");
		
		office_element_ptr pr;
		create_element(L"style", L"header-footer-properties", pr, odf_context_);
		elm->add_child_element(pr);

		props = layout_state_list_.back().get_header_properties();
	}
	return props;
}

style_header_footer_properties *odf_page_layout_context::get_footer_properties()
{
	if (layout_state_list_.size() < 1) return NULL;
	style_header_footer_properties *props = layout_state_list_.back().get_footer_properties();
	if (props == NULL)
	{
		office_element_ptr elm;
		create_element(L"style", L"footer-style", elm, odf_context_);
			layout_state_list_.back().add_child(elm, office_element_ptr(),L"");

		office_element_ptr pr;
		create_element(L"style", L"header-footer-properties", pr, odf_context_);
		elm->add_child_element(pr);

		props = layout_state_list_.back().get_footer_properties();
	}
	return props;
}

////////////////////////////////////////////////////////////////////////////////////////
void odf_page_layout_context::set_page_orientation(int type)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (type == 0)
		props->style_page_layout_properties_attlist_.style_print_orientation_ = L"landscape";
	else
		props->style_page_layout_properties_attlist_.style_print_orientation_ = L"portrait";

}

}
}
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

#include <iostream>

#include "odf_controls_context.h"
#include "odf_conversion_context.h"

#include "office_forms.h"


namespace cpdoccore {
namespace odf_writer {

struct 	odf_control_state
{
	office_element_ptr	elm;
	
	_CP_OPT(double) cx;
	_CP_OPT(double) cy;
	
	std::wstring name;
	std::wstring description;
	
	_CP_OPT(unsigned int) fill_color;

	std::vector<office_element_ptr> properties;
	std::vector<office_element_ptr> events;
};

class odf_controls_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) : odf_context_(odf_context)
    {	
	} 
	bool start_control(int type);
	
	std::vector<odf_control_state> controls_;

	odf_conversion_context* odf_context_;
};

//-----------------------------------------------------------------------------------------------
odf_controls_context::odf_controls_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_controls_context::Impl(odf_context))
{
}
bool odf_controls_context::Impl::start_control(int type)
{
	office_element_ptr element;

	std::wstring control_implementation;
	switch(type)
	{
		case 0: // objectButton
		{
			create_element(L"form", L"button", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.CommandButton";
		}break;
		case 1: // objectCheckBox
		{
			create_element(L"form", L"checkbox", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.CheckBox";
		}break;
		case 2: // objectDrop
		{
			create_element(L"form", L"combobox", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.ComboBox";
		}break;
		case 3: // objectGBox
		{
			create_element(L"form", L"frame", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.GroupBox";
		}break;
		case 4: // objectLabel
		{
			create_element(L"form", L"fixed-text", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.FixedText";
		}break;
		case 5: // objectList
		{
			create_element(L"form", L"listbox", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.ListBox";
		}break;
		case 6: // objectRadio
		{
			create_element(L"form", L"radio", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.RadioButton";
		}break;
		case 7: // objectScroll
		{
			create_element(L"form", L"value-range", element, odf_context_);
			control_implementation = L"com.sun.star.awt.UnoControlScrollBar";
		}break;
		case 8: // objectSpin
		{
			create_element(L"form", L"value-range", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.SpinButton";
		}break;
		case 9: // objectEditBox
		{
			create_element(L"form", L"textarea", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.TextField";
		}break;
		case 10: // objectDialog
		{
		}break;
	}
	odf_writer::form_element* form_element = dynamic_cast<odf_writer::form_element*>(element.get());

	if (!form_element) return false;

	form_element->control_implementation_ = control_implementation;

	odf_control_state state;
	state.elm = element;

	controls_.push_back(state);
	return true;
}

odf_controls_context::~odf_controls_context()
{
}
bool odf_controls_context::start_control(int type)
{
	return impl_->start_control(type);
}
void odf_controls_context::end_control()
{
}

void odf_controls_context::set_name(const std::wstring & name)
{
	if (name.empty()) return;
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().name = name;
}
void odf_controls_context::set_description (const std::wstring & description)
{
	if (description.empty()) return;
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().description = description;
}

void odf_controls_context::set_fill_color(unsigned int Color)
{
	impl_->controls_.back().fill_color = Color;
}
//void odf_controls_context::set_solid_fill(std::wstring hexColor)
//{
//	if (!impl_->current_graphic_properties)	return;
//	if (hexColor.empty()) return;
//	
//	size_t res = hexColor.find(L"#");
//	if (std::wstring::npos == res)
//		hexColor = std::wstring(L"#") + hexColor;
//
//	switch(impl_->current_drawing_part_)
//	{
//		case Area:
//			impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_color_ = hexColor;
//			//impl_->current_graphic_properties->common_background_color_attlist_.fo_background_color_	= color(hexColor); - default transparent
//			//последнее нужно - что если будут вводить текст - под текстом будет цвет фона (или он поменяется в полях текста)
//			
//			if ((/*impl_->is_footer_ || impl_->is_header_ ||*/ impl_->is_background_) && 
//				(impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_) && 
//				(impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_->get_type() == draw_fill::bitmap))
//			{
//			}
//			else
//				impl_->current_graphic_properties->common_draw_fill_attlist_.draw_fill_ = draw_fill::solid;
//			break;
//		case Line:
//			impl_->current_graphic_properties->svg_stroke_color_ =  hexColor;
//			if (!impl_->current_graphic_properties->draw_stroke_)
//				impl_->current_graphic_properties->draw_stroke_ = line_style(line_style::Solid);//default
//			if (!impl_->current_graphic_properties->svg_stroke_width_)
//				impl_->current_graphic_properties->svg_stroke_width_ = length(length(1, length::pt).get_value_unit(length::cm), length::cm);//default
//			break;
//	}
//}

void odf_controls_context::set_size( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt)
{
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().cx = width_pt;
	impl_->controls_.back().cy = height_pt;
}

bool odf_controls_context::is_exist_content()
{
	return (impl_->controls_.empty() ? false : true);
}

void odf_controls_context::finalize(office_element_ptr & root_elm)//для привязки 
{
	for (size_t i = 0; i < impl_->controls_.size(); i++)
	{
		if (false == impl_->controls_[i].properties.empty())
		{
			office_element_ptr elm;
			create_element(L"form", L"properties", elm, impl_->odf_context_);
			
			for (size_t j = 0; j < impl_->controls_[i].properties.size(); j++)
			{
				elm->add_child_element(impl_->controls_[i].properties[j]);
			}
			impl_->controls_[i].elm->add_child_element(elm);
		}
		if (false == impl_->controls_[i].events.empty())
		{
			office_element_ptr elm;
			create_element(L"office", L"event-listeners", elm, impl_->odf_context_);
			
			for (size_t j = 0; j < impl_->controls_[i].events.size(); j++)
			{
				elm->add_child_element(impl_->controls_[i].events[j]);
			}
			impl_->controls_[i].elm->add_child_element(elm);
		}
		root_elm->add_child_element(impl_->controls_[i].elm);
	}	
}
}
}

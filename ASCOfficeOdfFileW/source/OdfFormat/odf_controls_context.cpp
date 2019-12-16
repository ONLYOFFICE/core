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

#include "../../../ASCOfficeOdfFile/formulasconvert/formulasconvert.h"

namespace cpdoccore {
namespace odf_writer {

struct 	odf_control_state
{
	office_element_ptr	elm;
	odf_writer::form_element* form_elm;
	
	_CP_OPT(double) cx;
	_CP_OPT(double) cy;
	
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
	std::wstring start_control(int type, bool items_set);
	
	std::vector<odf_control_state> controls_;

	odf_conversion_context* odf_context_;
};

//-----------------------------------------------------------------------------------------------
odf_controls_context::odf_controls_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_controls_context::Impl(odf_context))
{
}
std::wstring odf_controls_context::Impl::start_control(int type, bool items_set)
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
			if (items_set)
			{
				create_element(L"form", L"combobox", element, odf_context_);
				control_implementation = L"ooo:com.sun.star.form.component.ComboBox";
			}
			else
			{
				create_element(L"form", L"listbox", element, odf_context_);
				control_implementation = L"ooo:com.sun.star.form.component.ListBox";
			}
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
			control_implementation = L"ooo:com.sun.star.form.component.ScrollBar";
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
		case 11: // date
		{
			create_element(L"form", L"date", element, odf_context_);
			control_implementation = L"ooo:com.sun.star.form.component.DateField";
		}break;
	}
	odf_writer::form_element* form_element = dynamic_cast<odf_writer::form_element*>(element.get());

	if (!form_element) return L"";

	odf_context_->mediaitems()->count_control += 1;
	form_element->id_ = L"control" + std::to_wstring(odf_context_->mediaitems()->count_control);
	form_element->control_implementation_ = control_implementation;

	odf_control_state state;
	state.elm = element;
	state.form_elm = form_element;

	controls_.push_back(state);
	return (*form_element->id_);
}

odf_controls_context::~odf_controls_context()
{
}
std::wstring odf_controls_context::start_control(int type, bool items_set)
{
	return impl_->start_control(type, items_set);
}
void odf_controls_context::end_control()
{
}

void odf_controls_context::set_name(const std::wstring & val)
{
	if (val.empty()) return;
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().form_elm->name_ = val;
}
void odf_controls_context::set_label(const std::wstring & val)
{
	if (val.empty()) return;
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().form_elm->label_ = val;
}
void odf_controls_context::set_linkedCell (const std::wstring & val)
{
	if (val.empty()) return;
	if (impl_->controls_.empty()) return;

	formulasconvert::oox2odf_converter formulas_converter;
	impl_->controls_.back().form_elm->linked_cell_ = formulas_converter.convert_named_ref(val);
}
void odf_controls_context::set_listFillRange (const std::wstring & val)
{
	if (val.empty()) return;
	if (impl_->controls_.empty()) return;

	odf_writer::form_listbox *listbox = dynamic_cast<odf_writer::form_listbox*>(impl_->controls_.back().form_elm);
	if (!listbox) return;

	formulasconvert::oox2odf_converter formulas_converter;
	listbox->source_cell_range_ = formulas_converter.convert_named_ref(val);
}
void odf_controls_context::set_drop_size(int val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_listbox *listbox = dynamic_cast<odf_writer::form_listbox*>(impl_->controls_.back().form_elm);
	if (listbox) listbox->size_ = val;
	
	odf_writer::form_combobox *combobox  = dynamic_cast<odf_writer::form_combobox*>(impl_->controls_.back().form_elm);
	if (combobox ) combobox ->size_ = val;
}
void odf_controls_context::set_macro(const std::wstring & val)
{
	if (val.empty()) return;
	if (impl_->controls_.empty()) return;

	//impl_->controls_.back().form_elm-> = val;
}
void odf_controls_context::set_disabled	(bool val)
{
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().form_elm->disabled_ = val;
}
void odf_controls_context::set_printable(bool val)
{
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().form_elm->printable_ = val;
}
void odf_controls_context::set_textHAlign(int val)
{
	if (impl_->controls_.empty()) return;
}
void odf_controls_context::set_textVAlign(int val)
{
	if (impl_->controls_.empty()) return;
}
void odf_controls_context::set_min_value(int val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_value_range *value_range = dynamic_cast<odf_writer::form_value_range*>(impl_->controls_.back().form_elm);
	if (!value_range) return;

	value_range->min_value_ = val;
}
void odf_controls_context::set_max_value(int val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_value_range *value_range = dynamic_cast<odf_writer::form_value_range*>(impl_->controls_.back().form_elm);
	if (!value_range) return;

	value_range->max_value_ = val;
}
void odf_controls_context::set_page_step(int val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_value_range *value_range = dynamic_cast<odf_writer::form_value_range*>(impl_->controls_.back().form_elm);
	if (!value_range) return;

	value_range->page_step_size_ = val;
}
void odf_controls_context::set_step(int val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_value_range *value_range = dynamic_cast<odf_writer::form_value_range*>(impl_->controls_.back().form_elm);
	if (!value_range) return;

	value_range->step_size_ = val;
}
void odf_controls_context::set_horiz(bool val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_value_range *value_range = dynamic_cast<odf_writer::form_value_range*>(impl_->controls_.back().form_elm);
	if (!value_range) return;

	value_range->orientation_ = val ? odf_types::table_centering::Horizontal : odf_types::table_centering::Vertical;
}
void odf_controls_context::set_drop_down(bool val)
{
	if (impl_->controls_.empty()) return;
	
	impl_->controls_.back().form_elm->dropdown_ = val;
}
void odf_controls_context::set_check_state(int val)
{
	if (impl_->controls_.empty()) return;
	
	odf_writer::form_checkbox *checkbox = dynamic_cast<odf_writer::form_checkbox*>(impl_->controls_.back().form_elm);
	
	if (checkbox)
		checkbox->current_state_ = (val == 1);
}
void odf_controls_context::set_value(const std::wstring & val)
{
	if (impl_->controls_.empty()) return;

	impl_->controls_.back().form_elm->value_ = val;
}
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
	if (impl_->controls_.empty()) return;

	office_element_ptr elm_form;
	create_element(L"form", L"form", elm_form, impl_->odf_context_);

	odf_writer::form_form* form = dynamic_cast<odf_writer::form_form*>(elm_form.get());

	form->control_implementation_ = L"ooo:com.sun.star.form.component.Form";
	form->name_ = L"Form";
	form->apply_filter_ = true;
	form->command_type_ = odf_types::command_type::table;
	
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
		elm_form->add_child_element(impl_->controls_[i].elm);
	}	
	root_elm->add_child_element(elm_form);
}
}
}

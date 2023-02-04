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

#include "office_forms.h"
#include "draw_frame.h"
//#include "text_content.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/utils.h>

#include "../../Formulas/formulasconvert.h"

namespace cpdoccore { 
namespace odf_writer {

// office:forms
//----------------------------------------------------------------------------------
const wchar_t * office_forms::ns = L"office";
const wchar_t * office_forms::name = L"forms";

void office_forms::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"form:apply-design-mode", apply_design_mode_);
			CP_XML_ATTR_OPT(L"form:automatic-focus", automatic_focus_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void office_forms::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void office_forms::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// form:form
//----------------------------------------------------------------------------------
const wchar_t * form_form::ns = L"form";
const wchar_t * form_form::name = L"form";

void form_form::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"form:allow-deletes",		allow_deletes_);
			CP_XML_ATTR_OPT(L"form:allow-inserts",		allow_inserts_);
			CP_XML_ATTR_OPT(L"form:allow-updates",		allow_updates_);
			CP_XML_ATTR_OPT(L"form:apply-filter",		apply_filter_);
			CP_XML_ATTR_OPT(L"form:command",			command_); 
			CP_XML_ATTR_OPT(L"form:command-type",		command_type_);
			CP_XML_ATTR_OPT(L"form:control-implementation", control_implementation_); 
			CP_XML_ATTR_OPT(L"form:datasource",			datasource_);
			CP_XML_ATTR_OPT(L"form:detail-fields",		detail_fields_);
			CP_XML_ATTR_OPT(L"form:enctype",			enctype_); 
			CP_XML_ATTR_OPT(L"form:escape-processing",	escape_processing_);
			CP_XML_ATTR_OPT(L"form:filter",				filter_);
			CP_XML_ATTR_OPT(L"form:ignore-result",		ignore_result_);
			CP_XML_ATTR_OPT(L"form:master-fields",		master_fields_); 
			CP_XML_ATTR_OPT(L"form:method",				method_);
			CP_XML_ATTR_OPT(L"form:name",				name_);
			CP_XML_ATTR_OPT(L"form:navigation-mode",	navigation_mode_);
			CP_XML_ATTR_OPT(L"form:order",				order_);
			CP_XML_ATTR_OPT(L"form:tabcycle",			tabcycle_); 

			CP_XML_ATTR_OPT(L"office:target-frame",		office_target_frame_);
			
			xlink_attlist_.serialize(CP_GET_XML_NODE());
			
			if (properties_)
			{
				properties_->serialize(CP_XML_STREAM());
			}
			if (office_event_listeners_)
			{
				office_event_listeners_->serialize(CP_XML_STREAM());
			}
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}


void form_form::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"form", L"properties")
    {
        CP_CREATE_ELEMENT(properties_);                    
    }
    else if CP_CHECK_NAME(L"office", L"event-listeners")
    {
        CP_CREATE_ELEMENT(office_event_listeners_);                    
    }
	else 
		CP_CREATE_ELEMENT(content_);
}
void form_form::add_child_element( const office_element_ptr & child_element)
{
	switch(child_element->get_type())
	{
		case typeFormProperties:
			properties_ = child_element;
			break;
		case typeOfficeEventListeners:
			office_event_listeners_ = child_element;
			break;
		default:
			content_.push_back(child_element);
			break;
	}
}
// form:properties
//----------------------------------------------------------------------------------
const wchar_t * form_properties::ns = L"form";
const wchar_t * form_properties::name = L"properties";

void form_properties::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void form_properties::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void form_properties::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// form:property
//----------------------------------------------------------------------------------
const wchar_t * form_property::ns = L"form";
const wchar_t * form_property::name = L"property";

void form_property::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR_OPT(L"form:property-name",	property_name_);
			value_and_type_.serialize(CP_GET_XML_NODE());
		}
	}
}

// form:list-property
//----------------------------------------------------------------------------------
const wchar_t * form_list_property::ns = L"form";
const wchar_t * form_list_property::name = L"list-property";

void form_list_property::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR_OPT(L"form:property-name",	property_name_);
			CP_XML_ATTR_OPT(L"form:value-type",		value_type_);
		}
	}
}
void form_list_property::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void form_list_property::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// form:list-value
//----------------------------------------------------------------------------------
const wchar_t * form_list_value::ns = L"form";
const wchar_t * form_list_value::name = L"list-value";

void form_list_value::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			value_and_type_.serialize(CP_GET_XML_NODE());
		}
	}
}

//----------------------------------------------------------------------------------
const wchar_t * form_element::ns = L"form";
const wchar_t * form_element::name = L"element";

void form_element::serialize_attlist(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"form:name", name_);
	CP_XML_ATTR_OPT(L"form:control-implementation",	control_implementation_);
	CP_XML_ATTR_OPT(L"form:data-field", data_field_);
	CP_XML_ATTR_OPT(L"form:linked-cell", linked_cell_);
	CP_XML_ATTR_OPT(L"form:disabled", disabled_);
	CP_XML_ATTR_OPT(L"form:id", id_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"form:label", label_);
	CP_XML_ATTR_OPT(L"form:printable", printable_);
	CP_XML_ATTR_OPT(L"form:tab-index", tab_index_);
	CP_XML_ATTR_OPT(L"form:tab-stop", tab_stop_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"form:title", title_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"form:value", value_);
	CP_XML_ATTR_OPT_ENCODE_STRING(L"form:current-value", current_value_);
	CP_XML_ATTR_OPT(L"form:dropdown", dropdown_);
	CP_XML_ATTR_OPT(L"xml:id", xml_id_);
	CP_XML_ATTR_OPT(L"xforms:bind", xforms_bind_);
}
void form_element::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"form", L"properties")
    {
        CP_CREATE_ELEMENT(properties_);                    
    }
    else if CP_CHECK_NAME(L"office", L"event-listeners")
    {
        CP_CREATE_ELEMENT(office_event_listeners_);                    
    }
}
void form_element::add_child_element( const office_element_ptr & child_element)
{
	switch(child_element->get_type())
	{
		case typeFormProperties:
			properties_ = child_element;
			break;
		case typeOfficeEventListeners:
			office_event_listeners_ = child_element;
			break;
		default:
			break;
	}
}
// form:button
//----------------------------------------------------------------------------------
const wchar_t * form_button::ns = L"form";
const wchar_t * form_button::name = L"button";

void form_button::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			form_element::serialize_attlist(CP_GET_XML_NODE());
		
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:image-frame 
//----------------------------------------------------------------------------------
const wchar_t * form_image_frame::ns = L"form";
const wchar_t * form_image_frame::name = L"image-frame";

void form_image_frame::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			form_element::serialize_attlist(CP_GET_XML_NODE());
			CP_XML_ATTR_OPT(L"form:image-data", image_data_);
		
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:text
//----------------------------------------------------------------------------------
const wchar_t * form_text::ns = L"form";
const wchar_t * form_text::name = L"text";

void form_text::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:textarea
//----------------------------------------------------------------------------------
const wchar_t * form_textarea::ns = L"form";
const wchar_t * form_textarea::name = L"textarea";

void form_textarea::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:fixed-text
//----------------------------------------------------------------------------------
const wchar_t * form_fixed_text::ns = L"form";
const wchar_t * form_fixed_text::name = L"fixed-text";

void form_fixed_text::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
//----------------------------------------------------------------------------------
// form:radio
//----------------------------------------------------------------------------------
const wchar_t * form_radio::ns = L"form";
const wchar_t * form_radio::name = L"radio";

void form_radio::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			if (current_state_)
			{
				CP_XML_ATTR(L"form:selected", current_state_ ? L"true" : L"false");			
				CP_XML_ATTR(L"form:current-selected", current_state_ ? L"true" : L"false");			
			}
			
			CP_XML_ATTR(L"form:visual-effect", L"flat");			
			CP_XML_ATTR(L"form:input-required", L"false");			
			CP_XML_ATTR(L"form:group-name", L"autoGroup_formControl");			

			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
//----------------------------------------------------------------------------------
// form:checkbox
//----------------------------------------------------------------------------------
const wchar_t * form_checkbox::ns = L"form";
const wchar_t * form_checkbox::name = L"checkbox";

void form_checkbox::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			if (current_state_)
			{
				CP_XML_ATTR(L"form:current-state", L"checked" );			
			}
			CP_XML_ATTR(L"form:image-position", image_position_);
			
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
//----------------------------------------------------------------------------------
// form:combobox
//----------------------------------------------------------------------------------
const wchar_t * form_combobox::ns = L"form";
const wchar_t * form_combobox::name = L"combobox";

void form_combobox::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			CP_XML_ATTR_OPT(L"form:source-cell-range",	source_cell_range_);
			CP_XML_ATTR_OPT(L"form:list-source",		list_source_);

			for (size_t i = 0; i < items_.size(); i++)
				items_[i]->serialize(CP_XML_STREAM());
			
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
void form_combobox::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"form", L"item")
    {
        CP_CREATE_ELEMENT(items_);                    
    }
	else
	{
		form_element::create_child_element(Ns, Name);
	}
}
void form_combobox::add_child_element( const office_element_ptr & child_element)
{
	switch(child_element->get_type())
	{
		case typeFormItem:
			items_.push_back(child_element);
			break;
		default:
			form_element::add_child_element(child_element);
			break;
	}
}
// form:listbox
//----------------------------------------------------------------------------------
const wchar_t * form_listbox::ns = L"form";
const wchar_t * form_listbox::name = L"listbox";

void form_listbox::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"form", L"option")
    {
        CP_CREATE_ELEMENT(options_);                    
    }
	else
	{
		form_element::create_child_element(Ns, Name);
	}
}
void form_listbox::add_child_element( const office_element_ptr & child_element)
{
	switch(child_element->get_type())
	{
		case typeFormOption:
			options_.push_back(child_element);
			break;
		default:
			form_element::add_child_element(child_element);
			break;
	}
}
void form_listbox::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			CP_XML_ATTR_OPT(L"form:source-cell-range",	source_cell_range_);
			CP_XML_ATTR_OPT(L"form:list-source",		list_source_);
			CP_XML_ATTR_OPT(L"form:list-source-type",	list_source_type_);
			CP_XML_ATTR_OPT(L"form:list-linkage-type",	list_linkage_type_);

			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:button
//----------------------------------------------------------------------------------
const wchar_t * form_date::ns = L"form";
const wchar_t * form_date::name = L"date";

void form_date::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:value-range
//----------------------------------------------------------------------------------
const wchar_t * form_value_range::ns = L"form";
const wchar_t * form_value_range::name = L"value-range";

void form_value_range::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			form_element::serialize_attlist(CP_GET_XML_NODE());
			
			CP_XML_ATTR_OPT	(L"form:min-value",			min_value_);
			CP_XML_ATTR_OPT	(L"form:max-value",			max_value_);
			CP_XML_ATTR_OPT	(L"form:step-size",			step_size_);
			CP_XML_ATTR_OPT	(L"form:page-step-size",	page_step_size_);
			CP_XML_ATTR_OPT	(L"form:orientation",		orientation_);
			CP_XML_ATTR		(L"form:delay-for-repeat",	delay_for_repeat_);
		
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:frame
//----------------------------------------------------------------------------------
const wchar_t * form_frame::ns = L"form";
const wchar_t * form_frame::name = L"frame";

void form_frame::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			form_element::serialize_attlist(CP_GET_XML_NODE());
						
			if (properties_) properties_->serialize(CP_XML_STREAM());
			if (office_event_listeners_) office_event_listeners_->serialize(CP_XML_STREAM());
		}
	}
}
// form:item
//----------------------------------------------------------------------------------
const wchar_t * form_item::ns = L"form";
const wchar_t * form_item::name = L"item";

void form_item::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"form:label", label_);
		}
	}
}

}
}

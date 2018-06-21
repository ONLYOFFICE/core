/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "text_content.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/utils.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

// office:forms
//----------------------------------------------------------------------------------
const wchar_t * office_forms::ns = L"office";
const wchar_t * office_forms::name = L"forms";

void office_forms::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"form:apply-design-mode", apply_design_mode_);
    CP_APPLY_ATTR(L"form:automatic-focus", automatic_focus_);
}

void office_forms::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void office_forms::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
}
// form:form
//----------------------------------------------------------------------------------
const wchar_t * form_form::ns = L"form";
const wchar_t * form_form::name = L"form";

void form_form::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"form:allow-deletes",	allow_deletes_);
	CP_APPLY_ATTR(L"form:allow-inserts",	allow_inserts_);
	CP_APPLY_ATTR(L"form:allow-updates",	allow_updates_);
	CP_APPLY_ATTR(L"form:apply-filter",		apply_filter_);
	CP_APPLY_ATTR(L"form:command",			command_); 
	CP_APPLY_ATTR(L"form:command_type",		command_type_);
	CP_APPLY_ATTR(L"form:control-implementation", control_implementation_); 
	CP_APPLY_ATTR(L"form:datasource",		datasource_);
	CP_APPLY_ATTR(L"form:detail-fields",	detail_fields_);
	CP_APPLY_ATTR(L"form:enctype",			enctype_); 
	CP_APPLY_ATTR(L"form:escape-processing",escape_processing_);
	CP_APPLY_ATTR(L"form:filter",			filter_);
	CP_APPLY_ATTR(L"form:ignore-result",	ignore_result_);
	CP_APPLY_ATTR(L"form:master-fields",	master_fields_); 
	CP_APPLY_ATTR(L"form:method",			method_);
	CP_APPLY_ATTR(L"form:name",				name_);
	CP_APPLY_ATTR(L"form:navigation-mode",	navigation_mode_);
	CP_APPLY_ATTR(L"form:order",			order_);
	CP_APPLY_ATTR(L"form:tabcycle",			tabcycle_); 

	CP_APPLY_ATTR(L"office:target-frame",	office_target_frame_);
	
	xlink_attlist_.add_attributes(Attributes);
}

void form_form::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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
void form_form::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
}
// form:properties
//----------------------------------------------------------------------------------
const wchar_t * form_properties::ns = L"form";
const wchar_t * form_properties::name = L"properties";

void form_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void form_properties::docx_convert(oox::docx_conversion_context & Context)
{
	//for (size_t i = 0; i < content_.size(); i++)
 //   {
 //       content_[i]->docx_convert(Context);
 //   }
}
// form:property
//----------------------------------------------------------------------------------
const wchar_t * form_property::ns = L"form";
const wchar_t * form_property::name = L"property";

void form_property::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"form:property-name",	property_name_);
	value_and_type_.add_attributes(Attributes);
}

// form:list-property
//----------------------------------------------------------------------------------
const wchar_t * form_list_property::ns = L"form";
const wchar_t * form_list_property::name = L"list-property";

void form_list_property::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"form:property-name",	property_name_);
	CP_APPLY_ATTR(L"form:value-type",	value_type_);
}
void form_list_property::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void form_list_property::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
}
// form:list-value
//----------------------------------------------------------------------------------
const wchar_t * form_list_value::ns = L"form";
const wchar_t * form_list_value::name = L"list-value";

void form_list_value::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	value_and_type_.add_attributes(Attributes);
}

//----------------------------------------------------------------------------------
const wchar_t * form_element::ns = L"form";
const wchar_t * form_element::name = L"element";

void form_element::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"form:control_implementation",	control_implementation_);
	CP_APPLY_ATTR(L"form:data-field",	data_field_);
	CP_APPLY_ATTR(L"form:linked-cell",	linked_cell_);
	CP_APPLY_ATTR(L"form:disabled",		disabled_);
	CP_APPLY_ATTR(L"form:id",			id_);
	CP_APPLY_ATTR(L"form:label",		label_);
	CP_APPLY_ATTR(L"form:name",			name_);
	CP_APPLY_ATTR(L"form:printable",	printable_);
	CP_APPLY_ATTR(L"form:tab-index",	tab_index_);
	CP_APPLY_ATTR(L"form:tab-stop",		tab_stop_);
	CP_APPLY_ATTR(L"form:title",		title_);
	CP_APPLY_ATTR(L"form:value",		value_);
	CP_APPLY_ATTR(L"form:current-value",current_value_);
	CP_APPLY_ATTR(L"xml:id",			xml_id_);
	CP_APPLY_ATTR(L"xforms:bind",		xforms_bind_);
}
void form_element::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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
void form_element::docx_convert(oox::docx_conversion_context & Context)
{
	std::wstring id = id_ ? *id_ : (xml_id_ ? *xml_id_ : L"");
	Context.get_forms_context().set_id(id);

	if (name_)						Context.get_forms_context().set_name(*name_);
	if (label_)						Context.get_forms_context().set_label(*label_);
	
	if (current_value_)				Context.get_forms_context().set_value(*current_value_);
	else if (value_)				Context.get_forms_context().set_value(*value_);
	
	if (control_implementation_)	Context.get_forms_context().set_uuid(*control_implementation_);

	Context.get_forms_context().end_element();

}
// form:button
//----------------------------------------------------------------------------------
const wchar_t * form_button::ns = L"form";
const wchar_t * form_button::name = L"button";

void form_button::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	form_element::add_attributes(Attributes);
}
void form_button::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(1);

	form_element::docx_convert(Context);
}

// form:text
//----------------------------------------------------------------------------------
const wchar_t * form_text::ns = L"form";
const wchar_t * form_text::name = L"text";

void form_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	form_element::add_attributes(Attributes);
}
void form_text::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(2);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_text::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;

	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + xml::utils::replace_text_to_xml(*name_) + L"\"/>";
			}
			Context.output_stream() << L"<w:id w:val=\"" + std::to_wstring(Context.get_drawing_context().get_current_shape_id()) + L"\"/>";
			//<w:lock w:val="sdtLocked"/>
			//<w:placeholder>
			// <w:docPart w:val="DefaultPlaceholder_-1854013440"/>
			//</w:placeholder>
		}
		Context.output_stream() << L"</w:sdtPr>";
		Context.output_stream() << L"<w:sdtContent>";
		{
			Context.add_new_run(L"");
			if (current_value_)
			{
				Context.output_stream() << L"<w:t xml:space=\"preserve\">";
				Context.output_stream() << xml::utils::replace_text_to_xml(*current_value_ );
				Context.output_stream() << L"</w:t>";
			}
			Context.finish_run();
		}
		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>"; 
}
void form_text::docx_convert_field(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;

	XmlUtils::replace_all( *name_, L" ", L"_");

	Context.add_new_run(L"");
		Context.output_stream() << L"<w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << *name_ << L"\"/><w:enabled/>";
		Context.output_stream() << L"</w:ffData></w:fldChar>";
	Context.finish_run();	

	Context.add_new_run(L"");
		Context.output_stream() << L"<w:instrText>FORMTEXT</w:instrText>";
	Context.finish_run();	
		
	Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	
	Context.add_new_run(L"");
		Context.output_stream() << L"<w:t>" << *current_value_ << L"</w:t>";
	Context.finish_run();	
    Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
}
// form:checkbox
//----------------------------------------------------------------------------------
const wchar_t * form_checkbox::ns = L"form";
const wchar_t * form_checkbox::name = L"checkbox";

void form_checkbox::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	_CP_OPT(std::wstring) strVal;
	CP_APPLY_ATTR(L"form:current-state", strVal);
	
	if ((strVal) && (*strVal == L"checked"))
		current_state_ = true;
	else current_state_ = false;

	form_element::add_attributes(Attributes);
}
void form_checkbox::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(3);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_checkbox::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;
	
	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + xml::utils::replace_text_to_xml(*name_) + L"\"/>";
			}
			Context.output_stream() << L"<w:id w:val=\"" + std::to_wstring(Context.get_drawing_context().get_current_shape_id()) + L"\"/>";
			Context.output_stream() << L"<w14:checkbox>";

			Context.output_stream() << L"<w14:checked w14:val=\"" + std::to_wstring(current_state_ ? 1 : 0) + L"\"/>";
			//Context.output_stream() << L"<w14:checkedState w14:val=\"2612\" w14:font=\"MS Gothic\"/>";
			//Context.output_stream() << L"<w14:uncheckedState w14:val=\"2610\" w14:font=\"MS Gothic\"/>";
			Context.output_stream() << L"</w14:checkbox>";
		}
		Context.output_stream() << L"</w:sdtPr>";
		Context.output_stream() << L"<w:sdtContent>";
		{
			Context.add_new_run(L"");
			if (current_state_)
				Context.output_stream() << L"<w:t>☒</w:t>";
			else
				Context.output_stream() << L"<w:t>☐</w:t>";
			Context.finish_run();
		}
		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>";
	
	if (label_)
	{
		Context.add_new_run(L"");
			Context.output_stream() << L"<w:t xml:space=\"preserve\">";
			Context.output_stream() << xml::utils::replace_text_to_xml(*label_ );
			Context.output_stream() << L"</w:t>";
		Context.finish_run();
	}
}

void form_checkbox::docx_convert_field(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;

	XmlUtils::replace_all( *name_, L" ", L"_");

	Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << *name_ << L"\"/><w:enabled/>";
	Context.output_stream() << L"<w:label w:val=\"" << xml::utils::replace_text_to_xml(*label_ ) << L"\"/>";
	Context.output_stream() << L"<w:checkBox><w:default w:val=\"" << std::to_wstring(current_state_) << L"\"/></w:checkBox></w:ffData>";
	Context.output_stream() << L"</w:fldChar></w:r>";
	Context.output_stream() << L"<w:r><w:instrText>FORMCHECKBOX</w:instrText></w:r>";
	//Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	//Context.output_stream() << L"<w:r><w:t>" << xml::utils::replace_text_to_xml(*label_ ) << L"</w:t></w:r>";
    Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
	
	Context.add_new_run(L"");
		Context.output_stream() << L"<w:t xml:space=\"preserve\">";
		Context.output_stream() << xml::utils::replace_text_to_xml(*label_ );
		Context.output_stream() << L"</w:t>";
	Context.finish_run();	
}

// form:combobox
//----------------------------------------------------------------------------------
const wchar_t * form_combobox::ns = L"form";
const wchar_t * form_combobox::name = L"combobox";

void form_combobox::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	form_element::add_attributes(Attributes);
}
void form_combobox::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(4);

	form_element::docx_convert(Context);
}
// form:listbox
//----------------------------------------------------------------------------------
const wchar_t * form_listbox::ns = L"form";
const wchar_t * form_listbox::name = L"listbox";

void form_listbox::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	form_element::add_attributes(Attributes);
}
void form_listbox::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(5);

	form_element::docx_convert(Context);
}
}
}

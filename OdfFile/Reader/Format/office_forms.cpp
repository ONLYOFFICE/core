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
#include "text_content.h"

#include <xml/xmlchar.h>
#include <xml/utils.h>

#include "serialize_elements.h"

#include "../../Formulas/formulasconvert.h"

#define OBJ_Group			0x0000
#define OBJ_Line			0x0001 
#define OBJ_Rectangle		0x0002 
#define OBJ_Oval			0x0003 
#define OBJ_Arc				0x0004 
#define OBJ_Text			0x0006 
#define OBJ_OfficeArt		0x001E  
#define OBJ_Polygon			0x0009 
#define OBJ_Picture			0x0008 
#define OBJ_Chart			0x0005   
#define OBJ_Button			0x0007 
#define OBJ_CheckBox		0x000B 
#define OBJ_RadioButton		0x000C  
#define OBJ_EditBox			0x000D 
#define OBJ_Label			0x000E 
#define OBJ_DialogBox		0x000F 
#define OBJ_SpinControl		0x0010 
#define OBJ_Scrollbar		0x0011 
#define OBJ_List			0x0012 
#define OBJ_GroupBox		0x0013 
#define OBJ_DropdownList	0x0014 
#define OBJ_Note			0x0019 

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
void office_forms::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
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
void form_form::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
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
void form_properties::xlsx_convert(oox::xlsx_conversion_context & Context)
{

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
void form_list_property::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
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
	CP_APPLY_ATTR(L"form:control-implementation",	control_implementation_);
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
	CP_APPLY_ATTR(L"form:dropdown",		dropdown_);
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
void form_element::xlsx_convert(oox::xlsx_conversion_context & Context)
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
// form:frame
//----------------------------------------------------------------------------------
const wchar_t * form_frame::ns = L"form";
const wchar_t * form_frame::name = L"frame";

void form_frame::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_GroupBox;
	form_element::add_attributes(Attributes);
}
void form_frame::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formFrame);

	form_element::docx_convert(Context);
}
void form_frame::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formFrame);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_frame::serialize_control_props(std::wostream & strm)
{
	formulasconvert::odf2oox_converter converter;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"GBox");

			CP_XML_ATTR(L"dx", L"20");
			CP_XML_ATTR(L"noThreeD", L"1");			
		}
	}
}
// form:button
//----------------------------------------------------------------------------------
const wchar_t * form_button::ns = L"form";
const wchar_t * form_button::name = L"button";

void form_button::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_Button;
	form_element::add_attributes(Attributes);
}
void form_button::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formButton);

	form_element::docx_convert(Context);
}
void form_button::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formButton);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_button::serialize_control_props(std::wostream & strm)
{
	formulasconvert::odf2oox_converter converter;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"Button");

			CP_XML_ATTR(L"dx", L"20");
			CP_XML_ATTR(L"noThreeD", L"1");			
		}
	}
}
// form:text
//----------------------------------------------------------------------------------
const wchar_t * form_text::ns = L"form";
const wchar_t * form_text::name = L"text";

void form_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_EditBox;
	form_element::add_attributes(Attributes);
}
void form_text::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formText);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_text::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formLabel);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_text::serialize_control_props(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"EditBox");

			CP_XML_ATTR(L"dx", L"20");
			CP_XML_ATTR(L"noThreeD", L"1");
		}
	}
}
void form_text::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;

	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
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
				Context.output_stream() << XmlUtils::EncodeXmlString(*current_value_ );
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
// form:textarea
//----------------------------------------------------------------------------------
const wchar_t * form_textarea::ns = L"form";
const wchar_t * form_textarea::name = L"textarea";

// form:fixed-text
//----------------------------------------------------------------------------------
const wchar_t * form_fixed_text::ns = L"form";
const wchar_t * form_fixed_text::name = L"fixed-text";

void form_fixed_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_Label;
	form_element::add_attributes(Attributes);
}
void form_fixed_text::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formLabel);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_fixed_text::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formLabel);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_fixed_text::serialize_control_props(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"Label");

			CP_XML_ATTR(L"lockText", L"1");
			CP_XML_ATTR(L"noThreeD", L"1");
		}
	}
}
void form_fixed_text::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;

	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
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
				Context.output_stream() << XmlUtils::EncodeXmlString(*current_value_ );
				Context.output_stream() << L"</w:t>";
			}
			Context.finish_run();
		}
		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>"; 
}
void form_fixed_text::docx_convert_field(oox::docx_conversion_context & Context, draw_control *draw)
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
	object_type_ = OBJ_CheckBox;

	_CP_OPT(std::wstring) strVal;
	CP_APPLY_ATTR(L"form:current-state", strVal);
	
	if ((strVal) && (*strVal == L"checked"))
		current_state_ = true;
	else current_state_ = false;

	form_element::add_attributes(Attributes);
}
void form_checkbox::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formCheckbox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_checkbox::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formCheckbox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_checkbox::serialize_control_props(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"CheckBox");
			if (current_state_)
				CP_XML_ATTR(L"checked", L"Checked");

			CP_XML_ATTR(L"dx", L"20");
			CP_XML_ATTR(L"noThreeD", L"1");			
		}
	}
}
void form_checkbox::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;
	
	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
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
			Context.output_stream() << XmlUtils::EncodeXmlString(*label_ );
			Context.output_stream() << L"</w:t>";
		Context.finish_run();
	}
}

void form_checkbox::docx_convert_field(oox::docx_conversion_context & Context, draw_control *draw)
{
	if (!draw) return;

	XmlUtils::replace_all( *name_, L" ", L"_");

	Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << *name_ << L"\"/><w:enabled/>";
	Context.output_stream() << L"<w:label w:val=\"" << XmlUtils::EncodeXmlString(*label_ ) << L"\"/>";
	Context.output_stream() << L"<w:checkBox><w:default w:val=\"" << std::to_wstring(current_state_) << L"\"/></w:checkBox></w:ffData>";
	Context.output_stream() << L"</w:fldChar></w:r>";
	Context.output_stream() << L"<w:r><w:instrText>FORMCHECKBOX</w:instrText></w:r>";
	//Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	//Context.output_stream() << L"<w:r><w:t>" << XmlUtils::EncodeXmlString(*label_ ) << L"</w:t></w:r>";
    Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
	
	Context.add_new_run(L"");
		Context.output_stream() << L"<w:t xml:space=\"preserve\">";
		Context.output_stream() << XmlUtils::EncodeXmlString(*label_ );
		Context.output_stream() << L"</w:t>";
	Context.finish_run();	
}
// form:checkbox
//----------------------------------------------------------------------------------
const wchar_t * form_radio::ns = L"form";
const wchar_t * form_radio::name = L"radio";

void form_radio::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_RadioButton;

	_CP_OPT(std::wstring) strVal;
	CP_APPLY_ATTR(L"form:current-selected", strVal);
	
	if ((strVal) && (*strVal == L"true"))
	{
		current_state_ = true;
	}
	else
	{
		current_state_ = false;
	}

	form_element::add_attributes(Attributes);
}
void form_radio::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formCheckbox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);

	//only to activeX object
}
void form_radio::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formCheckbox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_radio::serialize_control_props(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"Radio");
			if (current_state_)
				CP_XML_ATTR(L"checked", L"Checked");

			CP_XML_ATTR(L"dx", L"20");
			CP_XML_ATTR(L"noThreeD", L"1");			
		}
	}
}
void form_radio::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
}
void form_radio::docx_convert_field(oox::docx_conversion_context & Context, draw_control *draw)
{
}
// form:combobox
//----------------------------------------------------------------------------------
const wchar_t * form_combobox::ns = L"form";
const wchar_t * form_combobox::name = L"combobox";

void form_combobox::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_DropdownList;
	form_element::add_attributes(Attributes);
	
	CP_APPLY_ATTR(L"form:source-cell-range", source_cell_range_);
	CP_APPLY_ATTR(L"form:list-source", list_source_);
	CP_APPLY_ATTR(L"form:size", size_);
	CP_APPLY_ATTR(L"form:value", n_value_);
}
void form_combobox::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"form", L"item")
    {
        CP_CREATE_ELEMENT(items_);                    
    }
	else
	{
		form_element::add_child_element(Reader, Ns, Name);
	}
}
void form_combobox::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formCombobox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_combobox::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formCombobox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_combobox::serialize_control_props(std::wostream & strm)
{
	formulasconvert::odf2oox_converter converter;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");
			CP_XML_ATTR(L"objectType", L"Drop");
			CP_XML_ATTR(L"dropStyle", L"combo");
			
			if (size_)
				CP_XML_ATTR(L"dx", 3 * (*size_));
			else	
				CP_XML_ATTR(L"dx", L"20");

			CP_XML_ATTR(L"noThreeD", L"1");
			
			if (linked_cell_)
			{
				std::wstring fmla = converter.convert_named_ref(*linked_cell_, true, L" ", true);
				CP_XML_ATTR(L"fmlaLink", fmla);
			}
			if (source_cell_range_)
			{
				std::wstring fmla = converter.convert_named_ref(*source_cell_range_, true, L" ", true);
				CP_XML_ATTR(L"fmlaRange", fmla);
			}
			CP_XML_ATTR(L"sel", L"3");
			if (n_value_)
				CP_XML_ATTR(L"val", *n_value_);
			else 
				CP_XML_ATTR(L"val", 0);
		}
	}
}
void form_combobox::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	Context.finish_run();
	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
			}
			Context.output_stream() << L"<w:id w:val=\"" + std::to_wstring(Context.get_drawing_context().get_current_shape_id()) + L"\"/>";
			
			Context.output_stream() << L"<w:dropDownList>";

			for (size_t i = 0; i < items_.size(); i++)
			{
				form_item* item = dynamic_cast<form_item*>(items_[i].get());
				if (!item) continue;

				std::wstring displayText = item->text_.empty() ? item->label_ : item->text_;
				Context.output_stream() << L"<w:listItem w:displayText=\"" << XmlUtils::EncodeXmlString(displayText);
				Context.output_stream() << L"\" w:value=\"" << XmlUtils::EncodeXmlString(item->label_) << L"\"/>";
			}
			
			Context.output_stream() << L"</w:dropDownList>";
		}
		Context.output_stream() << L"</w:sdtPr>";
		Context.output_stream() << L"<w:sdtContent>";
		{
			Context.add_new_run(L"");
				Context.output_stream() << L"<w:t xml:space=\"preserve\">";
				if (current_value_)
				{
					Context.output_stream() << XmlUtils::EncodeXmlString(*current_value_ );
				}
				else
				{
					Context.output_stream() << L"Select item";
				}
				Context.output_stream() << L"</w:t>";
			Context.finish_run();
		}
		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>";
	
	if (label_)
	{
		Context.add_new_run(L"");
			Context.output_stream() << L"<w:t xml:space=\"preserve\">";
			Context.output_stream() << XmlUtils::EncodeXmlString(*label_ );
			Context.output_stream() << L"</w:t>";
		Context.finish_run();
	}
}
// form:listbox
//----------------------------------------------------------------------------------
const wchar_t * form_listbox::ns = L"form";
const wchar_t * form_listbox::name = L"listbox";

void form_listbox::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_List;
	form_element::add_attributes(Attributes);
	
	CP_APPLY_ATTR(L"form:source-cell-range", source_cell_range_);
	CP_APPLY_ATTR(L"form:list-source", list_source_);
	CP_APPLY_ATTR(L"form:list-source-type", list_source_type_);
	CP_APPLY_ATTR(L"form:list-linkage-type", list_linkage_type_);
	CP_APPLY_ATTR(L"form:size", size_);
	CP_APPLY_ATTR(L"form:value", n_value_);
}
void form_listbox::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formListbox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_listbox::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formListbox);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_listbox::serialize_control_props(std::wostream & strm)
{
	formulasconvert::odf2oox_converter converter;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", L"List");
			if ((dropdown_) && (dropdown_->get()))
			{
				CP_XML_ATTR(L"dropStyle", L"combo");
			}
			if (size_)
				CP_XML_ATTR(L"dx", 3 * (*size_));
			else
				CP_XML_ATTR(L"dx", L"20");

			CP_XML_ATTR(L"noThreeD", L"1");
			
			if (linked_cell_)
			{
				std::wstring fmla = converter.convert_named_ref(*linked_cell_, true, L" ", true);
				CP_XML_ATTR(L"fmlaLink", fmla);
			}
			if (source_cell_range_)
			{
				std::wstring fmla = converter./*convert_named_expr*/convert_named_ref(*source_cell_range_, true, L" ", true);
				CP_XML_ATTR(L"fmlaRange", fmla);
			}
			//CP_XML_ATTR(L"sel", L"3");
			if (n_value_)
				CP_XML_ATTR(L"val", *n_value_);

		}
	}
}
// form:date
//----------------------------------------------------------------------------------
const wchar_t * form_date::ns = L"form";
const wchar_t * form_date::name = L"date";

void form_date::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = 0x0015;
	form_element::add_attributes(Attributes);
}
void form_date::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formDateTime);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_date::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formDateTime);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_date::serialize_control_props(std::wostream & strm)
{
}
void form_date::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	Context.finish_run();

	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
			}
			Context.output_stream() << L"<w:id w:val=\"" + std::to_wstring(Context.get_drawing_context().get_current_shape_id()) + L"\"/>";
			
			Context.output_stream() << L"<w:date>";
				Context.output_stream() << L"<w:dateFormat w:val=\"\"/>";
				Context.output_stream() << L"<w:lid w:val=\"en-US\"/>";
				Context.output_stream() << L"<w:storeMappedDataAs w:val=\"dateTime\"/>";
				Context.output_stream() << L"<w:calendar w:val=\"gregorian\"/>";				
			Context.output_stream() << L"</w:date>";
		}
		Context.output_stream() << L"</w:sdtPr>";
		Context.output_stream() << L"<w:sdtContent>";
		{
			Context.add_new_run(L"");
				Context.output_stream() << L"<w:t xml:space=\"preserve\">";
				if (current_value_)
				{
					Context.output_stream() << XmlUtils::EncodeXmlString(*current_value_ );
				}
				else
				{
					Context.output_stream() << L"[Insert date]";
				}
				Context.output_stream() << L"</w:t>";
			Context.finish_run();
		}
		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>";
	
	if (label_)
	{
		Context.add_new_run(L"");
			Context.output_stream() << L"<w:t xml:space=\"preserve\">";
			Context.output_stream() << XmlUtils::EncodeXmlString(*label_ );
			Context.output_stream() << L"</w:t>";
		Context.finish_run();
	}
}
// form:time
//----------------------------------------------------------------------------------
const wchar_t * form_time::ns = L"form";
const wchar_t * form_time::name = L"time";

void form_time::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = 0x0016;
	form_element::add_attributes(Attributes);
}
void form_time::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formDateTime);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_time::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formDateTime);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_time::serialize_control_props(std::wostream & strm)
{
}
void form_time::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
	Context.finish_run();

	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			if (name_)
			{
				Context.output_stream() << L"<w:alias w:val=\"" + XmlUtils::EncodeXmlString(*name_) + L"\"/>";
			}
			Context.output_stream() << L"<w:id w:val=\"" + std::to_wstring(Context.get_drawing_context().get_current_shape_id()) + L"\"/>";
			
			Context.output_stream() << L"<w:date>";
				Context.output_stream() << L"<w:dateFormat w:val=\"\"/>";
				Context.output_stream() << L"<w:lid w:val=\"en-US\"/>";
				Context.output_stream() << L"<w:storeMappedDataAs w:val=\"dateTime\"/>";
				Context.output_stream() << L"<w:calendar w:val=\"gregorian\"/>";				
			Context.output_stream() << L"</w:date>";
		}
		Context.output_stream() << L"</w:sdtPr>";
		Context.output_stream() << L"<w:sdtContent>";
		{
			Context.add_new_run(L"");
				Context.output_stream() << L"<w:t xml:space=\"preserve\">";
				if (current_value_)
				{
					Context.output_stream() << XmlUtils::EncodeXmlString(*current_value_ );
				}
				else
				{
					Context.output_stream() << L"[Insert time]";
				}
				Context.output_stream() << L"</w:t>";
			Context.finish_run();
		}
		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>";
	
	if (label_)
	{
		Context.add_new_run(L"");
			Context.output_stream() << L"<w:t xml:space=\"preserve\">";
			Context.output_stream() << XmlUtils::EncodeXmlString(*label_ );
			Context.output_stream() << L"</w:t>";
		Context.finish_run();
	}
}
// form:image-frame
//----------------------------------------------------------------------------------
const wchar_t * form_image_frame::ns = L"form";
const wchar_t * form_image_frame::name = L"image-frame";

void form_image_frame::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_Picture;
	form_element::add_attributes(Attributes);

	CP_APPLY_ATTR(L"form:image-data", image_data_);
}
void form_image_frame::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formImage);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_image_frame::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_forms_context().start_element(oox::formImage);
	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_image_frame::serialize_control_props(std::wostream & strm)
{
}
void form_image_frame::docx_convert_sdt(oox::docx_conversion_context & Context, draw_control *draw)
{
}
// form:listbox
//----------------------------------------------------------------------------------
const wchar_t * form_value_range::ns = L"form";
const wchar_t * form_value_range::name = L"value-range";

void form_value_range::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	object_type_ = OBJ_Scrollbar;
	
	form_element::add_attributes(Attributes);	

	CP_APPLY_ATTR(L"form:min-value", min_value_);
	CP_APPLY_ATTR(L"form:max-value", max_value_);
	CP_APPLY_ATTR(L"form:step-size", step_size_);
	CP_APPLY_ATTR(L"form:page-step-size", page_step_size_);
	CP_APPLY_ATTR(L"form:orientation", orientation_);
	CP_APPLY_ATTR(L"form:delay-for-repeat", delay_for_repeat_);

	if (control_implementation_)
	{
		if (control_implementation_->find(L"SpinButton") != std::wstring::npos)
		{
			object_type_ = OBJ_SpinControl;
			if (!orientation_) orientation_ = L"vertical";
		}
		else
		{
			object_type_ = OBJ_Scrollbar;
			if (!orientation_) orientation_ = L"horizontal";
		}
	}
}
void form_value_range::docx_convert(oox::docx_conversion_context & Context)
{
	if (!control_implementation_) return;
	
	if (control_implementation_->find(L"SpinButton") != std::wstring::npos)
		Context.get_forms_context().start_element(oox::formSpin); 
	else
		Context.get_forms_context().start_element(oox::formScroll); 

	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::docx_convert(Context);
}
void form_value_range::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	if (!control_implementation_) return;
	
	if (control_implementation_->find(L"SpinButton") != std::wstring::npos)
		Context.get_forms_context().start_element(oox::formSpin);
	else
		Context.get_forms_context().start_element(oox::formScroll);

	Context.get_forms_context().set_element(dynamic_cast<form_element*>(this));

	form_element::xlsx_convert(Context);
}
void form_value_range::serialize_control_props(std::wostream & strm)
{
	if (!control_implementation_) return;
	
	formulasconvert::odf2oox_converter converter;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"formControlPr")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");

			CP_XML_ATTR(L"objectType", object_type_ == OBJ_SpinControl ? L"Spin" : L"Scroll");

			CP_XML_ATTR(L"noThreeD", L"1");
			
			if (linked_cell_)
			{
				std::wstring fmla = converter.convert_named_ref(*linked_cell_, true, L" ", true);
				CP_XML_ATTR(L"fmlaLink", fmla);
			}
			if (value_)			CP_XML_ATTR(L"val", *value_);
			if (min_value_)		CP_XML_ATTR(L"min", *min_value_);
			if (max_value_)		CP_XML_ATTR(L"max", *max_value_);
			if (step_size_)		CP_XML_ATTR(L"inc", *step_size_);
			if (page_step_size_)CP_XML_ATTR(L"page",*page_step_size_);
			if (orientation_)
			{
				if (*orientation_ == L"horizontal")
					CP_XML_ATTR(L"horiz", 1);
				else
					CP_XML_ATTR(L"verticalBar", 1);
			}
		}
	}
}

// form:item
//----------------------------------------------------------------------------------
const wchar_t * form_item::ns = L"form";
const wchar_t * form_item::name = L"item";

void form_item::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"form:label", label_, std::wstring(L""));
}
void form_item::add_text(const std::wstring & Text)
{
	text_ = Text;
}

}
}

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

#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/commandtype.h"

namespace cpdoccore { 
namespace odf_reader {

	class draw_control;

//  office:forms
class office_forms : public office_element_impl<office_forms>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeForms;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    office_element_ptr_array	content_;
	_CP_OPT(odf_types::Bool)	apply_design_mode_;
	_CP_OPT(odf_types::Bool)	automatic_focus_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(office_forms);

//  form:form
class form_form : public office_element_impl<form_form>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormForm;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    office_element_ptr_array			content_;
	office_element_ptr					office_event_listeners_;
	office_element_ptr					properties_;

	_CP_OPT(odf_types::Bool)			allow_deletes_;
	_CP_OPT(odf_types::Bool)			allow_inserts_;
	_CP_OPT(odf_types::Bool)			allow_updates_;

	_CP_OPT(odf_types::Bool)			apply_filter_;
	_CP_OPT(std::wstring)				command_; 
	_CP_OPT(odf_types::command_type)	command_type_;
	_CP_OPT(std::wstring)				control_implementation_; 
	_CP_OPT(std::wstring)				datasource_;
	_CP_OPT(std::wstring)				detail_fields_;
	_CP_OPT(std::wstring)				enctype_; 
	_CP_OPT(odf_types::Bool)			escape_processing_;
	_CP_OPT(std::wstring)				filter_;
	_CP_OPT(odf_types::Bool)			ignore_result_;
	_CP_OPT(std::wstring)				master_fields_; 
	_CP_OPT(std::wstring)				method_;
	_CP_OPT(std::wstring)				name_;
	_CP_OPT(std::wstring)				navigation_mode_;
	_CP_OPT(std::wstring)				order_;
	_CP_OPT(std::wstring)				tabcycle_; 

	odf_types::common_xlink_attlist		xlink_attlist_;
	_CP_OPT(std::wstring)				office_target_frame_;

    
};
CP_REGISTER_OFFICE_ELEMENT2(form_form);

//  form:properties
class form_properties : public office_element_impl<form_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormProperties;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    office_element_ptr_array content_; //form property && form list-property
};
CP_REGISTER_OFFICE_ELEMENT2(form_properties);

//  form:property
class form_property : public office_element_impl<form_property>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormProperty;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

public:
	_CP_OPT(std::wstring)						property_name_;
	odf_types::common_value_and_type_attlist	value_and_type_;
};
CP_REGISTER_OFFICE_ELEMENT2(form_property);

//  form:list-property
class form_list_property : public office_element_impl<form_list_property>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormListProperty;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	_CP_OPT(std::wstring)					property_name_;
	_CP_OPT(odf_types::office_value_type)	value_type_;

	office_element_ptr_array				content_;		// form:list-value
};
CP_REGISTER_OFFICE_ELEMENT2(form_list_property);

// form:list-value
class form_list_value : public office_element_impl<form_list_value>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormListValue;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

public:
	odf_types::common_value_and_type_attlist	value_and_type_;
};
CP_REGISTER_OFFICE_ELEMENT2(form_list_value);
//-----------------------------------------------------------------------------------------------------

class form_element : public office_element_impl<form_element>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    
	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormElement;

	form_element() : object_type_(0) {}

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw) {}
	virtual void docx_convert_field	(oox::docx_conversion_context & Context, draw_control* draw) {}

	virtual void serialize_control_props(std::wostream & strm){}

	friend class odf_document;
//----------------------------------------------------------------------------------------------
	office_element_ptr			office_event_listeners_;
	office_element_ptr			properties_;

	_CP_OPT(std::wstring)		control_implementation_; 
	_CP_OPT(std::wstring)		data_field_;
	_CP_OPT(std::wstring)		linked_cell_;//cell ref
	_CP_OPT(odf_types::Bool)	disabled_;
	_CP_OPT(std::wstring)		id_;
	_CP_OPT(std::wstring)		label_;
	_CP_OPT(std::wstring)		name_;
	_CP_OPT(odf_types::Bool)	printable_;
	_CP_OPT(unsigned int)		tab_index_;
	_CP_OPT(odf_types::Bool)	tab_stop_; 
	_CP_OPT(std::wstring)		title_;
	_CP_OPT(std::wstring)		value_;
	_CP_OPT(std::wstring)		xml_id_;
	_CP_OPT(std::wstring)		xforms_bind_;
	_CP_OPT(std::wstring)		current_value_;
	_CP_OPT(odf_types::Bool)	dropdown_;
//----------------------------------------------------------------------------------------------

	int object_type_;
};
//-----------------------------------------------------------------------------------------
//  form:frame
class form_frame : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormFrame;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void serialize_control_props(std::wostream & strm);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
};
CP_REGISTER_OFFICE_ELEMENT2(form_frame);
//-----------------------------------------------------------------------------------------
//  form:button
class form_button : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormButton;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void serialize_control_props(std::wostream & strm);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:

	//form:image-align
	//form:image-position
	//form:button-type
	//form:default-button
	//form:delayfor-repeat
	//form:focus-on-click
	//form:image-data
	//form:repeat
	//form:toggle 
	//form:xforms-submission
	//office:target-frame
	//xlink:href
};
CP_REGISTER_OFFICE_ELEMENT2(form_button);

//  form:text
class form_text : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormText;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw);
	virtual void docx_convert_field	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	//form:convert-empty-to-null
	//form:readonly
	//form:max-length
};
CP_REGISTER_OFFICE_ELEMENT2(form_text);
//--------------------------------------------------------------------------------------------
//  form:fixed-text 
class form_fixed_text : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormFixedText;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw);
	virtual void docx_convert_field	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
};
CP_REGISTER_OFFICE_ELEMENT2(form_fixed_text);
//--------------------------------------------------------------------------------------------
//  form:textarea 
class form_textarea : public form_text
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormTextarea;
    CPDOCCORE_DEFINE_VISITABLE();
};
CP_REGISTER_OFFICE_ELEMENT2(form_textarea);
//--------------------------------------------------------------------------------------------
//  form:checkbox
class form_checkbox : public form_text
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormCheckbox;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control *draw);
	virtual void docx_convert_field	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	bool current_state_;
	//form:image-align
	//form:image-position
	//form:is-tristate
	//form:visual-effect
};
CP_REGISTER_OFFICE_ELEMENT2(form_checkbox);
//--------------------------------------------------------------------------------------------
//  form:radio
class form_radio : public form_checkbox
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormRadio;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control *draw);
	virtual void docx_convert_field	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
};
CP_REGISTER_OFFICE_ELEMENT2(form_radio);
//--------------------------------------------------------------------------------------------
//  form:value-range
class form_value_range : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormValueRange;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control *draw){}
	virtual void docx_convert_field	(oox::docx_conversion_context & Context, draw_control* draw){}
	
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	_CP_OPT(int)			min_value_;
	_CP_OPT(int)			max_value_;
	_CP_OPT(int)			step_size_;
	_CP_OPT(int)			page_step_size_;
	_CP_OPT(std::wstring)	orientation_;
	_CP_OPT(std::wstring)	delay_for_repeat_;
};
CP_REGISTER_OFFICE_ELEMENT2(form_value_range);

//  form:combobox
class form_combobox : public form_text
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormCombobox;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	office_element_ptr_array	items_;
	_CP_OPT(int	)				size_;
	_CP_OPT(int)				n_value_;

	_CP_OPT(std::wstring)		source_cell_range_;
	_CP_OPT(std::wstring)		list_source_;
	
	//form:auto-complete

};
CP_REGISTER_OFFICE_ELEMENT2(form_combobox);

//  form:listbox
class form_listbox : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormListbox;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	_CP_OPT(std::wstring)		list_linkage_type_;
	_CP_OPT(std::wstring)		source_cell_range_;
	_CP_OPT(std::wstring)		list_source_;
	_CP_OPT(std::wstring)		list_source_type_;
	_CP_OPT(int)				size_;
	_CP_OPT(int)				n_value_;
	
	//form:bound-column
	//form:xforms-list-source
	//form:multiple
	//form:listlinkage-type
};
CP_REGISTER_OFFICE_ELEMENT2(form_listbox);

//  form:date
class form_date : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormDate;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:

};
CP_REGISTER_OFFICE_ELEMENT2(form_date);

//  form:time
class form_time : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormTime;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:

};
CP_REGISTER_OFFICE_ELEMENT2(form_time);

//  form:image-frame
class form_image_frame : public form_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormImageFrame;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context){}
	
	virtual void docx_convert_sdt	(oox::docx_conversion_context & Context, draw_control* draw);
	
	virtual void serialize_control_props(std::wostream & strm);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	_CP_OPT(std::wstring)	image_data_;

};
CP_REGISTER_OFFICE_ELEMENT2(form_image_frame);

//  form:item
class form_item : public office_element_impl<form_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFormItem;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

public:
	std::wstring label_;
	std::wstring text_;
};
CP_REGISTER_OFFICE_ELEMENT2(form_item);
}
}
//<form:connection-resource>7.6.2,  
//<form:file> 13.5.5,
//<form:formatted-text> 13.5.6, 
//<form:generic-control> 13.5.25, 
//<form:grid> 13.5.22, 
//<form:hidden> 13.5.21,
//<form:image> 13.5.16, 
//<form:number> 13.5.7,
//<form:password> 13.5.4, 

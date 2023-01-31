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

#include "text_content.h"
#include "office_elements_create.h"

#include "../../DataTypes/textdisplay.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {
namespace text {

class paragraph
{
public:
    paragraph() : next_section_(false), next_end_section_(false), is_header_(false), is_present_hyperlink_(false) {}

    std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
   
	void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
    void add_text			(const std::wstring & Text);
	void add_space			(const std::wstring & Text);

 	//_CP_OPT(std::wstring)	next_element_style_name; //for master page
   
	void set_next_section(bool Val) 
    {
        next_section_ = Val;
    }

    void set_next_end_section(bool Val) 
    {
        next_end_section_ = Val;
    }
  
	void docx_convert (oox::docx_conversion_context & Context, _CP_OPT(std::wstring) next_element_style_name) ;
    void xlsx_convert (oox::xlsx_conversion_context & Context) ;
    void pptx_convert (oox::pptx_conversion_context & Context) ;

	size_t drop_cap_docx_convert(oox::docx_conversion_context & Context);

	paragraph_attrs				attrs_;

    office_element_ptr_array	content_;
	
	office_element_ptr			sequence_;

	bool						is_header_;
	bool						is_present_hyperlink_;
private:
	void drop_cap_text_docx_convert(office_element_ptr first_text_paragraph,oox::docx_conversion_context & Context);
 
	bool					next_section_;
    bool					next_end_section_;	

    friend class p;   
	friend class h;   
   
};

//---------------------------------------------------------------------------------------------------
class h : public office_element_impl<h>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextH;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    void pptx_convert(oox::pptx_conversion_context & Context) ;

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    paragraph paragraph_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
	virtual void add_space(const std::wstring & Text){}

    // heading-attrs
    _CP_OPT(unsigned int)	outline_level_;
    _CP_OPT(bool)			restart_numbering_;
    _CP_OPT(unsigned int)	start_value_;
    _CP_OPT(bool)			is_list_header_;
    _CP_OPT(std::wstring)	number_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(h);

//---------------------------------------------------------------------------------------------------
class p : public office_element_impl<p>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextP;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context) ;
    void xlsx_convert(oox::xlsx_conversion_context & Context) ;
	void pptx_convert(oox::pptx_conversion_context & Context) ;

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    p(){};
	paragraph paragraph_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
	virtual void add_space(const std::wstring & Text);

};
CP_REGISTER_OFFICE_ELEMENT2(p);

//---------------------------------------------------------------------------------------------------
class list : public office_element_impl<list>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextList;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    list(){};

    void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    std::wstring				style_name_;
    _CP_OPT(bool)				continue_numbering_;

	office_element_ptr          list_header_;
    office_element_ptr_array    list_items_;

};
CP_REGISTER_OFFICE_ELEMENT2(list);

//---------------------------------------------------------------------------------------------------
class soft_page_break : public office_element_impl<soft_page_break>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSoftPageBreak;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

};
CP_REGISTER_OFFICE_ELEMENT2(soft_page_break);

//---------------------------------------------------------------------------------------------------
class section : public office_element_impl<section>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSection;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    
	virtual void afterCreate();
    virtual void afterReadContent();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    odf_types::section_attlists	section_attr_;
    office_element_ptr			section_source_;
    office_element_ptr_array	content_;          

};

CP_REGISTER_OFFICE_ELEMENT2(section);

class section_source_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    odf_types::common_xlink_attlist	xlink_attlist_;

    _CP_OPT(std::wstring)			section_name_;
    _CP_OPT(std::wstring)			filter_name_;
    
};

//---------------------------------------------------------------------------------------------------
class section_source : public office_element_impl<section_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSectionSource;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    section_source_attr	section_source_attr_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(section_source);

//---------------------------------------------------------------------------------------------------
class common_entry_template : public office_element_impl<common_entry_template>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextCommonEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

	_CP_OPT(std::wstring)		style_name_;
	_CP_OPT(int)				outline_level_;
	office_element_ptr_array	content_;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

};
//---------------------------------------------------------------------------------------------------

class table_of_content : public office_element_impl<table_of_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTableOfContent;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	odf_types::section_attlists	section_attr_;
    
	office_element_ptr			table_of_content_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_of_content);
//---------------------------------------------------------------------------------------------------
// text:table-index
//---------------------------------------------------------------------------------------------------
class table_index : public office_element_impl<table_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextTableIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    odf_types::section_attlists	section_attr_;
    office_element_ptr			table_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_index);
//---------------------------------------------------------------------------------------------------
// text:illustration-index
//---------------------------------------------------------------------------------------------------
class illustration_index : public office_element_impl<illustration_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextIllustrationIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
  
	virtual void afterCreate();
    virtual void afterReadContent();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    odf_types::section_attlists	section_attr_;
    office_element_ptr			illustration_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(illustration_index);
//---------------------------------------------------------------------------------------------------
// text:alphabetical-index
//---------------------------------------------------------------------------------------------------
class alphabetical_index : public office_element_impl<alphabetical_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextAlphabeticalIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
    virtual void afterCreate();
    virtual void afterReadContent();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    odf_types::section_attlists	section_attr_;
    office_element_ptr			alphabetical_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index);
//---------------------------------------------------------------------------------------------------
// text:object-index
//---------------------------------------------------------------------------------------------------
class object_index : public office_element_impl<object_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextObjectIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
    virtual void afterCreate();
    virtual void afterReadContent();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    odf_types::section_attlists	section_attr_;
    office_element_ptr			object_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(object_index);
//---------------------------------------------------------------------------------------------------
// text:user-index
//---------------------------------------------------------------------------------------------------
class user_index : public office_element_impl<user_index>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextUserIndex;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
    virtual void afterCreate();
    virtual void afterReadContent();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    odf_types::section_attlists	section_attr_;
    office_element_ptr			user_index_source_;
    office_element_ptr			index_body_;

};
CP_REGISTER_OFFICE_ELEMENT2(user_index);
//------------------------------------------------------------------------------------------------------------
// text:bibliography
//------------------------------------------------------------------------------------------------------------
class bibliography : public office_element_impl<bibliography>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextBibliography;
    
	CPDOCCORE_DEFINE_VISITABLE();
    
    virtual void afterCreate();
    virtual void afterReadContent();

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    odf_types::section_attlists	section_attr_;
    office_element_ptr			bibliography_source_;
    office_element_ptr			index_body_;
};
CP_REGISTER_OFFICE_ELEMENT2(bibliography);
//------------------------------------------------------------------------------------------------------------
// text:bibliography-source
//------------------------------------------------------------------------------------------------------------
class bibliography_source: public office_element_impl<bibliography_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextBibliographySource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(bibliography_source);
//------------------------------------------------------------------------------------------------------------
// text:bibliography-entry-template
//------------------------------------------------------------------------------------------------------------
class bibliography_entry_template: public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextBibliographyEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );

	_CP_OPT(std::wstring)		bibliography_type_;//article, book, email, ...
};
CP_REGISTER_OFFICE_ELEMENT2(bibliography_entry_template);
//---------------------------------------------------------------------------------------------------
//text:index-body
//---------------------------------------------------------------------------------------------------
class index_body : public office_element_impl<index_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexBody;
    CPDOCCORE_DEFINE_VISITABLE();
    
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	office_element_ptr			index_title_;
    office_element_ptr_array	content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(index_body);
//---------------------------------------------------------------------------------------------------
class index_title : public office_element_impl<index_title>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexTitle;
    CPDOCCORE_DEFINE_VISITABLE();
  	
	void pptx_convert(oox::pptx_conversion_context & Context) ;
	void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	_CP_OPT(std::wstring)		xml_id_;
	odf_types::section_attlists	section_attr_;
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(index_title);
//---------------------------------------------------------------------------------------------------
class index_title_template : public office_element_impl<index_title_template>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextIndexTitleTemplate;
    CPDOCCORE_DEFINE_VISITABLE();
  	
	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(std::wstring)	style_name_;
	office_element_ptr		content_;
private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text			( const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(index_title_template);
//---------------------------------------------------------------------------------------------------
class unknown_base_change : public office_element_impl<unknown_base_change>
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;   	
	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeNone;

    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context);

    office_element_ptr			office_change_info_;
    office_element_ptr_array	content_;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

};

//---------------------------------------------------------------------------------------------------
class unknown_change : public unknown_base_change
{
public:
	static const wchar_t * ns;
    static const wchar_t * name;   	
	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUnknownChange;
    
	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(unknown_change);

//---------------------------------------------------------------------------------------------------
class insertion : public unknown_base_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextInsertion;

    virtual void docx_convert(oox::docx_conversion_context & Context);
};

CP_REGISTER_OFFICE_ELEMENT2(insertion);

//---------------------------------------------------------------------------------------------------
class deletion : public unknown_base_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextDeletion;

	virtual void docx_convert(oox::docx_conversion_context & Context);
};

CP_REGISTER_OFFICE_ELEMENT2(deletion);

//---------------------------------------------------------------------------------------------------
class format_change : public unknown_base_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextFormatChange;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void docx_convert(oox::docx_conversion_context & Context);
	
	_CP_OPT(std::wstring)	style_name_; //не по снецификации ... но КАК сохранить то что было изменено в формате?????
};
CP_REGISTER_OFFICE_ELEMENT2(format_change);

//---------------------------------------------------------------------------------------------------
class tracked_changes : public office_element_impl<tracked_changes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextTrackedChanges;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	_CP_OPT(odf_types::Bool)	track_changes_;
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(tracked_changes);

//---------------------------------------------------------------------------------------------------
class changed_region : public office_element_impl<changed_region>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChangedRegion;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	_CP_OPT(std::wstring)		id_; //== xml:id
    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(changed_region);

//---------------------------------------------------------------------------------------------------
class add_change : public office_element_impl<add_change>
{
public:
 	static const wchar_t * ns;
    static const wchar_t * name;   	

	static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChange;
   
	CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) = 0;

	_CP_OPT(std::wstring)	change_id_;    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

};

//---------------------------------------------------------------------------------------------------
class change : public add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChange;  
   
	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(change);

//---------------------------------------------------------------------------------------------------
class change_start : public add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChangeStart;
	
	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(change_start);

//---------------------------------------------------------------------------------------------------
class change_end : public add_change
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextChangeEnd;

	virtual void docx_convert(oox::docx_conversion_context & Context);
};
CP_REGISTER_OFFICE_ELEMENT2(change_end);
//-------------------------------------------------------------------------------------------------------------------
//text:variable-input
//---------------------------------------------------------------------------------------------------
class variable_input : public office_element_impl<variable_input>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableInput;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::office_value_type)	office_value_type_;
	_CP_OPT(std::wstring)					style_data_style_name_;
	_CP_OPT(std::wstring)					description_;
	_CP_OPT(std::wstring)					display_;
	_CP_OPT(std::wstring)					name_;

    std::wstring							text_;
};
CP_REGISTER_OFFICE_ELEMENT2(variable_input);
//-------------------------------------------------------------------------------------------------------------------
//text:variable-get
//---------------------------------------------------------------------------------------------------
class variable_get : public office_element_impl<variable_get>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableGet;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(std::wstring)	display_;
	_CP_OPT(std::wstring)	name_;

};
CP_REGISTER_OFFICE_ELEMENT2(variable_get);
//-------------------------------------------------------------------------------------------------------------------
//text:variable-set
//---------------------------------------------------------------------------------------------------
class variable_set : public office_element_impl<variable_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableSet;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
	virtual void add_text(const std::wstring & Text);
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	odf_types::common_value_and_type_attlist office_value_;
	_CP_OPT(std::wstring) office_formula_;

	_CP_OPT(std::wstring) style_data_style_name_;
	_CP_OPT(std::wstring) display_;
	_CP_OPT(std::wstring) name_;

	office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(variable_set);
//---------------------------------------------------------------------------------------------------
//text:variable-decl
//---------------------------------------------------------------------------------------------------
class variable_decl : public office_element_impl<variable_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableDecl;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(odf_types::office_value_type)	office_value_type_;
	_CP_OPT(std::wstring)	display_;
	_CP_OPT(std::wstring)	name_;

};
CP_REGISTER_OFFICE_ELEMENT2(variable_decl);
//---------------------------------------------------------------------------------------------------
//text:variable-decls
//---------------------------------------------------------------------------------------------------
class variable_decls : public office_element_impl<variable_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextVariableDecls;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(variable_decls);
//---------------------------------------------------------------------------------------------------
//text:user-field-decl
//---------------------------------------------------------------------------------------------------
class user_field_decl : public office_element_impl<user_field_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUserFieldDecl;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    //virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    //virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	odf_types::common_value_and_type_attlist	office_value_;
	_CP_OPT(std::wstring)						office_formula_;
	_CP_OPT(std::wstring)						name_;
};
CP_REGISTER_OFFICE_ELEMENT2(user_field_decl);
//---------------------------------------------------------------------------------------------------
//text:user-field-decls
//---------------------------------------------------------------------------------------------------
class user_field_decls : public office_element_impl<user_field_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUserFieldDecls;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    //virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    //virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(user_field_decls);
//---------------------------------------------------------------------------------------------------
//text:sequence-decl
//---------------------------------------------------------------------------------------------------
class sequence_decl : public office_element_impl<sequence_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextSequenceDecl;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(std::wstring)	separation_character_; //one char
	_CP_OPT(unsigned int)	display_outline_level_;
	_CP_OPT(std::wstring)	name_;
};
CP_REGISTER_OFFICE_ELEMENT2(sequence_decl);
//---------------------------------------------------------------------------------------------------
//text:sequence-decls
//---------------------------------------------------------------------------------------------------
class sequence_decls : public office_element_impl<sequence_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextSequenceDecls;
    CPDOCCORE_DEFINE_VISITABLE()
    
    void docx_convert(oox::docx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(sequence_decls);
//---------------------------------------------------------------------------------------------------
//text:table-of-content-source
//---------------------------------------------------------------------------------------------------
class table_of_content_source: public office_element_impl<table_of_content_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextTableOfContentSource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	_CP_OPT(int)				outline_level_;
	_CP_OPT(odf_types::Bool)	use_index_marks_;
	_CP_OPT(odf_types::Bool)	use_index_source_styles_;
	_CP_OPT(odf_types::Bool)	use_outline_level_;
	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
	office_element_ptr_array	index_source_styles_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_of_content_source);

//---------------------------------------------------------------------------------------------------
//text:table-of-content-entry-template
//---------------------------------------------------------------------------------------------------
class table_of_content_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextTableOfContentEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
};
CP_REGISTER_OFFICE_ELEMENT2(table_of_content_entry_template);
//---------------------------------------------------------------------------------------------------
//text:index-entry-bibliography
//---------------------------------------------------------------------------------------------------
class index_entry_bibliography: public office_element_impl<index_entry_bibliography>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryBibliography;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

public:
	_CP_OPT(std::wstring)		style_name_;
	_CP_OPT(std::wstring)		bibliography_data_field_;
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_bibliography);
//---------------------------------------------------------------------------------------------------
//text:index-entry-chapter
//---------------------------------------------------------------------------------------------------
class index_entry_chapter: public office_element_impl<index_entry_chapter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryChapter;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_chapter);
//---------------------------------------------------------------------------------------------------
//text:index-entry-link-end
//---------------------------------------------------------------------------------------------------
class index_entry_link_end: public office_element_impl<index_entry_link_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryLinkEnd;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_link_end);
//---------------------------------------------------------------------------------------------------
//text:index-entry-link-start
//---------------------------------------------------------------------------------------------------
class index_entry_link_start: public office_element_impl<index_entry_link_start>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryLinkStart;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_link_start);
//---------------------------------------------------------------------------------------------------
//text:index-entry-page-number
//---------------------------------------------------------------------------------------------------
class index_entry_page_number: public office_element_impl<index_entry_page_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryPageNumber;
    CPDOCCORE_DEFINE_VISITABLE()

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_page_number);
//---------------------------------------------------------------------------------------------------
//text:index-entry-span
//---------------------------------------------------------------------------------------------------
class index_entry_span: public office_element_impl<index_entry_span>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntrySpan;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_span);
//---------------------------------------------------------------------------------------------------
//text:index-entry-tab-stop
//---------------------------------------------------------------------------------------------------
class index_entry_tab_stop: public office_element_impl<index_entry_tab_stop>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryTabStop;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_tab_stop);
//---------------------------------------------------------------------------------------------------
//text:index-entry-text
//---------------------------------------------------------------------------------------------------
class index_entry_text: public office_element_impl<index_entry_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexEntryText;
    CPDOCCORE_DEFINE_VISITABLE()
    
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(index_entry_text);
//---------------------------------------------------------------------------------------------------
//text:illustration-index-source
//---------------------------------------------------------------------------------------------------
class illustration_index_source: public office_element_impl<illustration_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIllustrationIndexSource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	_CP_OPT(std::wstring)		caption_sequence_name_;
	_CP_OPT(std::wstring)		caption_sequence_format_;

	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_caption_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(illustration_index_source);
//---------------------------------------------------------------------------------------------------
//text:illustration-index-entry-template
//---------------------------------------------------------------------------------------------------
class illustration_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIllustrationIndexEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
};
CP_REGISTER_OFFICE_ELEMENT2(illustration_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-source
//---------------------------------------------------------------------------------------------------
class alphabetical_index_source : public office_element_impl<alphabetical_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextAlphabeticalIndexSource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:
	_CP_OPT(odf_types::Bool)	alphabetical_separators_; 
	_CP_OPT(odf_types::Bool)	ignore_case_; 

//fo:country
//fo:language 
//fo:script
//style:rfclanguage-tag
//text:alphabetical-separators 
//text:capitalizeentries
//text:combine-entries 
//text:combine-entries-with-dash
//text:combine-entries-with-pp
//text:comma-separated
//text:main-entry-style-name
//text:relative-tab-stop-position
//text:sort-algorithm
//text:use-keys-as-entries

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_source);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-entry-template
//---------------------------------------------------------------------------------------------------
class alphabetical_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextAlphabeticalIndexEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:table-index-source
//---------------------------------------------------------------------------------------------------
class table_index_source: public office_element_impl<table_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextTableIndexSource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	_CP_OPT(std::wstring)		caption_sequence_name_;
	_CP_OPT(std::wstring)		caption_sequence_format_;

	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_caption_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_index_source);
//---------------------------------------------------------------------------------------------------
//text:table-index-entry-template
//---------------------------------------------------------------------------------------------------
class table_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextTableIndexEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
};
CP_REGISTER_OFFICE_ELEMENT2(table_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:object-index-source
//---------------------------------------------------------------------------------------------------
class object_index_source: public office_element_impl<object_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextObjectIndexSource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_chart_objects_;
	_CP_OPT(odf_types::Bool)	use_draw_objects_;
	_CP_OPT(odf_types::Bool)	use_math_objects_;
	_CP_OPT(odf_types::Bool)	use_other_objects_;
	_CP_OPT(odf_types::Bool)	use_spreadsheet_objects_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
};
CP_REGISTER_OFFICE_ELEMENT2(object_index_source);
//---------------------------------------------------------------------------------------------------
//text:object-index-entry-template
//---------------------------------------------------------------------------------------------------
class object_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextObjectIndexEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
};
CP_REGISTER_OFFICE_ELEMENT2(object_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:user-index-source
//---------------------------------------------------------------------------------------------------
class user_index_source: public office_element_impl<user_index_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUserIndexSource;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	_CP_OPT(std::wstring)		index_name_;
	_CP_OPT(odf_types::Bool)	copy_outline_levels_;
	_CP_OPT(odf_types::Bool)	relative_tab_stop_position_;
	_CP_OPT(odf_types::Bool)	use_chart_objects_;
	_CP_OPT(odf_types::Bool)	use_draw_objects_;
	_CP_OPT(odf_types::Bool)	use_graphics_;
	_CP_OPT(odf_types::Bool)	use_floating_frames_;
	_CP_OPT(odf_types::Bool)	use_index_marks_;
	_CP_OPT(odf_types::Bool)	use_index_source_styles_;
	_CP_OPT(odf_types::Bool)	use_objects_;
	_CP_OPT(odf_types::Bool)	use_tables_;
	_CP_OPT(std::wstring)		index_scope_; // chapter or document:

	office_element_ptr			index_title_template_;
	office_element_ptr_array	entry_templates_;
	office_element_ptr_array	index_source_styles_;
};
CP_REGISTER_OFFICE_ELEMENT2(user_index_source);
//---------------------------------------------------------------------------------------------------
//text:user-index-entry-template
//---------------------------------------------------------------------------------------------------
class user_index_entry_template : public common_entry_template
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUserIndexEntryTemplate;
    CPDOCCORE_DEFINE_VISITABLE()
};
CP_REGISTER_OFFICE_ELEMENT2(user_index_entry_template);
//---------------------------------------------------------------------------------------------------
//text:index-source-styles
//---------------------------------------------------------------------------------------------------
class index_source_styles: public office_element_impl<index_source_styles>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexSourceStyles;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	
	_CP_OPT(int)				outline_level_;
	office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(index_source_styles);
//---------------------------------------------------------------------------------------------------
//text:index-source-style
//---------------------------------------------------------------------------------------------------
class index_source_style: public office_element_impl<index_source_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextIndexSourceStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	
	_CP_OPT(std::wstring)	style_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(index_source_style);
}
}
}

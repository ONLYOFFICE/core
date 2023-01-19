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
#include <vector>

#include "../../Common/CPWeakPtr.h"

#include "office_elements_create.h"

#include "../../DataTypes/targetframename.h"
#include "../../DataTypes/noteclass.h"
#include "../../DataTypes/bool.h"
#include "../../DataTypes/bibliography.h"

#include "../../Reader/Converter/docx_conversion_context.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {

namespace text {

class paragraph_content_element :  public office_element_impl<paragraph_content_element>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTextElement;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	virtual void docx_convert(oox::docx_conversion_context & Context){}
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

	virtual void docx_serialize_sdt_placeholder(const std::wstring & name, office_element_ptr & text, oox::docx_conversion_context & Context);
	virtual void docx_serialize_field(const std::wstring & field_name, office_element_ptr & text, oox::docx_conversion_context & Context, bool bLock = false);
	virtual void docx_serialize_run(office_element_ptr & text, oox::docx_conversion_context & Context);

	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context){}
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
//-------------------------------------------------------------------------------------------------------------------
// simple text
//-------------------------------------------------------------------------------------------------------------------
class text : public paragraph_content_element
{
public:
    static office_element_ptr create(const std::wstring & Text);

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeText;
    static const ElementType type = typeTextText;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	bool preserve_;
    
	text(const std::wstring & Text);
	text() : preserve_(true) {}

    std::wstring text_;

private:
    
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) {}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text);
	virtual void add_space(const std::wstring & Text);
};

//-------------------------------------------------------------------------------------------------------------------
// text:s
//-------------------------------------------------------------------------------------------------------------------
class s : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextS;

	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    
    s(unsigned int c) : text_c_(c) {};
    s() {};

    _CP_OPT(unsigned int) text_c_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) ;
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring) content_;
};
CP_REGISTER_OFFICE_ELEMENT2(s);
//-------------------------------------------------------------------------------------------------------------------
// text:tab
//-------------------------------------------------------------------------------------------------------------------
class tab : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTab;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    tab(unsigned int ab_ref) : text_tab_ref_(ab_ref) {};
    tab() {};

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;
    
    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
   
    _CP_OPT(unsigned int) text_tab_ref_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}
};
CP_REGISTER_OFFICE_ELEMENT2(tab);
//-------------------------------------------------------------------------------------------------------------------
// text:tab-stop - openoffice xml 1.0
//-------------------------------------------------------------------------------------------------------------------
class tab_stop : public tab
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeTextTabStop;
};
CP_REGISTER_OFFICE_ELEMENT2(tab_stop);
//-------------------------------------------------------------------------------------------------------------------
// text:line-break
//-------------------------------------------------------------------------------------------------------------------
class line_break : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextLineBreak;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) {}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};

CP_REGISTER_OFFICE_ELEMENT2(line_break);
//-------------------------------------------------------------------------------------------------------------------
// text:bookmark
//-------------------------------------------------------------------------------------------------------------------
class bookmark : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmark;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    virtual void docx_convert(oox::docx_conversion_context & Context);

	std::wstring text_name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};

CP_REGISTER_OFFICE_ELEMENT2(bookmark);
//-------------------------------------------------------------------------------------------------------------------
// text:bookmark-start
//-------------------------------------------------------------------------------------------------------------------
class bookmark_start : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkStart;
  
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    virtual void docx_convert(oox::docx_conversion_context & Context);

    std::wstring name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};
CP_REGISTER_OFFICE_ELEMENT2(bookmark_start);
//-------------------------------------------------------------------------------------------------------------------
// text:bookmark-end
//-------------------------------------------------------------------------------------------------------------------
class bookmark_end : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkEnd;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    virtual void docx_convert(oox::docx_conversion_context & Context);

    std::wstring name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};
CP_REGISTER_OFFICE_ELEMENT2(bookmark_end);

//-------------------------------------------------------------------------------------------------------------------
// text:bookmark-ref
//-------------------------------------------------------------------------------------------------------------------
class bookmark_ref : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextBookmarkRef;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    std::wstring			ref_name_;
    _CP_OPT(std::wstring)	reference_format_;
    std::wstring			content_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(bookmark_ref);
//-------------------------------------------------------------------------------------------------------------------
// text:reference-ref
//-------------------------------------------------------------------------------------------------------------------
class reference_ref : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceRef;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    std::wstring			ref_name_;
    _CP_OPT(std::wstring)	reference_format_;
    std::wstring			content_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(reference_ref);
//-------------------------------------------------------------------------------------------------------------------
// text:reference-mark
//-------------------------------------------------------------------------------------------------------------------
class reference_mark : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMark;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    std::wstring text_name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};
CP_REGISTER_OFFICE_ELEMENT2(reference_mark);
//-------------------------------------------------------------------------------------------------------------------
// text:reference-mark-start
//-------------------------------------------------------------------------------------------------------------------
class reference_mark_start : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMarkStart;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    std::wstring text_name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}
};

CP_REGISTER_OFFICE_ELEMENT2(reference_mark_start);
//-------------------------------------------------------------------------------------------------------------------
// text:reference-mark-end
//-------------------------------------------------------------------------------------------------------------------
class reference_mark_end : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextReferenceMarkEnd;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    std::wstring text_name_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}

};

CP_REGISTER_OFFICE_ELEMENT2(reference_mark_end);
//-------------------------------------------------------------------------------------------------------------------
// text:span
//-------------------------------------------------------------------------------------------------------------------
class span : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSpan;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

 	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	span() {}
   
	office_element_ptr_array content_;
    
	std::wstring				text_style_name_;
	std::vector<std::wstring>	text_class_names_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
	virtual void add_space(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(span);
//-------------------------------------------------------------------------------------------------------------------
// text:a
//-------------------------------------------------------------------------------------------------------------------
class a : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextA;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	a() {}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
	virtual void add_space(const std::wstring & Text);

	odf_types::common_xlink_attlist			xlink_attlist_;

    std::wstring							office_name_;
    _CP_OPT(odf_types::target_frame_name)	office_target_frame_name_;

    std::wstring						text_style_name_;
    std::wstring						text_visited_style_name_;

    office_element_ptr_array			content_;
};

CP_REGISTER_OFFICE_ELEMENT2(a);
//-------------------------------------------------------------------------------------------------------------------
// text:note
//-------------------------------------------------------------------------------------------------------------------
class note : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNote;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    note();

	std::wstring			text_id_;
	odf_types::noteclass	text_note_class_;
    office_element_ptr		text_note_citation_;
    office_element_ptr		text_note_body_;  
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(note);
//-------------------------------------------------------------------------------------------------------------------
// text:endnote openoffice xml 1.0
//-------------------------------------------------------------------------------------------------------------------
class endnote : public note
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeTextNote;

	CPDOCCORE_DEFINE_VISITABLE();
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

	endnote()
	{
		text_note_class_ = odf_types::noteclass::Endnote;
	}
private:
	virtual void add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(endnote);
//-------------------------------------------------------------------------------------------------------------------
// text:footnote openoffice xml 1.0
//-------------------------------------------------------------------------------------------------------------------
class footnote : public note
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeTextNote;

	CPDOCCORE_DEFINE_VISITABLE();
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

	footnote()
	{
		text_note_class_ = odf_types::noteclass::Footnote;
	}
private:
	virtual void add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(footnote);
//-------------------------------------------------------------------------------------------------------------------
// text:ruby
//-------------------------------------------------------------------------------------------------------------------
class ruby : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextRuby;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    std::wstring		text_style_name_;
    office_element_ptr	text_ruby_base_;
    office_element_ptr	text_ruby_text_;
  
};
CP_REGISTER_OFFICE_ELEMENT2(ruby);
//-------------------------------------------------------------------------------------------------------------------
// text:title
//-------------------------------------------------------------------------------------------------------------------
class title : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTitle;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

};

CP_REGISTER_OFFICE_ELEMENT2(title);

// text:chapter
//////////////////////////////////////////////////////////////////////////////////////////////////
class chapter : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextChapter;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(chapter);
//-------------------------------------------------------------------------------------------------------------------
//text:subject
//-------------------------------------------------------------------------------------------------------------------
class subject : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSubject;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;     
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(subject);
//-------------------------------------------------------------------------------------------------------------------
// text:placeholder
//-------------------------------------------------------------------------------------------------------------------
class text_placeholder : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPlaceholder;
    
	CPDOCCORE_DEFINE_VISITABLE();
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(std::wstring)	text_description_;
	_CP_OPT(std::wstring)	text_placeholder_type_;
	office_element_ptr		text_;       
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(text_placeholder);
//-------------------------------------------------------------------------------------------------------------------
// text:page-number
//-------------------------------------------------------------------------------------------------------------------
class text_page_number: public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPageNumber;
   
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 
  
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

 	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::style_numformat)	style_num_format_;
	_CP_OPT(odf_types::Bool)			style_num_letter_sync_;
	_CP_OPT(odf_types::Bool)			text_fixed_;
	_CP_OPT(int)						text_page_adjust_;
	_CP_OPT(std::wstring)				text_select_page_; //todooo to type

	office_element_ptr					text_;        
};

CP_REGISTER_OFFICE_ELEMENT2(text_page_number);
//-------------------------------------------------------------------------------------------------------------------
// text:page-count
//-------------------------------------------------------------------------------------------------------------------
class text_page_count : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextPageCount;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

 	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(odf_types::style_numformat)	style_num_format_;
	_CP_OPT(odf_types::Bool)			style_num_letter_sync_;
  
	office_element_ptr			text_;    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_page_count);
//-------------------------------------------------------------------------------------------------------------------
// text:date 
//-------------------------------------------------------------------------------------------------------------------
class text_date : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextDate;
    
	CPDOCCORE_DEFINE_VISITABLE();
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

 	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);
	std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
	
	_CP_OPT(std::wstring)		style_data_style_name_;
	_CP_OPT(odf_types::Bool)	text_fixed_;
	_CP_OPT(std::wstring)		text_date_value_;//with format
  
	office_element_ptr			text_;    

private:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
	void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    void add_text(const std::wstring & Text);
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_date);
//-------------------------------------------------------------------------------------------------------------------
// text:modification-date
//-------------------------------------------------------------------------------------------------------------------
class text_modification_date : public text_date
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextModificationDate;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);
	
	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(odf_types::Bool)text_fixed_;
	_CP_OPT(std::wstring)	text_date_value_;//with format
  
	office_element_ptr		text_;    
};

CP_REGISTER_OFFICE_ELEMENT2(text_modification_date);

//-------------------------------------------------------------------------------------------------------------------
// text:time 
//-------------------------------------------------------------------------------------------------------------------
class text_time : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTime;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 
   
	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

 	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);
   
	std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(odf_types::Bool)text_fixed_;
	_CP_OPT(std::wstring)	text_time_value_;//with format
  
	office_element_ptr		text_;    

private:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
	void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    void add_text(const std::wstring & Text);
};

CP_REGISTER_OFFICE_ELEMENT2(text_time);

//-------------------------------------------------------------------------------------------------------------------
// text:modification-time
//-------------------------------------------------------------------------------------------------------------------
class text_modification_time : public text_time
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextModificationTime;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);
	
	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(odf_types::Bool)text_fixed_;
	_CP_OPT(std::wstring)	text_time_value_;//with format
  
	office_element_ptr		text_;    
};

CP_REGISTER_OFFICE_ELEMENT2(text_modification_time);

//-------------------------------------------------------------------------------------------------------------------
// text:file-name 
//-------------------------------------------------------------------------------------------------------------------
class text_file_name : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextFileName;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 
   
	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

 	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

  	_CP_OPT(odf_types::Bool)text_fixed_;
	office_element_ptr		text_;    
	//text:display    
};
CP_REGISTER_OFFICE_ELEMENT2(text_file_name);
//-------------------------------------------------------------------------------------------------------------------
// text:hidden-paragraph
//-------------------------------------------------------------------------------------------------------------------
class hidden_paragraph : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextHiddenParagraph;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(odf_types::Bool)	is_hidden_;
	_CP_OPT(std::wstring)		condition_;

	std::wstring				content_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(hidden_paragraph);
//-------------------------------------------------------------------------------------------------------------------
// text:hidden-text
//-------------------------------------------------------------------------------------------------------------------
class hidden_text : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextHiddenText;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(odf_types::Bool)	is_hidden_;
	_CP_OPT(std::wstring)		condition_;
	_CP_OPT(std::wstring)		string_value_;

	std::wstring				content_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(hidden_text);
//-------------------------------------------------------------------------------------------------------------------
// text:sequence
//-------------------------------------------------------------------------------------------------------------------
class sequence : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSequence;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(odf_types::style_numformat)	style_num_format_;
	_CP_OPT(std::wstring)				style_num_letter_sync_;
	_CP_OPT(std::wstring)				formula_;
	_CP_OPT(std::wstring)				name_;
	_CP_OPT(std::wstring)				ref_name_;

	_CP_OPT(std::wstring)		template_;
    office_element_ptr_array	text_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(sequence);
//-------------------------------------------------------------------------------------------------------------------
// text:expression
//-------------------------------------------------------------------------------------------------------------------
class expression: public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextExpression;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(std::wstring)						style_data_style_name_;
	_CP_OPT(std::wstring)						text_display_;
	_CP_OPT(std::wstring)						text_formula_;
	odf_types::common_value_and_type_attlist	office_value_;
	
	std::wstring text_; 
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(expression);
//-------------------------------------------------------------------------------------------------------------------
// text:text-input
//-------------------------------------------------------------------------------------------------------------------
class text_input: public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextTextInput;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(std::wstring)	text_description_;
	std::wstring			text_; 
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(text_input);
//-------------------------------------------------------------------------------------------------------------------
// text:sequence_ref
//-------------------------------------------------------------------------------------------------------------------
class sequence_ref : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSequenceRef;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(std::wstring)	reference_format_;//caption, category-and-value, value, chapter, direction, page, text, number, number-all-superior, number-no-superior
	_CP_OPT(std::wstring)	ref_name_;
   
	office_element_ptr		text_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(sequence_ref);
//-------------------------------------------------------------------------------------------------------------------
//text:drop-down
//-------------------------------------------------------------------------------------------------------------------
class text_drop_down : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextDropDown;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring)		text_name_;
    office_element_ptr_array	content_;
    
    std::wstring				text_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_drop_down);
//-------------------------------------------------------------------------------------------------------------------
//text:label
//-------------------------------------------------------------------------------------------------------------------
class text_label : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextLabel;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

	_CP_OPT(std::wstring) text_value_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_label);
//-------------------------------------------------------------------------------------------------------------------
// text:sheet-name
//-------------------------------------------------------------------------------------------------------------------
class sheet_name : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSheetName;
    
	CPDOCCORE_DEFINE_VISITABLE();    
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	virtual void docx_convert(oox::docx_conversion_context & Context);
 	
	virtual void xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	office_element_ptr	text_;    
    
};
CP_REGISTER_OFFICE_ELEMENT2(sheet_name);
//-------------------------------------------------------------------------------------------------------------------
// text:author-name
//-------------------------------------------------------------------------------------------------------------------
class author_name : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextAuthorName;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(author_name);
//-------------------------------------------------------------------------------------------------------------------
// text:author-initials
//-------------------------------------------------------------------------------------------------------------------
class author_initials : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextAuthorInitials;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(author_initials);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-city
//-------------------------------------------------------------------------------------------------------------------
class sender_city : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderCity;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_city);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-postal-code
//-------------------------------------------------------------------------------------------------------------------
class sender_postal_code : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderPostalCode;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
	
	virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_postal_code);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-street
//-------------------------------------------------------------------------------------------------------------------
class sender_street : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderStreet;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_street);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-state-or-province
//-------------------------------------------------------------------------------------------------------------------
class sender_state_or_province : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderStateOrProvince;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_state_or_province);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-email
//-------------------------------------------------------------------------------------------------------------------
class sender_email : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderEmail;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_email);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-firstname
//-------------------------------------------------------------------------------------------------------------------
class sender_firstname : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderFirstname;
    
	CPDOCCORE_DEFINE_VISITABLE();    
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_firstname);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-lastname
//-------------------------------------------------------------------------------------------------------------------
class sender_lastname : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderLastname;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_lastname);
//-------------------------------------------------------------------------------------------------------------------
// text:sender-company
//-------------------------------------------------------------------------------------------------------------------
class sender_company : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextSenderCompany;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(odf_types::Bool)	text_fixed_;
	office_element_ptr			text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(sender_company);

//-------------------------------------------------------------------------------------------------------------------
//text:user-field-get
//---------------------------------------------------------------------------------------------------
class text_user_field_get : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUserFieldGet;
    
	CPDOCCORE_DEFINE_VISITABLE()
	CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring)	style_data_style_name_;
	_CP_OPT(std::wstring)	text_display_;
	_CP_OPT(std::wstring)	text_name_;
	
	office_element_ptr		text_;    
};
CP_REGISTER_OFFICE_ELEMENT2(text_user_field_get);
//-------------------------------------------------------------------------------------------------------------------
//text:user-defined
//---------------------------------------------------------------------------------------------------
class text_user_defined : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTextUserDefined;
    
	CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    virtual void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);

	_CP_OPT(std::wstring)						style_data_style_name_;
	_CP_OPT(std::wstring)						text_name_;
	_CP_OPT(odf_types::Bool)					text_fixed_;
	odf_types::common_value_and_type_attlist	office_value_;
	
	office_element_ptr	text_;    

};
CP_REGISTER_OFFICE_ELEMENT2(text_user_defined);
//---------------------------------------------------------------------------------------------------
//text:bibliography-mark
//---------------------------------------------------------------------------------------------------
class bibliography_mark : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextBibliographyMark;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context) ;

	void serialize(std::wostream & strm);

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    std::wstring			identifier_;
	odf_types::bibliography	bibliography_type_; 
    
	_CP_OPT(std::wstring)	url_;
    _CP_OPT(std::wstring)	author_;
    _CP_OPT(std::wstring)	title_;
    _CP_OPT(std::wstring)	year_;
    _CP_OPT(std::wstring)	isbn_;
	_CP_OPT(std::wstring)	chapter_;
	_CP_OPT(std::wstring)	address_;
	_CP_OPT(std::wstring)	annote_;
	_CP_OPT(std::wstring)	booktitle_;
	_CP_OPT(std::wstring)	edition_;
	_CP_OPT(std::wstring)	editor_;
	_CP_OPT(std::wstring)	howpublished_;
	_CP_OPT(std::wstring)	institution_;
	_CP_OPT(std::wstring)	issn_;
	_CP_OPT(std::wstring)	journal_;
	_CP_OPT(std::wstring)	month_;
	_CP_OPT(std::wstring)	note_;
	_CP_OPT(std::wstring)	number_;
	_CP_OPT(std::wstring)	organizations_;
	_CP_OPT(std::wstring)	pages_;
	_CP_OPT(std::wstring)	publisher_;
	_CP_OPT(std::wstring)	report_type_;
	_CP_OPT(std::wstring)	school_;
	_CP_OPT(std::wstring)	series_;
	_CP_OPT(std::wstring)	volume_;

	office_element_ptr		text_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text			(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(bibliography_mark);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-auto-mark-file
//---------------------------------------------------------------------------------------------------
class alphabetical_index_auto_mark_file : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextAlphabeticalIndexAutoMarkFile;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

    odf_types::common_xlink_attlist xlink_attlist_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_auto_mark_file);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-mark-start
//---------------------------------------------------------------------------------------------------
class alphabetical_index_mark_start : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextAlphabeticalIndexMarkStart;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	std::wstring id_;
	_CP_OPT(std::wstring)		key1_;
	_CP_OPT(std::wstring)		key1_phonetic_;
	_CP_OPT(std::wstring)		key2_;
	_CP_OPT(std::wstring)		key2_phonetic_;
	_CP_OPT(odf_types::Bool)	main_entry_;
	_CP_OPT(std::wstring)		string_value_phonetic_;
private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_mark_start);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-mark-end
//---------------------------------------------------------------------------------------------------
class alphabetical_index_mark_end : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextAlphabeticalIndexMarkEnd;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	std::wstring id_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_mark_end);
//---------------------------------------------------------------------------------------------------
//text:alphabetical-index-mark
//---------------------------------------------------------------------------------------------------
class alphabetical_index_mark : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextAlphabeticalIndexMark;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(std::wstring)		key1_;
	_CP_OPT(std::wstring)		key1_phonetic_;
	_CP_OPT(std::wstring)		key2_;
	_CP_OPT(std::wstring)		key2_phonetic_;
	_CP_OPT(odf_types::Bool)	main_entry_;
	_CP_OPT(std::wstring)		string_value_;
	_CP_OPT(std::wstring)		string_value_phonetic_;
private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(alphabetical_index_mark);
//---------------------------------------------------------------------------------------------------
//text:user-index-mark-start
//---------------------------------------------------------------------------------------------------
class user_index_mark_start : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextUserIndexMarkStart;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	std::wstring			id_;
	_CP_OPT(std::wstring)	index_name_;
    _CP_OPT(int)			outline_level_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(user_index_mark_start);
//---------------------------------------------------------------------------------------------------
//text:user-index-mark-end
//---------------------------------------------------------------------------------------------------
class user_index_mark_end : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextUserIndexMarkEnd;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	std::wstring id_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(user_index_mark_end);
//---------------------------------------------------------------------------------------------------
//text:user-index-mark
//---------------------------------------------------------------------------------------------------
class user_index_mark : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextUserIndexMark;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(std::wstring)	index_name_;
    _CP_OPT(int)			outline_level_;
	_CP_OPT(std::wstring)	string_value_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(user_index_mark);
//---------------------------------------------------------------------------------------------------
//text:toc-mark-start
//---------------------------------------------------------------------------------------------------
class toc_mark_start : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextTocMarkStart;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

    _CP_OPT(int)	outline_level_;
	std::wstring	id_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(toc_mark_start);
//---------------------------------------------------------------------------------------------------
//text:toc-mark-end
//---------------------------------------------------------------------------------------------------
class toc_mark_end : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextTocMarkEnd;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	std::wstring	id_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(toc_mark_end);
//---------------------------------------------------------------------------------------------------
//text:toc-mark
//---------------------------------------------------------------------------------------------------
class toc_mark : public paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType	xml_type	= xml::typeElement;
    static const ElementType	type		= typeTextTocMark;    
	
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);

	_CP_OPT(std::wstring)	string_value_;
    _CP_OPT(int)			outline_level_;

private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(toc_mark);
//-------------------------------------------------------------------------------------------------------------------
} // namespace text
//-------------------------------------------------------------------------------------------------------------------
//presentation:footer
//-------------------------------------------------------------------------------------------------------------------
class presentation_footer : public text::paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationFooter;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	//void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_footer);

//-------------------------------------------------------------------------------------------------------------------
//presentation:date-time
//-------------------------------------------------------------------------------------------------------------------
class presentation_date_time: public text::paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationDateTime;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	//void docx_convert(oox::docx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    
};
CP_REGISTER_OFFICE_ELEMENT2(presentation_date_time);


//-------------------------------------------------------------------------------------------------------------------
// field:fieldmark-start 
//-------------------------------------------------------------------------------------------------------------------
class field_fieldmark_start : public text::paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFieldFieldmarkStart;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    _CP_OPT(std::wstring)	text_name_;
	_CP_OPT(std::wstring)	field_type_;
	
	void docx_convert(oox::docx_conversion_context & Context);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
};
CP_REGISTER_OFFICE_ELEMENT2(field_fieldmark_start);

//-------------------------------------------------------------------------------------------------------------------
// field:fieldmark-end 
//-------------------------------------------------------------------------------------------------------------------
class field_fieldmark_end : public text::paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFieldFieldmarkStart;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

	void docx_convert(oox::docx_conversion_context & Context);
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
};
CP_REGISTER_OFFICE_ELEMENT2(field_fieldmark_end);

//-------------------------------------------------------------------------------------------------------------------
// field:fieldmark 
//-------------------------------------------------------------------------------------------------------------------
class field_fieldmark : public text::paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFieldFieldmark;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    _CP_OPT(std::wstring)	text_name_;
	_CP_OPT(std::wstring)	field_type_;

	office_element_ptr_array	field_params_;

	void docx_convert(oox::docx_conversion_context & Context);
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(field_fieldmark);

//-------------------------------------------------------------------------------------------------------------------
// field:param
//-------------------------------------------------------------------------------------------------------------------
class field_param : public text::paragraph_content_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeFieldParam;
    
	CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_; 

    _CP_OPT(std::wstring)	field_name_;
	_CP_OPT(std::wstring)	field_value_;

	void docx_convert(oox::docx_conversion_context & Context);
private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
};
CP_REGISTER_OFFICE_ELEMENT2(field_param);
} // namespace odf_reader
} // namespace cpdoccore

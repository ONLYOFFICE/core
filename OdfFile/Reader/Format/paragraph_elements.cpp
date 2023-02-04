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

#include "paragraph_elements.h"

#include <ostream>
#include <string>

#include <boost/algorithm/string.hpp>

#include <xml/xmlchar.h>

#include <xml/utils.h>
#include "../../Common/readstring.h"
#include "odf_document.h"

#include "styles.h"
#include "note.h"
#include "ruby.h"
#include "office_elements_create.h"

#include "serialize_elements.h"
#include "odfcontext.h"

#include "style_text_properties.h"

#include "../../DataTypes/targetframename.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

namespace text {

//------------------------------------------------------------------------------------------------------------
const wchar_t * paragraph_content_element::ns = L"";
const wchar_t * paragraph_content_element::name = L"";

void paragraph_content_element::docx_serialize_field(const std::wstring & field_name, office_element_ptr & text, 
															   oox::docx_conversion_context & Context, bool bLock )
{
	std::wostream & strm = Context.output_stream();
	Context.finish_run();	
	
	if (false == field_name.empty())
	{
		strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"";
		if (bLock)
		{
			strm << L" w:fldLock=\"1\"";
		}
		strm << L"/></w:r>";
		strm << L"<w:r><w:instrText>" << field_name << L"</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	}

	docx_serialize_run(text, Context);
 	
	if (false == field_name.empty())
	{
	   strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
	}
}
void paragraph_content_element::docx_serialize_sdt_placeholder(const std::wstring & name, office_element_ptr & text, oox::docx_conversion_context & Context)
{
	std::wostream & strm = Context.output_stream();
	Context.finish_run();	

	strm << L"<w:sdt><w:sdtPr><w:alias w:val=\"";
	strm << name;
	strm << L"\"/><w:temporary/>";
	strm << L"<w:showingPlcHdr/><w:text/></w:sdtPr><w:sdtContent>";

	docx_serialize_run(text, Context);

	strm << L"</w:sdtContent></w:sdt>";
}

void paragraph_content_element::docx_serialize_run(office_element_ptr & text, oox::docx_conversion_context & Context)
{
	Context.add_new_run();
	if (text)
	{
		text->docx_convert(Context);
	}
	Context.finish_run();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text::ns = L"";
const wchar_t * text::name = L"";

std::wostream & text::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	_Wostream << (bXmlEncode ? XmlUtils::EncodeXmlString( text_, true ) : text_);
    return _Wostream;
}
void text::add_space(const std::wstring & Text) 
{
	text_.reserve(Text.length());
	for (size_t i =0; i < Text.length(); i++)
	{
		if (Text[i] < 0x20) continue;
		text_ += Text[i];
	}
}

void text::add_text(const std::wstring & Text) 
{
	text_.reserve(Text.length());
	for (size_t i =0; i < Text.length(); i++)
	{
		if (Text[i] < 0x20) continue;
		text_ += Text[i];
	}
}
text::text(const std::wstring & Text) : preserve_(true)
{
    add_text(Text);
}
void text::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_process_note() != oox::docx_conversion_context::noNote && 
		Context.get_delete_text_state()) 
			return; //в ms нет рецензирования notes

	bool add_del_run = false;
	if (Context.get_drawing_state_content() && Context.get_delete_text_state())
	{	//0503IG-AddingFormattingText.odt - удаленый текст в удаленом объекте

		oox::text_tracked_context::_state  &state = Context.get_text_tracked_context().get_tracked_change(L"");
		if (state.type == 2)
		{
			add_del_run = true;
			Context.output_stream() << L"<w:del>";
		}
	}  
	if (Context.is_table_content())
	{
		int type = Context.get_table_content_context().get_type_current_content_template_index();
		if (type == 6)
		{
		}
	}	
	Context.add_element_to_run();
  
	std::wstring textNode = L"w:t";

	if (Context.get_delete_text_state()) textNode = L"w:delText";

	Context.output_stream() << L"<" << textNode;
	if (preserve_ && !Context.get_delete_text_state()) 
		Context.output_stream() << L" xml:space=\"preserve\"";
	Context.output_stream() << L">";

	Context.output_stream() << XmlUtils::EncodeXmlString( text_, true );//0xf4 0x80 0x80 0x81-??? - Gangs_Aff-Neg.odt
    Context.output_stream() << L"</" << textNode << L">";

	if (add_del_run)
	{
		Context.finish_run();
		Context.output_stream() << L"</w:del>";
	}

}

void text::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.get_text_context().add_text(text_);
}
void text::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().add_text(text_);
}
office_element_ptr text::create(const std::wstring & Text)
{
    return boost::make_shared<text>(Text);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * s::ns = L"text";
const wchar_t * s::name = L"s";

std::wostream & s::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    if (text_c_)
    {
        _Wostream << std::wstring(*text_c_, L' ');
    }
    else
        _Wostream << std::wstring(1, L' ');

	if (content_)
		_Wostream << *content_;

    return _Wostream;
}

void s::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:c", text_c_);
}
void s::add_text(const std::wstring & Text)
{
    content_ = Text;
}
void s::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();

	std::wostream & strm = Context.output_stream();

	std::wstring textNode = L"w:t";
	if (Context.get_delete_text_state()) textNode = L"w:delText";

	strm << L"<" << textNode;
	if (!Context.get_delete_text_state())	
		strm << L" xml:space=\"preserve\"";
	strm << L">";
		text_to_stream(strm);
    strm << L"</" << textNode << L">"; 
}

void s::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void s::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * tab_stop::ns = L"text";
const wchar_t * tab_stop::name = L"tab-stop";

//------------------------------------------------------------------------------------------------------------
const wchar_t * tab::ns = L"text";
const wchar_t * tab::name = L"tab";

std::wostream & tab::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream << std::wstring(L"\t");
    return _Wostream;
}

void tab::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:tab-ref", text_tab_ref_);
}

void tab::docx_convert(oox::docx_conversion_context & Context)
{
    Context.add_element_to_run();
  
	std::wostream & _Wostream = Context.output_stream();
    _Wostream << L"<w:tab/>";
}

void tab::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    Context.get_text_context().add_text(L"\t");
}
void tab::pptx_convert(oox::pptx_conversion_context & Context) 
{
    Context.get_text_context().add_text(L"\t");
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * line_break::ns = L"text";
const wchar_t * line_break::name = L"line-break";

std::wostream & line_break::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream << std::wstring(L"\r\n");
    return _Wostream;
}
void line_break::docx_convert(oox::docx_conversion_context & Context)
{
	bool in_drawing	= false;

 	if (Context.get_drawing_context().get_current_shape() || Context.get_drawing_context().get_current_frame())
	{
		in_drawing = true;

		Context.finish_run();
		Context.finish_paragraph();
		Context.start_paragraph();
		Context.process_paragraph_style(Context.get_current_paragraph_style());
	}
	else
	{
		Context.add_element_to_run();
		Context.output_stream() << L"<w:br/>";
	}
}
void line_break::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.get_text_context().add_text(L"\n");
}
void line_break::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().add_text(L"\n");
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark::ns = L"text";
const wchar_t * bookmark::name = L"bookmark";

void bookmark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}
std::wostream & bookmark::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}
void bookmark::docx_convert(oox::docx_conversion_context & Context)
{
	Context.start_bookmark(text_name_);
	Context.end_bookmark(text_name_);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark_start::ns = L"text";
const wchar_t * bookmark_start::name = L"bookmark-start";

void bookmark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", name_, std::wstring(L""));
}
void bookmark_start::docx_convert(oox::docx_conversion_context & Context)
{
	Context.start_bookmark(name_);
}
std::wostream & bookmark_start::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark_end::ns = L"text";
const wchar_t * bookmark_end::name = L"bookmark-end";

void bookmark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", name_, std::wstring(L""));
}
void bookmark_end::docx_convert(oox::docx_conversion_context & Context)
{
	Context.end_bookmark(name_);
}
std::wostream & bookmark_end::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * bookmark_ref::ns = L"text";
const wchar_t * bookmark_ref::name = L"bookmark-ref";

void bookmark_ref::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:ref-name", ref_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"text:reference-format", reference_format_);
}
void bookmark_ref::add_text(const std::wstring & Text)
{
    content_ = Text;
}
// text:reference-ref
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_ref::ns = L"text";
const wchar_t * reference_ref::name = L"reference-ref";

void reference_ref::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:ref-name", ref_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"text:reference-format", reference_format_);
}
void reference_ref::add_text(const std::wstring & Text)
{
    content_ = Text;
}
// text:reference-mark
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark::ns = L"text";
const wchar_t * reference_mark::name = L"reference-mark";

void reference_mark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:reference-mark-start
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark_start::ns = L"text";
const wchar_t * reference_mark_start::name = L"reference-mark-start";

void reference_mark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}

// text:reference-mark-end
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * reference_mark_end::ns = L"text";
const wchar_t * reference_mark_end::name = L"reference-mark-end";

void reference_mark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_, std::wstring(L""));
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * hidden_paragraph::ns = L"text";
const wchar_t * hidden_paragraph::name = L"hidden-paragraph";

void hidden_paragraph::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:is-hidden", is_hidden_);
    CP_APPLY_ATTR(L"text:condition", condition_);
}
void hidden_paragraph::add_text(const std::wstring & Text)
{
    content_ = Text;
}
void hidden_paragraph::docx_convert(oox::docx_conversion_context & Context)
{
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * hidden_text::ns = L"text";
const wchar_t * hidden_text::name = L"hidden-text";

void hidden_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:is-hidden", is_hidden_);
    CP_APPLY_ATTR(L"text:condition", condition_);
    CP_APPLY_ATTR(L"text:string-value", string_value_);
}
void hidden_text::add_text(const std::wstring & Text)
{
    content_ = Text;
}
void hidden_text::docx_convert(oox::docx_conversion_context & Context)
{
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * span::ns = L"text";
const wchar_t * span::name = L"span";

std::wostream & span::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void span::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:style-name", text_style_name_, std::wstring(L""));
    
    std::wstring classNames = Attributes->get_val< std::wstring >(L"text:class-names").get_value_or(L"");
    std::vector< std::wstring > classNamesArray;

    if (false == classNames.empty())
    {
        boost::algorithm::split(classNamesArray, classNames, boost::algorithm::is_any_of(L" "));

        for (size_t i = 0; i < classNamesArray.size(); i++)
        {
            text_class_names_.push_back( classNamesArray[i] );        
        }
    }
}

void span::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void span::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text);
    content_.push_back( elm );
}
void span::add_space(const std::wstring & Text) 
{
    office_element_ptr elm = text::create(Text);
    content_.push_back( elm );
}
void span::docx_convert(oox::docx_conversion_context & Context)
{
	bool addNewRun = false;
    bool pushed = false;

	if (!content_.empty() && (content_[0]->get_type() == typeTextNote))
	{
		pushed = pushed;
	}

    std::wostream & _Wostream = Context.output_stream();

    if (!text_style_name_.empty()/* && !drawing*/)
    {
        if (style_instance *styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text, Context.process_headers_footers_))
        {
            if (styleInst->is_automatic())
            {
                if (const style_content *styleContent = styleInst->content())
                {
					style_text_properties *text_props = styleContent->get_style_text_properties();
					std::wstring parent = styleInst->parent_name();

					if (false == parent.empty())
					{
						text_props->content_.r_style_ = Context.styles_map_.get(parent, styleInst->type());
					}
                    
					Context.push_text_properties(text_props);
                    pushed = true;
                    Context.get_styles_context().start_process_style(styleInst);
					Context.add_new_run();
                    Context.get_styles_context().end_process_style();
                    addNewRun = true;

					if (text_props)
						text_props->content_.r_style_ = boost::none;
                }                            
            }
            else
            {
                const std::wstring id = Context.styles_map_.get( styleInst->name(), styleInst->type() );
                Context.add_new_run(id);
                addNewRun = true;
            }                        
        }
                                     
    }


    if (!addNewRun)Context.add_new_run();

    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }

	Context.finish_run();
	
	if (pushed)
        Context.pop_text_properties();
}

void span::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_span(text_style_name_);
    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
    Context.end_span();
}
void span::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	if (content_.empty()) return;

	if (false == text_style_name_.empty())
	{
	    if (style_instance *styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text, true))
		{
			if (style_content* style = styleInst->content())
			{
				style->xlsx_serialize(_Wostream, Context);
			}
		}
	}
    for (size_t i = 0; i < content_.size(); i++)
    {
		text *t = dynamic_cast<text*>(content_[i].get());
		if (t)
		{
			t->text_to_stream(_Wostream, true);
		}
		else
		{
			text::paragraph_content_element *element = dynamic_cast<text::paragraph_content_element*>(content_[i].get());
			if (element)
			{
				element->xlsx_serialize(_Wostream, Context);
			}
		}
    }
}
void span::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (style_instance * styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text, false))
		Context.get_text_context().get_styles_context().start_process_style(styleInst);
   
	Context.get_text_context().start_span(text_style_name_);
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
    Context.get_text_context().end_span();
	Context.get_text_context().get_styles_context().end_process_style();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * a::ns = L"text";
const wchar_t * a::name = L"a";

std::wostream & a::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void a::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"office:name",				office_name_,			std::wstring(L""));
    CP_APPLY_ATTR(L"office:target-frame-name",	office_target_frame_name_);
    CP_APPLY_ATTR(L"text:style-name",			text_style_name_,		std::wstring(L""));
    CP_APPLY_ATTR(L"text:visited-style-name",	text_visited_style_name_,std::wstring(L""));
}

void a::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void a::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void a::add_space(const std::wstring & Text) 
{
    office_element_ptr elm = text::create(Text);
    content_.push_back( elm );
}
void a::docx_convert(oox::docx_conversion_context & Context)
{
    bool pushed_style = false;
    
	bool addNewRun = false;
    Context.finish_run();
    
    std::wostream & _Wostream = Context.output_stream();

	std::wstring ref = xlink_attlist_.href_.get_value_or(L"");
	
	if (Context.is_table_content())
	{
		size_t pos_outline = ref.find(L"|outline");
		if (std::wstring::npos != pos_outline)//без #
		{
			std::wstringstream strm; 
			text_to_stream(strm, false);
			std::wstring outline = strm.str();

			ref = L"_TOC_" + std::to_wstring(0x4321001 + Context.get_table_content_context().mapReferences.size());
			
			Context.get_table_content_context().mapReferences.insert(std::make_pair(outline, ref));
		}
		else
		{
			ref = XmlUtils::EncodeXmlString(ref.substr(1));
		}

		_Wostream << L"<w:hyperlink w:anchor=\"" << ref << L"\" w:history=\"1\">"; 
		int type = Context.get_table_content_context().get_type_current_content_template_index();
		//type == 3 (LinkStart)
		Context.get_table_content_context().next_level_index();
	}
	else
	{
		std::wstring rId;
		rId = Context.add_hyperlink(ref, false);

		_Wostream << L"<w:hyperlink r:id=\"" << rId << L"\">";
	}

    style_instance * styleInst = NULL;
 	style_text_properties_ptr tempStyleTextProp;
   
    if (!text_style_name_.empty())
        styleInst = Context.root()->odf_context().styleContainer().style_by_name(text_style_name_, style_family::Text, Context.process_headers_footers_);
    else if (false == Context.is_table_content())
        styleInst = Context.root()->odf_context().styleContainer().hyperlink_style();
        
    if (styleInst)
    {
        if (styleInst->is_automatic())
        {
            if (const style_content * styleContent = styleInst->content())
            {
                Context.push_text_properties(styleContent->get_style_text_properties());
                pushed_style = true;
                Context.get_styles_context().start_process_style(styleInst);
				Context.add_new_run();
                Context.get_styles_context().end_process_style();
                addNewRun = true;
            }                            
        }
        else
        {
			const std::wstring id = Context.styles_map_.get( styleInst->name(), styleInst->type() );
            tempStyleTextProp = style_text_properties_ptr( new style_text_properties(id) );
            Context.push_text_properties( tempStyleTextProp.get() );
            pushed_style = true;

            Context.add_new_run();
            addNewRun = true;
        }                        
    }   

    if (!addNewRun)
        Context.add_new_run();

    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
 		if (Context.is_table_content())
		{
			Context.get_table_content_context().next_level_index();
		}
	}

	Context.finish_run();
	
	if (pushed_style)
        Context.pop_text_properties();
    _Wostream << L"</w:hyperlink>";
}

void a::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_hyperlink(text_style_name_);
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->xlsx_convert(Context);
    }
    Context.end_hyperlink(xlink_attlist_.href_.get_value_or(L""));
}
void a::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().start_hyperlink();
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
	
	std::wstring hId = Context.get_slide_context().add_hyperlink(xlink_attlist_.href_.get_value_or(L""));
	Context.get_text_context().end_hyperlink(hId);

}
//------------------------------------------------------------------------------------------------------------
const wchar_t * endnote::ns = L"text";
const wchar_t * endnote::name = L"endnote";

void endnote::add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"endnote-citation")
	{
		create_element_and_read(Reader, L"text", L"note-citation", text_note_citation_, getContext());
	}
	else if CP_CHECK_NAME(L"text", L"endnote-body")
	{
		create_element_and_read(Reader, L"text", L"note-body", text_note_body_, getContext());
	}
	else
		CP_NOT_APPLICABLE_ELM();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * footnote::ns = L"text";
const wchar_t * footnote::name = L"footnote";

void footnote::add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"footnote-citation")
	{
		create_element_and_read(Reader, L"text", L"note-citation", text_note_citation_, getContext());
	}
	else if CP_CHECK_NAME(L"text", L"footnote-body")
	{
		create_element_and_read(Reader, L"text", L"note-body", text_note_body_, getContext());
	}
	else
		CP_NOT_APPLICABLE_ELM();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * note::ns = L"text";
const wchar_t * note::name = L"note";

note::note()
{}

std::wostream & note::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    if (text_note_citation_)
        text_note_citation_->text_to_stream(_Wostream, bXmlEncode);

    if (text_note_body_)
        text_note_body_->text_to_stream(_Wostream, bXmlEncode);

    return _Wostream;
}

void note::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", text_id_, std::wstring(L""));
    CP_APPLY_ATTR(L"text:note-class", text_note_class_, noteclass(noteclass::Footnote));
}

void note::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"note-citation")
    {
        CP_CREATE_ELEMENT(text_note_citation_);
    }
    else if CP_CHECK_NAME(L"text", L"note-body")
    {
        CP_CREATE_ELEMENT(text_note_body_);        
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
void note::pptx_convert(oox::pptx_conversion_context & Context)
{
	//см presentation:notes
}
void note::docx_convert(oox::docx_conversion_context & Context)
{
    //bool addNewRun = false;
	const text_notes_configuration *notes_conf = Context.root()->odf_context().noteConfiguration().getConfiguration(text_note_class_.get_type());
    if (notes_conf)
    {
        const std::wstring styleName = notes_conf->text_citation_body_style_name_.get_value_or(L"");
        if (!styleName.empty())
        {
			style_instance * styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, style_family::Text, Context.process_headers_footers_);
            if (styleInst)
            {
                const std::wstring styleId = Context.styles_map_.get( styleInst->name(), styleInst->type() );
				Context.add_new_run(styleId);
                //addNewRun = true;
            }
                                         
        }
    }
    if (false == Context.get_run_state())
        Context.add_new_run();

	Context.get_notes_context().set_current_note(text_note_class_.get_type(), dynamic_cast<const note_citation *>(text_note_citation_.get()));

    if (text_note_class_.get_type() == noteclass::Footnote)
    {
	   Context.output_stream() << "<w:footnoteReference w:id=\"" << Context.get_notes_context().next_id() << "\"/>";
    }
    else 
    {
		Context.output_stream() << "<w:endnoteReference w:id=\"" << Context.get_notes_context().next_id() << "\"/>";
    }

    if (text_note_citation_)
        text_note_citation_->docx_convert(Context);

    Context.set_process_note( (text_note_class_.get_type() == noteclass::Footnote) ? oox::docx_conversion_context::footNote : oox::docx_conversion_context::endNote );
    if (text_note_body_)
        text_note_body_->docx_convert(Context);
    Context.set_process_note(oox::docx_conversion_context::noNote);

	//Context.finish_run();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * ruby::ns = L"text";
const wchar_t * ruby::name = L"ruby";

std::wostream & ruby::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    if (text_ruby_base_)
        text_ruby_base_->text_to_stream(_Wostream, bXmlEncode);

    if (text_ruby_text_)
        text_ruby_text_->text_to_stream(_Wostream, bXmlEncode);

    return _Wostream;
}

void ruby::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_style_name_ = Attributes->get_val< std::wstring >(L"text:style-name").get_value_or(L"") ;
}

void ruby::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"ruby-base")
    {
        CP_CREATE_ELEMENT(text_ruby_base_);
    }
    else if CP_CHECK_NAME(L"text", L"ruby-text")
    {
        CP_CREATE_ELEMENT(text_ruby_text_);        
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
// text:title
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * title::ns = L"text";
const wchar_t * title::name = L"title";

std::wostream & title::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void title::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void title::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void title::docx_convert(oox::docx_conversion_context & Context)
{
 	docx_serialize_field(L"TITLE", text_, Context);
}

void title::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void title::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}

// text:chapter
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * subject::ns = L"text";
const wchar_t * subject::name = L"subject";

std::wostream & subject::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void subject::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}

void subject::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void subject::docx_convert(oox::docx_conversion_context & Context)
{
 	docx_serialize_field(L"SUBJECT", text_, Context);
}

void subject::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void subject::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * chapter::ns = L"text";
const wchar_t * chapter::name = L"chapter";

std::wostream & chapter::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void chapter::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void chapter::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void chapter::docx_convert(oox::docx_conversion_context & Context)
{
 	docx_serialize_field(L"BIBLIOGRAPHY", text_, Context);
}

void chapter::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}
void chapter::pptx_convert(oox::pptx_conversion_context & Context)
{
    std::wstringstream val;
    this->text_to_stream(val);
    Context.get_text_context().add_text(val.str());
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_placeholder::ns = L"text";
const wchar_t * text_placeholder::name = L"placeholder";

std::wostream & text_placeholder::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void text_placeholder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void text_placeholder::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_placeholder::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Click placeholder and overwrite", text_, Context);
}

void text_placeholder::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (text_)
    {
        text_->pptx_convert(Context);
    }
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_page_number::ns = L"text";
const wchar_t * text_page_number::name = L"page-number";

std::wostream & text_page_number::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void text_page_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void text_page_number::add_text(const std::wstring & Text)
{
	text_ = text::create(Text) ;

	text *t = dynamic_cast<text*>(text_.get());
	if (t) t->preserve_ = false;
}

void text_page_number::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"PAGE", text_, Context);
}
void text_page_number::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;P";
}
void text_page_number::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().start_field(oox::page_number, L"");
	
    if (text_)
    {
        text_->pptx_convert(Context);
    }

    Context.get_text_context().end_field();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_page_count::ns = L"text";
const wchar_t * text_page_count::name = L"page-count";

std::wostream & text_page_count::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void text_page_count::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    //CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void text_page_count::add_text(const std::wstring & Text)
{
	text_ = text::create(Text) ;

	text *t = dynamic_cast<text*>(text_.get());
	if (t) t->preserve_ = false;
}

void text_page_count::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"NUMPAGES", text_, Context);
}
void text_page_count::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;N";
}
void text_page_count::pptx_convert(oox::pptx_conversion_context & Context)
{
	//поскольку такого поля в ms нет - конвертим как обычный текст
	if (text_)
    {
        text_->pptx_convert(Context);
    }
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_date::ns = L"text";
const wchar_t * text_date::name = L"date";

std::wostream & text_date::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void text_date::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name", style_data_style_name_);
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
    CP_APPLY_ATTR(L"text:date-value", text_date_value_);
}

void text_date::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_date::docx_convert(oox::docx_conversion_context & Context)
{
	//bool bLock = text_fixed_ ? text_fixed_->get() : false;
	//docx_serialize_field(L"DATE", text_, Context, bLock);

	Context.finish_run();

	Context.output_stream() << L"<w:sdt>";
		Context.output_stream() << L"<w:sdtPr>";
		{
			Context.output_stream() << L"<w:date" << (text_date_value_ ?  (L" w:fullDate=\"" + *text_date_value_ + L"\"") : L"") << L">";
				
				std::wstring format;
				if (style_data_style_name_)
				{
				}
				Context.output_stream() << L"<w:dateFormat w:val=\"" << format << L"\"/>";

				Context.output_stream() << L"<w:lid w:val=\"en-US\"/>";
				Context.output_stream() << L"<w:storeMappedDataAs w:val=\"dateTime\"/>";
				Context.output_stream() << L"<w:calendar w:val=\"gregorian\"/>";				
			Context.output_stream() << L"</w:date>";
		}
		Context.output_stream() << L"</w:sdtPr>";
		Context.output_stream() << L"<w:sdtContent>";
	
		docx_serialize_run(text_, Context);

		Context.output_stream() << L"</w:sdtContent>";
	Context.output_stream() << L"</w:sdt>";
}
void text_date::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;D";
}
void text_date::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::date, style_data_style_name_.get_value_or(L""));
    if (text_)
    {
        text_->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_modification_date::ns = L"text";
const wchar_t * text_modification_date::name = L"modification-date";

void text_modification_date::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = true;//text_fixed_.get_value_or(false);

	if (asText)
	{
		docx_serialize_run(text_, Context);
	}
	else
	{
		docx_serialize_field(L"SAVEDATE \\@ \"dd.MM.yy\"", text_, Context);
	}
}
void text_modification_date::pptx_convert(oox::pptx_conversion_context & Context)
{
	text_date::pptx_convert(Context);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_time::ns = L"text";
const wchar_t * text_time::name = L"time";

std::wostream & text_time::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void text_time::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name", style_data_style_name_);

    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
    CP_APPLY_ATTR(L"text:time-value", text_time_value_);
}
void text_time::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_time::docx_convert(oox::docx_conversion_context & Context)
{
	bool bLock = text_fixed_ ? text_fixed_->get() : false;

	docx_serialize_field(L"TIME", text_, Context, bLock);
}
void text_time::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;T";
}
void text_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::time, style_data_style_name_.get_value_or(L""));
	if (text_)
    {
        text_->pptx_convert(Context);
    }
    Context.get_text_context().end_field();
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_modification_time::ns = L"text";
const wchar_t * text_modification_time::name = L"modification-time";

void text_modification_time::docx_convert(oox::docx_conversion_context & Context)
{
	bool asText = true;//text_fixed_.get_value_or(false);

	if (asText)
	{
		docx_serialize_run(text_, Context);
	}
	else
	{
		docx_serialize_field(L"SAVEDATE  \\@ \"h:mm:ss am/pm\"", text_, Context);
	}
}
void text_modification_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    text_time::pptx_convert(Context);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * text_file_name::ns = L"text";
const wchar_t * text_file_name::name = L"file-name";

std::wostream & text_file_name::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}
void text_file_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void text_file_name::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}

void text_file_name::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"FILENAME", text_, Context);
}
void text_file_name::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;F";
}
void text_file_name::pptx_convert(oox::pptx_conversion_context & Context)
{
//	Context.get_text_context().start_field(oox::file_name, style_data_style_name_.get_value_or(L""));
	if (text_)
    {
        text_->pptx_convert(Context);
    }
  //  Context.get_text_context().end_field();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sequence_ref::ns = L"text";
const wchar_t * sequence_ref::name = L"sequence-ref";

void sequence_ref::add_attributes( const xml::attributes_wc_ptr & Attributes )
{	
	CP_APPLY_ATTR(L"text:ref-name", ref_name_);
	CP_APPLY_ATTR(L"text:reference-format", reference_format_);
}
void sequence_ref::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sequence_ref::docx_convert(oox::docx_conversion_context & Context) 
{
	std::wstring ref, sequence;
	if (!ref_name_) return;

	Context.finish_run();
	Context.output_stream() << L"<w:fldSimple w:instr=\" REF " << *ref_name_ << L" \\h\">";
	Context.add_new_run();
	if (text_)
		text_->docx_convert(Context);
	Context.finish_run();

	Context.output_stream() << L"</w:fldSimple>";
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sequence::ns = L"text";
const wchar_t * sequence::name = L"sequence";

void sequence::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:num-format",		style_num_format_);
	CP_APPLY_ATTR(L"style:num-letter-sync", style_num_letter_sync_);
	CP_APPLY_ATTR(L"text:formula",			formula_);
	CP_APPLY_ATTR(L"text:ref-name",			ref_name_);
	CP_APPLY_ATTR(L"text:name",				name_);
}
std::wostream & sequence::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}
void sequence::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(text_);
}

void sequence::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    text_.push_back( elm );
}

void sequence::docx_convert(oox::docx_conversion_context & Context) 
{
	std::wstring ref, sequence;
	if (ref_name_)
	{
		sequence = Context.get_table_content_context().get_sequence(*ref_name_);
		size_t pos = ref_name_->find(L"ref" + ref);
		if (pos != std::wstring::npos)
		{
			ref = sequence + L"!" +  ref_name_->substr(pos + 3 + sequence.length()) + L"|sequence";
		}
	}
	//if (!ref.empty())
	//{
	//	Context.start_bookmark(ref);
	//}

	if (template_)
	{
		Context.add_new_run();
		Context.output_stream() << L"<w:t>" << *template_ << L"</w:t>";
		Context.finish_run();
	}

	std::wstring num_format = L"ARABIC";
	if (style_num_format_)
	{
		switch(style_num_format_->get_type())
		{
			case odf_types::style_numformat::romanUc:	num_format= L"ROMANUC"; break;
			case odf_types::style_numformat::romanLc:	num_format= L"ROMANLC"; break;
			case odf_types::style_numformat::alphaUc:	num_format= L"ALPHAUC"; break;
			case odf_types::style_numformat::alphaLc:	num_format= L"ALPHALC"; break;
			case odf_types::style_numformat::arabic:
			default:
														num_format= L"ARABIC"; break;
		}
	}
	Context.start_bookmark(*ref_name_);
	Context.output_stream() << L"<w:fldSimple w:instr=\" SEQ " << XmlUtils::EncodeXmlString(sequence) << L" \\* " << num_format << L" \">";
	Context.add_new_run();
		for (size_t i = 0; i < text_.size(); i++)
		{
			text_[i]->docx_convert(Context);
		}
	Context.finish_run();
	
	//if (!ref.empty())
	//{
	//	Context.end_bookmark(ref);
	//}
	Context.output_stream() << L"</w:fldSimple>";

	Context.end_bookmark(*ref_name_);
}
void sequence::pptx_convert(oox::pptx_conversion_context & Context) 
{
	for (size_t i = 0; i < text_.size(); i++)
    {
        text_[i]->pptx_convert(Context);
    }
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * expression::ns = L"text";
const wchar_t * expression::name = L"expression";

void expression::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	office_value_.add_attributes(Attributes);
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:display",			text_display_);
	CP_APPLY_ATTR(L"text:formula",			text_formula_);

}
std::wostream & expression::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	_Wostream << (bXmlEncode ? XmlUtils::EncodeXmlString( text_ ) : text_);
    return _Wostream;
}
void expression::add_text(const std::wstring & Text)
{
    text_ = Text;
}
void expression::docx_convert(oox::docx_conversion_context & Context) 
{//???
    std::wostream & strm = Context.output_stream();
	Context.finish_run();
	
	strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"></w:fldChar></w:r>";
	strm << L"<w:r><w:instrText>FORMTEXT</w:instrText></w:r>";
	strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	strm << L"<w:r><w:t>" << text_ << L"</w:t></w:r>";
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * text_input::ns = L"text";
const wchar_t * text_input::name = L"text-input";

void text_input::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:description", text_description_);

}
std::wostream & text_input::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream << (bXmlEncode ? XmlUtils::EncodeXmlString( text_ ) : text_);
    return _Wostream;
}
void text_input::add_text(const std::wstring & Text)
{
    text_ = Text;
}
void text_input::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & strm = Context.output_stream();
	Context.finish_run();
	
	strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"></w:fldChar></w:r>";
	strm << L"<w:r><w:instrText>FORMTEXT</w:instrText></w:r>";
	strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	strm << L"<w:r><w:t>" << text_ << L"</w:t></w:r>";
    strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
}
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * text_drop_down::ns		= L"text";
const wchar_t * text_drop_down::name	= L"drop-down";

void text_drop_down::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:name", text_name_);
}

void text_drop_down::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"text", L"label")
	{
		CP_CREATE_ELEMENT(content_);
	}
	else
	{
	}
}

void text_drop_down::add_text(const std::wstring & Text) 
{
    text_ = Text;
}
std::wostream & text_drop_down::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream << (bXmlEncode ? XmlUtils::EncodeXmlString( text_ ) : text_);
    return _Wostream;
}

void text_drop_down::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & strm = Context.output_stream();
	Context.finish_run();
	
	//strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << text_name_.get_value_or(L"") << L"\"/><w:enabled/>";

	//strm << L"<w:ddList><w:result w:val=\"0\"/>";
	//for (size_t i = 0; i < content_.size(); i++)
	//{
	//	content_[i]->docx_convert(Context);
	//}
	//strm << L"</w:ddList></w:ffData>";

	//strm << L"</w:fldChar></w:r>";
	//strm << L"<w:r><w:instrText>FORMDROPDOWN</w:instrText></w:r>";
	//strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
	//strm << L"<w:r><w:t>" << text_ << L"</w:t></w:r>";
	//strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";

	_UINT32 id = 0x7fff + Context.nFormFieldId_++;
	strm << L"<w:sdt><w:sdtPr><w:alias w:val=\"\"/><w:id w:val=\"" + std::to_wstring(id) << L"\"/><w:dropDownList>";
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->docx_convert(Context);
	}
	strm << L"</w:dropDownList></w:sdtPr><w:sdtEndPr/><w:sdtContent>";
	strm << L"<w:r><w:t>" << text_ << L"</w:t></w:r>";
	strm << L"</w:sdtContent></w:sdt>";
}
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * text_label::ns		= L"text";
const wchar_t * text_label::name	= L"label";

void text_label::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:value", text_value_);
}
void text_label::docx_convert(oox::docx_conversion_context & Context) 
{
    std::wostream & strm = Context.output_stream();
	
	strm << L"<w:listItem w:value=\"" << text_value_.get_value_or(L"") << L"\" w:displayText=\"" << text_value_.get_value_or(L"") << L"\"/>";
}
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * sheet_name::ns = L"text";
const wchar_t * sheet_name::name = L"sheet-name";

void sheet_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sheet_name::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sheet_name::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"sheet name", text_, Context);
}
void sheet_name::xlsx_serialize(std::wostream & _Wostream, oox::xlsx_conversion_context & Context)
{
	_Wostream << L"&amp;A";
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * author_name::ns = L"text";
const wchar_t * author_name::name = L"author-name";

void author_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void author_name::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void author_name::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"AUTHOR", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * author_initials::ns = L"text";
const wchar_t * author_initials::name = L"author-initials";

void author_initials::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void author_initials::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void author_initials::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_field(L"USERINITIALS", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_city::ns = L"text";
const wchar_t * sender_city::name = L"sender-city";

void sender_city::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void sender_city::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_city::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender city", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_email::ns = L"text";
const wchar_t * sender_email::name = L"sender-email";

void sender_email::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_email::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_email::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender email", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_lastname::ns = L"text";
const wchar_t * sender_lastname::name = L"sender-lastname";

void sender_lastname::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_lastname::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_lastname::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender last name", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_firstname::ns = L"text";
const wchar_t * sender_firstname::name = L"sender-firstname";

void sender_firstname::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_firstname::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_firstname::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender first name", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_company::ns = L"text";
const wchar_t * sender_company::name = L"sender-company";

void sender_company::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_company::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_company::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender company", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_postal_code::ns = L"text";
const wchar_t * sender_postal_code::name = L"sender-postal-code";

std::wostream & sender_postal_code::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}

void sender_postal_code::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:fixed", text_fixed_);
}
void sender_postal_code::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_postal_code::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender postal code", text_, Context);
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_street::ns = L"text";
const wchar_t * sender_street::name = L"sender-street";

void sender_street::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_street::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_street::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender street", text_, Context);
}

//------------------------------------------------------------------------------------------------------------
const wchar_t * sender_state_or_province::ns = L"text";
const wchar_t * sender_state_or_province::name = L"sender-state-or-province";

void sender_state_or_province::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}
void sender_state_or_province::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void sender_state_or_province::docx_convert(oox::docx_conversion_context & Context)
{
	docx_serialize_sdt_placeholder(L"Sender state or province", text_, Context);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * text_user_field_get::ns		= L"text";
const wchar_t * text_user_field_get::name	= L"user-field-get";

void text_user_field_get::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:display",			text_display_);
	CP_APPLY_ATTR(L"text:name",				text_name_);
}
void text_user_field_get::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;
}
void text_user_field_get::docx_convert(oox::docx_conversion_context & Context)
{
	if (!text_name_) return;

	if (!text_)
	{
		std::wstring value = Context.get_user_field(*text_name_);
		
		if (!value.empty())
			text_ = text::create(value) ;
	}
	
	docx_serialize_run(text_, Context);
}
//---------------------------------------------------------------------------------------------------
const wchar_t * text_user_defined::ns	= L"text";
const wchar_t * text_user_defined::name	= L"user-defined";

void text_user_defined::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
	CP_APPLY_ATTR(L"text:name",				text_name_);
    CP_APPLY_ATTR(L"text:fixed",			text_fixed_);
	
	office_value_.add_attributes(Attributes);
}
void text_user_defined::add_text(const std::wstring & Text)
{
    text_ = text::create(Text) ;//cache
}
void text_user_defined::docx_convert(oox::docx_conversion_context & Context)
{
	if (!text_name_) return;

	odf_reader::odf_read_context & odfContext = Context.root()->odf_context();
	
	std::wstring value = odfContext.DocProps().get_user_defined(*text_name_);
	if (!value.empty())
		text_ = text::create(value) ;
	
	docx_serialize_run(text_, Context);
}
//-----------------------------------------------------------------------------------------------
// text:bibliography-mark
//-----------------------------------------------------------------------------------------------
const wchar_t * bibliography_mark::ns = L"text";
const wchar_t * bibliography_mark::name = L"bibliography-mark";

void bibliography_mark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:identifier",		identifier_, std::wstring(L""));
	CP_APPLY_ATTR(L"text:bibliography-type",bibliography_type_, odf_types::bibliography(odf_types::bibliography::book));
    CP_APPLY_ATTR(L"text:author",			author_);
    CP_APPLY_ATTR(L"text:url",				url_);
    CP_APPLY_ATTR(L"text:title",			title_);
    CP_APPLY_ATTR(L"text:year",				year_);
	CP_APPLY_ATTR(L"text:address",			address_);
	CP_APPLY_ATTR(L"text:annote",			annote_);
	CP_APPLY_ATTR(L"text:booktitle",		booktitle_);
	CP_APPLY_ATTR(L"text:edition",			edition_);
	CP_APPLY_ATTR(L"text:editor_",			editor_);
	CP_APPLY_ATTR(L"text:howpublished",		howpublished_);
	CP_APPLY_ATTR(L"text:institution",		institution_);
	CP_APPLY_ATTR(L"text:issn",				issn_);
	CP_APPLY_ATTR(L"text:journal",			journal_);
	CP_APPLY_ATTR(L"text:month",			month_);
	CP_APPLY_ATTR(L"text:note",				note_);
	CP_APPLY_ATTR(L"text:number",			number_);
	CP_APPLY_ATTR(L"text:organizations",	organizations_);
	CP_APPLY_ATTR(L"text:pages",			pages_);
	CP_APPLY_ATTR(L"text:publisher",		publisher_);
	CP_APPLY_ATTR(L"text:report-type",		report_type_);
	CP_APPLY_ATTR(L"text:school",			school_);
	CP_APPLY_ATTR(L"text:series",			series_);
	CP_APPLY_ATTR(L"text:volume",			volume_);

	if (std::wstring::npos != identifier_.find(L"CITATION "))
	{
		XmlUtils::replace_all(identifier_, L"CITATION ", L"");
	}
	XmlUtils::replace_all(identifier_, L" ", L"");
	XmlUtils::replace_all(identifier_, L"\\", L"");
}

void bibliography_mark::add_text(const std::wstring & Text)
{
	text_ = text::create(Text) ;
}

std::wostream & bibliography_mark::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(text_, bXmlEncode);
    return _Wostream;
}
void bibliography_mark::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"b:Tag")
		{
			CP_XML_STREAM() << XmlUtils::EncodeXmlString(identifier_);
		}
		CP_XML_NODE(L"b:SourceType")
		{
			std::wstring type;
			switch(bibliography_type_.get_type())
			{
			case odf_types::bibliography::article:			type = L"ArticleInAPeriodical"; break;
			case odf_types::bibliography::book:				type = L"Book"; break;
			case odf_types::bibliography::booklet:			type = L"BookSection"; break;
			case odf_types::bibliography::conference:		type = L"ConferenceProceedings"; break;
			case odf_types::bibliography::email:			type = L"ElectronicSource"; break;
			case odf_types::bibliography::inbook:			type = L"Book"; break;
			case odf_types::bibliography::incollection:		type = L"Misc"; break;
			case odf_types::bibliography::inproceedings:	type = L"Misc"; break;
			case odf_types::bibliography::journal:			type = L"JournalArticle"; break;
			case odf_types::bibliography::manual:			type = L"ElectronicSource"; break;
			case odf_types::bibliography::mastersthesis:	type = L"Misc"; break;
			case odf_types::bibliography::misc:				type = L"Misc"; break;
			case odf_types::bibliography::phdthesis:		type = L"Misc"; break;
			case odf_types::bibliography::proceedings:		type = L"ConferenceProceedings"; break;
			case odf_types::bibliography::techreport:		type = L"Report"; break;
			case odf_types::bibliography::unpublished:		type = L"Misc"; break;
			case odf_types::bibliography::www:				type = L"InternetSite"; break;
			}
			CP_XML_STREAM() << type;
		}
		//CP_XML_NODE(L"b:Guid")
		//{
		//}
		if (title_)
		{
			CP_XML_NODE(L"b:Title")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*title_);
			}
		}
		if (author_)
		{
			CP_XML_NODE(L"b:Author")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*author_);
			}
		}
		if (year_)
		{
			CP_XML_NODE(L"b:Year")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*year_);
			}
		}
		if (url_)
		{
			CP_XML_NODE(L"b:InternetSiteTitle")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*url_);
			}
		}
		if (note_)
		{
			CP_XML_NODE(L"b:Comments")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*note_);
			}
		}
		if (chapter_)
		{
			CP_XML_NODE(L"b:ChapterNumber")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*chapter_);
			}
		}
		if (journal_)
		{
			CP_XML_NODE(L"b:JournalName")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*journal_);
			}
		}
		if (month_)
		{
			CP_XML_NODE(L"b:Month")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*month_);
			}
		}
		if (pages_)
		{
			CP_XML_NODE(L"b:Pages")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*pages_);
			}
		}
		if (publisher_)
		{
			CP_XML_NODE(L"b:Publisher")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*publisher_);
			}
		}
		if (volume_)
		{
			CP_XML_NODE(L"b:Volume")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*volume_);
			}
		}
		if (address_)
		{
			CP_XML_NODE(L"b:City")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*address_);
			}
		}
		if (booktitle_)
		{
			CP_XML_NODE(L"b:BookTitle")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*booktitle_);
			}
		}
		if (edition_)
		{
			CP_XML_NODE(L"b:Edition")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*edition_);
			}
		}
		if (institution_)
		{
			CP_XML_NODE(L"b:Institution")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*institution_);
			}
		}	
		if (institution_)
		{
			CP_XML_NODE(L"b:Institution")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(*institution_);
			}
		}
//isbn_;
//annote_;
//editor_;
//howpublished_;
//issn_;
//number_;
//organizations_;
//report_type_;
//school_;
//series_;
	}
}
void bibliography_mark::docx_convert(oox::docx_conversion_context & Context)
{
	std::wstringstream strm;
	serialize(strm);

	Context.add_bibliography_item(strm.str());

	if (text_)
	{
		docx_serialize_field(L"CITATION " + XmlUtils::EncodeXmlString(identifier_), text_, Context, false);		
		//Context.finish_run();
		//Context.output_stream() << L"<w:fldSimple w:instr=\" CITATION " << content_ << L" \\h\"/>";
		
		//Context.add_new_run();
		//	content_->docx_convert(Context);
		//Context.finish_run();

		//Context.output_stream() << L"</w:fldSimple>";
	}

}

void bibliography_mark::pptx_convert(oox::pptx_conversion_context & Context)
{
 	std::wstringstream strm;
	serialize(strm);

	//if (content_)
    //    content_->pptx_convert(Context);
}
//-----------------------------------------------------------------------------------------------
// text:alphabetical-index-auto-mark-file
//-----------------------------------------------------------------------------------------------
const wchar_t * alphabetical_index_auto_mark_file::ns = L"text";
const wchar_t * alphabetical_index_auto_mark_file::name = L"alphabetical-index-auto-mark-file";

void alphabetical_index_auto_mark_file::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	xlink_attlist_.add_attributes(Attributes);
}
void alphabetical_index_auto_mark_file::docx_convert(oox::docx_conversion_context & Context)
{
}
//-----------------------------------------------------------------------------------------------
// text:alphabetical-index-mark
//-----------------------------------------------------------------------------------------------
const wchar_t * alphabetical_index_mark::ns = L"text";
const wchar_t * alphabetical_index_mark::name = L"alphabetical-index-mark";

void alphabetical_index_mark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:key1", key1_);
	CP_APPLY_ATTR(L"text:key1-phonetic", key1_phonetic_);
	CP_APPLY_ATTR(L"text:key2", key2_);
	CP_APPLY_ATTR(L"text:key2-phonetic", key2_phonetic_);
	CP_APPLY_ATTR(L"text:main-entry", main_entry_);
	CP_APPLY_ATTR(L"text:string-value", string_value_);
	CP_APPLY_ATTR(L"text:string-value-phonetic", string_value_phonetic_);
}
void alphabetical_index_mark::docx_convert(oox::docx_conversion_context & Context)
{
	std::wstring value;
	if (string_value_ && false == string_value_->empty())
	{
		if (*string_value_ != L" ")
			value = *string_value_;
	}
	if (value.empty() && key1_)
	{
		value = *key1_;
	}
	if (value.empty()) return;

	Context.finish_run();	
	
	Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>";
	Context.output_stream() << L"<w:r><w:instrText> XE \"" <<  XmlUtils::EncodeXmlString(value) << L"\"</w:instrText></w:r><w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
}
//-----------------------------------------------------------------------------------------------
// text:alphabetical-index-mark-start
//-----------------------------------------------------------------------------------------------
const wchar_t * alphabetical_index_mark_start::ns = L"text";
const wchar_t * alphabetical_index_mark_start::name = L"alphabetical-index-mark-start";

void alphabetical_index_mark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", id_, std::wstring(L""));
	CP_APPLY_ATTR(L"text:key1", key1_);
	CP_APPLY_ATTR(L"text:key1-phonetic", key1_phonetic_);
	CP_APPLY_ATTR(L"text:key2", key2_);
	CP_APPLY_ATTR(L"text:key2-phonetic", key2_phonetic_);
	CP_APPLY_ATTR(L"text:main-entry", main_entry_);
	CP_APPLY_ATTR(L"text:string-value-phonetic", string_value_phonetic_);
}
void alphabetical_index_mark_start::docx_convert(oox::docx_conversion_context & Context)
{
	Context.start_alphabetical_index(id_);
}
//-----------------------------------------------------------------------------------------------
// text:alphabetical-index-mark-end
//-----------------------------------------------------------------------------------------------
const wchar_t * alphabetical_index_mark_end::ns = L"text";
const wchar_t * alphabetical_index_mark_end::name = L"alphabetical-index-mark-end";

void alphabetical_index_mark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", id_, std::wstring(L""));
}
void alphabetical_index_mark_end::docx_convert(oox::docx_conversion_context & Context)
{
	Context.end_alphabetical_index(id_);
}
//-----------------------------------------------------------------------------------------------
// text:user-index-mark
//-----------------------------------------------------------------------------------------------
const wchar_t * user_index_mark::ns = L"text";
const wchar_t * user_index_mark::name = L"user-index-mark";

void user_index_mark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:index-name", index_name_);
	CP_APPLY_ATTR(L"text:outline-level", outline_level_);
	CP_APPLY_ATTR(L"text:string-value", string_value_);
}
void user_index_mark::docx_convert(oox::docx_conversion_context & Context)
{
}
//-----------------------------------------------------------------------------------------------
// text:user-index-mark-start
//-----------------------------------------------------------------------------------------------
const wchar_t * user_index_mark_start::ns = L"text";
const wchar_t * user_index_mark_start::name = L"user-index-mark-start";

void user_index_mark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:id", id_, std::wstring(L""));
	CP_APPLY_ATTR(L"text:index-name", index_name_);
	CP_APPLY_ATTR(L"text:outline-level", outline_level_);
}
void user_index_mark_start::docx_convert(oox::docx_conversion_context & Context)
{
}
//-----------------------------------------------------------------------------------------------
// text:user-index-mark-end
//-----------------------------------------------------------------------------------------------
const wchar_t * user_index_mark_end::ns = L"text";
const wchar_t * user_index_mark_end::name = L"user-index-mark-end";

void user_index_mark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", id_, std::wstring(L""));
}
void user_index_mark_end::docx_convert(oox::docx_conversion_context & Context)
{
}
//-----------------------------------------------------------------------------------------------
// text:toc-mark
//-----------------------------------------------------------------------------------------------
const wchar_t * toc_mark::ns = L"text";
const wchar_t * toc_mark::name = L"toc-mark";

void toc_mark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"text:outline-level", outline_level_);
	CP_APPLY_ATTR(L"text:string-value", string_value_);
}
void toc_mark::docx_convert(oox::docx_conversion_context & Context)
{
}
//-----------------------------------------------------------------------------------------------
// text:toc-mark-start
//-----------------------------------------------------------------------------------------------
const wchar_t * toc_mark_start::ns = L"text";
const wchar_t * toc_mark_start::name = L"toc-mark-start";

void toc_mark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", id_, std::wstring(L""));
	CP_APPLY_ATTR(L"text:outline-level", outline_level_);
}
void toc_mark_start::docx_convert(oox::docx_conversion_context & Context)
{
}
//-----------------------------------------------------------------------------------------------
// text:toc-mark-end
//-----------------------------------------------------------------------------------------------
const wchar_t * toc_mark_end::ns = L"text";
const wchar_t * toc_mark_end::name = L"toc-mark-end";

void toc_mark_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:id", id_, std::wstring(L""));
}
void toc_mark_end::docx_convert(oox::docx_conversion_context & Context)
{
}
//------------------------------------------------------------------------------------------------------------
}//namespace text

//------------------------------------------------------------------------------------------------------------
const wchar_t * presentation_footer::ns = L"presentation";
const wchar_t * presentation_footer::name = L"footer";

void presentation_footer::pptx_convert(oox::pptx_conversion_context & Context)
{
	//Context.get_slide_context.set_footer_enabled();
}

const wchar_t * presentation_date_time::ns = L"presentation";
const wchar_t * presentation_date_time::name = L"date-time";

void presentation_date_time::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.get_text_context().start_field(oox::date, L"");
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * field_fieldmark_start::ns = L"field";
const wchar_t * field_fieldmark_start::name = L"fieldmark-start";

void field_fieldmark_start::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_);
    CP_APPLY_ATTR(L"field:type", field_type_);
}
void field_fieldmark_start::docx_convert(oox::docx_conversion_context & Context)
{
	if (!field_type_) return;
	if (!text_name_) return;

	if (std::wstring::npos != field_type_->find(L"vnd.oasis.opendocument.field."))
	{
		Context.start_field(field_type_->substr(29), *text_name_);
	}
	else
	{
		Context.start_field(*field_type_, *text_name_);
	}

}
//------------------------------------------------------------------------------------------------------------
const wchar_t * field_fieldmark_end::ns = L"field";
const wchar_t * field_fieldmark_end::name = L"fieldmark-end";

void field_fieldmark_end::docx_convert(oox::docx_conversion_context & Context)
{
	Context.end_field();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * field_fieldmark::ns = L"field";
const wchar_t * field_fieldmark::name = L"fieldmark";

void field_fieldmark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:name", text_name_);
    CP_APPLY_ATTR(L"field:type", field_type_);
}
void field_fieldmark::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(field_params_);
}
void field_fieldmark::docx_convert(oox::docx_conversion_context & Context)
{
	if (!field_type_) return;
	if (!text_name_) return;

	if (std::wstring::npos != field_type_->find(L"FORMCHECKBOX"))
	{
		XmlUtils::replace_all( *text_name_, L" ", L"_");

		Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << *text_name_ << L"\"/><w:enabled/>";
		Context.output_stream() << L"<w:checkBox>";
		//Context.output_stream() << L"<w:default w:val=\"" << std::to_wstring(current_state_) << L"\"/>
		Context.output_stream() << L"<w:sizeAuto/>";
		for (size_t i = 0; i < field_params_.size(); i++)
		{
			field_param *param = dynamic_cast<field_param*>(field_params_[i].get());
			if ((param) && (param->field_name_) && (std::wstring::npos != param->field_name_->find(L"Checkbox_Checked")))
			{
				odf_types::Bool value = Bool::parse(*param->field_value_);
				if (value.get())
 					Context.output_stream() << L"<w:checked/>";
				break;
			}
		}
		Context.output_stream() << L"</w:checkBox></w:ffData>";
		Context.output_stream() << L"</w:fldChar></w:r>";
		Context.output_stream() << L"<w:r><w:instrText>FORMCHECKBOX</w:instrText></w:r>";
		Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
		Context.output_stream() << L"<w:r><w:t>" << L"" << L"</w:t></w:r>";
		Context.output_stream() << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
	}
	else if (std::wstring::npos != field_type_->find(L"FORMDROPDOWN"))
	{
		std::wostream & strm = Context.output_stream();
		Context.finish_run();
		
		//strm << L"<w:r><w:fldChar w:fldCharType=\"begin\"><w:ffData><w:name w:val=\"" << text_name_.get_value_or(L"") << L"\"/><w:enabled/>";

		//strm << L"<w:ddList><w:result w:val=\"0\"/>";
		//for (size_t i = 0; i < field_params_.size(); i++)
		//{
		//	field_params_[i]->docx_convert(Context);
		//}
		//strm << L"</w:ddList></w:ffData>";

		//strm << L"</w:fldChar></w:r>";
		//strm << L"<w:r><w:instrText>FORMDROPDOWN</w:instrText></w:r>";
		//strm << L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>";
		//strm << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";

		_UINT32 id = 0x7fff + Context.nFormFieldId_++;
		std::wstring text_;
		strm << L"<w:sdt><w:sdtPr><w:alias w:val=\"\"/><w:id w:val=\"" + std::to_wstring(id) << L"\"/><w:dropDownList>";
		for (size_t i = 0; i < field_params_.size(); i++)
		{
			field_param* param = dynamic_cast<field_param*>(field_params_[i].get());
			if (!param) continue;
			
			strm << L"<w:listItem w:value=\"" << XmlUtils::EncodeXmlString(param->field_value_.get_value_or(L""))
				<< L"\" w:displayText=\"" << XmlUtils::EncodeXmlString(param->field_value_.get_value_or(L"")) << L"\"/>";

			if (text_.empty())
				text_ = param->field_value_.get_value_or(L"");
		}
		strm << L"</w:dropDownList></w:sdtPr><w:sdtEndPr/><w:sdtContent>";
		strm << L"<w:r><w:t>" << XmlUtils::EncodeXmlString(text_) << L"</w:t></w:r>";
		strm << L"</w:sdtContent></w:sdt>";
	}
	else if (std::wstring::npos != field_type_->find(L"FORMTEXT"))
	{
	}
}
//------------------------------------------------------------------------------------------------------------
const wchar_t * field_param::ns = L"field";
const wchar_t * field_param::name = L"param";

void field_param::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"field:name", field_name_);
    CP_APPLY_ATTR(L"field:value", field_value_);
}
void field_param::docx_convert(oox::docx_conversion_context & Context)
{
}
}
}

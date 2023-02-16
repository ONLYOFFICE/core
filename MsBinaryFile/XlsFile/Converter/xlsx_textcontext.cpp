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

#include "xlsx_textcontext.h"
#include "xlsx_conversion_context.h"

#include <iostream>
#include <list>

#include "../../Common/Utils/simple_xml_writer.h"
#include "../../../OOXML/Base/Unit.h"

namespace oox {

void removeCharsFromString( std::wstring &str, std::wstring charsToRemove ) 
{
   for ( unsigned int i = 0; i < charsToRemove.length(); ++i ) 
   {
	   str.erase( std::remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
   }
}

class xlsx_text_context::Impl: boost::noncopyable
{
public:
	Impl();
public:
	void add_text(const std::wstring & text);
    
    void			start_paragraph(const std::wstring & styleName);
    void			end_paragraph();

    void			start_span(const std::wstring & styleName);
    void			end_span();
    std::wstring	end_span2();

    void			start_cell_content();
	void			set_cell_text_properties( );
    int				end_cell_content(bool need_cache);

	void			start_comment_content();
	std::wstring	end_comment_content();

	void			start_drawing_content();
	std::wstring	end_drawing_content(); 


	void ApplyTextProperties();

	void set_local_styles_container();//это если стили объектов содержатся в другом документе

	bool is_drawing_context(){return in_draw;}

	void end_hyperlink(std::wstring hId);
	void start_hyperlink();

private:
	std::wstring hyperlink_hId;
	
	bool in_draw;
	bool in_comment;
	bool in_span;
	bool in_paragraph;
	bool in_cell_content;

	//odf_reader::styles_container				& styles_;
	//odf_reader::text_format_properties_content	* text_properties_cell_;
	//odf_reader::styles_container				* local_styles_ptr_;

    std::wstring dump_text();
    void write_rPr(std::wostream & strm);
   
	size_t paragraphs_cout_; //???? тока из за начала отсчета?
   
	std::wstringstream	text_;
    std::wstringstream	output_;
   
	std::wstring paragraph_style_name_;//был вектор ... не нужен, так как в один момент времени может быть тока один стиль параграфа,текста,объекта при приходе нового - дампится
    std::wstring span_style_name_;

};



xlsx_text_context::Impl::Impl(): paragraphs_cout_(0),
				in_comment(false),in_draw(false),in_paragraph(false),in_span(false),in_cell_content(false)
{
	//text_properties_cell_ = NULL;
}

void xlsx_text_context::Impl::add_text(const std::wstring & text)
{
    text_ << text;
	
	if (!in_comment && !in_draw)
		dump_text();
}

void xlsx_text_context::Impl::set_local_styles_container(/*odf_reader::styles_container * local_styles_*/)
{
	//local_styles_ptr_= local_styles_;
}

void xlsx_text_context::Impl::start_paragraph(const std::wstring & styleName)
{
    if (paragraphs_cout_++ > 0)
    {	
		if (paragraph_style_name_ != styleName)
		{
			dump_text();
		}
	}else
	{
		text_.str(std::wstring());
	}
	paragraph_style_name_ = styleName;
	in_paragraph = true;
}

void xlsx_text_context::Impl::end_paragraph()
{
    if (!in_comment && !in_draw)
	{
		dump_text();
		paragraph_style_name_ = L"";
	}
	in_paragraph = false;
}

void xlsx_text_context::Impl::start_span(const std::wstring & styleName)//кусок текста в абзаце(параграфе) со своими свойствами - этто может быть и 1 буква
{
     if (!in_comment && !in_draw)
	 {
		 text_.str(std::wstring());
	 }
	 else
	 {
		if (span_style_name_ !=styleName || in_span)
		{
			dump_text();
		}
	 }
	 span_style_name_ = styleName;
	 in_span=true;
}

void xlsx_text_context::Impl::end_span() //odf корявенько написан - возможны повторы стилей в последовательных кусках текста
//пока с анализом стилей тока комменты - остальные текстовые куски как есть.. с охрененным возможно дубляжом
{
     if (!in_comment && !in_draw)
	 {
		dump_text();
		span_style_name_=L"";
	 }
	 in_span=false;
}

std::wstring xlsx_text_context::Impl::end_span2()
{
    const std::wstring content = dump_text();
    span_style_name_ = L"";
	
	in_span = false;
    return content;
}
void xlsx_text_context::Impl::start_hyperlink()
{
	dump_text();
}

void xlsx_text_context::Impl::end_hyperlink(std::wstring hId)
{
	hyperlink_hId = hId;
}

void xlsx_text_context::Impl::ApplyTextProperties(/*std::wstring style,odf_reader::text_format_properties_content & propertiesOut, odf_types::style_family::type Type*/)
{

}

void xlsx_text_context::Impl::set_cell_text_properties()
{
}

void xlsx_text_context::Impl::write_rPr(std::wostream & strm)
{

	CP_XML_WRITER(strm)
    {
		if (in_draw)
		{
			//oox_serialize_style_text(strm,text_properties_);
			//oox_serialize_style_text(strm,odf_reader::text_format_properties_content & properties);
			CP_XML_NODE(L"a:rPr")
			{
				//стр 3197
				//if (dValFontSize)									{CP_XML_ATTR(L"sz", (int)(dValFontSize.get()*100));}
				//if ((iValFontStyle) && (iValFontStyle.get() >0))	{CP_XML_ATTR(L"i", "1");} //"true");} Exercícios de Aprendizagem.ods
				//if ((iValFontWeight) && (iValFontWeight.get() >0))	{CP_XML_ATTR(L"b", "1");} //"true");} Exercícios de Aprendizagem.ods				
				//if (sValFontFamily)									{CP_XML_ATTR(L"typeface", sValFontFamily.get());} 
				//
				//if (sValFontColor){CP_XML_NODE(L"a:solidFill")	{CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val", sValFontColor.get());}}}

				if (hyperlink_hId.length()>0)
				{
					CP_XML_NODE(L"a:hlinkClick ")
					{
						CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
						CP_XML_ATTR(L"r:id", hyperlink_hId);
					}
				}
			}
		}
		else
		{
			CP_XML_NODE(L"rPr")
			{
				//if (sValFontFamily)							{CP_XML_NODE(L"rFont")	{CP_XML_ATTR(L"val", sValFontFamily.get());}}
				//if (dValFontSize)							{CP_XML_NODE(L"sz")		{CP_XML_ATTR(L"val", (int)(dValFontSize.get()));}}
				//if (sValFontColor)							{CP_XML_NODE(L"color")	{CP_XML_ATTR(L"rgb", sValFontColor.get());}}
				//if ((iValFontStyle) && (iValFontStyle.get() >0))  {CP_XML_NODE(L"i")		{CP_XML_ATTR(L"val", "true");}}
				//if ((iValFontWeight) && (iValFontWeight.get() >0)){CP_XML_NODE(L"b")		{CP_XML_ATTR(L"val", "true");}}
			}
		}
    }
}

std::wstring xlsx_text_context::Impl::dump_text()
{
    const std::wstring content = XmlUtils::EncodeXmlString(text_.str());

	if (content.empty()) 
		return content;

	std::wstring	prefix_draw;
	if (in_draw)	prefix_draw=L"a:";
   
	CP_XML_WRITER(output_)
    {
        if (!content.empty())
        {
			CP_XML_NODE(prefix_draw + L"r")
			{
				write_rPr(CP_XML_STREAM());                
				
				CP_XML_NODE(prefix_draw + L"t")
				{
					if (!in_draw)CP_XML_ATTR(L"xml:space", L"preserve");
					CP_XML_STREAM() << content;
                }
	         }
            text_.str(std::wstring());
        }
    }

	hyperlink_hId =L"";
    return content;
}

void xlsx_text_context::Impl::start_cell_content()
{
    paragraphs_cout_ = 0;
   
	output_.str(std::wstring());//строка дампа
    
	text_.str(std::wstring()); //приходящие куски текста
    
	paragraph_style_name_ = L"";
    span_style_name_ = L"";

	in_cell_content = true;
	
}

void xlsx_text_context::Impl::start_comment_content()
{
    paragraphs_cout_ = 0;
   
	output_.str(std::wstring());
    text_.str(std::wstring());
    
	paragraph_style_name_ = L"";
    span_style_name_ = L"";

	in_comment = true;
}
void xlsx_text_context::Impl::start_drawing_content()
{
    paragraphs_cout_ = 0;
   
	output_.str(std::wstring());
    text_.str(std::wstring());
    
	paragraph_style_name_ = L"";
    span_style_name_ = L"";

	in_draw = true;
}
std::wstring xlsx_text_context::Impl::end_comment_content()
{
	dump_text();//если в комменте куча абзацев со одним стилем - сдампится здесь - иначе дампится по мере прихода каждого нового стиля

	std::wstring comment= output_.str();
  
	paragraphs_cout_ = 0;
    
	output_.str(std::wstring());
    text_.str(std::wstring());
	
	paragraph_style_name_ = L"";
    span_style_name_=L"";

	in_comment = false;
	return comment;
}
std::wstring xlsx_text_context::Impl::end_drawing_content()
{
	dump_text();//если в draw куча абзацев со одним стилем - сдампится здесь - иначе дампится по мере прихода каждого нового стиля

	std::wstring draw= output_.str();
  
	paragraphs_cout_ = 0;
    
	output_.str(std::wstring());
    text_.str(std::wstring());
	
	paragraph_style_name_ = L"";
    span_style_name_=L"";

	in_draw = false;
	return draw;
}
int xlsx_text_context::Impl::end_cell_content(bool need_cache)
{
	dump_text();

	const int sharedStrId = 0;//output_.str().empty() ? (-1) :  static_cast<int>(xlsx_shared_strings_.add(output_.str()));
	//???? нужно ли здесь очищать все ????? - проверить стили на кучках - и проверить как меняются стили внутри одной ячейки - то есть здешнее переопределение внешнего стиля
	in_cell_content = false;   
	return sharedStrId;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

xlsx_text_context::xlsx_text_context(): impl_(new xlsx_text_context::Impl())
{}


xlsx_text_context::~xlsx_text_context()
{
}
void xlsx_text_context::set_local_styles_container()
{
	return impl_->set_local_styles_container();
}

void xlsx_text_context::set_cell_text_properties()
{
	return impl_->set_cell_text_properties();
}


void xlsx_text_context::add_text(const std::wstring & text)
{
    return impl_->add_text(text);
}

void xlsx_text_context::start_paragraph(const std::wstring & styleName)
{
    return impl_->start_paragraph(styleName);
}

void xlsx_text_context::end_paragraph()
{
    return impl_->end_paragraph();
}

void xlsx_text_context::start_span(const std::wstring & styleName)
{
    return impl_->start_span(styleName);
}

void xlsx_text_context::end_span()
{
    return impl_->end_span();
}

std::wstring xlsx_text_context::end_span2()
{
    return impl_->end_span2();
}
bool xlsx_text_context::is_drawing_context()
{
	return impl_->is_drawing_context();
}

void xlsx_text_context::start_cell_content()
{
    return impl_->start_cell_content();
}

int xlsx_text_context::end_cell_content(bool need_cache)
{
    return impl_->end_cell_content(need_cache);
}

void xlsx_text_context::start_comment_content()
{
	return impl_->start_comment_content();
}

std::wstring xlsx_text_context::end_comment_content()
{
	return impl_->end_comment_content();
}
void xlsx_text_context::start_drawing_content()
{
	return impl_->start_drawing_content();
}
void xlsx_text_context::start_hyperlink()
{
	return impl_->start_hyperlink();
}
void xlsx_text_context::end_hyperlink(std::wstring hId)
{
	return impl_->end_hyperlink(hId);
}
std::wstring xlsx_text_context::end_drawing_content()
{
	return impl_->end_drawing_content();
}

    
}

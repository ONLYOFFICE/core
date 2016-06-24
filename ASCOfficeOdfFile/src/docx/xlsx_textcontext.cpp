/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include "xlsx_textcontext.h"
#include "xlsxconversioncontext.h"
#include <iostream>
#include "xlsx_sharedstrings.h"
#include <list>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <boost/foreach.hpp>

#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/utils.h>

#include "../odf/odfcontext.h"
#include "../odf/style_text_properties.h"
#include "../odf/style_paragraph_properties.h"
#include "../odf/calcs_styles.h"


namespace cpdoccore {
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
	Impl(odf_reader::styles_container & styles_);
public:
	void add_text(const std::wstring & text);
    
    void			start_paragraph(const std::wstring & styleName);
    void			end_paragraph();

    void			start_span(const std::wstring & styleName);
    void			end_span();
    std::wstring	end_span2();

    void			start_cell_content();
	void			set_cell_text_properties( odf_reader::text_format_properties_content * text_properties);
    int				end_cell_content();

	void			start_comment_content();
	std::wstring	end_comment_content();

	void			start_drawing_content();
	std::wstring	end_drawing_content(); 

    void serialize_shared_strings(std::wostream & strm);
	
	void ApplyTextProperties		(std::wstring style, odf_reader::text_format_properties_content & propertiesOut, odf_types::style_family::type Type);
	void ApplyParagraphProperties	(std::wstring style, odf_reader::paragraph_format_properties & propertiesOut, odf_types::style_family::type Type);

	void set_local_styles_container(odf_reader::styles_container*  local_styles_);//это если стили объектов содержатся в другом документе

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

	odf_reader::styles_container				& styles_;
	odf_reader::text_format_properties_content	* text_properties_cell_;
	odf_reader::styles_container				* local_styles_ptr_;

    std::wstring	dump_paragraph();
	std::wstring	dump_run();

    void write_rPr(std::wostream & strm);
	void write_pPr(std::wostream & strm);
  
	size_t paragraphs_cout_; //???? тока из за начала отсчета?
   
 	std::wstringstream	text_;		//приходящий текст
    std::wstringstream	paragraph_;	//перманенто скидываемые параграфы
    std::wstringstream	run_;		//перманенто скидываемые куски с быть может разными свойствами
  
    xlsx_shared_strings xlsx_shared_strings_;
	
	std::wstring		paragraph_style_name_;//был вектор ... не нужен, так как в один момент времени может быть тока один стиль параграфа,текста,объекта при приходе нового - дампится
    std::wstring		span_style_name_;

};

void xlsx_text_context::Impl::serialize_shared_strings(std::wostream & strm)
{
    xlsx_shared_strings_.serialize(strm);    
}


xlsx_text_context::Impl::Impl(odf_reader::styles_container & styles): paragraphs_cout_(0),styles_(styles),
				in_comment(false),in_draw(false),in_paragraph(false),in_span(false),in_cell_content(false)
{
	text_properties_cell_ = NULL;
}

void xlsx_text_context::Impl::add_text(const std::wstring & text)
{
    text_ << text;
	
	if (!in_comment && !in_draw)
		dump_run();
}

void xlsx_text_context::Impl::set_local_styles_container(odf_reader::styles_container * local_styles_)
{
	local_styles_ptr_= local_styles_;
}

void xlsx_text_context::Impl::start_paragraph(const std::wstring & styleName)
{
    if (paragraphs_cout_++ > 0)
    {	
		if ( in_comment == true )
		{
		// конец предыдущего абзаца и начало следующего
		//text_ << L"&#10;";
			text_ << L"\n"; 
		}
		else/* (paragraph_style_name_ != styleName)*/
		{
			dump_paragraph();
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
		dump_run();
		paragraph_style_name_ = L"";
	}
	in_paragraph = false;
}

void xlsx_text_context::Impl::start_span(const std::wstring & styleName)//кусок текста в абзаце(параграфе) со своими свойствами - этто может быть и 1 буква
{
 	int text_size = text_.str().length();

	if (in_comment || in_draw)
	 {
		if (( span_style_name_ != styleName && text_size > 0 ) || in_span)
		{
			dump_run();
		}

		if (in_draw && !in_paragraph)
			start_paragraph(L"");
	 }
	 else
	 {
		text_.str(std::wstring());
	 }
	 span_style_name_	= styleName;
	 in_span			= true;
}

void xlsx_text_context::Impl::end_span() //odf корявенько написан - возможны повторы стилей в последовательных кусках текста
//пока с анализом стилей тока комменты - остальные текстовые куски как есть.. с охрененным возможно дубляжом
{
     if (!in_comment)
	 {
		dump_run();
		span_style_name_=L"";
	 }
	 in_span=false;
}

std::wstring xlsx_text_context::Impl::end_span2()
{
    const std::wstring content = dump_run();
    span_style_name_ = L"";
	
	in_span = false;
    return content;
}

void xlsx_text_context::Impl::start_hyperlink()
{
	dump_run();
}

void xlsx_text_context::Impl::end_hyperlink(std::wstring hId)
{
	hyperlink_hId = hId;
}

void xlsx_text_context::Impl::ApplyParagraphProperties	(std::wstring style, odf_reader::paragraph_format_properties & propertiesOut, odf_types::style_family::type Type)
{
	std::vector<const odf_reader::style_instance *> instances;

	if (local_styles_ptr_)
	{
		odf_reader::style_instance * defaultStyle = local_styles_ptr_->style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf_reader::style_instance* styleInst = local_styles_ptr_->style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	else
	{
		odf_reader::style_instance * defaultStyle = styles_.style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf_reader::style_instance* styleInst = styles_.style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	propertiesOut.apply_from(calc_paragraph_properties_content(instances));
}
void xlsx_text_context::Impl::ApplyTextProperties(std::wstring style, odf_reader::text_format_properties_content & propertiesOut, odf_types::style_family::type Type)
{
	std::vector<const odf_reader::style_instance *> instances;

	if (local_styles_ptr_)
	{
		odf_reader::style_instance * defaultStyle = local_styles_ptr_->style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf_reader::style_instance* styleInst = local_styles_ptr_->style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	else
	{
		odf_reader::style_instance * defaultStyle = styles_.style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf_reader::style_instance* styleInst = styles_.style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	propertiesOut.apply_from(calc_text_properties_content(instances));
}

void xlsx_text_context::Impl::set_cell_text_properties(odf_reader::text_format_properties_content * text_properties)
{
	text_properties_cell_ = text_properties;
}

void xlsx_text_context::Impl::write_pPr	(std::wostream & strm)
{
	if (paragraph_style_name_.empty())return;

	odf_reader::paragraph_format_properties		paragraph_format_properties_;	
	
	ApplyParagraphProperties	(paragraph_style_name_,	paragraph_format_properties_	, odf_types::style_family::Paragraph);
	paragraph_format_properties_.xlsx_convert(strm, in_draw);
}

void xlsx_text_context::Impl::write_rPr(std::wostream & strm)
{
	if (paragraph_style_name_.empty() && span_style_name_.empty() 
			&& !(!hyperlink_hId.empty()	&& in_draw) 
			&& !(text_properties_cell_	&& in_cell_content))return;

	odf_reader::text_format_properties_content		text_properties_paragraph_;	
	odf_reader::text_format_properties_content		text_properties_span_;
	
	ApplyTextProperties	(paragraph_style_name_,	text_properties_paragraph_	, odf_types::style_family::Paragraph);
	ApplyTextProperties (span_style_name_,		text_properties_span_		, odf_types::style_family::Text);

	odf_reader::text_format_properties_content text_properties_;

	if (in_cell_content && text_properties_cell_)
	{
		text_properties_.apply_from(*text_properties_cell_);
	}
	text_properties_.apply_from(text_properties_paragraph_);
	text_properties_.apply_from(text_properties_span_);

	_CP_OPT(double)	dValFontSize;
	if (text_properties_.fo_font_size_)
		dValFontSize=text_properties_.fo_font_size_->get_length().get_value();
	
	_CP_OPT(std::wstring) sValFontFamily;	
	if (text_properties_.fo_font_family_)	
	{
		std::wstring val =text_properties_.fo_font_family_.get();
		//'Arial' глючит
		removeCharsFromString(val, _T("'"));

		sValFontFamily=text_properties_.fo_font_family_.get();
	}
	//else if (text_properties_.style_font_name_) - тут может быть отсылка к font_face)decl !!!!
	//	sValFontFamily=text_properties_.style_font_name_.get();

	_CP_OPT(std::wstring) sValFontColor;
	if (text_properties_.fo_color_)		
		sValFontColor=text_properties_.fo_color_->get_hex_value();

	_CP_OPT(int)	iValFontWeight;
	if (text_properties_.fo_font_weight_)
		iValFontWeight=text_properties_.fo_font_weight_->get_type();

	_CP_OPT(int)	iValFontStyle;
	if(text_properties_.fo_font_style_)
		iValFontStyle=text_properties_.fo_font_style_->get_type();

	CP_XML_WRITER(strm)
    {
		if (in_draw)
		{
			//oox_serialize_style_text(strm,text_properties_);
			//oox_serialize_style_text(strm,odf_reader::text_format_properties_content & properties);
			CP_XML_NODE(L"a:rPr")
			{
				//стр 3197
				if (dValFontSize)									{CP_XML_ATTR(L"sz", (int)(dValFontSize.get()*100));}
				if ((iValFontStyle) && (iValFontStyle.get() >0))	{CP_XML_ATTR(L"i", "1");} //"true");} Exercícios de Aprendizagem.ods
				if ((iValFontWeight) && (iValFontWeight.get() >0))	{CP_XML_ATTR(L"b", "1");} //"true");} Exercícios de Aprendizagem.ods				
				if (sValFontColor)
				{
					CP_XML_NODE(L"a:solidFill")	{CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val", sValFontColor.get());}}
				}
				if (sValFontFamily)									
				{
					CP_XML_NODE(L"a:latin"){CP_XML_ATTR(L"typeface", sValFontFamily.get());}
				} 
				

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
				if (sValFontFamily)				{CP_XML_NODE(L"rFont")	{CP_XML_ATTR(L"val", sValFontFamily.get());}}
				if (dValFontSize)				{CP_XML_NODE(L"sz")		{CP_XML_ATTR(L"val", (int)(dValFontSize.get()));}}
				if (sValFontColor)				{CP_XML_NODE(L"color")	{CP_XML_ATTR(L"rgb", sValFontColor.get());}}
				if ((iValFontStyle) &&
					(iValFontStyle.get() >0))	{CP_XML_NODE(L"i")		{CP_XML_ATTR(L"val", "true");}}
				if ((iValFontWeight) && 
					(iValFontWeight.get() >0))	{CP_XML_NODE(L"b")		{CP_XML_ATTR(L"val", "true");}}
			}
		}
    }
}


std::wstring xlsx_text_context::Impl::dump_paragraph(/*bool last*/)
{				
	if (!in_draw) return L""; 

	end_span();

    std::wstring str_run = run_.str();

	if (str_run.length() > 0 || paragraph_style_name_.length() > 0)
	{
		CP_XML_WRITER(paragraph_)
		{
			CP_XML_NODE(L"a:p")
			{
				write_pPr(CP_XML_STREAM());

				if (str_run.length() > 0)
				{
					CP_XML_STREAM() << run_.str();
				}
				else
				{
					CP_XML_NODE(L"a:endParaRPr");
				}
			}
		}
		run_.str(std::wstring());
	}
	return paragraph_.str();
}

std::wstring xlsx_text_context::Impl::dump_run()
{
    const std::wstring content = xml::utils::replace_text_to_xml(text_.str());

	if (content.length()<1) 
		return L"";

	std::wstring	prefix_draw;
	if (in_draw)	prefix_draw=L"a:";
   
	CP_XML_WRITER(run_)
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
 	local_styles_ptr_ =NULL;
   
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
    
	paragraph_style_name_	= L"";
    span_style_name_		= L"";

	in_cell_content			= true;
	
	text_properties_cell_	= NULL;
}

void xlsx_text_context::Impl::start_comment_content()
{
    paragraphs_cout_ = 0;
   
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
    
	paragraph_style_name_	= L"";
    span_style_name_		= L"";

	in_comment				= true;
}
void xlsx_text_context::Impl::start_drawing_content()
{
    paragraphs_cout_ = 0;
   
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
    
	paragraph_style_name_	= L"";
    span_style_name_		= L"";

	in_draw					= true;
}
std::wstring xlsx_text_context::Impl::end_comment_content()
{
	dump_run();//если в комменте куча абзацев со одним стилем - сдампится здесь - иначе дампится по мере прихода каждого нового стиля

	std::wstring comment= run_.str();
  
	paragraphs_cout_ = 0;
    
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());

	paragraph_style_name_	= L"";
    span_style_name_		= L"";

	in_comment				= false;
	return comment;
}
std::wstring xlsx_text_context::Impl::end_drawing_content()
{
	dump_paragraph();//если в draw куча абзацев со одним стилем - сдампится здесь - иначе дампится по мере прихода каждого нового стиля

	std::wstring draw = paragraph_.str();
  
	paragraphs_cout_ = 0;    
	
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());

	paragraph_style_name_	= L"";
    span_style_name_		= L"";

	in_draw = false;
	return draw;
}
int xlsx_text_context::Impl::end_cell_content()
{
	dump_run();

	const int sharedStrId = run_.str().empty() ? (-1) :  static_cast<int>(xlsx_shared_strings_.add(run_.str()));
	//???? нужно ли здесь очищать все ????? - проверить стили на кучках - и проверить как меняются стили внутри одной ячейки - то есть здешнее переопределение внешнего стиля
	in_cell_content = false;   
	return sharedStrId;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

xlsx_text_context::xlsx_text_context(odf_reader::styles_container & _styles): impl_(new xlsx_text_context::Impl(_styles))
{}


xlsx_text_context::~xlsx_text_context()
{
}
void xlsx_text_context::set_local_styles_container(odf_reader::styles_container*  local_styles_)
{
	return impl_->set_local_styles_container(local_styles_);
}

void xlsx_text_context::set_cell_text_properties(odf_reader::text_format_properties_content *text_properties)
{
	return impl_->set_cell_text_properties(text_properties);
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

int xlsx_text_context::end_cell_content()
{
    return impl_->end_cell_content();
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
void xlsx_text_context::serialize_shared_strings(std::wostream & strm)
{
    return impl_->serialize_shared_strings(strm);
}

    
}
}

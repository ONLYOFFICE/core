#include "precompiled_cpodf.h"
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
#include "../odf/calcs_styles.h"


namespace cpdoccore {
namespace oox {

class xlsx_text_context::Impl: boost::noncopyable
{
public:
	Impl(odf::styles_container & styles_);
public:
	void add_text(const std::wstring & text);
    
    void start_paragraph(const std::wstring & styleName);
    void end_paragraph();

    void start_span(const std::wstring & styleName);
    void end_span();
    std::wstring end_span2();

    void start_cell_content();
    int end_cell_content();

	void start_comment_content();
	std::wstring end_comment_content();

	void start_drawing_content();
	std::wstring end_drawing_content(); 

    void write_shared_strings(std::wostream & strm);
	
	void ApplyTextProperties(std::wstring style,odf::text_format_properties_content & propertiesOut, odf::style_family::type Type);

	void set_local_styles_container(odf::styles_container*  local_styles_);//это если стили объектов содержатс€ в другом документе

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

	odf::styles_container & styles_;

	odf::styles_container * local_styles_ptr_;

    std::wstring dump_text();
    void write_rPr(std::wostream & strm);
   
	size_t paragraphs_cout_; //???? тока из за начала отсчета?
   
	std::wstringstream text_;
    std::wstringstream output_;
    xlsx_shared_strings xlsx_shared_strings_;
   
	std::wstring styles_paragraph_;//был вектор ... не нужен, так как в один момент времени может быть тока один стиль параграфа,текста,объекта при приходе нового - дампитс€
    std::wstring styles_span_;

};

void xlsx_text_context::Impl::write_shared_strings(std::wostream & strm)
{
    xlsx_shared_strings_.xlsx_serialize(strm);    
}


xlsx_text_context::Impl::Impl(odf::styles_container & styles): paragraphs_cout_(0),styles_(styles),
				in_comment(false),in_draw(false),in_paragraph(false),in_span(false),in_cell_content(false)
{
}

void xlsx_text_context::Impl::add_text(const std::wstring & text)
{
    text_ << text;
	
	if (!in_comment && !in_draw)
		dump_text();
}

void xlsx_text_context::Impl::set_local_styles_container(odf::styles_container * local_styles_)
{
	local_styles_ptr_= local_styles_;
}

void xlsx_text_context::Impl::start_paragraph(const std::wstring & styleName)
{
    if (paragraphs_cout_++ > 0)
    {	
		if (styles_paragraph_ != styleName)
		{
			dump_text();
		}
		// конец предыдущего абзаца и начало следующего
		//text_ << L"&#10;";
        text_ << L"\n"; 
	}else
	{
		text_.str(std::wstring());
	}
	styles_paragraph_ = styleName;
	in_paragraph = true;
}

void xlsx_text_context::Impl::end_paragraph()
{
    if (!in_comment && !in_draw)
	{
		dump_text();
		styles_paragraph_ = L"";
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
		if (styles_span_ !=styleName || in_span)
		{
			dump_text();
		}
	 }
	 styles_span_ = styleName;
	 in_span=true;
}

void xlsx_text_context::Impl::end_span() //odf кор€венько написан - возможны повторы стилей в последовательных кусках текста
//пока с анализом стилей тока комменты - остальные текстовые куски как есть.. с охрененным возможно дубл€жом
{
     if (!in_comment && !in_draw)
	 {
		dump_text();
		styles_span_=L"";
	 }
	 in_span=false;
}

std::wstring xlsx_text_context::Impl::end_span2()
{
    const std::wstring content = dump_text();
    styles_span_ = L"";
	
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

void xlsx_text_context::Impl::ApplyTextProperties(std::wstring style,odf::text_format_properties_content & propertiesOut, odf::style_family::type Type)
{
	std::vector<const odf::style_instance *> instances;

	if (local_styles_ptr_)
	{
		odf::style_instance * defaultStyle = local_styles_ptr_->style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf::style_instance* styleInst = local_styles_ptr_->style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	else
	{
		odf::style_instance * defaultStyle = styles_.style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf::style_instance* styleInst = styles_.style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	propertiesOut.apply_from(calc_text_properties_content(instances));
}

void xlsx_text_context::Impl::write_rPr(std::wostream & strm)
{
	if (styles_paragraph_.length()<1 && styles_span_.length()<1 && !(hyperlink_hId.length()>0 && in_draw) )return;

	odf::text_format_properties_content		text_properties_paragraph_;
	ApplyTextProperties	(styles_paragraph_,	text_properties_paragraph_,odf::style_family::Paragraph);
	
	odf::text_format_properties_content		text_properties_span_;
	ApplyTextProperties(styles_span_,		text_properties_span_,odf::style_family::Text);

	odf::text_format_properties_content text_properties_;

	text_properties_.apply_from(text_properties_paragraph_);
	text_properties_.apply_from(text_properties_span_);

	_CP_OPT(double)	dValFontSize;
	if (text_properties_.fo_font_size_)
		dValFontSize=text_properties_.fo_font_size_->get_length().get_value();
	
	_CP_OPT(std::wstring) sValFontFamily;	
	if (text_properties_.fo_font_family_)	
		sValFontFamily=text_properties_.fo_font_family_.get();
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
			//oox_serialize_style_text(strm,odf::text_format_properties_content & properties);
			CP_XML_NODE(L"a:rPr")
			{
				//стр 3197
				if (dValFontSize)									{CP_XML_ATTR(L"sz", (int)(dValFontSize.get()*100));}
				if ((iValFontStyle) && (iValFontStyle.get() >0))	{CP_XML_ATTR(L"i", "true");}
				if ((iValFontWeight) && (iValFontWeight.get() >0))	{CP_XML_ATTR(L"b", "true");}				
				if (sValFontFamily)									{CP_XML_ATTR(L"typeface", sValFontFamily.get());}
				
				if (sValFontColor){CP_XML_NODE(L"a:solidFill")	{CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val", sValFontColor.get());}}}

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
				if (sValFontFamily)							{CP_XML_NODE(L"rFont")	{CP_XML_ATTR(L"val", sValFontFamily.get());}}
				if (dValFontSize)							{CP_XML_NODE(L"sz")		{CP_XML_ATTR(L"val", (int)(dValFontSize.get()));}}
				if (sValFontColor)							{CP_XML_NODE(L"color")	{CP_XML_ATTR(L"rgb", sValFontColor.get());}}
				if ((iValFontStyle) && (iValFontStyle.get() >0))  {CP_XML_NODE(L"i")		{CP_XML_ATTR(L"val", "true");}}
				if ((iValFontWeight) && (iValFontWeight.get() >0)){CP_XML_NODE(L"b")		{CP_XML_ATTR(L"val", "true");}}
			}
		}
    }
}

std::wstring xlsx_text_context::Impl::dump_text()
{
    const std::wstring content = xml::utils::replace_text_to_xml(text_.str());

	if (content.length()<1) 
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
					CP_XML_ATTR(L"xml:space", L"preserve");
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
   
	output_.str(std::wstring());//строка дампа
    
	text_.str(std::wstring()); //приход€щие куски текста
    
	styles_paragraph_ = L"";
    styles_span_ = L"";

	in_cell_content = true;
}

void xlsx_text_context::Impl::start_comment_content()
{
    paragraphs_cout_ = 0;
   
	output_.str(std::wstring());
    text_.str(std::wstring());
    
	styles_paragraph_ = L"";
    styles_span_ = L"";

	in_comment = true;
}
void xlsx_text_context::Impl::start_drawing_content()
{
    paragraphs_cout_ = 0;
   
	output_.str(std::wstring());
    text_.str(std::wstring());
    
	styles_paragraph_ = L"";
    styles_span_ = L"";

	in_draw = true;
}
std::wstring xlsx_text_context::Impl::end_comment_content()
{
	dump_text();//если в комменте куча абзацев со одним стилем - сдампитс€ здесь - иначе дампитс€ по мере прихода каждого нового стил€

	std::wstring comment= output_.str();
  
	paragraphs_cout_ = 0;
    
	output_.str(std::wstring());
    text_.str(std::wstring());
	
	styles_paragraph_ = L"";
    styles_span_=L"";

	in_comment = false;
	return comment;
}
std::wstring xlsx_text_context::Impl::end_drawing_content()
{
	dump_text();//если в draw куча абзацев со одним стилем - сдампитс€ здесь - иначе дампитс€ по мере прихода каждого нового стил€

	std::wstring draw= output_.str();
  
	paragraphs_cout_ = 0;
    
	output_.str(std::wstring());
    text_.str(std::wstring());
	
	styles_paragraph_ = L"";
    styles_span_=L"";

	in_draw = false;
	return draw;
}
int xlsx_text_context::Impl::end_cell_content()
{
	dump_text();

	const int sharedStrId = output_.str().empty() ? (-1) :  static_cast<int>(xlsx_shared_strings_.add(output_.str()));
	//???? нужно ли здесь очищать все ????? - проверить стили на кучках - и проверить как мен€ютс€ стили внутри одной €чейки - то есть здешнее переопределение внешнего стил€
	in_cell_content = false;   
	return sharedStrId;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

xlsx_text_context::xlsx_text_context(odf::styles_container & _styles): impl_(new xlsx_text_context::Impl(_styles))
{}


xlsx_text_context::~xlsx_text_context()
{
}
void xlsx_text_context::set_local_styles_container(odf::styles_container*  local_styles_)
{
	return impl_->set_local_styles_container(local_styles_);
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
void xlsx_text_context::write_shared_strings(std::wostream & strm)
{
    return impl_->write_shared_strings(strm);
}

    
}
}

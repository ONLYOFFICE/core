#include "precompiled_cpodf.h"

#include "pptx_conversion_context.h"
#include "pptx_text_context.h"

#include <iostream>
#include <list>
#include <boost/foreach.hpp>

#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/utils.h>

#include "../odf/odfcontext.h"
#include "../odf/style_text_properties.h"
#include "../odf/calcs_styles.h"


namespace cpdoccore {
namespace oox {

class pptx_text_context::Impl: boost::noncopyable
{
public:
	Impl(odf::odf_read_context & odf_context_,	pptx_conversion_context & pptx_context_);
public:
	styles_context & get_styles_context() { return styles_context_; }

	void add_text(const std::wstring & text);
    
    void start_paragraph(const std::wstring & styleName);
    void end_paragraph();

    void start_span(const std::wstring & styleName);
    void end_span();
    std::wstring end_span2();

	void start_drawing_content();
	std::wstring end_drawing_content();

	void ApplyTextProperties(std::wstring style,odf::text_format_properties_content & propertiesOut, odf::style_family::type Type);

	void set_local_styles_container(odf::styles_container*  local_styles_);//это если стили объектов содержатся в другом документе

	void end_hyperlink(std::wstring hId);
	void start_hyperlink();

    void start_list(const std::wstring & StyleName, bool Continue = false);
    void end_list();
    void start_list_item(bool restart = false);
	void end_list_item();


private:
    styles_context styles_context_;

	odf::odf_read_context & odf_context_ ;
	std::wstring hyperlink_hId;
	
	bool in_span;
	bool in_paragraph;
	bool in_list;

	odf::styles_container * local_styles_ptr_;

    void write_rPr(std::wostream & strm);
	void write_pPr(std::wostream & strm);
   
    std::wstring dump_paragraph();
	void dump_run();
  
	size_t paragraphs_cout_; //???? тока из за начала отсчета?
   
	std::wstringstream text_; //приходящий текст
    std::wstringstream paragraph_; //перманенто скидываемые параграфы
    std::wstringstream run_; //перманенто скидываемые куски с быть может разными свойствами
   
	std::wstring styles_paragraph_;//был вектор ... не нужен, так как в один момент времени может быть тока один стиль параграфа,текста,объекта при приходе нового - дампится
    std::wstring styles_span_;

/////////////lists////////////
    std::list<std::wstring> list_style_stack_;
    bool first_element_list_item_;
    // счетчик для нумерации имен созданных в процессе конвертации стилей
    size_t new_list_style_number_;
    // цепочки переименований нумераций
    boost::unordered_map<std::wstring, std::wstring> list_style_renames_;
   
	void write_list_styles(std::wostream & strm);
	void write_list_properties(std::wostream & strm);
	std::wstring find_list_rename(const std::wstring & ListStyleName);
	std::wstring current_list_style();
///////////////////////////

	pptx_conversion_context & pptx_context_;

};

pptx_text_context::Impl::Impl(odf::odf_read_context & odf_contxt_, pptx_conversion_context & pptx_contxt_): 
		odf_context_(odf_contxt_),	pptx_context_(pptx_contxt_),
		paragraphs_cout_(0),in_paragraph(false),in_span(false)
{
	new_list_style_number_=0;
	local_styles_ptr_ = NULL;
}

void pptx_text_context::Impl::add_text(const std::wstring & text)
{
    text_ << text;
}

void pptx_text_context::Impl::set_local_styles_container(odf::styles_container * local_styles_)
{
	local_styles_ptr_= local_styles_;
}

void pptx_text_context::Impl::start_paragraph(const std::wstring & styleName)
{
    if (paragraphs_cout_++ > 0)
    {	
		if (styles_paragraph_ != styleName)
		{
			dump_paragraph();
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
	
	get_styles_context().start();
}

void pptx_text_context::Impl::end_paragraph()
{
	in_paragraph = false;
}

void pptx_text_context::Impl::start_span(const std::wstring & styleName)//кусок текста в абзаце(параграфе) со своими свойствами - этто может быть и 1 буква
{
	if (styles_span_ !=styleName || in_span)
	{
		dump_run();
	}

	styles_span_ = styleName;

	in_span=true;
}

void pptx_text_context::Impl::end_span() 
{

	 in_span=false;
}

std::wstring pptx_text_context::Impl::end_span2()
{
    dump_run();
    styles_span_ = L"";
	
	in_span = false;
    return run_.str();
}
void pptx_text_context::Impl::start_hyperlink()
{
	dump_paragraph();//проверить
}

void pptx_text_context::Impl::end_hyperlink(std::wstring hId)
{
	hyperlink_hId = hId;
}

void pptx_text_context::Impl::ApplyTextProperties(std::wstring style,odf::text_format_properties_content & propertiesOut, odf::style_family::type Type)
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
		odf::style_instance * defaultStyle = odf_context_.styleContainer().style_default_by_type(Type);
		if (defaultStyle)instances.push_back(defaultStyle);

		odf::style_instance* styleInst = odf_context_.styleContainer().style_by_name(style, Type,false/*process_headers_footers_*/);
		if(styleInst)instances.push_back(styleInst);
	}
	propertiesOut.apply_from(calc_text_properties_content(instances));
}

void pptx_text_context::Impl::write_pPr(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{			
		CP_XML_NODE(L"a:pPr")
		{
			if (!list_style_stack_.empty())
			{		
				int level = list_style_stack_.size()-1;
				CP_XML_ATTR(L"lvl",level);
				//if (styles_paragraph_.length()>0)
				//{
				//	if (odf::style_instance * styleInst= odf_context_.styleContainer().style_by_name(styles_paragraph_, odf::style_family::Paragraph,false) )
				//	{
				//	}
				//}
				//write_list_properties(strm);
				odf::list_style_container & list_styles = odf_context_.listStyleContainer();

				if (!list_styles.empty())
				{
					odf::text_list_style * s = list_styles.list_style_by_name(list_style_stack_.back());
					 if (s)
					 {
						odf::office_element_ptr  elm = s->get_content()[level];
						if (elm)
						{
							elm->pptx_convert(pptx_context_);
						}
						CP_XML_STREAM() << get_styles_context().list_style().str();
					}
				}
			}
			//else
			//{
			//	if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))
			//	{
			//		textProperties->content().pptx_convert(Context);
			//		CP_XML_STREAM() << get_styles_context().text_style().str();
			//	}
			//}

		}
	}  
}

void pptx_text_context::Impl::write_rPr(std::wostream & strm)
{
	if (styles_paragraph_.length()<1 && styles_span_.length()<1 && !(hyperlink_hId.length()>0) )return;

	odf::text_format_properties_content		text_properties_paragraph_;
	ApplyTextProperties	(styles_paragraph_,	text_properties_paragraph_,odf::style_family::Paragraph);
	
	odf::text_format_properties_content		text_properties_span_;
	ApplyTextProperties(styles_span_,		text_properties_span_,odf::style_family::Text);

	odf::text_format_properties_content text_properties_;

	text_properties_.apply_from(text_properties_paragraph_);
	text_properties_.apply_from(text_properties_span_);

	text_properties_.pptx_convert(pptx_context_);

	strm << get_styles_context().text_style().str();

	//_CP_OPT(double)	dValFontSize;
	//if (text_properties_.fo_font_size_)
	//	dValFontSize=text_properties_.fo_font_size_->get_length().get_value();
	//
	//_CP_OPT(std::wstring) sValFontFamily;	
	//if (text_properties_.fo_font_family_)	
	//	sValFontFamily=text_properties_.fo_font_family_.get();
	////else if (text_properties_.style_font_name_) - тут может быть отсылка к font_face)decl !!!!
	////	sValFontFamily=text_properties_.style_font_name_.get();

	//_CP_OPT(std::wstring) sValFontColor;
	//if (text_properties_.fo_color_)		
	//	sValFontColor=text_properties_.fo_color_->get_hex_value();

	//_CP_OPT(int)	iValFontWeight;
	//if (text_properties_.fo_font_weight_)
	//	iValFontWeight=text_properties_.fo_font_weight_->get_type();

	//_CP_OPT(int)	iValFontStyle;
	//if(text_properties_.fo_font_style_)
	//	iValFontStyle=text_properties_.fo_font_style_->get_type();

	//CP_XML_WRITER(strm)
 //   {
	//	//oox_serialize_style_text(strm,text_properties_);
	//	//oox_serialize_style_text(strm,odf::text_format_properties_content & properties);
	//	CP_XML_NODE(L"a:rPr")
	//	{
	//		//стр 3197
	//		if (dValFontSize)									{CP_XML_ATTR(L"sz", (int)(dValFontSize.get()*100));}
	//		if ((iValFontStyle) && (iValFontStyle.get() >0))	{CP_XML_ATTR(L"i", "true");}
	//		if ((iValFontWeight) && (iValFontWeight.get() >0))	{CP_XML_ATTR(L"b", "true");}				
	//		if (sValFontFamily)									{CP_XML_ATTR(L"typeface", sValFontFamily.get());}
	//		
	//		if (sValFontColor){CP_XML_NODE(L"a:solidFill")	{CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val", sValFontColor.get());}}}

	//		if (hyperlink_hId.length()>0)
	//		{
	//			CP_XML_NODE(L"a:hlinkClick ")
	//			{
	//				CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
	//				CP_XML_ATTR(L"r:id", hyperlink_hId);
	//			}
	//		}
	//	}

		//CP_XML_NODE(L"charset")
		//{
		//	CP_XML_ATTR(L"val",204);
		//}	
		//CP_XML_NODE(L"family")
		//{
		//	CP_XML_ATTR(L"val",2);
		//}
    //}
}
std::wstring pptx_text_context::Impl::dump_paragraph()
{				
	dump_run();//last

	std::wstring & str_run = run_.str();

	if (str_run.length() > 0)
	{
		CP_XML_WRITER(paragraph_)
		{
			CP_XML_NODE(L"a:p")
			{
				write_pPr(CP_XML_STREAM());

				CP_XML_STREAM() << run_.str();
			}
		}
		run_.str(std::wstring());
	}
	return paragraph_.str();
}

void pptx_text_context::Impl::dump_run()
{
	const std::wstring content = xml::utils::replace_text_to_xml(text_.str());
 	if (content.length()<1) return ;     

	CP_XML_WRITER(run_)
    {
        if (!content.empty())
        {
			CP_XML_NODE(L"a:r")
			{
				write_rPr(CP_XML_STREAM());                
				
				CP_XML_NODE(L"a:t")
				{
					//CP_XML_ATTR(L"xml:space", L"preserve"); временно уберу ...
					CP_XML_STREAM() << content;
                }
	         }
            text_.str(std::wstring());
        }
    }
	hyperlink_hId =L"";
}


void pptx_text_context::Impl::start_drawing_content()
{
    paragraphs_cout_ = 0;
   
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
    
	styles_paragraph_ = L"";
    styles_span_ = L"";

	get_styles_context().start();

}

std::wstring pptx_text_context::Impl::end_drawing_content()
{
	std::wstringstream str_styles;

	write_list_styles(str_styles);
	str_styles << dump_paragraph();
  
	paragraphs_cout_ = 0;
    
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
	
	styles_paragraph_ = L"";
    styles_span_=L"";

	return str_styles.str();
}
void pptx_text_context::Impl::start_list_item(bool restart)
{
	in_list = true;
    first_element_list_item_ = true;

    if (restart && !list_style_stack_.empty())
    {
        const std::wstring curStyleName = current_list_style();
        const std::wstring newStyleName = curStyleName + boost::lexical_cast<std::wstring>(new_list_style_number_++);
        list_style_renames_[curStyleName] = newStyleName;

        odf::list_style_container & lists = odf_context_.listStyleContainer();
        odf::text_list_style * curStyle = lists.list_style_by_name(curStyleName);
        lists.add_list_style(curStyle, newStyleName);
        end_list();
        start_list(newStyleName);
    }
}

void pptx_text_context::Impl::start_list(const std::wstring & StyleName, bool Continue)
{
    if (!StyleName.empty())
    {
        std::wstring name = StyleName;
        if (Continue)
            name = find_list_rename(name);

        list_style_stack_.push_back(name);
    }
    else if (!list_style_stack_.empty())
        list_style_stack_.push_back(list_style_stack_.back());
    else
        list_style_stack_.push_back(L"");
}

void pptx_text_context::Impl::end_list()
{
	in_list = false;
  
	list_style_stack_.pop_back();// пока не стираем .. как сохраним в lstStyles - очистим
}

std::wstring pptx_text_context::Impl::current_list_style()
{
    if (!list_style_stack_.empty())
        return list_style_stack_.back();    
    else
        return L"";
}
std::wstring pptx_text_context::Impl::find_list_rename(const std::wstring & ListStyleName)
{
    std::wstring name = ListStyleName;
    while (list_style_renames_.count(name) > 0)
        name = list_style_renames_.at(name);

    return name;
}

void pptx_text_context::Impl::end_list_item()
{
	dump_paragraph();
	paragraphs_cout_--;
	styles_paragraph_ = L"";

	in_list = false;
}

void pptx_text_context::Impl::write_list_properties(std::wostream & strm)
{
    if (!list_style_stack_.empty())
    {
        //if (first_element_list_item_)
        {
            const int id = odf_context_.listStyleContainer().id_by_name( current_list_style() );
            //const int id = list_context_.id_by_name( current_list_style() );
			CP_XML_WRITER(strm)
			{
				CP_XML_NODE(L"a:buAutoNum")
				{
					CP_XML_ATTR(L"type",L"arabicPeriod");
					CP_XML_ATTR(L"startAt",id);//
				}
				first_element_list_item_ = false;
			}
		}
    }
}
void pptx_text_context::Impl::write_list_styles(std::wostream & strm)//defaults style paragraph & lists
{
    odf::list_style_container & list_styles = odf_context_.listStyleContainer();

    if (list_styles.empty())
        return;
   
	get_styles_context().start();
	get_styles_context().extern_node() = L"a:defRPr";

	CP_XML_WRITER(strm)
	{ 	
		CP_XML_NODE(L"a:lstStyle");
		//{
		//	//defPPr
		//	//...

		//	//list levels 0 - 8
		//	BOOST_FOREACH(std::wstring & st_name, list_style_stack_ )
		//	{
		//		odf::text_list_style * s = list_styles.list_style_by_name(st_name);
		//        
		//		BOOST_FOREACH(odf::office_element_ptr & elm, s->get_content())
		//		{
		//			elm->pptx_convert(pptx_context_);
		//		}
		//	}

		//	CP_XML_STREAM() << get_styles_context().list_style().str();
		//}
	}

	list_style_stack_.clear();
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

pptx_text_context::pptx_text_context(odf::odf_read_context & odf_context_, pptx_conversion_context & pptx_context_):
	impl_(new pptx_text_context::Impl(odf_context_,pptx_context_))
{}


pptx_text_context::~pptx_text_context()
{
}
void pptx_text_context::set_local_styles_container(odf::styles_container*  local_styles_)
{
	return impl_->set_local_styles_container(local_styles_);
}

void pptx_text_context::add_text(const std::wstring & text)
{
    return impl_->add_text(text);
}

void pptx_text_context::start_paragraph(const std::wstring & styleName)
{
    return impl_->start_paragraph(styleName);
}

void pptx_text_context::end_paragraph()
{
    return impl_->end_paragraph();
}

void pptx_text_context::start_span(const std::wstring & styleName)
{
    return impl_->start_span(styleName);
}

void pptx_text_context::end_span()
{
    return impl_->end_span();
}

std::wstring pptx_text_context::end_span2()
{
    return impl_->end_span2();
}

void pptx_text_context::start_list_item(bool restart)
{
	return impl_->start_list_item(restart);
}
void pptx_text_context::end_list_item()
{
	return impl_->end_list_item();
}
void pptx_text_context::start_list(const std::wstring & StyleName, bool Continue)
{
	return impl_->start_list(StyleName,Continue);
}
void pptx_text_context::end_list()
{
	return impl_->end_list();
}
void pptx_text_context::start_drawing_content()
{
	return impl_->start_drawing_content();
}
void pptx_text_context::start_hyperlink()
{
	return impl_->start_hyperlink();
}
void pptx_text_context::end_hyperlink(std::wstring hId)
{
	return impl_->end_hyperlink(hId);
}
std::wstring pptx_text_context::end_drawing_content()
{
	return impl_->end_drawing_content();
}

styles_context & pptx_text_context::get_styles_context() 
{ 
	return  impl_->get_styles_context() ; 
}
}
}

#include "precompiled_cpodf.h"
#include "docx_conversion_context.h"
#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/xml/utils.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../odf/odfcontext.h"
#include "../odf/style_text_properties.h"
#include "../odf/style_graphic_properties.h"
#include "../odf/style_ref.h"

#include "docx_package.h"
#include "docx_rels.h"
#include "logging.h"

namespace cpdoccore { 
namespace oox {
   

docx_conversion_context::docx_conversion_context(package::docx_document * OutputDocument, odf::odf_document * OdfDocument) : 
	streams_man_( streams_man::create(temp_stream_) ), 
	output_document_(OutputDocument), 
	odf_document_(OdfDocument),
	current_run_(false),
	current_processed_style_( NULL ),
	page_break_after_(false),
	page_break_before_(false),
	in_automatic_style_(false),
	next_dump_page_properties_(false),
	mediaitems_( OdfDocument->get_folder() ),
	in_paragraph_(false),
	table_context_(*this),
	section_properties_in_table_(NULL),
	new_list_style_number_(0),
	rtl_(false),
	delayed_converting_(false),
	process_headers_footers_(false),
	process_note_(noNote)
{}

void docx_conversion_context::add_element_to_run()
{
    if (!current_run_)
    {
        current_run_ = true;
	output_stream() << L"<w:r>";

    if (!text_properties_stack_.empty())
    {
        odf::style_text_properties_ptr textProp = this->current_text_properties();
        get_styles_context().start();
        textProp->content().docx_convert(*this);
        get_styles_context().write_text_style( *this );
    }

}
}

void docx_conversion_context::start_paragraph()
{
	if (in_paragraph_)
		finish_paragraph();

	output_stream() << L"<w:p>";
	
	in_paragraph_ = true;
    rtl_ = false; 
}

void docx_conversion_context::finish_paragraph()
{
	if (in_paragraph_)
	{
		output_stream() << L"</w:p>";
	}
	in_paragraph_ = false;
}


void docx_conversion_context::finish_run()
{
    if (current_run_)
    {
        output_stream() << L"</w:r>";
        current_run_ = false;
		if (get_comments_context().state()==2)
		{
			output_stream()<< L"<w:commentRangeEnd w:id=\"" << get_comments_context().current_id() << L"\" />";
			
			add_element_to_run();
				output_stream()<< L"<w:commentReference w:id=\"" << get_comments_context().current_id() << L"\" />";			
				get_comments_context().state(0);
			finish_run();
		}
    }
}
void docx_conversion_context::start_chart(std::wstring const & name)
{
	charts_.push_back(oox_chart_context::create(name));

}
void docx_conversion_context::end_chart()
{
	//current_chart().set_drawing_link(current_sheet().get_drawing_link());
	//излишняя инфа
}
oox_chart_context & docx_conversion_context::current_chart()
{
    if (!charts_.empty())
    {
        return *charts_.back().get();
    }
    else
    {
        CP_ASSERT(false);
        throw std::runtime_error("internal error");
    }
}
void docx_conversion_context::add_new_run()
{
	finish_run();
	if (get_comments_context().state()==1)
	{
		output_stream() << L"<w:commentRangeStart w:id=\"" << get_comments_context().current_id() << L"\" />";
		get_comments_context().state(2);//active
	}
    add_element_to_run();
}

std::wstring docx_conversion_context::add_hyperlink(const std::wstring & href, bool draw)
{
    return hyperlinks_.add(href,draw);
}
hyperlinks::_ref  docx_conversion_context::last_hyperlink()
{
	return hyperlinks_.last();
}


void docx_conversion_context::dump_hyperlinks(rels & Rels)
{
    hyperlinks_.dump_rels(Rels);
}

void docx_conversion_context::dump_mediaitems(rels & Rels)
{
    mediaitems_.dump_rels(Rels);
}

void docx_conversion_context::dump_headers_footers(rels & Rels) const
{
    headers_footers_.dump_rels(Rels);
}

void docx_conversion_context::dump_notes(rels & Rels) const
{
    notes_context_.dump_rels(Rels);
}

std::wstring docx_conversion_context::add_mediaitem(const std::wstring & uri, mediaitems::Type type, bool & isInternal, std::wstring & ref)
{
    return mediaitems_.add_or_find(uri, type, isInternal, ref);    
}

void docx_conversion_context::start_document()
{
    streams_man_ = streams_man::create(document_xml_);

	output_stream() << L"<w:document ";
	
	output_stream() << L"xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" "; 
	output_stream() << L"xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" "; 
	output_stream() << L"xmlns:o=\"urn:schemas-microsoft-com:office:office\" "; 
	output_stream() << L"xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "; 
	output_stream() << L"xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" "; 
	output_stream() << L"xmlns:v=\"urn:schemas-microsoft-com:vml\" "; 
	output_stream() << L"xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" "; 
	output_stream() << L"xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" "; 
	output_stream() << L"xmlns:w10=\"urn:schemas-microsoft-com:office:word\" "; 
	output_stream() << L"xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" "; 
	output_stream() << L"xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" "; 
	output_stream() << L"xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" "; 
	output_stream() << L"xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" "; 
	output_stream() << L"xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" "; 
	output_stream() << L"xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" "; 
	output_stream() << L"mc:Ignorable=\"w14 wp14\">";


	//apply page-default prop
	//пока временно сюда воткнем обработку свойств документа в целом
}


void docx_conversion_context::end_document()
{
    output_stream() << L"</w:document>";
    output_document_->get_word_files().set_document( package::simple_element::create(L"document.xml", document_xml_.str()) );

    output_document_->get_word_files().set_media( mediaitems_ );
    output_document_->get_word_files().set_headers_footers(headers_footers_);
    output_document_->get_word_files().set_notes(notes_context_);
	output_document_->get_word_files().set_comments(comments_context_);
	output_document_->get_word_files().set_settings(package::simple_element::create(L"settings.xml",dump_settings_document()));

////////////////////////	
	int count = 0;
    BOOST_FOREACH(const oox_chart_context_ptr& chart, charts_)
    {
		count++;
		package::chart_content_ptr content = package::chart_content::create();

		chart->write_to(content->content());

		output_document_->get_word_files().add_charts(content);
	
	}    
////////////////////////////////
	//content->add_rel(relationship(dId, kType, dName));

	output_document_->get_word_files().update_rels(*this);
/////////////////////////////////////
	
}


std::wstring  docx_conversion_context::dump_settings_document()
{
	std::wstringstream output(L"");
    CP_XML_WRITER(output)
    {
        CP_XML_NODE(L"w:settings")
        {
			CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
			CP_XML_ATTR(L"xmlns:o",		L"urn:schemas-microsoft-com:office:office");
			CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:m",		L"http://schemas.openxmlformats.org/officeDocument/2006/math");
			CP_XML_ATTR(L"xmlns:v",		L"urn:schemas-microsoft-com:vml");
			CP_XML_ATTR(L"xmlns:w10",	L"urn:schemas-microsoft-com:office:word");
			CP_XML_ATTR(L"xmlns:w",		L"http://schemas.openxmlformats.org/wordprocessingml/2006/main");
			CP_XML_ATTR(L"xmlns:w14",	L"http://schemas.microsoft.com/office/word/2010/wordml");
			CP_XML_ATTR(L"xmlns:sl",	L"http://schemas.openxmlformats.org/schemaLibrary/2006/main");
			CP_XML_ATTR(L"mc:Ignorable", L"w14");

			_CP_OPT(bool) boolVal;
			_CP_OPT(std::wstring) strVal;
			_CP_OPT(int) intVal;

			if (odf::GetProperty(settings_properties_,L"evenAndOddHeaders",boolVal))
			{
				CP_XML_NODE(L"w:evenAndOddHeaders");
			}
		}
	//output << L"<w:zoom w:percent=\"57\"/> ";
	}
	return output.str();
}

void docx_conversion_context::start_body()
{
    output_stream() << L"<w:body>";
}

void docx_conversion_context::end_body()
{
    //odf_document_->odf_context.pageLayoutContainer().instances()

    output_stream() << L"</w:body>";
}

void docx_conversion_context::start_office_text()
{}

void docx_conversion_context::end_office_text()
{}

namespace 
{
    std::wstring StyleTypeOdf2Docx(odf::style_family::type Type)
    {
        switch(Type)
        {
        case odf::style_family::Text:
            return L"character";
        case odf::style_family::Paragraph:
            return L"paragraph";
            // TODO
        default:
            return L"";
        }
    }

    std::wstring StyleDisplayName(const std::wstring & Name, odf::style_family::type Type)
    {
        if (!Name.empty())
            return Name;
        else
        {
            switch(Type)
            {
            case odf::style_family::Paragraph:
                return L"Normal";
                break;
            default:
                return std::wstring(L"DStyle_") + boost::lexical_cast<std::wstring>(odf::style_family( Type) );
            }
        }
    }
}

void docx_conversion_context::process_list_styles()
{
    streams_man_ = streams_man::create(numbering_xml_);
    std::wostream & strm = output_stream();
    
    odf::odf_document * doc = root();
    if (!doc)
        return;

    odf::odf_read_context & context =  doc->odf_context();
    odf::list_style_container & list_styles = context.listStyleContainer();

    if (list_styles.empty())
        return;
    
	strm << L"<w:numbering xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">";

    std::vector<int> numIds;
	BOOST_FOREACH(odf::list_style_instance_ptr & inst, list_styles.instances())
    {
        //const int abstractNumId = list_context_.add_list_style(inst->get_text_list_style()->get_style_name());
        const int abstractNumId = list_styles.id_by_name(inst->get_style_name());
        strm << L"<w:abstractNum w:abstractNumId=\"" << abstractNumId << "\">";
        numIds.push_back(abstractNumId);

        odf::office_element_ptr_array & content = inst->get_text_list_style()->get_content();
        
        BOOST_FOREACH(odf::office_element_ptr & elm, content)
        {
            start_text_list_style(inst->get_text_list_style()->get_style_name());
            elm->docx_convert(*this);
            // TODO
            end_text_list_style();        
        }

        strm << L"</w:abstractNum>";
    }

    BOOST_FOREACH(int numId, numIds)
    {
        strm << L"<w:num w:numId=\"" << numId << L"\" >";
        strm << L"<w:abstractNumId w:val=\"" << numId << "\" />";
        strm << L"</w:num>";    
    }

    strm << L"</w:numbering>";

    output_document_->get_word_files().set_numbering(package::simple_element::create(L"numbering.xml", numbering_xml_.str()) );
}

void docx_conversion_context::process_fonts()
{
    streams_man_ = streams_man::create(fontTable_xml_);
    std::wostream & strm = output_stream();
    strm << L"<w:fonts xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">";

    if (odf::odf_document * doc = root())
    {
        odf::odf_read_context & context =  doc->odf_context();
        odf::fonts_container & fonts = context.fontContainer();

        BOOST_FOREACH(odf::font_instance_ptr & inst, fonts.instances())
        {
            strm << L"<w:font w:name=\"" << inst->name() << L"\" >";

            if (!inst->charset().empty())
                strm << L"<w:charset w:val=\"" << inst->charset() <<"\" />";

            if (!inst->family().empty())
                strm << L"<w:family w:val=\"" << inst->family() << "\" />";

            if (!inst->pitch().empty())
                strm << L"<w:pitch w:val=\"" << inst->pitch() << "\" />";

            if (!inst->alt_name().empty())
                strm << L"<w:altName w:val=\"" << inst->alt_name() << "\" />";

            strm << L"</w:font>";
        }
    }

    strm << L"</w:fonts>";

    output_document_->get_word_files().set_fontTable(package::simple_element::create(L"fontTable.xml", fontTable_xml_.str()));
}

void docx_conversion_context::process_styles()
{
    streams_man_ = streams_man::create(styles_xml_);

    std::wostream & _Wostream = output_stream();
  
	_Wostream << L"<w:styles ";

	_Wostream << L"xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" ";
	_Wostream << L"xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "; 
	_Wostream << L"xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" "; 
	_Wostream << L"xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" ";
	_Wostream << L"mc:Ignorable=\"w14\"> ";
	
    if (odf::odf_document * doc = root())
    {
        odf::odf_read_context & context =  doc->odf_context();
        odf::styles_container & styles = context.styleContainer();

        // add all styles to the map
        BOOST_FOREACH(odf::style_instance_ptr & inst, styles.instances())
        {
            style_map_.get(inst->name(), inst->type());
        }

        _Wostream << L"<w:docDefaults>";

        if (odf::style_instance * defaultParStyle = styles.style_default_by_type(odf::style_family::Paragraph))
        {
            _Wostream << L"<w:pPrDefault>";
            if ( odf::style_content * content = defaultParStyle->content())
            {
                start_process_style(defaultParStyle);
                content->docx_convert(*this);
                end_process_style();
			}
            _Wostream << L"</w:pPrDefault>";
        }

        _Wostream << L"</w:docDefaults>";

        BOOST_FOREACH(odf::style_instance_ptr & inst, styles.instances())
        {
            if (!inst->is_automatic() && 
                (
                inst->type() == odf::style_family::Paragraph ||
                inst->type() == odf::style_family::Text
                )
                )
            {
                const std::wstring id = style_map_.get(inst->name(), inst->type());
                _Wostream << L"<w:style w:styleId=\"" << id << L"\" w:type=\"" << StyleTypeOdf2Docx(inst->type()) << L"\" >\n";

                const std::wstring displayName = StyleDisplayName(inst->name(), inst->type());

                _Wostream << L"<w:name w:val=\"" << displayName << L"\" />\n";

                if (odf::style_instance * baseOn = inst->parent())
                {
                    const std::wstring basedOnId = style_map_.get(baseOn->name(), baseOn->type());
                    _Wostream << L"<w:basedOn w:val=\"" << basedOnId << "\" />\n";
                }
                else if (!inst->is_default() && style_map_.check(L"", inst->type()))
                {
                    const std::wstring basedOnId = style_map_.get(L"", inst->type());
                    _Wostream << L"<w:basedOn w:val=\"" << basedOnId << "\" />\n";
                }

                if (odf::style_instance * next = inst->next())
                {
                    const std::wstring nextId = style_map_.get(next->name(), next->type());
                    _Wostream << L"<w:next w:val=\"" << nextId << "\" />\n";
                }
                else if (inst->is_default())
                {
                    // self
                    _Wostream << L"<w:next w:val=\"" << id << "\" />\n";
                }

                if (odf::style_content * content = inst->content())
                {
                    start_process_style(inst.get());
                    content->docx_convert(*this);
                    end_process_style();
                }

                _Wostream << L"</w:style>\n";                
            }
        }
    }
    _Wostream << L"</w:styles>";

    output_document_->get_word_files().set_styles( package::simple_element::create(L"styles.xml", styles_xml_.str()) );
}

void docx_conversion_context::start_process_style(const odf::style_instance * Instance)
{
    current_processed_style_ = Instance;
}

void docx_conversion_context::end_process_style()
{
    current_processed_style_ = NULL;
}

void docx_conversion_context::start_process_style_content()
{
    styles_context_.start();
}

void docx_conversion_context::process_page_properties()
{
    if (is_next_dump_page_properties())
    {
        const std::wstring pageProperties = get_page_properties();
        root()->odf_context().pageLayoutContainer().page_layout_by_name(pageProperties)->docx_convert(*this);
    }
}

void docx_conversion_context::end_process_style_content()
{
    styles_context_.write_paragraph_style(*this, automatic_parent_style_);
    
    if (automatic_parent_style_.empty())
        styles_context_.write_text_style(*this);
}

void docx_conversion_context::start_automatic_style(const std::wstring & ParentId)
{
    in_automatic_style_ = true;
    automatic_parent_style_ = ParentId;
}

void docx_conversion_context::end_automatic_style()
{
    in_automatic_style_ = false;
    automatic_parent_style_ = L"";
}

bool docx_conversion_context::in_automatic_style()
{
    return in_automatic_style_;
}

void docx_conversion_context::push_text_properties(const odf::style_text_properties * TextProperties)
{
    text_properties_stack_.push_back(TextProperties);
}

void docx_conversion_context::pop_text_properties()
{
    text_properties_stack_.pop_back();
}

odf::style_text_properties_ptr docx_conversion_context::current_text_properties()
{
    odf::style_text_properties_ptr cur = boost::make_shared<odf::style_text_properties>();
    BOOST_FOREACH(const odf::style_text_properties * prop, text_properties_stack_)
    {
        if (prop)
            cur->content().apply_from( prop->content() );
    }
    return cur;
}

void docx_conversion_context::add_page_break_after()
{
    page_break_after_ = true;
}

bool docx_conversion_context::check_page_break_after()
{
    bool t = page_break_after_;
    page_break_after_ = false;
    return t;
}

void docx_conversion_context::add_page_break_before()
{
    page_break_before_ = true;
}

bool docx_conversion_context::check_page_break_before()
{
    bool t = page_break_before_;
    page_break_before_ = false;
    return t;
}


void docx_conversion_context::set_page_properties(const std::wstring & StyleName)
{
    current_page_properties_ = StyleName;
}

const std::wstring & docx_conversion_context::get_page_properties() const
{
    return current_page_properties_;
}

void docx_conversion_context::next_dump_page_properties()
{
    if (!process_headers_footers_)
        next_dump_page_properties_ = true;
}

bool docx_conversion_context::is_next_dump_page_properties()
{
    bool t = next_dump_page_properties_;
    next_dump_page_properties_ = false;
    return t;    
}


void styles_context::start()
{
    text_style_.str( std::wstring() );
    text_style_.clear();

    paragraph_style_.str( std::wstring() );
    paragraph_style_.clear();

    table_style_.str( std::wstring() );
    table_style_.clear();
}

std::wostream & styles_context::text_style()
{
    return text_style_;
}

std::wostream & styles_context::paragraph_style()
{
    return paragraph_style_;
}

std::wostream & styles_context::table_style()
{
    return table_style_;
}

void styles_context::write_text_style(docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();
    if (!text_style_.str().empty())
        _Wostream << L"<w:rPr>" << text_style_.str() << L"</w:rPr>";
}

void styles_context::write_paragraph_style(docx_conversion_context & Context, const std::wstring & ParentId)
{
    std::wostream & _Wostream = Context.output_stream();
    if (!paragraph_style_.str().empty() || !ParentId.empty())
    {
        _Wostream << L"<w:pPr>";
        Context.process_page_properties();
        if (!ParentId.empty())
            _Wostream << L"<w:pStyle w:val=\"" << ParentId << "\" />";            
        _Wostream << paragraph_style_.str();
        Context.write_list_properties();
        _Wostream << L"</w:pPr>";
    }
}

void styles_context::write_table_style(docx_conversion_context & Context)
{
    std::wostream & _Wostream = Context.output_stream();
    if (!table_style_.str().empty())
    {
        _Wostream << L"<w:tblPr>";
        _Wostream << table_style_.str();
        _Wostream << L"</w:tblPr>";        
    }
}

void docx_conversion_context::start_text_list_style(const std::wstring & StyleName)
{
    text_list_style_name_ = StyleName;    
}

void docx_conversion_context::end_text_list_style()
{
    text_list_style_name_ = L"";
}

const std::wstring & docx_conversion_context::get_text_list_style_name()
{
    return text_list_style_name_;
}

void docx_conversion_context::start_list(const std::wstring & StyleName, bool Continue)
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

void docx_conversion_context::end_list()
{
    list_style_stack_.pop_back();
}

const std::wstring docx_conversion_context::current_list_style() const
{
    if (!list_style_stack_.empty())
        return list_style_stack_.back();    
    else
        return L"";
}

void docx_conversion_context::start_list_item(bool restart)
{
    first_element_list_item_ = true;
    if (restart && !list_style_stack_.empty())
    {
        const std::wstring curStyleName = current_list_style();
        const std::wstring newStyleName = curStyleName + boost::lexical_cast<std::wstring>(new_list_style_number_++);
        list_style_renames_[curStyleName] = newStyleName;

        odf::list_style_container & lists = root()->odf_context().listStyleContainer();
        odf::text_list_style * curStyle = lists.list_style_by_name(curStyleName);
        lists.add_list_style(curStyle, newStyleName);
        end_list();
        start_list(newStyleName);
    }
}

std::wstring docx_conversion_context::find_list_rename(const std::wstring & ListStyleName) const 
{
    std::wstring name = ListStyleName;
    while (list_style_renames_.count(name) > 0)
        name = list_style_renames_.at(name);

    return name;
}

void docx_conversion_context::end_list_item()
{
}

void docx_conversion_context::write_list_properties()
{
    if (!list_style_stack_.empty())
    {
        if (first_element_list_item_)
        {
            const int id = root()->odf_context().listStyleContainer().id_by_name( current_list_style() );
            //const int id = list_context_.id_by_name( current_list_style() );
            output_stream() << L"<w:numPr>";
            output_stream() << L"<w:ilvl w:val=\"" << (list_style_stack_.size() - 1) << "\" />";
            output_stream() << L"<w:numId w:val=\"" << id << "\" />";
            output_stream() << L"</w:numPr>";
            first_element_list_item_ = false;
        }
    }
}

void docx_conversion_context::add_delayed_element(odf::office_element * Elm)
{
    delayed_elements_.push_back(Elm);    
}

void docx_conversion_context::docx_convert_delayed()
{
	if(delayed_converting_)return; //зацикливание иначе

	delayed_converting_ = true;
    while(!delayed_elements_.empty())
    {
        odf::office_element * elm = delayed_elements_.front();
        elm->docx_convert(*this);
			delayed_elements_.pop_front();
    }
	delayed_converting_=false;
}

void section_context::start_section(const std::wstring & SectionName, const std::wstring & Style)
{
    section newSec = {SectionName, Style};
    sections_.push_back(newSec);
}

void section_context::end_section()
{
    sections_.pop_back();
    set_after_section(true);
}

void docx_conversion_context::section_properties_in_table(odf::office_element * Elm)
{
    section_properties_in_table_ = Elm;
}

odf::office_element * docx_conversion_context::get_section_properties_in_table()
{
    odf::office_element * elm = section_properties_in_table_;
    section_properties_in_table_ = NULL;
    return elm;
}

namespace 
{
    // обработка Header/Footer
    // конвертируем содержимое header/footer и сохраняем результат в виде строки
    void process_one_header_footer(docx_conversion_context & Context,
        const std::wstring & styleName,
        odf::office_element * elm,
        headers_footers::Type type)
    {
        if (!elm) return;
        
        boost::shared_ptr<std::wstringstream> newXml( new std::wstringstream() );
        Context.set_stream_man( streams_man::create(*newXml) );
        
		elm->docx_convert(Context);
        
		const std::wstring & dbgStr = newXml->str();
       
		//слить если есть mediaitems, добавить релсы и обнулить их для основного документа.
		rels internal_rels;

		Context.dump_mediaitems(internal_rels);
		Context.dump_hyperlinks(internal_rels);
		Context.get_headers_footers().add(styleName, dbgStr, type, internal_rels);

		if (type == headers_footers::headerLeft || type == headers_footers::footerLeft)
		{
			Context.set_settings_property(odf::_property(L"evenAndOddHeaders",true));
		}
   }

}
void docx_conversion_context::set_settings_property(odf::_property & prop)
{
	settings_properties_.push_back(prop);
}
std::vector<odf::_property> & docx_conversion_context::get_settings_properties()
{
	return settings_properties_;
}
void docx_conversion_context::process_headers_footers()
{
    odf::odf_document * doc = root();
    if (!doc)
        return;

    process_headers_footers_ = true;

    odf::odf_read_context & context =  doc->odf_context();
    odf::page_layout_container & pageLayouts = context.pageLayoutContainer();

    // проходим по всем page layout
    BOOST_FOREACH(const odf::style_master_page * page, pageLayouts.master_pages())
    {
        const std::wstring & styleName = page->style_master_page_attlist_.style_name_.get_value_or( odf::style_ref(L"") ).style_name();
        const std::wstring masterPageNameLayout =context.pageLayoutContainer().page_layout_name_by_style(styleName);
        set_page_properties(masterPageNameLayout);
		
		process_one_header_footer(*this, styleName, page->style_header_.get(), headers_footers::header);
        process_one_header_footer(*this, styleName, page->style_footer_.get(), headers_footers::footer );
        process_one_header_footer(*this, styleName, page->style_header_first_.get(), headers_footers::headerFirst);
        process_one_header_footer(*this, styleName, page->style_footer_first_.get(), headers_footers::footerFirst );
        process_one_header_footer(*this, styleName, page->style_header_left_.get(), headers_footers::headerLeft );
        process_one_header_footer(*this, styleName, page->style_footer_left_.get(), headers_footers::footerLeft );
    }
    process_headers_footers_ = false;
}

void docx_conversion_context::set_master_page_name(const std::wstring & MasterPageName)
{
    current_master_page_name_ = MasterPageName;
}

const std::wstring & docx_conversion_context::get_master_page_name() const
{
    return current_master_page_name_;
}

std::wstring notes_context::add(const std::wstring & Content, const std::wstring & Id)
{
    instances_map & map = (type_ == odf::noteclass::Endnote) ? instances_endnotes_ : instances_footnotes_;
    instance_ptr inst = instance_ptr( new instance(Id, Content) );
    map[Id] = inst;
    return Id;
}

std::wstring notes_context::next_id()
{
    instances_map & map = (type_ == odf::noteclass::Endnote) ? instances_endnotes_ : instances_footnotes_;
    const std::wstring s = boost::lexical_cast<std::wstring>(map.size() + 1);
    return s;        
}

void notes_context::dump_rels(rels & Rels) const
{
    if (instances_footnotes_.size())
    {
        Rels.add( relationship(L"rFNId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes",  L"footnotes.xml", L"") );
    }

    if (instances_endnotes_.size())
    {
        Rels.add( relationship(L"rENId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes", L"endnotes.xml", L"") );
    }
}

void docx_conversion_context::add_note_reference()
{
    if (process_note_ != noNote)
    {
        add_element_to_run();
        output_stream() << ((process_note_ == footNote) ? L"<w:footnoteRef />" : L"<w:endnoteRef />");
        finish_run();
        process_note_ = noNote;
    }
}

}
}

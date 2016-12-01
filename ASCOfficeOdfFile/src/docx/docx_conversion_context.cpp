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


#include <boost/foreach.hpp>

#include <iostream>

#include <cpdoccore/xml/utils.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "docx_conversion_context.h"

#include "../odf/odfcontext.h"
#include "../odf/style_paragraph_properties.h"
#include "../odf/style_text_properties.h"
#include "../odf/style_table_properties.h"
#include "../odf/style_graphic_properties.h"
#include "../odf/datatypes/style_ref.h"

#include "docx_package.h"
#include "oox_rels.h"
#include "logging.h"

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

static int current_id_changes = 0;

namespace cpdoccore { 
namespace oox {

text_tracked_context::text_tracked_context(docx_conversion_context & context) :
	docx_context_(context)
{
	current_state_.clear();
}

void text_tracked_context::start_changes_content()
{
	docx_stream_ = docx_context_.get_stream_man();
	
	changes_stream_.str((std::wstring(L"")));
	docx_context_.set_stream_man( StreamsManPtr( new oox::streams_man(changes_stream_)));

	bParaStateDocx_	= docx_context_.get_paragraph_state();
	bRunStateDocx_	= docx_context_.get_run_state();

	docx_context_.set_paragraph_state	(true);		
	docx_context_.set_run_state			(false);		
	docx_context_.set_delete_text_state	(true);		
}

void text_tracked_context::end_changes_content()
{
	docx_context_.finish_run();	//0106GS-GettingStartedWithWriter_el.odt - удаленный заголовок

	current_state_.content.push_back(changes_stream_.str());
	
	docx_context_.set_delete_text_state	(false);		
	docx_context_.set_paragraph_state	(bParaStateDocx_);	
	docx_context_.set_run_state			(bRunStateDocx_);	

	docx_context_.set_stream_man		(docx_stream_);
}
void text_tracked_context::start_change (std::wstring id)
{
	current_state_.clear();

	current_state_.id	= id;
}
void text_tracked_context::end_change ()
{
	mapChanges_.insert( std::pair<std::wstring, _state>(current_state_.id, current_state_));

	current_state_.clear();
}
void text_tracked_context::set_type(int type)
{
	current_state_.type	= type;
}
void text_tracked_context::set_user_info (std::wstring &author, std::wstring &date)
{
	current_state_.author	= author;
	current_state_.date		= date;
}
void text_tracked_context::set_style_name (std::wstring style_name)
{
	current_state_.style_name = style_name;
}

text_tracked_context::_state & text_tracked_context::get_tracked_change(std::wstring id)
{
	std::map<std::wstring, _state>::iterator it = mapChanges_.find(id);
	
	if (it != mapChanges_.end())
	{
		return it->second;
	}
	else 
		return 	current_state_; //empty
}
//----------------------------------------------------------------------------------------------------------------

docx_conversion_context::docx_conversion_context(odf_reader::odf_document * OdfDocument) : 
	next_dump_page_properties_	(false),
	page_break_after_			(false),
	page_break_before_			(false),
	in_run_						(false),
	in_automatic_style_			(false),
	in_paragraph_				(false),
	in_header_					(false),
	in_drawing_content_			(false),
	text_tracked_context_		(*this),
	table_context_				(*this),
	output_document_			(NULL),
	process_note_				(noNote),
	new_list_style_number_		(0),
	is_rtl_						(false),
	is_paragraph_keep_			(false),
	is_delete_text_				(false),
	delayed_converting_			(false),
	process_headers_footers_	(false),
	process_comment_			(false),
	mediaitems_					(OdfDocument->get_folder() ),
	math_context_				(OdfDocument->odf_context().fontContainer(), false),
	odf_document_				(OdfDocument)
{
	streams_man_		= streams_man::create(temp_stream_);
	applicationFonts_	= new CApplicationFonts();
}
docx_conversion_context::~docx_conversion_context()
{
    if (applicationFonts_)
        delete applicationFonts_;
}
void docx_conversion_context::set_output_document(package::docx_document * document)
{
	output_document_ = document;
}
void docx_conversion_context::set_font_directory(std::wstring pathFonts)
{
    if (applicationFonts_)
        applicationFonts_->InitializeFromFolder(pathFonts);
}
std::wstring styles_map::get(const std::wstring & Name, odf_types::style_family::type Type)
{
    const std::wstring n = name(Name, Type);
	
	//typedef boost::unordered::unordered_map<std::wstring,std::wstring>::iterator _mapIter;
	typedef std::multimap<std::wstring,std::wstring>::iterator _mapIter;
	
	_mapIter it = map_.find( n );
    
	if (it != map_.end() )
    {
        return it->second;
    }
    else
    {
        const std::wstring id = std::wstring(L"style") + boost::lexical_cast<std::wstring>(count_++);
        map_.insert(std::make_pair(n, id));
        return id;        
    }
}
std::wstring styles_map::name(const std::wstring & Name, odf_types::style_family::type Type)
{
    return Name + L":" + boost::lexical_cast<std::wstring>(odf_types::style_family(Type));
}
void docx_conversion_context::add_element_to_run(std::wstring parenStyleId)
{
    if (!in_run_)
    {
        in_run_ = true;
		output_stream() << L"<w:r>";

		if (!text_properties_stack_.empty() || parenStyleId.length() > 0)
		{
			if (!text_properties_stack_.empty())
			{
				odf_reader::style_text_properties_ptr textProp = this->current_text_properties();
				get_styles_context().start();


				if(( textProp) && (textProp->content().r_style_))parenStyleId = _T("");
				textProp->content().docx_convert(*this);
			}
	        
			get_styles_context().docx_serialize_text_style( output_stream(), parenStyleId, text_tracked_context_.dumpRPr_);
		}

	}
}

void docx_conversion_context::start_paragraph(bool is_header)
{
	if (in_paragraph_)
		finish_paragraph();

	output_stream() << L"<w:p>";

	in_header_		= is_header;
	in_paragraph_	= true;
    is_rtl_			= false; 
	
	start_changes();
}

void docx_conversion_context::finish_paragraph()
{
	if (in_paragraph_)
	{
		end_changes();

		output_stream() << L"</w:p>";
	}
	
	in_paragraph_		= false;
	in_header_			= false;
	is_paragraph_keep_	= false;
}


void docx_conversion_context::finish_run()
{
    if (in_run_)
    {
        output_stream() << L"</w:r>";
        in_run_ = false;
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
void docx_conversion_context::start_math_formula()
{
	math_context_.start();
}

void docx_conversion_context::end_math_formula()
{
	std::wstring math_content = math_context_.end();

	if (!math_content.empty())
	{
		output_stream() << L"<m:oMath>" << math_content << L"</m:oMath>";
	}
}

void docx_conversion_context::start_chart(std::wstring  name)
{
	charts_.push_back(oox_chart_context_ptr(new oox_chart_context(mediaitems_, name)));
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
        throw std::runtime_error("internal error");
    }
}
void docx_conversion_context::add_new_run(std::wstring parentStyleId)
{
	finish_run();
	if (get_comments_context().state()==1)
	{
		output_stream() << L"<w:commentRangeStart w:id=\"" << get_comments_context().current_id() << L"\" />";
		get_comments_context().state(2);//active
	}
    add_element_to_run(parentStyleId);
}

std::wstring docx_conversion_context::add_hyperlink(const std::wstring & href, bool draw)
{
	hyperlinks::_type_place type = hyperlinks::document_place;
	
	if (process_comment_ == true)											type = hyperlinks::comment_place;
	else if	(process_note_ == footNote	|| process_note_ == footNoteRefSet)	type = hyperlinks::footnote_place;
	else if	(process_note_ == endNote	|| process_note_ == endNoteRefSet )	type = hyperlinks::endnote_place;
	
	std::wstring href_correct = xml::utils::replace_text_to_xml(href);
    boost::algorithm::replace_all(href_correct, L" .", L".");//1 (130).odt
	
	return hyperlinks_.add(href_correct, type, draw);
}
hyperlinks::_ref  docx_conversion_context::last_hyperlink()
{
	return hyperlinks_.last();
}


void docx_conversion_context::dump_hyperlinks(rels & Rels, hyperlinks::_type_place type)
{
    hyperlinks_.dump_rels(Rels, type);
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

std::wstring docx_conversion_context::add_mediaitem(const std::wstring & uri, RelsType type, bool & isInternal, std::wstring & ref)
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

    output_document_->get_word_files().set_document	( package::simple_element::create(L"document.xml", document_xml_.str()) );
	output_document_->get_word_files().set_settings	( package::simple_element::create(L"settings.xml",dump_settings_document()));
	output_document_->get_word_files().set_media	( mediaitems_, applicationFonts_);
	output_document_->get_word_files().set_comments	( comments_context_);
    output_document_->get_word_files().set_headers_footers( headers_footers_);

	package::content_types_file	& content_file_ = output_document_->get_content_types_file();
	content_file_.set_media( mediaitems_);

////////////////////////////////////////////////////////////////////////////
	dump_hyperlinks (notes_context_.footnotesRels(), hyperlinks::footnote_place);
	dump_hyperlinks (notes_context_.endnotesRels(), hyperlinks::endnote_place);
   
	output_document_->get_word_files().set_notes(notes_context_);
////////////////////////	
	int count = 0;
    BOOST_FOREACH(const oox_chart_context_ptr& chart, charts_)
    {
		count++;
		package::chart_content_ptr content = package::chart_content::create();

		chart->serialize(content->content());
		chart->dump_rels(content->get_rel_file()->get_rels());

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

            _CP_OPT(bool)  boolVal;
            _CP_OPT(std::wstring)  strVal;
            _CP_OPT(int)   intVal;

			if (odf_reader::GetProperty(settings_properties_,L"evenAndOddHeaders",boolVal))
			{
				CP_XML_NODE(L"w:evenAndOddHeaders");
			}
			if (odf_reader::GetProperty(settings_properties_,L"displayBackgroundShape",boolVal))
			{
				CP_XML_NODE(L"w:displayBackgroundShape");
			}
			if (odf_reader::GetProperty(settings_properties_,L"zoom",intVal))
			{
				CP_XML_NODE(L"w:zoom")
				{
					CP_XML_ATTR(L"w:percent",intVal.get());
				}
			}
		}
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
{
}

void docx_conversion_context::end_office_text()
{
	finish_paragraph();
}

namespace 
{
    std::wstring StyleTypeOdf2Docx(odf_types::style_family::type Type)
    {
        switch(Type)
        {
        case odf_types::style_family::Text:
            return L"character";
        case odf_types::style_family::Paragraph:
            return L"paragraph";
            // TODO
        default:
            return L"";
        }
    }

    std::wstring StyleDisplayName(const std::wstring & Name, odf_types::style_family::type Type)
    {
        if (!Name.empty())
            return Name;
        else
        {
            switch(Type)
            {
            case odf_types::style_family::Paragraph:
                return L"Normal";
                break;
            default:
                return std::wstring(L"DStyle_") + boost::lexical_cast<std::wstring>(odf_types::style_family( Type) );
            }
        }
    }
}

void docx_conversion_context::process_list_styles()
{
    streams_man_ = streams_man::create(numbering_xml_);
    std::wostream & strm = output_stream();
    
    odf_reader::odf_document * doc = root();
    if (!doc)
        return;

    odf_reader::odf_read_context & context =  doc->odf_context();
    odf_reader::list_style_container & list_styles = context.listStyleContainer();

    if (list_styles.empty())
        return;
    
	strm << L"<w:numbering \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 wp14\">";

    std::vector<int> numIds;
	BOOST_FOREACH(odf_reader::list_style_instance_ptr & inst, list_styles.instances())
    {
        odf_reader::office_element_ptr_array & content = inst->get_text_list_style()->get_content();

		if (content.size() < 1) 
			continue;
       
		const int abstractNumId = list_styles.id_by_name(inst->get_style_name());
        
        strm << L"<w:abstractNum w:abstractNumId=\"" << abstractNumId << "\">";
        numIds.push_back(abstractNumId);		
        for (int i = 0; i < (std::min)( content.size(), (size_t)9); i++)
        {
            start_text_list_style(inst->get_text_list_style()->get_style_name());
            content[i]->docx_convert(*this);
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

    if (odf_reader::odf_document * doc = root())
    {
        odf_reader::odf_read_context & context =  doc->odf_context();
        odf_reader::fonts_container & fonts = context.fontContainer();

        BOOST_FOREACH(odf_reader::font_instance_ptr & inst, fonts.instances())
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
	
    if (odf_reader::odf_document * doc = root())
    {
        odf_reader::odf_read_context & context =  doc->odf_context();
        odf_reader::styles_container & styles = context.styleContainer();

        // add all styles to the map
        BOOST_FOREACH(odf_reader::style_instance_ptr & inst, styles.instances())
        {
            styles_map_.get(inst->name(), inst->type());
        }

        _Wostream << L"<w:docDefaults>";

        if (odf_reader::style_instance * defaultParStyle = styles.style_default_by_type(odf_types::style_family::Paragraph))
        {
            _Wostream << L"<w:pPrDefault>";
            if ( odf_reader::style_content * content = defaultParStyle->content())
            {
                get_styles_context().start_process_style(defaultParStyle);
                content->docx_convert(*this);
				get_styles_context().end_process_style();
			}
            _Wostream << L"</w:pPrDefault>";
        }

        _Wostream << L"</w:docDefaults>";

        BOOST_FOREACH(odf_reader::style_instance_ptr & inst, styles.instances())
        {
            if (!inst->is_automatic() && 
					(
					inst->type() == odf_types::style_family::Paragraph ||
					inst->type() == odf_types::style_family::Text
					))
            {
                const std::wstring id = styles_map_.get(inst->name(), inst->type());
                _Wostream << L"<w:style w:styleId=\"" << id << L"\" w:type=\"" << StyleTypeOdf2Docx(inst->type()) << L"\""; 
				if (!inst->is_default())
				{
					_Wostream << L" w:customStyle=\"1\"";
				}
				_Wostream << L">";

                const std::wstring displayName = StyleDisplayName(inst->name(), inst->type());

                _Wostream << L"<w:name w:val=\"" << displayName << L"\" />";

                if (odf_reader::style_instance * baseOn = inst->parent())
                {
                    const std::wstring basedOnId = styles_map_.get(baseOn->name(), baseOn->type());
                    _Wostream << L"<w:basedOn w:val=\"" << basedOnId << "\" />";
                }
                else if (!inst->is_default() && styles_map_.check(L"", inst->type()))
                {
                    const std::wstring basedOnId = styles_map_.get(L"", inst->type());
                    _Wostream << L"<w:basedOn w:val=\"" << basedOnId << "\" />";
                }

                if (odf_reader::style_instance * next = inst->next())
                {
                    const std::wstring nextId = styles_map_.get(next->name(), next->type());
                    _Wostream << L"<w:next w:val=\"" << nextId << "\" />";
                }
                else if (inst->is_default())
                {
                    // self
                    _Wostream << L"<w:next w:val=\"" << id << "\" />";
                }

                if (odf_reader::style_content * content = inst->content())
                {
                    get_styles_context().start_process_style(inst.get());
                    content->docx_convert(*this, true);
                    get_styles_context().end_process_style();
                }

                _Wostream << L"</w:style>";                
            }
        }
    }
    _Wostream << L"</w:styles>";

    output_document_->get_word_files().set_styles( package::simple_element::create(L"styles.xml", styles_xml_.str()) );
}


void docx_conversion_context::start_process_style_content()
{
    styles_context_.start();
}

void docx_conversion_context::process_section(std::wostream & strm, odf_reader::style_columns * columns)
{
	int count_columns = 1;
	bool sep_columns = false;

	oox::section_context::_section & section = get_section_context().get();
	
	if (columns)
	{
		if ((columns->fo_column_count_) && (*columns->fo_column_count_ > 1))
		{
			count_columns =  *columns->fo_column_count_;
		}
		if (odf_reader::style_column_sep * columns_sep = dynamic_cast<odf_reader::style_column_sep *>( columns->style_column_sep_.get() ))
		{
			if (columns_sep->style_style_ != _T("none"))
				sep_columns = true;
		}
	}
	if (const odf_reader::style_instance * secStyle = root()->odf_context().styleContainer().style_by_name(section.style_, odf_types::style_family::Section, process_headers_footers_))
	{
		if (const odf_reader::style_content * content = secStyle->content())
		{
			if (odf_reader::style_section_properties * sectPr = content->get_style_section_properties())
			{
				if (odf_reader::style_columns * columns = dynamic_cast<odf_reader::style_columns *>( sectPr->style_columns_.get() ))
				{
					if (columns->fo_column_count_)
					{
						count_columns =  *columns->fo_column_count_;
					}
					if (odf_reader::style_column_sep * columns_sep = dynamic_cast<odf_reader::style_column_sep *>( columns->style_column_sep_.get() ))
					{
						if (columns_sep->style_style_ != _T("none"))
							sep_columns = true;
					}
				}

				section.margin_left_	= sectPr->common_horizontal_margin_attlist_.fo_margin_left_;
				section.margin_right_	= sectPr->common_horizontal_margin_attlist_.fo_margin_right_;
			}
		}
		if (section.is_dump_)
		{
			get_section_context().remove_section();				
		}
	}

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"w:cols")
		{
			CP_XML_ATTR(L"w:equalWidth", L"true");
			CP_XML_ATTR(L"w:num", count_columns);
			CP_XML_ATTR(L"w:sep", sep_columns);
			CP_XML_ATTR(L"w:space",0);
		}
	}
}
bool docx_conversion_context::process_page_properties(std::wostream & strm)
{
    if (is_next_dump_page_properties() || get_section_context().get().is_dump_)
    {
        std::wstring pageProperties = get_page_properties();
		odf_reader::page_layout_instance * page_layout_instance_ = root()->odf_context().pageLayoutContainer().page_layout_by_name(pageProperties);

		if (page_layout_instance_) 
		{
				page_layout_instance_->docx_convert_serialize(strm, *this);
		}
		else
		{
			CP_XML_WRITER(strm)
			{
				CP_XML_NODE(L"w:sectPr")
				{
					process_section( CP_XML_STREAM(), NULL);

					CP_XML_NODE(L"w:type")
					{				
						CP_XML_ATTR(L"w:val", L"continuous");
					}
				}
			}
		}
		next_dump_page_properties(false);
		return true;
    }
	return false;
}

void docx_conversion_context::end_process_style_content(bool in_styles)
{
   docx_serialize_paragraph_style(output_stream(), automatic_parent_style_, in_styles);
    
    if (automatic_parent_style_.empty())
        styles_context_.docx_serialize_text_style( output_stream(), L"", text_tracked_context_.dumpRPr_);
}

void docx_conversion_context::docx_serialize_paragraph_style(std::wostream & strm, const std::wstring & ParentId, bool in_styles)
 //in_styles = true -> styles.xml
//почему то конструкция <pPr><rPr/></pPr><rPr/> "не работает" в части в rPr в ms2010 )
{
	bool in_drawing	= false;

 	if (get_drawing_context().get_current_shape() || get_drawing_context().get_current_frame())
	{
		in_drawing = true;
	}
	std::wstringstream & paragraph_style	= get_styles_context().paragraph_nodes();
 	std::wstringstream & run_style			= get_styles_context().text_style();
   
	CP_XML_WRITER(strm)
	{
		if (get_section_context().dump_.empty() == false && (!ParentId.empty() || get_section_context().get().is_dump_ || in_header_) 
			 && !get_table_context().in_table() && !in_drawing)
		{//две подряд секции или если стиль определен и в заголовки нельзя пихать !!!
			CP_XML_NODE(L"w:pPr")
			{
				CP_XML_STREAM() << get_section_context().dump_;
				get_section_context().dump_.clear();
			}
			finish_paragraph();
			start_paragraph();
		}

		if (!paragraph_style.str().empty() || !ParentId.empty())
		{		
			CP_XML_NODE(L"w:pPr")
			{
				if ( !get_table_context().in_table() && !in_drawing)
				{
					CP_XML_STREAM() << get_section_context().dump_;
					get_section_context().dump_.clear();
				}
				if (!ParentId.empty())
				{
					CP_XML_NODE(L"w:pStyle")
					{
						CP_XML_ATTR(L"w:val", ParentId);
					}
				}
				CP_XML_STREAM() << paragraph_style.str();
				docx_serialize_list_properties(CP_XML_STREAM());
				

				if ((run_style.tellp() > 0 && in_styles == false) || !get_text_tracked_context().dumpRPrInsDel_.empty())
				{
					CP_XML_NODE(L"w:rPr")
					{
						CP_XML_STREAM() << get_text_tracked_context().dumpRPrInsDel_;
						
						const std::wstring & test_str = run_style.str();
						CP_XML_STREAM() << test_str;
					}
					get_text_tracked_context().dumpRPrInsDel_.clear();
				}
				if (!get_text_tracked_context().dumpPPr_.empty())
				{
					CP_XML_STREAM() << get_text_tracked_context().dumpPPr_;
					get_text_tracked_context().dumpPPr_.clear();
				}
			}
		}
    }
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

void docx_conversion_context::push_text_properties(const odf_reader::style_text_properties * TextProperties)
{
    text_properties_stack_.push_back(TextProperties);
}

void docx_conversion_context::pop_text_properties()
{
    text_properties_stack_.pop_back();
}

odf_reader::style_text_properties_ptr docx_conversion_context::current_text_properties()
{
    odf_reader::style_text_properties_ptr cur = boost::make_shared<odf_reader::style_text_properties>();
    BOOST_FOREACH(const odf_reader::style_text_properties * prop, text_properties_stack_)
    {
        if (prop)
            cur->content().apply_from( prop->content() );
    }
    return cur;
}

void docx_conversion_context::set_page_break_after(bool val)
{
    page_break_after_ = val;
}

bool docx_conversion_context::get_page_break_after()
{
    return page_break_after_ ;
}
void docx_conversion_context::set_page_break(bool val)
{
    page_break_ = val;
}
bool docx_conversion_context::get_page_break()
{
    return page_break_;
}
void docx_conversion_context::set_page_break_before(bool val)
{
    page_break_before_ = val;
}

bool docx_conversion_context::get_page_break_before()
{
    return page_break_before_;
}


void docx_conversion_context::add_page_properties(const std::wstring & StyleName)
{
	section_context::_section & s = section_context_.get();
	
	s.page_properties_.push_back( StyleName);
}

std::wstring docx_conversion_context::get_page_properties()
{
	section_context::_section & s = section_context_.get();

	if (s.page_properties_.size() > 1)			return s.page_properties_[1];
	else if (s.page_properties_.size() == 1)	return s.page_properties_[0];
	else										return L"";
}
void docx_conversion_context::remove_page_properties()
{
	section_context::_section & s = section_context_.get();

	if (s.page_properties_.size() > 1)
	{
		//первая общая (если есть) для всего документа - оставляем ее
		s.page_properties_.erase(s.page_properties_.begin() + 1, s.page_properties_.begin() + 2);
	}
	else if (s.page_properties_.size() == 1)
	{
		s.page_properties_.clear();
	}
}

void docx_conversion_context::next_dump_page_properties(bool val)
{
	if (process_headers_footers_ && val) return;
		
	next_dump_page_properties_ = val;
}

bool docx_conversion_context::is_next_dump_page_properties()
{
    return next_dump_page_properties_;    
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

        odf_reader::list_style_container & lists = root()->odf_context().listStyleContainer();
        odf_reader::text_list_style * curStyle = lists.list_style_by_name(curStyleName);
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

void docx_conversion_context::docx_serialize_list_properties(std::wostream & strm)
{
    if (!list_style_stack_.empty())
    {
        if (first_element_list_item_)
        {
            const int id = root()->odf_context().listStyleContainer().id_by_name( current_list_style() );

			CP_XML_WRITER(strm)
			{
				CP_XML_NODE(L"w:numPr")
				{
					CP_XML_NODE(L"w:ilvl")
					{
						CP_XML_ATTR(L"w:val", (list_style_stack_.size() - 1));
					}
					CP_XML_NODE(L"w:numId")
					{
						CP_XML_ATTR(L"w:val", id );
					}
				}
			}
            first_element_list_item_ = false;
        }
    }
}

void docx_conversion_context::add_delayed_element(odf_reader::office_element * Elm)
{
   delayed_elements_.push_back(Elm);    
}

void docx_conversion_context::docx_convert_delayed()
{
	if (delayed_elements_.empty()) return;

	if(delayed_converting_)return; //зацикливание иначе
	if(get_drawing_context().get_current_level() > 0 )
		return; //вложенный frame

	delayed_converting_ = true;
    while(!delayed_elements_.empty())
    {
        odf_reader::office_element * elm = delayed_elements_.front();
        elm->docx_convert(*this);
		delayed_elements_.pop_front();
    }
	delayed_converting_=false;
}

void section_context::add_section(const std::wstring & SectionName, const std::wstring & Style, const std::wstring & PageProperties)
{
    _section newSec(SectionName, Style, PageProperties );
    sections_.push_back(newSec);
}

namespace 
{
    // обработка Header/Footer
    // конвертируем содержимое header/footer и сохраняем результат в виде строки
    void process_one_header_footer(docx_conversion_context & Context,
        const std::wstring & styleName,
        odf_reader::office_element * elm,
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
		Context.dump_hyperlinks(internal_rels, hyperlinks::document_place);

		Context.get_headers_footers().add(styleName, dbgStr, type, internal_rels);

		if (type == headers_footers::headerLeft || type == headers_footers::footerLeft)
		{
			Context.set_settings_property(odf_reader::_property(L"evenAndOddHeaders",true));
		}
   }

}
void docx_conversion_context::set_settings_property(const odf_reader::_property & prop)
{
	settings_properties_.push_back(prop);
}
std::vector<odf_reader::_property> & docx_conversion_context::get_settings_properties()
{
	return settings_properties_;
}
void docx_conversion_context::process_headers_footers()
{
    odf_reader::odf_document * doc = root();
    if (!doc)
        return;

    process_headers_footers_ = true;

    odf_reader::odf_read_context & context =  doc->odf_context();
    odf_reader::page_layout_container & pageLayouts = context.pageLayoutContainer();

    // проходим по всем page layout
    BOOST_FOREACH(const odf_reader::style_master_page* page, pageLayouts.master_pages())
    {
        const std::wstring & styleName = page->style_master_page_attlist_.style_name_.get_value_or( odf_types::style_ref(L"") ).style_name();
        const std::wstring masterPageNameLayout =context.pageLayoutContainer().page_layout_name_by_style(styleName);
        add_page_properties(masterPageNameLayout);
		
		process_one_header_footer(*this, styleName, page->style_header_.get(), headers_footers::header);
        process_one_header_footer(*this, styleName, page->style_footer_.get(), headers_footers::footer );
        process_one_header_footer(*this, styleName, page->style_header_first_.get(), headers_footers::headerFirst);
        process_one_header_footer(*this, styleName, page->style_footer_first_.get(), headers_footers::footerFirst );
        process_one_header_footer(*this, styleName, page->style_header_left_.get(), headers_footers::headerLeft );
        process_one_header_footer(*this, styleName, page->style_footer_left_.get(), headers_footers::footerLeft );

		if (!page->style_header_ && !page->style_footer_ && !page->style_header_first_ && !page->style_footer_first_
			&& !page->style_header_left_ && !page->style_footer_left_)
		{
			//отключенные колонтитулы
			rels rels_;
			get_headers_footers().add(styleName, L"", headers_footers::none, rels_);
		}
		
		remove_page_properties();
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
    instances_map & map = (type_ == odf_types::noteclass::Endnote) ? instances_endnotes_ : instances_footnotes_;
    instance_ptr inst = instance_ptr( new instance(Id, Content) );
    map[Id] = inst;
    return Id;
}

std::wstring notes_context::next_id()
{
    instances_map & map = (type_ == odf_types::noteclass::Endnote) ? instances_endnotes_ : instances_footnotes_;
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

void docx_conversion_context::add_note_reference ()
{
    if (process_note_ == footNote || process_note_ ==  endNote)
    {
        add_element_to_run(_T(""));
        output_stream() << ((process_note_ == footNote) ? L"<w:footnoteRef />" : L"<w:endnoteRef />");
        finish_run();
        process_note_ = (NoteType) (process_note_ + 1); //add ref set
    }
}

typedef std::map<std::wstring, text_tracked_context::_state>::iterator map_changes_iterator;

void docx_conversion_context::start_text_changes (std::wstring id)
{
	text_tracked_context::_state  &state_add = text_tracked_context_.get_tracked_change(id);
	if (state_add.id != id) return;
	
	map_current_changes_.insert(std::pair<std::wstring, text_tracked_context::_state> (id, state_add));

	if (in_paragraph_ && ( state_add.type == 1 || state_add.type == 2 ))
	{
		map_changes_iterator it = map_current_changes_.find(id);
		text_tracked_context::_state  &state = it->second;
		
		std::wstring format_change =	L" w:date=\""	+ state.date	+ L"\"" +
										L" w:author=\""	+ state.author	+ L"\"" ;

		finish_run();
		state.active = true;
		
		if (state.type	== 1)
		{

			output_stream() << L"<w:ins" << format_change << L" w:id=\"" << std::to_wstring(current_id_changes++) <<  L"\">";
		}
		
		if (state.type	== 2)
		{
			for (int i = 0 ; i < state.content.size(); i++)
			{
				output_stream() << L"<w:del" << format_change << L" w:id=\"" << std::to_wstring(current_id_changes++) <<  L"\">";

				output_stream() << state.content[i];

				output_stream() << L"</w:del>";
			}
			map_current_changes_.erase(it);
		}
	}
}

void docx_conversion_context::start_changes()
{
	if (map_current_changes_.empty()) return;
	if (process_comment_) return;

	text_tracked_context_.dumpPPr_.clear();
	text_tracked_context_.dumpRPr_.clear();
	text_tracked_context_.dumpRPrInsDel_.clear();
	text_tracked_context_.dumpTcPr_.clear();
	text_tracked_context_.dumpTblPr_.clear();

	for (map_changes_iterator it = map_current_changes_.begin(); it != map_current_changes_.end(); it++)
	{
		text_tracked_context::_state  &state = it->second;

		if (state.type == 0)	continue; //unknown change ... todooo
		if (state.active)		continue;

		std::wstring change_attr;
		change_attr += L" w:date=\""	+ state.date	+ L"\"";
		change_attr += L" w:author=\""	+ state.author	+ L"\"";
		change_attr += L" w:id=\""		+ std::to_wstring(current_id_changes++) + L"\"";

		if (state.type	== 1)
		{
			text_tracked_context_.dumpRPrInsDel_ = L"<w:ins" + change_attr + L"/>";
		}

		if (state.type	== 2)
		{
			text_tracked_context_.dumpRPrInsDel_ = L"<w:del" + change_attr + L"/>";
		}
		
		if (state.type	== 3)
		{
			odf_reader::style_instance * styleInst = root()->odf_context().styleContainer().style_by_name(state.style_name, odf_types::style_family::Paragraph, false);
			if (styleInst)
			{
				odf_reader::style_paragraph_properties	* props			= styleInst->content()->get_style_paragraph_properties();
				odf_reader::style_text_properties		* props_text	= styleInst->content()->get_style_text_properties();	
	
				text_tracked_context_.dumpPPr_ += L"<w:pPrChange" + change_attr;
				
				if (props)
				{
					props->docx_convert(*this);	
					text_tracked_context_.dumpPPr_ += get_styles_context().paragraph_attr().str();
				}
				text_tracked_context_.dumpPPr_ += L">";
				
				if (props)	text_tracked_context_.dumpPPr_ += get_styles_context().paragraph_nodes().str();
				if (props_text)
				{
					props_text->docx_convert(*this);

					text_tracked_context_.dumpPPr_ += L"<w:rPr>";
					text_tracked_context_.dumpPPr_ += get_styles_context().text_style().str();
					text_tracked_context_.dumpPPr_ += L"</w:rPr>";
				}
				text_tracked_context_.dumpPPr_ += L"</w:pPrChange>";
			}
			else if (styleInst = root()->odf_context().styleContainer().style_by_name(state.style_name, odf_types::style_family::Text, false))
			{
				text_tracked_context_.dumpRPr_ = L"<w:rPrChange" + change_attr + L">";
				odf_reader::style_text_properties * props = NULL;	
				props = styleInst->content()->get_style_text_properties();
				if (props)
				{
					props->docx_convert(*this);
					text_tracked_context_.dumpRPr_ += get_styles_context().text_style().str();
				}
				text_tracked_context_.dumpRPr_ += L"</w:rPrChange>";
			}
			else if (styleInst = root()->odf_context().styleContainer().style_by_name(state.style_name, odf_types::style_family::Table, false))
			{
				text_tracked_context_.dumpTblPr_ = L"<w:TblPrChange" + change_attr + L">";
				odf_reader::style_table_properties		* props			= styleInst->content()->get_style_table_properties();
				odf_reader::style_table_cell_properties * props_cell	= styleInst->content()->get_style_table_cell_properties();
				if (props)
				{
					props->docx_convert(*this);
					text_tracked_context_.dumpTblPr_ += get_styles_context().table_style().str();
				}
				text_tracked_context_.dumpTblPr_ += L"</w:TblPrChange>";
			}
			else if (styleInst = root()->odf_context().styleContainer().style_by_name(state.style_name, odf_types::style_family::TableCell, false))
			{
				text_tracked_context_.dumpTcPr_ = L"<w:TcPrChange" + change_attr + L">";
				odf_reader::style_table_cell_properties * props	= styleInst->content()->get_style_table_cell_properties();
				if (props)
				{
					props->docx_convert(*this);
					text_tracked_context_.dumpTcPr_ += get_styles_context().table_style().str();
				}
				text_tracked_context_.dumpTcPr_ += L"</w:TcPrChange>";
			}
		}
	}
}

void docx_conversion_context::end_changes()
{
	if (process_comment_) return;

	for (map_changes_iterator it = map_current_changes_.begin(); it != map_current_changes_.end(); it++)
	{
		text_tracked_context::_state  &state = it->second;

		if (state.type	== 0)	continue; //unknown change ... libra format change skip
		if (state.type	== 3)	continue;
		if (!state.active)		continue;

		if (state.type	== 1)	output_stream() << L"</w:ins>";
		if (state.type	== 2)	output_stream() << L"</w:del>";

		state.active = false;
	}

	text_tracked_context_.dumpTcPr_.clear();
	text_tracked_context_.dumpTblPr_.clear();
	text_tracked_context_.dumpRPrInsDel_.clear();
	text_tracked_context_.dumpPPr_.clear();
	text_tracked_context_.dumpRPr_.clear();
}
void docx_conversion_context::end_text_changes (std::wstring id)
{
	if (map_current_changes_.empty()) return;

	map_changes_iterator it = map_current_changes_.find(id);

	if (it == map_current_changes_.end()) return;
	
	text_tracked_context::_state  &state = it->second;
	
	if (state.active)
	{
		if (in_paragraph_)
			finish_run();
		
		if (state.type	== 1)	output_stream() << L"</w:ins>";
		if (state.type	== 2)	output_stream() << L"</w:del>";
	}

	map_current_changes_.erase(it);
}

}
}

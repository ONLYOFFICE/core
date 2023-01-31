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

#include <iostream>

#include <xml/utils.h>
#include <xml/simple_xml_writer.h>

#include "docx_conversion_context.h"

#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"
#include "../Format/text_content.h"
#include "../Format/calcs_styles.h"
#include "../Format/style_paragraph_properties.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_table_properties.h"
#include "../Format/style_graphic_properties.h"

#include "docx_package.h"
#include "xlsx_package.h"
#include "oox_rels.h"
#include "../Common/logging.h"

#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../OOXML/Base/Unit.h"

static int current_id_changes = 1;

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

	current_state_.id = id;
}
void text_tracked_context::end_change ()
{
	mapChanges_.insert( std::make_pair(current_state_.id, current_state_));

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
docx_conversion_context::docx_conversion_context(odf_reader::odf_document * _odf_document) : 
	last_dump_page_properties_	(true),
	next_dump_page_properties_	(false),
	page_break_					(false),
	page_break_after_			(false),
	page_break_before_			(false),
	in_automatic_style_			(false),
	in_header_					(false),
	in_drawing_content_			(false),
	in_table_content_			(false),
	text_tracked_context_		(*this),
	table_context_				(*this),
	output_document_			(NULL),
	current_process_note_		(noNote),
	new_list_style_number_		(0),
	current_margin_left_		(0),
	current_outline_level_		(-1),
	is_rtl_						(false),
	is_delete_text_				(false),
	delayed_converting_			(false),
	process_headers_footers_	(false),
        current_process_comment_	(false),
	odf_document_				(_odf_document),
	math_context_				(_odf_document->odf_context().fontContainer(), false)
{
	mediaitems_ = boost::make_shared<mediaitems>(odf_document_->get_folder());
	chart_drawing_handle_ =  boost::make_shared<xlsx_drawing_context_handle>(mediaitems_);
	
	streams_man_ = streams_man::create(temp_stream_);
}
docx_conversion_context::~docx_conversion_context()
{
}
void docx_conversion_context::set_output_document(package::docx_document * document)
{
	output_document_ = document;
}
void docx_conversion_context::set_font_directory(std::wstring pathFonts)
{
	mediaitems_->set_font_directory(pathFonts);
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
        const std::wstring id = std::wstring(L"style") + std::to_wstring(count_++);
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
	if (false == current_process_comment_)
	{
		for (size_t i = 0; i < get_comments_context().ref_start_.size(); i++)
		{
			output_stream() << L"<w:commentRangeStart w:id=\"" << get_comments_context().ref_start_[i] << L"\"/>";
		}
		get_comments_context().ref_start_.clear();

		if (false == get_comments_context().ref_end_.empty())
		{
			for (size_t i = 0; i < get_comments_context().ref_end_.size(); i++)
			{
				output_stream()<< L"<w:commentRangeEnd w:id=\"" << get_comments_context().ref_end_[i] << L"\"/>";
			}
			
			for (size_t i = 0; i < get_comments_context().ref_end_.size(); i++)
			{
				output_stream()<< L"<w:commentReference w:id=\"" << get_comments_context().ref_end_[i] << L"\"/>";			
			}
		
			get_comments_context().ref_end_.clear();		
		}
	}
	if (!state_.in_run_)
    {
        state_.in_run_ = true;
		output_stream() << L"<w:r>";

		start_changes();

		if (!state_.text_properties_stack_.empty() || parenStyleId.length() > 0)
		{
			if (!state_.text_properties_stack_.empty())
			{
				odf_reader::style_text_properties_ptr textProp = this->current_text_properties();
				get_styles_context().start();


				if(( textProp) && (textProp->content_.r_style_)) parenStyleId = _T("");
				textProp->content_.docx_convert(*this);
			}
	        
			get_styles_context().docx_serialize_text_style( output_stream(), parenStyleId, text_tracked_context_.dumpRPr_);
		}

	}
}

void docx_conversion_context::start_paragraph(bool is_header)
{
	if (state_.in_paragraph_)
		finish_paragraph();

	output_stream() << L"<w:p>";

	in_header_		= is_header;
    is_rtl_			= false; 
	
	state_.in_paragraph_ = true;
	start_changes();
}

void docx_conversion_context::finish_paragraph()
{
	if (state_.in_paragraph_)
	{
		end_changes();

		if (false == current_process_comment_ && false == get_comments_context().ref_end_.empty())
		{
				for (size_t i = 0; i < get_comments_context().ref_end_.size(); i++)
				{
					output_stream()<< L"<w:commentRangeEnd w:id=\"" << get_comments_context().ref_end_[i] << L"\"/>";
				}
				
				for (size_t i = 0; i < get_comments_context().ref_end_.size(); i++)
				{
					output_stream()<< L"<w:commentReference w:id=\"" << get_comments_context().ref_end_[i] << L"\"/>";			
				}
			
				get_comments_context().ref_end_.clear();	
		}	
		output_stream() << L"</w:p>";
	}
	
	in_header_					= false;
	state_.is_paragraph_keep_	= false;
	state_.in_paragraph_		= false;
}


void docx_conversion_context::finish_run()
{
    if (false == state_.in_run_) return;

	if (false == current_process_comment_)
	{
		for (size_t i = 0; i < get_comments_context().ref_.size(); i++)
		{
			output_stream()<< L"<w:commentReference w:id=\"" << get_comments_context().ref_[i] << L"\"/>";			
		}
		get_comments_context().ref_.clear();
	}
	
	output_stream() << L"</w:r>";
    state_.in_run_ = false;
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
void docx_conversion_context::start_sdt(int type)
{
	in_table_content_ = true;

	table_content_context_.type_table_content = type;

	if (table_content_context_.type_table_content < 4)
	{
		output_stream() << L"<w:sdt><w:sdtPr>";
		if (table_content_context_.type_table_content == 3)
		{
			output_stream() << L"<w:bibliography/>";
		}
		else
		{
			output_stream() << L"<w:docPartObj><w:docPartGallery w:val=\"";
			
			if (table_content_context_.type_table_content == 1) output_stream() << L"Table of Contents";
			if (table_content_context_.type_table_content == 2) output_stream() << L"List od Illustrations";

			output_stream() << L"\"/><w:docPartUnique/></w:docPartObj>";

		}
		output_stream() << L"</w:sdtPr><w:sdtContent>";
	}
}

void docx_conversion_context::start_index_content()
{
	if (!in_table_content_) return;

	start_paragraph(false);

	std::wstring sInstrText;

	if (table_content_context_.type_table_content == 3)
	{
		sInstrText = L" BIBLIOGRAPHY "; 
	}
	else if (table_content_context_.type_table_content == 5)
	{
		sInstrText = L" INDEX"; 
		if (table_content_context_.bSeparators)
			sInstrText += L" \\h \"A\""; 
	}
	else
	{
		sInstrText += L" TOC";
	
		bool bLink = false, bPages = false;

		if (table_content_context_.current_template.empty())
		{
			bLink = true;
			bPages = true;
		}
		else
		{
			for (size_t i = 0; i < table_content_context_.current_template.size(); i++)
			{
				for (size_t j = 0; j < table_content_context_.current_template[i].content.size(); j++)
				{
					if (table_content_context_.current_template[i].content[j] == 3) bLink = true;
					if (table_content_context_.current_template[i].content[j] == 6) bPages = true;
				}
			}
		}

		if (bLink)
			sInstrText += L" \\h";

		if (table_content_context_.type_table_content == 1)
			sInstrText += L" \\f \\u";
		else 
			sInstrText += L" \\z";

		if (table_content_context_.min_outline_level > 0)
		{
			if (table_content_context_.max_outline_level > 9)
				table_content_context_.max_outline_level = 9;

			sInstrText += L" \\o \"" +	std::to_wstring(table_content_context_.min_outline_level) + L"-" + 
										std::to_wstring(table_content_context_.max_outline_level) + L"\"";
			if (!bPages)
				sInstrText += L" \\n "+	std::to_wstring(table_content_context_.min_outline_level) + L"-" + 
										std::to_wstring(table_content_context_.max_outline_level);
		}
		else if (table_content_context_.type_table_content == 1)
			sInstrText += L" \\o";

		if (false == table_content_context_.outline_level_styles.empty())
		{
			sInstrText += L" \\t \"";

			for (std::map<int, std::wstring>::iterator it = table_content_context_.outline_level_styles.begin(); 
														it != table_content_context_.outline_level_styles.end(); ++it)
			{
				sInstrText += it->second + L";" + std::to_wstring(it->first) + L";";
			}

			sInstrText += L"\"";
		}

		if (!table_content_context_.caption_sequence_name.empty())
		{
			 sInstrText += L" \\c \"" + table_content_context_.caption_sequence_name + L"\""; 
		}
	}
	start_field(sInstrText, L"");
	
	finish_paragraph();
}
void docx_conversion_context::end_index_content()
{
	if (!in_table_content_) return;

	start_paragraph(false);
	end_field();
	
	finish_paragraph();
}
void docx_conversion_context::start_field(const std::wstring & sInstrText, const std::wstring & sName)
{
	output_stream() << L"<w:r>";
	output_stream() << L"<w:fldChar w:fldCharType=\"begin\">";

	if (!sName.empty())
	{
		output_stream() << L"<w:ffData><w:name w:val=\"" << sName << L"\"/><w:enabled/><w:calcOnExit w:val=\"0\"/></w:ffData>";
	}
	output_stream() << L"</w:fldChar>";
	output_stream() << L"</w:r>";
	output_stream() << L"<w:r>";
	output_stream() << L"<w:instrText xml:space=\"preserve\">" << XmlUtils::EncodeXmlString(sInstrText) << L" </w:instrText>";
	output_stream() << L"</w:r>";
	output_stream() << L"<w:r>";

	output_stream() << L"<w:fldChar w:fldCharType=\"separate\"/>";
	output_stream() << L"</w:r>";

	if (!sName.empty())
	{
		start_bookmark(sName);
	}
	fields_names_stack_.push_back(sName);
}
void docx_conversion_context::end_field()
{
	if (fields_names_stack_.empty()) return;

	output_stream() << L"<w:r>";
	output_stream() << L"<w:fldChar w:fldCharType=\"end\"/>";
	output_stream() << L"</w:r>";	

	std::wstring sName = fields_names_stack_.back();
	fields_names_stack_.pop_back();

	if (!sName.empty())
	{
		end_bookmark(sName);
	}
}
void docx_conversion_context::end_sdt()
{
	if (!in_table_content_) return;

	if (table_content_context_.type_table_content < 4)
	{
		output_stream() << L"</w:sdtContent></w:sdt>";
	}
	in_table_content_ = false;
	table_content_context_.clear_all();
}
void docx_conversion_context::start_index_element()
{
	table_content_context_.clear_current_content_template_index();
}
void docx_conversion_context::end_index_element()
{
	table_content_context_.clear_current_content_template_index();
}
void docx_conversion_context::add_bibliography_item	(const std::wstring & item)
{
	arBibliography.push_back(item);
}
void docx_conversion_context::start_bookmark (const std::wstring &name)
{
	std::map<std::wstring, int>::iterator pFind = mapBookmarks.find(name);

	int id = -1;
	if (pFind == mapBookmarks.end())
	{
		id = mapBookmarks.size() + 1;
		mapBookmarks.insert(std::make_pair(name, id));
	}
	else
	{
		id = pFind->second;
	}

	finish_run();
	output_stream() << L"<w:bookmarkStart w:id=\"" << std::to_wstring(id) << L"\" w:name=\"" << XmlUtils::EncodeXmlString(name) << L"\"/>";
}

void docx_conversion_context::end_bookmark (const std::wstring &name)
{
	std::map<std::wstring, int>::iterator pFind = mapBookmarks.find(name);

	int id = -1;
	if (pFind == mapBookmarks.end())
	{
		return; //???
	}
	else
	{
		id = pFind->second;
	}
	finish_run();
	output_stream() << L"<w:bookmarkEnd w:id=\"" << std::to_wstring(id) << L"\"/>";
}

void docx_conversion_context::start_alphabetical_index (const std::wstring &id)
{
	std::map<std::wstring, std::vector<odf_reader::office_element_ptr>>::iterator pFind = mapAlphabeticals.find(id);

	if (pFind != mapAlphabeticals.end()) return;

	std::vector<odf_reader::office_element_ptr> texts;
	mapAlphabeticals.insert(std::make_pair(id, texts));

	current_alphabetic_index_ = id;
}
void docx_conversion_context::add_alphabetical_index_text (odf_reader::office_element_ptr & elem)
{
	std::map<std::wstring, std::vector<odf_reader::office_element_ptr>>::iterator pFind = mapAlphabeticals.find(current_alphabetic_index_);
	if (pFind == mapAlphabeticals.end())
	{
		return; 
	}
	pFind->second.push_back(elem);
}
void docx_conversion_context::end_alphabetical_index (const std::wstring &id)
{
	std::map<std::wstring, std::vector<odf_reader::office_element_ptr>>::iterator pFind = mapAlphabeticals.find(id);

	if (pFind == mapAlphabeticals.end())
	{
		return; 
	}
	finish_run();

	output_stream() << L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>";
	output_stream() << L"<w:r><w:instrText>XE \"";
	
	ElementType type;
	for (size_t i = 0; i < pFind->second.size(); i++)
	{
		type = pFind->second[i]->get_type();
		pFind->second[i]->text_to_stream(output_stream());
	}

	output_stream() << L"\"</w:instrText></w:r>";

	output_stream() << L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>";
	
	for (size_t i = 0; i < pFind->second.size(); i++)
	{
		pFind->second[i]->docx_convert(*this);
	}
 	
	mapAlphabeticals.erase(pFind);
	
	if (mapAlphabeticals.empty())
		current_alphabetic_index_.clear();
	else
		current_alphabetic_index_ = mapAlphabeticals.begin()->first; // todooo vector+map+level
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
void docx_conversion_context::reset_context_state()
{
	keep_state_.push_back(state_);
	
	state_.in_paragraph_		= false;
	state_.in_run_				= false;
	state_.is_paragraph_keep_	= false;
	
	state_.drawing_text_props_.clear();	
	state_.text_properties_stack_.clear();
	
	get_styles_context().text_style_ext().clear();
}
void docx_conversion_context::back_context_state()
{
	state_ = keep_state_.back();
	keep_state_.pop_back();

	get_styles_context().text_style_ext().clear();
}

void docx_conversion_context::add_new_run(std::wstring parentStyleId)
{
	finish_run();

    add_element_to_run(parentStyleId);
}

std::wstring docx_conversion_context::add_hyperlink(const std::wstring & href, bool draw)
{
	oox::_rels_type_place type_place = get_type_place();
		
	std::wstring href_correct = XmlUtils::EncodeXmlString(href);
    XmlUtils::replace_all( href_correct, L" .", L".");//1 (130).odt
	
	return hyperlinks_.add(href_correct, type_place, draw);
}
hyperlinks::_ref  docx_conversion_context::last_hyperlink()
{
	return hyperlinks_.last();
}
_rels_type_place docx_conversion_context::get_type_place()
{
	if (current_process_comment_)					return oox::comment_place;
	if (current_process_note_ == footNote || 
		current_process_note_ == footNoteRefSet)	return oox::footnote_place;
	if (current_process_note_ == endNote ||
		current_process_note_ == endNoteRefSet )	return oox::endnote_place;
	
	if (process_headers_footers_)					return oox::header_footer_place;

	return oox::document_place;
}
void docx_conversion_context::dump_hyperlinks(rels & Rels, _rels_type_place type)
{
    hyperlinks_.dump_rels(Rels, type);
}

void docx_conversion_context::dump_headers_footers(rels & Rels) const
{
    headers_footers_.dump_rels(Rels);
}

void docx_conversion_context::dump_notes(rels & Rels) const
{
    notes_context_.dump_rels(Rels);
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
	output_stream() << L"xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" ";
	output_stream() << L"xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" ";
	output_stream() << L"xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" ";
	output_stream() << L"xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" ";
	output_stream() << L"xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" ";
	output_stream() << L"mc:Ignorable=\"w14 w15 w16se wne wp14\">";


	//apply page-default prop
	//пока временно сюда воткнем обработку свойств документа в целом
}


void docx_conversion_context::end_document()
{
    output_stream() << L"</w:document>";

	output_document_->get_word_files().set_document	( package::simple_element::create(L"document.xml", document_xml_.str()) );
	output_document_->get_word_files().set_settings	( package::simple_element::create(L"settings.xml", dump_settings_document()));
	output_document_->get_word_files().set_media	( mediaitems_);
	output_document_->get_word_files().set_comments	( comments_context_);
	output_document_->get_word_files().set_headers_footers( headers_footers_);
 
	package::xl_drawings_ptr drawings = package::xl_drawings::create(chart_drawing_handle_->content(), chart_drawing_handle_->content_vml());
	output_document_->get_word_files().set_drawings(drawings);

	package::content_types_file	& content_file_ = output_document_->get_content_types_file();
	content_file_.set_media( mediaitems_);

////////////////////////////////////////////////////////////////////////////
	dump_bibliography();
	dump_hyperlinks (notes_context_.footnotesRels(), oox::footnote_place);
	dump_hyperlinks (notes_context_.endnotesRels(), oox::endnote_place);

	get_mediaitems()->dump_rels(notes_context_.footnotesRels(), oox::footnote_place);
 	get_mediaitems()->dump_rels(notes_context_.endnotesRels(), oox::endnote_place);
  
	output_document_->get_word_files().set_notes(notes_context_);

	output_document_->get_docProps_files().set_app(package::simple_element::create(L"app.xml", dump_settings_app()));
	output_document_->get_docProps_files().set_core(package::simple_element::create(L"core.xml", dump_settings_core()));

	for (size_t i = 0; i < charts_.size(); i++)
    {
		package::chart_content_ptr content = package::chart_content::create();

		charts_[i]->serialize(content->content());
		charts_[i]->dump_rels(content->get_rel_file()->get_rels());

		output_document_->get_word_files().add_charts(content);
	
	}    
	output_document_->get_word_files().update_rels(*this);
}
void docx_conversion_context::dump_bibliography()
{
	if (arBibliography.empty()) return;

	std::wstringstream output(L"");
    CP_XML_WRITER(output)
    {
        CP_XML_NODE(L"b:Sources")
        {
			CP_XML_ATTR(L"Version",	6);
			CP_XML_ATTR(L"StyleName", L"APA");
			CP_XML_ATTR(L"SelectedStyle", L"\\APASixthEditionOfficeOnline.xsl");
			CP_XML_ATTR(L"xmlns",	L"http://schemas.openxmlformats.org/officeDocument/2006/bibliography");
			CP_XML_ATTR(L"xmlns:b",	L"http://schemas.openxmlformats.org/officeDocument/2006/bibliography");

			for (size_t i = 0; i < arBibliography.size(); i++)
			{
				CP_XML_NODE(L"b:Source")
				{				
					CP_XML_STREAM() << arBibliography[i];
					CP_XML_NODE(L"b:RefOrder")
					{
						CP_XML_STREAM() << std::to_wstring(i + 1);
					}
				}
			}
		}
	}
	std::wstringstream output_props(L"");
    CP_XML_WRITER(output_props)
    {
        CP_XML_NODE(L"ds:datastoreItem")
        {
			CP_XML_ATTR(L"xmlns:ds", L"http://schemas.openxmlformats.org/officeDocument/2006/customXml");
			CP_XML_ATTR(L"ds:itemID", L"{28C8D49A-D84A-4837-A0AC-8E2C3AE46B82}");

			CP_XML_NODE(L"ds:schemaRefs")
			{
				CP_XML_NODE(L"ds:schemaRef")
				{
					CP_XML_ATTR(L"ds:uri", L"http://schemas.openxmlformats.org/officeDocument/2006/bibliography");
				}
			}
		}
	}

	package::customXml_content_ptr content = package::customXml_content::create(output.str(), output_props.str());
	int id = output_document_->add_customXml(content);

	const std::wstring sRId			= std::wstring(L"cstId") + std::to_wstring(id);
	const std::wstring sRel			= L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml";
	const std::wstring sFileRef		= std::wstring(L"../customXml/item") + std::to_wstring(id) + L".xml";

	output_document_->get_word_files().add_rels(relationship(sRId, sRel, sFileRef));
}
std::wstring  docx_conversion_context::dump_settings_app()
{
	std::wstringstream output;
	CP_XML_WRITER(output)
	{
		CP_XML_NODE(L"Properties")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties");
			CP_XML_ATTR(L"xmlns:vt", L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes");

			if (!odf_document_->odf_context().DocProps().template_.empty())
			{
				CP_XML_NODE(L"Template")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().template_;
				}
			}
			if (odf_document_->odf_context().DocProps().revision_)
			{
				CP_XML_NODE(L"TotalTime")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().revision_;
				}
			}
			CP_XML_NODE(L"Application")
			{
				if (!odf_document_->odf_context().DocProps().application_.empty())
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().application_;
				}
				else
				{
					std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
					if (sApplication.empty())
						sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
					CP_XML_STREAM() << sApplication;
#if defined(INTVER)
					std::string s = VALUE2STR(INTVER);
					CP_XML_STREAM() << L"/" << std::wstring(s.begin(), s.end());
#endif				
				}
			}
			if (odf_document_->odf_context().DocProps().page_count_)
			{
				CP_XML_NODE(L"Pages")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().page_count_;
				}
			}
			if (odf_document_->odf_context().DocProps().word_count_)
			{
				CP_XML_NODE(L"Words")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().word_count_;
				}
			}
			if (odf_document_->odf_context().DocProps().character_count_)
			{
				CP_XML_NODE(L"CharactersWithSpaces")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().character_count_;
				}
			}
			if (odf_document_->odf_context().DocProps().non_whitespace_character_count_)
			{
				CP_XML_NODE(L"Characters")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().non_whitespace_character_count_;
				}
			}
			if (odf_document_->odf_context().DocProps().paragraph_count_)
			{
				CP_XML_NODE(L"Paragraphs")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().paragraph_count_;
				}
			}
		}
	}
	return output.str();
}
std::wstring  docx_conversion_context::dump_settings_core()
{
	std::wstringstream output;
	
	CP_XML_WRITER(output)
	{
		CP_XML_NODE(L"cp:coreProperties")
		{
			CP_XML_ATTR(L"xmlns:cp", L"http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
			CP_XML_ATTR(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");
			CP_XML_ATTR(L"xmlns:dc", L"http://purl.org/dc/elements/1.1/");
			CP_XML_ATTR(L"xmlns:dcmitype", L"http://purl.org/dc/dcmitype/");
			CP_XML_ATTR(L"xmlns:dcterms", L"http://purl.org/dc/terms/");
			
			if (!odf_document_->odf_context().DocProps().creation_date_.empty())
			{
				CP_XML_NODE(L"dcterms:created")
				{
					CP_XML_ATTR(L"xsi:type", L"dcterms:W3CDTF");
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().creation_date_;
				}
			}
			if (!odf_document_->odf_context().DocProps().dc_date_.empty())
			{
				CP_XML_NODE(L"dcterms:modified")
				{
					CP_XML_ATTR(L"xsi:type", L"dcterms:W3CDTF");
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().dc_date_;
				}
			}
			if (!odf_document_->odf_context().DocProps().dc_creator_.empty())
			{
				CP_XML_NODE(L"dc:creator")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().dc_creator_;
				}
			}
			if (!odf_document_->odf_context().DocProps().dc_title_.empty())
			{
				CP_XML_NODE(L"dc:title")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().dc_title_;
				}
			}
			if (!odf_document_->odf_context().DocProps().dc_subject_.empty())
			{
				CP_XML_NODE(L"dc:subject")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().dc_subject_;
				}
			}
			if (!odf_document_->odf_context().DocProps().dc_description_.empty())
			{
				CP_XML_NODE(L"dc:description")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().dc_description_;
				}
			}
			if (!odf_document_->odf_context().DocProps().dc_language_.empty())
			{
				CP_XML_NODE(L"dc:language")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().dc_language_;
				}
			}
			CP_XML_NODE(L"cp:lastModifiedBy")
			{
				std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
				if (sApplication.empty())
					sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
				CP_XML_STREAM() << sApplication;
#if defined(INTVER)
				std::string s = VALUE2STR(INTVER);
				CP_XML_STREAM() << L"/" << std::wstring(s.begin(), s.end());
#endif	
			}
			if (!odf_document_->odf_context().DocProps().keyword_.empty())
			{
				CP_XML_NODE(L"cp:keywords")
				{
					CP_XML_STREAM() << odf_document_->odf_context().DocProps().keyword_;
				}
			}
			if (odf_document_->odf_context().DocProps().revision_)
			{
				CP_XML_NODE(L"cp:revision")
				{
					CP_XML_STREAM() << *odf_document_->odf_context().DocProps().revision_;
				}
			}
		}
	}
	return output.str();
}
std::wstring  docx_conversion_context::dump_settings_document()
{
	std::wstringstream output;
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

			if (odf_reader::GetProperty(settings_properties_,L"evenAndOddHeaders", boolVal))
			{
				CP_XML_NODE(L"w:evenAndOddHeaders");
			}
			if (odf_reader::GetProperty(settings_properties_,L"displayBackgroundShape", boolVal))
			{
				CP_XML_NODE(L"w:displayBackgroundShape");
			}
			if (odf_reader::GetProperty(settings_properties_,L"zoom", intVal))
			{
				CP_XML_NODE(L"w:zoom")
				{
					CP_XML_ATTR(L"w:percent",intVal.get());
				}
			}
			if (odf_reader::GetProperty(settings_properties_,L"mirrorMargins", boolVal))
			{
				CP_XML_NODE(L"w:mirrorMargins");
			}
			
			CP_XML_NODE(L"w:compat")
			{
				CP_XML_NODE(L"w:compatSetting")
				{
					CP_XML_ATTR(L"w:name",  L"compatibilityMode" );
					CP_XML_ATTR(L"w:uri",  L"http://schemas.microsoft.com/office/word" );
					CP_XML_ATTR(L"w:val",  L"15" );
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

    std::wstring StyleDisplayName(const std::wstring & Name, const std::wstring & DisplayName, odf_types::style_family::type Type, bool &bDisplayed)
    {
		bDisplayed = true;
        if (false == DisplayName.empty())
            return DisplayName;

		if (false == Name.empty())
		{
			bDisplayed = false;
            return Name;
		}
        else
        {
			bDisplayed = false;
			return std::wstring(L"DStyle_") + boost::lexical_cast<std::wstring>(odf_types::style_family(Type) );
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

	odf_reader::list_style_container::instances_array & arListStyles = list_styles.instances();
	
	for (size_t i = 0; i < arListStyles.size(); i++)
	{
        odf_reader::office_element_ptr_array & content = arListStyles[i]->get_text_list_style()->content_;

		if (content.empty()) 
			continue;
       
		const int abstractNumId = list_styles.id_by_name(arListStyles[i]->get_style_name());
        numIds.push_back(abstractNumId);		
        
        strm << L"<w:abstractNum w:abstractNumId=\"" << abstractNumId << "\">";
       
        for (size_t j = 0; j < (std::min)( content.size(), (size_t)9); j++)
        {
            start_text_list_style(arListStyles[i]->get_text_list_style()->attr_.style_name_);
				content[j]->docx_convert(*this);
            end_text_list_style();        
        }

        strm << L"</w:abstractNum>";
	}
	if (list_styles.outline_style())
	{
		const int abstractNumId = list_styles.id_outline();
        numIds.push_back(abstractNumId);		
        
        strm << L"<w:abstractNum w:abstractNumId=\"" << abstractNumId << "\">";
       
        for (size_t j = 0; j < (std::min)( list_styles.outline_style()->content_.size(), (size_t)9); j++)
        {
            start_text_list_style(L"Outline");
				list_styles.outline_style()->content_[j]->docx_convert(*this);
            end_text_list_style();        
        }

        strm << L"</w:abstractNum>";
	}

	for (size_t i = 0; i < numIds.size(); i++)
    {
        strm << L"<w:num w:numId=\"" << numIds[i] << L"\">";
        strm << L"<w:abstractNumId w:val=\"" << numIds[i] << "\"/>";
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

		odf_reader::fonts_container::instances_array &arFonts = fonts.instances();
		for (size_t i = 0; i < arFonts.size(); i++)
        {
			if (!arFonts[i]) continue;
			if (arFonts[i]->name().empty()) continue;

            strm << L"<w:font w:name=\"" << arFonts[i]->name() << L"\">";

            if (!arFonts[i]->charset().empty())
                strm << L"<w:charset w:val=\"" << arFonts[i]->charset() <<"\"/>";

            if (!arFonts[i]->family().empty())
                strm << L"<w:family w:val=\"" << arFonts[i]->family() << "\"/>";

            if (!arFonts[i]->pitch().empty())
                strm << L"<w:pitch w:val=\"" << arFonts[i]->pitch() << "\"/>";

            if (!arFonts[i]->alt_name().empty())
                strm << L"<w:altName w:val=\"" << arFonts[i]->alt_name() << "\"/>";

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
	_Wostream << L"mc:Ignorable=\"w14\">";
	
    if (odf_reader::odf_document * doc = root())
    {
        odf_reader::odf_read_context & context =  doc->odf_context();
        odf_reader::styles_container & styles = context.styleContainer();

        // add all styles to the map
		odf_reader::styles_container::instances_array &arStyles = styles.instances();
		for (size_t i = 0; i < arStyles.size(); i++)
        {
			if (!arStyles[i]) continue;
            styles_map_.get(arStyles[i]->name(), arStyles[i]->type());
        }

        _Wostream << L"<w:docDefaults>";

        if (odf_reader::style_instance * defaultParStyle = styles.style_default_by_type(odf_types::style_family::Paragraph))
        {
            _Wostream << L"<w:pPrDefault>";
            if ( odf_reader::style_content * content = defaultParStyle->content())
            {
				if (content->get_style_paragraph_properties())
				{
					if(content->get_style_paragraph_properties()->content_.fo_background_color_)
					{
						odf_types::background_color color = *content->get_style_paragraph_properties()->content_.fo_background_color_;
						if ((color.get_type() != odf_types::background_color::Transparent && 
							color.get_color() == odf_types::color(L"ffffff")) ||
							(color.get_type() == odf_types::background_color::Transparent))
						{
							content->get_style_paragraph_properties()->content_.fo_background_color_ = boost::none;
						}
					}
				}
                get_styles_context().start_process_style(defaultParStyle);
                content->docx_convert(*this);
				get_styles_context().end_process_style();
			}
            _Wostream << L"</w:pPrDefault>";
        }
        if (odf_reader::style_instance * defaultParStyle = styles.style_default_by_type(odf_types::style_family::Text))
        {
            _Wostream << L"<w:rPrDefault>";
            if ( odf_reader::style_content * content = defaultParStyle->content())
            {
                get_styles_context().start_process_style(defaultParStyle);
                content->docx_convert(*this);
				get_styles_context().end_process_style();
			}
            _Wostream << L"</w:rPrDefault>";
        }
        _Wostream << L"</w:docDefaults>";

		std::wstring default_style;

		for (size_t i = 0; i < arStyles.size(); i++)
		{
            if (false == arStyles[i]->is_automatic() && 
					(arStyles[i]->type() == odf_types::style_family::Paragraph ||
					 arStyles[i]->type() == odf_types::style_family::Text))
            {
                const std::wstring id = styles_map_.get(arStyles[i]->name(), arStyles[i]->type());
				bool bDefault = (arStyles[i]->style_class() == L"default");
				bool bDisplayed = (arStyles[i]->type() == odf_types::style_family::Paragraph);
                
				_Wostream << L"<w:style w:styleId=\"" << id << L"\" w:type=\"" << StyleTypeOdf2Docx(arStyles[i]->type()) << L"\""; 

				if (bDefault)  // style
				{
					_Wostream << L" w:default=\"1\"";

					if (arStyles[i]->type() == odf_types::style_family::Paragraph)
					{
						default_style = id;
					}
				}
				else if (!arStyles[i]->is_default()) // default-style
				{
					_Wostream << L" w:customStyle=\"1\"";
				}
				_Wostream << L">";
                
				const std::wstring displayName = StyleDisplayName(arStyles[i]->name(), arStyles[i]->display_name(), arStyles[i]->type(), bDisplayed);

				_Wostream << L"<w:name w:val=\"" << XmlUtils::EncodeXmlString(displayName) << L"\"/>";

                if (odf_reader::style_instance * baseOn = arStyles[i]->parent())
                {
                    const std::wstring basedOnId = styles_map_.get(baseOn->name(), baseOn->type());
                    _Wostream << L"<w:basedOn w:val=\"" << basedOnId << "\"/>";
                }
				else if (!bDefault && !default_style.empty())
				{
					_Wostream << L"<w:basedOn w:val=\"" << default_style << "\"/>";
				}
     //           else if (false == bDefault && false == arStyles[i]->is_default() && styles_map_.check(L"", arStyles[i]->type()))
     //           {
					//bDisplayed = false;
     //               const std::wstring basedOnId = styles_map_.get(L"", arStyles[i]->type());
     //               _Wostream << L"<w:basedOn w:val=\"" << basedOnId << "\"/>";
     //           }
				if (bDisplayed)
				{
					_Wostream << L"<w:qFormat/>";
				}

				if (odf_reader::style_instance * next = arStyles[i]->next())
				{
				    const std::wstring nextId = styles_map_.get(next->name(), next->type());
				    _Wostream << L"<w:next w:val=\"" << nextId << "\"/>";
				}
                //else if (arStyles[i]->is_default())
                //{
                //    // self
                //    _Wostream << L"<w:next w:val=\"" << id << "\"/>";
                //}

                if (odf_reader::style_content * content = arStyles[i]->content())
                {
					get_tabs_context().clear();
					calc_tab_stops(arStyles[i].get(), get_tabs_context());
					
					get_styles_context().start_process_style(arStyles[i].get());
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

void docx_conversion_context::process_section(std::wostream & strm, odf_reader::style_columns *columns_page)//from page layout
{
	if (root()->odf_context().pageLayoutContainer().linenumbering())
	{
		root()->odf_context().pageLayoutContainer().linenumbering()->docx_serialize(strm, *this);
	}	

	int count_columns = 1;
	bool sep_columns = false;

	oox::section_context::_section & section = get_section_context().get_last();

	odf_reader::style_columns *columns = columns_page;
	//if (!columns)
	{
		if (const odf_reader::style_instance * secStyle = root()->odf_context().styleContainer().style_by_name(section.style_, odf_types::style_family::Section, process_headers_footers_))
		{
			if (const odf_reader::style_content * content = secStyle->content())
			{
				if (odf_reader::style_section_properties * sectPr = content->get_style_section_properties())
				{
					columns = dynamic_cast<odf_reader::style_columns *>( sectPr->style_columns_.get());
					
					section.margin_left_	= sectPr->common_horizontal_margin_attlist_.fo_margin_left_;
					section.margin_right_	= sectPr->common_horizontal_margin_attlist_.fo_margin_right_;		
				}
			}
			if (section.is_dump_)
			{
				get_section_context().remove_section();				
			}
		}
	}
	
	std::vector<std::pair<double, double>> width_space;
	if (columns)
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

		if (!columns->style_columns_.empty())
		{
			double page_width = 0;
			const odf_reader::page_layout_instance * pp = root()->odf_context().pageLayoutContainer().page_layout_first();
			
			if ((pp) && (pp->properties()))
			{
				odf_reader::style_page_layout_properties_attlist & attr_page = pp->properties()->attlist_;
				if (attr_page.fo_page_width_)
				{
					page_width = attr_page.fo_page_width_->get_value_unit(odf_types::length::pt);
				}
				if (attr_page.common_horizontal_margin_attlist_.fo_margin_left_)
				{
					page_width -= attr_page.common_horizontal_margin_attlist_.fo_margin_left_->get_length().get_value_unit(odf_types::length::pt);
				}
				if (attr_page.common_horizontal_margin_attlist_.fo_margin_right_)
				{
					page_width -= attr_page.common_horizontal_margin_attlist_.fo_margin_right_->get_length().get_value_unit(odf_types::length::pt);
				}
			}		
			for (size_t i = 0; page_width > 0, i < columns->style_columns_.size(); i++)
			{
				odf_reader::style_column *col = dynamic_cast<odf_reader::style_column*>( columns->style_columns_[i].get());
				if (!col) continue;

				double percent = col->style_rel_width_ ? col->style_rel_width_->get_value() : 0;
				if (percent > 0x7FFE)
				{
					//auto
					break;
				}
				else
				{
					if (percent > 1000.) percent /= 100.;

					double width = page_width * percent / 100.;

					double space_end = col->fo_end_indent_ ? col->fo_end_indent_->get_value_unit(odf_types::length::pt) : 0;
					double space_start = col->fo_start_indent_ ? col->fo_start_indent_->get_value_unit(odf_types::length::pt) : 0;

					width -= space_end;
					width -= space_start;

					if (i < columns->style_columns_.size() - 1)
					{
						col = dynamic_cast<odf_reader::style_column*>(columns->style_columns_[i + 1].get());
						space_start = col->fo_start_indent_ ? col->fo_start_indent_->get_value_unit(odf_types::length::pt) : 0;
					}

					width_space.push_back(std::make_pair(width, space_start + space_end));
				}
			}
		}
	}

	CP_XML_WRITER(strm)
	{			
		CP_XML_NODE(L"w:cols")
		{
			CP_XML_ATTR(L"w:equalWidth", width_space.empty());
			CP_XML_ATTR(L"w:num", count_columns);
			CP_XML_ATTR(L"w:sep", sep_columns);
			CP_XML_ATTR(L"w:space", 708);

			for (size_t i = 0; i < width_space.size(); i++)
			{
				CP_XML_NODE(L"w:col")
				{
					CP_XML_ATTR(L"w:w", (int)(width_space[i].first * 20));	
					CP_XML_ATTR(L"w:space", (int)(width_space[i].second * 20));	
				}
			}
		}
	}
}
bool docx_conversion_context::process_page_properties(std::wostream & strm)
{
    if (is_next_dump_page_properties() || get_section_context().get_last().is_dump_)
    {
        std::wstring pageProperties = get_page_properties();
		odf_reader::page_layout_instance * page_layout_instance_ = root()->odf_context().pageLayoutContainer().page_layout_by_name(pageProperties);

		if (page_layout_instance_) 
		{
			page_layout_instance_->docx_serialize(strm, *this);
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
   serialize_paragraph_style(output_stream(), automatic_parent_style_, in_styles);
    
    if (automatic_parent_style_.empty())
        styles_context_.docx_serialize_text_style( output_stream(), L"", text_tracked_context_.dumpRPr_);
}

void docx_conversion_context::serialize_paragraph_style(std::wostream & strm, const std::wstring & ParentId, bool in_styles)
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
		////Tutor_Charlotte_Tutor_the_Entire_World_.odt
		//if (get_section_context().dump_.empty() == false && (!ParentId.empty() || get_section_context().get().is_dump_ || in_header_) 
		//	 && !get_table_context().in_table() && !in_drawing)
		//{//две подряд секции или если стиль определен и в заголовки нельзя пихать !!!
		//	CP_XML_NODE(L"w:pPr")
		//	{
		//		CP_XML_STREAM() << get_section_context().dump_;
		//		get_section_context().dump_.clear();
		//	}
		//	finish_paragraph();
		//	start_paragraph();
		//}

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
				serialize_list_properties(CP_XML_STREAM());
				

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
    automatic_parent_style_.clear();
	tabs_context_.clear();
	current_outline_level_ = -1;
}

bool docx_conversion_context::in_automatic_style()
{
    return in_automatic_style_;
}

void docx_conversion_context::push_text_properties(const odf_reader::style_text_properties * TextProperties)
{
    state_.text_properties_stack_.push_back(TextProperties);
}

void docx_conversion_context::pop_text_properties()
{
    state_.text_properties_stack_.pop_back();
}

odf_reader::style_text_properties_ptr docx_conversion_context::current_text_properties()
{
    odf_reader::style_text_properties_ptr cur = boost::make_shared<odf_reader::style_text_properties>();

	for (size_t i = 0; i < state_.text_properties_stack_.size(); i++)
    {
        if (state_.text_properties_stack_[i])
            cur->content_.apply_from( state_.text_properties_stack_[i]->content_);
    }
    return cur;
}

void docx_conversion_context::set_page_break_after(int val)
{
    page_break_after_ = val;
}

int docx_conversion_context::get_page_break_after()
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
void docx_conversion_context::set_page_break_before(int val)
{
    page_break_before_ = val;
}

int docx_conversion_context::get_page_break_before()
{
    return page_break_before_;
}


void docx_conversion_context::add_page_properties(const std::wstring & StyleName)
{
	section_context::_section & s = section_context_.get_last();
	
	s.page_properties_.push_back( StyleName);
}

std::wstring docx_conversion_context::get_page_properties()
{
	section_context::_section & s = section_context_.get_last();

	if (s.page_properties_.size() > 1)			return s.page_properties_[1];
	else if (s.page_properties_.size() == 1)	return s.page_properties_[0];
	else										return L"";
}
void docx_conversion_context::remove_page_properties()
{
	section_context::_section & s = section_context_.get_last();

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
	if (val)
		last_dump_page_properties(true);
}

bool docx_conversion_context::is_next_dump_page_properties()
{
    return next_dump_page_properties_;    
}
void docx_conversion_context::last_dump_page_properties(bool val)
{
	last_dump_page_properties_ = val;
}
bool docx_conversion_context::is_last_dump_page_properties()
{
    return last_dump_page_properties_;    
}
void docx_conversion_context::start_text_list_style(const std::wstring & StyleName)
{
    text_list_style_name_ = StyleName;    
}

void docx_conversion_context::end_text_list_style()
{
    text_list_style_name_ = L"";
}

std::wstring docx_conversion_context::get_text_list_style_name()
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

std::wstring docx_conversion_context::current_list_style()
{
    if (false == list_style_stack_.empty())
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
        const std::wstring newStyleName = curStyleName + std::to_wstring(new_list_style_number_++);
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
int docx_conversion_context::process_text_attr(odf_reader::text::paragraph_attrs *Attr)
{
	if ( Attr->text_style_name_.empty() ) return 0;

	odf_reader::style_instance * styleInst =
				root()->odf_context().styleContainer().style_by_name(Attr->text_style_name_, odf_types::style_family::Paragraph, process_headers_footers_);

	if (!styleInst) return 0;

	if (false == styleInst->is_automatic()) return 0;

    odf_reader::style_content *styleContent = styleInst->content();
    
	if (!styleContent) return 0;

	push_text_properties(styleContent->get_style_text_properties());
	return 1;
}
int docx_conversion_context::process_paragraph_style(_CP_OPT(std::wstring) style_name_ptr)
{
	if (!style_name_ptr) return 0;
	if (style_name_ptr->empty()) return 0;

	std::wstring & style_name = *style_name_ptr;

	if (odf_reader::style_instance * styleInst =
			root()->odf_context().styleContainer().style_by_name(style_name, odf_types::style_family::Paragraph, process_headers_footers_))
    {
		double font_size = odf_reader::text_format_properties_content::process_font_size_impl(odf_types::font_size(odf_types::percent(100.0)), styleInst);
		if (font_size > 0) current_fontSize.push_back(font_size);
		
		process_page_break_after(styleInst);

		if (styleInst->is_automatic())
        {
            if (odf_reader::style_content * styleContent = styleInst->content())
            {
                std::wstring id;
				//office_element_ptr parent_tab_stops_;
                if (const odf_reader::style_instance * parentStyleContent = styleInst->parent())
				{
					std::wstring parent_name = parentStyleContent->name();
                    id = styles_map_.get( parent_name, parentStyleContent->type() );

					if (in_table_content_ && table_content_context_.empty_current_table_content_level_index())
					{
						table_content_context_.set_current_level(parent_name);
					}
				}

                start_automatic_style(id);

				calc_tab_stops(styleInst, get_tabs_context());
				
				//вытаскивает rtl c цепочки стилей !! - просто прописать в наследуемом НЕЛЬЗЯ !!
				odf_reader::paragraph_format_properties properties = odf_reader::calc_paragraph_properties_content(styleInst);
				if (properties.style_writing_mode_)
				{
					odf_types::writing_mode::type type = properties.style_writing_mode_->get_type();
					switch(type)
					{
					case odf_types::writing_mode::RlTb:
					case odf_types::writing_mode::TbRl:
					case odf_types::writing_mode::Rl:
						set_rtl(true);
						break;
					default:
						set_rtl(false);
					}
				}

				styleContent->docx_convert(*this);                
               
				end_automatic_style();

                //push_text_properties(styleContent->get_style_text_properties());
				return 1;
            }            
        }
        else
        {
            const std::wstring id = styles_map_.get( styleInst->name(), styleInst->type() );
            output_stream() << L"<w:pPr>";

			output_stream() << L"<w:pStyle w:val=\"" << id << L"\" />";

			if (!get_text_tracked_context().dumpPPr_.empty())
			{
				output_stream() << get_text_tracked_context().dumpPPr_;
				get_text_tracked_context().dumpPPr_.clear();
			}

			serialize_list_properties(output_stream());
			
			if (!get_text_tracked_context().dumpRPrInsDel_.empty())
			{
				output_stream() << L"<w:rPr>";
					output_stream() << get_text_tracked_context().dumpRPrInsDel_;
					get_text_tracked_context().dumpRPrInsDel_.clear();
				output_stream() << L"</w:rPr>";
			}
            output_stream() << L"</w:pPr>";
			return 2;
		}
	}
	return 0;
}
int docx_conversion_context::process_paragraph_attr(odf_reader::text::paragraph_attrs *Attr)
{
	if (!Attr) return 0;

	bool in_drawing	= false;

 	if (get_drawing_context().get_current_shape() || get_drawing_context().get_current_frame())
	{
		in_drawing = true;
	}
	
	if (false == Attr->text_style_name_.empty())
    {
		if (in_table_content_ && Attr->text_style_name_.empty())
		{
			table_content_context_.set_current_level(Attr->text_style_name_);
		}
		if (odf_reader::style_instance * styleInst =
				root()->odf_context().styleContainer().style_by_name(Attr->text_style_name_, odf_types::style_family::Paragraph, process_headers_footers_)
            )
		{
			double font_size = odf_reader::text_format_properties_content::process_font_size_impl(odf_types::font_size(odf_types::percent(100.0)), styleInst);
			if (font_size > 0) current_fontSize.push_back(font_size);
			
			_CP_OPT(int) outline_level = calc_outline_level(Attr->outline_level_, styleInst);
			
			process_page_break_after(styleInst);

			if (styleInst->is_automatic())
			{
				if (odf_reader::style_content * styleContent = styleInst->content())
				{
					std::wstring id;
					//office_element_ptr parent_tab_stops_;
					if (const odf_reader::style_instance * parentStyleContent = styleInst->parent())
					{
						std::wstring parent_name = parentStyleContent->name();
						id = styles_map_.get( parent_name, parentStyleContent->type() );

						if (in_table_content_ && table_content_context_.empty_current_table_content_level_index())
						{
							table_content_context_.set_current_level(parent_name);
						}
					}

					start_automatic_style(id);

					calc_tab_stops(styleInst, get_tabs_context());
					
					//вытаскивает rtl c цепочки стилей !! - просто прописать в наследуемом НЕЛЬЗЯ !!
					odf_reader::paragraph_format_properties properties = odf_reader::calc_paragraph_properties_content(styleInst);
					if (properties.style_writing_mode_)
					{
						odf_types::writing_mode::type type = properties.style_writing_mode_->get_type();
						switch(type)
						{
						case odf_types::writing_mode::RlTb:
						case odf_types::writing_mode::TbRl:
						case odf_types::writing_mode::Rl:
							set_rtl(true);
							break;
						default:
							set_rtl(false);
						}
					}
					get_styles_context().start_process_style(styleInst);

					if (outline_level)
					{
						set_outline_level(*outline_level);
					}
					styleContent->docx_convert(*this);  
					get_styles_context().end_process_style();
	               
					end_automatic_style();

					push_text_properties(styleContent->get_style_text_properties());

					if (!get_section_context().dump_.empty()
						&& !get_table_context().in_table()
						&& (get_process_note() == oox::docx_conversion_context::noNote)
						&& !in_drawing)
					{
						output_stream() << L"<w:pPr>";
						if (is_paragraph_header() )
						{
							output_stream() << get_section_context().dump_;
							get_section_context().dump_.clear();

							output_stream() << L"</w:pPr>";
							finish_paragraph();
							start_paragraph();
							//process_paragraph_style(Context.get_current_paragraph_style()); ??

							//if ((Attr->outline_level_) && (*Attr->outline_level_ > 0))
							if ((outline_level) && (*outline_level < 10))
							{
								output_stream() << L"<w:pPr>";
									output_stream() << L"<w:outlineLvl w:val=\"" << *outline_level << L"\"/>";
								output_stream() << L"</w:pPr>";
							}					
						}
						else
						{
							output_stream() << get_section_context().dump_;
							get_section_context().dump_.clear();
							//if ((Attr->outline_level_) && (*Attr->outline_level_ > 0))
							if ((outline_level) && (*outline_level < 10))
							{
								output_stream() << L"<w:outlineLvl w:val=\"" << *outline_level << L"\"/>";
							}
							output_stream() << L"</w:pPr>";
						}
					}
					return 1;
				}            
			}
			else
			{
				const std::wstring id = styles_map_.get( styleInst->name(), styleInst->type() );
				output_stream() << L"<w:pPr>";
	//todooo причесать			
				if (!get_section_context().dump_.empty()
					&& !get_table_context().in_table()
					&& (get_process_note() == oox::docx_conversion_context::noNote)
					&& !in_drawing)
				{
					if (is_paragraph_header() )
					{
						output_stream() << get_section_context().dump_;
						get_section_context().dump_.clear();

						output_stream() << L"</w:pPr>";
						finish_paragraph();
						start_paragraph();					
						output_stream() << L"<w:pPr>";
					}
					else
					{
						output_stream() << get_section_context().dump_;
						get_section_context().dump_.clear();
					}
				}

				output_stream() << L"<w:pStyle w:val=\"" << id << L"\" />";

				if (!get_text_tracked_context().dumpPPr_.empty())
				{
					output_stream() << get_text_tracked_context().dumpPPr_;
					get_text_tracked_context().dumpPPr_.clear();
				}

				serialize_list_properties(output_stream());
				
				//if ((Attr->outline_level_) && (*Attr->outline_level_ > 0))
				if (outline_level)
				{
					odf_reader::list_style_container & list_styles = root()->odf_context().listStyleContainer();
					
					if (list_style_stack_.empty() && list_styles.outline_style())
					{
						output_stream() << L"<w:numPr>";
							output_stream() << L"<w:ilvl w:val=\"" << *outline_level - 1  << L"\"/>";
							output_stream() << L"<w:numId w:val=\"" << list_styles.id_outline() << L"\"/>";
						output_stream() << L"</w:numPr>";
					}				   
					output_stream() << L"<w:outlineLvl w:val=\"" << *outline_level << L"\"/>";
				}

				if (!get_text_tracked_context().dumpRPrInsDel_.empty())
				{
					output_stream() << L"<w:rPr>";
						output_stream() << get_text_tracked_context().dumpRPrInsDel_;
						get_text_tracked_context().dumpRPrInsDel_.clear();
					output_stream() << L"</w:rPr>";
				}
				output_stream() << L"</w:pPr>";
				return 2;
			}
		}
	}
	if (!get_section_context().dump_.empty() 
		&& !get_table_context().in_table()
		&& (get_process_note() == oox::docx_conversion_context::noNote)
		&& !in_drawing)
	{
        output_stream() << L"<w:pPr>";
			output_stream() << get_section_context().dump_;
			get_section_context().dump_.clear();
			//todooo выяснить реальны ли заголовки без стилей и свойств
		output_stream() << L"</w:pPr>";
		return 3;
	}

    return 0;
}


void docx_conversion_context::process_page_break_after(const odf_reader::style_instance * styleInst)
{
    if (styleInst)
    {
        const odf_reader::style_instance * inst = styleInst;
        while (inst)
        {
            if (inst->content() && inst->content()->get_style_paragraph_properties())
            {
                _CP_OPT(odf_types::fo_break) fo_break_val = inst->content()->get_style_paragraph_properties()->content_.fo_break_after_;
                if (fo_break_val)
                {
					set_page_break_after(fo_break_val->get_type());
					break;
                }
            }
            inst = inst->parent();
        }
    }
}
void docx_conversion_context::serialize_list_properties(std::wostream & strm)
{
    if (list_style_stack_.empty()) return;
   
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

void docx_conversion_context::set_drawing_text_props (const std::wstring &props)
{
	get_styles_context().text_style_ext() = props;
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
		delayed_elements_.erase(delayed_elements_.begin(), delayed_elements_.begin() + 1);
    }
	delayed_converting_=false;
}

void section_context::add_section(const std::wstring & SectionName, const std::wstring & Style, const std::wstring & PageProperties)
{
    _section newSec(SectionName, Style, PageProperties );
    sections_.push_back(newSec);
}
section_context::_section & section_context::get_first()		
{ 
	if (sections_.empty())
		return main_section_;
	else
		return sections_[0]; 
}
section_context::_section & section_context::get_last()		
{ 
	if (sections_.empty())
		return main_section_;
	else
		return sections_.back();
}
void section_context::remove_section()	
{
	if (sections_.empty()) return;

	sections_.pop_back();
	//sections_.erase(sections_.begin(), sections_.begin() + 1);
	if (sections_.empty())
	{
		//после оканчания разметки секциями и начале (возобновлении) основного раздела нужен разрыв (хотя настройки страницы могут и не поменяться)
		//щас разрыв на текущей странице
		//todooo проверить - может типо если следующий будет заголовок - разорвать
		main_section_.is_dump_ = false;
	}
}
namespace 
{
    // обработка Header/Footer
    // конвертируем содержимое header/footer и сохраняем результат в виде строки
    void process_one_header_footer(docx_conversion_context & Context, const std::wstring & styleName, odf_reader::office_element *elm, headers_footers::Type type)
    {
        if (!elm) return;
        
        boost::shared_ptr<std::wstringstream> newXml( new std::wstringstream() );
        Context.set_stream_man( streams_man::create(*newXml) );
        
		elm->docx_convert(Context);
        
		const std::wstring & dbgStr = newXml->str();
       
		//слить если есть mediaitems, добавить релсы и обнулить их для основного документа.
		rels internal_rels;

		Context.get_mediaitems()->dump_rels(internal_rels, oox::header_footer_place);
		Context.dump_hyperlinks(internal_rels, oox::header_footer_place);

		Context.get_headers_footers().add(styleName, dbgStr, type, internal_rels);
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
	std::vector<odf_reader::style_master_page*> & master_pages = pageLayouts.master_pages();

	bool bOddEvenPages = false;
	for (size_t i = 0; i < master_pages.size(); i++)
	{
		if (master_pages[i]->style_header_left_ || master_pages[i]->style_footer_left_)
			bOddEvenPages = true;
	}
	if (bOddEvenPages)
	{
		set_settings_property(odf_reader::_property(L"evenAndOddHeaders", true));
	}
 	for (size_t i = 0; i < master_pages.size(); i++)
	{
        const std::wstring & styleName = master_pages[i]->attlist_.style_name_.get_value_or( L"" );
        const std::wstring masterPageNameLayout =context.pageLayoutContainer().page_layout_name_by_style(styleName);
       
		add_page_properties(masterPageNameLayout);
		
		process_one_header_footer(*this, styleName, master_pages[i]->style_header_.get(),		headers_footers::header);
        process_one_header_footer(*this, styleName, master_pages[i]->style_footer_.get(),		headers_footers::footer );
        process_one_header_footer(*this, styleName, master_pages[i]->style_header_first_.get(),	headers_footers::headerFirst);
        process_one_header_footer(*this, styleName, master_pages[i]->style_footer_first_.get(),	headers_footers::footerFirst );
        process_one_header_footer(*this, styleName, master_pages[i]->style_header_left_.get(),	headers_footers::headerLeft );
        process_one_header_footer(*this, styleName, master_pages[i]->style_footer_left_.get(),	headers_footers::footerLeft );

		if (bOddEvenPages && !master_pages[i]->style_header_left_)
			process_one_header_footer(*this, styleName, master_pages[i]->style_header_.get(),	headers_footers::headerLeft);
		if (bOddEvenPages && !master_pages[i]->style_footer_left_)
			process_one_header_footer(*this, styleName, master_pages[i]->style_footer_.get(),	headers_footers::footerLeft );

		if (!master_pages[i]->style_header_ && !master_pages[i]->style_footer_ && !master_pages[i]->style_header_first_ && !master_pages[i]->style_footer_first_
			&& !master_pages[i]->style_header_left_ && !master_pages[i]->style_footer_left_)
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
    const std::wstring s = std::to_wstring(map.size() + 1);
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
    if (current_process_note_ == footNote || current_process_note_ ==  endNote)
    {
        add_element_to_run(_T(""));
        output_stream() << ((current_process_note_ == footNote) ? L"<w:footnoteRef />" : L"<w:endnoteRef />");
        finish_run();
        current_process_note_ = (NoteType) (current_process_note_ + 1); //add ref set
    }
}

typedef std::map<std::wstring, text_tracked_context::_state>::iterator map_changes_iterator;

void docx_conversion_context::start_text_changes (const std::wstring &id)
{
	text_tracked_context::_state  &state_add = text_tracked_context_.get_tracked_change(id);
	if (state_add.id != id) return;
	
	map_current_changes_.insert(std::pair<std::wstring, text_tracked_context::_state> (id, state_add));

	if ( state_add.type == 1 || state_add.type == 2 )
	{
		map_changes_iterator it = map_current_changes_.find(id);
		text_tracked_context::_state  &state = it->second;

		if (state_.in_paragraph_)
		{
			std::wstring format_change = L" w:date=\"" + state.date + L"\" w:author=\"" + state.author + L"\"";

			finish_run();
			state.in_drawing = get_drawing_state_content();

			if (state.oox_id == 0)
			{
				state.oox_id = current_id_changes++;
			}			

			if (state.type == 1 && !state.active)
			{
				output_stream() << L"<w:ins" << format_change << L" w:id=\"" << std::to_wstring(state.oox_id) << L"\">";
				state.active = true;
			}

			if (state.type == 2)
			{
				for (size_t i = 0; i < state.content.size(); i++)
				{
					output_stream() << L"<w:del" << format_change << L" w:id=\"" << std::to_wstring(state.oox_id) << L"\">";

					output_stream() << state.content[i];

					output_stream() << L"</w:del>";
				}
				map_current_changes_.erase(it);
			}
		}
		else
		{
			state.in_drawing = get_drawing_state_content();
			state.out_active = true;
		}
	}
}

void docx_conversion_context::start_changes()
{
	if (map_current_changes_.empty()) return;
	if (current_process_comment_) return;

	text_tracked_context_.dumpPPr_.clear();
	text_tracked_context_.dumpRPr_.clear();
	text_tracked_context_.dumpRPrInsDel_.clear();
	text_tracked_context_.dumpTcPr_.clear();
	text_tracked_context_.dumpTblPr_.clear();

	for (map_changes_iterator it = map_current_changes_.begin(); it != map_current_changes_.end(); ++it)
	{
		text_tracked_context::_state  &state = it->second;

		if (state.type == 0)	continue; //unknown change ... todooo
		if (state.active)		continue;

		std::wstring change_attr;
		change_attr += L" w:date=\"" + state.date + L"\"";
		change_attr += L" w:author=\"" + state.author + L"\"";

		if (state.oox_id == 0)
		{
			state.oox_id = current_id_changes++;
		}
		change_attr += L" w:id=\"" + std::to_wstring(state.oox_id) + L"\"";
		
		if (state.out_active)
		{
			if (state.type == 1)
			{
				output_stream() << L"<w:ins" + change_attr + L">";
				state.active = true;
			}

			if (state.type == 2)
			{
				output_stream() << L"<w:del" + change_attr + L">";
				state.active = true;
			}
		}
		else
		{
			if (state.type == 1)
			{
				text_tracked_context_.dumpRPrInsDel_ = L"<w:ins" + change_attr + L"/>";
			}

			if (state.type == 2)
			{
				text_tracked_context_.dumpRPrInsDel_ = L"<w:del" + change_attr + L"/>";
			}

			if (state.type == 3 && false == state.style_name.empty())
			{
				odf_reader::style_instance * styleInst = root()->odf_context().styleContainer().style_by_name(state.style_name, odf_types::style_family::Paragraph, false);
				if (styleInst)
				{
					odf_reader::style_paragraph_properties	* props = styleInst->content()->get_style_paragraph_properties();
					odf_reader::style_text_properties		* props_text = styleInst->content()->get_style_text_properties();

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
					odf_reader::style_table_properties		* props = styleInst->content()->get_style_table_properties();
					odf_reader::style_table_cell_properties * props_cell = styleInst->content()->get_style_table_cell_properties();
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
					odf_reader::style_table_cell_properties * props = styleInst->content()->get_style_table_cell_properties();
					if (props)
					{
						props->docx_convert(*this);
						text_tracked_context_.dumpTcPr_ += get_styles_context().table_style().str();
					}
					text_tracked_context_.dumpTcPr_ += L"</w:TcPrChange>";
				}
			}
			else if (state.type == 3 && state.style_name.empty())
			{
				if (state_.in_run_)
					text_tracked_context_.dumpRPr_ += L"<w:rPrChange" + change_attr + L"/>";
				else
					text_tracked_context_.dumpPPr_ += L"<w:pPrChange" + change_attr + L"/>";
			}
		}
	}
}

void docx_conversion_context::end_changes()
{
	if (current_process_comment_) return;

	for (map_changes_iterator it = map_current_changes_.begin(); it != map_current_changes_.end(); ++it)
	{
		text_tracked_context::_state  &state = it->second;

		if (state.type	== 0)	continue; //unknown change ... libra format change skip
		if (state.type	== 3)	continue;
		if (!state.active)		continue;
		
		if (state.in_drawing != get_drawing_state_content())
			continue;

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
void docx_conversion_context::end_text_changes (const std::wstring &id)
{
	if (map_current_changes_.empty()) return;

	map_changes_iterator it = map_current_changes_.find(id);

	if (it == map_current_changes_.end()) return;
	
	text_tracked_context::_state  &state = it->second;
	
	if (state.active)
	{
		if (state_.in_paragraph_)
			finish_run();
		
		if (state.type	== 1)	output_stream() << L"</w:ins>";
		if (state.type	== 2)	output_stream() << L"</w:del>";
	}

	map_current_changes_.erase(it);
}
void docx_conversion_context::add_user_field(const std::wstring & name, const std::wstring & value)
{
	map_user_fields.insert(std::make_pair(name, value));
}

std::wstring docx_conversion_context::get_user_field(const std::wstring & name)
{
	std::map<std::wstring, std::wstring>::iterator pFind = map_user_fields.find(name);

	return pFind != map_user_fields.end() ? pFind->second : L"";
}

void docx_conversion_context::add_jsaProject(const std::string &content)
{
	if (content.empty()) return;
	
	output_document_->get_word_files().add_jsaProject(content);
	output_document_->get_content_types_file().add_or_find_default(L"bin");
}

}
}

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

#include <boost/foreach.hpp>

#include "docx_package.h"
#include "docx_conversion_context.h"
#include "headers_footers.h"

namespace cpdoccore { 
namespace oox {
namespace package {

docx_content_types_file::docx_content_types_file()
{
    content()->add_default(L"rels",    L"application/vnd.openxmlformats-package.relationships+xml");
    content()->add_default(L"xml",     L"application/xml");
	//
    content()->add_default(L"jpg",     L"image/jpeg");
    content()->add_default(L"png",     L"image/png");
	//
    content()->add_override(L"/word/document.xml",            L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
	content()->add_override(L"/word/settings.xml",            L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml");
    content()->add_override(L"/word/styles.xml",              L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml");
    content()->add_override(L"/word/fontTable.xml",           L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml");
    //content()->add_override(L"/word/_rels/document.xml.rels", L"application/vnd.openxmlformats-package.relationships+xml");
    //content()->add_override(L"/_rels/.rels",                  L"application/vnd.openxmlformats-package.relationships+xml");
    content()->add_override(L"/docProps/app.xml",             L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    content()->add_override(L"/docProps/core.xml",            L"application/vnd.openxmlformats-package.core-properties+xml");
}


////////////////////////////////////////////////////////////////////////////////////////

word_files::word_files()
{
    rels_file_ptr relFile = rels_file_ptr( new rels_file(L"document.xml.rels") );
    rels_files_.add_rel_file( relFile );
}

void word_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"word";
    NSDirectory::CreateDirectory(path.c_str());

    if (document_)
        document_->write( path );

    if (styles_)
    {
        rels_files_.add( relationship(L"rId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", L"styles.xml" ) );
        styles_->write( path );
    }

    if (fontTable_)
    {
        rels_files_.add( relationship(L"rId2", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable", L"fontTable.xml" ) );
        fontTable_->write( path );
    }

    if (numbering_)
    {
        rels_files_.add( relationship(L"rId3", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering", L"numbering.xml" ) );
        numbering_->write( path );
    }

    if (media_)
    {
        media_->write( path );
    }
   
	if (embeddings_)
    {
        embeddings_->write( path );
    }
	if (drawings_)
    {
        drawings_->set_main_document(get_main_document());
        drawings_->write( path );
    }
	if (headers_footers_)
    {
        headers_footers_->write( path );
    }

    if (settings_)
    {
       rels_files_.add( relationship(L"rId4", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings", L"settings.xml" ) );
       settings_->write( path );
    }

	if (comments_)
	{
       rels_files_.add( relationship(L"rId5", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments", L"comments.xml" ) );
       comments_->write( path );
	}

    {
        charts_files_.set_main_document(get_main_document());
        charts_files_.write(path);
    }

	if (jsaProject_)
	{
		rels_files_.add( relationship(L"jsaId", L"http://schemas.onlyoffice.com/jsaProject", L"jsaProject.bin" ) );
		jsaProject_->write( path );
	}
   
	if (notes_)
    {
		notes_->write( path );
	}
    rels_files_.write(path);
}

void word_files::update_rels(docx_conversion_context & Context)
{
	Context.get_mediaitems()->dump_rels(rels_files_.get_rel_file()->get_rels(), oox::document_place);
	
	Context.dump_hyperlinks		(rels_files_.get_rel_file()->get_rels(), oox::document_place);
    Context.dump_headers_footers(rels_files_.get_rel_file()->get_rels());
    Context.dump_notes			(rels_files_.get_rel_file()->get_rels());
}

void word_files::set_media(mediaitems_ptr & _mediaitems)
{
	if (_mediaitems->count_image + _mediaitems->count_media > 0)
	{
		media_ = element_ptr( new media(_mediaitems, _mediaitems->applicationFonts()) );
	}
	if (_mediaitems->count_object > 0)
	{
		embeddings_ = element_ptr( new embeddings(_mediaitems) );
		embeddings_->set_main_document( get_main_document() );
	}
}

void word_files::set_styles(element_ptr Element) 
{ 
    styles_ = Element;
}

void word_files::set_document(element_ptr Element)
{
    document_ = Element;
}

void word_files::set_fontTable(element_ptr Element)
{
    fontTable_ = Element;
}

void word_files::set_numbering(element_ptr Element)
{
    numbering_ = Element;
}
void word_files::set_settings(element_ptr Element)
{
	settings_ = Element;
}
void word_files::add_charts(chart_content_ptr chart)
{
    charts_files_.add_chart(chart);
}
bool word_files::has_numbering() 
{ 
    return numbering_ ? true : false;
}
void word_files::add_jsaProject(const std::string &content)
{
	jsaProject_ = package::simple_element::create(L"jsaProject.bin", content);
}
void word_files::set_drawings(element_ptr Element)
{
    drawings_ = Element;
}
void word_files::set_headers_footers(headers_footers & HeadersFooters)
{
    headers_footers_elements * elm = new headers_footers_elements(HeadersFooters); 
    elm->set_main_document( get_main_document() );
    headers_footers_ = element_ptr( elm );
}

void word_files::set_notes(notes_context & notesContext)
{
    notes_elements * elm = new notes_elements(notesContext); 
    elm->set_main_document( get_main_document() );
    notes_ = element_ptr( elm );
}
void word_files::set_comments(comments_context & commentsContext)
{
	if (commentsContext.comments_.size()<1)return;

    comments_elements * elm = new comments_elements(commentsContext); 
    elm->set_main_document( get_main_document() );
    comments_ = element_ptr( elm );
}
void word_files::add_rels(relationship const & r)
{
    rels_files_.add(r);

}
//----------------------------------------------------------
int customXml_files::add_customXml(customXml_content_ptr customXml)
{
    customXmls_.push_back(customXml);
	return customXmls_.size();
}

void customXml_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR +  L"customXml";
    NSDirectory::CreateDirectory(path.c_str());

	std::wstring path_rels = path + FILE_SEPARATOR_STR + L"_rels";
    NSDirectory::CreateDirectory(path_rels.c_str());

	for (size_t i = 0 ; i < customXmls_.size(); i++)
    {
        if (!customXmls_[i]) continue;
        
		const std::wstring fileNameItem = std::wstring(L"item") + std::to_wstring(i+1) + L".xml";
		const std::wstring fileNameProps = std::wstring(L"itemProps") + std::to_wstring(i+1) + L".xml";
       
		content_type * contentTypes = get_main_document()->get_content_types_file().content();           
        contentTypes->add_override(std::wstring(L"/customXml/") + fileNameProps, 
							L"application/vnd.openxmlformats-officedocument.customXmlProperties+xml");

        package::simple_element(fileNameItem, customXmls_[i]->item()).write(path);
        package::simple_element(fileNameProps, customXmls_[i]->props()).write(path);
		
		rels_file_ptr rels_file = rels_file::create(fileNameItem + L".rels");
		
		rels_file->get_rels().add(relationship(L"rId1", 
			L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXmlProps", 
			fileNameProps));
		
		rels_file->write(path_rels);
    }
}
//----------------------------------------------------------
void docx_charts_files::add_chart(chart_content_ptr chart)
{
    charts_.push_back(chart);
}

void docx_charts_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR +  L"charts";
    NSDirectory::CreateDirectory(path.c_str());

    size_t count = 0;

	for (size_t i = 0 ; i < charts_.size(); i++)
    {
        if (!charts_[i]) continue;
        
		count++;
        const std::wstring fileName = std::wstring(L"chart") + std::to_wstring(count) + L".xml";
		const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml";
       
		content_type * contentTypes = get_main_document()->get_content_types_file().content();           
        contentTypes->add_override(std::wstring(L"/word/charts/") + fileName, kWSConType);

        package::simple_element(fileName, charts_[i]->str()).write(path);
		
		rels_files relFiles;

		charts_[i]->get_rel_file()->set_file_name(fileName + L".rels");
		    
		relFiles.add_rel_file(charts_[i]->get_rel_file());
		relFiles.write(path);
    }
}
///////////////////////////

headers_footers_elements::headers_footers_elements(headers_footers & HeadersFooters) : headers_footers_(HeadersFooters)
{
}

void headers_footers_elements::write(const std::wstring & RootPath)
{
	BOOST_FOREACH(const headers_footers::instances_map::value_type & instAr, headers_footers_.instances())
    {
        BOOST_FOREACH(const headers_footers::instance_ptr & inst, instAr.second)
        {
			if (inst->type_ == headers_footers::none) continue;

            simple_element(inst->name_, inst->content_).write(RootPath);

            if (document * doc = this->get_main_document())
            {
                const std::wstring ContentType 
                    = (inst->type_ == headers_footers::header || inst->type_ == headers_footers::headerLeft || inst->type_ == headers_footers::headerFirst) ?
                    L"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml" : 
                    L"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml";

                    doc->get_content_types_file().content()->add_override(std::wstring(L"/word/") + inst->name_, ContentType);
            }
			//нужно сформировать релсы с объектов
			if (!inst->rels_.empty())
			{
				rels_files relFiles;
				rels_file_ptr rels_elem = rels_file::create(inst->name_ + L".rels");
				relFiles.add_rel_file(rels_elem);
				
				BOOST_FOREACH(relationship const & r, inst->rels_.relationships())
				{
					relFiles.add(r);
				} 
				relFiles.write(RootPath);
			}
        }
    }
}

///////////////////

notes_elements::notes_elements(notes_context & notesContext) : notes_context_(notesContext)
{
}

namespace 
{
	void process_notes(const notes_context::instances_map & Instances, 
		const std::wstring & Node,
		const std::wstring & Name,
		const std::wstring & ContentType,
		const std::wstring & RootPath,
		rels & Rels,
		document * doc
		)
	{
		if (Instances.size())
		{
			std::wstringstream content; 
	        
			// внимание! L"s - это не ошибка
			content << L"<w:" << Node <<    L"s \
								xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
								xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
								xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
								xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
								xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
								xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
								xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
								xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
								xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
								xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
								xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
								xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
								xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
								xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
								xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
								xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" >";

							//mc:Ignorable=\"w14 wp14\" 
							//xmlns:v=\"urn:schemas-microsoft-com:vml\" 

			BOOST_FOREACH(const notes_context::instances_map::value_type & elm, Instances)
			{
				content << L"<w:" << Node << L" w:id=\"" << elm.second->id << L"\">";
				content << elm.second->content;
				content << L"</w:" << Node << L">";
			}
			content << L"</w:" << Node << L"s>";

			simple_element(Name, content.str()).write(RootPath);
	        
			if (doc)
			{
				doc->get_content_types_file().content()->add_override(std::wstring(L"/word/") + Name, ContentType);
			}

			if (Rels.relationships().size() > 0)
			{
				rels_files relFiles;
				rels_file_ptr rels_elem = rels_file::create( Node + L"s.xml.rels");
				relFiles.add_rel_file(rels_elem);
			
				BOOST_FOREACH(relationship & r, Rels.relationships())
				{
					relFiles.add(r);
				} 
				relFiles.write(RootPath);
			}
		}
	}
}

void notes_elements::write(const std::wstring & RootPath)
{
    process_notes(notes_context_.footnotes(), 
        L"footnote", 
        L"footnotes.xml", 
        L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml", 
        RootPath, 
		notes_context_.footnotesRels(),
        get_main_document());

    process_notes(notes_context_.endnotes(),
        L"endnote",
        L"endnotes.xml",
        L"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml",
        RootPath,
		notes_context_.endnotesRels(),
        get_main_document());
}


///////////////////

comments_elements::comments_elements(comments_context & commentsContext) : comments_context_(commentsContext)
{
}


void comments_elements::write(const std::wstring & RootPath)
{
    if (comments_context_.comments_.size())
    {
        std::wstringstream content; 
        
        content << L"<w:comments xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" " <<
                                L"xmlns:w10=\"urn:schemas-microsoft-com:office:word\" " <<
                                L"xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" " <<
                                L"xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" " <<
                                L"xmlns:o=\"urn:schemas-microsoft-com:office:office\" " <<
                                L">";

        BOOST_FOREACH(const comments_context::_comment_desc & elm, comments_context_.comments_)
        {
            content << L"<w:comment w:id=\"" << elm.id << "\" ";
			if (elm.author.length()>0)
				content << L"w:author=\""<< elm.author << "\" ";
			if (elm.date.length()>0)
				content << L"w:date=\""<< elm.date << "\" ";
			content<< L">";
				content << elm.content;
            content << L"</w:comment>";
        }
        content << L"</w:comments>";

        simple_element(L"comments.xml", content.str()).write(RootPath);
        
        if (get_main_document())
            get_main_document()->get_content_types_file().content()->add_override(L"/word/comments.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml");
    }
	if (!comments_context_.get_rels().empty())
	{
		rels_files relFiles;
		rels_file_ptr rels_elem = rels_file::create(L"comments.xml.rels");
		relFiles.add_rel_file(rels_elem);
		
		BOOST_FOREACH(relationship const & r, comments_context_.get_rels().relationships())
		{
			relFiles.add(r);
		} 
		relFiles.write(RootPath);
	}
}

////////////////////////////////////////////////////////////////////////////////////////

docx_document::docx_document()
{
	this->set_main_document(this);
    
	word_files_.set_main_document(this);
	customXml_files_.set_main_document(this);
   
	rels_file_ptr relFile = rels_file_ptr( new rels_file(L".rels") );
    relFile->get_rels().relationships().push_back( 
        relationship(L"rId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties", L"docProps/app.xml" ) 
        );
    relFile->get_rels().relationships().push_back( 
        relationship(L"rId2", L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties", L"docProps/core.xml" ) 
        );
    relFile->get_rels().relationships().push_back( 
        relationship(L"rId3", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"word/document.xml" ) 
        );

	get_rels_files().add_rel_file( relFile );

}
int docx_document::add_customXml(customXml_content_ptr customXml)
{
    return customXml_files_.add_customXml(customXml);
}
void docx_document::write(const std::wstring & RootPath)
{
    if (word_files_.has_numbering())
    {
        content_type_file_.content()->get_override().push_back( override_content_type(L"/word/numbering.xml",
				L"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml") );
    }
    customXml_files_.write (RootPath);
    word_files_.write (RootPath);
	get_rels_files().write (RootPath);
	get_docProps_files().write (RootPath);
    content_type_file_.write(RootPath);
}



}
}
}

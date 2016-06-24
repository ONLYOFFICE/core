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

#include "pptx_package.h"
#include "pptx_output_xml.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

#include <cpdoccore/utf8cpp/utf8.h>

namespace cpdoccore { 
namespace oox {
namespace package {


pptx_content_types_file::pptx_content_types_file()
{
    content()->add_default(L"rels",  L"application/vnd.openxmlformats-package.relationships+xml");
    content()->add_default(L"xml",   L"application/xml");

    content()->add_default(L"jpg",     L"image/jpeg");
    content()->add_default(L"png",     L"image/png");
 
	content()->add_override(L"/_rels/.rels",			L"application/vnd.openxmlformats-package.relationships+xml");
    
	content()->add_override(L"/ppt/presentation.xml",L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
  //  content()->add_override(L"/ppt/tableStyles.xml",	L"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml");необяательно
  
	content()->add_override(L"/docProps/app.xml",	L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    content()->add_override(L"/docProps/core.xml",	L"application/vnd.openxmlformats-package.core-properties+xml");
}

pptx_document::pptx_document()
{
    ppt_files_.set_main_document(this);
   
	rels_file_ptr relFile = rels_file::create(L".rels");
  
	relFile->get_rels().add(relationship(L"rId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument",		L"ppt/presentation.xml"));
    relFile->get_rels().add(relationship(L"rId2", L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties",		L"docProps/core.xml"));
    relFile->get_rels().add(relationship(L"rId3", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties",	L"docProps/app.xml"));
   
	rels_files_.add_rel_file( relFile );
}

void pptx_document::write(const std::wstring & RootPath)
{
    ppt_files_.write		(RootPath);
    docProps_files_.write	(RootPath);
    rels_files_.write		(RootPath);
    content_type_file_.write(RootPath);
}

////////////////////////////////////////////

slide_content::slide_content() : rels_(rels_file::create(L""))
{
        
}

_CP_PTR(slide_content) slide_content::create()
{
    return boost::make_shared<slide_content>();
}
  
void slide_content::add_rel(relationship const & r)
{
    rels_->get_rels().add(r);
}

void slide_content::add_rels(rels & r)
{
    BOOST_FOREACH(relationship & item, r.relationships())
	{
		rels_->get_rels().add(item);
	}
}
////////////

slides_files::slides_files()
{}

void slides_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slides_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"slides" ;
	FileSystem::Directory::CreateDirectory(path.c_str());

	content_type_content * contentTypes = this->get_main_document()->get_content_types_file().content();
	static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.presentationml.slide+xml";
    
	size_t count = 0;

    BOOST_FOREACH(const slide_content_ptr & item, slides_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName = std::wstring(L"slide") + boost::lexical_cast<std::wstring>(count) + L".xml";
           
			contentTypes->add_override(std::wstring(L"/ppt/slides/") + fileName, kWSConType);

            if (rels_)
            {
                const std::wstring id = std::wstring(L"sId") + boost::lexical_cast<std::wstring>(count);
                static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide";
                const std::wstring fileRef = std::wstring(L"slides/") + fileName;
                rels_->add(id, kWSRel, fileRef);
            }

            item->get_rel_file()->set_file_name(fileName + L".rels");//внитренние релсы
            
			rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(path);           

            package::simple_element(fileName, item->str()).write(path);
        }
    }
}

////////////////////////////////////////////
slideMasters_files::slideMasters_files()
{}

void slideMasters_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slideMasters_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"slideMasters" ;
	FileSystem::Directory::CreateDirectory(path.c_str());

	content_type_content * contentTypes = this->get_main_document()->get_content_types_file().content();
	static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml";
   
	size_t count = 0;

    BOOST_FOREACH(const slide_content_ptr & item, slides_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName = std::wstring(L"slideMaster") + boost::lexical_cast<std::wstring>(count) + L".xml";
            contentTypes->add_override(std::wstring(L"/ppt/slideMasters/") + fileName, kWSConType);

            if (rels_)
            {
                const std::wstring id = std::wstring(L"smId") + boost::lexical_cast<std::wstring>(count);
                static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster";
                const std::wstring fileRef = std::wstring(L"slideMasters/") + fileName;
                rels_->add(id, kWSRel, fileRef);
            }

            item->get_rel_file()->set_file_name(fileName + L".rels");//внитренние релсы
            
			rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(path);           

            package::simple_element(fileName, item->str()).write(path);
        }
    }
}

////////////////////////////////////////////
slideLayouts_files::slideLayouts_files()
{}

void slideLayouts_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slideLayouts_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"slideLayouts" ;
	FileSystem::Directory::CreateDirectory(path.c_str());

    size_t count = 0;

    BOOST_FOREACH(const slide_content_ptr & item, slides_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName		= std::wstring(L"slideLayout") + boost::lexical_cast<std::wstring>(count) + L".xml";
            const std::wstring kWSConType	= L"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml";
           
			content_type_content * contentTypes = get_main_document()->get_content_types_file().content();
            contentTypes->add_override(std::wstring(L"/ppt/slideLayouts/") + fileName, kWSConType);

            item->get_rel_file()->set_file_name(fileName + L".rels");//внитренние релсы
            
			rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(path);           

            package::simple_element(fileName, item->str()).write(path);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////
authors_comments_element::authors_comments_element(pptx_xml_authors_comments_ptr & authors_comments) : authors_comments_(authors_comments)
{
}
void authors_comments_element::write(const std::wstring & RootPath)
{
	if (authors_comments_ == NULL) return;

	const std::wstring file_name = std::wstring(L"commentAuthors.xml");

	std::wstringstream content_authors_comments;
	authors_comments_->write_to(content_authors_comments);

    simple_element(file_name, content_authors_comments.str()).write(RootPath);
    
    if (get_main_document())
	{
        get_main_document()->get_content_types_file().content()->add_override(std::wstring(L"/ppt/commentAuthors.xml"),
			L"application/vnd.openxmlformats-officedocument.presentationml.commentAuthors+xml");
	}

}
////////////////////////////
void ppt_charts_files::add_chart(chart_content_ptr chart)
{
    charts_.push_back(chart);
}
void ppt_charts_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"charts";
	FileSystem::Directory::CreateDirectory(path.c_str());

    size_t count = 0;

    BOOST_FOREACH(const chart_content_ptr & item, charts_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName = std::wstring(L"chart") + boost::lexical_cast<std::wstring>(count) + L".xml";
            content_type_content * contentTypes = get_main_document()->get_content_types_file().content();
           
			static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml";
            contentTypes->add_override(std::wstring(L"/ppt/charts/") + fileName, kWSConType);

            package::simple_element(fileName, item->str()).write(path);
        }
    }
}
///////////////////////////////////
void ppt_themes_files::add_theme(pptx_xml_theme_ptr theme)
{
    themes_.push_back(theme);
}
void ppt_themes_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"theme" ;
	FileSystem::Directory::CreateDirectory(path.c_str());

    size_t count = 0;

    BOOST_FOREACH(const pptx_xml_theme_ptr & item, themes_)
    {
        if (item)
        {
            count++;
            
			const std::wstring fileName		= std::wstring(L"theme") + boost::lexical_cast<std::wstring>(count) + L".xml";
			const std::wstring kWSConType	= L"application/vnd.openxmlformats-officedocument.theme+xml";
           
			content_type_content * contentTypes = get_main_document()->get_content_types_file().content();
            contentTypes->add_override(std::wstring(L"/ppt/theme/") + fileName, kWSConType);

			std::wstringstream content;
			item->write_to(content);
            package::simple_element(fileName, content.str()).write(path);
        }
    }
}
//////////////////////////////
ppt_comments_files_ptr ppt_comments_files::create(const std::vector<pptx_comment_elm> & elms)
{
    return boost::make_shared<ppt_comments_files>(boost::ref(elms));
}

void ppt_comments_files::write(const std::wstring & RootPath)
{
	std::wstring comm_path = RootPath + FILE_SEPARATOR_STR +  L"comments";
	FileSystem::Directory::CreateDirectory(comm_path.c_str());
   
	BOOST_FOREACH(pptx_comment_elm const & e, comments_)
    {
		content_type_content * contentTypes = get_main_document()->get_content_types_file().content();

		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.presentationml.comments+xml";
        contentTypes->add_override(std::wstring(L"/ppt/comments/") + e.filename, kWSConType);
			
		package::simple_element(e.filename, e.content).write( comm_path);        
	}
}
////////////////////////////////////////////
ppt_files::ppt_files()
{
    rels_files_.add_rel_file(rels_file::create(L"presentation.xml.rels"));
}

void ppt_files::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"ppt";
	FileSystem::Directory::CreateDirectory(path.c_str());

    slides_files_.set_rels(&rels_files_);
    slides_files_.set_main_document( this->get_main_document() );
    slides_files_.write(path);

    slideLayouts_files_.set_main_document( this->get_main_document() );
    slideLayouts_files_.write(path);

    slideMasters_files_.set_rels(&rels_files_);
    slideMasters_files_.set_main_document( this->get_main_document() );
    slideMasters_files_.write(path);
	

	//rels_files_.add( relationship( L"hId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"ppt/presentation.xml" ) );

    //if (styles_)
    //{
    //   styles_->write(path);
    //   rels_files_.add( relationship( L"stId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", L"tableStyles.xml" ) );
    //}

    if (presentation_)
    {
        presentation_->write(path);
    }

    if (media_)
    {
        media_->set_main_document(get_main_document());
        media_->write(path);
    }
    {
        charts_files_.set_main_document(get_main_document());
        charts_files_.write(path);
    }
    {
        themes_files_.set_main_document(get_main_document());
        themes_files_.write(path);
    }
	{
        comments_->set_main_document(get_main_document());
        comments_->write(path);
    }
    if (authors_comments_)
    {
		rels_files_.add( relationship( L"auId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors", L"commentAuthors.xml" ) );
        authors_comments_->write(path);
    }
	rels_files_.write(path);
}

void ppt_files::set_presentation(pptx_xml_presentation & presentation)
{
	std::wstringstream strm_presentation;
	presentation.write_to(strm_presentation);

	element_ptr  elm = package::simple_element::create(L"presentation.xml", strm_presentation.str());

    elm->set_main_document( this->get_main_document() );
    presentation_ = elm;
}
void ppt_files::set_comments(element_ptr Element)
{
    comments_ = Element;
}
void ppt_files::set_styles(element_ptr Element)
{
    tableStyles_ = Element;
}

void ppt_files::add_slide(slide_content_ptr slide)
{
    slides_files_.add_slide(slide);
}
void ppt_files::add_slideLayout(slide_content_ptr slide)
{
    slideLayouts_files_.add_slide(slide);
}
void ppt_files::add_slideMaster(slide_content_ptr slide)
{
    slideMasters_files_.add_slide(slide);
}

void ppt_files::set_media(mediaitems & _Mediaitems, CApplicationFonts *pAppFonts)
{
    media_ = element_ptr( new media(_Mediaitems, pAppFonts) );
}
void ppt_files::set_authors_comments(pptx_xml_authors_comments_ptr & authors_comments)
{
	if (authors_comments)
	{
		authors_comments_element * elm = new authors_comments_element(authors_comments); 
		elm->set_main_document( this->get_main_document() );
		authors_comments_ = element_ptr( elm );
	}
}
void ppt_files::add_charts(chart_content_ptr chart)
{
    charts_files_.add_chart(chart);
}
void ppt_files::add_theme(pptx_xml_theme_ptr theme)
{
    themes_files_.add_theme(theme);
}
}
}
}

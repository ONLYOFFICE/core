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

#include "xlsx_package.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

#include <cpdoccore/utf8cpp/utf8.h>

namespace cpdoccore { 
namespace oox {
namespace package {


xlsx_content_types_file::xlsx_content_types_file()
{
    content()->add_default(L"rels",  L"application/vnd.openxmlformats-package.relationships+xml");
    content()->add_default(L"xml",   L"application/xml");

    content()->add_default(L"jpg",     L"image/jpeg");
    content()->add_default(L"png",     L"image/png");

    content()->add_default(L"vml",    L"application/vnd.openxmlformats-officedocument.vmlDrawing");
 
	content()->add_override(L"/_rels/.rels",                  L"application/vnd.openxmlformats-package.relationships+xml");
    content()->add_override(L"/xl/_rels/workbook.xml.rels",   L"application/vnd.openxmlformats-package.relationships+xml");
    content()->add_override(L"/xl/sharedStrings.xml",         L"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml");
    content()->add_override(L"/xl/workbook.xml",              L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
    content()->add_override(L"/xl/styles.xml",                L"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml");
    content()->add_override(L"/docProps/app.xml",             L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    content()->add_override(L"/docProps/core.xml",            L"application/vnd.openxmlformats-package.core-properties+xml");
}

xlsx_document::xlsx_document()
{
    xl_files_.set_main_document(this);
    rels_file_ptr relFile = rels_file::create(L".rels");
   
	relFile->get_rels().add(relationship(L"rId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"xl/workbook.xml"));
    relFile->get_rels().add(relationship(L"rId2", L"http://schemas.openxmlformats.org/officedocument/2006/relationships/metadata/core-properties", L"docProps/core.xml"));
    relFile->get_rels().add(relationship(L"rId3", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties", L"docProps/app.xml"));
   
	rels_files_.add_rel_file( relFile );
}

void xlsx_document::write(const std::wstring & RootPath)
{
    xl_files_.write			(RootPath);
    docProps_files_.write	(RootPath);
    rels_files_.write		(RootPath);
    content_type_file_.write(RootPath);
}

////////////////////////////////////////////

sheet_content::sheet_content() : rels_(rels_file::create(L""))
{
        
}

_CP_PTR(sheet_content) sheet_content::create()
{
    return boost::make_shared<sheet_content>();
}

void sheet_content::add_rel(relationship const & r)
{
    rels_->get_rels().add(r);
}

void sheet_content::add_rels(rels & r)
{
    BOOST_FOREACH(relationship & item, r.relationships())
	{
		rels_->get_rels().add(item);
	}
}
////////////

sheets_files::sheets_files()
{}

void sheets_files::add_sheet(sheet_content_ptr sheet)
{
    sheets_.push_back(sheet);
}

void sheets_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"worksheets";
	FileSystem::Directory::CreateDirectory(path.c_str());

    size_t count = 0;

    BOOST_FOREACH(const sheet_content_ptr & item, sheets_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName		= std::wstring(L"sheet") + boost::lexical_cast<std::wstring>(count) + L".xml";
			const std::wstring kWSConType	= L"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml";
            
			content_type_content * contentTypes = this->get_main_document()->get_content_types_file().content();
            contentTypes->add_override(std::wstring(L"/xl/worksheets/") + fileName, kWSConType);

            if (rels_)
            {
                const std::wstring id = std::wstring(L"sId") + boost::lexical_cast<std::wstring>(count);
                static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";
                const std::wstring fileRef = std::wstring(L"worksheets/") + fileName;
                rels_->add(id, kWSRel, fileRef);
            }

            item->get_rel_file()->set_file_name(fileName + L".rels"); 
            rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(path);
            
            //item->get_rel_file()->write(path.string<std::wstring>());

            package::simple_element(fileName, item->str()).write(path);
        }
    }
}

////////////////////////////////////////////

xl_files::xl_files()
{
    rels_files_.add_rel_file(rels_file::create(L"workbook.xml.rels"));
}

void xl_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"xl";
	FileSystem::Directory::CreateDirectory(path.c_str());

    sheets_files_.set_rels(&rels_files_);
    sheets_files_.set_main_document( this->get_main_document() );
    sheets_files_.write(path);

	int index = 1;
    if (true)
    {
        //workbook_->hyperlinks->write(path);
        rels_files_.add( relationship( L"hId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"xl/workbook.xml" ) );
	}
	if (sharedStrings_)
    {
        sharedStrings_->write(path);
        rels_files_.add( relationship( L"shId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings", L"sharedStrings.xml" ) );
    }
    if (styles_)
    {
       styles_->write(path);
       rels_files_.add( relationship( L"stId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", L"styles.xml" ) );
    }
    if (workbook_)
    {
        workbook_->write(path);
    }
    if (theme_)
    {
        theme_->write(path);
    }
    if (media_)
    {
        media_->set_main_document(get_main_document());
        media_->write(path);
    }
    if (embeddings_)
    {
        embeddings_->set_main_document(get_main_document());
        embeddings_->write(path);
    }
	{
        charts_files_.set_main_document(get_main_document());
        charts_files_.write(path);
    }
	if (drawings_)
    {
        drawings_->set_main_document(get_main_document());
        drawings_->write(path);
    }
	if (comments_)
    {
        comments_->set_main_document(get_main_document());
        comments_->write(path);
    }
    rels_files_.write(path);
}

void xl_files::set_workbook(element_ptr Element)
{
    workbook_ = Element;
}

void xl_files::set_styles(element_ptr Element)
{
    styles_ = Element;
}

void xl_files::set_sharedStrings(element_ptr Element)
{
    sharedStrings_ = Element;
}

void xl_files::add_sheet(sheet_content_ptr sheet)
{
    sheets_files_.add_sheet(sheet);
}

void xl_files::set_media(mediaitems & _Mediaitems, CApplicationFonts *pAppFonts)
{
	if (_Mediaitems.count_image + _Mediaitems.count_media > 0)
	{
		media_ = element_ptr( new media(_Mediaitems, pAppFonts) );
	}

	if (_Mediaitems.count_object > 0)
	{
		embeddings_ = element_ptr( new embeddings(_Mediaitems) );
	}
}
void xl_files::set_comments(element_ptr Element)
{
    comments_ = Element;
}
void xl_files::set_drawings(element_ptr Element)
{
    drawings_ = Element;
}
void xl_files::set_vml_drawings(element_ptr Element)
{
    vml_drawings_ = Element;
}
void xl_files::add_charts(chart_content_ptr chart)
{
    charts_files_.add_chart(chart);
}
////////////////////////////
void xl_charts_files::add_chart(chart_content_ptr chart)
{
    charts_.push_back(chart);
}
void xl_charts_files::write(const std::wstring & RootPath)
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
            content_type_content * contentTypes = this->get_main_document()->get_content_types_file().content();
           
			static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml";
            contentTypes->add_override(std::wstring(L"/xl/charts/") + fileName, kWSConType);

            package::simple_element(fileName, item->str()).write(path);
			
			rels_files relFiles;

			item->get_rel_file()->set_file_name(fileName + L".rels");
			    
			relFiles.add_rel_file(item->get_rel_file());
			relFiles.write(path);
       }
    }
}
//////////////////////////
xl_drawings_ptr xl_drawings::create(const std::vector<drawing_elm> & elms)
{
    return boost::make_shared<xl_drawings>(boost::ref(elms));
}

void xl_drawings::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"drawings";
	FileSystem::Directory::CreateDirectory(path.c_str());

    BOOST_FOREACH(drawing_elm const & e, drawings_)
    {
        package::simple_element(e.filename, e.content).write(path);        

        rels_files relFiles;
        rels_file_ptr r = rels_file::create(e.filename + L".rels");
        
		e.drawings->dump_rels_drawing(r->get_rels());
                
        relFiles.add_rel_file(r);
        relFiles.write(path);

        content_type_content * contentTypes = this->get_main_document()->get_content_types_file().content();

        const std::wstring kDrawingCT = L"application/vnd.openxmlformats-officedocument.drawing+xml";
        contentTypes->add_override(L"/xl/drawings/" + e.filename, kDrawingCT);
    }
}

//////////////////////////////
xl_comments_ptr xl_comments::create(const std::vector<comment_elm> & elms)
{
    return boost::make_shared<xl_comments>(boost::ref(elms));
}

void xl_comments::write(const std::wstring & RootPath)
{
	std::wstring vml_path = RootPath + FILE_SEPARATOR_STR + L"drawings";
	FileSystem::Directory::CreateDirectory(vml_path.c_str());
   
	BOOST_FOREACH(comment_elm const & e, comments_)
    {
		content_type_content * contentTypes = this->get_main_document()->get_content_types_file().content();

		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml";
        contentTypes->add_override(std::wstring(L"/xl/") + e.filename, kWSConType);
			
		package::simple_element(e.filename, e.content).write(RootPath);        
		package::simple_element(e.vml_filename, e.vml_content).write(vml_path);        
	}
}


}
}
}

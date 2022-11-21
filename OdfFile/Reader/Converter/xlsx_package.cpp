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

#include "xlsx_package.h"
#include "docx_package.h"
#include "pptx_package.h"

#include <boost/ref.hpp>

#include "../../Common/utf8cpp/utf8.h"

#include "../../../DesktopEditor/common/Directory.h"

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
   
	get_rels_files().add_rel_file( relFile );
}

void xlsx_document::write(const std::wstring & RootPath)
{
	_CP_LOG << L"[info][xlsx] process writing" << std::endl;

    xl_files_.write (RootPath);
	get_docProps_files().write (RootPath);
	get_rels_files().write (RootPath);
    content_type_file_.write(RootPath);
}

//--------------------------------------------------------------------------------------------
pivot_cache_content::pivot_cache_content() : definitions_rels_file_(rels_file::create(L""))
{      
}
_CP_PTR(pivot_cache_content) pivot_cache_content::create()
{
    return boost::make_shared<pivot_cache_content>();
}
//--------------------------------------------------------------------------------------------
pivot_table_content::pivot_table_content() : rels_file_(rels_file::create(L""))
{      
}
_CP_PTR(pivot_table_content) pivot_table_content::create()
{
    return boost::make_shared<pivot_table_content>();
}
//--------------------------------------------------------------------------------------------
external_links_content::external_links_content() : rels_file_(rels_file::create(L""))
{      
}
_CP_PTR(external_links_content) external_links_content::create()
{
    return boost::make_shared<external_links_content>();
}
//--------------------------------------------------------------------------------------------
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
	std::vector<relationship> & items = r.relationships();
    for (size_t i = 0; i < items.size(); i++)
	{
		rels_->get_rels().add(items[i]);
	}
}
//--------------------------------------------------------------------------------------------
sheets_files::sheets_files()
{}

void sheets_files::add_sheet(sheet_content_ptr sheet)
{
    sheets_.push_back(sheet);
}

void sheets_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"worksheets";
    NSDirectory::CreateDirectory(path.c_str());

    for (size_t i = 0; i < sheets_.size(); i++)
    {
		sheet_content_ptr & item = sheets_[i];
		if (!item) continue;

        const std::wstring fileName		= std::wstring(L"sheet") + std::to_wstring(i + 1) + L".xml";
		const std::wstring kWSConType	= L"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml";
        
		content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
        contentTypes->add_override(std::wstring(L"/xl/worksheets/") + fileName, kWSConType);

        if (rels_)
        {
            const std::wstring id = std::wstring(L"sId") + std::to_wstring(i + 1);
            static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";
            const std::wstring fileRef = std::wstring(L"worksheets/") + fileName;
            rels_->add(id, kWSRel, fileRef);
        }

        item->get_rel_file()->set_file_name(fileName + L".rels"); 
        rels_files relFiles;
        relFiles.add_rel_file(item->get_rel_file());
        relFiles.write(path);
        
        //item->get_rel_file()->write(path.string<std::wstring>());

        package::simple_element(fileName, item->content().str()).write(path);
    }
}
//--------------------------------------------------------------------------------------------
xl_files::xl_files()
{
    rels_files_.add_rel_file(rels_file::create(L"workbook.xml.rels"));
}

void xl_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"xl";
    NSDirectory::CreateDirectory(path.c_str());

    {
		pivot_cache_files_.set_rels(&rels_files_);
        pivot_cache_files_.set_main_document(get_main_document());
		pivot_cache_files_.write(path);
    }
	{
		pivot_table_files_.set_main_document(get_main_document());
		pivot_table_files_.write(path);
	}
	{
		sheets_files_.set_rels(&rels_files_);
		sheets_files_.set_main_document( this->get_main_document() );
		sheets_files_.write(path);
	}
	{
		control_props_files_.set_main_document( this->get_main_document() );
		control_props_files_.write(path);
	}
	{
		external_links_files_.set_rels(&rels_files_);
        external_links_files_.set_main_document(get_main_document());
        external_links_files_.write(path);
    }
    {
        //workbook_->hyperlinks->write(path);
        rels_files_.add( relationship( L"hId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"xl/workbook.xml" ) );
	}
	if (sharedStrings_)
    {
        sharedStrings_->write(path);
        rels_files_.add( relationship( L"shId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings", L"sharedStrings.xml" ) );
    }
	if (connections_)
	{
        connections_->write(path);
        rels_files_.add( relationship( L"cnId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/connections", L"connections.xml" ) );

		content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
		contentTypes->add_override(L"/xl/connections.xml", L"application/vnd.openxmlformats-officedocument.spreadsheetml.connections+xml");
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
	{
        table_part_files_.set_main_document(get_main_document());
        table_part_files_.write(path);
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
	if (jsaProject_)
	{
		rels_files_.add( relationship(L"jsaId", L"http://schemas.onlyoffice.com/jsaProject", L"jsaProject.bin" ) );
		jsaProject_->write( path );
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
void xl_files::set_connections(element_ptr Element)
{
    connections_ = Element;
}
void xl_files::add_sheet(sheet_content_ptr sheet)
{
    sheets_files_.add_sheet(sheet);
}

void xl_files::set_media(mediaitems_ptr & _mediaitems)
{
	if (_mediaitems->count_image + _mediaitems->count_media > 0)
	{
		media_ = element_ptr( new media(_mediaitems, _mediaitems->applicationFonts()) );
	}

	if (_mediaitems->count_object > 0)
	{
		embeddings_ = element_ptr( new embeddings(_mediaitems) );
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
void xl_files::add_pivot_cache(pivot_cache_content_ptr pivot_cache)
{
    pivot_cache_files_.add_pivot_cache(pivot_cache);
}
void xl_files::add_pivot_table(pivot_table_content_ptr pivot_table)
{
    pivot_table_files_.add_pivot_table(pivot_table);
}
void xl_files::add_table_part(const std::wstring &content)
{
	table_part_files_.add_table_part(content);
}
void xl_files::add_jsaProject(const std::string &content)
{
	jsaProject_ = package::simple_element::create(L"jsaProject.bin", content);
}
void xl_files::add_control_props (simple_element_ptr element)
{
	control_props_files_.add_control_props(element);
}
void xl_files::add_external_links(external_links_content_ptr content)
{
    external_links_files_.add_external_links(content);
}
//----------------------------------------------------------------------------------------
void xl_pivot_cache_files::add_pivot_cache(pivot_cache_content_ptr pivot_cache)
{
    pivot_caches_.push_back(pivot_cache);
}
void xl_pivot_cache_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"pivotCache";
    NSDirectory::CreateDirectory(path.c_str());

	content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
	
	static const std::wstring kWSConTypeD = L"application/vnd.openxmlformats-officedocument.spreadsheetml.pivotCacheDefinition+xml";
	static const std::wstring kWSConTypeR = L"application/vnd.openxmlformats-officedocument.spreadsheetml.pivotCacheRecords+xml";
	
	for (size_t i = 0; i < pivot_caches_.size(); i++)
    {
        if (pivot_caches_[i])
        {
            const std::wstring fileNameD = std::wstring(L"pivotCacheDefinition") + std::to_wstring(i + 1) + L".xml";
           
            contentTypes->add_override(std::wstring(L"/xl/pivotCache/") + fileNameD, kWSConTypeD);

            package::simple_element(fileNameD, pivot_caches_[i]->definitions().str()).write(path);

            if (pivot_caches_[i]->get_rels().empty() == false)
			{
				rels_files relFiles;
				pivot_caches_[i]->definitions_rels_file_->set_file_name(fileNameD + L".rels");
				
				relFiles.add_rel_file(pivot_caches_[i]->definitions_rels_file_);
				relFiles.write(path);
			}
			if (rels_) //for workbook
			{
				const std::wstring id = std::wstring(L"pcId") + std::to_wstring(i + 1);
				static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition";
				const std::wstring fileRef = std::wstring(L"pivotCache/") + fileNameD;
				rels_->add(id, kWSRel, fileRef);
			}
			std::wstring content_records = pivot_caches_[i]->records().str();
			if (!content_records.empty())
			{
				const std::wstring fileNameR = std::wstring(L"pivotCacheRecords") + std::to_wstring(i + 1) + L".xml";
	           
				contentTypes->add_override(std::wstring(L"/xl/pivotCache/") + fileNameR, kWSConTypeR);

				package::simple_element(fileNameR, content_records).write(path);
			}
        }
    }
}
//----------------------------------------------------------------------------------------
void xl_pivot_table_files::add_pivot_table(pivot_table_content_ptr pivot_table)
{
    pivot_tables_.push_back(pivot_table);
}
void xl_pivot_table_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"pivotTables";
    NSDirectory::CreateDirectory(path.c_str());

	content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
	
	static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.pivotTable+xml";
	
	for (size_t i = 0; i < pivot_tables_.size(); i++)
    {
        if (pivot_tables_[i])
        {
            const std::wstring fileName = std::wstring(L"pivotTable") + std::to_wstring(i + 1) + L".xml";
           
            contentTypes->add_override(std::wstring(L"/xl/pivotTables/") + fileName, kWSConType);

            package::simple_element(fileName, pivot_tables_[i]->str()).write(path);

            if (pivot_tables_[i]->get_rels().empty() == false)
			{
				rels_files relFiles;
				pivot_tables_[i]->rels_file_->set_file_name(fileName + L".rels");
				
				relFiles.add_rel_file(pivot_tables_[i]->rels_file_);
				relFiles.write(path);
			}
        }
    }
}
//------------------------------------------------------------------------------------------------------
void xl_external_links_files::add_external_links(external_links_content_ptr content)
{
    external_links_.push_back(content);
}
void xl_external_links_files::write(const std::wstring & RootPath)
{
	if (external_links_.empty()) return;

	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"externalLinks";
    NSDirectory::CreateDirectory(path.c_str());
	
	for (size_t i = 0; i < external_links_.size(); i++)
    {
        const std::wstring fileName = std::wstring(L"externalLink") + std::to_wstring(i + 1) + L".xml";
        content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
       
		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.externalLink+xml";
        contentTypes->add_override(std::wstring(L"/xl/externalLinks/") + fileName, kWSConType);

        package::simple_element(fileName, external_links_[i]->content_.str()).write(path);

		{
			rels_files relFiles;
			external_links_[i]->rels_file_->set_file_name(fileName + L".rels");
			
			relFiles.add_rel_file(external_links_[i]->rels_file_);
			relFiles.write(path);
		}
        if (rels_)
        {
            static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink";
            const std::wstring fileRef = std::wstring(L"externalLinks/") + fileName;
            rels_->add(external_links_[i]->rId_, kWSRel, fileRef);
        }

    }
}
//------------------------------------------------------------------------------------------------------
void xl_table_part_files::add_table_part(const std::wstring & table_part)
{
    table_parts_.push_back(table_part);
}
void xl_table_part_files::write(const std::wstring & RootPath)
{
	if (table_parts_.empty()) return;

	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"tables";
    NSDirectory::CreateDirectory(path.c_str());
	
	for (size_t i = 0; i < table_parts_.size(); i++)
    {
        const std::wstring fileName = std::wstring(L"table") + std::to_wstring(i + 1) + L".xml";
        content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
       
		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.table+xml";
        contentTypes->add_override(std::wstring(L"/xl/tables/") + fileName, kWSConType);

        package::simple_element(fileName, table_parts_[i]).write(path);
    }
}
//------------------------------------------------------------------------------------------------------
void xl_charts_files::add_chart(chart_content_ptr chart)
{
    charts_.push_back(chart);
}
void xl_charts_files::write(const std::wstring & RootPath)
{
	if (charts_.empty()) return;

	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"charts";
    NSDirectory::CreateDirectory(path.c_str());

	for (size_t i = 0; i < charts_.size(); i++)
    {
        const std::wstring fileName = std::wstring(L"chart") + std::to_wstring(i + 1) + L".xml";
        content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
       
		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml";
        contentTypes->add_override(std::wstring(L"/xl/charts/") + fileName, kWSConType);

        package::simple_element(fileName, charts_[i]->str()).write(path);
		
		rels_files relFiles;

		charts_[i]->get_rel_file()->set_file_name(fileName + L".rels");
		    
		relFiles.add_rel_file(charts_[i]->get_rel_file());
		relFiles.write(path);
    }
}
//----------------------------------------------------------------------------------------
void xl_control_props_files::add_control_props(simple_element_ptr query_table)
{
	control_props_.push_back(query_table);
}
void xl_control_props_files::write(const std::wstring & RootPath)
{
	if (control_props_.empty()) return;

	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"ctrlProps";

	NSDirectory::CreateDirectory(path);

	content_type *contentTypes = this->get_main_document()->get_content_types_file().content();
	static const std::wstring kWSConType = L"application/vnd.ms-excel.controlproperties+xml";
	
	for (size_t i = 0; i < control_props_.size(); i++)
    {
        if (!control_props_[i])continue;

        const std::wstring fileName = control_props_[i]->get_filename();
       
        contentTypes->add_override(std::wstring(L"/xl/ctrlProps/") + fileName, kWSConType);

		control_props_[i]->write(path);
    }
}
//------------------------------------------------------------------------------------------------------
xl_drawings_ptr xl_drawings::create(const std::vector<drawing_elm> & elms, const std::vector<drawing_elm> & vml_elms)
{
    return boost::make_shared<xl_drawings>(boost::ref(elms), boost::ref(vml_elms));
}

void xl_drawings::write(const std::wstring & RootPath)
{
	if (drawings_.empty() && vml_drawings_.empty()) return;

	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"drawings";
    NSDirectory::CreateDirectory(path.c_str());

	content_type * contentTypes = this->get_main_document()->get_content_types_file().content();
	
	pptx_document *pptx = dynamic_cast<pptx_document*>(this->get_main_document());
	xlsx_document *xlsx = dynamic_cast<xlsx_document*>(this->get_main_document());
	docx_document *docx = dynamic_cast<docx_document*>(this->get_main_document());

	const std::wstring override_str = docx ? L"/word/drawings/" : (pptx ? L"/ppt/drawings/" : L"/xl/drawings/");
    
	for (size_t i = 0; i < drawings_.size(); i++)
    {
        package::simple_element(drawings_[i].filename, drawings_[i].content).write(path);        

        rels_files relFiles;
        rels_file_ptr r = rels_file::create(drawings_[i].filename + L".rels");
        
		drawings_[i].drawings->dump_rels_drawing(r->get_rels());
                
        relFiles.add_rel_file(r);
        relFiles.write(path);

		const std::wstring kDrawingCT = drawings_[i].type == typeChartUserShapes ?  
										L"application/vnd.openxmlformats-officedocument.drawingml.chartshapes+xml" :
										L"application/vnd.openxmlformats-officedocument.drawing+xml";
       
		contentTypes->add_override(override_str + drawings_[i].filename, kDrawingCT);
    }
	for (size_t i = 0; i < vml_drawings_.size(); i++)
    {
        package::simple_element(vml_drawings_[i].filename, vml_drawings_[i].content).write(path);        

        rels_files relFiles;
        rels_file_ptr r = rels_file::create(vml_drawings_[i].filename + L".rels");
        
		vml_drawings_[i].drawings->dump_rels_vml_drawing(r->get_rels());
                
        relFiles.add_rel_file(r);
        relFiles.write(path);

		//content types - default
    }
}

//////////////////////////////
xl_comments_ptr xl_comments::create(const std::vector<comment_elm> & elms)
{
    return boost::make_shared<xl_comments>(boost::ref(elms));
}

void xl_comments::write(const std::wstring & RootPath)
{
    for (size_t i = 0; i < comments_.size(); i++)
    {
		content_type * contentTypes = this->get_main_document()->get_content_types_file().content();

		static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml";
        contentTypes->add_override(std::wstring(L"/xl/") + comments_[i].filename, kWSConType);
			
		package::simple_element(comments_[i].filename, comments_[i].content).write(RootPath);        
	}
}


}
}
}

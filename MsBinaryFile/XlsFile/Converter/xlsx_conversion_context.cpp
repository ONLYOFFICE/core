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

#include "xlsx_conversion_context.h"

#include <iostream>
#include "../../Common/Utils/simple_xml_writer.h"

#include "xlsx_package.h"


namespace oox {
    

xlsx_conversion_context::xlsx_conversion_context( package::xlsx_document * outputDocument)	: output_document_(outputDocument),
	next_vml_file_id_				( 1 ),
	sheet_context_					( *this ), 
	xlsx_drawing_context_handle_	( next_vml_file_id_, get_mediaitems())
{
}
xlsx_conversion_context::~xlsx_conversion_context()
{
}

xlsx_comments_context & xlsx_conversion_context::get_comments_context()
{
    return get_sheet_context().get_comments_context();
}

xlsx_comments_context_handle & xlsx_conversion_context::get_comments_context_handle()
{
    return xlsx_comments_context_handle_;
}

xlsx_drawing_context & xlsx_conversion_context::get_drawing_context()
{
    return get_sheet_context().get_drawing_context();
}

void xlsx_conversion_context::start_document()
{
}

xlsx_xml_worksheet & xlsx_conversion_context::current_sheet()
{
    if (!sheets_.empty())
    {
        return *sheets_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}

oox_chart_context & xlsx_conversion_context::current_chart()
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
oox_external_context & xlsx_conversion_context::current_external()
{
    if (!externals_.empty())
    {
        return *externals_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
oox_activeX_context & xlsx_conversion_context::current_activeX()
{
    if (!activeXs_.empty())
    {
        return *activeXs_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
bool xlsx_conversion_context::start_sheet()
{
    sheets_.push_back(xlsx_xml_worksheet::create());
    get_sheet_context().start_table();

	return true;
}

void xlsx_conversion_context::set_sheet_type(int type)
{
	if (sheets_.empty()) return;

	sheets_.back()->type = type;
}
void xlsx_conversion_context::start_table()
{
}
void xlsx_conversion_context::end_table()
{
	int index = tables_context_.get_count();

	std::wstring rid = L"tpId" + std::to_wstring(index);

	current_sheet().sheet_rels().add(oox::relationship(rid,
		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/table", 
		L"../tables/table" + std::to_wstring(index) +L".xml"));

	CP_XML_WRITER(current_sheet().tableParts())
	{
        CP_XML_NODE(L"tablePart")
        {
			CP_XML_ATTR(L"r:id", rid);
		}
	}
}
void xlsx_conversion_context::set_sheet_name(const std::wstring & name)
{
	if (name.empty()) return;

	sheets_.back()->name = name;
}
void xlsx_conversion_context::set_sheet_state(const std::wstring & state)
{
	if (state.empty()) return;

	sheets_.back()->state = state;
}
void xlsx_conversion_context::set_sheet_id(int id)
{
	if (id < 0) return;
	sheets_.back()->id = id;
	
	sheets_map_.insert(std::make_pair(id, sheets_.size() - 1));
}
void xlsx_conversion_context::start_chart()
{
	charts_.push_back(oox_chart_context::create());
	//добавляем новую форму для диаграммы
	 //в ней будет информационная часть - и она пишется каждый раз в свою xml (их - по числу диаграмм)
	//этот контекст нужно передавать в файл

}

std::wstring xlsx_conversion_context::start_activeX()
{
	activeXs_.push_back(oox_activeX_context::create());
	
	size_t index = activeXs_.size();

	std::wstring rid = L"ocxId" + std::to_wstring(index);

	current_sheet().sheet_rels().add(oox::relationship(rid,
		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/control", 
		L"../activeX/activeX" + std::to_wstring(index) +L".xml"));
	
	return rid;
}
void xlsx_conversion_context::start_external()
{
	externals_.push_back(oox_external_context::create());
}
void xlsx_conversion_context::end_external()
{
}


void xlsx_conversion_context::end_sheet()
{
	get_sheet_context().serialize_ole_objects	(current_sheet().ole_objects());
	get_sheet_context().serialize_controls		(current_sheet().activeXs());

	get_sheet_context().dump_rels_drawing(current_sheet().sheet_rels());
	
	get_sheet_context().serialize_hyperlinks(current_sheet().hyperlinks());
	get_sheet_context().dump_rels_hyperlinks(current_sheet().sheet_rels());

    get_sheet_context().end_table();
}

xlsx_drawing_context_handle & xlsx_conversion_context::get_drawing_context_handle()
{
    return xlsx_drawing_context_handle_;
}

void xlsx_conversion_context::add_connections(const std::wstring & connections)
{
	if (connections.empty()) return;
	connections_ = connections;
}

void xlsx_conversion_context::add_query_table (const std::wstring & query_table)
{
	if (query_table.empty()) return;

	std::wstring rid	= L"qtId" + std::to_wstring(query_tables_.size() + 1);
	std::wstring target = L"queryTable" + std::to_wstring(query_tables_.size() + 1) + L".xml";

	query_tables_.insert(std::make_pair(target, query_table));

	current_sheet().sheet_rels().add(oox::relationship(rid,
		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/queryTable", 
		L"../queryTables/" + target));

}

void xlsx_conversion_context::add_control_props(const std::wstring & target, const std::wstring & props)
{
	if (props.empty()) return;
	control_props_.insert(std::make_pair(target, props));
}

void xlsx_conversion_context::end_document()
{
	std::wstringstream workbook_content;

	//for (size_t i = 0; i < sheets_.size(); i++) нужно по id
	for (std::map<int, int>::iterator it = sheets_map_.begin(); it != sheets_map_.end(); ++it)
	{
		int i = it->second;

        package::sheet_content_ptr content = package::sheet_content::create();
				
		const std::wstring slideRId = std::wstring(L"sId") + std::to_wstring(i + 1);
		content->set_rId(slideRId);

 ////////////////////////////////////////////////////////////////////////////////////////////       
		const std::pair<std::wstring, std::wstring> p1 = sheets_[i]->get_drawing_link();
        
		if (!p1.first.empty())
        {
            const std::wstring dId = p1.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing"; 
            const std::wstring dName = std::wstring(L"../drawings/" + p1.first);
            content->add_rel(relationship(dId, kType, dName));
        }
//////////////////////////////////////////////////////////////////////////////////////////////////
        content->add_rels(sheets_[i]->sheet_rels());
/////////////////////////////////////////////////////////////////////////////////////////////////
		const std::pair<std::wstring, std::wstring> p2 = sheets_[i]->get_comments_link();        
		if (!p2.first.empty())
        {
            const std::wstring			dId		= p2.second;
            static const std::wstring	kType	= L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"; 
            const std::wstring			dName	= std::wstring(L"../" + p2.first);
            content->add_rel(relationship(dId, kType, dName));
        }

		const std::pair<std::wstring, std::wstring> p3 = sheets_[i]->get_vml_drawing_link();		
		if (!p3.first.empty())
        {
            const std::wstring			dId		= p3.second;
            static const std::wstring	kType	= L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing"; 
            const std::wstring			dName	= std::wstring(L"../drawings/" + p3.first);
			content->add_rel(relationship(dId, kType, dName));
       }

		const std::pair<std::wstring, std::wstring> p4 = sheets_[i]->get_vml_drawing_HF_link();		
		if (!p4.first.empty())
        {
            const std::wstring			dId		= p4.second;
            static const std::wstring	kType	= L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing"; 
            const std::wstring			dName	= std::wstring(L"../drawings/" + p4.first);
			content->add_rel(relationship(dId, kType, dName));
       }
/////////////////////////////////////////////////////////////////////////////////////////////////
		sheets_[i]->write_to(content->content());

		output_document_->get_xl_files().add_sheet(sheets_[i]->type, content);

	/////////////////////////////////////////////
        CP_XML_WRITER(workbook_content)
        {
            CP_XML_NODE(L"sheet")
            {
                CP_XML_ATTR(L"name",	sheets_[i]->name);
                CP_XML_ATTR(L"sheetId", sheets_[i]->id);
                CP_XML_ATTR(L"state",	sheets_[i]->state);
                CP_XML_ATTR(L"r:id",	slideRId);            
            }
        }

    }
    unsigned int count = 0;
	for (size_t i = 0; i < activeXs_.size(); i++)
    {
		package::activeX_content_ptr content = package::activeX_content::create();

        activeXs_[i]->dump_rels(content->get_rels());
		activeXs_[i]->write_to(content->content());

		output_document_->get_xl_files().add_activeX(content);
	}
	for (size_t i = 0; i < charts_.size(); i++)
    {
		package::chart_content_ptr content = package::chart_content::create();

        charts_[i]->dump_rels(content->get_rels());
		charts_[i]->write_to(content->content());

		output_document_->get_xl_files().add_chart(content);
	}
	if (!connections_.empty())
	{
		std::wstringstream strm;
		CP_XML_WRITER(strm)    
		{
			CP_XML_NODE(L"connections")
			{
				CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");

				CP_XML_STREAM() << connections_;
			}
		}
		output_document_->get_xl_files().set_connections( package::simple_element::create(L"connections.xml", strm.str()) );
	}     
	for (std::map<std::wstring, std::wstring>::iterator it = query_tables_.begin(); it != query_tables_.end(); ++it)
	{
		output_document_->get_xl_files().add_query_table( package::simple_element::create(it->first, it->second) );
	}
	for (std::map<std::wstring, std::wstring>::iterator it = control_props_.begin(); it != control_props_.end(); ++it)
	{
		output_document_->get_xl_files().add_control_props( package::simple_element::create(it->first, it->second) );
	}
	//workbook_content << L"<calcPr iterateCount=\"100\" refMode=\"A1\" iterate=\"false\" iterateDelta=\"0.0001\" />";

	output_document_->get_xl_files().set_sharedStrings( package::simple_element::create(L"sharedStrings.xml", xlsx_shared_strings_.str()) );


    std::wstringstream strm_workbook;

    CP_XML_WRITER(strm_workbook)
    {
        CP_XML_NODE(L"workbook")
        {
            CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
			CP_XML_ATTR(L"mc:Ignorable", L"x15");
			CP_XML_ATTR(L"xmlns:x15", L"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main");

			CP_XML_STREAM() << xlsx_workbook_pr_.str();

			CP_XML_STREAM() << xlsx_workbookProtection_.str();

			std::wstring str_bookViews = xlsx_workbook_views_.str();

			if (false == str_bookViews.empty())
			{
				CP_XML_NODE(L"bookViews")
				{
					CP_XML_STREAM() << str_bookViews;
				}
			}
            CP_XML_NODE(L"sheets")
            {
                CP_XML_STREAM() << workbook_content.str();
            }
			if (externals_.empty() == false)
			{
				CP_XML_NODE(L"externalReferences")
				{
					for (size_t i = 0; i < externals_.size(); i++)
					{
						std::wstring rId = L"extId" + std::to_wstring(i+1);

						CP_XML_NODE(L"externalReference")
						{
							CP_XML_ATTR(L"r:id", rId);
						}	
						package::external_content_ptr content = package::external_content::create();

						externals_[i]->dump_rels(content->get_rels());
						externals_[i]->write_to(content->content());

						output_document_->get_xl_files().add_external(content);
					}
				}
			}
            CP_XML_NODE(L"definedNames")
            {
				CP_XML_STREAM() << xlsx_defined_names_.str();
			}
			
			std::wstring str_ = xlsx_custom_views_.str();

			if (!str_.empty())
			{
				CP_XML_NODE(L"customWorkbookViews")
				{
					CP_XML_STREAM() << str_;
				}
			}
			int pivot_cache_count = pivots_context_.get_cache_count();
			if (pivot_cache_count > 0)
			{
				CP_XML_NODE(L"pivotCaches")
				{
					for (int i = 0; i < pivot_cache_count; i++)
					{
						std::wstring				rId		= L"pcId" + std::to_wstring(i+1);
						static const std::wstring	sType	= L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition"; 
						const std::wstring			sName	= std::wstring(L"../pivotCache/pivotCacheDefinition" + std::to_wstring(i + 1) + L".xml");
						
						package::pivot_cache_content_ptr content = package::pivot_cache_content::create();
						
						CP_XML_NODE(L"pivotCache")
						{
							CP_XML_ATTR(L"cacheId", std::to_wstring(i));
							CP_XML_ATTR(L"r:id", rId);
						}

						pivots_context_.dump_rels_cache(i, content->get_rels());
						pivots_context_.write_cache_definitions_to(i, content->definitions());
						pivots_context_.write_cache_records_to(i, content->records());

						output_document_->get_xl_files().add_pivot_cache(content);	
					}
				}
			}
			int pivot_view_count = pivots_context_.get_view_count();
			if (pivot_view_count > 0)
			{
				for (int i = 0; i < pivot_view_count; i++)
				{
					package::pivot_table_content_ptr content = package::pivot_table_content::create();

					pivots_context_.dump_rels_view(i, content->get_rels());
					pivots_context_.write_table_view_to(i, content->content());

					output_document_->get_xl_files().add_pivot_table(content);	
				}
			}

		}
		int table_parts_count = tables_context_.get_count();
		if (table_parts_count > 0)
		{
			for (int i = 0; i < table_parts_count; i++)
			{
				package::table_part_content_ptr content = package::table_part_content::create();

				tables_context_.dump_rels(i, content->get_rels());
				tables_context_.write_to(i, content->content());

				output_document_->get_xl_files().add_table_part(content);	
			}
		}
		output_document_->get_xl_files().set_workbook( package::simple_element::create(L"workbook.xml", strm_workbook.str()) );

		output_document_->content_type().set_media(get_mediaitems());
        output_document_->get_xl_files().set_media(get_mediaitems());

        package::xl_drawings_ptr drawings = package::xl_drawings::create(xlsx_drawing_context_handle_.content());
        output_document_->get_xl_files().set_drawings(drawings);
	
		package::xl_drawings_ptr drawings_vml = package::xl_drawings::create(xlsx_drawing_context_handle_.content_vml());
        output_document_->get_xl_files().set_vml_drawings(drawings_vml);

		package::xl_comments_ptr comments = package::xl_comments::create(xlsx_comments_context_handle_.content());
        output_document_->get_xl_files().set_comments(comments);        
	}
}

}

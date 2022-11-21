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

#include "xlsxconversioncontext.h"

#include <iostream>
#include <xml/simple_xml_writer.h>

#include "measuredigits.h"
#include "xlsx_package.h"
#include "xlsx_utils.h"
#include "xlsx_cell_format.h"

#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"
#include "../Format/calcs_styles.h"

namespace cpdoccore { 

namespace odf_reader 
{
    class odf_document;
}

namespace oox {
    
namespace package
{
    class xlsx_document;
}

xlsx_conversion_context::xlsx_conversion_context(odf_reader::odf_document * odfDocument) : 
	odf_document_		(odfDocument),
	output_document_	(NULL),
	num_format_context_	(odf_document_->odf_context()),
	xlsx_text_context_	(odf_document_->odf_context()),
	xlsx_table_context_	(this, xlsx_text_context_),
	math_context_		(odf_document_->odf_context().fontContainer(), true),
	xlsx_style_			(this),

	maxDigitSize_	(std::make_pair(-1.f, -1.f) ),
	default_style_	( (std::numeric_limits<size_t>::max)() )
{
	mediaitems_ = boost::make_shared<mediaitems>(odf_document_->get_folder());
	drawing_context_handle_ =  boost::make_shared<xlsx_drawing_context_handle>(mediaitems_);
}

std::unordered_map<std::wstring, int> xlsx_conversion_context::mapExternalLink_;

void xlsx_conversion_context::set_output_document (package::xlsx_document * document)
{
	output_document_ = document;
}

xlsx_conversion_context::~xlsx_conversion_context()
{
}

void xlsx_conversion_context::set_font_directory(std::wstring pathFonts)
{
	mediaitems_->set_font_directory(pathFonts);
}
void xlsx_conversion_context::set_drawing_context_handle(xlsx_drawing_context_handle_ptr &handle)
{
	drawing_context_handle_ = handle;
}

void xlsx_conversion_context::set_mediaitems(mediaitems_ptr &items)
{
	mediaitems_ = items;
}

void xlsx_conversion_context::start_chart(std::wstring name)
{
	charts_.push_back(oox_chart_context_ptr(new oox_chart_context(mediaitems_, name)));
	//добавляем новую форму для диаграммы
	 //в ней будет информационная часть - и она пишется каждый раз в свою xml (их - по числу диаграмм)
	//этот контекст нужно передавать в файл
}

void xlsx_conversion_context::end_chart()
{
}

void xlsx_conversion_context::start_document()
{
    odf_reader::odf_read_context & odfContext = root()->odf_context();
    std::vector<const odf_reader::style_instance *> instances;
    
	instances.push_back(odfContext.styleContainer().style_default_by_type(odf_types::style_family::TableCell));
	instances.push_back(odfContext.styleContainer().style_by_name(L"Default", odf_types::style_family::TableCell, false));

    odf_reader::text_format_properties_content_ptr	textFormatProperties	= calc_text_properties_content(instances);
    odf_reader::paragraph_format_properties			parFormatProperties		= calc_paragraph_properties_content(instances);
    odf_reader::style_table_cell_properties_attlist	cellFormatProperties	= calc_table_cell_properties(instances);

    oox::xlsx_cell_format cellFormat;
   
	cellFormat.set_cell_type(XlsxCellType::s);
    cellFormat.set_num_format(oox::odf_string_to_build_in(0));

    default_style_ = get_style_manager().xfId(textFormatProperties, &parFormatProperties, &cellFormatProperties, &cellFormat, L"", 0, true);

}

void xlsx_conversion_context::end_document()
{
	std::wstringstream workbook_content;

	if (sheets_.empty())
	{ // owncloud new document  ... oO
		start_table(L"Sheet1", L"");
		current_sheet().cols() << L"<col min=\"1\" max=\"1024\" width=\"11.6\" customWidth=\"0\"/>";
		end_table();
	}

	for (size_t i = 0; i < sheets_.size(); i++)
    {
		xlsx_xml_worksheet_ptr& sheet = sheets_[i];
		
		const std::wstring id = std::wstring(L"sId") + std::to_wstring(i + 1);

        package::sheet_content_ptr content = package::sheet_content::create();
 ////////////////////////////////////////////////////////////////////////////////////////////       
		const std::pair<std::wstring, std::wstring> p1 = sheet->get_drawing_link();
        
		if (!p1.first.empty())
        {
            const std::wstring dId = p1.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing"; 
            const std::wstring dName = std::wstring(L"../drawings/" + p1.first);
            content->add_rel(relationship(dId, kType, dName));
        }
//////////////////////////////////////////////////////////////////////////////////////////////////
        content->add_rels(sheet->sheet_rels());
/////////////////////////////////////////////////////////////////////////////////////////////////
		const std::pair<std::wstring, std::wstring> p2 = sheet->get_comments_link();        
		if (!p2.first.empty())
        {
            const std::wstring dId = p2.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"; 
            const std::wstring dName = std::wstring(L"../" + p2.first);
            content->add_rel(relationship(dId, kType, dName));
        }

		const std::pair<std::wstring, std::wstring> p3 = sheet->get_vml_drawing_link();		
		if (!p3.first.empty())
        {
            const std::wstring dId = p3.second;
            static const std::wstring kType = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing"; 
            const std::wstring dName = std::wstring(L"../drawings/" + p3.first);
			content->add_rel(relationship(dId, kType, dName));
       }
/////////////////////////////////////////////////////////////////////////////////////////////////
		sheet->write_to(content->content());
        output_document_->get_xl_files().add_sheet(content);

	/////////////////////////////////////////////
        CP_XML_WRITER(workbook_content)
        {
            CP_XML_NODE(L"sheet")
            {
                CP_XML_ATTR(L"name",	XmlUtils::EncodeXmlString(sheet->name())); // office 2010 ! ограничение на длину имени !!!
                CP_XML_ATTR(L"sheetId", i + 1);
				CP_XML_ATTR(L"state",	sheet->hidden() ? L"hidden" : L"visible");
                CP_XML_ATTR(L"r:id",	id);            
            }
        }

    }
	for (std::map<std::wstring, std::wstring>::iterator it = control_props_.begin(); it != control_props_.end(); ++it)
	{
		output_document_->get_xl_files().add_control_props( package::simple_element::create(it->first, it->second) );
	}
    for (size_t i = 0; i < charts_.size(); i++)
    {
		package::chart_content_ptr content = package::chart_content::create();

		charts_[i]->serialize(content->content());
		charts_[i]->dump_rels(content->get_rel_file()->get_rels());
		
		output_document_->get_xl_files().add_charts(content);
	}
    for (size_t i = 0; i < table_parts_.size(); i++)
    {
		output_document_->get_xl_files().add_table_part(table_parts_[i]);
	}
    //workbook_content << L"<calcPr iterateCount=\"100\" refMode=\"A1\" iterate=\"false\" iterateDelta=\"0.0001\" />";

    {
        std::wstringstream strm;
        xlsx_text_context_.serialize_shared_strings(strm);
        output_document_->get_xl_files().set_sharedStrings( package::simple_element::create(L"sharedStrings.xml", strm.str()) );
    }

    {
        std::wstringstream strm;
        xlsx_style_.xlsx_serialize(strm);
        output_document_->get_xl_files().set_styles( package::simple_element::create(L"styles.xml", strm.str()) );
    }

    {
        std::wstringstream strm_workbook;

        CP_XML_WRITER(strm_workbook)
        {
            CP_XML_NODE(L"workbook")
            {
                CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
                CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

				if (table_structure_protected_)
				{
					CP_XML_NODE(L"workbookProtection")
					{
						CP_XML_ATTR(L"lockStructure", 1);
					}
				}
				serialize_bookViews (CP_XML_STREAM());

                CP_XML_NODE(L"sheets")
                {
                    CP_XML_STREAM() << workbook_content.str();
                }
				if (false == mapExternalLink_.empty())
				{
					CP_XML_NODE(L"externalReferences")
					{
						for (std::unordered_map<std::wstring, int>::iterator it = mapExternalLink_.begin(); 
							it != mapExternalLink_.end(); ++it)
						{
							package::external_links_content_ptr content = package::external_links_content::create();
							content->rId() = L"extRef" + std::to_wstring(it->second);
							{
								CP_XML_WRITER(content->content())
								{
									CP_XML_NODE(L"externalLink")
									{
										CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
										CP_XML_NODE(L"externalBook")
										{
											CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
											CP_XML_ATTR(L"r:id", L"rId1");
										}
									}
								}
							}
							
							content->get_rels().add(relationship(L"rId1", mediaitems::get_rel_type(typeExternalLink), it->first, L"External"));

							output_document_->get_xl_files().add_external_links(content);	

							CP_XML_NODE(L"externalReference")
							{
								CP_XML_ATTR(L"r:id", content->rId());
							}
						}
					}
				}
                get_xlsx_defined_names().xlsx_serialize(CP_XML_STREAM());

				int pivot_cache_count = xlsx_pivots_context_.get_count();
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

							xlsx_pivots_context_.dump_rels_cache(i, content->get_rels());
							xlsx_pivots_context_.write_cache_definitions_to(i, content->definitions());
							xlsx_pivots_context_.write_cache_records_to(i, content->records());

							output_document_->get_xl_files().add_pivot_cache(content);	
						}
					}
				}
				int pivot_view_count = xlsx_pivots_context_.get_count();
				if (pivot_view_count > 0)
				{
					for (int i = 0; i < pivot_view_count; i++)
					{
						package::pivot_table_content_ptr content = package::pivot_table_content::create();

						xlsx_pivots_context_.dump_rels_view(i, content->get_rels());
						xlsx_pivots_context_.write_table_view_to(i, content->content());

						output_document_->get_xl_files().add_pivot_table(content);	
					}
				}
				if (xlsx_pivots_context_.is_connections())
				{
					std::wstringstream strm;
					xlsx_pivots_context_.write_connections_to(strm);

					output_document_->get_xl_files().set_connections( package::simple_element::create(L"connections.xml", strm.str()) );
				}
            }
        }

        output_document_->get_xl_files().set_workbook( package::simple_element::create(L"workbook.xml", strm_workbook.str()) );

		output_document_->get_content_types_file().set_media(get_mediaitems());
        output_document_->get_xl_files().set_media(get_mediaitems());

		package::xl_drawings_ptr drawings = package::xl_drawings::create(drawing_context_handle_->content(), drawing_context_handle_->content_vml());
		output_document_->get_xl_files().set_drawings(drawings);
	
        package::xl_comments_ptr comments = package::xl_comments::create(xlsx_comments_context_handle_.content());
        output_document_->get_xl_files().set_comments(comments);        
	}
}


void xlsx_conversion_context::serialize_bookViews(std::wostream & strm)
{
	odf_reader::settings_container &settings = odf_document_->odf_context().Settings();

	if (settings.get_views_count() < 1) return;

    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"bookViews")
        {
			for (int i = 0; i < settings.get_views_count(); i++)
			{
				_CP_OPT(std::wstring) sActiveTable	= settings.find_view_by_name(L"ActiveTable", i);
				_CP_OPT(std::wstring) sAreaWidth	= settings.find_view_by_name(L"VisibleAreaWidth", i);
				_CP_OPT(std::wstring) sAreaHeight	= settings.find_view_by_name(L"VisibleAreaHeight", i);
				_CP_OPT(std::wstring) sAreaTop		= settings.find_view_by_name(L"VisibleAreaTop", i);
				_CP_OPT(std::wstring) sAreaLeft		= settings.find_view_by_name(L"VisibleAreaLeft", i);

				CP_XML_NODE(L"workbookView")
				{
					if (sActiveTable)
					{
						for (size_t i = 0; i < sheets_.size(); i++)
						{
							if (sheets_[i]->name() == *sActiveTable)
							{
								CP_XML_ATTR(L"activeTab", i);
							}
						}		
					}
					if (sAreaWidth)		CP_XML_ATTR(L"windowWidth", *sAreaWidth);
					if (sAreaHeight)	CP_XML_ATTR(L"windowHeight", *sAreaHeight);

					if (sAreaTop)		CP_XML_ATTR(L"yWindow", *sAreaTop);
					if (sAreaLeft)		CP_XML_ATTR(L"xWindow", *sAreaLeft);

					CP_XML_ATTR(L"showSheetTabs",	true);
					CP_XML_ATTR(L"showVerticalScroll",	true);
					CP_XML_ATTR(L"showHorizontalScroll",true);

				}
			}
		}
	}
}

void xlsx_conversion_context::serialize_calcPr (std::wostream & strm)
{
}

void xlsx_conversion_context::start_body()
{}

void xlsx_conversion_context::end_body()
{}

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
xlsx_xml_worksheet & xlsx_conversion_context::current_sheet(int index)
{
    if (!sheets_.empty())
    {
		if (index < 0)	return *sheets_.back().get();
		else			return *sheets_[index].get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
int xlsx_conversion_context::find_sheet_by_name(std::wstring tableName)
{
	if (tableName.empty()) return -1;

	if (0 == tableName.find(L"'"))
	{
		tableName = tableName.substr(1, tableName.length() - 2);
	}
	for (size_t i = 0; i < sheets_.size(); i++)
	{
		if (sheets_[i]->name() == tableName)
			return i;
	}
	return -1;
}

bool xlsx_conversion_context::start_table(std::wstring tableName, std::wstring tableStyleName)
{
	get_table_context().start_table(tableName, tableStyleName, sheets_.size() - 1);

    sheets_.push_back(xlsx_xml_worksheet::create(tableName, get_table_context().state()->get_table_hidden()));
	current_sheet().cols() << L"<cols>";
    return true;
}

void xlsx_conversion_context::end_table()
{
    const double lastWidht = table_column_last_width();
    if (lastWidht > 0.0)
    {
        unsigned int cMin = get_table_context().columns_count() + 1;
        unsigned int cMax = (std::max)((unsigned int)1024, get_table_context().columns_count() + 100);

		if (cMin < 16384)
		{
			if (cMax > 16384) cMax = 16384;
			CP_XML_WRITER(current_sheet().cols())
			{
				CP_XML_NODE(L"col")
				{
					//CP_XML_ATTR(L"collapsed", L"false");            
					//CP_XML_ATTR(L"hidden", L"false");            
					CP_XML_ATTR(L"max", cMax);
					CP_XML_ATTR(L"min", cMin);
					//CP_XML_ATTR(L"style", 0);
					CP_XML_ATTR(L"width", lastWidht);
					CP_XML_ATTR(L"customWidth", 0);
				}
			}
		}
    }    
    current_sheet().cols() << L"</cols>";
    
	get_table_context().serialize_table_format			(current_sheet().sheetFormat());
	get_table_context().serialize_page_properties		(current_sheet().page_properties());
	get_table_context().serialize_header_footer			(current_sheet().header_footer());
	get_table_context().serialize_conditionalFormatting	(current_sheet().conditionalFormatting());
    get_table_context().serialize_tableParts			(current_sheet().tableParts(), current_sheet().sheet_rels());
    get_table_context().serialize_autofilter			(current_sheet().autofilter());
    get_table_context().serialize_sort					(current_sheet().sort());
    get_table_context().serialize_merge_cells			(current_sheet().mergeCells());
	get_table_context().serialize_data_validation		(current_sheet().dataValidations());
	get_table_context().serialize_data_validation_x14	(current_sheet().dataValidationsX14());
 	get_table_context().serialize_protection			(current_sheet().protection());
    get_table_context().serialize_breaks				(current_sheet().breaks());
   
	get_drawing_context().set_odf_packet_path			(root()->get_folder());
    get_drawing_context().process_objects				(get_table_metrics());
	
	get_table_context().serialize_hyperlinks			(current_sheet().hyperlinks());
    get_table_context().serialize_ole_objects			(current_sheet().ole_objects());
    get_table_context().serialize_controls				(current_sheet().controls());

	get_table_context().dump_rels_hyperlinks			(current_sheet().sheet_rels());
	get_table_context().dump_rels_ole_objects			(current_sheet().sheet_rels());

	typedef std::multimap<std::wstring, int> _mapPivotsTableView;
	std::pair<_mapPivotsTableView::iterator, _mapPivotsTableView::iterator> range;

	range = mapPivotsTableView_.equal_range(current_sheet().name());

	for (_mapPivotsTableView::iterator it = range.first; it != range.second; ++it)
	{
		current_sheet().sheet_rels().add(oox::relationship(L"pvId" + std::to_wstring(it->second),
			L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable",
			L"../pivotTables/pivotTable" + std::to_wstring(it->second) + L".xml"));
	}

	if (false == get_drawing_context().empty())
    {
        std::wstringstream strm;
        get_drawing_context().serialize(strm);
        
        const std::pair<std::wstring, std::wstring> drawingName 
            = drawing_context_handle_->add_drawing_xml(strm.str(), get_drawing_context().get_drawings() );

        current_sheet().set_drawing_link(drawingName.first, drawingName.second);
	}
	if (false == get_drawing_context().vml_empty())
    {
        std::wstringstream strm;
        get_drawing_context().serialize_vml(strm);
        
        const std::pair<std::wstring, std::wstring> vml_drawingName 
            = drawing_context_handle_->add_drawing_vml(strm.str(), get_drawing_context().get_drawings() );

        current_sheet().set_vml_drawing_link(vml_drawingName.first, vml_drawingName.second);
	}
	//get_table_context().serialize_background (current_sheet().picture());

	if (false == get_comments_context().empty())
    {
        std::wstringstream strm;
        get_comments_context().serialize(strm);
	
		const std::pair<std::wstring, std::wstring> commentsName 
            = xlsx_comments_context_handle_.add_comments_xml(strm.str(), get_comments_context().get_comments() );

        current_sheet().set_comments_link(commentsName.first, commentsName.second);
    }    
    get_table_context().end_table();
}
//int xlsx_conversion_context::add_external_link(const std::wstring & external)
//{
//	std::unordered_map<std::wstring, int>::iterator pFind = mapExternalLink_.find(external);
//	if ( pFind == mapExternalLink_.end())
//	{
//		int id = (int)mapExternalLink_.size() + 1;
//		mapExternalLink_.insert(std::make_pair(external, id));
//		return id;
//	}
//	else
//	{
//		return pFind->second;
//	}
//}
void xlsx_conversion_context::add_control_props(const std::wstring & rid, const std::wstring & target, const std::wstring & props)
{
	if (rid.empty()) return;
	if (props.empty()) return;

	control_props_.insert(std::make_pair(target, props));

	current_sheet().sheet_rels().add(oox::relationship(rid,
			L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/ctrlProp",
			L"../ctrlProps/" + target));
}

void xlsx_conversion_context::start_table_column(unsigned int repeated, const std::wstring & defaultCellStyleName, int & cMin, int & cMax)
{
    cMin = get_table_context().columns_count();
    get_table_context().start_column(repeated, defaultCellStyleName);
    cMax = get_table_context().columns_count();
}

void xlsx_conversion_context::end_table_column()
{
}


int xlsx_conversion_context::current_table_column()
{
    return xlsx_table_context_.current_column();
}

int xlsx_conversion_context::current_table_row()
{
    return xlsx_table_context_.current_row();
}

std::wstring xlsx_conversion_context::current_cell_address()
{
	int col = current_table_column(); 
	int row = current_table_row();

	return oox::getCellAddress(col < 0 ? 0 : col, row < 0 ? 0 : row); //under covered cell
}

void xlsx_conversion_context::start_office_spreadsheet(const odf_reader::office_element * elm)
{
    spreadsheet_ = elm;
}
void xlsx_conversion_context::set_table_structure_protected(bool val)
{
	table_structure_protected_ = val;
}
const odf_reader::office_element * xlsx_conversion_context::get_spreadsheet()
{
    return spreadsheet_;
}

void xlsx_conversion_context::end_office_spreadsheet()
{}

void xlsx_conversion_context::start_paragraph(const std::wstring & styleName)
{
    xlsx_text_context_.start_paragraph(styleName);
}

void xlsx_conversion_context::end_paragraph()
{
    xlsx_text_context_.end_paragraph();
}

void xlsx_conversion_context::start_span(const std::wstring & styleName)
{
    xlsx_text_context_.start_span(styleName);
}

void xlsx_conversion_context::end_span()
{
    xlsx_text_context_.end_span();
}

void xlsx_conversion_context::start_table_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned)
{
    get_table_context().start_cell(formula, columnsSpanned, rowsSpanned);
}

bool xlsx_conversion_context::in_table_cell()
{
	return get_table_context().state()->in_cell;
}

void xlsx_conversion_context::end_table_cell()
{
    get_table_context().end_cell();
}

void xlsx_conversion_context::start_table_covered_cell()
{
    get_table_context().start_covered_cell();
}

void xlsx_conversion_context::end_table_covered_cell()
{
    get_table_context().end_covered_cell();
}

void xlsx_conversion_context::set_current_cell_style_id(unsigned int xfId)
{
    return get_table_context().set_current_cell_style_id(xfId);
}

int xlsx_conversion_context::get_current_cell_style_id()
{
    return get_table_context().get_current_cell_style_id();
}
int xlsx_conversion_context::get_dxfId_style(const std::wstring &style_name)
{
	if (style_name.empty()) return -1;

	int dxfId = -1;
	odf_reader::style_instance * instStyle =
		root()->odf_context().styleContainer().style_by_name(style_name, odf_types::style_family::TableCell, false);
	
	if (!instStyle)
		instStyle = root()->odf_context().styleContainer().style_by_display_name(style_name, odf_types::style_family::TableCell, false);
	
	if (instStyle)
	{
		odf_reader::text_format_properties_content_ptr	textFormats = calc_text_properties_content(instStyle);
		odf_reader::graphic_format_properties			graphicFormats = calc_graphic_properties_content(instStyle);
		odf_reader::style_table_cell_properties_attlist	cellFormats = calc_table_cell_properties(instStyle);

		dxfId = get_style_manager().dxfId(textFormats, &graphicFormats, &cellFormats);
	}
	return dxfId;
}
std::pair<double, double> xlsx_conversion_context::getMaxDigitSize()
{
    if (maxDigitSize_.first <= 0.1)
    {
		std::wstring font_name;
		int font_size = 10;

		std::vector<const odf_reader::style_instance *> instances;
		
		odf_reader::odf_read_context & odfContext = root()->odf_context();
		
		odf_reader::style_instance *inst = odfContext.styleContainer().style_default_by_type(odf_types::style_family::TableCell);
		if (inst) instances.push_back(inst);
		
		inst = odfContext.styleContainer().style_by_name(L"Default", odf_types::style_family::TableCell, false);
		if (inst) instances.push_back(inst);
		else
		{
			inst = odfContext.styleContainer().style_by_name(L"Normal", odf_types::style_family::TableCell, false);
			if (inst) instances.push_back(inst);
		}

		odf_reader::text_format_properties_content_ptr textFormatProperties	= calc_text_properties_content(instances);

		if (textFormatProperties)
		{
			if (textFormatProperties->fo_font_family_)
				font_name = textFormatProperties->fo_font_family_.get();
			else
			{
				std::wstring style_font_name;
				
				if (textFormatProperties->style_font_name_)					style_font_name = textFormatProperties->style_font_name_.get();
				else if (textFormatProperties->style_font_name_complex_)	style_font_name = textFormatProperties->style_font_name_complex_.get();
				else if (textFormatProperties->style_font_name_asian_)		style_font_name = textFormatProperties->style_font_name_asian_.get();
				
				odf_reader::fonts_container & fonts = odf_document_->odf_context().fontContainer();
				odf_reader::font_instance * font = fonts.font_by_style_name(style_font_name);
				if (font)
				{
					font_name = font->name();
				}
			}
			if ((textFormatProperties->fo_font_size_) && (textFormatProperties->fo_font_size_->get_type() == odf_types::font_size::Length))
				font_size = (int)(0.5 + textFormatProperties->fo_font_size_->get_length().get_value_unit(odf_types::length::pt));
		}
		if (font_name.empty()) font_name = L"Arial";
	
        maxDigitSize_ = utils::GetMaxDigitSizePixels(font_name.c_str(), font_size, 96., 0, mediaitems_->applicationFonts());
    }    
    return maxDigitSize_;
}

void xlsx_conversion_context::process_styles()
{
}

xlsx_table_metrics & xlsx_conversion_context::get_table_metrics()
{
    return get_table_context().get_table_metrics();
}

xlsx_drawing_context & xlsx_conversion_context::get_drawing_context()
{
    return get_table_context().get_drawing_context();
}
xlsx_conditionalFormatting_context	& xlsx_conversion_context::get_conditionalFormatting_context()
{
	return get_table_context().state()->get_conditionalFormatting_context();
}
xlsx_drawing_context_handle_ptr & xlsx_conversion_context::get_drawing_context_handle()
{
    return drawing_context_handle_;
}
xlsx_comments_context & xlsx_conversion_context::get_comments_context()
{
    return get_table_context().get_comments_context();
}

xlsx_comments_context_handle & xlsx_conversion_context::get_comments_context_handle()
{
    return xlsx_comments_context_handle_;
}

void xlsx_conversion_context::table_column_last_width(double w)
{
    return get_table_context().table_column_last_width(w);
}

double xlsx_conversion_context::table_column_last_width()
{
    return get_table_context().table_column_last_width();
}

void xlsx_conversion_context::start_hyperlink(const std::wstring & styleName)
{	
	xlsx_text_context_.start_hyperlink();
	xlsx_text_context_.start_span(styleName);
	
	get_table_context().start_hyperlink();
}

void xlsx_conversion_context::end_hyperlink(std::wstring const & href)
{
	if (xlsx_text_context_.is_drawing_context() == false)
	{
		std::wstring content = xlsx_text_context_.end_span2();
		xlsx_text_context_.end_hyperlink(get_table_context().end_hyperlink(current_cell_address(), href, L""));
	}
	else
	{
		std::wstring hId = get_drawing_context().add_hyperlink(href);
		xlsx_text_context_.end_hyperlink(hId); 
		
		xlsx_text_context_.end_span2();
	}
}
void xlsx_conversion_context::add_pivot_sheet_source (const std::wstring & sheet_name, int index_table_view)
{//ващето в либре жесткая привязка что на одном листе тока одна сводная может быть ..
	mapPivotsTableView_.insert(std::make_pair(sheet_name, index_table_view));
}
void xlsx_conversion_context::add_jsaProject(const std::string &content)
{
	if (content.empty()) return;
	
	output_document_->get_xl_files().add_jsaProject(content);
	output_document_->get_content_types_file().add_or_find_default(L"bin");
}

}
}

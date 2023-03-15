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

#include "xlsx_tablecontext.h"
#include "xlsx_textcontext.h"
#include "xlsxconversioncontext.h"
#include "xlsx_table_state.h"
#include "xlsx_utils.h"

#include "../Format/odf_document.h"
#include "../../Formulas/formulasconvert.h"
#include "../../Common/logging.h"

#include <iostream>

#include <xml/simple_xml_writer.h>

#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore {
namespace oox {

//-----------------------------------------------------------------------------------------------------------------------

xlsx_table_state_ptr xlsx_table_context::state()
{
	if (!xlsx_table_states_.empty())
		return xlsx_table_states_.back();
	else 
		return xlsx_table_state_ptr();
}

bool xlsx_table_context::start_database_range(const std::wstring & name, const std::wstring & ref, bool bNamedRangeOnly)
{	
	formulasconvert::odf2oox_converter convert;	
	std::wstring oox_ref = convert.convert_named_ref(ref);

	std::wstring ref1, ref2;
	size_t pos = oox_ref.find(L":");

	std::wstring xlsx_table_name;

	if (pos != std::wstring::npos)
	{
		ref1 = oox_ref.substr(0, pos );
		ref2 = oox_ref.substr(pos + 1);
		
		pos = ref1.find(L"!");							
		if (pos > 0)
		{
			xlsx_table_name = ref1.substr(0, pos);
			ref1 = ref1.substr(pos + 1);
		}
		
		pos = ref2.find(L"!");
		if (pos > 0) ref2 = ref2.substr(pos + 1);
	
		size_t col1, col2, row1, row2;
		
		XmlUtils::replace_all( xlsx_table_name, L"'", L"");

		bool res1 = getCellAddressInv(ref1, col1, row1);
		bool res2 = getCellAddressInv(ref2, col2, row2);

		if (!res1) 
			return false;
		
		if (!res2)
		{
			ref2 = ref1; col2 = col1; row2 = row1;
		}
		
		xlsx_data_ranges_.push_back(xlsx_data_range_ptr(new xlsx_data_range()));
		
		if (/*name.find(L"__Anonymous_Sheet_DB__") != std::wstring::npos ||*/ col1 == col2 || bNamedRangeOnly)
		{//check range in pivots
			xlsx_data_ranges_.back()->bTablePart = false;
		}
		xlsx_data_ranges_.back()->name = name;
		xlsx_data_ranges_.back()->table_name = xlsx_table_name;
		xlsx_data_ranges_.back()->ref = ref1 + L":" + ref2;
		xlsx_data_ranges_.back()->cell_start = std::pair<int, int>(col1, row1);
		xlsx_data_ranges_.back()->cell_end = std::pair<int, int>(col2, row2);

		xlsx_data_ranges_.back()->set_header(row1, col1, col2);
	}
//-----------------------------------------------------------------------
	if (!xlsx_table_name.empty())
	{
		if (xlsx_data_ranges_.back()->bTablePart)
		{
			std::pair<std::multimap<std::wstring, int>::iterator, std::multimap<std::wstring, int>::iterator> range = xlsx_data_ranges_map_.equal_range(xlsx_table_name);

			for (std::multimap<std::wstring, int>::iterator it = range.first; it != range.second; ++it)
			{
				if (xlsx_data_ranges_[it->second]->bTablePart)
				{
					if (std::wstring::npos != xlsx_data_ranges_[it->second]->name.find(L"__Anonymous_Sheet_DB__"))
						xlsx_data_ranges_[it->second]->bTablePart = false;
					else
						xlsx_data_ranges_.back()->bTablePart = false;
					break;
				}

			}
		}
		//-----------------------------------------------------------------------
		xlsx_data_ranges_map_.insert(std::pair<std::wstring, int> (xlsx_table_name, xlsx_data_ranges_.size() - 1));
	}
	return true;
}
void xlsx_table_context::set_database_orientation (bool val)
{
	if (xlsx_data_ranges_.empty()) return;

	xlsx_data_ranges_.back()->byRow = val;
}
void xlsx_table_context::set_database_header (bool val)
{
	if (xlsx_data_ranges_.empty()) return;

	xlsx_data_ranges_.back()->withHeader = val;
}
void xlsx_table_context::set_database_filter (bool val)
{
	if (xlsx_data_ranges_.empty()) return;

	xlsx_data_ranges_.back()->filter = val;
}
void xlsx_table_context::end_database_range()
{
	if (!xlsx_data_ranges_.back()->bTablePart && !xlsx_data_ranges_.back()->filter && !xlsx_data_ranges_.back()->table_name.empty())
	{
		xlsx_conversion_context_->get_xlsx_defined_names().add(xlsx_data_ranges_.back()->name, 
			xlsx_data_ranges_.back()->table_name + L"!" + xlsx_data_ranges_.back()->ref, false, -1);
	}
}

void xlsx_table_context::set_database_range_value(int index, const std::wstring& value)
{
	if (index < 0 || index > (int)xlsx_data_ranges_.size()) return;

	size_t col = state()->current_column();
	size_t row = state()->current_row();

	xlsx_data_ranges_[index]->set_header_value(col, row, value);
}
void xlsx_table_context::check_database_range_intersection(const std::wstring& table_name, const std::wstring& ref)
{
	std::wstring ref1, ref2;
	size_t col_1, row_1, col_2, row_2;

	size_t pos = ref.find(L":");
	if (pos != std::wstring::npos)
	{
		ref1 = ref.substr(0, pos );
		ref2 = ref.substr(pos + 1);
	}
	if (false == getCellAddressInv(ref1, col_1, row_1) ||
		false == getCellAddressInv(ref2, col_2, row_2)) return;
		
	for (size_t i = 0; i < xlsx_data_ranges_.size(); i++)
	{
		if (xlsx_data_ranges_[i]->table_name != table_name) continue;
		
		//if	( xlsx_data_ranges_[i]->cell_start.second < row_2 || xlsx_data_ranges_[i]->cell_end.second > row_1 
		//	|| xlsx_data_ranges_[i]->cell_end.first < col_1 || xlsx_data_ranges_[i]->cell_start.first > col_2 )

		if	(((col_1 <= xlsx_data_ranges_[i]->cell_start.first && xlsx_data_ranges_[i]->cell_start.first <= col_2) || 
			(xlsx_data_ranges_[i]->cell_start.first <= col_1 && col_1 <= xlsx_data_ranges_[i]->cell_end.first)) 
			&& 
			(( row_1 <= xlsx_data_ranges_[i]->cell_start.second && xlsx_data_ranges_[i]->cell_start.second <= row_2) ||
			(xlsx_data_ranges_[i]->cell_start.second <= row_1 && row_1 <= xlsx_data_ranges_[i]->cell_end.second )))
		{
			xlsx_data_ranges_[i]->bTablePart = false;
		}
	}
}
int xlsx_table_context::in_database_range()
{
	int col = state()->current_column();
	int row = state()->current_row();

	for (size_t i = 0; i < xlsx_data_ranges_.size(); i++)
	{
		if (xlsx_data_ranges_[i]->table_name != state()->get_table_name()) continue;

		if (/*(xlsx_data_ranges_values_[i]->withHeader || xlsx_data_ranges_values_[i]->filter)&& */
			xlsx_data_ranges_[i]->in_header(col, row)) 
		{
			return (int)i;
		}
	}
	return -1;
}
void xlsx_table_context::add_database_sort(int field_number, int order)
{
	xlsx_data_ranges_.back()->bySort.push_back(std::pair<int, bool>(field_number, order == 1 ? false : true ));
}


xlsx_table_context::
xlsx_table_context(xlsx_conversion_context * Context, xlsx_text_context & textContext): xlsx_conversion_context_(Context),
xlsx_text_context_(textContext)
{        
}

void xlsx_table_context::start_table(const std::wstring & tableName, const std::wstring & tableStyleName, int id)
{
	xlsx_table_state_ptr  state = boost::make_shared<xlsx_table_state>(xlsx_conversion_context_, tableStyleName, tableName, id);
    xlsx_table_states_.push_back( state);
}
void xlsx_table_context::set_protection(bool val, const std::wstring &key, const std::wstring &algorithm)
{
	xlsx_table_states_.back()->set_protection(val, key, algorithm);
}
void xlsx_table_context::end_table()
{
	xlsx_conversion_context_->get_dataValidations_context().clear();
}	
void xlsx_table_context::start_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned)
{
    state()->start_cell(columnsSpanned, rowsSpanned);    
}

void xlsx_table_context::end_cell()
{    
    state()->end_cell();
}

void xlsx_table_context::set_current_cell_style_id(unsigned int xfId)
{
    return state()->set_current_cell_style_id(xfId);
}

int xlsx_table_context::get_current_cell_style_id()
{
    return state()->get_current_cell_style_id();
}

void xlsx_table_context::start_cell_content()
{
    xlsx_text_context_.start_cell_content();
}

int xlsx_table_context::end_cell_content(bool need_cache)
{
    return xlsx_text_context_.end_cell_content(need_cache);
}

void xlsx_table_context::start_covered_cell()
{
    return state()->start_covered_cell();
}

void xlsx_table_context::end_covered_cell()
{
    return state()->end_covered_cell();
}

void xlsx_table_context::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    return state()->start_column(repeated, defaultCellStyleName);
}

unsigned int xlsx_table_context::columns_count()
{
    return state()->columns_count();    
}

void xlsx_table_context::set_header_page(_CP_OPT(double) val)
{
	state()->header_page = val;
}
_CP_OPT(double) xlsx_table_context::get_header_page()
{
	return state()->header_page;
}
void xlsx_table_context::set_footer_page(_CP_OPT(double) val)
{
	state()->footer_page = val;
}
_CP_OPT(double) xlsx_table_context::get_footer_page()
{
	return state()->footer_page;
}

std::wstring xlsx_table_context::default_row_cell_style()
{
    return state()->default_row_cell_style();
}

std::wstring xlsx_table_context::default_column_cell_style()
{
    return state()->default_column_cell_style();
}

int xlsx_table_context::current_column()
{
    return state()->current_column();
}

int xlsx_table_context::current_row()
{
    return state()->current_row();
}


void xlsx_table_context::serialize_sort(std::wostream & _Wostream)
{
	if (xlsx_data_ranges_.empty()) return;

	std::pair<std::multimap<std::wstring, int>::iterator, std::multimap<std::wstring, int>::iterator> range;

	range = xlsx_data_ranges_map_.equal_range(state()->tableName_);

	for (std::multimap<std::wstring, int>::iterator it = range.first; it != range.second; ++it)
	{
		if (xlsx_data_ranges_[it->second]->bTablePart) continue;

		xlsx_data_ranges_[it->second]->serialize_sort(_Wostream);
	}
}
void xlsx_table_context::serialize_tableParts(std::wostream & _Wostream, rels & Rels)
{
	if (xlsx_data_ranges_.empty()) return;

	std::pair<std::multimap<std::wstring, int>::iterator, std::multimap<std::wstring, int>::iterator> range;

	range = xlsx_data_ranges_map_.equal_range(state()->get_table_name());

	for (std::multimap<std::wstring, int>::iterator it = range.first; it != range.second; ++it)
	{
		if (false == xlsx_data_ranges_[it->second]->bTablePart) continue;

// из за дебелизма мсофис которому ОБЯЗАТЕЛЬНО нужно прописывать имена колонок таблицы (и они должны быть еще 
// прописаны и в самих данных таблицы !!

		while (xlsx_data_ranges_[it->second]->header_values.size() > xlsx_data_ranges_[it->second]->cell_end.first - 
											xlsx_data_ranges_[it->second]->cell_start.first + 1)
		{
			xlsx_data_ranges_[it->second]->header_values.pop_back();
		}
		int i = xlsx_data_ranges_[it->second]->header_values.size() - 1;
		for (; i >= 0; i--)
		{
			if (false == xlsx_data_ranges_[it->second]->header_values[i].empty())
			{
				break;
			}
		}

		if (i == -1)
		{
			xlsx_data_ranges_[it->second]->bTablePart = false;
			continue;
		}
		else
		{
			size_t erase = xlsx_data_ranges_[it->second]->header_values.size() - 1 - i;
			if (erase > 0)
			{
				xlsx_data_ranges_[it->second]->header_values.erase(xlsx_data_ranges_[it->second]->header_values.begin() + i + 1, xlsx_data_ranges_[it->second]->header_values.end());
				xlsx_data_ranges_[it->second]->cell_end.first -= erase;

				std::wstring ref1 = getCellAddress(xlsx_data_ranges_[it->second]->cell_start.first, xlsx_data_ranges_[it->second]->cell_start.second);
				std::wstring ref2 = getCellAddress(xlsx_data_ranges_[it->second]->cell_end.first, xlsx_data_ranges_[it->second]->cell_end.second);
				
				xlsx_data_ranges_[it->second]->ref = ref1 + L":" + ref2;
			}
		}
		//--------------------------------------------------------

		size_t id = xlsx_conversion_context_->get_table_parts_size() + 1;

		std::wstring rId = L"tprtId" + std::to_wstring(id);
		std::wstring ref = L"../tables/table" + std::to_wstring(id) + L".xml";
		
		CP_XML_WRITER(_Wostream)
		{
			CP_XML_NODE(L"tablePart")
			{
				CP_XML_ATTR(L"r:id", rId);
			}
		}
		Rels.add( relationship(rId, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/table", ref));
//--------------------------------------------------------
		std::wstringstream strm;
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"table")
			{
				CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");

				CP_XML_ATTR(L"id", id);
				CP_XML_ATTR(L"name", xlsx_data_ranges_[it->second]->name);
				CP_XML_ATTR(L"displayName", xlsx_data_ranges_[it->second]->name);
				CP_XML_ATTR(L"ref", xlsx_data_ranges_[it->second]->ref);
				
				if (xlsx_data_ranges_[it->second]->withHeader == false && 
					xlsx_data_ranges_[it->second]->filter == false)
					CP_XML_ATTR(L"headerRowCount", 0);

				//CP_XML_ATTR(L"totalsRowCount", 0);
				CP_XML_ATTR(L"totalsRowShown", 0);

				xlsx_data_ranges_[it->second]->serialize_autofilter(CP_XML_STREAM());
				xlsx_data_ranges_[it->second]->serialize_sort(CP_XML_STREAM());

				CP_XML_NODE(L"tableColumns")
				{
					CP_XML_ATTR(L"count",	xlsx_data_ranges_[it->second]->cell_end.first - 
											xlsx_data_ranges_[it->second]->cell_start.first + 1);
					
					for (int id = 0, i = xlsx_data_ranges_[it->second]->cell_start.first; i <= xlsx_data_ranges_[it->second]->cell_end.first; i++, id++)
					{
						CP_XML_NODE(L"tableColumn")
						{
							std::wstring column_name = xlsx_data_ranges_[it->second]->header_values[id];
							if (column_name.empty())
							{
								column_name = L"Column_" + std::to_wstring(id + 1); 
							}
							CP_XML_ATTR(L"id", id + 1);
							CP_XML_ATTR(L"name", column_name);
						}
					}
				}
				CP_XML_NODE(L"tableStyleInfo")
				{
					CP_XML_ATTR(L"showFirstColumn", 0);
					CP_XML_ATTR(L"showLastColumn", 0);
					CP_XML_ATTR(L"showRowStripes", 1);
					CP_XML_ATTR(L"showColumnStripes", 0);				
				}
			}
		}
		xlsx_conversion_context_->add_table_part(strm.str());
	}
}
void xlsx_table_context::serialize_autofilter(std::wostream & _Wostream)
{
	if (xlsx_data_ranges_.empty()) return;

	std::wstring ref;

	std::pair<int, int> cell_start	(-1, -1);
	std::pair<int, int> cell_end	(-1, -1);

	std::pair<std::multimap<std::wstring, int>::iterator, std::multimap<std::wstring, int>::iterator> range;

	range = xlsx_data_ranges_map_.equal_range(state()->tableName_);

	for (std::multimap<std::wstring, int>::iterator it = range.first; it != range.second; ++it)
	{
		if (xlsx_data_ranges_[it->second]->bTablePart) continue;
		if (!xlsx_data_ranges_[it->second]->filter) continue;

		if (cell_start.first < 0 || xlsx_data_ranges_[it->second]->cell_start.first < cell_start.first )
			cell_start.first = xlsx_data_ranges_[it->second]->cell_start.first;

		if (cell_start.second < 0 || xlsx_data_ranges_[it->second]->cell_start.second < cell_start.second)
			cell_start.second = xlsx_data_ranges_[it->second]->cell_start.second;

		if (cell_end.first < 0 || xlsx_data_ranges_[it->second]->cell_end.first > cell_end.first)
			cell_end.first = xlsx_data_ranges_[it->second]->cell_end.first;

		if (cell_end.second < 0 || xlsx_data_ranges_[it->second]->cell_end.second > cell_end.second)
			cell_end.second = xlsx_data_ranges_[it->second]->cell_end.second;

		ref = xlsx_data_ranges_[it->second]->ref + L";";
	}
	if (cell_end.first < 0  || cell_start.first < 0) return;

	ref.erase(ref.size() - 1, 1);

	CP_XML_WRITER(_Wostream)
	{			
		CP_XML_NODE(L"autoFilter")
		{
			//в автофильтре тока простые диапазоны .. для сложных - tablePart
			CP_XML_ATTR(L"ref", getCellAddress(cell_start.first, cell_start.second) + L":" + getCellAddress(cell_end.first, cell_end.second));
			//CP_XML_ATTR(L"ref", ref);
		}
	}
}
void xlsx_table_context::serialize_protection(std::wostream & _Wostream)
{
    return state()->serialize_protection(_Wostream);
}
void xlsx_table_context::serialize_conditionalFormatting(std::wostream & _Wostream)
{
    return state()->serialize_conditionalFormatting(_Wostream);
}
void xlsx_table_context::serialize_merge_cells(std::wostream & _Wostream)
{
    return state()->serialize_merge_cells(_Wostream);
}
void xlsx_table_context::serialize_table_format(std::wostream & _Wostream)
{
    return state()->serialize_table_format(_Wostream);
}
void xlsx_table_context::serialize_page_properties(std::wostream & _Wostream)
{
    return state()->serialize_page_properties(_Wostream);
}
void xlsx_table_context::serialize_header_footer(std::wostream & _Wostream)
{
    return state()->serialize_header_footer(_Wostream);
}
void xlsx_table_context::serialize_background(std::wostream & _Wostream)
{
    return state()->serialize_background(_Wostream);
}
void xlsx_table_context::serialize_data_validation(std::wostream & _Wostream)
{
    return xlsx_conversion_context_->get_dataValidations_context().serialize(_Wostream);
}
void xlsx_table_context::serialize_data_validation_x14(std::wostream & _Wostream)
{
	return xlsx_conversion_context_->get_dataValidations_context().serialize_x14(_Wostream);
}
void xlsx_table_context::serialize_hyperlinks(std::wostream & _Wostream)
{
    return state()->serialize_hyperlinks(_Wostream);
}
void xlsx_table_context::serialize_ole_objects(std::wostream & _Wostream)
{
    return state()->serialize_ole_objects(_Wostream);
}
void xlsx_table_context::serialize_controls(std::wostream & _Wostream)
{
    return state()->serialize_controls(_Wostream);
}
void xlsx_table_context::serialize_breaks(std::wostream & _Wostream)
{
    return state()->serialize_breaks(_Wostream);
}
void xlsx_table_context::dump_rels_hyperlinks(rels & Rels)
{
	return state()->dump_rels_hyperlinks(Rels);
}
void xlsx_table_context::dump_rels_ole_objects(rels & Rels)
{
	return state()->dump_rels_ole_objects(Rels);
}
xlsx_table_metrics & xlsx_table_context::get_table_metrics()
{
    return state()->get_table_metrics();
}

xlsx_drawing_context & xlsx_table_context::get_drawing_context()
{
    return state()->get_drawing_context();
}

xlsx_comments_context & xlsx_table_context::get_comments_context()
{
    return state()->get_comments_context();
}
void xlsx_table_context::table_column_last_width(double w)
{
    return state()->table_column_last_width(w);
}

double xlsx_table_context::table_column_last_width()
{
    return state()->table_column_last_width();
}

void xlsx_table_context::start_hyperlink()
{
    return state()->start_hyperlink();
}

 std::wstring xlsx_table_context::end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display)
{
    return state()->end_hyperlink(ref, href, display);
}


}
}

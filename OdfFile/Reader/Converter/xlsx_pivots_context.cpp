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
#include "xlsx_pivots_context.h"
#include "xlsx_utils.h"

#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <xml/simple_xml_writer.h>
#include "../../../OOXML/Base/Unit.h"
#include "../../../OOXML/Base/Base.h"

#include <map>

namespace cpdoccore {
namespace oox {

class xlsx_pivots_context::Impl
{
public:
	struct _pivot_xml
	{	
		std::wstring	name;
		std::wstring	sheet;
		std::wstring	definitionsData_;	//cacheData
		std::wstring	recordsData_;		//cacheRecorda

		std::wstring	viewData_;			//tableView
	};

	Impl() : connection_count(0) {}
    
	std::vector<_pivot_xml>		pivot_xmls_;
	std::wstring				connections_;
	int							connection_count;

	struct _field_value
	{
		_field_value(const std::wstring &val, const std::wstring &type, const bool &sd) : sVal(val), sNode(type), show_details(sd) {}
		std::wstring	sVal;
		std::wstring	sNode;
		bool			show_details;
	};
	struct _reference
	{
		std::wstring	name;
		int				type = -1;
		std::wstring	member_name;
		int				member_type = -1;
	};
	struct _group
	{
		std::wstring				name;
		std::vector<_field_value>	caches;
	};
	struct _field
	{
		std::wstring				name;
		bool						name_enabled = false;
		std::wstring				display_name;
		int							type = -1;
		int							hierarchy = -1;
		int							function = -1;
		std::wstring				user_function;
		bool						data_layout = false;
		bool						show_empty = false;
		bool						repeat_item_labels = true;
		int							sort = 0;
		
		std::vector<int>			subtotals;
		
		std::vector<_field_value>	caches;
		std::vector<_reference>		references;
		int							references_field = -1;
		bool						used_in_referenes = false;
		
		int							type_groups = 0;
		int							base_group = -1;
		std::wstring				source_groups;
		std::vector<_group>			groups;
		std::vector<int>			groups_discrete;

		bool						bDataFieldEnabled = false;
		bool						bString = false;
		bool						bDate = false;
		bool						bNumber = false;
		bool						bEmpty = false;
		bool						bInteger = false;
		bool						bBool = false;
	};
	struct _desc
	{
		void clear()
		{
			name.clear();
			location_ref.clear();
			location_table_name.clear();
			
			source_ref.clear();
			source_table_name.clear();
			source_database.clear();
			source_database_query.clear();
			source_database_sql.clear();
			
			fields.clear();
			row_fields.clear();
			page_fields.clear();
			col_fields.clear();
			data_fields.clear();
			headers.clear();

			fields_count = 0;
			data_on_row = false;
			identify_categories = false;
			drill_enabled = true;
			grand_total = -1;
			firstDataRow = 0;
			firstDataCol = 0;
			bAxisRow = true;
			bAxisCol = true;
		}
		std::wstring				name;
		std::wstring				location_ref;
		std::wstring				location_table_name;
		
		std::wstring				source_ref;
		std::wstring				source_database;
		std::wstring				source_table_name;
		std::wstring				source_database_query;
		std::wstring				source_database_sql;

		std::vector<std::wstring>	headers;
		int							fields_count = 0;
		std::vector<_field>			fields;
		std::vector<int>			row_fields;
		std::vector<int>			page_fields;
		std::vector<int>			col_fields;
		std::vector<int>			data_fields;
		
		int							grand_total = -1;
		bool						identify_categories = false;
		bool						drill_enabled = true;
		bool						ignore_empty_rows = false;

		bool						data_on_row = false;
		bool						in_group = false;
//-----------------------------------------------------------------------------
		int							firstDataRow = 0;
		int							firstDataCol = 0;
		bool						bAxisRow = true;
		bool						bAxisCol = true;
	}current_;

	void calc_headers();
	void sort_fields();

	void serialize_view(std::wostream & strm);
	void serialize_cache(std::wostream & strm);

	void serialize_type_field(CP_ATTR_NODE, _field & field);

private:
	bool clear_header_map(std::map<size_t, size_t> & map)
	{//отсев по секонд - нужны тока повторы
		std::map<size_t, size_t> count;

		size_t max_size = 0;
		for (std::map<size_t, size_t>::iterator it = map.begin(); it != map.end(); ++it)
		{
			std::map<size_t, size_t>::iterator pFind = count.find(it->second);
			if (pFind != count.end())
			{
				pFind->second++;

				if ( pFind->second > max_size)
					max_size = pFind->second;
			}
			else
			{
				count.insert(std::make_pair(it->second, 1));
				if ( 1 > max_size)
					max_size = 1;
			}
		}
		if (max_size == 1) return false;

		size_t found = 0;

		for (std::map<size_t, size_t>::iterator it = count.begin() ; it != count.end(); ++it)
		{
			if (it->second == max_size)
			{
				found = it->first;
				break;
			}
		}
		//удалим все что не найденое
		std::map<size_t, size_t>::iterator it = map.begin();
		while( it != map.end())
		{
			if (it->second != found)
			{
				std::map<size_t, size_t>::iterator del = it; ++it;
				map.erase(del);
			}
			else ++it;
		}
		return true;
	}
	void clear_header_map2(std::map<size_t, size_t> & map, std::map<size_t, size_t> & map_by)
	{//отсев тех кто во втором
		for (std::map<size_t, size_t>::iterator it = map_by.begin() ; it != map_by.end(); ++it)
		{
			std::map<size_t, size_t>::iterator pFind = map.find(it->second);
			if (pFind != map.end())
			{
				if (pFind->second == it->first)
				{
					map.erase(pFind);
				}
			}
		}
	}
	void dump_connection()
	{
		std::wstringstream strm;
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"connection")
			{ 
				CP_XML_ATTR(L"id", connection_count);
				CP_XML_ATTR(L"name", L"Connection" + std::to_wstring(connection_count));
				CP_XML_ATTR(L"type", 5);//??
				CP_XML_ATTR(L"refreshedVersion", 3);
				CP_XML_ATTR(L"saveData", 1);
				CP_XML_NODE(L"dbPr")
				{ 
					CP_XML_ATTR(L"connection", current_.source_database);
					if (!current_.source_table_name.empty())
					{
						CP_XML_ATTR(L"command", current_.source_table_name);
						CP_XML_ATTR(L"commandType", 3);
					}
					else if (!current_.source_database_query.empty())
					{
						CP_XML_ATTR(L"command", current_.source_database_query);
						CP_XML_ATTR(L"commandType", 4); //???? 1, 5 ????
					}
					else if (!current_.source_database_sql.empty())
					{
						CP_XML_ATTR(L"command", current_.source_database_sql);
						CP_XML_ATTR(L"commandType", 2);
					}
				}
			}
		}
		connections_ += strm.str();
	}


};

xlsx_pivots_context::xlsx_pivots_context() : impl_(new xlsx_pivots_context::Impl())
{
}
void xlsx_pivots_context::Impl::calc_headers()
{
	if (current_.headers.empty()) return;

	std::map<size_t, size_t> mapRowHeader;
	std::map<size_t, size_t> mapColHeader;

	std::map<size_t, size_t>::iterator pFind;

	//current_.bAxisCol = false;
	//current_.bAxisRow = false;

	size_t min_col = 0xffffff, min_row = 0xffffff;
	size_t prev_col, prev_row;
	for (size_t i = 0; i < current_.headers.size(); i++)//("F18","F19","F23","G21","H21")
	{
		size_t row = 0, col = 0;
		if (false == oox::getCellAddressInv(current_.headers[i], col, row)) continue;

		//if (i > 0)
		//{
		//	if (col != prev_col)current_.bAxisCol = true;
		//	if (row != prev_row)current_.bAxisRow = true;
		//}
		prev_col = col;
		prev_row = row;

		if (col < min_col) min_col = col;
		if (row < min_row) min_row = row;

		pFind = mapRowHeader.find(row);
		if (pFind == mapRowHeader.end())
		{
			mapRowHeader.insert(std::make_pair(row, col));
		}
		else
		{
			if (pFind->second > col ) 
				pFind->second = col;
		}
		
		pFind = mapColHeader.find(col);
		if (pFind == mapColHeader.end())
		{
			mapColHeader.insert(std::make_pair(col, row));
		}
		else
		{
			if (pFind->second > row ) 
				pFind->second = row;
		}
	}
	bool resRow = clear_header_map(mapRowHeader);
	bool resCol = clear_header_map(mapColHeader);

	if (resRow == resCol && resCol == false)
	{
		current_.firstDataRow = current_.firstDataCol = 1; 
	}
	else if (resRow == false)
	{
		current_.bAxisCol = false;
		clear_header_map2(mapRowHeader, mapColHeader);
	}
	else if (resCol == false)
	{
		current_.bAxisRow = false;
		clear_header_map2(mapColHeader, mapRowHeader);
	}

	if (resRow || resCol)
	{
		current_.firstDataCol = mapRowHeader.empty() ? 1 : mapRowHeader.begin()->second - min_col;
		
		if (mapColHeader.empty())
		{
			pFind = mapRowHeader.end(); pFind--;
			min_row = pFind->first;
			current_.firstDataRow = 1;
		}
		else
		{
			min_row = mapColHeader.begin()->second;
			if (!mapRowHeader.empty() && current_.page_fields.empty())
			{
				if (min_row > mapRowHeader.begin()->first)
					min_row = mapRowHeader.begin()->first;
			}
			current_.firstDataRow = mapColHeader.begin()->second - min_row + 1;
		}
	}
	if (current_.firstDataCol < 1) current_.firstDataCol = 1;
	if (current_.firstDataRow < 1) current_.firstDataRow = 1;

	std::vector<std::wstring> split_ref;
	boost::algorithm::split(split_ref, current_.location_ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
	
	if (false == split_ref.empty())
	{
		size_t row = 0, col = 0;
		if (oox::getCellAddressInv(split_ref[0], col, row))
		{
			if (min_col != 0xffffff && min_row != 0xffffff)
			{
				col = min_col;
				row = min_row;
			}
			split_ref[0] = oox::getColAddress(col) + oox::getRowAddress(row);
		}
	}
	if (split_ref.size() > 1)
	{
		current_.location_ref = split_ref[0] + L":" + split_ref[1];
	}
}
void xlsx_pivots_context::Impl::sort_fields()
{
	size_t count_skip = 0;
	for (size_t i = 0; i < current_.fields.size() - count_skip; i++)
	{
		if (!current_.fields[i].source_groups.empty() && i != current_.fields.size() - count_skip)
		{
			current_.fields.push_back(current_.fields[i]);
			current_.fields.erase(current_.fields.begin() + i , current_.fields.begin() + i + 1);
			i--;
			count_skip++;
		}
	}
	count_skip = 0;
	for (size_t i = 0; i < current_.fields.size() - count_skip; i++)
	{
		if (current_.fields[i].name.empty() && i != current_.fields.size() - count_skip)
		{
			current_.fields.push_back(current_.fields[i]);
			current_.fields.erase(current_.fields.begin() + i , current_.fields.begin() + i + 1);
			i--;
			count_skip++;
		}
	}
	
	bool bAddRepeateRow = false;
	bool bAddRepeateCol = false;
	bool bShowEmptyCol	= true;
	bool bShowEmptyRow	= true;

	int count_items_col = -1, count_items_row = -1;
	
	int index_current = 0;
	for (size_t i = 0; i < current_.fields.size(); i++, index_current++)
	{
		if ( current_.fields[i].type_groups > 0 )
		{
			current_.fields[i].base_group = 0;

			int index_group = 0;
			for (size_t k = 0; k < current_.fields.size(); k++)
			{
				if (current_.fields[k].type == 7) continue;
				if (current_.fields[k].name == current_.fields[i].source_groups && !current_.fields[i].source_groups.empty())
				{
					if (current_.fields[k].type_groups == 0)
						current_.fields[k].base_group = index_current; //опорный (если он и базовый - не писать)
					current_.fields[i].base_group = index_group;

					for (size_t c = 0; !current_.fields[i].groups.empty() && c < current_.fields[k].caches.size(); c++)
					{
						for (size_t g = 0; g < current_.fields[i].groups.size(); g++)
						{
							for (size_t h = 0; h < current_.fields[i].groups[g].caches.size(); h++)
							{
								if (current_.fields[i].groups[g].caches[h].sVal == current_.fields[k].caches[c].sVal)
									current_.fields[i].groups_discrete.push_back(g);
							}
						}
					}
					break;
				}
				index_group++;
			}
		}
		if ( current_.fields[i].name.empty() &&
			!current_.fields[i].data_layout)
			continue;

		switch(current_.fields[i].type)
		{
			case 0:	// column
			{
				if (!current_.fields[i].name.empty())
				{
					current_.col_fields.push_back(i);	

					if ( count_items_col < 0)
					{
						count_items_col = current_.fields[i].caches.size();
					}
					else
					{
						if (count_items_col != current_.fields[i].caches.size())
							bAddRepeateCol = true;
					}	
					if (!current_.fields[i].show_empty || !current_.fields[i].repeat_item_labels )
						bShowEmptyCol = false;

				}
				else if (current_.fields[i].name_enabled)
				{
					count_items_col = 0;
					bAddRepeateCol = true; //add col axis
				}
			}break;
			case 1:	// data
			{
				int index_field = -1;
				
				for (size_t j = 0; j < i/*current_.fields.size()*/; j++)
				{
					if ( current_.fields[j].name == current_.fields[i].name )
					{
						current_.fields[j].bDataFieldEnabled = true;
						index_field = j;
						break;
					}
				}
				if (index_field >= 0)
				{
					if (current_.fields[i].caches.empty())
					{
						current_.fields[i].type = 7; //skip 
						current_.fields_count--; 
						index_current--;
					}
					
					current_.fields[i].references_field = index_field;
				}

				index_field = i;
				current_.fields[i].bDataFieldEnabled = true;
				current_.data_fields.push_back(index_field);	
			}break;
			case 2:	// hidden
			{
			}break;
			case 3:	// page
			{
				current_.page_fields.push_back(i);	
			}break;
			case 4:	// row
			{
				if (!current_.fields[i].name.empty())
				{
					current_.row_fields.push_back(i);	

					if ( count_items_row < 0)
					{
						count_items_row = current_.fields[i].caches.size();
					}
					else
					{
						if (count_items_row < (int)current_.fields[i].caches.size())
							bAddRepeateRow = true;
					}
				}
				else if (current_.fields[i].name_enabled)
				{
					count_items_row = 0;
					bAddRepeateRow = true; //add row axis
				}
				if (!current_.fields[i].show_empty)
					bShowEmptyRow = false;

				if (!current_.fields[i].repeat_item_labels)
					bAddRepeateCol = false;

				if (current_.fields[i].data_layout)
				{
					current_.data_on_row = true;

					bAddRepeateCol = false;

					if ((current_.fields[i].name.empty() && (current_.fields[i].hierarchy >= 0)) ||
						current_.fields[i].used_in_referenes )
					{
						bAddRepeateRow = true;
					}
				}

			}break;
		}

		if (current_.fields[i].name.empty())
		{
			current_.fields.erase(current_.fields.begin() + i, current_.fields.begin() + i + 1);
			current_.fields_count--; 
			i--;
		}
	}
	bool data_col_repeate = false;
	if ((bAddRepeateRow || (count_items_row == 0 && current_.bAxisRow)) && (current_.grand_total == 1 || current_.grand_total == 3 || current_.data_on_row))/* && bShowEmptyRow*/
	{
		current_.row_fields.push_back(-2);	
		data_col_repeate = true;
	}

	if (!data_col_repeate && (bAddRepeateCol || (count_items_col == 0 && current_.bAxisCol)) && (current_.grand_total == 1 || current_.grand_total == 2 || current_.data_fields.size() > 1 ))/* && bShowEmptyCol*/ ///* || (bEmptyColCache && current_.grand_total < 0)*/?? Financial Execution (template).ods
		current_.col_fields.push_back(-2);	
	

}
void xlsx_pivots_context::Impl::serialize_view(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotTableDefinition")
		{ 
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
			
			CP_XML_ATTR(L"name",				current_.name); 
			CP_XML_ATTR(L"cacheId",				pivot_xmls_.size()); 
			if (current_.data_on_row)
			{
				CP_XML_ATTR(L"dataOnRows",		true); 
			}
			CP_XML_ATTR(L"applyNumberFormats",	0);
			CP_XML_ATTR(L"applyBorderFormats",	0); 
			CP_XML_ATTR(L"applyFontFormats",	0);
			CP_XML_ATTR(L"applyPatternFormats",	0);
			CP_XML_ATTR(L"applyAlignmentFormats",	0);
			CP_XML_ATTR(L"applyWidthHeightFormats",	1);
			
			CP_XML_ATTR(L"dataCaption",		L"Values"); 
			if (!current_.drill_enabled)
			{
				CP_XML_ATTR(L"enableDrill",			0); 
			}
			CP_XML_ATTR(L"useAutoFormatting",		1); 

			//CP_XML_ATTR(L"asteriskTotals",		1); 
			//CP_XML_ATTR(L"showMemberPropertyTips",0);

			CP_XML_ATTR(L"itemPrintTitles",			1);  
			CP_XML_ATTR(L"indent",					0); 
			CP_XML_ATTR(L"compact",					0);  
			CP_XML_ATTR(L"compactData",				0); 

			CP_XML_NODE(L"location")
			{
				CP_XML_ATTR(L"ref", current_.location_ref);
				
				CP_XML_ATTR(L"firstHeaderRow", 1 );
				CP_XML_ATTR(L"firstDataRow", current_.firstDataRow);
				CP_XML_ATTR(L"firstDataCol", current_.firstDataCol);
				
				if (current_.page_fields.empty() == false)
				{
					CP_XML_ATTR(L"rowPageCount", current_.page_fields.size()); //????
					CP_XML_ATTR(L"colPageCount", 1);
				}
			}
			std::map<std::wstring, bool> used_field_name;
			CP_XML_NODE(L"pivotFields")
			{
				CP_XML_ATTR(L"count", current_.fields_count);
				for (size_t i = 0; i < current_.fields.size(); i++)
				{
					if  (current_.fields[i].type == 7)
						continue;
					
					if (used_field_name.end() != used_field_name.find(current_.fields[i].name))
						continue;

					used_field_name.insert(std::make_pair(current_.fields[i].name, true));
					
					CP_XML_NODE(L"pivotField")
					{					 
						switch(current_.fields[i].type)
						{
							case 0:	CP_XML_ATTR(L"axis", L"axisCol");	break;
							case 1:	break;// data,
							case 2:	break;// hidden,
							case 3:	CP_XML_ATTR(L"axis", L"axisPage");	break;
							case 4:	CP_XML_ATTR(L"axis", L"axisRow");	break;
						}
						CP_XML_ATTR(L"compact",					0);
						CP_XML_ATTR(L"outline",					0);
						if (current_.fields[i].bDataFieldEnabled)
						{
							CP_XML_ATTR(L"dataField",			1);
						}
						else 
						{
							CP_XML_ATTR(L"subtotalTop",			0);
							CP_XML_ATTR(L"showAll",				0);
						}
						CP_XML_ATTR(L"includeNewItemsInFilter", 1);
						if (current_.fields[i].type == 0 || current_.fields[i].type == 4)
						{
							if (current_.fields[i].sort == 1)	CP_XML_ATTR(L"sortType", L"ascending");
							if (current_.fields[i].sort == 2)	CP_XML_ATTR(L"sortType", L"descending");
						}
						CP_XML_ATTR(L"defaultSubtotal", 0);

						if (current_.fields[i].type != 2) 
						{
							if (!current_.fields[i].groups.empty())
							{
								CP_XML_NODE(L"items")
								{					 
									CP_XML_ATTR(L"count", current_.fields[i].groups.size());
									for (size_t j = 0; j < current_.fields[i].groups.size(); j++)
									{
										CP_XML_NODE(L"item")
										{					 
											CP_XML_ATTR(L"x", j);
										}
									}
								}
							}
							else if (!current_.fields[i].caches.empty())
							{
								CP_XML_NODE(L"items")
								{					 
									CP_XML_ATTR(L"count", current_.fields[i].caches.size());
									for (size_t j = 0; j < current_.fields[i].caches.size(); j++)
									{
										CP_XML_NODE(L"item")
										{					 
											if (current_.fields[i].caches[j].show_details == false)
											{
												CP_XML_ATTR(L"sd", 0);
											}
											CP_XML_ATTR(L"x", j);
										}
									}
								}
							}
						}
					}
				}		
			}
			if (!current_.row_fields.empty())
			{
				CP_XML_NODE(L"rowFields")
				{
					CP_XML_ATTR(L"count", current_.row_fields.size());
					for (size_t i = 0; i < current_.row_fields.size(); i++)
					{
						CP_XML_NODE(L"field")
						{					 
							CP_XML_ATTR(L"x", current_.row_fields[i]);
						}
					}
				}
			}
			if (!current_.col_fields.empty())
			{
				CP_XML_NODE(L"colFields")
				{
					CP_XML_ATTR(L"count", current_.col_fields.size());
					for (size_t i = 0; i < current_.col_fields.size(); i++)
					{
						CP_XML_NODE(L"field")
						{					 
							CP_XML_ATTR(L"x", current_.col_fields[i]);
						}
					}
				}
			}
			if (!current_.page_fields.empty())
			{
				CP_XML_NODE(L"pageFields")
				{
					CP_XML_ATTR(L"count", current_.page_fields.size());
					for (size_t i = 0; i < current_.page_fields.size(); i++)
					{
						CP_XML_NODE(L"pageField")
						{					 
							CP_XML_ATTR(L"fld", current_.page_fields[i]);
							//CP_XML_ATTR(L"item", 0);
							CP_XML_ATTR(L"hier", -1);
						}
					}
				}
			}
			if (!current_.data_fields.empty())
			{
				CP_XML_NODE(L"dataFields")
				{
					CP_XML_ATTR(L"count", current_.data_fields.size());
					for (size_t i = 0; i < current_.data_fields.size(); i++)
					{
						int ind_field_dirty = current_.data_fields[i];

						CP_XML_NODE(L"dataField")
						{	
							int fld = 0, ind_fld = current_.fields[ind_field_dirty].references_field < 0 ? ind_field_dirty : 
														current_.fields[ind_field_dirty].references_field;
							for (int k = 0; k < ind_fld; k++)
							{
								if (current_.fields[k].type != 7 ) fld++;
							}
						
							CP_XML_ATTR(L"fld", fld);
							if (current_.fields[ind_field_dirty].references.empty())
							{
								CP_XML_ATTR(L"baseField", fld);
							}
							else 
							{
								size_t base_field = 0, base_item = 0;
								for (size_t k = 0; k < current_.fields.size(); k++)
								{
									if (current_.fields[k].type == 7 ) continue;

									if (current_.fields[k].name == current_.fields[ind_field_dirty].references[0].name)
									{
										for (size_t j = 0; j < current_.fields[k].caches.size(); j++)
										{
											if (current_.fields[k].caches[j].sVal == current_.fields[ind_field_dirty].references[0].member_name)
											{
												base_item = j;
												break;
											}
										}
										break;
									}
									base_field++;
								}
								CP_XML_ATTR(L"baseField", base_field);
								CP_XML_ATTR(L"baseItem", base_item);
								
								switch(current_.fields[ind_field_dirty].references[0].type)
								{
									case 1: CP_XML_ATTR(L"showDataAs", L"difference");	break;
									case 2: CP_XML_ATTR(L"showDataAs", L"percent");		break;
									case 3: CP_XML_ATTR(L"showDataAs", L"percentDiff"); break;
								}
							}
							std::wstring name;

							if (ind_field_dirty >= 0 && ind_field_dirty < (int)current_.fields.size())
							{
								switch(current_.fields[ind_field_dirty].function)
								{
									case 1:		CP_XML_ATTR(L"subtotal", L"average");	name = L"average ";		break;
									case 2:		CP_XML_ATTR(L"subtotal", L"count");		name = L"count ";		break;
									case 3:		CP_XML_ATTR(L"subtotal", L"countNums");	name = L"countNums ";	break;
									case 4:		CP_XML_ATTR(L"subtotal", L"max");		name = L"max ";			break;
									case 5:		CP_XML_ATTR(L"subtotal", L"min");		name = L"min ";			break;
									case 6:		CP_XML_ATTR(L"subtotal", L"product");	name = L"product ";		break;
									case 7:		CP_XML_ATTR(L"subtotal", L"stdDev");	name = L"stdDev Dev ";	break;
									case 8:		CP_XML_ATTR(L"subtotal", L"stdDevp");	name = L"stdDevp ";		break;
									case 9:		CP_XML_ATTR(L"subtotal", L"sum");		name = L"sum by ";		break;
									case 10:	CP_XML_ATTR(L"subtotal", L"var");		name = L"var ";			break;
									case 11:	CP_XML_ATTR(L"subtotal", L"varp");		name = L"varp ";		break;
									case 12:
										{
											CP_XML_ATTR(L"subtotal", current_.fields[current_.data_fields[i]].user_function);
										}break;		
									default: break;
								}
								if (current_.fields[ind_field_dirty].display_name.empty())
								{
									name += current_.fields[ind_field_dirty].name;
								}
								else
								{
									name = current_.fields[ind_field_dirty].display_name;
								}
								CP_XML_ATTR(L"name", name);
							}
						}
					}
				}
			}
			CP_XML_NODE(L"pivotTableStyleInfo")
			{
				CP_XML_ATTR(L"showRowHeaders", 1);
				CP_XML_ATTR(L"showColHeaders", 1);
				CP_XML_ATTR(L"showRowStripes", 0);
				CP_XML_ATTR(L"showColStripes", 0);
				CP_XML_ATTR(L"showLastColumn", 1);
			}
		}
	}
}
void xlsx_pivots_context::Impl::serialize_type_field(CP_ATTR_NODE, _field & field)
{
	_CP_OPT(bool) containsSemiMixedTypes;
	_CP_OPT(bool) containsMixedTypes;
	_CP_OPT(bool) containsNonDate;
	_CP_OPT(bool) containsDate;
	_CP_OPT(bool) containsString;
	_CP_OPT(bool) containsBlank;
	_CP_OPT(bool) containsNumber;
	_CP_OPT(bool) containsInteger;
									
	if (field.bDate		& field.bNumber/* || 
		field.bNumber	& field.bString*/)
	{
		containsSemiMixedTypes = true;
	}
	else if (field.bDate	& field.bString || 
			 field.bNumber	& field.bString || 
			 field.bInteger & field.bString)
	{
		containsMixedTypes = true;
		
		if (field.bInteger)
		{
			if (field.bNumber) field.bInteger = false;
			field.bNumber = true;
		}
	}
	else if (!field.bEmpty && !field.bString && !field.bBool)
	{
		containsSemiMixedTypes = false;
	}
	if (field.bDate && 
		!(field.bNumber || field.bInteger || field.bString || field.bEmpty ))
	{
		containsNonDate = false;
	}
	if (field.bDate)
	{
		containsDate = true;
	}
	if (!field.bString && 
		(field.bInteger || field.bDate || field.bNumber || field.bEmpty))
	{
		containsString = false;
		if (field.bInteger)
		{
			if (field.bNumber) field.bInteger = false;
			field.bNumber = true;
		}
		if (/*!field.bDate && */field.bEmpty)
			containsNonDate = false;
	}
	if (field.bEmpty)
	{
		containsBlank = true;
	}
	if (field.bNumber)
	{
		containsNumber = true;	
	}
	if (field.bInteger && !field.bDate)
	{
		if (containsMixedTypes)
		{
			containsNumber = true;	
			containsInteger = true;	
		}
		else
			containsInteger = true;	
	}	

	if (containsNonDate)		CP_XML_ATTR(L"containsNonDate",			*containsNonDate);
	if (containsSemiMixedTypes) CP_XML_ATTR(L"containsSemiMixedTypes",	*containsSemiMixedTypes);
	if (containsString)			CP_XML_ATTR(L"containsString",			*containsString);
	if (containsBlank)			CP_XML_ATTR(L"containsBlank",			*containsBlank);
	if (containsMixedTypes)		CP_XML_ATTR(L"containsMixedTypes",		*containsMixedTypes);
	if (containsDate)			CP_XML_ATTR(L"containsDate",			*containsDate);
	if (containsNumber)			CP_XML_ATTR(L"containsNumber",			*containsNumber);
	if (containsInteger)		CP_XML_ATTR(L"containsInteger",			*containsInteger);
}
void xlsx_pivots_context::Impl::serialize_cache(std::wostream & strm)
{
	std::map<std::wstring, bool> used_field_name, used_field_name_lower;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotCacheDefinition")
		{          
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		
			//if (records file)
			//{
			//	CP_XML_ATTR(L"r:id", L"rId1" );
			//}
			//else 
			{
				CP_XML_ATTR(L"saveData", 0);
			}
			//CP_XML_ATTR(L"enableRefresh",	1);
			CP_XML_ATTR(L"refreshedBy",		L"");
			CP_XML_ATTR(L"refreshedDate",	0);
			CP_XML_ATTR(L"recordCount",		0);

			if (true)
			{
				CP_XML_NODE(L"cacheSource")
				{
					if (!current_.source_ref.empty())
					{
						CP_XML_ATTR(L"type", L"worksheet");
						CP_XML_NODE(L"worksheetSource")
						{
							CP_XML_ATTR(L"ref", current_.source_ref);
							CP_XML_ATTR(L"sheet", current_.source_table_name);
						}
					}
					if (!current_.source_database.empty())
					{
						CP_XML_ATTR(L"type", L"external");
						CP_XML_ATTR(L"connectionId", ++connection_count);
						
						dump_connection();
					}
				}	
			}
			int index_current = 0;

			if (current_.fields.empty() == false)
			{
				CP_XML_NODE(L"cacheFields")
				{
					CP_XML_ATTR(L"count", current_.fields_count);

					for (size_t i = 0; i < current_.fields.size(); i++)
					{
						if (current_.fields[i].type == 7)
							continue;

						if (used_field_name.end() != used_field_name.find(current_.fields[i].name))
							continue;
//---------------------------------------------------------------------------
						used_field_name.insert(std::make_pair(current_.fields[i].name, true)); //дублированые поля

						std::wstring name = current_.fields[i].name; // в мс "H" и "h" одно имя (

						std::wstring name_lower = XmlUtils::GetLower(name);

						while (used_field_name_lower.end() != used_field_name_lower.find(name_lower))
						{
							name += L"_";
							name_lower = XmlUtils::GetLower(name);
						}

						used_field_name_lower.insert(std::make_pair(name_lower, true)); 
//---------------------------------------------------------------------------
						CP_XML_NODE(L"cacheField")
						{
							CP_XML_ATTR(L"name", name);
							CP_XML_ATTR(L"numFmtId", 0);

							if (!current_.fields[i].source_groups.empty())
							{
								CP_XML_ATTR(L"databaseField", 0);
							}

							CP_XML_NODE(L"sharedItems")
							{
								if (current_.fields[i].caches.empty() == false/* && 
									current_.fields[i].type != 2*/)
								{									
									if (current_.fields[i].type_groups == 0)
									{
										CP_XML_ATTR(L"count", current_.fields[i].caches.size());
									}
									else if (current_.fields[i].type_groups < 8)
									{//group by  date
										current_.fields[i].bDate = true;
										current_.fields[i].bString = false;
									}
									//else group by fields
								
									serialize_type_field(CP_GET_XML_NODE(), current_.fields[i]);					
									if ( current_.fields[i].type_groups == 0 )
									{
										for (size_t j = 0; j < current_.fields[i].caches.size(); j++)
										{
											CP_XML_NODE(current_.fields[i].caches[j].sNode)
											{
												if (current_.fields[i].caches[j].sVal.empty() == false)
												{
													CP_XML_ATTR(L"v", current_.fields[i].caches[j].sVal);
												}
											}
										}
									}
								}
							}
							if (current_.fields[i].base_group >= 0)
							{
								CP_XML_NODE(L"fieldGroup")
								{	
									if ( current_.fields[i].type_groups > 0)
									{
										CP_XML_ATTR(L"base", current_.fields[i].base_group);
									}
									else
									{
										CP_XML_ATTR(L"par", current_.fields[i].base_group);	
									}
									if (current_.fields[i].type_groups > 0)
									{
										if (current_.fields[i].type_groups < 8)
										{
											CP_XML_NODE(L"rangePr")
											{
												switch(current_.fields[i].type_groups)
												{
												case 1: CP_XML_ATTR(L"groupBy",	L"seconds");	break;
												case 2: CP_XML_ATTR(L"groupBy",	L"minutes");	break;
												case 3: CP_XML_ATTR(L"groupBy",	L"hours");		break;
												case 4: CP_XML_ATTR(L"groupBy",	L"days");		break;
												case 5: CP_XML_ATTR(L"groupBy",	L"months");		break;
												case 6: CP_XML_ATTR(L"groupBy",	L"quarters");	break;
												case 7: CP_XML_ATTR(L"groupBy",	L"years");		break;
												}
												CP_XML_ATTR(L"startDate", L"1899-12-31T00:00:00");
												CP_XML_ATTR(L"endDate", L"1899-12-31T00:00:00");
											}
										}
										if (!current_.fields[i].groups_discrete.empty())
										{
											CP_XML_NODE(L"discretePr")
											{
												CP_XML_ATTR(L"count", current_.fields[i].groups_discrete.size());
												for (size_t j = 0; j < current_.fields[i].groups_discrete.size(); j++)
												{
													CP_XML_NODE(L"x")
													{
														CP_XML_ATTR(L"v", current_.fields[i].groups_discrete[j]);
													}
												}
											}
										}
										CP_XML_NODE(L"groupItems")
										{
											if (!current_.fields[i].groups.empty())
											{
												CP_XML_ATTR(L"count", current_.fields[i].groups.size());
												for (size_t j = 0; j < current_.fields[i].groups.size(); j++)
												{
													CP_XML_NODE(L"s")
													{
														CP_XML_ATTR(L"v", current_.fields[i].groups[j].name);
													}
												}
											}
											else
											{
												CP_XML_ATTR(L"count", current_.fields[i].caches.size());
												
												for (size_t j = 0; j < current_.fields[i].caches.size(); j++)
												{
													CP_XML_NODE(current_.fields[i].caches[j].sNode)
													{
														if (current_.fields[i].caches[j].sVal.empty() == false)
														{
															CP_XML_ATTR(L"v", current_.fields[i].caches[j].sVal);
														}
													}
												}
											}
										}
									}
								}
							}
						}							

						index_current++;
					}
				}
			}
			//if (pivot_cache->m_arSXFORMULA.empty() == false)
			//{
			//	CP_XML_NODE(L"calculatedItems")
			//	{
			//		CP_XML_ATTR(L"count", pivot_cache->m_arSXFORMULA.size());

			//		for (size_t i = 0; i < pivot_cache->m_arSXFORMULA.size(); i++)
			//		{
			//			pivot_cache->m_arSXFORMULA[i]->serialize(CP_XML_STREAM());
			//		}
			//	}
			//}
		}
	}
}

int xlsx_pivots_context::get_count()
{
	return (int)impl_->pivot_xmls_.size();
}
bool xlsx_pivots_context::is_connections()
{
	return !impl_->connections_.empty();
}
void xlsx_pivots_context::dump_rels_cache(int index, rels & Rels)
{
	if (impl_->pivot_xmls_[index].recordsData_.empty() == false)
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
						L"pivotCacheRecords" + std::to_wstring(index + 1) + L".xml", L""));
	}
}
void xlsx_pivots_context::dump_rels_view(int index, rels & Rels)
{
	Rels.add(relationship(L"rId1",							
					L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition",
					L"../pivotCache/pivotCacheDefinition" + std::to_wstring(index + 1) + L".xml", L""));
}
std::wstring xlsx_pivots_context::get_chart_source(std::wstring name)
{
	std::wstring result;

	for (size_t i = 0; i < impl_->pivot_xmls_.size(); i++)
	{
		if (impl_->pivot_xmls_[i].name == name)
		{
			std::wstring sheet_name = impl_->pivot_xmls_[i].sheet;
			if (0 == sheet_name.find(L"'"))
			{
				sheet_name = sheet_name.substr(1, sheet_name.length() - 2);
			}	
			result = L"[]" + sheet_name + L"!PivotTable" + std::to_wstring(i + 1);
			break;
		}
	}

	return result;
}
void xlsx_pivots_context::write_cache_definitions_to(int index, std::wostream & strm)
{
	strm << impl_->pivot_xmls_[index].definitionsData_;
}
void xlsx_pivots_context::write_connections_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"connections")
		{
            CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");

			CP_XML_STREAM() << impl_->connections_;
		}
	}
}

void xlsx_pivots_context::write_cache_records_to(int index, std::wostream & strm)
{
	strm << impl_->pivot_xmls_[index].recordsData_;
}
void xlsx_pivots_context::write_table_view_to(int index, std::wostream & strm)
{
	strm << impl_->pivot_xmls_[index].viewData_;
}

void xlsx_pivots_context::start_table()
{
	impl_->current_.clear();
}

int xlsx_pivots_context::end_table()
{
	std::wstringstream	view_strm;
	std::wstringstream	cache_strm;
	std::wstringstream	rec_strm;
	
	impl_->calc_headers();
	impl_->sort_fields();

	impl_->serialize_view(view_strm);
	impl_->serialize_cache(cache_strm);

    std::wstring str_view = view_strm.str();

	if (str_view.empty()) return -1;

	Impl::_pivot_xml pivot_xml = {impl_->current_.name, impl_->current_.location_table_name, cache_strm.str(), rec_strm.str(), str_view};
	
	impl_->pivot_xmls_.push_back(pivot_xml);

	return impl_->pivot_xmls_.size();
}

void xlsx_pivots_context::set_view_name(std::wstring name)
{
	impl_->current_.name = name;
}
void xlsx_pivots_context::set_view_target_range(std::wstring ref)
{
	impl_->current_.location_ref = ref;
}
void xlsx_pivots_context::set_view_target_table_name(std::wstring name)
{
	impl_->current_.location_table_name = name;
}
void xlsx_pivots_context::add_button_header(std::wstring ref)
{
	impl_->current_.headers.push_back(ref);
}

void xlsx_pivots_context::set_identify_categories(bool val)
{
	impl_->current_.identify_categories = val;
}
void xlsx_pivots_context::set_ignore_empty_rows(bool val)
{
	impl_->current_.ignore_empty_rows = val;
}
void xlsx_pivots_context::set_drill(bool val)
{
	impl_->current_.drill_enabled = val;
}
void xlsx_pivots_context::set_grand_total(int type)
{
	impl_->current_.grand_total = type;
}
void xlsx_pivots_context::start_field()
{
	Impl::_field f;
	impl_->current_.fields.push_back(f);

	impl_->current_.fields_count++;
}
void xlsx_pivots_context::set_field_name(std::wstring name)
{
	impl_->current_.fields.back().name = name;
	impl_->current_.fields.back().name_enabled = true;
}
void xlsx_pivots_context::set_field_display(std::wstring name)
{
	impl_->current_.fields.back().display_name = name;
}
void xlsx_pivots_context::set_field_type(int type, int hierarchy)
{
	impl_->current_.fields.back().type		= type;
	impl_->current_.fields.back().hierarchy = hierarchy;

	//if ( impl_->current_.fields.back().name.empty() &&
	//	!impl_->current_.fields.back().data_layout)
	//	return;

	//switch(type)
	//{
	//	case 0:	// column
	//	{
	//		if (impl_->current_.fields.back().data_layout)
	//		{
	//			if (impl_->current_.fields.back().name.empty() && (!impl_->current_.identify_categories || hierarchy >= 0))
	//				//impl_->current_.fields.back().repeat_item_labels )) //???? 
	//					impl_->current_.col_fields.push_back(-2);	
	//		}
	//		else
	//			impl_->current_.col_fields.push_back(impl_->current_.fields.size() - 1);	
	//	}break;
	//	case 1:	// data
	//	{
	//		int index_field = -1;
	//		
	//		for (size_t i = 0; i < impl_->current_.fields.size() - 1; i++)
	//		{
	//			if (impl_->current_.fields[i].name == impl_->current_.fields.back().name && 
	//				(impl_->current_.fields[i].type != 1 || !impl_->current_.fields.back().references.empty()))
	//			{
	//				impl_->current_.fields[i].bDataFieldEnabled = true;
	//				index_field = i;
	//				break;
	//			}
	//		}
	//		if (index_field >= 0)
	//		{
	//			impl_->current_.fields.back().type = 7; //skip 
	//			impl_->current_.fields_count--; 
	//			
	//			if (!impl_->current_.fields.back().references.empty())
	//			{
	//				impl_->current_.fields.back().references_field = index_field;
	//				impl_->current_.fields.back().bDataFieldEnabled = true;
	//			
	//				index_field = impl_->current_.fields.size() - 1;
	//			}
	//		}
	//		else
	//		{
	//			index_field = impl_->current_.fields.size() - 1;
	//			impl_->current_.fields.back().bDataFieldEnabled = true;
	//		}
	//		impl_->current_.data_fields.push_back(index_field);	
	//	}break;
 //       case 2:	// hidden
	//		break;
 //       case 3:	// page
	//		impl_->current_.page_fields.push_back(impl_->current_.fields.size() - 1);	
	//		break;
	//	case 4:	// row
	//		if (impl_->current_.fields.back().data_layout)
	//		{
	//			impl_->current_.data_on_row = true;

	//			if (impl_->current_.fields.back().name.empty() && (!impl_->current_.identify_categories || hierarchy >= 0))
	//				//(impl_->current_.fields.back().repeat_item_labels ||
	//			 //  !impl_->current_.identify_categories ))
	//					impl_->current_.row_fields.push_back(-2);	

	//		}
	//		else
	//			impl_->current_.row_fields.push_back(impl_->current_.fields.size() - 1);	
	//		break;
	//}
}
void xlsx_pivots_context::set_field_function(int type)
{
	impl_->current_.fields.back().function = type;
}
void xlsx_pivots_context::set_field_user_function(std::wstring f)
{
	impl_->current_.fields.back().user_function = f;
}
void xlsx_pivots_context::add_field_subtotal(int function_type)
{
	impl_->current_.fields.back().subtotals.push_back(function_type);
}
void xlsx_pivots_context::set_field_data_layout(bool val)
{
	impl_->current_.fields.back().data_layout = val;
}
void xlsx_pivots_context::set_field_show_empty(bool val)
{
	impl_->current_.fields.back().show_empty = val;
	//add_field_cache(impl_->current_.fields.back().caches.size(), L""); 
}
void xlsx_pivots_context::set_repeat_item_labels(bool val)
{
	impl_->current_.fields.back().repeat_item_labels = val;
}
void xlsx_pivots_context::set_field_groups(int type)
{
	impl_->current_.fields.back().type_groups = type + 1;
}
void xlsx_pivots_context::set_field_groups_source(std::wstring name)
{
	impl_->current_.fields.back().source_groups = name;
}
void xlsx_pivots_context::start_field_group()
{
	impl_->current_.in_group = true;
	Impl::_group group;
	impl_->current_.fields.back().groups.push_back(group);
}
void xlsx_pivots_context::set_field_group_name (std::wstring name)
{
	impl_->current_.fields.back().groups.back().name = name;
}
void xlsx_pivots_context::end_field_group()
{
	impl_->current_.in_group = false;
}
void xlsx_pivots_context::set_field_sort(int type)
{
	impl_->current_.fields.back().sort = type + 1;
}
void xlsx_pivots_context::start_field_reference()
{
	Impl::_reference ref;
	impl_->current_.fields.back().references.push_back(ref);
}
void xlsx_pivots_context::end_field_reference()
{
}
void xlsx_pivots_context::set_field_ref_name(std::wstring name)
{
	impl_->current_.fields.back().references.back().name = name;

	for (size_t i = 0; i < impl_->current_.fields.size(); i++)
	{
		if (impl_->current_.fields[i].name == name)
		{
			impl_->current_.fields[i].used_in_referenes = true;
		}
	}
}
void xlsx_pivots_context::set_field_ref_type (int type)
{
	impl_->current_.fields.back().references.back().type = type;
}
void xlsx_pivots_context::set_field_ref_member_name(std::wstring name)
{
	impl_->current_.fields.back().references.back().member_name = name;
}
void xlsx_pivots_context::set_field_ref_member_type(int type)
{
	impl_->current_.fields.back().references.back().member_type = type;
}
void xlsx_pivots_context::add_field_cache(int index, std::wstring value, bool show_details)
{
	if (index < 0)
		index = impl_->current_.fields.back().caches.size();

	while (index > (int)impl_->current_.fields.back().caches.size())
	{
		Impl::_field_value f(L"", L"m", true);

		impl_->current_.fields.back().caches.push_back(f);
		impl_->current_.fields.back().bEmpty = true;
	}
	std::wstring node_name = L"s";
	if (value.empty())
	{
		node_name = L"m";
		impl_->current_.fields.back().bEmpty = true;
	}
	else
	{
		int pos = (int)value.find(L"$");	//$1,33,333.00
		
		_CP_OPT(double) dVal;
		if (pos >= 0)//финановый .. todooo общее правило бы...
		{
			//value = value.substr(pos + 1);
			//XmlUtils::replace_all(value, L",", L"");
			//XmlUtils::replace_all(value, L" ", L"");
			//XmlUtils::replace_all(value, L"\x00A0", L"");		
		}
		std::wstring strNumber = oox::GetNumberFromString(value);
		if (false == strNumber.empty())
		{
			try
			{	
				XmlUtils::replace_all(strNumber, L" ", L"");
				XmlUtils::replace_all(strNumber, L",", L".");
				
				dVal = boost::lexical_cast<double>(strNumber);
			}
			catch(...)
			{
			}
		}
		if (dVal)
		{
			node_name = L"n";

			_INT64 iVal = *dVal;
			if ( *dVal - iVal > 0.00001)
			{
				value = std::to_wstring(*dVal);
				impl_->current_.fields.back().bNumber = true;
			}
			else
			{
				value = std::to_wstring(iVal);
				impl_->current_.fields.back().bInteger = true;
			}
		}
		else if (value == L"false" || value == L"true")
		{
			node_name = L"b";
			if (value == L"true")	value = L"1";
			else					value = L"0";
		}
		else
		{
			impl_->current_.fields.back().bString = true;
		}
	}
	impl_->current_.fields.back().caches.push_back(Impl::_field_value(value, node_name, show_details));

	if (impl_->current_.in_group)
	{
		impl_->current_.fields.back().groups.back().caches.push_back(impl_->current_.fields.back().caches.back());
	}

}
void xlsx_pivots_context::end_field()
{
	//if (impl_->current_.fields.back().name.empty())
	//{
	//	impl_->current_.fields_count--; 
	//	impl_->current_.fields.pop_back();
	//}
}
void xlsx_pivots_context::set_source_range(std::wstring table_name, std::wstring ref)
{
	impl_->current_.source_table_name	= table_name;
	impl_->current_.source_ref			= ref;
}
void xlsx_pivots_context::set_source_database(std::wstring database, std::wstring table_name)
{
	impl_->current_.source_table_name	= table_name;
	impl_->current_.source_database		= database;
}
void xlsx_pivots_context::set_source_database_query(std::wstring database, std::wstring query)
{
	impl_->current_.source_database			= database;
	impl_->current_.source_database_query	= query;
}
void xlsx_pivots_context::set_source_database_sql(std::wstring database, std::wstring sql)
{
	impl_->current_.source_database		= database;
	impl_->current_.source_database_sql	= sql;
}

void xlsx_pivots_context::add_connections(std::wstring connections)
{
	if (connections.empty()) return;

	impl_->connections_ = connections;
}

xlsx_pivots_context::~xlsx_pivots_context()
{
}


}
}

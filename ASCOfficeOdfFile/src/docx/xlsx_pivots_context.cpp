/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "xlsx_pivots_context.h"

#include <boost/make_shared.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

class xlsx_pivots_context::Impl
{
public:
	struct _pivot_xml
	{	
		std::wstring  definitionsData_; //cacheData
		std::wstring  recordsData_;		//cacheRecorda

		std::wstring  viewData_;		//tableView
	};

	Impl() {}
    
	std::vector<_pivot_xml>		pivot_xmls_;
	std::wstring				connections_;

	struct _field
	{
		std::wstring				name;
		int							type = -1;
		int							function = -1;
		std::wstring				user_function;
		std::vector<int>			subtotals;
		std::vector<std::wstring>	caches;
	};
	struct _desc
	{
		void clear()
		{
			name.clear();
			location_ref.clear();
			source_ref.clear();
			fields.clear();
			row_fields.clear();
			page_fields.clear();
			col_fields.clear();
			data_fields.clear();
		}
		std::wstring		name;
		std::wstring		location_ref;
		std::wstring		source_ref;
		std::vector<_field>	fields;
		std::vector<int>	row_fields;
		std::vector<int>	page_fields;
		std::vector<int>	col_fields;
		std::vector<int>	data_fields;
	}current_;

	void serialize_view(std::wostream & strm);
};

xlsx_pivots_context::xlsx_pivots_context() : impl_(new xlsx_pivots_context::Impl())
{
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
			//CP_XML_ATTR(L"dataOnRows",			view->sxaxis4Data.bRw); 
			//CP_XML_ATTR(L"applyNumberFormats",	view->fAtrNum);
			//CP_XML_ATTR(L"applyBorderFormats",	view->fAtrBdr); 
			//CP_XML_ATTR(L"applyFontFormats",		view->fAtrFnt);
			//CP_XML_ATTR(L"applyPatternFormats",	view->fAtrPat);
			//CP_XML_ATTR(L"applyAlignmentFormats",	view->fAtrAlc);
			//CP_XML_ATTR(L"applyWidthHeightFormats",	view->fAtrProc);
			//if (!view->stData.value().empty())
			//{
			//	CP_XML_ATTR(L"dataCaption",			view->stData.value()); 
			//}
			//CP_XML_ATTR(L"asteriskTotals",			1); 
			//CP_XML_ATTR(L"showMemberPropertyTips",	0);
			//CP_XML_ATTR(L"useAutoFormatting",		view->fAutoFormat); 
			//CP_XML_ATTR(L"autoFormatId",			view->itblAutoFmt);
			CP_XML_ATTR(L"itemPrintTitles",			1);  
			CP_XML_ATTR(L"indent",					0); 
			CP_XML_ATTR(L"compact",					0);  
			CP_XML_ATTR(L"compactData",				0); 
			CP_XML_ATTR(L"gridDropZones",			1); 	

			CP_XML_NODE(L"location")
			{
				CP_XML_ATTR(L"ref", current_.location_ref);
				//CP_XML_ATTR(L"firstHeaderRow", view->rwFirstHead - view->ref.rowFirst );
				//CP_XML_ATTR(L"firstDataRow", view->rwFirstData - view->ref.rowFirst);
				//CP_XML_ATTR(L"firstDataCol", view->colFirstData - view->ref.columnFirst); 
				CP_XML_ATTR(L"rowPageCount", 1); 
				CP_XML_ATTR(L"colPageCount", 1);
			}
			CP_XML_NODE(L"pivotFields")
			{
				CP_XML_ATTR(L"count", current_.fields.size());
				for (size_t i = 0; i < current_.fields.size(); i++)
				{
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
						CP_XML_ATTR(L"defaultSubtotal", 0);
	//compact="0" outline="0" subtotalTop="0" showAll="0" includeNewItemsInFilter="1" sortType="ascending"
						CP_XML_NODE(L"items")
						{					 
							CP_XML_ATTR(L"count", current_.fields[i].caches.size());
							for (size_t j = 0; j < current_.fields[i].caches.size(); j++)
							{
								CP_XML_NODE(L"item")
								{					 
									CP_XML_ATTR(L"x", j);
								}
							}
						}
					}
					//CP_XML_STREAM() << fields_[i].view_;
				}		
			}
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
			CP_XML_NODE(L"pageFields")
			{
				CP_XML_ATTR(L"count", current_.page_fields.size());
				for (size_t i = 0; i < current_.page_fields.size(); i++)
				{
					CP_XML_NODE(L"pageField")
					{					 
						CP_XML_ATTR(L"fld", current_.page_fields[i]);
						CP_XML_ATTR(L"item", 0);
						CP_XML_ATTR(L"hier", -1);
					}
				}
			}
			CP_XML_NODE(L"dataFields")
			{
				CP_XML_ATTR(L"count", current_.data_fields.size());
				for (size_t i = 0; i < current_.data_fields.size(); i++)
				{
					CP_XML_NODE(L"dataField")
					{					 
						CP_XML_ATTR(L"fld", current_.data_fields[i]);
						CP_XML_ATTR(L"baseField", 0);
						//CP_XML_ATTR(L"baseItem", -1);
						//CP_XML_ATTR(L"name", L"");
					}
				}
			}
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

	impl_->serialize_view(view_strm);

	Impl::_pivot_xml v = {cache_strm.str(), rec_strm.str(), view_strm.str()};
	
	impl_->pivot_xmls_.push_back(v);

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
void xlsx_pivots_context::start_field()
{
	Impl::_field f;
	impl_->current_.fields.push_back(f);
}
void xlsx_pivots_context::set_field_name(std::wstring name)
{
	impl_->current_.fields.back().name = name;
}
void xlsx_pivots_context::set_field_type(int type)
{
	impl_->current_.fields.back().type = type;

	switch(type)
	{
		case 0:	impl_->current_.col_fields.push_back(impl_->current_.fields.size() - 1);	break;// column,
		case 1:	impl_->current_.data_fields.push_back(impl_->current_.fields.size() - 1);	break;// data,
        case 2:	break;// hidden,
        case 3:	impl_->current_.page_fields.push_back(impl_->current_.fields.size() - 1);	break;// page,
		case 4:	impl_->current_.row_fields.push_back(impl_->current_.fields.size() - 1);	break;// row
	}
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
void xlsx_pivots_context::add_field_cache(int index, std::wstring value)
{
	while (index > impl_->current_.fields.back().caches.size())
	{
		impl_->current_.fields.back().caches.push_back(L"");
	}
	impl_->current_.fields.back().caches.push_back(value);
}
void xlsx_pivots_context::end_field()
{

}
void xlsx_pivots_context::set_source_range(std::wstring ref)
{
	impl_->current_.source_ref = ref;
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
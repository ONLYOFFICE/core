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
	struct _pivot_cache
	{	
		std::wstring  definitionsData_;
		std::wstring  recordsData_;
	};
	struct _pivot_view
	{	
		std::wstring	data_;
		int				indexCache_;
	};
	struct _pivot_field
	{	
		std::wstring	view_;
		std::wstring	cache_;
	};
	Impl() {}
    
	std::vector<_pivot_cache>	caches_;
	std::vector<_pivot_view>	views_;
	std::wstring				connections_;

	std::vector<_pivot_field>	fields_;
	
	std::wstringstream			view_;
	std::wstringstream			cache_;
};

xlsx_pivots_context::xlsx_pivots_context() : impl_(new xlsx_pivots_context::Impl())
{
}

void xlsx_pivots_context::add_cache(std::wstring definitions, std::wstring records)
{
	Impl::_pivot_cache c = {definitions, records};
	impl_->caches_.push_back(c);
}

int xlsx_pivots_context::get_cache_count()
{
	return (int)impl_->caches_.size();
}
bool xlsx_pivots_context::is_connections()
{
	return !impl_->connections_.empty();
}
void xlsx_pivots_context::dump_rels_cache(int index, rels & Rels)
{
	if (impl_->caches_[index].recordsData_.empty() == false)
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
						L"pivotCacheRecords" + std::to_wstring(index + 1) + L".xml", L""));
	}
}
void xlsx_pivots_context::dump_rels_view(int index, rels & Rels)
{
	if (impl_->views_[index].indexCache_ >= 0)
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition",
						L"../pivotCache/pivotCacheDefinition" + std::to_wstring(impl_->views_[index].indexCache_ + 1) + L".xml", L""));
	}
}
void xlsx_pivots_context::write_cache_definitions_to(int index, std::wostream & strm)
{
	strm << impl_->caches_[index].definitionsData_;
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
	strm << impl_->caches_[index].recordsData_;
}
void xlsx_pivots_context::write_table_view_to(int index, std::wostream & strm)
{
	strm << impl_->views_[index].data_;
}

void xlsx_pivots_context::start_table()
{
	impl_->fields_.clear();
}

int xlsx_pivots_context::end_table()
{
	std::wstringstream strm;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotTableDefinition")
		{ 
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
			
			//CP_XML_ATTR(L"name",				view->stTable.value()); 
			//CP_XML_ATTR(L"cacheId",				view->iCache); 
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
				//CP_XML_ATTR(L"ref", view->ref.toString());
				//CP_XML_ATTR(L"firstHeaderRow", view->rwFirstHead - view->ref.rowFirst );
				//CP_XML_ATTR(L"firstDataRow", view->rwFirstData - view->ref.rowFirst);
				//CP_XML_ATTR(L"firstDataCol", view->colFirstData - view->ref.columnFirst); 
				CP_XML_ATTR(L"rowPageCount", 1); 
				CP_XML_ATTR(L"colPageCount", 1);
			}
			CP_XML_NODE(L"pivotFields")
			{
				CP_XML_ATTR(L"count", impl_->fields_.size());
				for (size_t i = 0; i < impl_->fields_.size(); i++)
				{
					impl_->fields_[i].view_;
				}		
			}
		}
	}
	Impl::_pivot_view v = {strm.str(), impl_->views_.size() + 1};
	impl_->views_.push_back(v);

	return impl_->views_.size();
}

void xlsx_pivots_context::start_field()
{
}
void xlsx_pivots_context::end_field()
{
	std::wstringstream strm;
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotField")
		{ 

		}
	}	
	Impl::_pivot_field f = {strm.str(), L""};
	
	impl_->fields_.push_back(f);
}

void xlsx_pivots_context::add_connections(std::wstring connections)
{
	if (connections.empty()) return;

	impl_->connections_ = connections;
}

int xlsx_pivots_context::get_view_count()
{
	return (int)impl_->views_.size();
}

xlsx_pivots_context::~xlsx_pivots_context()
{
}

std::wostream & xlsx_pivots_context::stream_view()
{
	return impl_->view_;
}

std::wostream & xlsx_pivots_context::stream_cache()
{
	return impl_->cache_;
}


}
}
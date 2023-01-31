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

#include <boost/make_shared.hpp>
#include "../../Common/Utils/simple_xml_writer.h"

#include <map>

namespace oox {

class xlsx_pivots_context::Impl
{
public:
	struct _pivot_cache
	{	
		_pivot_cache(const std::wstring & def, const std::wstring & rec) : definitions_(def), records_(rec) {}

		std::wstring  definitions_;
		std::wstring  records_;

		std::unordered_map<std::wstring, std::wstring> externals_;
	};
	struct _pivot_view
	{	
		std::wstring	data_;
		int				indexCache_;
	};

	Impl() {}
    
	std::vector<_pivot_cache>	caches_;
	std::vector<_pivot_view>	views_;
};

xlsx_pivots_context::xlsx_pivots_context() : impl_(new xlsx_pivots_context::Impl())
{
}

void xlsx_pivots_context::add_cache(std::wstring definitions, std::wstring records)
{
	if (definitions.empty()) return;

	Impl::_pivot_cache c(definitions, records);
	impl_->caches_.push_back(c);
}
void xlsx_pivots_context::add_cache_external(std::unordered_map<std::wstring, std::wstring> &externals)
{
	impl_->caches_.back().externals_ = externals;
}

int xlsx_pivots_context::get_cache_count()
{
	return (int)impl_->caches_.size();
}

void xlsx_pivots_context::dump_rels_cache(int index, rels & Rels)
{
	if (impl_->caches_[index].records_.empty() == false)
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
						L"pivotCacheRecords" + std::to_wstring(index + 1) + L".xml", L""));
	}
	int i = 0;
	for (std::unordered_map<std::wstring, std::wstring>::iterator	it =  impl_->caches_[index].externals_.begin(); 
																	it !=  impl_->caches_[index].externals_.end(); ++it, i++)
	{
		Rels.add(relationship(L"extId" + std::to_wstring(i + 1),							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLinkPath",
						it->second, L"External"));
	}
}
void xlsx_pivots_context::dump_rels_view(int index, rels & Rels)
{
	if (impl_->views_[index].indexCache_ > 0)
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition",
						L"../pivotCache/pivotCacheDefinition" + std::to_wstring( impl_->views_[index].indexCache_ ) + L".xml", L""));
	}
}
void xlsx_pivots_context::write_cache_definitions_to(int index, std::wostream & strm)
{
	strm << impl_->caches_[index].definitions_;
}

void xlsx_pivots_context::write_cache_records_to(int index, std::wostream & strm)
{
	strm << impl_->caches_[index].records_;
}
void xlsx_pivots_context::write_table_view_to(int index, std::wostream & strm)
{
	strm << impl_->views_[index].data_;
}
int xlsx_pivots_context::add_view(std::wstring table_view, int indexCache)
{
	if (table_view.empty()) return 0;

	Impl::_pivot_view v = {table_view, indexCache + 1};
	impl_->views_.push_back(v);

	return (int)impl_->views_.size();
}

int xlsx_pivots_context::get_view_count()
{
	return (int)impl_->views_.size();
}

xlsx_pivots_context::~xlsx_pivots_context()
{
}


}


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
#include <simple_xml_writer.h>

namespace oox {



class xlsx_pivots_context::Impl
{
public:
	struct _pivot_cache
	{	
		std::wstring  definitionsData_;
		std::wstring  recordsData_;
	};

	Impl() {}
    
	std::vector<_pivot_cache> caches_;
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
void xlsx_pivots_context::dump_rels(int index, rels & Rels)
{
	if (!impl_->caches_[index].recordsData_.empty())
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
						L"pivotCacheRecords" + std::to_wstring(index) + L".xml", L""));
	}
}
void xlsx_pivots_context::write_definitions_to(int index, std::wostream & strm)
{
	strm << impl_->caches_[index].definitionsData_;
}

void xlsx_pivots_context::write_records_to(int index, std::wostream & strm)
{
	strm << impl_->caches_[index].recordsData_;
}

xlsx_pivots_context::~xlsx_pivots_context()
{
}


}


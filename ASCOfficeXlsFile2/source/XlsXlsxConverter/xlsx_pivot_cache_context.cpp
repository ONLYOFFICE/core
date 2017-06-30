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
#include "xlsx_pivot_cache_context.h"

#include "oox_rels.h"
#include "mediaitems_utils.h"

#include <boost/make_shared.hpp>
#include <simple_xml_writer.h>

namespace oox {

class xlsx_pivot_cache_context::Impl
{
  //  struct rel_
  //  { 
  //      rel_(bool is_internal, std::wstring const & rid, std::wstring const & target,  external_items::Type type) :
  //      is_internal_(is_internal),
  //      rid_(rid),
		//target_(target),
  //      type_(type)
  //      {}

  //      bool is_internal_;
  //      std::wstring rid_;
  //      std::wstring target_;
  //      external_items::Type type_;
  //  };

public:
	Impl() {}
    
    std::wstringstream  definitionsData_;
    std::wstringstream  recordsData_;
	
	//rels				rels_;
   
};

xlsx_pivot_cache_context_ptr xlsx_pivot_cache_context::create()
{
    return boost::make_shared<xlsx_pivot_cache_context>();
}

int countPivotCaches = 0;

xlsx_pivot_cache_context::xlsx_pivot_cache_context() : impl_(new xlsx_pivot_cache_context::Impl()), isRecordsPresent(false)
{
	index = ++countPivotCaches;
}
std::wostream & xlsx_pivot_cache_context::definitions()
{
    return impl_->definitionsData_;
}
std::wostream & xlsx_pivot_cache_context::records()
{
    return impl_->recordsData_;
}
//void xlsx_pivot_cache_context::add_rels(
//        bool isInternal,
//        std::wstring const & rid,
//        std::wstring const & target,
//        external_items::Type type)
//{
//    impl_->add_rels(isInternal, rid, target, type);
//}
void xlsx_pivot_cache_context::dump_rels(rels & Rels)
{
	if (isRecordsPresent)
	{
		Rels.add(relationship(L"rId1",							
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
						L"pivotCacheRecords" + std::to_wstring(index) + L".xml", L""));
	}
}
void xlsx_pivot_cache_context::write_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotCacheDefinition")
		{          
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		
			if (isRecordsPresent)
			{
				CP_XML_ATTR(L"r:id", L"rId1" );
			}
			CP_XML_ATTR(L"recordCount", 0);
			//refreshedBy="Debra S Dalgleish" 
			//refreshedDate="39449.496381365738" 
			//createdVersion="1" 
			//refreshedVersion="2" 
			//recordCount="11" 
			//upgradeOnRefresh="1">

			CP_XML_STREAM() << impl_->definitionsData_.str();
		}
	}

}

void xlsx_pivot_cache_context::write_records_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotCacheRecords")
		{          
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

			CP_XML_ATTR(L"count", 0);

			CP_XML_STREAM() << impl_->recordsData_.str();
		}
	}

}

xlsx_pivot_cache_context::~xlsx_pivot_cache_context()
{
}


}


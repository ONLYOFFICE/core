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
#include "xlsx_conversion_context.h"
#include "oox_rels.h"
#include "mediaitems_utils.h"

#include <boost/make_shared.hpp>

#include <simple_xml_writer.h>

namespace oox {

class oox_activeX_context::Impl
{
    struct rel_
    { 
        rel_(bool is_internal, std::wstring const & rid, std::wstring const & target, std::wstring const & type) :
        is_internal_(is_internal),
        rid_(rid),
		target_(target),
        type_(type)
        {}

        bool is_internal_;
        std::wstring rid_;
        std::wstring target_;
        std::wstring type_;
    };

public:
	Impl() {}
    
	std::wstring		activeXDataBinRid;
	std::wstring		activeXClassId;

	std::vector<rel_>	activeXRels_;
   
	void dump_rels(rels & Rels)
    {
		for (size_t i = 0; i < activeXRels_.size(); i++)
        {
			rel_ & r = activeXRels_[i];
			
			Rels.add(relationship(r.rid_, r.type_, r.target_, (r.is_internal_ ? L"" : L"External")));	
		}
    }


    void add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		std::wstring const & type)
    {
		bool present = false;
        
		for (size_t i = 0; i < activeXRels_.size(); i++)
        {
			rel_ & r = activeXRels_[i];
			if (r.rid_ == rid && r.target_ == target)
				present = true;
		}
		if (!present)
		{
			activeXRels_.push_back(rel_(isInternal, rid, target, type));
		}
    }

};

oox_activeX_context_ptr oox_activeX_context::create()
{
    return boost::make_shared<oox_activeX_context>();
}

oox_activeX_context::oox_activeX_context() : impl_(new oox_activeX_context::Impl())
{
}
oox_activeX_context::~oox_activeX_context()
{
}
void oox_activeX_context::setDataBinRid(const std::wstring &rid, const std::wstring &bin_data)
{
    impl_->activeXDataBinRid = rid;
	
	impl_->add_rels(true, rid, bin_data, L"http://schemas.microsoft.com/office/2006/relationships/activeXControlBinary");
}
void oox_activeX_context::setClassId(const std::wstring &classId)
{
    impl_->activeXClassId = classId;
}
void oox_activeX_context::add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
        external_items::Type type)
{
	impl_->add_rels(isInternal, rid, target, utils::media::get_rel_type(type));
}
void oox_activeX_context::dump_rels(rels & Rels)
{
	impl_->dump_rels(Rels);
}
void oox_activeX_context::write_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"ax:ocx")
		{ 
			CP_XML_ATTR(L"xmlns:ax", L"http://schemas.microsoft.com/office/2006/activeX");
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			
			CP_XML_ATTR(L"ax:classid", impl_->activeXClassId);
			CP_XML_ATTR(L"ax:persistence", L"persistStreamInit");
			CP_XML_ATTR(L"r:id", impl_->activeXDataBinRid);
		}
	}
}



}


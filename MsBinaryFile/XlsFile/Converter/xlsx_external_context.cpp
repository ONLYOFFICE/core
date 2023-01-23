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
#include "oox_rels.h"
#include "mediaitems_utils.h"

#include <boost/make_shared.hpp>

#include "../../Common/Utils/simple_xml_writer.h"

namespace oox {

class oox_external_context::Impl
{
    struct rel_
    { 
        rel_(bool is_internal, std::wstring const & rid, std::wstring const & target,  external_items::Type type) :
        is_internal_(is_internal),
        rid_(rid),
		target_(target),
        type_(type)
        {}

        bool is_internal_;
        std::wstring rid_;
        std::wstring target_;
        external_items::Type type_;
    };

public:
	Impl() {}
    
    std::wstringstream  externalData_;
	std::vector<rel_>	externalRels_;
   
	void dump_rels(rels & Rels)
    {
		for (size_t i = 0; i < externalRels_.size(); i++)
        {
			rel_ & r = externalRels_[i];
			if (r.type_ == external_items::typeImage || 
				r.type_ == external_items::typeExternalLink)
			{
				Rels.add(relationship(
							r.rid_,
							utils::media::get_rel_type(r.type_),
							r.is_internal_ ? std::wstring(L"../") + r.target_ : r.target_,
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
 			else if (r.type_ == external_items::typeHyperlink)
			{
				Rels.add(relationship(
							r.rid_,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
							r.target_,
							L"External")
				);
			}
		}
    }


    void add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type)
    {
		bool present = false;
        
		for (size_t i = 0; i < externalRels_.size(); i++)
        {
			rel_ & r = externalRels_[i];
			if (r.rid_ == rid && r.target_ == target)
				present = true;
		}
		if (!present)
		{
			externalRels_.push_back(rel_(isInternal, rid, target, type));
		}
    }

};

oox_external_context_ptr oox_external_context::create()
{
    return boost::make_shared<oox_external_context>();
}

oox_external_context::oox_external_context() : impl_(new oox_external_context::Impl())
{
}
std::wostream & oox_external_context::externalData()
{
    return impl_->externalData_;
}
void oox_external_context::add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
        external_items::Type type)
{
    impl_->add_rels(isInternal, rid, target, type);
}
void oox_external_context::dump_rels(rels & Rels)
{
	impl_->dump_rels(Rels);
}
void oox_external_context::write_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"externalLink")
		{          
            CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
 			CP_XML_ATTR(L"mc:Ignorable", L"x14");
           CP_XML_ATTR(L"xmlns:x14", L"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main");
		
			CP_XML_STREAM() << impl_->externalData_.str();
		}
	}
}

oox_external_context::~oox_external_context()
{
}


}


/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "xlsx_drawings.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <vector>

#include "../../Common/Utils/simple_xml_writer.h"

#include "mediaitems_utils.h"
#include "oox_rels.h"

namespace oox {

class xlsx_drawings_rels::Impl
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
    void add(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type)
    {
		bool present = false;
        
		BOOST_FOREACH(rel_ const & r, rels_)
        {		
			if (r.rid_ == rid && r.target_ == target)
				present = true;
		}
		if (!present)
		{
			rels_.push_back(rel_(isInternal, rid, target, type));
		}
    }

   void dump_rels(rels & Rels)
    {
        BOOST_FOREACH(rel_ const & r, rels_)
        {
			if (r.type_ == external_items::typeHyperlink)
			{
				Rels.add(relationship(
							r.rid_,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
							r.target_,
							(r.is_internal_ ? L"" : L"External"))
				);
			}
			else
			{
				Rels.add(relationship(
							r.rid_,
							utils::media::get_rel_type(r.type_),
							(r.is_internal_ ? std::wstring(L"../") + r.target_ : r.target_),
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
		}
    }

private:
	std::vector<rel_>	rels_;
};

xlsx_drawings_rels::xlsx_drawings_rels() : impl_( new xlsx_drawings_rels::Impl() )
{
}

xlsx_drawings_rels::~xlsx_drawings_rels()
{
}

void xlsx_drawings_rels::add(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
        external_items::Type type)
{
    impl_->add(isInternal, rid, target, type);
}

void xlsx_drawings_rels::dump_rels(rels & Rels)
{
    return impl_->dump_rels(Rels);
}

xlsx_drawings_rels_ptr xlsx_drawings_rels::create()
{
    return boost::make_shared<xlsx_drawings_rels>();
}

}

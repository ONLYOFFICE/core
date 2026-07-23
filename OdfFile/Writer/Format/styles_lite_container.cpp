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
#include <vector>
#include <boost/unordered_map.hpp>

#include "styles_lite_container.h"

namespace cpdoccore { 
namespace odf {

struct style_ref_lite_container
{
    int style_oox;
    office_element_ptr style;
};

struct styles_lite_container::Impl
{
    typedef std::vector<style_ref_lite_container> array_t;
    array_t array_;
    typedef boost::unordered_map<int, size_t> map_t;
    map_t map_;
};

styles_lite_container::styles_lite_container(): impl_(new styles_lite_container::Impl() )
{
}

styles_lite_container::~styles_lite_container()
{
}

void styles_lite_container::add(const int style_oox, office_element_ptr content)
{
    style_ref_lite_container ref = {style_oox, content};
    impl_->array_.push_back(ref);
    impl_->map_[style_oox] = impl_->array_.size() - 1;    
}

office_element_ptr styles_lite_container::find_by_style_oox(const int style_oox)
{
    styles_lite_container::Impl::map_t::const_iterator i = impl_->map_.find(style_oox);
    if (i != impl_->map_.end())
    {
        return impl_->array_[i->second].style;
    }
    else
    {
        return office_element_ptr();
    }
}

}
}

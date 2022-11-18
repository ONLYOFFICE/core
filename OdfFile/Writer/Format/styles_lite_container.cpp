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

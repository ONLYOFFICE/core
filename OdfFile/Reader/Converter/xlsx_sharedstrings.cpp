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

#include <xml/utils.h>

#include "xlsx_sharedstrings.h"

namespace cpdoccore {
namespace oox {

class xlsx_shared_strings::Impl
{
public:
    void serialize(std::wostream & _Wostream) const;
    size_t add(const std::wstring & content);

private:
    std::vector<std::wstring> strings_;
};

void xlsx_shared_strings::Impl::serialize(std::wostream & _Wostream) const
{
    _Wostream << L"<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"" << 
        strings_.size() << "\" uniqueCount=\"" << strings_.size() << "\">";
    
    for (size_t i = 0; i < strings_.size(); i++)
    {
        _Wostream << L"<si>";
			_Wostream << strings_[i];
        _Wostream << L"</si>";
    }

    _Wostream << L"</sst>";
}

size_t xlsx_shared_strings::Impl::add(const std::wstring & content)
{
    strings_.push_back(content);
    return strings_.size() - 1;
}

xlsx_shared_strings::xlsx_shared_strings(): impl_( new xlsx_shared_strings::Impl() ) 
{
}

xlsx_shared_strings::~xlsx_shared_strings()
{
}

void xlsx_shared_strings::serialize(std::wostream & _Wostream) const
{
    return impl_->serialize(_Wostream);
}

size_t xlsx_shared_strings::add(const std::wstring & content)
{
    return impl_->add(content);
}

}
}

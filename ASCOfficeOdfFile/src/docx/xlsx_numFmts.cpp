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

#include "xlsx_numFmts.h"

namespace cpdoccore {
namespace oox {

struct xlsx_num_fmts::Impl
{
    std::vector<std::wstring> formats_;

    static size_t transform_id(size_t id)
    {
        return id + 164;
    }

    size_t num_format_id(const std::wstring & format_code);
};

size_t xlsx_num_fmts::Impl::num_format_id(const std::wstring & format_code)
{
    for (size_t i = 0; i < formats_.size(); ++i)
    {
        if (formats_[i] == format_code)
            return transform_id(i);
    }

    formats_.push_back(format_code);
    return transform_id(formats_.size() - 1);            
}

xlsx_num_fmts::xlsx_num_fmts() : impl_(new xlsx_num_fmts::Impl())
{
}

xlsx_num_fmts::~xlsx_num_fmts()
{}

size_t xlsx_num_fmts::num_format_id(const std::wstring & format_code)
{
    return impl_->num_format_id(format_code);
}

void xlsx_num_fmts::serialize(std::wostream & _Wostream) const
{
    _Wostream << L"<numFmts count=\"" << impl_->formats_.size() << L"\">";
    for (size_t i = 0; i < impl_->formats_.size(); ++i)
    {
        _Wostream << L"<numFmt formatCode=\"" << xml::utils::replace_text_to_xml(impl_->formats_[i]) <<
            L"\" numFmtId=\"" << impl_->transform_id(i) << "\" />";
    }
        
    _Wostream << L"</numFmts>";
}


   
}
}

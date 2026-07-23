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

#include "xlsx_complex_number_format.h"
#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore {
namespace oox {

class complex_number_format::Impl
{
public:
    std::wstring result_value()
    {
        std::wstring positive = (!positive_.empty() ? positive_ : default_);
        std::wstring negative = (!negative_.empty() ? negative_ : default_);
        std::wstring zero = (!zero_.empty() ? zero_ : default_);

        if (zero == positive && zero == negative)
        {
            return positive;
        }

        return positive + L";" + negative + (!zero.empty() ? (L";" + zero) : std::wstring(L""));        
    }

    void add_format(std::wstring const & condition, std::wstring const & format)
    {
        std::wstring c = XmlUtils::GetLower(condition);

        XmlUtils::replace_all( c, L" ", L"");
        XmlUtils::replace_all( c, L"\t", L"");

        if (L"" == c)
        {
            default_ = format;
        }
        else if (L"value()>0" == c)
        {
            positive_ = format;               
        } 
        else if (L"value()>=0" == c)
        {
            positive_ = format;
            zero_ = format;
        }
        else if (L"value()<=0" == c)
        {
            negative_ = format;
            zero_ = format;
        }
        else if (L"value()<0" == c)
        {
            negative_ = format;
        }
        else if (L"value()=0" == c)
        {
            zero_ = format;
        }
        else 
        {
            default_ = format; // unknown
        }
    }

private:
    std::wstring default_;
    std::wstring positive_;
    std::wstring negative_;
    std::wstring zero_;
};

complex_number_format::complex_number_format() : impl_(new Impl())
{}

complex_number_format::~complex_number_format()
{
}

void complex_number_format::reset()
{
    impl_.reset(new Impl());
}

std::wstring complex_number_format::result_value()
{
    return impl_->result_value();
}

void complex_number_format::add_format(std::wstring const & condition, std::wstring const & format)
{
    return impl_->add_format(condition, format);
}

}
}

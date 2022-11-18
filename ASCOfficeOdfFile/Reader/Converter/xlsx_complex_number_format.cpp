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

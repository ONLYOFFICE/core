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

#include "referenceformat.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { 
namespace odf_types { 
std::wostream & operator << (std::wostream & _Wostream, const reference_format & _Val)
{
    switch(_Val.get_type())
    {
	case reference_format::chapter:
        _Wostream << L"chapter";
        break;
	case reference_format::direction:
        _Wostream << L"direction";
        break;
    case reference_format::caption:
        _Wostream << L"caption";
        break;
    case reference_format::category_and_value:
        _Wostream << L"category-and-value";
        break;
    case reference_format::value:
        _Wostream << L"value";
        break;
    case reference_format::number:
        _Wostream << L"number";
        break;
    case reference_format::number_all_superior:
        _Wostream << L"number-all-superior";
        break;
    case reference_format::number_no_superior:
        _Wostream << L"number-no-superior";
        break;
    default:
    case reference_format::text:
        _Wostream << L"text";
        break;
  }
    return _Wostream;
}

reference_format reference_format::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"chapter")
        return reference_format(chapter);
    else if (tmp == L"direction")
        return reference_format(direction);
    else if (tmp == L"text")
        return reference_format(text);
    else if (tmp == L"caption")
        return reference_format(caption);
    else if (tmp == L"category-and-value")
        return reference_format(category_and_value);
    else if (tmp == L"value")
        return reference_format(value);
    else if (tmp == L"number")
        return reference_format(number);
    else if (tmp == L"number-all-superior")
        return reference_format(number_all_superior);
    else if (tmp == L"number-no-superior")
        return reference_format(number_no_superior);
    else
    {
        return reference_format(text);
    }
}
}
}

/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

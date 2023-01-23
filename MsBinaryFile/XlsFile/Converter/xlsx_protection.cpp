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

#include <ostream>
#include <boost/functional/hash/hash.hpp>

#include "xlsx_protection.h"

namespace oox {

bool xlsx_protection::operator == (const xlsx_protection & rVal) const
{
     const bool res = 
         hidden.get_value_or(false) == rVal.hidden.get_value_or(false) &&
         locked.get_value_or(false) == rVal.locked.get_value_or(false);
     return res;
}

bool xlsx_protection::operator != (const xlsx_protection & rVal) const
{
    return !(this->operator ==(rVal));
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_protection & protection)
{
    _Wostream << L"<protection ";
    
    if (protection.hidden)
        _Wostream << L"hidden=\"" << protection.hidden.get() << L"\" ";
    if (protection.locked)
        _Wostream << L"locked=\"" << protection.locked.get() << L"\" ";

    _Wostream << L"/>";    
}

std::size_t hash_value(xlsx_protection const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.hidden.get_value_or(false));
    boost::hash_combine(seed, val.locked.get_value_or(false));
    return seed;
}
}


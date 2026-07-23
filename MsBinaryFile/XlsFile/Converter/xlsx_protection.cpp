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


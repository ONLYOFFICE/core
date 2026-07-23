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
#include <boost/algorithm/string.hpp>

#include "bibliography.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const bibliography & _Val)
{
    switch(_Val.get_type())
    {
    case bibliography::article:
        _Wostream << L"article";
        break;
    case bibliography::book:
        _Wostream << L"book";
        break;
    case bibliography::booklet:
        _Wostream << L"booklet";
        break;
    case bibliography::conference:
        _Wostream << L"conference";
        break;
    case bibliography::email:
        _Wostream << L"email";
        break;
    case bibliography::inbook:
        _Wostream << L"inbook";
        break;
    case bibliography::incollection:
        _Wostream << L"incollection";
        break;
    case bibliography::inproceedings:
        _Wostream << L"inproceedings";
        break;
    case bibliography::journal:
        _Wostream << L"journal";
        break;
    case bibliography::manual:
        _Wostream << L"manual";
        break;
    case bibliography::mastersthesis:
        _Wostream << L"mastersthesis";
        break;
    case bibliography::misc:
        _Wostream << L"misc";
        break;
    case bibliography::phdthesis:
        _Wostream << L"phdthesis";
        break;
    case bibliography::proceedings:
        _Wostream << L"proceedings";
        break;
    case bibliography::techreport:
        _Wostream << L"techreport";
        break;
    case bibliography::unpublished:
        _Wostream << L"unpublished";
        break;
    case bibliography::www:
        _Wostream << L"www";
        break;
    default:
        break;
    }
    return _Wostream;    
}

bibliography bibliography::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"article")
        return bibliography( article );
    if (tmp == L"book")
        return bibliography( book );
    if (tmp == L"booklet")
        return bibliography( booklet );
    if (tmp == L"conference")
        return bibliography( conference );
    if (tmp == L"email")
        return bibliography( email );
    if (tmp == L"inbook")
        return bibliography( inbook );
    if (tmp == L"incollection")
        return bibliography( incollection );
    if (tmp == L"inproceedings")
        return bibliography( inproceedings );
    if (tmp == L"journal")
        return bibliography( journal );
    if (tmp == L"manual")
        return bibliography( manual );
    if (tmp == L"mastersthesis")
        return bibliography( mastersthesis );
    if (tmp == L"misc")
        return bibliography( misc );
    if (tmp == L"phdthesis")
        return bibliography( phdthesis );
    if (tmp == L"proceedings")
        return bibliography( proceedings );
    if (tmp == L"techreport")
        return bibliography( techreport );
    if (tmp == L"unpublished")
        return bibliography( unpublished );
    if (tmp == L"www")
        return bibliography( www );
    else
    {
        return bibliography( book );
    }
}

} }

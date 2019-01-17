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

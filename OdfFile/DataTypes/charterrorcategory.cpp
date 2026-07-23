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

#include "charterrorcategory.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const chart_error_category & _Val)
{
	switch(_Val.get_type())
	{
	case   chart_error_category::none:			_Wostream <<  L"none"; break;
	case   chart_error_category::variance:		 _Wostream <<  L"variance"; break;
	case   chart_error_category::standard_deviation: _Wostream <<  L"standard-deviation"; break;
	case   chart_error_category::percentage:	 _Wostream <<  L"percentage"; break;
	case   chart_error_category::error_margin:	 _Wostream <<  L"error-margin"; break;
	case   chart_error_category::constant:		 _Wostream <<  L"constant"; break;
	}
    return _Wostream;    
}

chart_error_category chart_error_category::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_error_category( none );
    else if (tmp == L"variance")
        return chart_error_category( variance );
    else if (tmp == L"standard-deviation")
        return chart_error_category( standard_deviation );
    else if (tmp == L"percentage")
        return chart_error_category( percentage );
    else if (tmp == L"error-margin")
        return chart_error_category( error_margin );
    else if (tmp == L"constant")
        return chart_error_category( constant );
    else
    {
        return chart_error_category( none );
    }
}

} }

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

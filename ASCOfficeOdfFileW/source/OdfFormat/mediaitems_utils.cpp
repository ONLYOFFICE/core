/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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


#include "mediaitems_utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace cpdoccore { 
namespace odf_writer{
namespace utils {

namespace media {

std::wstring get_rel_type(_mediaitems::Type type)
{
    switch (type)
    {
    case _mediaitems::typeImage:
        return L"";
   default:
        return L"";
    }
}

std::wstring get_default_file_name(_mediaitems::Type type)
{
    switch (type)
    {
    case _mediaitems::typeImage:
        return L"image";
	default:
        return L"media";
    }
}

std::wstring create_file_name(const std::wstring & uri, _mediaitems::Type type, size_t Num)
{
 	std::wstring sExt;
	int n = uri.rfind(L".");
	if (n>=0) sExt = uri.substr(n);
	//todooo проверить
	
	return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + sExt;
}


}

}
}
}

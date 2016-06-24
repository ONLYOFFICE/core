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
#include <boost/algorithm/string.hpp>

#include "markerstyle.h"

#include <ostream>

namespace cpdoccore { 
namespace odf_types {

std::wostream & operator << (std::wostream & _Wostream, const marker_style & _Val)
{
    //switch(_Val.get_type())
    //{
    //case marker_style::
    //    _Wostream << "none";
    //    break;
    //case marker_style::Single:
    //    _Wostream << "single";
    //    break;
    //case marker_style::Double:
    //    _Wostream << "double";
    //    break;
    //default:
    //    break;
    //}
	_Wostream << _Val.get();
    return _Wostream;
}

std::wstring marker_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	int pos1,pos2;
	if ((pos1 = tmp.find(L"arrow"))>=0 && (pos2 =tmp.find(L"line"))>=0)	
		return L"arrow";
    else if ((pos1 =tmp.find(L"square"))>=0 || (pos2 =tmp.find(L"diamond"))>=0) 
		return L"diamond";
    else if ((pos1 =tmp.find(L"circle"))>=0)
		return L"oval";
    else
		return L"triangle";

		//Arrow_20_concave, 
		//Circle, 
		//Dimension_20_Lines, 
		//Double_20_Arrow, 
		//Line_20_Arrow, 
		//Rounded_20_large_20_Arrow, 
		//Rounded_20_short_20_Arrow, 
		//Small_20_Arrow, 
		//Square_20_45, and 
		//Symmetric_20_Arrow
}

//stealth (Stealth Arrow) Stealth arrow head ----??????


} }
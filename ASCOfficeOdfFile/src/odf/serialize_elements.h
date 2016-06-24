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
#pragma once

#include "office_elements.h"
#include <boost/foreach.hpp>

namespace cpdoccore {
namespace odf_reader {

inline ::std::wostream & serialize_elements(::std::wostream & _Wostream, const office_element_ptr & elm)
{
    if (elm)
        elm->xml_to_stream(_Wostream);
    return _Wostream;
}

inline ::std::wostream & serialize_elements(::std::wostream & _Wostream, const office_element_ptr_array & elms)
{
    BOOST_FOREACH(const office_element_ptr & elm, elms)
    {
        serialize_elements(_Wostream, elm);
    }
    return _Wostream;
}

inline ::std::wostream & serialize_elements_text(::std::wostream & _Wostream, const office_element_ptr & elm)
{
    if (elm)
        elm->text_to_stream(_Wostream);
    return _Wostream;
}

inline ::std::wostream & serialize_elements_text(::std::wostream & _Wostream, const office_element_ptr_array & elms)
{
    BOOST_FOREACH(const office_element_ptr & elm, elms)
    {
        serialize_elements_text(_Wostream, elm);
    }
    return _Wostream;
}

#define CP_SERIALIZE_XML(ELEMENT) serialize_elements(_Wostream, (ELEMENT))
#define CP_SERIALIZE_TEXT(ELEMENT) serialize_elements_text(_Wostream, (ELEMENT))


}
}

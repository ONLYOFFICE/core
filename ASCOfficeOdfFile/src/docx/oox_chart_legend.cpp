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


#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_chart_legend.h"

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

void oox_chart_legend::oox_serialize(std::wostream & _Wostream)
{
	if (content_.bEnabled == false) return;

	oox_chart_shape shape;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:legend")
        {

			CP_XML_NODE(L"c:legendPos")
			{
				CP_XML_ATTR(L"val", "r");//  "b" | "l" |  "r" |  "t"// == bottom left right top

			}
            layout_.oox_serialize(CP_XML_STREAM());		
			
			CP_XML_NODE(L"c:overlay")
			{
				CP_XML_ATTR(L"val", 0);
			}

			shape.set(content_.graphic_properties_, content_.fill_);
			shape.oox_serialize(CP_XML_STREAM());

			oox_serialize_default_text(CP_XML_STREAM(), content_.text_properties_);

		}
    }

}
}
}

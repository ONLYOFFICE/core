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

#include "oox_layout.h"
#include <boost/functional.hpp>

#include <xml/simple_xml_writer.h>
#include "../Format/style_text_properties.h"

namespace cpdoccore {
namespace oox {


void oox_layout::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:layout")
        {
			//CP_XML_NODE(L"c:manualLayout") //вручную созданное описание и место
			//{
			//	CP_XML_NODE(L"c:layoutTarget") //вручную созданное описание и место
			//	{
			//		CP_XML_ATTR(L"val", "inner"); //все позиции относительно внешнего объекта (а не документа в целом)
			//	}
			//	//<c:xMode val="edge"/> 
			//	//<c:yMode val="edge"/>
			//	//<c:x val="3.9382768941490093E-2"/>
			//	//<c:y val="0.37756627842484747"/>
			//	//<c:w val="0.67611762675758824"/>
			//	//<c:h val="0.53420410427066001"/>
			//}
        }
    }

}

}
}

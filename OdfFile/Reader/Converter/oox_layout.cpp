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
			if( x || y || w || h )
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					CP_XML_NODE(L"c:layoutTarget")
					{
						CP_XML_ATTR(L"val", L"outer");
					}
					CP_XML_NODE(L"c:xMode")
					{
						CP_XML_ATTR(L"val", L"edge");
					}
					CP_XML_NODE(L"c:yMode")
					{
						CP_XML_ATTR(L"val", L"edge");
					}
					if( x )
					{
						CP_XML_NODE(L"c:x")
						{
							CP_XML_ATTR(L"val", std::to_wstring(*x));
						}
					}
					if( y )
					{
						CP_XML_NODE(L"c:y")
						{
							CP_XML_ATTR(L"val", std::to_wstring(*y));
						}
					}
					if( w )
					{
						CP_XML_NODE(L"c:w")
						{
							CP_XML_ATTR(L"val", std::to_wstring(*w));
						}
					}
					if( h )
					{
						CP_XML_NODE(L"c:h")
						{
							CP_XML_ATTR(L"val", std::to_wstring(*h));
						}
					}
				}
			}
        }
    }

}

}
}

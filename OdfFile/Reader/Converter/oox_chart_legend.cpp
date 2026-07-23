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


#include <boost/functional.hpp>
#include <xml/simple_xml_writer.h>

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
				CP_XML_ATTR(L"val", content_.position);

			}
			layout_.x = content_.x;
			layout_.y = content_.y;
			layout_.w = content_.width;
			layout_.h = content_.height;
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

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

#include "oox_title.h"
#include <boost/functional.hpp>

#include <xml/simple_xml_writer.h>
#include "../Format/odfcontext.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_chart_properties.h"

namespace cpdoccore {
namespace oox {

void oox_title::oox_content_serialize(std::wostream & _Wostream, odf_reader::chart::title & content)
{
	if (content_.content_.empty()) return;

	_CP_OPT(int) rotateVal;
	if (content.properties_)
	{
		rotateVal = content.properties_->common_rotation_angle_attlist_.style_rotation_angle_;
	}

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"a:p")
		{
			CP_XML_NODE(L"a:pPr")
			{
				CP_XML_NODE(L"a:defRPr"){}
			}
			CP_XML_NODE(L"a:r")
			{
				//odf_reader::fonts_container & fonts = context.fontContainer();
				odf_reader::fonts_container fonts;
				if (content.text_properties_)
				{
					content.text_properties_->oox_serialize(CP_XML_STREAM(), true, fonts);
				}

				CP_XML_NODE(L"a:t")
				{
					CP_XML_CONTENT(content.content_);
				}
			}
		}
	}
}

void oox_title::oox_serialize(std::wostream & _Wostream)
{
	if (!content_.bEnabled && !sub_.bEnabled) return;
	
	_CP_OPT(int) rotateVal;
	if (content_.properties_)
	{
		rotateVal = content_.properties_->common_rotation_angle_attlist_.style_rotation_angle_;
	}

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:title")
        {
			if (false == content_.content_.empty() || false == sub_.content_.empty())
			{
				CP_XML_NODE(L"c:tx")
				{
					CP_XML_NODE(L"c:rich")
					{
						CP_XML_NODE(L"a:bodyPr")
						{
							if (rotateVal)
							{
								CP_XML_ATTR(L"rot", -(int)(*rotateVal * 60000));
							}
						}
						CP_XML_NODE(L"a:lstStyle"){}
						
						oox_content_serialize(CP_XML_STREAM(), content_);
						oox_content_serialize(CP_XML_STREAM(), sub_);				
					}
				}
			}
			else if (content_.text_properties_)
			{
				CP_XML_NODE(L"c:txPr")
				{
					CP_XML_NODE(L"a:bodyPr")
					{
						if (rotateVal)
						{
							double d = 360 - (*rotateVal) * 180. / 3.14159265358979323846;
							CP_XML_ATTR(L"rot", (int)(d * 60000));
						}
					}
					CP_XML_NODE(L"a:lstStyle"){}
					
					CP_XML_NODE(L"a:p")
					{
						CP_XML_NODE(L"a:pPr")
						{
							CP_XML_NODE(L"a:defRPr")
							{
								//odf_reader::fonts_container & fonts = context.fontContainer();
								odf_reader::fonts_container fonts;
								content_.text_properties_->oox_serialize(CP_XML_STREAM(), true, fonts);
							}
						}
					}			
				}
			}
            layout_.oox_serialize(CP_XML_STREAM());
			CP_XML_NODE(L"c:overlay")
			{
				CP_XML_ATTR(L"val", 0);
			}
			
			oox_chart_shape shape;
			shape.set(content_.graphic_properties_, content_.fill_);
			shape.oox_serialize(CP_XML_STREAM());
		}
    }

}
}
}

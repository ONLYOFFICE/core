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
#include <boost/algorithm/string.hpp>

#include <xml/simple_xml_writer.h>
#include "../Format/odfcontext.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_chart_properties.h"

namespace cpdoccore {
namespace oox {

void oox_title::oox_content_serialize(std::wostream & _Wostream, odf_reader::chart::title & content)
{
	if (content.content_.empty()) return;

	std::vector<std::vector<odf_reader::chart::title::part> > lines(1);

	if (content.parts_.empty())
	{
		std::vector<std::wstring> strings;
		boost::algorithm::split(strings, content.content_, boost::algorithm::is_any_of(L"\n"));

		for (size_t i = 0; i < strings.size(); i++)
		{
			if (false == strings[i].empty() && strings[i][strings[i].length() - 1] == L'\r')
				strings[i].erase(strings[i].length() - 1);

			odf_reader::chart::title::part p;
			p.content_ = strings[i];

			if (i > 0) lines.push_back(std::vector<odf_reader::chart::title::part>());
			lines.back().push_back(p);
		}
	}
	else
	{
		for (size_t i = 0; i < content.parts_.size(); i++)
		{
			if (content.parts_[i].new_line_)
			{
				lines.push_back(std::vector<odf_reader::chart::title::part>());
				continue;
			}
			lines.back().push_back(content.parts_[i]);
		}
	}

	CP_XML_WRITER(_Wostream)
	{
		for (size_t l = 0; l < lines.size(); l++)
		{
			CP_XML_NODE(L"a:p")
			{
				CP_XML_NODE(L"a:pPr")
				{
					CP_XML_NODE(L"a:defRPr"){}
				}
				for (size_t i = 0; i < lines[l].size(); i++)
				{
					CP_XML_NODE(L"a:r")
					{
						//odf_reader::fonts_container & fonts = context.fontContainer();
						odf_reader::fonts_container fonts;

						odf_reader::text_format_properties_ptr text_properties = lines[l][i].text_properties_ ?
								lines[l][i].text_properties_ : content.text_properties_;

						if (text_properties)
						{
							text_properties->oox_serialize(CP_XML_STREAM(), true, fonts);
						}

						CP_XML_NODE(L"a:t")
						{
							CP_XML_CONTENT(lines[l][i].content_);
						}
					}
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
		rotateVal = odf_reader::oox_convert_rotation_angle(content_.properties_->common_rotation_angle_attlist_.style_rotation_angle_);
	}

	const bool bTitleText = (false == content_.content_.empty() || false == sub_.content_.empty());

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:title")
        {
			if (bTitleText)
			{
				CP_XML_NODE(L"c:tx")
				{
					CP_XML_NODE(L"c:rich")
					{
						CP_XML_NODE(L"a:bodyPr")
						{
							if (rotateVal)
							{
								CP_XML_ATTR(L"rot", *rotateVal);
								CP_XML_ATTR(L"vert", L"horz");
							}
						}
						CP_XML_NODE(L"a:lstStyle"){}
						
						oox_content_serialize(CP_XML_STREAM(), content_);
						oox_content_serialize(CP_XML_STREAM(), sub_);				
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

			if (false == bTitleText && content_.text_properties_)
			{
				CP_XML_NODE(L"c:txPr")
				{
					CP_XML_NODE(L"a:bodyPr")
					{
						if (rotateVal)
						{
							CP_XML_ATTR(L"rot", *rotateVal);
							CP_XML_ATTR(L"vert", L"horz");
						}
					}
					CP_XML_NODE(L"a:lstStyle"){}
					
					CP_XML_NODE(L"a:p")
					{
						CP_XML_NODE(L"a:pPr")
						{
							//odf_reader::fonts_container & fonts = context.fontContainer();
							odf_reader::fonts_container fonts;
							content_.text_properties_->oox_serialize(CP_XML_STREAM(), true, fonts, true);
						}
					}			
				}
			}
		}
    }

}
}
}

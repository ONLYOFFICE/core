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

#include "oox_title.h"
#include <boost/functional.hpp>

#include <xml/simple_xml_writer.h>
#include "../Format/odfcontext.h"
#include "../Format/style_text_properties.h"

namespace cpdoccore {
namespace oox {

void oox_title::oox_content_serialize(std::wostream & _Wostream, odf_reader::chart::title & content)
{
	if (content_.content_.empty()) return;

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
					content.text_properties_->oox_serialize(CP_XML_STREAM(), true, fonts);

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
						CP_XML_NODE(L"a:bodyPr"){}
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
					CP_XML_NODE(L"a:bodyPr"){}
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

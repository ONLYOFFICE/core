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
#include "draw_page.h"

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>
#include "../formulasconvert/formulasconvert.h"

#include <boost/lexical_cast.hpp>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>

#include "calcs_styles.h"
#include "draw_common.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 

    using namespace odf_types;

namespace odf_reader {

void draw_page::pptx_convert(oox::pptx_conversion_context & Context)
{
	const std::wstring pageStyleName	= draw_page_attr_.draw_style_name_.get_value_or(L"");
    const std::wstring pageName			= draw_page_attr_.draw_name_.get_value_or(L"");
    const std::wstring layoutName		= draw_page_attr_.page_layout_name_.get_value_or(L"");
    const std::wstring masterName		= draw_page_attr_.master_page_name_.get_value_or(L"");

    _CP_LOG << L"[info][xlsx] process page(slide) \"" << pageName /*L"" */<< L"\"" << std::endl;

    Context.start_page(pageName, pageStyleName, layoutName,masterName);

	if (draw_page_attr_.draw_style_name_)
	{
		style_instance * style_inst = Context.root()->odf_context().styleContainer().style_by_name(pageStyleName,style_family::DrawingPage,false);

		if ((style_inst) && (style_inst->content()))
		{
			const style_drawing_page_properties * properties = style_inst->content()->get_style_drawing_page_properties();

			if (properties)
			{				
				oox::_oox_fill fill;
				Compute_GraphicFill(properties->content().common_draw_fill_attlist_, Context.root()->odf_context().drawStyles() ,fill);
				Context.get_slide_context().add_background(fill);
			}
		}
	}
	BOOST_FOREACH(const office_element_ptr& elm, content_)
    {
		elm->pptx_convert(Context);
	}

    Context.end_page();
}

}
}

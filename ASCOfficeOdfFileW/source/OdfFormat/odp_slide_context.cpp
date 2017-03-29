/*
 * (c) Copyright Ascensio System SIA 2010-2017
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


#include "odp_slide_context.h"
//#include "table.h"

#include "odp_conversion_context.h"
#include "logging.h"

#include <iostream>


namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

odp_slide_context::odp_slide_context(odp_conversion_context & Context): context_(Context)
{        
}

odp_page_state & odp_slide_context::state()
{
    return page_state_list_.back();
}

void odp_slide_context::start_page(office_element_ptr & elm)
{
	page_state_list_.push_back( odp_page_state(&context_, elm) );
	
	std::wstring style_name_new = L"dp" + boost::lexical_cast<std::wstring>(page_state_list_.size());

	office_element_ptr & style = context_.styles_context()->add_or_find(style_name_new, style_family::DrawingPage, true);
	style->create_child_element(L"style", L"drawing-page-properties");


	state().set_page_style(style);

	//для свойств страницы, а не таблицы - нужно создать master-page c page layout и связать по имени со стилем таблицы
	//причем здесь, т.к. с другой стороны это ВСЕ еще свойства листа. то есть совйства листа разделить на свйства страницы и таблицы ..
	//todooo
	//????
}

void odp_slide_context::end_page()
{
	state().drawing_context()->finalize(state().draw_page_);
}

}
}

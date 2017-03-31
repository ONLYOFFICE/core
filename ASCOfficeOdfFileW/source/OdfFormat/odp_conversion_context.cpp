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

#include <boost/algorithm/string.hpp>

#include "../utils.h"

#include "odp_conversion_context.h"
#include "office_presentation.h"

#include "styles.h"
#include "style_table_properties.h"

#include "odf_text_context.h"
#include "paragraph_elements.h"
#include "odf_settings_context.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {


odp_conversion_context::odp_conversion_context(package::odf_document * outputDocument) 
	:	odf_conversion_context		(PresentationDocument, outputDocument), slide_context_(*this)
		//, current_text_context_(NULL)
{
}


void odp_conversion_context::start_document()
{
	start_presentation();
	
	root_presentation_ = dynamic_cast<office_presentation*>(get_current_object_element().get());
}

void odp_conversion_context::end_document()
{
	odf_conversion_context::end_document();
}
void odp_conversion_context::start_slide()
{
	slide_context_.set_styles_context(styles_context());

	create_element(L"draw", L"page", root_presentation_->pages_, this);		
	slide_context_.start_page(root_presentation_->pages_.back());	
	
	drawing_context()->set_presentation(false);
}

void odp_conversion_context::start_master_slide(std::wstring name)
{
	slide_context_.set_styles_context(page_layout_context()->get_local_styles_context());
	
	page_layout_context()->add_master_page(name);	
	slide_context_.start_page(page_layout_context()->last_master()->get_root());
	
	drawing_context()->set_presentation(true);	
}

void odp_conversion_context::end_master_slide()
{
	slide_context_.end_page();
}

void odp_conversion_context::end_slide()
{
	slide_context_.end_page();
}

void odp_conversion_context::start_text_context()
{
	current_text_context_ = new odf_text_context(this);

}
void odp_conversion_context::end_text_context()
{
	if (current_text_context_)
		delete current_text_context_;
	current_text_context_ = NULL;
}

void odp_conversion_context::start_drawings()
{
}
void odp_conversion_context::end_drawings()
{
	current_slide().drawing_context()->clear();
}
void odp_conversion_context::start_image(const std::wstring & image_file_name)
{
	std::wstring odf_ref_name ;
	
	mediaitems()->add_or_find(image_file_name,_mediaitems::typeImage,odf_ref_name);

	current_slide().drawing_context()->start_image(odf_ref_name);
}
}
}

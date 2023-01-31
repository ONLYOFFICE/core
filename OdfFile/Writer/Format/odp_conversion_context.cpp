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

#include <boost/algorithm/string.hpp>

#include "../../Common/utils.h"

#include "odp_conversion_context.h"
#include "office_presentation.h"

#include "draw_page.h"

#include "styles.h"
#include "style_table_properties.h"

#include "odf_text_context.h"
#include "paragraph_elements.h"
#include "odf_settings_context.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {


odp_conversion_context::odp_conversion_context(package::odf_document * outputDocument) 
	:	odf_conversion_context (PresentationDocument, outputDocument), root_presentation_(NULL), slide_context_(*this)
{
}
odf_text_context* odp_conversion_context::text_context()
{
	if (false == text_context_.empty())
	{
		return text_context_.back().get();
	}
	else
	{
		return NULL;
	}
}
odp_slide_context* odp_conversion_context::slide_context()
{
	return &slide_context_;
}
odf_controls_context* odp_conversion_context::controls_context()
{
	if (slide_context_.page_state_list_.empty()) return NULL;
	
	return slide_context_.state().controls_context();
}
odf_drawing_context* odp_conversion_context::drawing_context()
{
	if (false == drawing_context_.empty())
	{
		return drawing_context_.back().get();
	}
	else if (false == slide_context_.page_state_list_.empty())
	{
		return slide_context_.state().drawing_context();
	}
	else
	{
		return NULL;
	}
}
odf_comment_context* odp_conversion_context::comment_context()
{
	if (slide_context_.page_state_list_.empty()) return NULL;
	
	return slide_context_.state().comment_context();
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

size_t odp_conversion_context::get_pages_count()
{
	return root_presentation_->pages_.size();
}
void odp_conversion_context::start_slide()
{
	slide_context_.set_styles_context(odf_conversion_context::styles_context());

	create_element(L"draw", L"page", root_presentation_->pages_, this);		
	slide_context_.start_page(root_presentation_->pages_.back());	
	
	drawing_context()->set_presentation(0);
}
void odp_conversion_context::end_slide()
{
	slide_context_.end_page();
}
void odp_conversion_context::start_master_slide(std::wstring name)
{
	slide_context_.set_styles_context(page_layout_context()->get_local_styles_context());
	
	page_layout_context()->add_master_page(name);	
	slide_context_.start_page(page_layout_context()->last_master()->get_root());
	
	drawing_context()->set_presentation(1);	
}
void odp_conversion_context::end_master_slide()
{
	slide_context_.end_page();
	slide_context_.set_styles_context(NULL); //возврат на базовый
}
void odp_conversion_context::start_layout_slide()
{
	slide_context_.set_styles_context(page_layout_context()->get_local_styles_context());
	
	office_element_ptr elm = page_layout_context()->add_presentation_layout_page();	
	
	slide_context_.start_page(elm);
	
	drawing_context()->set_presentation(1);	
}
void odp_conversion_context::end_layout_slide()
{
	slide_context_.end_page();
	slide_context_.set_styles_context(NULL); //возврат на базовый
}

odf_style_context* odp_conversion_context::styles_context()	
{
	odf_style_context* result = slide_context_.get_styles_context();

	if (!result) result = odf_conversion_context::styles_context();
	
	return result;
}

void odp_conversion_context::start_note(bool bMaster)
{
	office_element_ptr note_elm;
	create_element(L"presentation", L"notes", note_elm, this);
	
	current_slide().drawing_context()->start_drawing();
	current_slide().drawing_context()->start_element(note_elm);
	
	slide_context_.start_page(note_elm);	

	if (bMaster)
	{
		page_layout_context()->create_layout_page();

		odf_writer::presentation_notes* notes = dynamic_cast<odf_writer::presentation_notes*>(note_elm.get());
		notes->attlist_.style_page_layout_name_ = page_layout_context()->last_layout()->get_name();
	
		drawing_context()->set_presentation(2);	
	}
	else
		drawing_context()->set_presentation(0);	
}
void odp_conversion_context::start_comment(int oox_comm_id)
{
	office_element_ptr comm_elm;
	create_element(L"office", L"annotation", comm_elm, this);

	current_slide().comment_context()->start_comment(comm_elm, oox_comm_id);
	
	current_slide().drawing_context()->start_drawing();
	current_slide().drawing_context()->start_element(comm_elm);
}
void odp_conversion_context::start_comment_content()
{
	current_slide().comment_context()->start_comment_content();
	
	office_element_ptr & root_comm_element = current_slide().drawing_context()->get_current_element();
	start_text_context();

	text_context()->start_element(root_comm_element);
	text_context()->start_paragraph();
}
void odp_conversion_context::end_comment_content()
{
	text_context()->end_paragraph();

	current_slide().comment_context()->end_comment_content();

	text_context()->end_element();
	end_text_context();
}
void odp_conversion_context::end_comment()
{
	current_slide().drawing_context()->end_element();
	current_slide().drawing_context()->end_drawing();
}
void odp_conversion_context::end_note()
{
	slide_context_.end_page();

	slide_context_.remove_page();

	current_slide().drawing_context()->end_element();
	current_slide().drawing_context()->end_drawing();
}


}
}

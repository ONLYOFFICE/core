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

#include "logging.h"

#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>

#include "odp_page_state.h"
#include "odf_text_context.h"
#include "odp_conversion_context.h"

#include "draw_page.h"
#include "office_annotation.h"
#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {


///////////////////////////////////////////////////////////////

odp_page_state::odp_page_state(odf_conversion_context * Context, office_element_ptr & elm) 
		 : context_(Context), drawing_context_(Context), comment_context_(Context)
{     
	page_elm_ = elm; 

}

void odp_page_state::set_page_name(std::wstring name)
{
	if (name.empty()) return;

	office_page_name_ = name;
	
	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page) 
		page->draw_page_attr_.draw_name_ = name;
	else
	{
		//style_master_page *master_page = dynamic_cast<style_master_page*>(page_elm_.get());
		//if (master_page)
		//	master_page->style_master_page_attlist_.style_display_name_ = name;
	}
}

void odp_page_state::set_layout_page(std::wstring name)
{
	if (name.empty())return;
	
	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page == NULL)return;

	page->draw_page_attr_.page_layout_name_ = name;
}

void odp_page_state::set_master_page(std::wstring name)
{
	if (name.empty())return;
	
	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page == NULL)return;

	page->draw_page_attr_.master_page_name_ = name;
}

void odp_page_state::set_page_style(office_element_ptr & elm)
{	
	page_style_elm_ = elm;

	style* office_page_style_ = dynamic_cast<style*>(elm.get());

	if (!office_page_style_)return;

	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page)
		page->draw_page_attr_.draw_style_name_ = office_page_style_->style_name_;
	else
	{
		style_master_page *master_page = dynamic_cast<style_master_page*>(page_elm_.get());
		if (master_page)
			master_page->style_master_page_attlist_.draw_style_name_ = office_page_style_->style_name_;
	}
}
void odp_page_state::add_child_element( const office_element_ptr & child_element)
{
	page_elm_->add_child_element(child_element);
}

}
}

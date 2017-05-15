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
#include "anim_elements.h"
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
void odp_page_state::set_page_id(int id)
{
	page_id_ = id;
}
void odp_page_state::set_page_name(std::wstring name)
{
	if (name.empty()) return;

	page_name_ = name;
	
	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page) 
		page->attlist_.draw_name_ = name;
	else
	{
		//style_master_page *master_page = dynamic_cast<style_master_page*>(page_elm_.get());
		//if (master_page)
		//	master_page->attlist_.style_display_name_ = name;
	}
}

void odp_page_state::set_layout_page(std::wstring name)
{
	if (name.empty())return;
	
	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page == NULL)return;

	page->attlist_.presentation_page_layout_name_ = name;
}

void odp_page_state::set_master_page(std::wstring name)
{
	if (name.empty())return;
	
	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page == NULL)return;

	page->attlist_.draw_master_page_name_ = name;
}

void odp_page_state::set_page_style(office_element_ptr & elm)
{	
	page_style_elm_ = elm;

	style* office_page_style_ = dynamic_cast<style*>(elm.get());

	if (!office_page_style_)return;

	draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
	if (page)
		page->attlist_.draw_style_name_ = office_page_style_->style_name_;
	else
	{
		style_master_page *master_page = dynamic_cast<style_master_page*>(page_elm_.get());
		if (master_page)
			master_page->attlist_.draw_style_name_ = office_page_style_->style_name_;
		else
		{
			presentation_notes* notes = dynamic_cast<presentation_notes*>(page_elm_.get());
			if (notes)
				notes->attlist_.draw_style_name_ = office_page_style_->style_name_;
		}
	}
}
void odp_page_state::add_child_element( const office_element_ptr & child_element)
{
	page_elm_->add_child_element(child_element);
}
void odp_page_state::set_anim_id (int val)
{
	if (anim_levels.empty())		return;
	if (!anim_levels.back().attlist)return;

	anim_levels.back().id = val;

	//anim_levels.back().attlist->smil_begin_ = L"id" + std::to_wstring(val) + L".begin";
}
void odp_page_state::set_anim_type(std::wstring val)
{
	if (anim_levels.empty())		return;
	if (!anim_levels.back().attlist)return;
	
	if (val == L"tmRoot")
	{
		anim_levels.back().attlist->presentation_node_type_ = L"timing-root";
		if (page_transaction)
		{
			std::wstring slide_id = L"slide_id" + std::to_wstring(page_id_);

			draw_page* page = dynamic_cast<draw_page*>(page_elm_.get());
			if (page) 
			{
				page->attlist_.draw_id_ = slide_id;
				
				start_timing_par();
					anim_levels.back().attlist->smil_begin_ = slide_id + L".begin";
					anim_levels.back().elm->add_child_element(	page_transaction );
				end_timing_par();
			}
		}
	}
}
void odp_page_state::set_anim_duration(std::wstring val)
{
	if (anim_levels.empty())		return;
	if (!anim_levels.back().attlist)return;

	//if (val == L"indefinite")
		anim_levels.back().attlist->smil_dur_ = val;

}
void odp_page_state::set_anim_restart(std::wstring val)
{
	if (anim_levels.empty())		return;
	if (!anim_levels.back().attlist)return;
	
	anim_levels.back().attlist->smil_restart_ = val;
}
void odp_page_state::start_transition()
{
	create_element(L"anim", L"transitionFilter", page_transaction, context_);
}
void odp_page_state::set_transition_type(int val)
{
	anim_transitionFilter *trans = dynamic_cast<anim_transitionFilter*>(page_transaction.get());
	if (trans)
		trans->attlist_.smil_type_ = odf_types::smil_transition_type((odf_types::smil_transition_type::type)val);
}
void odp_page_state::set_transition_subtype(std::wstring val)
{
	anim_transitionFilter *trans = dynamic_cast<anim_transitionFilter*>(page_transaction.get());
	if (trans)
		trans->attlist_.smil_subtype_ = val;
}
void odp_page_state::set_transition_speed(int val)
{
	anim_transitionFilter *trans = dynamic_cast<anim_transitionFilter*>(page_transaction.get());
	if (trans)
	{
		if (val == 0)	trans->attlist_.smil_dur_ = odf_types::clockvalue(3000);
		if (val == 1)	trans->attlist_.smil_dur_ = odf_types::clockvalue(4000);
		if (val == 2)	trans->attlist_.smil_dur_ = odf_types::clockvalue(5000);
	}
}
void odp_page_state::set_transition_duration(int val)
{
	anim_transitionFilter *trans = dynamic_cast<anim_transitionFilter*>(page_transaction.get());
	if (trans)
		trans->attlist_.smil_dur_ = odf_types::clockvalue(val * 1000);
}
void odp_page_state::start_timing_seq()
{
	if (anim_levels.empty()) return;

	anim_state anim;
	create_element(L"anim", L"seq", anim.elm, context_);
	if (!anim.elm) return;
	
	anim_seq *seq = dynamic_cast<anim_seq*>(anim.elm.get());
	if (seq) anim.attlist = &seq->attlist_;

	anim_levels.back().empty = false;
	anim_levels.back().elm->add_child_element(anim.elm);
	
	anim_levels.push_back(anim);
}
void odp_page_state::end_timing_seq()
{
	anim_levels.pop_back();
	
}
void odp_page_state::start_timing_par()
{
	if (anim_levels.empty()) return;

	anim_state anim;
	create_element(L"anim", L"par", anim.elm, context_);
	if (!anim.elm) return;

	anim_par *par = dynamic_cast<anim_par*>(anim.elm.get());
	if (par) anim.attlist = &par->attlist_;

	anim_levels.back().empty = false;
	anim_levels.back().elm->add_child_element(anim.elm);

	anim_levels.push_back(anim);
}
void odp_page_state::end_timing_par()
{
	anim_levels.pop_back();
	
}
}
}

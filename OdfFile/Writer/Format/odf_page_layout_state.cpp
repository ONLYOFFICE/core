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

#include "logging.h"

#include "odf_page_layout_state.h"
#include "odf_conversion_context.h"

#include "styles.h"
#include "style_page_layout_properties.h" 

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

odf_master_state::odf_master_state(office_element_ptr & master_elm )
{        
	size_t level =0;
	odf_element_state state(master_elm, L"", office_element_ptr(), level);
	
	elements_.push_back(state);

	current_level_.push_back(master_elm);

}

office_element_ptr & odf_master_state::get_root()
{
	if (elements_.size() > 0)
		return elements_[0].elm;
	else
		throw;
}
office_element_ptr & odf_master_state::get_last_element()
{
	if (elements_.size() >0)
		return elements_.back().elm;
	else
		throw;
}
void odf_master_state::add_child(office_element_ptr & child_elm, office_element_ptr  style_elm, std::wstring style_name )
{
	size_t level = current_level_.size();
	odf_element_state state(child_elm, style_name, style_elm, level);
	
	elements_.push_back(state);
	current_level_.back()->add_child_element(child_elm);
}

void odf_master_state::set_name(std::wstring & name)
{
	style_master_page* style_ = dynamic_cast<style_master_page*>(elements_[0].elm.get());

	if (!style_)return;
	
	style_->attlist_.style_name_ = name;
}
void odf_master_state::set_display_name(std::wstring & name)
{
	style_master_page* style_ = dynamic_cast<style_master_page*>(elements_[0].elm.get());

	if (!style_)return;
	
	style_->attlist_.style_display_name_ = name;
}

void odf_master_state::set_layout_style_name(std::wstring name)
{
	style_master_page* style_ = dynamic_cast<style_master_page*>(elements_[0].elm.get());

	if (!style_)return;
	
	style_->attlist_.style_page_layout_name_ = name;
}
std::wstring odf_master_state::get_name()
{
	style_master_page* style_ = dynamic_cast<style_master_page*>(elements_[0].elm.get());
	if (!style_)return L"";

	return style_->attlist_.style_name_.get_value_or(L"");
}
void odf_master_state::add_footer(office_element_ptr & elm)
{
	style_master_page* style_ = dynamic_cast<style_master_page*>(elements_[0].elm.get());
	if (!style_)return;

	style_->add_child_element(elm);

	odf_element_state state(elm, L"", office_element_ptr(), 1);
	elements_.push_back(state);

}
void odf_master_state::add_header(office_element_ptr & elm)
{
	style_master_page* style_ = dynamic_cast<style_master_page*>(elements_[0].elm.get());
	if (!style_)return;

	style_->add_child_element(elm);

	odf_element_state state(elm, L"", office_element_ptr(), 1);
	elements_.push_back(state);

}
///////////////////////////////////////////////////////////////////////////////
odf_layout_state::odf_layout_state(office_element_ptr & layout_elm )
{        
	odf_element_state state(layout_elm);
	
	elements_.push_back(state);
	current_level_.push_back(layout_elm);
}

void odf_layout_state::add_child(office_element_ptr & child_elm, office_element_ptr  style_elm, std::wstring style_name )
{
	if (current_level_.empty()) return;

	size_t level = current_level_.size();
	odf_element_state state(child_elm, style_name, style_elm, level);
	
	elements_.push_back(state);
	current_level_.back()->add_child_element(child_elm);
}

void odf_layout_state::set_name(std::wstring & name)
{
	style_page_layout* style_ = dynamic_cast<style_page_layout*>(elements_[0].elm.get());//0 - root

	if (!style_)return;
	
	style_->style_page_layout_attlist_.style_name_ = name;
}
std::wstring odf_layout_state::get_name()
{
	style_page_layout *style_ = dynamic_cast<style_page_layout*>(elements_[0].elm.get());
	if (!style_)return L"";

	return style_->style_page_layout_attlist_.style_name_.get_value_or(L"");
}

void odf_layout_state::set_pages_mirrored(bool val)
{
	style_page_layout* style_ = dynamic_cast<style_page_layout*>(elements_[0].elm.get());//0 - root

	if (!style_)return;

	style_->style_page_layout_attlist_.style_page_usage_ = page_usage(page_usage::Mirrored);
}

style_page_layout_properties *odf_layout_state::get_properties()
{
	style_page_layout_properties *result=NULL;

	for (size_t i= 1; i<elements_.size(); i++)//"0" - root
	{
		result = dynamic_cast<style_page_layout_properties *>(elements_[i].elm.get());
		if (result) return result;
	}
	return NULL;
}

style_header_footer_properties *odf_layout_state::get_header_properties()
{
	for (size_t i = 1; i < elements_.size(); i++)//"0" - root
	{
		style_header_style *style_ = dynamic_cast<style_header_style *>(elements_[i].elm.get());
		if (style_)
		{
			return dynamic_cast<style_header_footer_properties *>(style_->style_header_footer_properties_.get());
		}
	}
	return NULL;
}
style_header_footer_properties *odf_layout_state::get_footer_properties()
{
	for (size_t i = 1; i < elements_.size(); i++)//"0" - root
	{
		style_footer_style *style_ = dynamic_cast<style_footer_style *>(elements_[i].elm.get());
		if (style_)
		{
			return dynamic_cast<style_header_footer_properties *>(style_->style_header_footer_properties_.get());
		}
	}
	return NULL;
}
}
}

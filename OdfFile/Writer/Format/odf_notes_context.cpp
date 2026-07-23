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

#include <boost/lexical_cast.hpp>

#include <iostream>

#include "odf_conversion_context.h"
#include "odf_notes_context.h"

#include "styles.h"

#include "paragraph_elements.h"

namespace cpdoccore 
{

	using namespace odf_types;

namespace odf_writer
{
	struct odf_note_state
	{
		office_element_ptr element;

		int				oox_id;
		std::wstring	odf_name;
		int				type;

		bool is_started;
		int state;
	};

class odf_notes_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		//styles_context_ = odf_context_->styles_context();
	} 

	std::vector<odf_note_state> notes_;

	odf_conversion_context *odf_context_;

};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
odf_notes_context::odf_notes_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_notes_context::Impl(odf_context))
{
}

odf_notes_context::~odf_notes_context()
{
}

void odf_notes_context::start_note(office_element_ptr &elm, int oox_id, int type)
{
	odf_note_state state;

	impl_->notes_.push_back(state);
	impl_->notes_.back().element = elm;
	impl_->notes_.back().oox_id = oox_id;
	impl_->notes_.back().odf_name = L"note_" + std::to_wstring(impl_->notes_.size());

	impl_->notes_.back().is_started = false;
	impl_->notes_.back().state		= 1;
	impl_->notes_.back().type		= type;

	text_note* note = dynamic_cast<text_note*>(elm.get());
	if (!note)return;

	if (type == 1)	note->text_note_class_ = odf_types::noteclass::Footnote;
	if (type == 2)	note->text_note_class_ = odf_types::noteclass::Endnote;
	
	note->text_id_ = impl_->notes_.back().odf_name;

	note->create_child_element(L"text", L"note-citation");
	text_note_citation* note_citation = dynamic_cast<text_note_citation*>(note->text_note_citation_.get());
	
	if (note_citation)
		note_citation->add_text(std::to_wstring(impl_->notes_.size()));

}
void odf_notes_context::end_note()
{
	impl_->notes_.back().state = 2;//stoped
}
void odf_notes_context::start_note_content()
{
	impl_->notes_.back().is_started = true;
	
	impl_->notes_.back().element->create_child_element(L"text", L"note-body");
}

office_element_ptr odf_notes_context::get_note_content()
{
	text_note* note = dynamic_cast<text_note*>(impl_->notes_.back().element.get());
	if (!note) return office_element_ptr();
	
	return note->text_note_body_;
}

void odf_notes_context::end_note_content()
{
	impl_->notes_.back().is_started = false;
}

int odf_notes_context::find_by_id(int oox_id, int type)
{
	for (size_t i=0; i < impl_->notes_.size(); i++)
	{
		if (impl_->notes_[i].oox_id == oox_id	&&
			impl_->notes_[i].type	== type)
		{
			return impl_->notes_[i].state;
		}
	}

	return 0;
}
std::wstring odf_notes_context::find_name_by_id(int oox_id, int type)
{
	for (size_t i=0; i < impl_->notes_.size(); i++)
	{
		if (impl_->notes_[i].oox_id == oox_id	&&
			impl_->notes_[i].type	== type)
		{
			return impl_->notes_[i].odf_name;
		}
	}

	return L"";
}
bool odf_notes_context::is_started()
{
	if (impl_->notes_.size() > 0)
		return 	impl_->notes_.back().is_started;
	else return false;

}
}
}
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
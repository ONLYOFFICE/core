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
#include "odf_comment_context.h"

#include "styles.h"

#include "office_annotation.h"

namespace cpdoccore 
{

	using namespace odf_types;

namespace odf_writer
{
	struct odf_comment_state
	{
		std::vector<office_element_ptr> elements_;

		int oox_id = -1;
		std::wstring odf_name;

		bool is_started = false;
		bool bContent = false;

		int state;

	};

class odf_comment_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		//styles_context_ = odf_context_->styles_context();

	} 

	std::vector<odf_comment_state> comments_;

	odf_conversion_context *odf_context_;

};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
odf_comment_context::odf_comment_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_comment_context::Impl(odf_context))
{
}

odf_comment_context::~odf_comment_context()
{
}

void odf_comment_context::start_comment(office_element_ptr &elm, int oox_id)
{
	odf_comment_state state;

	impl_->comments_.push_back(state);
	impl_->comments_.back().elements_.push_back(elm);//"0" - root comment eleemnt
	impl_->comments_.back().oox_id = oox_id < 0 ? impl_->comments_.back().elements_.size() : oox_id;
	impl_->comments_.back().odf_name = L"comment_" + boost::lexical_cast<std::wstring>(oox_id);

	impl_->comments_.back().is_started = false;
	impl_->comments_.back().state = 1;
	impl_->comments_.back().bContent = false;

	office_annotation* comm = dynamic_cast<office_annotation*>(elm.get());
	if (!comm)return;

	comm->attr_.name_ = impl_->comments_.back().odf_name;


}
void odf_comment_context::end_comment(office_element_ptr &elm, int oox_id)
{
	if (impl_->comments_.back().bContent == false)
	{
		start_comment_content();
		end_comment_content();
	}
	for (size_t i = 0; i < impl_->comments_.size(); i++)
	{
		if (impl_->comments_[i].oox_id == oox_id)
		{
			office_annotation_end* comm = dynamic_cast<office_annotation_end*>(elm.get());
			if (!comm)return;

			comm->attr_.name_ = impl_->comments_[i].odf_name;

			impl_->comments_[i].state = 2;//stoped

			impl_->comments_[i].elements_.push_back(elm);
			return;
		}
	}
}
void odf_comment_context::start_comment_content()
{
	impl_->comments_.back().is_started = true;
}
void odf_comment_context::end_comment_content()
{
	impl_->comments_.back().is_started = false;
	impl_->comments_.back().bContent = true;
}
int odf_comment_context::find_by_id(int oox_id)
{
	for (size_t i=0; i < impl_->comments_.size(); i++)
	{
		if (impl_->comments_[i].oox_id == oox_id)
		{
			return impl_->comments_[i].state;
		}
	}

	return 0;
}
std::wstring odf_comment_context::find_name_by_id(int oox_id)
{
	for (size_t i = 0; i < impl_->comments_.size(); i++)
	{
		if (impl_->comments_[i].oox_id == oox_id) return impl_->comments_[i].odf_name;
	}

	return L"";
}
bool odf_comment_context::is_started()
{
	if (!impl_->comments_.empty())
		return 	impl_->comments_.back().is_started;
	else return false;

}
void odf_comment_context::set_author(std::wstring author)
{
	if ((impl_->comments_.empty()) || !is_started()) return;

	office_element_ptr elm;
	create_element(L"dc", L"creator", elm, impl_->odf_context_);
	
	dc_creator* creator = dynamic_cast<dc_creator*>(elm.get());
	if (!creator)return;

	creator->content_ = author;

	impl_->comments_.back().elements_[0]->add_child_element(elm);
	impl_->comments_.back().elements_.push_back(elm);
}
void odf_comment_context::set_initials(std::wstring initials)
{
	if ((impl_->comments_.empty()) || !is_started()) return;

}
void odf_comment_context::set_date(std::wstring _date)
{
	if ((impl_->comments_.empty()) || !is_started()) return;
	
	office_element_ptr elm;
	create_element(L"dc", L"date", elm, impl_->odf_context_);
	
	dc_date* date = dynamic_cast<dc_date*>(elm.get());
	if (!date)return;

	date->content_ = _date;

	impl_->comments_.back().elements_[0]->add_child_element(elm);
	impl_->comments_.back().elements_.push_back(elm);
}
void odf_comment_context::set_position (double x, double y)
{
	if ((impl_->comments_.empty()) || !is_started()) return;

	office_annotation* comm = dynamic_cast<office_annotation*>(impl_->comments_.back().elements_.back().get());

	if (comm)
	{
		comm->attr_.svg_x_ = odf_types::length(x, odf_types::length::pt);
		comm->attr_.svg_y_ = odf_types::length(y, odf_types::length::pt);
	}
}

}
}

#include "logging.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

#include "odf_conversion_context.h"
#include "odf_comment_context.h"

#include "styles.h"

#include "office_annotation.h"

namespace cpdoccore 
{
namespace odf
{
	struct odf_comment_state
	{
		std::vector<office_element_ptr> elements_;

		int oox_id;
		std::wstring odf_name;

		bool is_started;

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
	impl_->comments_.back().oox_id = oox_id;
	impl_->comments_.back().odf_name = L"comment_" + boost::lexical_cast<std::wstring>(oox_id);

	impl_->comments_.back().is_started = false;
	impl_->comments_.back().state = 1;

	office_annotation* comm = dynamic_cast<office_annotation*>(elm.get());
	if (!comm)return;

	comm->office_annotation_attr_.name_ = impl_->comments_.back().odf_name;


}
void odf_comment_context::end_comment(office_element_ptr &elm, int oox_id)
{
	for(long i=0; i < impl_->comments_.size(); i++)
	{
		if (impl_->comments_[i].oox_id == oox_id)
		{
			office_annotation_end* comm = dynamic_cast<office_annotation_end*>(elm.get());
			if (!comm)return;

			comm->office_annotation_attr_.name_ = impl_->comments_[i].odf_name;

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
}
int odf_comment_context::find_by_id(int oox_id)
{
	for(long i=0; i < impl_->comments_.size(); i++)
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
	for(long i=0; i < impl_->comments_.size(); i++)
	{
		if (impl_->comments_[i].oox_id == oox_id) return impl_->comments_[i].odf_name;
	}

	return L"";
}
bool odf_comment_context::is_started()
{
	if (impl_->comments_.size()>0)
		return 	impl_->comments_.back().is_started;
	else return false;

}
void odf_comment_context::set_author(std::wstring author)
{
	if ((impl_->comments_.size()<1) || !is_started()) return;

	office_element_ptr elm;
	create_element(L"dc", L"creator",elm,impl_->odf_context_);
	
	dc_creator* creator = dynamic_cast<dc_creator*>(elm.get());
	if (!creator)return;

	creator->content_ = author;

	impl_->comments_.back().elements_[0]->add_child_element(elm);
	impl_->comments_.back().elements_.push_back(elm);

}
void odf_comment_context::set_date(std::wstring _date)
{
	if ((impl_->comments_.size()<1) || !is_started()) return;
	office_element_ptr elm;
	create_element(L"dc", L"date",elm,impl_->odf_context_);
	
	dc_date* date = dynamic_cast<dc_date*>(elm.get());
	if (!date)return;

	date->content_ = _date;

	impl_->comments_.back().elements_[0]->add_child_element(elm);
	impl_->comments_.back().elements_.push_back(elm);
}
}
}
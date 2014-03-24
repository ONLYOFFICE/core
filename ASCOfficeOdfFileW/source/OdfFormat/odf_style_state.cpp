#include "precompiled_cpodf.h"
#include "logging.h"

#include "odf_style_state.h"
#include "odf_conversion_context.h"

#include "styles.h"

namespace cpdoccore {
namespace odf {

odf_style_state::odf_style_state(office_element_ptr & elm, const style_family family )
{        
	odf_style_ = elm;
	
	automatic_= false;
	default_ = false;

	num_fmt_id_ =0;
	style_family_ = family;

	style* style_ = dynamic_cast<style*>(elm.get());
	if (style_)
	{
		style_->style_family_ = style_family_;
	}
	default_style* default_style_ = dynamic_cast<default_style*>(elm.get());

	if (default_style_)
	{
		default_style_->style_family_ = style_family_;
	}
}

office_element_ptr & odf_style_state::get_office_element()
{
	return odf_style_;
}

void odf_style_state::add_child(office_element_ptr & child)
{
	odf_style_->add_child_element(child);
}

void odf_style_state::set_name(std::wstring name)
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return;
	
	style_->style_name_ = name;
}
std::wstring & odf_style_state::get_name()
{
	style* style_ = dynamic_cast<style*>(odf_style_.get());
	if (!style_)return std::wstring();

	return style_->style_name_;
}
void odf_style_state::set_automatic(bool val)
{
	automatic_ = val;
}
void odf_style_state::set_root(bool val)//метка того что в файле styles, а не content
{
	root_ = val;
}
void odf_style_state::set_default(bool val)
{
	default_ = val;
}
void odf_style_state::convert()
{
	if (odf_style_== NULL)return;


}

}
}

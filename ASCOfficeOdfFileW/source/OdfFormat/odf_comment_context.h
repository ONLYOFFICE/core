#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore {
namespace odf
{

class odf_conversion_context;
class odf_style_context;

class odf_comment_context
{
public:
	odf_comment_context(odf_conversion_context *odf_context);
    ~odf_comment_context();

	void set_styles_context(odf_style_context*  styles_context);

	int find_by_id(int oox_id);
	
	bool is_started();// for content

	void start_comment(office_element_ptr &elm, int oox_id);
	void end_comment(office_element_ptr & elm, int oox_id);

	void start_comment_content();
	void end_comment_content();

	void set_author(std::wstring author);
	void set_date(std::wstring author);

private:
	std::wstring find_name_by_id(int oox_id);

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}
#pragma once

#include <string>
#include <vector>
//
//#include "ods_row_spanned.h"
//#include "ods_merge_cells.h"
//#include "ods_table_metrics.h"
//#include "ods_drawing_context.h"
//#include "ods_comments_context.h"
//#include "ods_hyperlinks.h"

#include "office_elements.h"
#include "office_elements_create.h"

#include "stylefamily.h"

namespace cpdoccore {
namespace odf {

class odf_conversion_context;
class odf_style_context;

//class style;

class odf_style_state
{
public:
	odf_style_state(/*odf_conversion_context & Context, */office_element_ptr & elm, const style_family style_family );

	void set_name(std::wstring);

	void set_automatic(bool val);
	void set_root(bool val);

	std::wstring & get_name();

	void convert();

	office_element_ptr & get_office_element();

	void add_child(office_element_ptr & child);

	void	set_number_format(int id)	{num_fmt_id_ = id;}
	int		get_number_format()			{return num_fmt_id_;}
	
private:
	std::wstring	style_oox_name_;

//инфа дл€ описани€ €чеек
	int				style_oox_id_;
	int				num_fmt_id_; //default =0 (general)
///////	
	bool automatic_;
	bool root_;

	style_family style_family_;
	
	office_element_ptr	odf_style_;

	//odf_conversion_context & context_;   
	friend class odf_style_context;
};


}
}


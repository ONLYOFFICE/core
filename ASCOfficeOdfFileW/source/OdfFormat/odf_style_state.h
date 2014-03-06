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
	odf_style_state(odf_conversion_context & Context, office_element_ptr & elm, const style_family style_family );

	void set_name(std::wstring);
	void set_automatic(bool val);

	std::wstring & get_name();

	void convert();

	office_element_ptr & get_office_element();

	void add_child(office_element_ptr & child);

	void set_oox_id(int id){style_oox_id_ = id;}
	
private:
	int style_oox_id_;
	
	bool automatic_;
	bool master_;
	
	office_element_ptr	odf_style_;//тут может быть всякий элемент который используется для описания стиля
	//важен порядок записи стилей !!! - сначала всякие там описания градиентов, шрифтов ... далее стили которые используются в  стилях, которые описаны ниже 
    //перед записью отсортировать???

	odf_conversion_context & context_;   
	friend class odf_style_context;
};


}
}


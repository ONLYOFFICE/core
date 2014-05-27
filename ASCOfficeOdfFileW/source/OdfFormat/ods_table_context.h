#pragma once

#include <string>
#include "ods_table_state.h"


namespace cpdoccore {


namespace odf {

class ods_conversion_context;
class ods_text_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

struct table_defined_expressions_state
{
	office_element_ptr				root;
	std::vector<office_element_ptr> defined;
};

/// \class ods_table_context
class ods_table_context
{
public:
    ods_table_context(ods_conversion_context & Context/*, ods_text_context & textCotnext*/);

public:
    void start_table(office_element_ptr & elm,std::wstring & name);
    void end_table();

    unsigned int columns_count();

    ods_table_state & state();
	
	void start_defined_expressions(office_element_ptr & root_elm);
	void add_defined_range(std::wstring & name,std::wstring & cell_range, int sheet_id);
 	void add_defined_expression(std::wstring & name,std::wstring & value, int sheet_id);
private:

    ods_conversion_context & context_;

	std::list<ods_table_state>		table_state_list_;
	table_defined_expressions_state	table_defined_expressions_;


};


}
}

#pragma once

#include "object_package.h"

namespace cpdoccore { 
namespace odf {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_style_manager;

class odf_conversion_context : boost::noncopyable
{
public:
    odf_conversion_context(package::odf_document * outputDocument);
    virtual ~odf_conversion_context();

	virtual void start_document() = 0 ;
	void end_document();

	office_element_ptr & getCurrentElement();

	std::vector<office_element_ptr>  content_;

    void add_rel(relationship const & r);

	package::odf_document * output_document_;
  

private:
	//odf_style_manager odf_styles_;
	rels	rels_;

	//office_spreadsheet* current_spreadsheet_;
};

}
}
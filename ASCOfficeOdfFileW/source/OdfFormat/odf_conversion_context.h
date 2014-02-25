#pragma once

#include "object_package.h"

namespace cpdoccore { 
namespace odf {

//class office_element;
//class relationship;
//class rels;

class odf_style_manager;

class odf_conversion_context : boost::noncopyable
{
public:
    odf_conversion_context(package::odf_document * outputDocument);
    virtual ~odf_conversion_context();

	void start_document();
	void end_document();


    void add_rel(relationship const & r);

	package::odf_document * output_document_;
  

private:
	//odf_style_manager odf_styles_;
	rels	rels_;
};

}
}
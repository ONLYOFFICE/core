#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

#include "../odf/chart_objects.h"

namespace cpdoccore {
namespace odf_reader 
{
	//class style_table_cell_properties_attlist;
}
}
                    
namespace cpdoccore {
namespace oox {

class oox_layout //расположение и формат(бордюрчики к примеру) любой хрени в документе
{
public:
	oox_layout(){}
	~oox_layout(){}

    //size_t size() const;
    //size_t borderId(const odf_reader::style_table_cell_properties_attlist * cellProp);
    //size_t borderId(const odf_reader::style_table_cell_properties_attlist * cellProp, bool & is_default);

    void oox_serialize(std::wostream & _Wostream);/* const;*/
    

};

}
}

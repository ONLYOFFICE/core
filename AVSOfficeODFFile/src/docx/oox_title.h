#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

#include "oox_layout.h"

            
namespace cpdoccore {
namespace oox {

class oox_title: boost::noncopyable
{
public:
	oox_title(){}
	~oox_title(){}
 
    void oox_serialize(std::wostream & _Wostream);
    void xlsx_serialize(std::wostream & _Wostream);
    void docx_serialize(std::wostream & _Wostream);

	void set_content(odf::chart::title &  t)
	{
		content_ = t;
	}
private:
	cpdoccore::oox::oox_layout layout_;
	odf::chart::title content_;

//extLst (Chart Extensibility) §21.2.2.64
//layout (Layout) §21.2.2.88
//overlay (Overlay) §21.2.2.132
//spPr (Shape Properties) §21.2.2.197
//tx (Chart Text) §21.2.2.214
//txPr (Text Properties)
};

}
}

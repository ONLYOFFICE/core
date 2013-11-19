#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

#include "oox_layout.h"
            
namespace cpdoccore {
namespace oox {

class oox_chart_legend: boost::noncopyable
{
public:
	oox_chart_legend(){}
	~oox_chart_legend(){}
 
    void oox_serialize(std::wostream & _Wostream);

	odf::chart::simple content_;  
private:
	void oox_serialize_default_text(std::wostream & _Wostream);
	
	cpdoccore::oox::oox_layout layout_;	//layout (Layout) §21.2.2.88

	//extLst (Chart Extensibility) §21.2.2.64
	//legendEntry (Legend Entry) §21.2.2.94
	//legendPos (Legend Position) §21.2.2.95
	//overlay (Overlay) §21.2.2.132
	//spPr (Shape Properties) §21.2.2.197
	//txPr (Text Properties) §21.2.2.216
};

}
}

#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

#include "..\odf\chart_objects.h"
#include "oox_drawing_fills.h"
            
namespace cpdoccore {
namespace oox {

class oox_chart_shape: boost::noncopyable
{
public:
	oox_chart_shape(){}
	~oox_chart_shape(){}
 
    void oox_serialize(std::wostream & _Wostream);
			
	std::vector<odf::_property> content_;

	_oox_fill fill;

    
private:

	//grpFill (Group Fill) §20.1.8.35
	//pattFill (Pattern Fill) §20.1.8.47	
	//noFill (No Fill) §20.1.8.44
	//blipFill (Picture Fill) §20.1.8.14
	//solidFill (Solid Fill) §20.1.8.54
	//gradFill (Gradient Fill) §20.1.8.33
	//ln (Outline) §20.1.2.2.24



    //friend void xlsx_serialize(std::wostream & _Wostream, const oox_chart_shape & legend_);


	//custGeom (Custom Geometry) §20.1.9.8
	//effectDag (Effect Container) §20.1.8.25
	//effectLst (Effect Container) §20.1.8.26
	//extLst (Extension List) §20.1.2.2.15

	//prstGeom (Preset geometry) §20.1.9.18
	//scene3d (3D Scene Properties) §20.1.4.1.26
	//sp3d (Apply 3D shape properties) §20.1.5.12
	//xfrm (2D Transform for Individual Objects) §20.1.7.6
};

}
}

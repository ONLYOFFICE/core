#pragma once

#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf { 

class smil_transition_type
{
public:
    enum type
    {
        barWipe,
		boxWipe,
		fourBoxWipe,
		barnDoorWipe,
		diagonalWipe,
		bowTieWipe,
		miscDiagonalWipe,
		veeWipe,
		barnVeeWipe,
		zigZagWipe,
		barnZigZagWipe,
		irisWipe,
		triangleWipe,
		arrowHeadWipe,
		pentagonWipe,
		hexagonWipe,
		ellipseWipe,
		eyeWipe,
		roundRectWipe,
		starWipe,
		miscShapeWipe,
		clockWipe,
		pinWheelWipe,
		singleSweepWipe,
		fanWipe,
		doubleFanWipe,
		doubleSweepWipe,
		saloonDoorWipe,
		windshieldWipe,
		snakeWipe,
		spiralWipe,
		parallelSnakesWipe,
		boxSnakesWipe,
		waterfallWipe,
		pushWipe,
		slideWipe,
		fade,
		checkerBoardWipe,
		blindsWipe,
		dissolve,
		randomBarWipe
    };

    smil_transition_type() {}

    smil_transition_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static smil_transition_type parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const smil_transition_type & _Val);


}
	APPLY_PARSE_XML_ATTRIBUTES(odf::smil_transition_type);
}

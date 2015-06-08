
#include "smil_transitiontype.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

smil_transition_type smil_transition_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

		 if(tmp == L"barwipe")			return smil_transition_type( barWipe );
	else if(tmp == L"boxwipe")			return smil_transition_type( boxWipe );
	else if(tmp == L"fourboxwipe")		return smil_transition_type( fourBoxWipe );
	else if(tmp == L"barndoorwipe")		return smil_transition_type( barnDoorWipe );
	else if(tmp == L"diagonalwipe")		return smil_transition_type( diagonalWipe );
	else if(tmp == L"bowtiewipe")		return smil_transition_type( bowTieWipe );
	else if(tmp == L"miscdiagonalwipe")	return smil_transition_type( miscDiagonalWipe );
	else if(tmp == L"veewipe")			return smil_transition_type( veeWipe );
	else if(tmp == L"barnveewipe")		return smil_transition_type( barnVeeWipe );
	else if(tmp == L"zigzagwipe")		return smil_transition_type( zigZagWipe );
	else if(tmp == L"barnzigzagwipe")	return smil_transition_type( barnZigZagWipe );
	else if(tmp == L"iriswipe")			return smil_transition_type( irisWipe);
	else if(tmp == L"trianglewipe")		return smil_transition_type( triangleWipe);
	else if(tmp == L"arrowheadwipe")	return smil_transition_type( arrowHeadWipe );
	else if(tmp == L"pentagonwipe")		return smil_transition_type( pentagonWipe );
	else if(tmp == L"hexagonwipe")		return smil_transition_type( hexagonWipe );
	else if(tmp == L"ellipsewipe")		return smil_transition_type( ellipseWipe );
	else if(tmp == L"eyewipe")			return smil_transition_type( eyeWipe );
	else if(tmp == L"roundrectwipe")	return smil_transition_type( roundRectWipe );
	else if(tmp == L"starwipe")			return smil_transition_type( starWipe );
	else if(tmp == L"miscshapewipe")	return smil_transition_type( miscShapeWipe );
	else if(tmp == L"clockwipe")		return smil_transition_type( clockWipe );
	else if(tmp == L"pinwheelwipe")		return smil_transition_type( pinWheelWipe );
	else if(tmp == L"singlesweepwipe")	return smil_transition_type( singleSweepWipe);
	else if(tmp == L"fanwipe")			return smil_transition_type( fanWipe );
	else if(tmp == L"doublefanwipe")	return smil_transition_type( doubleFanWipe );
	else if(tmp == L"doublesweepwipe")	return smil_transition_type( doubleSweepWipe );
	else if(tmp == L"saloondoorwipe")	return smil_transition_type( saloonDoorWipe );
	else if(tmp == L"windshieldwipe")	return smil_transition_type( windshieldWipe );
	else if(tmp == L"snakewipe")		return smil_transition_type( snakeWipe );
	else if(tmp == L"spiralwipe")		return smil_transition_type( spiralWipe );
	else if(tmp == L"parallelsnakeswipe")return smil_transition_type( parallelSnakesWipe );
	else if(tmp == L"boxsnakeswipe")	return smil_transition_type( boxSnakesWipe );
	else if(tmp == L"waterfallwipe")	return smil_transition_type( waterfallWipe );
	else if(tmp == L"pushwipe")			return smil_transition_type( pushWipe );
	else if(tmp == L"slidewipe")		return smil_transition_type( slideWipe );
	else if(tmp == L"fade")				return smil_transition_type( fade );
	else if(tmp == L"checkerboardwipe")	return smil_transition_type( checkerBoardWipe);
	else if(tmp == L"blindswipe")		return smil_transition_type( blindsWipe);
	else if(tmp == L"dissolve")			return smil_transition_type( dissolve);
	else if(tmp == L"randombarwipe")	return smil_transition_type( randomBarWipe);
	else
    {
        return smil_transition_type( barWipe );
    }
}

} }

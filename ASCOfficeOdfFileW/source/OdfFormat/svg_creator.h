#pragma once   

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.h>

#include <cpdoccore/CPOptional.h>

namespace cpdoccore 
{ 

namespace svg_path
{
	struct _point
	{
		_point(double _x,double _y){x=_x; y=_y;}
		
		_CP_OPT(double) x;
		_CP_OPT(double) y;
		
	};
	struct _polyline
	{
		std::wstring command;
		std::vector<_point> points; //будем бить строку пути по количеству точек в буковках

	};
	//m - 1 point
	//c - 3 point
	//s - 2 point
	//l - 1 point
	//z - finish
	//h - 0.5 point :)
	//v - 0.5 point
	//q - 2 point
	//t - 1 point
	//a - [[[[

	bool createSvgD(std::vector<_polyline> & Polyline, const std::wstring &  rSvgDStatement, bool bWrongPositionAfterZ);
	bool createPolygon(std::vector<_polyline> & Polyline, const std::wstring &  rPolygonStatement, bool bWrongPositionAfterZ);

}
}


#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class DonutType: public ShapeType
	{
	public: 
		DonutType() : ShapeType(msosptDonut)
		{
			ShapeConcentricFill =	false;
			Joins				=	round;
			Path				=	_T("m,10800qy10800,,21600,10800,10800,21600,,10800xm@0,10800qy10800@2@1,10800,10800@0@0,10800xe");

			Formulas.push_back(_T("val #0" ) );
			Formulas.push_back(_T("sum width 0 #0" ) );
			Formulas.push_back(_T("sum height 0 #0" ) );
			Formulas.push_back(_T("prod @0 2929 10000" ) );
			Formulas.push_back(_T("sum width 0 @3" ) );
			Formulas.push_back(_T("sum height 0 @3" ) );
			
			AdjustmentValues	=	_T("5400");
			ConnectorLocations	=	_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163");
			TextBoxRectangle	=	_T("3163,3163,18437,18437");

			Handle HandleOne;
			HandleOne.position	=	_T("#0,center");
			HandleOne.xrange	=	_T("0,10800");
			Handles.push_back(HandleOne);
		}
	};
}
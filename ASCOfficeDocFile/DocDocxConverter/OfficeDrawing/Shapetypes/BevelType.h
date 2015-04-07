#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class BevelType : public ShapeType
	{
	public:
		BevelType () : ShapeType(msosptBevel)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0e");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum width 0 #0"));
			Formulas.push_back(_T("sum height 0 #0"));
			Formulas.push_back(_T("prod width 1 2"));
			Formulas.push_back(_T("prod height 1 2"));
			Formulas.push_back(_T("prod #0 1 2"));
			Formulas.push_back(_T("prod #0 3 2"));
			Formulas.push_back(_T("sum @1 @5 0"));
			Formulas.push_back(_T("sum @2 @5 0"));

			AdjustmentValues	=	_T("2700");
			ConnectorLocations	=	_T("0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0");
			TextBoxRectangle	=	_T("@0,@0,@1,@2");

			Handle one;
			one.position		=	_T("#0,topLeft");
			one.xrange			=	_T("0,10800"); 
			Handles.push_back (one);
       
			Limo				=	_T("10800,10800");
		}
	};
}
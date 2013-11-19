#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class MoonType : public ShapeType
	{
	public:
		MoonType () : ShapeType(msosptMoon)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m21600,qx,10800,21600,21600wa@0@10@6@11,21600,21600,21600,xe");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 21600 0 #0"));
			Formulas.push_back(_T("prod #0 #0 @1"));
			Formulas.push_back(_T("prod 21600 21600 @1"));
			Formulas.push_back(_T("prod @3 2 1"));
			Formulas.push_back(_T("sum @4 0 @2"));
			Formulas.push_back(_T("sum @5 0 #0"));
			Formulas.push_back(_T("prod @5 1 2"));
			Formulas.push_back(_T("sum @7 0 #0"));
			Formulas.push_back(_T("prod @8 1 2"));
			Formulas.push_back(_T("sum 10800 0 @9"));
			Formulas.push_back(_T("sum @9 10800 0"));
			Formulas.push_back(_T("prod #0 9598 32768"));
			Formulas.push_back(_T("sum 21600 0 @12"));
			Formulas.push_back(_T("ellipse @13 21600 10800"));
			Formulas.push_back(_T("sum 10800 0 @14"));
			Formulas.push_back(_T("sum @14 10800 0"));

			AdjustmentValues	=	_T("10800");
			ConnectorLocations	=	_T("21600,0;0,10800;21600,21600;@0,10800");
			ConnectorAngles		=	_T("270,180,90,0");
			TextBoxRectangle	=	_T("@12,@15,@0,@16");

			Handle one;
			one.position		=	_T("#0,center");
			one.xrange			=	_T("0,18900"); 
			Handles.push_back (one);
		}
	};
}
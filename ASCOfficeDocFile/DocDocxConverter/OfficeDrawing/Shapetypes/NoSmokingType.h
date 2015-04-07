#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class NoSmokingType : public ShapeType
	{
	public:
		NoSmokingType () : ShapeType(msosptNoSmoking)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m,10800qy10800,,21600,10800,10800,21600,,10800xar@0@0@16@16@12@14@15@13xar@0@0@16@16@13@15@14@12xe");

			Formulas.push_back(_T( "val #0" ));
			Formulas.push_back(_T( "prod @0 2 1" ));
			Formulas.push_back(_T( "sum 21600 0 @1" ));
			Formulas.push_back(_T( "prod @2 @2 1" ));
			Formulas.push_back(_T( "prod @0 @0 1" ));
			Formulas.push_back(_T( "sum @3 0 @4" ));
			Formulas.push_back(_T( "prod @5 1 8" ));
			Formulas.push_back(_T( "sqrt @6" ));
			Formulas.push_back(_T( "prod @4 1 8" ));
			Formulas.push_back(_T( "sqrt @8" ));
			Formulas.push_back(_T( "sum @7 @9 0" ));
			Formulas.push_back(_T( "sum @7 0 @9" ));
			Formulas.push_back(_T( "sum @10 10800 0" ));
			Formulas.push_back(_T( "sum 10800 0 @10" ));
			Formulas.push_back(_T( "sum @11 10800 0" ));
			Formulas.push_back(_T( "sum 10800 0 @11" ));
			Formulas.push_back(_T( "sum 21600 0 @0" ));

			AdjustmentValues	=	_T( "2700" );
			ConnectorLocations	=	_T( "10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163" );
			TextBoxRectangle	=	_T( "3163,3163,18437,18437" );

			Handle one;
			one.position		=	_T("#0,center");
			one.xrange			=	_T("0,7200"); 
			Handles.push_back (one);
		}
	};
}
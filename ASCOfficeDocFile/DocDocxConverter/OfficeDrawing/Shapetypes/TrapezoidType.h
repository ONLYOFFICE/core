#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class TrapezoidType : public ShapeType
	{
	public: 
		TrapezoidType () : ShapeType(msosptTrapezoid)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

			Path				= _T("m,l@0,21600@1,21600,21600,xe");

			Formulas.push_back( _T( "val #0" ) );
			Formulas.push_back( _T( "sum width 0 #0" ) );
			Formulas.push_back( _T( "prod #0 1 2" ) );
			Formulas.push_back( _T( "sum width 0 @2" ) );
			Formulas.push_back( _T( "mid #0 width" ) );
			Formulas.push_back( _T( "mid @1 0" ) );
			Formulas.push_back( _T( "prod height width #0" ) );
			Formulas.push_back( _T( "prod @6 1 2" ) );
			Formulas.push_back( _T( "sum height 0 @7" ) );
			Formulas.push_back( _T( "prod width 1 2" ) );
			Formulas.push_back( _T( "sum #0 0 @9" ) );
			Formulas.push_back( _T( "if @10 @8 0" ) );
			Formulas.push_back( _T( "if @10 @7 height" ) );

			AdjustmentValues	=	_T("5400");
			ConnectorLocations	=	_T("@3,10800;10800,21600;@2,10800;10800,0");
			TextBoxRectangle	=	_T("1800,1800,19800,19800;4500,4500,17100,17100;7200,7200,14400,14400");

			Handle one;
			
			one.position		=	_T("#0,bottomRight");
			one.xrange			=	_T("0,10800");
			
			Handles.push_back (one);
		}

		virtual ~TrapezoidType()
		{
		}
	};
}
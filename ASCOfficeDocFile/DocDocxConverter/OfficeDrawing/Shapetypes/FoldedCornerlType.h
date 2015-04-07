#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class FoldedCornerlType : public ShapeType
	{
	public:
		FoldedCornerlType () : ShapeType(msosptFoldedCorner)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T( "m,l,21600@0,21600,21600@0,21600,xem@0,21600nfl@3@5c@7@9@11@13,21600@0e" );

			Formulas.push_back(_T( "val #0" ));
			Formulas.push_back(_T( "sum 21600 0 @0" ));
			Formulas.push_back(_T( "prod @1 8481 32768" ));
			Formulas.push_back(_T( "sum @2 @0 0" ));
			Formulas.push_back(_T( "prod @1 1117 32768" ));
			Formulas.push_back(_T( "sum @4 @0 0" ));
			Formulas.push_back(_T( "prod @1 11764 32768" ));
			Formulas.push_back(_T( "sum @6 @0 0" ));
			Formulas.push_back(_T( "prod @1 6144 32768" ));
			Formulas.push_back(_T( "sum @8 @0 0" ));
			Formulas.push_back(_T( "prod @1 20480 32768" ));
			Formulas.push_back(_T( "sum @10 @0 0" ));
			Formulas.push_back(_T( "prod @1 6144 32768" ));
			Formulas.push_back(_T( "sum @12 @0 0" ));

			AdjustmentValues	=	_T( "18900" );
			TextBoxRectangle	=	_T( "0,0,21600,@13" );

			Handle one;
			one.position		=	_T("#0,bottomRight");
			one.xrange			=	_T("10800,21600"); 
			Handles.push_back (one);
		}
	};
}
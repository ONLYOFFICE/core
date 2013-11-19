#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class SmileyFaceType : public ShapeType
	{
	public:
		SmileyFaceType () : ShapeType(msosptSmileyFace)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m10800,qx,10800,10800,21600,21600,10800,10800,xem7340,6445qx6215,7570,7340,8695,8465,7570,7340,6445xnfem14260,6445qx13135,7570,14260,8695,15385,7570,14260,6445xnfem4960@0c8853@3,12747@3,16640@0nfe");

			Formulas.push_back(_T( "sum 33030 0 #0" ));
			Formulas.push_back(_T( "prod #0 4 3" ));
			Formulas.push_back(_T( "prod @0 1 3" ));
			Formulas.push_back(_T( "sum @1 0 @2" ));

			AdjustmentValues	=	_T( "17520" );
			ConnectorLocations	=	_T( "10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163" );
			TextBoxRectangle	=	_T( "3163,3163,18437,18437" );

			Handle one;
			one.position		=	_T("center,#0");
			one.xrange			=	_T("15510,17520"); 
			Handles.push_back (one);
		}
	};
}
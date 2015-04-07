#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
	class ArcType : public ShapeType
	{
	public:
		ArcType () : ShapeType(msosptArc)
		{
			ShapeConcentricFill		=	true;
			Joins					=	round;
			Path					=	_T( "wr-21600,,21600,43200,,,21600,21600nfewr-21600,,21600,43200,,,21600,21600l,21600nsxe" );

			Formulas.push_back( _T( "val #2" ) );
			Formulas.push_back( _T( "val #3" ) );
			Formulas.push_back( _T( "val #4" ) );

			AdjustmentValues		=	_T( "-5898240,,,21600,21600" );
			ConnectorLocations		=	_T( "0,0;21600,21600;0,21600" );

			Handle HandleOne;
			HandleOne.position		=	_T( "@2,#0" );
			HandleOne.polar			=	_T( "@0,@1" ); 
			Handles.push_back( HandleOne );

			Handle HandleTwo;
			HandleOne.position		=	_T( "@2,#1" );
			HandleOne.polar			=	_T( "@0,@1" ); 
			Handles.push_back( HandleTwo );
		}
	};
}
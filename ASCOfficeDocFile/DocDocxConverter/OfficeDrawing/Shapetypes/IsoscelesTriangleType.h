#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class IsoscelesTriangleType: public ShapeType
  {
    public: 
	  IsoscelesTriangleType():
      ShapeType(msosptIsocelesTriangle)
      {
	    this->ShapeConcentricFill = true;
        this->Joins = miter;

        this->Path = _T( "m@0,l,21600r21600,xe" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "prod #0 1 2" ) );
        this->Formulas.push_back( _T( "sum @1 10800 0") );

        this->AdjustmentValues = _T( "10800" );

        this->ConnectorLocations = _T( "@0,0;@1,10800;0,21600;10800,21600;21600,21600;@2,10800" );

        this->TextBoxRectangle = _T( "0,10800,10800,18000;5400,10800,16200,18000;10800,10800,21600,18000;0,7200,7200,21600;7200,7200,14400,21600;14400,7200,21600,21600" );

        Handle HandleOne;
        HandleOne.position = _T( "#0,topLeft" );
        HandleOne.xrange = _T( "0,21600" );
        this->Handles.push_back( HandleOne );
	  }

	  virtual ~IsoscelesTriangleType()
	  {
	  }
  };
}
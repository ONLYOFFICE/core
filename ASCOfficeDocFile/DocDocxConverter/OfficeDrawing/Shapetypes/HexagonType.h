#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class HexagonType: public ShapeType
  {
    public: 
	  HexagonType():
      ShapeType(msosptHexagon)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m@0,l,10800@0,21600@1,21600,21600,10800@1,xe" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "sum width 0 #0" ) );
        this->Formulas.push_back( _T( "sum height 0 #0" ) );
        this->Formulas.push_back( _T( "prod @0 2929 10000" ) );
        this->Formulas.push_back( _T( "sum width 0 @3" ) );
        this->Formulas.push_back( _T( "sum height 0 @3" ) );

        this->AdjustmentValues = _T( "5400" );
            
        this->ConnectorLocations = _T( "Rectangle" );

        this->TextBoxRectangle = _T( "1800,1800,19800,19800;3600,3600,18000,18000;6300,6300,15300,15300" );

        Handle HandleOne;
        HandleOne.position = _T( "#0,topLeft" );
        HandleOne.xrange = _T( "0,10800" );
        Handles.push_back( HandleOne );
	  }

	  virtual ~HexagonType()
	  {
	  }
  };
}
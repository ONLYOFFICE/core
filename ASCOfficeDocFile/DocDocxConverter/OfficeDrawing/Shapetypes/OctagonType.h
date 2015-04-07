#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class OctagonType: public ShapeType
  {
    public: 
	  OctagonType():
      ShapeType(msosptOctagon)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m@0,l0@0,0@2@0,21600@1,21600,21600@2,21600@0@1,xe" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "sum width 0 #0" ) );
        this->Formulas.push_back( _T( "sum height 0 #0" ) );
        this->Formulas.push_back( _T( "prod @0 2929 10000" ) );
        this->Formulas.push_back( _T( "sum width 0 @3" ) );
        this->Formulas.push_back( _T( "sum height 0 @3" ) );
        this->Formulas.push_back( _T( "val width" ) );
        this->Formulas.push_back( _T( "val height" ) );
        this->Formulas.push_back( _T( "prod width 1 2" ) );
        this->Formulas.push_back( _T( "prod height 1 2" ) );

        this->AdjustmentValues = _T( "6326" );

        this->ConnectorLocations = _T( "@8,0;0,@9;@8,@7;@6,@9" );

        this->TextBoxRectangle = _T( "0,0,21600,21600;2700,2700,18900,18900;5400,5400,16200,16200" );

        Handle HandleOne;
        HandleOne.position = _T( "#0,topLeft" );
        HandleOne.switchHandle = _T( "true" );
        HandleOne.xrange = _T( "0,10800" );
        this->Handles.push_back( HandleOne );

        this->Limo = _T( "10800,10800" );
	  }

	  virtual ~OctagonType()
	  {
	  }
  };
}
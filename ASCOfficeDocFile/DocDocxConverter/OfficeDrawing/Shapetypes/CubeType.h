#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
  class CubeType: public ShapeType
  {
    public:
	  CubeType():
      ShapeType(msosptCube)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m@0,l0@0,,21600@1,21600,21600@2,21600,xem0@0nfl@1@0,21600,em@1@0nfl@1,21600e" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "sum width 0 #0" ) );
        this->Formulas.push_back( _T( "sum height 0 #0" ) );
        this->Formulas.push_back( _T( "mid height #0" ) );
        this->Formulas.push_back( _T( "prod @1 1 2" ) );
        this->Formulas.push_back( _T( "prod @2 1 2" ) );
        this->Formulas.push_back( _T( "mid width #0" ) );

        this->AdjustmentValues = _T( "5400" );

        this->ConnectorLocations = _T( "@6,0;@4,@0;0,@3;@4,21600;@1,@3;21600,@5" );

        this->ConnectorAngles = _T( "270,270,180,90,0,0" );

        this->TextBoxRectangle = _T( "0,@0,@1,21600" );

        Handle HandleOne;
        HandleOne.position = _T( "topLeft,#0" );
        HandleOne.switchHandle = _T( "true" );
        HandleOne.yrange = _T( "0,21600" );
        this->Handles.push_back( HandleOne );

        this->Limo = _T( "10800,10800" );
      }

	  virtual ~CubeType()
	  {
	  }
  };
}
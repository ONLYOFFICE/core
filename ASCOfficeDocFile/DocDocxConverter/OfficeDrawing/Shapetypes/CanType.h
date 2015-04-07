#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class CanType: public ShapeType
  {
    public: 
	  CanType():
      ShapeType(msosptCan)
      {
        this->ShapeConcentricFill = true;

        this->Joins = round;

        this->Path = _T( "m10800,qx0@1l0@2qy10800,21600,21600@2l21600@1qy10800,xem0@1qy10800@0,21600@1nfe" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "prod #0 1 2" ) );
        this->Formulas.push_back( _T( "sum height 0 @1" ) );

        this->AdjustmentValues = _T( "5400" );

        this->ConnectorLocations = _T( "10800,@0;10800,0;0,10800;10800,21600;21600,10800" );

        this->ConnectorAngles = _T( "270,270,180,90,0" );

        this->TextBoxRectangle = _T( "0,@0,21600,@2" );

        Handle HandleOne;
        HandleOne.position = _T( "center,#0" );
        HandleOne.yrange = _T( "0,10800" );
        this->Handles.push_back( HandleOne );
	  }

	  virtual ~CanType()
	  {
	  }
  };
}
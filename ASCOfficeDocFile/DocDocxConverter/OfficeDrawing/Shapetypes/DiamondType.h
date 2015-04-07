#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class DiamondType: public ShapeType
  {
    public:
	  DiamondType():
      ShapeType(msosptDiamond)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m10800,l,10800,10800,21600,21600,10800xe" );

        this->ConnectorLocations = _T( "Rectangle" );

        this->TextBoxRectangle = _T( "5400,5400,16200,16200" );
      }

	  virtual ~DiamondType()
	  {
	  }
  };
}
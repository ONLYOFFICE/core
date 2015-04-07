#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class RightTriangleType: public ShapeType
  {
    public: 
	  RightTriangleType():
      ShapeType(msosptRightTriangle)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m,l,21600r21600,xe" );

        this->ConnectorLocations = _T( "0,0;0,10800;0,21600;10800,21600;21600,21600;10800,10800" );

        this->TextBoxRectangle = _T( "1800,12600,12600,19800" );
	  }

	  virtual ~RightTriangleType()
	  {
	  }
  };
}
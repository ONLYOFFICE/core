#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
  class RectangleType: public ShapeType
  {
    public:
	  RectangleType():
	  ShapeType(msosptRectangle)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m,l,21600r21600,l21600,xe" );
      }

	  virtual ~RectangleType()
	  {
	  }
  };
}

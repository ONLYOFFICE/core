#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
  class OvalType: public ShapeType
  {
    public:
	  OvalType():
      ShapeType(msosptEllipse)
      {
        this->ShapeConcentricFill = true;

        this->Joins = round;
      }

	  virtual ~OvalType()
	  {
	  }
  };
}
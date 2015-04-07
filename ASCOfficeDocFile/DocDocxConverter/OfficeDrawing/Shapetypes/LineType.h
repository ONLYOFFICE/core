#pragma once

#include "../ShapeType.h"

namespace DocFileFormat
{
  class LineType: public ShapeType
  {
    public: 
	  LineType():
      ShapeType(msosptLine)
      {
	  }

	  virtual ~LineType()
	  {
	  }
  };
}

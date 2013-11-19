#pragma once

#include "PropertyExceptions.h"

namespace DocFileFormat
{
  class SectionPropertyExceptions: public PropertyExceptions
  {
    public:  
	  /// Parses the bytes to retrieve a SectionPropertyExceptions
	  SectionPropertyExceptions( byte* bytes, int size ):
      PropertyExceptions( bytes, size )
      {
      }

	  virtual ~SectionPropertyExceptions()
	  {
	  }
  };
}
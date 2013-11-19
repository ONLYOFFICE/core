#pragma once

#include "PropertyExceptions.h"

namespace DocFileFormat
{
  class CharacterPropertyExceptions: public PropertyExceptions
  {
    public:
	  /// Creates a CHPX wich doesn't modify anything.
      /// The grpprl list is empty
      CharacterPropertyExceptions(): PropertyExceptions()
      {
      }

      /// Parses the bytes to retrieve a CHPX
	  CharacterPropertyExceptions( byte* bytes, int size ): PropertyExceptions( bytes, size )
      {
      }
  };
}
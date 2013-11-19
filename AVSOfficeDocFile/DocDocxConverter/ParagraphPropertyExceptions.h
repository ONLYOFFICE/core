#pragma once

#include "PropertyExceptions.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
  class ParagraphPropertyExceptions: public PropertyExceptions
  {
    public:
      /// Index to style descriptor of the style from which the 
      /// paragraph inherits its paragraph and character properties
      unsigned short istd;
      /// Creates a PAPX wich doesn't modify anything.
      /// The grpprl list is empty
    
	  ParagraphPropertyExceptions(): 
	  PropertyExceptions(), istd(0)
      {
      }

	  ParagraphPropertyExceptions( const list<SinglePropertyModifier>& grpprl ): 
	  PropertyExceptions( grpprl ), istd(0)
      {
      }

	  virtual ~ParagraphPropertyExceptions()
      {
      }

      /// Parses the bytes to retrieve a PAPX
	  ParagraphPropertyExceptions( byte* bytes, int size, IStream* dataStream );
  };
}

#pragma once

#include "VirtualStreamReader.h"

namespace DocFileFormat
{
  class ByteStructure
  {
    protected:
	  ByteStructure() {}

    public:
   	  virtual ~ByteStructure() {}
	  virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length ) = 0; // Virtual constructor
  };

  class EmptyStructure: public ByteStructure
  {
    public:
	  static const int STRUCTURE_SIZE = 0;
		
	  EmptyStructure() {}

	  virtual ~EmptyStructure() {}

	  virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
      {
        EmptyStructure *newObject = new EmptyStructure();

	    return static_cast<ByteStructure*>( newObject );
      }
  };
}

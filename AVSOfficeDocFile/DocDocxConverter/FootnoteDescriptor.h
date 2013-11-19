#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
  class FootnoteDescriptor: public ByteStructure
  {
    private:
	  short aFtnIdx;

    public:
	  static const int STRUCTURE_SIZE = 2;

	  FootnoteDescriptor():
      aFtnIdx(0)
	  {
	  }

	  virtual ~FootnoteDescriptor()
	  {
	  }
  
      virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
      {
        FootnoteDescriptor *newObject = new FootnoteDescriptor();

		newObject->aFtnIdx = reader->ReadInt16();

		return static_cast<ByteStructure*>( newObject );
      }
  };
}
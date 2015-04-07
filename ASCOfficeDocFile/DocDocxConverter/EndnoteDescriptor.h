#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
  class EndnoteDescriptor: public ByteStructure
  {
    private:
	  short aEndIdx;

    public:
      static const int STRUCTURE_SIZE = 2;

	  EndnoteDescriptor():
      aEndIdx(0)
	  {
	  }

	  virtual ~EndnoteDescriptor()
	  {
	  }
  
      virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
      {
        EndnoteDescriptor *newObject = new EndnoteDescriptor();

		newObject->aEndIdx = reader->ReadInt16();

		return static_cast<ByteStructure*>( newObject );
      }
  };
}
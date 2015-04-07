#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
  class SectionDescriptor: public ByteStructure
  {
    friend class WordDocument;

    private:
      static const int SED_LENGTH = 12;
      short fn;
      short fnMpr;
      int fcMpr;
      /// A signed integer that specifies the position in the WordDocument Stream where a Sepx structure is located.
      int fcSepx;

    public:
      static const int STRUCTURE_SIZE = 12;

      SectionDescriptor()
	  {
	  }

	  virtual ~SectionDescriptor()
	  {
	  }

      virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
	  {
        SectionDescriptor *newObject = new SectionDescriptor();

	    newObject->fn = reader->ReadInt16();
        newObject->fcSepx = reader->ReadInt32();
        newObject->fnMpr = reader->ReadInt16();
        newObject->fcMpr = reader->ReadInt32();

        return static_cast<ByteStructure*>( newObject );
      }
  };
}

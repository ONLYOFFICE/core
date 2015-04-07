#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
  class BookmarkFirst: public ByteStructure
  {
    private:
      /// An unsigned integer that specifies a zero-based index into the PlcfBkl or PlcfBkld 
      /// that is paired with the PlcfBkf  or PlcfBkfd containing this FBKF.
      /// The entry found at said index specifies the location of the end of the bookmark associated with this FBKF.
      /// Ibkl MUST be unique for all FBKFs inside a given PlcfBkf or PlcfBkfd.
      short ibkl;
      /// A BKC that specifies further information about the bookmark associated with this FBKF.
      short bkc;

    public:
      static const int STRUCTURE_SIZE = 4;

	  BookmarkFirst()
	  {
	  }

	  short GetIndex() const
	  {
	    return this->ibkl;
	  }

	  short GetInformation() const
	  {
	    return this->bkc;
	  }
	
	  virtual ~BookmarkFirst()
	  {
	  }

	  virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
      {
        BookmarkFirst *newObject = new BookmarkFirst();

	    newObject->ibkl = reader->ReadInt16();
        newObject->bkc = reader->ReadInt16();

	    return static_cast<ByteStructure*>( newObject );
      }
  };
}

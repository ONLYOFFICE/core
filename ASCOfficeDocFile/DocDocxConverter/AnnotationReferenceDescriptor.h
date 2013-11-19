#pragma once

#include "ByteStructure.h"

namespace DocFileFormat
{
  class AnnotationReferenceDescriptor: public ByteStructure
  {
    private:
      /// The initials of the user who left the annotation.
      wstring UserInitials;
      /// An index into the string table of comment author names.
      unsigned short AuthorIndex;
      /// Identifies a bookmark.
      int BookmarkId;

    public:
      static const int STRUCTURE_SIZE = 30;

	  wstring GetUserInitials() const
	  {
	    return UserInitials;
	  }

      unsigned short GetAuthorIndex() const
	  {
	    return AuthorIndex;
	  }

      int GetBookmarkId() const
	  {
	    return BookmarkId;
	  }
	  
	  AnnotationReferenceDescriptor():
	  UserInitials( _T( "" ) ), AuthorIndex(0), BookmarkId(0)
	  {
	  }
	  
	  virtual ~AnnotationReferenceDescriptor() 
	  {
	  }

	  virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length );
  };
}

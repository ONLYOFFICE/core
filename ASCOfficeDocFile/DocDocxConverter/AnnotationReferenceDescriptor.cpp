#include "stdafx.h"
#include "AnnotationReferenceDescriptor.h"

namespace DocFileFormat
{
  ByteStructure* AnnotationReferenceDescriptor::ConstructObject( VirtualStreamReader* reader, int length )
  {
    AnnotationReferenceDescriptor *newObject = new AnnotationReferenceDescriptor();

    //read the user initials (LPXCharBuffer9)
    short cch = reader->ReadInt16();
        
    byte *chars = reader->ReadBytes( 18, true );
       
    FormatUtils::GetSTLCollectionFromBytes<wstring>( &(newObject->UserInitials), chars, ( cch * 2 ), ENCODING_UNICODE );

    newObject->AuthorIndex = reader->ReadUInt16();

    //skip 4 bytes
    reader->ReadBytes( 4, false );

    newObject->BookmarkId = reader->ReadInt32();

	RELEASEARRAYOBJECTS( chars );

    return static_cast<ByteStructure*>( newObject );
  }
}
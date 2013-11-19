#pragma once

#include "..\VirtualStreamReader.h"
#include "..\IVisitable.h"
#include "..\MemoryStream.h"

namespace DocFileFormat
{
  class Record: public IVisitable
  {
    friend class RegularContainer;
	friend struct RecordFactory;
    
    protected: 
	  IBinaryReader* Reader;
	  static const unsigned int HEADER_SIZE_IN_BYTES = ( 16 + 16 + 32 ) / 8;
      unsigned int HeaderSize;
      /// Size == BodySize
	  byte* RawData;
      /// Index of sibling, 0 for first child in container, 1 for second child and so on...
      unsigned int SiblingIdx;

    public:
      unsigned int TypeCode;
      unsigned int Version;
      unsigned int Instance;

	  unsigned int BodySize;
      
	  Record* _ParentRecord;
	  
	  virtual ~Record();
	  Record();
      Record( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance );
	  unsigned int GetTotalSize() const;
      virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance ) = 0;
  };
}
#pragma once

#include "Record.h"

namespace DocFileFormat
{
  class UnknownRecord: public Record
  {
    public:
      unsigned char* _bytes;
	  unsigned int _size;

      UnknownRecord():
	  Record(), _bytes(NULL), _size(0)
	  {
	  }

	  UnknownRecord( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
      Record( _reader, size, typeCode, version, instance ), _bytes(NULL), _size(0)
      {
	    this->_size = (unsigned int)( this->Reader->GetSize() - this->Reader->GetPosition() );
          
		if ( this->_size >= size )
        {
	      this->_size = size;
		}
        
		this->_bytes = this->Reader->ReadBytes( this->_size, true );
      }

	  virtual ~UnknownRecord()
	  {
	    RELEASEARRAYOBJECTS( this->_bytes );
	  }

	  virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	  {
	    return new UnknownRecord( _reader, bodySize, typeCode, version, instance );
	  }
  };
}
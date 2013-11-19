#pragma once

#include "Record.h"

namespace DocFileFormat
{
  class DrawingRecord: public Record
  {
    public:
      static const unsigned short TYPE_CODE_0xF008 = 0xF008;

      /// The number of shapes in this drawing
      unsigned int csp;
      /// The last MSOSPID given to an SP in this DG
      int spidCur;

	  DrawingRecord():
	  Record(), csp(0), spidCur(0)
	  {
	  }

	  DrawingRecord( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
      Record( _reader, size, typeCode, version, instance )
      {
	    this->csp = this->Reader->ReadUInt32();
		this->spidCur = this->Reader->ReadInt32();
      }

	  virtual ~DrawingRecord()
	  {
	  }

	  virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	  {
	    return new DrawingRecord( _reader, bodySize, typeCode, version, instance );
	  }
  };
}
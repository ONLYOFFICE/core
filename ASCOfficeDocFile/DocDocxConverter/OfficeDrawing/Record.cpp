#include "stdafx.h"
#include "Record.h"

namespace DocFileFormat
{
  Record::~Record()
  {
    RELEASEARRAYOBJECTS( this->RawData );
	RELEASEOBJECT( this->Reader );
  }

  /*========================================================================================================*/

  Record::Record():
  HeaderSize(0), BodySize(0), RawData(NULL), SiblingIdx(0), TypeCode(0), Version(0), Instance(0), Reader(NULL),
  _ParentRecord(NULL)
  {
  }

  /*========================================================================================================*/

  Record::Record( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance ):
  HeaderSize(0), BodySize(0), RawData(NULL), SiblingIdx(0), TypeCode(0), Version(0), Instance(0), Reader(NULL),
  _ParentRecord(NULL)
  {
    this->BodySize = bodySize;
    this->TypeCode = typeCode;
    this->Version = version;
    this->Instance = instance;
	this->HeaderSize = Record::HEADER_SIZE_IN_BYTES;

	this->RawData = _reader->ReadBytes( (int)this->BodySize, true );
	this->Reader = new MemoryStream( this->RawData, this->BodySize, false );
  }

  /*========================================================================================================*/

  unsigned int Record::GetTotalSize() const
  {
    return ( this->HeaderSize + this->BodySize );
  }
}
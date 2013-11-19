#pragma once

#include "Record.h"

namespace DocFileFormat
{
  struct RecordFactory
  {
    static Record* ReadRecord( IBinaryReader* reader, unsigned int siblingIdx );
    static Record* NewRecord( unsigned short typeCode );
  };  
}
#pragma once

#include "RegularContainer.h"

namespace DocFileFormat
{
  class DrawingContainer: public RegularContainer
  {
    public: 
	  static const unsigned short TYPE_CODE_0xF002 = 0xF002;
		
	  DrawingContainer():
      RegularContainer()
	  {
	  }
		
	  DrawingContainer( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
      RegularContainer( _reader, size, typeCode, version, instance )
	  {
	  }

	  virtual ~DrawingContainer() 
	  {
	  }

	  virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	  {
	    return new DrawingContainer( _reader, bodySize, typeCode, version, instance );
	  }
  }; 
}
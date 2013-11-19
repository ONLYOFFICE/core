#pragma once

#include "RegularContainer.h"

namespace DocFileFormat
{
	class DrawingGroup: public RegularContainer
	{
	public: 
		static const unsigned short TYPE_CODE_0xF000 = 0xF000;

		DrawingGroup():	RegularContainer()
		{
		}

		DrawingGroup (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : RegularContainer( _reader, size, typeCode, version, instance )
		{
		}

		virtual ~DrawingGroup() 
		{
		}

		virtual Record* NewObject (IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
		{
			return new DrawingGroup (_reader, bodySize, typeCode, version, instance);
		}
	}; 
}
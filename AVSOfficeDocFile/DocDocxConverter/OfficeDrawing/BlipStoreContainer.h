#pragma once

#include "RegularContainer.h"

namespace DocFileFormat
{
	class BlipStoreContainer: public RegularContainer
	{
	public:
		static const unsigned short TYPE_CODE_0xF001 = 0xF001;

		BlipStoreContainer () : RegularContainer()
		{
		}

		BlipStoreContainer (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : RegularContainer (_reader, size, typeCode, version, instance) 
		{ 
		}

		virtual ~BlipStoreContainer()
		{
		}

		virtual Record* NewObject (IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
		{
			return new BlipStoreContainer( _reader, bodySize, typeCode, version, instance );
		}
	};
}
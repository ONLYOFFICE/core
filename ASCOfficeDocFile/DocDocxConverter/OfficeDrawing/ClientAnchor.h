#pragma once

#include "Record.h"

namespace DocFileFormat
{
	class ClientAnchor: public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF010 = 0xF010;

		ClientAnchor() : Record(), clientanchor(0)
		{
		}

		ClientAnchor (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance), clientanchor(0)
		{
			clientanchor	=	Reader->ReadInt32();
		}

		virtual ~ClientAnchor()
		{
		}

		virtual Record* NewObject (IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
		{
			return new ClientAnchor (_reader, bodySize, typeCode, version, instance);
		}

	private:

		int	clientanchor;
	};
}
#pragma once

#include "Record.h"

namespace DocFileFormat
{
	class ClientData : public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF011 = 0xF011;

	public:

		ClientData () : Record(), clientdata(0)
		{

		}

		ClientData (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance), clientdata(0)
		{
			clientdata	=	Reader->ReadInt32();
		}

		virtual ~ClientData()
		{

		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new ClientData( _reader, bodySize, typeCode, version, instance );
		}

	private:

		int	clientdata;
	};
}
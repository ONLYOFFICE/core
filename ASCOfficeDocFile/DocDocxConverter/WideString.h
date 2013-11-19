#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
	class WideString: public wstring, public ByteStructure
	{
	public:
		WideString( VirtualStreamReader reader, int length ): wstring()
		{
		}

		WideString(): wstring()
		{
		}

		virtual ~WideString()
		{
		}

		virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
		{
			WideString* newObject = new WideString();

			byte *bytes = NULL;
			bytes = reader->ReadBytes( length, true );

			//It's a real string table
			FormatUtils::GetSTLCollectionFromBytes<WideString>( newObject, bytes, length, ENCODING_UNICODE );

			RELEASEARRAYOBJECTS( bytes );

			return static_cast<ByteStructure*>( newObject );
		}
	};
}

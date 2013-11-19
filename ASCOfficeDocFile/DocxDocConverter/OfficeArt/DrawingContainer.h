#pragma once

#include "OfficeArtRecordHeader.h"
#include "OfficeArtDgContainer.h"

namespace OfficeArt
{
	class DrawingContainer : public IOperand
	{
	public: 
		DrawingContainer () : rh(0xF, 0x000, 0xF000, 0), bytes(NULL), sizeInBytes(0)
		{
		}

		DrawingContainer (const OfficeArtDgContainer& shape) :  rh(0xF, 0x000, 0xF000, 0), bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes +=	shape.Size();
			sizeInBytes	+=	sizeof(rh);

			if ( sizeInBytes != 0 )
			{
				bytes = new byte[sizeInBytes];

				if (bytes != NULL )
				{
					memset( bytes, 0, sizeInBytes );

					unsigned int offset = 0;

					memcpy( ( bytes + offset ), (byte*)(rh), sizeof(rh) );
					offset += sizeof(rh);

					memcpy( ( bytes + offset ), (byte*)shape, shape.Size() );
					offset += shape.Size();
				}
			}
		}

		DrawingContainer(const DrawingContainer& data): bytes(NULL), sizeInBytes(data.sizeInBytes)
		{
			if (sizeInBytes)
			{
				bytes = new byte[sizeInBytes];

				if ( bytes != NULL )
				{
					memset(bytes, 0, sizeInBytes);
					memcpy(bytes, data.bytes, sizeInBytes);
				}
			}
		}

		bool operator == (const DrawingContainer& data)
		{
			return ( ( sizeInBytes == data.sizeInBytes ) && ( memcmp( bytes, data.bytes, sizeInBytes ) == 0 ) );
		}

		bool operator != (const DrawingContainer& data)
		{
			return !( this->operator == ( data ) );
		}

		DrawingContainer& operator = (const DrawingContainer& data)
		{
			if (*this != data)
			{
				RELEASEARRAYOBJECTS (bytes);

				sizeInBytes	=	data.sizeInBytes;
				if (sizeInBytes)
				{
					bytes	=	new byte[sizeInBytes];
					if (bytes)
					{
						memcpy(bytes, data.bytes, sizeInBytes);
					}
				}
			}

			return *this;
		}

		virtual ~DrawingContainer()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator byte*() const
		{
			return (byte*)(bytes);
		}

		virtual operator const byte*() const
		{
			return (const byte*)bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:

		OfficeArtRecordHeader	rh;

		byte*					bytes;
		unsigned int			sizeInBytes;
	}; 
}
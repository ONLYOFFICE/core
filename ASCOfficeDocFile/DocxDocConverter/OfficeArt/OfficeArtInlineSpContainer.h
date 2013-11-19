#pragma once

#include "OfficeArtSpContainer.h"
#include "OfficeArtBStoreContainerFileBlock.h"

namespace OfficeArt
{
	class OfficeArtInlineSpContainer: public IOfficeArtAbstractContainer<OfficeArtBStoreContainerFileBlock>
	{
	public:
		OfficeArtInlineSpContainer() : shape(), bytes(NULL), size(0)
		{
			Initialize();
		}

		explicit OfficeArtInlineSpContainer (const OfficeArtSpContainer& oShape) : shape(oShape), bytes(NULL), size(0)
		{
			Initialize();  
		}

		OfficeArtInlineSpContainer (const OfficeArtInlineSpContainer& _officeArtInlineSpContainer) : shape(_officeArtInlineSpContainer.shape), rgfb(_officeArtInlineSpContainer.rgfb), bytes(NULL), size(_officeArtInlineSpContainer.size)
		{
			if (size != 0)
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, _officeArtInlineSpContainer.bytes, size);
				}
			}
		}

		virtual operator const byte* () const
		{
			return (const byte*)(bytes);
		}

		virtual operator byte* () const
		{
			return (byte*)(bytes);
		}

		virtual unsigned int Size() const
		{
			return this->size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtInlineSpContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtInlineSpContainer( *this );
		}

		virtual void PushBack (const OfficeArtBStoreContainerFileBlock& _officeArtBStoreContainerFileBlock )
		{
			rgfb.push_back( OfficeArtBStoreContainerFileBlockPtr( static_cast<OfficeArtBStoreContainerFileBlock*>( _officeArtBStoreContainerFileBlock.Clone() ) ) );
			Initialize();
		}

		virtual unsigned int Count() const
		{
			return (unsigned int)rgfb.size();
		}

		virtual void Clear()
		{
			rgfb.clear();
			Initialize();
		}

		virtual ~OfficeArtInlineSpContainer() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		void Initialize()
		{
			size = shape.Size();

			for ( list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = this->rgfb.begin(); iter != this->rgfb.end(); iter++ )
			{
				this->size += (*iter)->Size();
			}

			RELEASEARRAYOBJECTS (bytes);

			if ( this->size != 0 )
			{
				this->bytes = new byte[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					unsigned int offset = 0;

					memcpy( this->bytes, (byte*)(this->shape), this->shape.Size() );
					offset += this->shape.Size();

					for ( list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = this->rgfb.begin(); iter != this->rgfb.end(); iter++ )
					{
						OfficeArtBStoreContainerFileBlock* officeArtBStoreContainerFileBlock = iter->get();

						if ( officeArtBStoreContainerFileBlock != NULL )
						{
							memcpy( ( this->bytes + offset ), (byte*)(*officeArtBStoreContainerFileBlock), officeArtBStoreContainerFileBlock->Size() );
							offset += officeArtBStoreContainerFileBlock->Size();
						}
					}
				}
			}
		}

	private:

		OfficeArtSpContainer shape;
		list<OfficeArtBStoreContainerFileBlockPtr> rgfb;

		byte* bytes;
		unsigned int size;
	};
}
#pragma once

#include "IOfficeArtAbstractContainer.h"

namespace OfficeArt
{
	class OfficeArtSpgrContainer: public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	protected:
		OfficeArtRecordHeader rh;
		list<OfficeArtRecordPtr> officeArtRecords;

		byte* bytes;
		unsigned int size;

	public:
		OfficeArtSpgrContainer() : rh( 0xF, 0x000, 0xF003, 0 ), bytes(NULL), size(sizeof(OfficeArtRecordHeader))
		{
			Initialize();
		}

		OfficeArtSpgrContainer( const OfficeArtSpgrContainer& _officeContainer ): rh( _officeContainer.rh ), officeArtRecords(_officeContainer.officeArtRecords), size(_officeContainer.size), bytes(NULL)
		{
			if ( size != 0 )
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset( bytes, 0, size );
					memcpy( bytes, _officeContainer.bytes, size );
				}
			}
		}

		virtual operator const byte* () const
		{
			return (const byte*)(this->bytes);
		}

		virtual operator byte* () const
		{
			return (byte*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtSpgrContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSpgrContainer( *this );
		}

		virtual void PushBack( const IOfficeArtRecord& _officeArtRecord )
		{
			this->officeArtRecords.push_back( OfficeArtRecordPtr( _officeArtRecord.Clone() ) );

			this->Initialize();
		}

		virtual unsigned int Count() const
		{
			return (unsigned int)this->officeArtRecords.size();
		}

		virtual void Clear()
		{
			this->officeArtRecords.clear();

			this->Initialize();
		}

		virtual ~OfficeArtSpgrContainer() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:
		void Initialize()
		{
			this->size = 0;

			for ( list<OfficeArtRecordPtr>::const_iterator iter = this->officeArtRecords.begin(); iter != this->officeArtRecords.end(); iter++ )
			{
				this->size += (*iter)->Size();
			}

			this->rh = OfficeArtRecordHeader( 0xF, 0x000, 0xF003, this->size );

			this->size += sizeof(this->rh);
		  
			RELEASEARRAYOBJECTS (bytes);

			if ( this->size != 0 )
			{
				this->bytes = new byte[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					unsigned int offset = 0;

					memcpy( ( this->bytes + offset ), (byte*)(this->rh), sizeof(this->rh) );
					offset += sizeof(this->rh);

					for ( list<OfficeArtRecordPtr>::const_iterator iter = this->officeArtRecords.begin(); iter != this->officeArtRecords.end(); iter++ )
					{
						IOfficeArtRecord* officeArtRecord = iter->get();

						if ( officeArtRecord != NULL )
						{
							memcpy( ( this->bytes + offset ), (byte*)(*officeArtRecord), officeArtRecord->Size() );
							offset += officeArtRecord->Size();
						}
					}
				}
			}
		}
	};
}
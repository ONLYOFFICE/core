#pragma once

#include "IOfficeArtAbstractContainer.h"
#include "OfficeArtFSP.h"

namespace OfficeArt
{
	class OfficeArtSpContainer: public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	public:
		OfficeArtSpContainer() : rh( 0xF, 0x000, 0xF004, 0 ), bytes(NULL), size(sizeof(OfficeArtRecordHeader))
		{
			Initialize();
		}

		OfficeArtSpContainer( const OfficeArtSpContainer& _officeArtSpContainer ) : rh( _officeArtSpContainer.rh ),officeArtRecords(_officeArtSpContainer.officeArtRecords), size(_officeArtSpContainer.size), bytes(NULL)
		{
			if ( this->size != 0 )
			{
				this->bytes = new byte[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					memcpy( this->bytes, _officeArtSpContainer.bytes, this->size );
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
			return new OfficeArtSpContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSpContainer( *this );
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
			officeArtRecords.clear();
			Initialize();
		}

		virtual ~OfficeArtSpContainer() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		void Initialize()
		{
			this->size = 0;

			for ( list<OfficeArtRecordPtr>::const_iterator iter = this->officeArtRecords.begin(); iter != this->officeArtRecords.end(); iter++ )
				this->size += (*iter)->Size();

			this->rh = OfficeArtRecordHeader( 0xF, 0x000, 0xF004, this->size );

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
	protected:
		OfficeArtRecordHeader rh;
		list<OfficeArtRecordPtr> officeArtRecords;

		byte* bytes;
		unsigned int size;
	};

	typedef OfficeArtSpContainer SpContainer;
}
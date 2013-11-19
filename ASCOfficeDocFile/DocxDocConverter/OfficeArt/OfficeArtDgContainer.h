#pragma once

#include "IOfficeArtAbstractContainer.h"

namespace OfficeArt
{
	class OfficeArtDgContainer: public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	public:
		OfficeArtDgContainer () : rh( 0xF, 0x000, 0xF002, 0 ), bytes(NULL), size(sizeof(OfficeArtRecordHeader))
		{
			Initialize();
		}

		OfficeArtDgContainer (const OfficeArtDgContainer& _officeArtDgContainer): rh( _officeArtDgContainer.rh ),officeArtRecords(_officeArtDgContainer.officeArtRecords), size(_officeArtDgContainer.size), bytes(NULL)
		{
			if ( this->size != 0 )
			{
				this->bytes = new byte[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );
					memcpy( this->bytes, _officeArtDgContainer.bytes, this->size );
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
			return new OfficeArtDgContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtDgContainer( *this );
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

		virtual ~OfficeArtDgContainer() 
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

			this->rh = OfficeArtRecordHeader( 0xF, 0x000, 0xF002, this->size );

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
}
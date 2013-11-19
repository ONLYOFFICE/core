#pragma once

namespace OfficeArt
{
	class OfficeArtWordDrawing : public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	public:
		
		OfficeArtWordDrawing (unsigned char type = 0) : dgglbl(type), bytes(NULL), size(sizeof(dgglbl))
		{
			Initialize();
		}

		OfficeArtWordDrawing (const OfficeArtWordDrawing& _container) : dgglbl(_container.dgglbl), officeArtRecords(_container.officeArtRecords), size(_container.size), bytes(NULL)
		{
			if ( size != 0 )
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset (bytes, 0, size);
					memcpy (bytes, _container.bytes, size);
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
			return new OfficeArtWordDrawing;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtWordDrawing (*this);
		}

		virtual void PushBack( const IOfficeArtRecord& _officeArtRecord )
		{
			officeArtRecords.push_back( OfficeArtRecordPtr( _officeArtRecord.Clone() ) );

			Initialize();
		}

		virtual unsigned int Count() const
		{
			return (unsigned int) officeArtRecords.size();
		}

		virtual void Clear()
		{
			officeArtRecords.clear();

			Initialize();
		}

		virtual ~OfficeArtWordDrawing() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}


	private:

		void Initialize()
		{
			size = 0;

			for (list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
			{
				size += (*iter)->Size();
			}

			size += sizeof(dgglbl);

			RELEASEARRAYOBJECTS (bytes);

			if ( size != 0 )
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset (bytes, 0, size);

					unsigned int offset = 0;

					memcpy( (bytes + offset ), &dgglbl, sizeof(dgglbl) );
					offset += sizeof(dgglbl);

					for (list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
					{
						IOfficeArtRecord* officeArtRecord = iter->get();

						if (officeArtRecord)
						{
							memcpy( (bytes + offset), (byte*)(*officeArtRecord), officeArtRecord->Size() );
							offset += officeArtRecord->Size();
						}
					}
				}
			}
		}

	protected:

		unsigned char				dgglbl;
		list<OfficeArtRecordPtr>	officeArtRecords;

		byte*						bytes;
		unsigned int				size;
	};
}
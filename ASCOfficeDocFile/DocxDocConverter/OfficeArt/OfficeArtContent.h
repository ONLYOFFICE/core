#pragma once

namespace OfficeArt
{
	class OfficeArtContent: public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	public:

		OfficeArtContent() : bytes(NULL), size (0)
		{
			Initialize();
		}

		OfficeArtContent(const OfficeArtContent& _office): officeArtRecords(_office.officeArtRecords), size(_office.size), bytes(NULL)
		{
			if (size)
			{
				bytes = new byte[size];
				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, _office.bytes, size);
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
			return size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtContent;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtContent(*this);
		}

		virtual void PushBack( const IOfficeArtRecord& _officeArtRecord )
		{
			officeArtRecords.push_back( OfficeArtRecordPtr( _officeArtRecord.Clone() ) );
			Initialize();
		}

		virtual unsigned int Count() const
		{
			return (unsigned int)officeArtRecords.size();
		}

		virtual void Clear()
		{
			officeArtRecords.clear();
			Initialize();
		}

		virtual ~OfficeArtContent() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		inline void Initialize()
		{
			size = 0;

			for (list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
				size += (*iter)->Size();

			RELEASEARRAYOBJECTS (bytes);

			if (size)
			{
				bytes = new byte[size];
				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					for (list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
					{
						const IOfficeArtRecord* officeArtRecord =  iter->operator->();
						if (officeArtRecord)
						{
							memcpy((bytes + offset), (byte*)(*officeArtRecord), officeArtRecord->Size());
							offset += officeArtRecord->Size();
						}
					}
				}
			}
		}

	protected:

		list<OfficeArtRecordPtr>	officeArtRecords;

		byte*						bytes;
		unsigned int				size;
	};
}
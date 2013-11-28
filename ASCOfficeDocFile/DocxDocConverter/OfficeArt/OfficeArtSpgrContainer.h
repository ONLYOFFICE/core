#pragma once

#include "IOfficeArtAbstractContainer.h"

namespace OfficeArt
{
	class OfficeArtSpgrContainer: public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	public:

		OfficeArtSpgrContainer() : rh(0xF, 0x000, 0xF003, 0), bytes(NULL), size(sizeof(OfficeArtRecordHeader))
		{
			Initialize();
		}

		OfficeArtSpgrContainer(const OfficeArtSpgrContainer& _officeContainer): rh(_officeContainer.rh), officeArtRecords(_officeContainer.officeArtRecords), size(_officeContainer.size), bytes(NULL)
		{
			if ( size != 0 )
			{
				bytes = new byte[size];
				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, _officeContainer.bytes, size);
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
			return new OfficeArtSpgrContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSpgrContainer(*this);
		}

		virtual void PushBack( const IOfficeArtRecord& _officeArtRecord )
		{
			officeArtRecords.push_back( OfficeArtRecordPtr(_officeArtRecord.Clone()));
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

		virtual ~OfficeArtSpgrContainer() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		inline void Initialize()
		{
			size = 0;

			for (std::list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
			{
				size += (*iter)->Size();
			}

			rh		=	OfficeArtRecordHeader(0xF, 0x000, 0xF003, size);
			size	+=	sizeof(rh);

			RELEASEARRAYOBJECTS (bytes);

			if (0 != size)
			{
				bytes = new byte[size];
				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					memcpy((bytes + offset), (byte*)(rh), sizeof(rh));
					offset += sizeof(rh);

					for (std::list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
					{
						const IOfficeArtRecord* officeArtRecord = iter->operator->();
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
		OfficeArtRecordHeader		rh;
		list<OfficeArtRecordPtr>	officeArtRecords;

		byte*						bytes;
		unsigned int				size;
	};
}
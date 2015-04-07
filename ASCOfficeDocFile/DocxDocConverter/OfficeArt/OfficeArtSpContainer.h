#pragma once

#include "IOfficeArtAbstractContainer.h"
#include "OfficeArtFSP.h"

namespace OfficeArt
{
	class OfficeArtSpContainer: public IOfficeArtAbstractContainer<IOfficeArtRecord>
	{
	public:
		OfficeArtSpContainer() : rh(0xF, 0x000, 0xF004, 0), bytes(NULL), size(sizeof(OfficeArtRecordHeader))
		{
			Initialize();
		}

		OfficeArtSpContainer(const OfficeArtSpContainer& _officeArtSpContainer) : rh(_officeArtSpContainer.rh),officeArtRecords(_officeArtSpContainer.officeArtRecords), size(_officeArtSpContainer.size), bytes(NULL)
		{
			if (0 != size)
			{
				bytes = new unsigned char[size];
				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, _officeArtSpContainer.bytes, size);
				}
			}
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(bytes);
		}

		virtual operator unsigned char* () const
		{
			return (unsigned char*)(bytes);
		}

		virtual unsigned int Size() const
		{
			return size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtSpContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSpContainer(*this);
		}

		virtual void PushBack(const IOfficeArtRecord& _officeArtRecord)
		{
			officeArtRecords.push_back(OfficeArtRecordPtr(_officeArtRecord.Clone()));
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

		virtual ~OfficeArtSpContainer() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		inline void Initialize()
		{
			size = 0;

			for (list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
				size += (*iter)->Size();

			rh = OfficeArtRecordHeader( 0xF, 0x000, 0xF004, size );

			size += sizeof(rh);

			RELEASEARRAYOBJECTS (bytes);

			if (0 != size)
			{
				bytes = new unsigned char[size];
				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					memcpy((bytes + offset), (unsigned char*)(rh), sizeof(rh));
					offset += sizeof(rh);

					for (std::list<OfficeArtRecordPtr>::const_iterator iter = officeArtRecords.begin(); iter != officeArtRecords.end(); ++iter)
					{
						const IOfficeArtRecord* officeArtRecord = iter->operator->();
						if (officeArtRecord)
						{
							memcpy((bytes + offset), (unsigned char*)(*officeArtRecord), officeArtRecord->Size());
							offset += officeArtRecord->Size();
						}
					}
				}
			}
		}
	protected:
		OfficeArtRecordHeader		rh;
		list<OfficeArtRecordPtr>	officeArtRecords;

		unsigned char*						bytes;
		unsigned int				size;
	};

	typedef OfficeArtSpContainer SpContainer;
}
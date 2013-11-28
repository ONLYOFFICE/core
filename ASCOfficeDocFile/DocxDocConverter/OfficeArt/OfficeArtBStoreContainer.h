#pragma once

#include "Common.h"
#include "Enumerations.h"

#include "IOfficeArtRecord.h"
#include "IOfficeArtAbstractContainer.h"

#include "OfficeArtRecordHeader.h"
#include "OfficeArtBStoreContainerFileBlock.h"

namespace OfficeArt
{
	class OfficeArtBStoreContainer: public IOfficeArtAbstractContainer<OfficeArtBStoreContainerFileBlock>
	{
	public:

		OfficeArtBStoreContainer () : rh(0xF, 0x0, 0xF001, 0x0), bytes(NULL), size(sizeof(OfficeArtRecordHeader))
		{
			Initialize();
		}

		OfficeArtBStoreContainer (const OfficeArtBStoreContainer& office) : rh(office.rh), rgfb(office.rgfb), bytes(NULL), size(office.size)
		{
			if (size)
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, office.bytes, size);
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
			return new OfficeArtBStoreContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtBStoreContainer( *this );
		}

		virtual void PushBack(const OfficeArtBStoreContainerFileBlock& fileBlock)
		{
			rgfb.push_back( OfficeArtBStoreContainerFileBlockPtr(static_cast<OfficeArtBStoreContainerFileBlock*>(fileBlock.Clone())));
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

		virtual ~OfficeArtBStoreContainer() 
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		inline void Initialize()
		{
			size = 0;

			for (std::list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = rgfb.begin(); iter != rgfb.end(); ++iter)
			{
				size += (*iter)->Size();
			}

			rh		=	OfficeArtRecordHeader (0xF, rgfb.size(), 0xF001, size);
			size	+=	sizeof(rh);

			RELEASEARRAYOBJECTS (bytes);

			if (size)
			{
				bytes = new byte[size];
				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					memcpy((bytes + offset), (byte*)(rh), sizeof(rh));
					offset += sizeof(rh);

					for (std::list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = rgfb.begin(); iter != rgfb.end(); ++iter)
					{
						const OfficeArtBStoreContainerFileBlock* officeArtBStoreContainerFileBlock = iter->operator->();
						if (officeArtBStoreContainerFileBlock)
						{
							memcpy((bytes + offset), (byte*)(*officeArtBStoreContainerFileBlock), officeArtBStoreContainerFileBlock->Size());
							offset += officeArtBStoreContainerFileBlock->Size();
						}
					}
				}
			}
		}

	private:

		OfficeArtRecordHeader						rh;
		list<OfficeArtBStoreContainerFileBlockPtr>	rgfb;

		byte*										bytes;
		unsigned int								size;
	};
}
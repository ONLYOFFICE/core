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
			if (0 != size)
			{
				bytes = new unsigned char[size];
				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, _officeArtInlineSpContainer.bytes, size);
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
			return this->size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtInlineSpContainer;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtInlineSpContainer(*this);
		}

		virtual void PushBack (const OfficeArtBStoreContainerFileBlock& _officeArtBStoreContainerFileBlock )
		{
			rgfb.push_back(OfficeArtBStoreContainerFileBlockPtr(static_cast<OfficeArtBStoreContainerFileBlock*>(_officeArtBStoreContainerFileBlock.Clone())));
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

		inline void Initialize()
		{
			size = shape.Size();

			for (std::list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = rgfb.begin(); iter != rgfb.end(); ++iter)
			{
				size += (*iter)->Size();
			}

			RELEASEARRAYOBJECTS(bytes);

			if (0 != size)
			{
				bytes = new unsigned char[size];
				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					memcpy(bytes, (unsigned char*)(shape), shape.Size());
					offset += shape.Size();

					for (std::list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = rgfb.begin(); iter != rgfb.end(); ++iter)
					{
						const OfficeArtBStoreContainerFileBlock* officeArtBStoreContainerFileBlock = iter->operator->();
						if ( officeArtBStoreContainerFileBlock != NULL )
						{
							memcpy((bytes + offset), (unsigned char*)(*officeArtBStoreContainerFileBlock), officeArtBStoreContainerFileBlock->Size());
							offset += officeArtBStoreContainerFileBlock->Size();
						}
					}
				}
			}
		}

	private:

		OfficeArtSpContainer							shape;
		std::list<OfficeArtBStoreContainerFileBlockPtr> rgfb;

		unsigned char*											bytes;
		unsigned int									size;
	};
}
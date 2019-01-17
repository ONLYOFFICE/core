/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
				bytes = new unsigned char[size];

				if (bytes)
				{
					memset(bytes, 0, size);
					memcpy(bytes, office.bytes, size);
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
				bytes = new unsigned char[size];
				if (bytes)
				{
					memset(bytes, 0, size);

					unsigned int offset = 0;

					memcpy((bytes + offset), (unsigned char*)(rh), sizeof(rh));
					offset += sizeof(rh);

					for (std::list<OfficeArtBStoreContainerFileBlockPtr>::const_iterator iter = rgfb.begin(); iter != rgfb.end(); ++iter)
					{
						const OfficeArtBStoreContainerFileBlock* officeArtBStoreContainerFileBlock = iter->operator->();
						if (officeArtBStoreContainerFileBlock)
						{
							memcpy((bytes + offset), (unsigned char*)(*officeArtBStoreContainerFileBlock), officeArtBStoreContainerFileBlock->Size());
							offset += officeArtBStoreContainerFileBlock->Size();
						}
					}
				}
			}
		}

	private:

		OfficeArtRecordHeader						rh;
		list<OfficeArtBStoreContainerFileBlockPtr>	rgfb;

		unsigned char*										bytes;
		unsigned int								size;
	};
}
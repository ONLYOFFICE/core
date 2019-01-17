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
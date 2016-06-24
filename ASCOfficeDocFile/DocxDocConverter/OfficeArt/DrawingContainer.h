/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "OfficeArtRecordHeader.h"
#include "OfficeArtDgContainer.h"

namespace OfficeArt
{
	class DrawingContainer : public IOperand
	{
	public: 
		DrawingContainer () : rh(0xF, 0x000, 0xF000, 0), bytes(NULL), sizeInBytes(0)
		{
		}

		DrawingContainer (const OfficeArtDgContainer& shape) :  rh(0xF, 0x000, 0xF000, 0), bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes +=	shape.Size();
			sizeInBytes	+=	sizeof(rh);

			if ( sizeInBytes != 0 )
			{
				bytes = new unsigned char[sizeInBytes];

				if (bytes != NULL )
				{
					memset( bytes, 0, sizeInBytes );

					unsigned int offset = 0;

					memcpy( ( bytes + offset ), (unsigned char*)(rh), sizeof(rh) );
					offset += sizeof(rh);

					memcpy( ( bytes + offset ), (unsigned char*)shape, shape.Size() );
					offset += shape.Size();
				}
			}
		}

		DrawingContainer(const DrawingContainer& data): bytes(NULL), sizeInBytes(data.sizeInBytes)
		{
			if (sizeInBytes)
			{
				bytes = new unsigned char[sizeInBytes];

				if ( bytes != NULL )
				{
					memset(bytes, 0, sizeInBytes);
					memcpy(bytes, data.bytes, sizeInBytes);
				}
			}
		}

		bool operator == (const DrawingContainer& data)
		{
			return ( ( sizeInBytes == data.sizeInBytes ) && ( memcmp( bytes, data.bytes, sizeInBytes ) == 0 ) );
		}

		bool operator != (const DrawingContainer& data)
		{
			return !( this->operator == ( data ) );
		}

		DrawingContainer& operator = (const DrawingContainer& data)
		{
			if (*this != data)
			{
				RELEASEARRAYOBJECTS (bytes);

				sizeInBytes	=	data.sizeInBytes;
				if (sizeInBytes)
				{
					bytes	=	new unsigned char[sizeInBytes];
					if (bytes)
					{
						memcpy(bytes, data.bytes, sizeInBytes);
					}
				}
			}

			return *this;
		}

		virtual ~DrawingContainer()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:

		OfficeArtRecordHeader	rh;

		unsigned char*					bytes;
		unsigned int			sizeInBytes;
	}; 
}
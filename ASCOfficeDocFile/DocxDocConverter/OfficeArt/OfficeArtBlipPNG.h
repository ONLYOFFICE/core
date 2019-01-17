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

#include "OfficeArtBlip.h"

namespace OfficeArt
{
	class OfficeArtBlipPNG: public OfficeArtBlip
	{
	public:
		
		OfficeArtBlipPNG () : OfficeArtBlip(OfficeArtRecordHeader( 0x0, 0x6E0, 0xF01E, 17 )), tag(0), BLIPFileData(NULL), blipFileDataSize(0), bytes(NULL), size(0)
		{
			memset ( rgbUid1, 0, rgbUidsSize );
			memset ( rgbUid2, 0, rgbUidsSize );

			Initialize();
		}

		OfficeArtBlipPNG (unsigned char* _blipFileData, unsigned int _blipFileDataSize, 
			const std::vector<unsigned char>& _rgbUid1 = std::vector<unsigned char>(), 
			const std::vector<unsigned char>& _rgbUid2 = std::vector<unsigned char>(),
			unsigned char _tag = 0xFF ):
		OfficeArtBlip(OfficeArtRecordHeader( 0x0, 0x6E0, 0xF01E, 17 )), tag(_tag), BLIPFileData(NULL), blipFileDataSize(_blipFileDataSize), bytes(NULL), size(0)
		{
			memset( rgbUid1, 0, rgbUidsSize );

			memset( rgbUid2, 0, rgbUidsSize );

			size = _blipFileDataSize;

			if ( _rgbUid2.size() != rgbUidsSize )
			{
				size += 17;

				rh = OfficeArtRecordHeader( 0x0, 0x6E0, 0xF01E, size );
			}
			else
			{
				size += 33;

				rh = OfficeArtRecordHeader( 0x0, 0x6E1, 0xF01E, size );
			}

			if ( _rgbUid1.size() == rgbUidsSize )
			{
				for ( unsigned int i = 0; i < _rgbUid1.size(); i++ )
				{
					rgbUid1[i] = _rgbUid1[i];
				}
			}

			if ( _rgbUid2.size() == rgbUidsSize )
			{
				for ( unsigned int i = 0; i < _rgbUid2.size(); i++ )
				{
					rgbUid2[i] = _rgbUid2[i];
				}
			}

			if ( ( _blipFileData != NULL ) && ( blipFileDataSize != 0 ) )
			{
				BLIPFileData = new unsigned char[blipFileDataSize];

				if ( BLIPFileData != NULL )
				{
					memset( BLIPFileData, 0, blipFileDataSize );

					memcpy( BLIPFileData, _blipFileData, blipFileDataSize );
				}
			}

			this->Initialize();
		}

		OfficeArtBlipPNG (const OfficeArtBlipPNG& _officeArtBlipPNG) : OfficeArtBlip(OfficeArtRecordHeader( _officeArtBlipPNG.rh )), tag(_officeArtBlipPNG.tag), BLIPFileData(NULL), blipFileDataSize(_officeArtBlipPNG.blipFileDataSize), bytes(NULL), size(_officeArtBlipPNG.size)
		{
			memset( rgbUid1, 0, rgbUidsSize );
			memcpy( rgbUid1, _officeArtBlipPNG.rgbUid1, rgbUidsSize );

			memset( rgbUid2, 0, rgbUidsSize );
			memcpy( rgbUid2, _officeArtBlipPNG.rgbUid2, rgbUidsSize );

			if ( this->blipFileDataSize != 0 )
			{
				this->BLIPFileData = new unsigned char[this->blipFileDataSize];

				if ( NULL != BLIPFileData )
				{
					memset( BLIPFileData, 0, blipFileDataSize );

					memcpy( BLIPFileData, _officeArtBlipPNG.BLIPFileData, blipFileDataSize );
				}
			}

			if ( 0 != size )
			{
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					memcpy( this->bytes, _officeArtBlipPNG.bytes, this->size );
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
			return new OfficeArtBlipPNG;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtBlipPNG(*this);
		}

		virtual ~OfficeArtBlipPNG() 
		{
			RELEASEARRAYOBJECTS(BLIPFileData);
			RELEASEARRAYOBJECTS(bytes);
		}

	private:
		inline void Initialize()
		{
			size = ( sizeof(rh) + rh.GetLength() );

			if ( size != 0 )
			{
				bytes = new unsigned char[size];

				if ( NULL != bytes )
				{
					memset( this->bytes, 0, this->size );

					unsigned int offset = 0;

					memcpy( ( this->bytes + offset ), (unsigned char*)(this->rh), sizeof(this->rh) );
					offset += sizeof(this->rh);

					memcpy( ( this->bytes + offset ), (unsigned char*)(this->rgbUid1), sizeof(this->rgbUid1) );
					offset += sizeof(this->rgbUid1);

					if ( this->rh.GetInstance() == 0x6E1 )
					{
						memcpy( ( this->bytes + offset ), (unsigned char*)(this->rgbUid2), sizeof(this->rgbUid2) );
						offset += sizeof(this->rgbUid2);  
					}

					memcpy( ( this->bytes + offset ), &(this->tag), sizeof(this->tag) );
					offset += sizeof(this->tag);

					if ( ( this->BLIPFileData != NULL ) && ( this->blipFileDataSize != 0 ) )
					{
						memcpy( ( this->bytes + offset ), this->BLIPFileData, this->blipFileDataSize );
						offset += this->blipFileDataSize;
					}
				}
			}	      
		}

	private:
		static const unsigned char rgbUidsSize = 16;
		unsigned char rgbUid1[rgbUidsSize];
		unsigned char rgbUid2[rgbUidsSize];
		unsigned char tag;
		unsigned char* BLIPFileData;
		unsigned int blipFileDataSize;

		unsigned char* bytes;
		unsigned int size;
	};
}
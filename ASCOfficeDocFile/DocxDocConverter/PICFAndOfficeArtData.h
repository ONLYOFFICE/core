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

#include "PICF.h"
#include "OfficeArt/OfficeArtInlineSpContainer.h"

namespace ASCDocFileFormat
{
	class PICFAndOfficeArtData : public IOperand
	{
	public:

		PICFAndOfficeArtData () : bytes(NULL), sizeInBytes(0)
		{
		}

		explicit PICFAndOfficeArtData (const PICF& _picf, const OfficeArt::OfficeArtInlineSpContainer& _picture, const char* _stPicName = NULL ) : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = _picf.Size();

			if ( ( _picf.GetPictureDataFormat() == Constants::MM_SHAPEFILE ) && ( _stPicName != NULL ) )
			{
				this->sizeInBytes += ( sizeof(unsigned char) + ( sizeof(char) * strlen( _stPicName ) ) );
			}

			this->sizeInBytes += _picture.Size();

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					unsigned int offset = 0;

					memcpy( ( this->bytes + offset ), (unsigned char*)_picf, _picf.Size() );
					offset += _picf.Size();

					if ( ( _picf.GetPictureDataFormat() == Constants::MM_SHAPEFILE ) && ( _stPicName != NULL ) )
					{
						unsigned char cchPicName = ( sizeof(char) * strlen( _stPicName ) );

						this->bytes[offset] = cchPicName;
						offset += sizeof(cchPicName);

						memcpy( ( this->bytes + offset ), _stPicName, cchPicName );
						offset += cchPicName;
					}

					memcpy( ( this->bytes + offset ), (unsigned char*)_picture, _picture.Size() );
					offset += _picture.Size();
				}
			}
		}

		PICFAndOfficeArtData( const PICFAndOfficeArtData& _pICFAndOfficeArtData ) : bytes(NULL), sizeInBytes(_pICFAndOfficeArtData.sizeInBytes)
		{
			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _pICFAndOfficeArtData.bytes, this->sizeInBytes );
				}
			}
		}

		bool operator == ( const PICFAndOfficeArtData& _pICFAndOfficeArtData )
		{
			return ( ( this->sizeInBytes == _pICFAndOfficeArtData.sizeInBytes ) && 
				( memcmp( this->bytes, _pICFAndOfficeArtData.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const PICFAndOfficeArtData& _pICFAndOfficeArtData )
		{
			return !( this->operator == ( _pICFAndOfficeArtData ) );
		}

		PICFAndOfficeArtData& operator = ( const PICFAndOfficeArtData& _pICFAndOfficeArtData )
		{
			if ( *this != _pICFAndOfficeArtData )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->sizeInBytes = _pICFAndOfficeArtData.sizeInBytes;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _pICFAndOfficeArtData.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~PICFAndOfficeArtData()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}

	private:

		unsigned char* bytes;
		unsigned int sizeInBytes;
	};
}

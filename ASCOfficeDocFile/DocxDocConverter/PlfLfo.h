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

#include "LFO.h"
#include "LFOData.h"

namespace ASCDocFileFormat
{
	class PlfLfo: public IOperand
	{
	private:
		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:
		PlfLfo() : bytes(NULL), sizeInBytes(sizeof(unsigned int))
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
			}
		}

		explicit PlfLfo( const vector<LFO>& _rgLfo, const vector<LFOData>& _rgLfoData ) : bytes(NULL), sizeInBytes(sizeof(unsigned int))
		{
			if ( _rgLfo.size() != _rgLfoData.size() )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );
				}
			}
			else
			{
				for ( vector<LFO>::const_iterator iter = _rgLfo.begin(); iter != _rgLfo.end(); iter++ )
				{
					this->sizeInBytes += iter->Size();
				}

				for ( vector<LFOData>::const_iterator iter = _rgLfoData.begin(); iter != _rgLfoData.end(); iter++ )
				{
					this->sizeInBytes += iter->Size();
				}

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					FormatUtils::SetBytes( this->bytes, (unsigned int)_rgLfo.size() );

					unsigned int offset = sizeof(unsigned int);

					if ( this->bytes != NULL )
					{
						for ( vector<LFO>::const_iterator iter = _rgLfo.begin(); iter != _rgLfo.end(); iter++ )
						{
							memcpy( ( this->bytes + offset ), (unsigned char*)(*iter), iter->Size() );
							offset += iter->Size();
						}

						for ( vector<LFOData>::const_iterator iter = _rgLfoData.begin(); iter != _rgLfoData.end(); iter++ )
						{
							memcpy( ( this->bytes + offset ), (unsigned char*)(*iter), iter->Size() );
							offset += iter->Size();
						}
					}
				}
			}
		}

		PlfLfo( const PlfLfo& _plfLfo ):
		bytes(NULL), sizeInBytes(_plfLfo.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if (bytes)
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _plfLfo.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const PlfLfo& _plfLfo )
		{
			return ( ( this->sizeInBytes == _plfLfo.sizeInBytes ) && 
				( memcmp( this->bytes, _plfLfo.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const PlfLfo& _plfLfo )
		{
			return !( this->operator == ( _plfLfo ) );
		}

		PlfLfo& operator = ( const PlfLfo& _plfLfo )
		{
			if ( *this != _plfLfo )
			{
				RELEASEARRAYOBJECTS(bytes);

				this->sizeInBytes = _plfLfo.sizeInBytes;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _plfLfo.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~PlfLfo()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		virtual operator unsigned char*() const
		{
			return this->bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	};
}
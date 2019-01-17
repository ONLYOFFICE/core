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

#include "Prl.h"

namespace ASCDocFileFormat
{
	class UpxChpx: public IOperand
	{
	public:
		UpxChpx() : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
		}

		UpxChpx( const vector<Prl>& _grpprlChpx ) : grpprlChpx(_grpprlChpx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			for ( vector<Prl>::const_iterator iter = this->grpprlChpx.begin(); iter != this->grpprlChpx.end(); iter++ )
			{
				this->sizeInBytesWithoutPadding += iter->Size();
			}

			this->sizeInBytes = this->sizeInBytesWithoutPadding;

			if ( this->sizeInBytes > 0 )
			{
				//The padding to be an even length
				if ( this->sizeInBytes % 2 != 0 )
				{
					this->sizeInBytes++;
				}

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					unsigned char* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0;

					for ( vector<Prl>::iterator iter = this->grpprlChpx.begin(); iter != this->grpprlChpx.end(); iter++ )
					{
						prlBytes = iter->GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( this->bytes + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}
		}

		UpxChpx( const UpxChpx& _upxChpx ) : grpprlChpx(_upxChpx.grpprlChpx), bytes(NULL), sizeInBytes(_upxChpx.sizeInBytes),sizeInBytesWithoutPadding(_upxChpx.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _upxChpx.bytes, this->sizeInBytes );
			}
		}

		virtual ~UpxChpx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		UpxChpx& operator = ( const UpxChpx& _upxChpx )
		{
			if ( this != &_upxChpx )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->grpprlChpx = _upxChpx.grpprlChpx;
				this->sizeInBytes = _upxChpx.sizeInBytes;
				this->sizeInBytesWithoutPadding = _upxChpx.sizeInBytesWithoutPadding;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _upxChpx.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		const vector<Prl> GetProperties() const
		{
			return this->grpprlChpx;
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

		unsigned int SizeWithoutPadding() const
		{
			return this->sizeInBytesWithoutPadding;
		}
	private:

		vector<Prl> grpprlChpx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};

	class LPUpxChpx : public IOperand
	{
	private: 

		void Init()
		{
			this->sizeInBytes = ( sizeof(unsigned short) + this->upxChpx.Size() );
			this->sizeInBytesWithoutPadding = ( sizeof(unsigned short) + this->upxChpx.SizeWithoutPadding() );

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				FormatUtils::SetBytes( this->bytes, (unsigned short)this->upxChpx.SizeWithoutPadding() );

				memcpy( ( this->bytes + sizeof(unsigned short) ), (unsigned char*)this->upxChpx, this->upxChpx.Size() );
			}
		}
	public:

		LPUpxChpx():  upxChpx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxChpx( const UpxChpx& _upxChpx ): upxChpx(_upxChpx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxChpx( const LPUpxChpx& _lPUpxChpx ) : upxChpx(_lPUpxChpx.upxChpx), bytes(NULL), sizeInBytes(_lPUpxChpx.sizeInBytes), sizeInBytesWithoutPadding(_lPUpxChpx.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0 ,this->sizeInBytes );

				memcpy( this->bytes, _lPUpxChpx.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const LPUpxChpx& _lPUpxChpx )
		{
			return ( ( this->sizeInBytes == _lPUpxChpx.sizeInBytes ) &&
				( this->sizeInBytesWithoutPadding == _lPUpxChpx.sizeInBytesWithoutPadding ) &&
				( memcmp( this->bytes, _lPUpxChpx.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const LPUpxChpx& _lPUpxChpx )
		{
			return !( this->operator == ( _lPUpxChpx ) );
		}

		LPUpxChpx& operator = ( const LPUpxChpx& _lPUpxChpx )
		{
			if ( this != &_lPUpxChpx )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->upxChpx = _lPUpxChpx.upxChpx;
				this->sizeInBytes = _lPUpxChpx.sizeInBytes;
				this->sizeInBytesWithoutPadding = _lPUpxChpx.sizeInBytesWithoutPadding;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _lPUpxChpx.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~LPUpxChpx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return this->upxChpx.GetProperties();
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

		unsigned int SizeWithoutPadding() const
		{
			return this->sizeInBytesWithoutPadding;
		}
	private:

		UpxChpx upxChpx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};
}
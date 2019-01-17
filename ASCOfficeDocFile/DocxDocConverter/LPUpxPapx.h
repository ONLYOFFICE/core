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
	class UpxPapx: public IOperand
	{
	public:

		UpxPapx() :  istd(0), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{

		}

		explicit UpxPapx( unsigned short _istd, const vector<Prl>& _grpprlPapx ) : istd(_istd), grpprlPapx(_grpprlPapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytesWithoutPadding = sizeof(this->istd);

			for ( vector<Prl>::const_iterator iter = this->grpprlPapx.begin(); iter != this->grpprlPapx.end(); iter++ )
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

					FormatUtils::SetBytes( this->bytes, this->istd );

					unsigned char* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0;

					for ( vector<Prl>::iterator iter = this->grpprlPapx.begin(); iter != this->grpprlPapx.end(); iter++ )
					{
						prlBytes = iter->GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( this->bytes + sizeof(this->istd) + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;
						
							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}
		}

		UpxPapx( const UpxPapx& _upxPapx ) : istd(_upxPapx.istd), grpprlPapx(_upxPapx.grpprlPapx), bytes(NULL), sizeInBytes(_upxPapx.sizeInBytes), sizeInBytesWithoutPadding(_upxPapx.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _upxPapx.bytes, this->sizeInBytes );
			}
		}

		virtual ~UpxPapx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		UpxPapx& operator = ( const UpxPapx& _upxPapx )
		{
			if ( this != &_upxPapx )
			{
				RELEASEARRAYOBJECTS (bytes);
				this->istd = _upxPapx.istd;
				this->grpprlPapx = _upxPapx.grpprlPapx;
				this->sizeInBytes = _upxPapx.sizeInBytes;
				this->sizeInBytesWithoutPadding = _upxPapx.sizeInBytesWithoutPadding;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _upxPapx.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		const vector<Prl> GetProperties() const
		{
			return this->grpprlPapx;
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

		unsigned short istd;
		vector<Prl> grpprlPapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};

	class LPUpxPapx: public IOperand
	{
	private:
		UpxPapx upxPapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;

	private: 

		void Init()
		{
			this->sizeInBytes = ( sizeof(unsigned short) + this->upxPapx.Size() );
			this->sizeInBytesWithoutPadding = ( sizeof(unsigned short) + this->upxPapx.SizeWithoutPadding() );

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				FormatUtils::SetBytes( this->bytes, (unsigned short)this->upxPapx.SizeWithoutPadding() );

				memcpy( ( this->bytes + sizeof(unsigned short) ), (unsigned char*)this->upxPapx, this->upxPapx.Size() );
			}
		}
	public:

		LPUpxPapx() : upxPapx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		explicit LPUpxPapx( const UpxPapx& _upxPapx ) : upxPapx(_upxPapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxPapx( const LPUpxPapx& _lPUpxPapx ) : upxPapx(_lPUpxPapx.upxPapx), bytes(NULL), sizeInBytes(_lPUpxPapx.sizeInBytes), sizeInBytesWithoutPadding(_lPUpxPapx.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0 ,this->sizeInBytes );

				memcpy( this->bytes, _lPUpxPapx.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const LPUpxPapx& _lPUpxPapx )
		{
			return ( ( this->sizeInBytes == _lPUpxPapx.sizeInBytes ) && 
				( this->sizeInBytesWithoutPadding == _lPUpxPapx.sizeInBytesWithoutPadding ) &&
				( memcmp( this->bytes, _lPUpxPapx.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const LPUpxPapx& _lPUpxPapx )
		{
			return !( this->operator == ( _lPUpxPapx ) );
		}

		LPUpxPapx& operator = ( const LPUpxPapx& _lPUpxPapx )
		{
			if ( this != &_lPUpxPapx )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->upxPapx = _lPUpxPapx.upxPapx;
				this->sizeInBytes = _lPUpxPapx.sizeInBytes;
				this->sizeInBytesWithoutPadding = _lPUpxPapx.sizeInBytesWithoutPadding;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _lPUpxPapx.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~LPUpxPapx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return this->upxPapx.GetProperties();
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
	};
}
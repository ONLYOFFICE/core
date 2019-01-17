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
	class UpxTapx : public IOperand
	{
	public:

		UpxTapx() : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{

		}

		UpxTapx( const vector<Prl>& _grpprlTapx ) : grpprlTapx(_grpprlTapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			for ( vector<Prl>::const_iterator iter = this->grpprlTapx.begin(); iter != this->grpprlTapx.end(); iter++ )
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

					for ( vector<Prl>::iterator iter = this->grpprlTapx.begin(); iter != this->grpprlTapx.end(); iter++ )
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

		UpxTapx( const UpxTapx& _upxTapx ):
		grpprlTapx(_upxTapx.grpprlTapx), bytes(NULL), sizeInBytes(_upxTapx.sizeInBytes),
			sizeInBytesWithoutPadding(_upxTapx.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _upxTapx.bytes, this->sizeInBytes );
			}
		}

		virtual ~UpxTapx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		UpxTapx& operator = ( const UpxTapx& _upxTapx )
		{
			if ( this != &_upxTapx )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->grpprlTapx = _upxTapx.grpprlTapx;
				this->sizeInBytes = _upxTapx.sizeInBytes;
				this->sizeInBytesWithoutPadding = _upxTapx.sizeInBytesWithoutPadding;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _upxTapx.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		const vector<Prl> GetProperties() const
		{
			return this->grpprlTapx;
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

		vector<Prl> grpprlTapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};

	class LPUpxTapx : public IOperand
	{	
	public:

		LPUpxTapx () : upxTapx(), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxTapx (const UpxTapx& _upxTapx) : upxTapx(_upxTapx), bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			Init();
		}

		LPUpxTapx (const LPUpxTapx& _lPUpxTapx) : upxTapx(_lPUpxTapx.upxTapx), bytes(NULL), sizeInBytes(_lPUpxTapx.sizeInBytes), sizeInBytesWithoutPadding(_lPUpxTapx.sizeInBytesWithoutPadding)
		{
			bytes = new unsigned char[sizeInBytes];

			if ( bytes != NULL )
			{
				memset( bytes, 0, sizeInBytes );
				memcpy( bytes, _lPUpxTapx.bytes, sizeInBytes );
			}
		}

		bool operator == (const LPUpxTapx& _lPUpxTapx)
		{
			return ( ( this->sizeInBytes == _lPUpxTapx.sizeInBytes ) &&	( this->sizeInBytesWithoutPadding == _lPUpxTapx.sizeInBytesWithoutPadding ) && ( memcmp( this->bytes, _lPUpxTapx.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != (const LPUpxTapx& _lPUpxTapx)
		{
			return !( this->operator == ( _lPUpxTapx ) );
		}

		LPUpxTapx& operator = (const LPUpxTapx& _lPUpxTapx)
		{
			if ( this != &_lPUpxTapx )
			{
				RELEASEARRAYOBJECTS (bytes);

				upxTapx						=	_lPUpxTapx.upxTapx;
				sizeInBytes					=	_lPUpxTapx.sizeInBytes;
				sizeInBytesWithoutPadding	=	_lPUpxTapx.sizeInBytesWithoutPadding;

				bytes						=	new unsigned char[sizeInBytes];

				if ( bytes != NULL )
				{
					memcpy ( bytes, _lPUpxTapx.bytes, sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~LPUpxTapx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return upxTapx.GetProperties();
		}

		virtual operator unsigned char*() const
		{
			return bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeInBytes;
		}

		unsigned int SizeWithoutPadding() const
		{
			return sizeInBytesWithoutPadding;
		}


	private: 

		void Init()
		{
			this->sizeInBytes = ( sizeof(unsigned short) + this->upxTapx.Size() );
			this->sizeInBytesWithoutPadding = ( sizeof(unsigned short) + this->upxTapx.SizeWithoutPadding() );

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				FormatUtils::SetBytes( this->bytes, (unsigned short)this->upxTapx.SizeWithoutPadding() );

				memcpy( ( this->bytes + sizeof(unsigned short) ), (unsigned char*)this->upxTapx, this->upxTapx.Size() );
			}
		}
	private:

		UpxTapx upxTapx;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};
}
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
	class SDxaColSpacingOperand : public IOperand
	{
	public:

		SDxaColSpacingOperand()
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );
		}

		explicit SDxaColSpacingOperand( unsigned char _iCol, XAS_nonNeg _dxaCol )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			if ( _iCol > 43 )
			{
				this->bytes[0] = 43;
			}
			else
			{
				this->bytes[0] = _iCol;
			}

			FormatUtils::SetBytes( ( this->bytes + sizeof(_iCol) ), (unsigned short)_dxaCol );
		}

		SDxaColSpacingOperand( const SDxaColSpacingOperand& _sDxaColSpacingOperand )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );
			memcpy( this->bytes, _sDxaColSpacingOperand.bytes, SIZE_IN_BYTES );
		}

		virtual ~SDxaColSpacingOperand()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}
	private:

		static const unsigned char SIZE_IN_BYTES = 3;
		unsigned char bytes[SIZE_IN_BYTES];
	};

	class SDxaColWidthOperand : public IOperand
	{
	public:
		SDxaColWidthOperand()
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );
		}

		explicit SDxaColWidthOperand( unsigned char _iCol, XAS_nonNeg _dxaCol )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			if ( _iCol > 43 )
			{
				this->bytes[0] = 43;
			}
			else
			{
				this->bytes[0] = _iCol;
			}

			unsigned short dxaCol = 718;

			if ( (unsigned short)_dxaCol > 718 )
			{
				dxaCol = (unsigned short)_dxaCol;  
			}

			FormatUtils::SetBytes( ( this->bytes + sizeof(_iCol) ), dxaCol );
		}

		SDxaColWidthOperand( const SDxaColWidthOperand& _sDxaColWidthOperand )
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			memcpy( this->bytes, _sDxaColWidthOperand.bytes, SIZE_IN_BYTES );
		}

		virtual ~SDxaColWidthOperand()
		{
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}
	private:

		static const unsigned char SIZE_IN_BYTES = 3;
		unsigned char bytes[SIZE_IN_BYTES];
	};

	class Sepx : public IOperand
	{
	public:

		Sepx() : cb(0), bytes(NULL), size(0)
		{
			this->size = ( sizeof(this->cb) + this->cb );

			if ( this->size != 0 )
			{
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );
					FormatUtils::SetBytes( this->bytes, this->cb );
				}
			}
		}

		Sepx( const vector<Prl>& _grpprl ):
		cb(0), bytes(NULL), size(0)
		{
			for ( unsigned int i = 0; i < _grpprl.size(); i++ )
			{
				this->cb += (short)_grpprl[i].Size();
				this->grpprl.push_back( _grpprl[i] );
			}

			this->size = ( sizeof(this->cb) + this->cb );

			if ( this->size != 0 )
			{
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );
					FormatUtils::SetBytes( this->bytes, this->cb );

					unsigned char* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0; 

					for ( unsigned int i = 0; i < this->grpprl.size(); i++ )
					{
						prlBytes = this->grpprl[i].GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( this->bytes + sizeof(this->cb) + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}
		}

		Sepx( const Sepx& sepx ) : cb(sepx.cb), bytes(NULL), size(sepx.size)
		{
			for ( unsigned int i = 0; i < sepx.grpprl.size(); i++ )
			{
				this->grpprl.push_back( sepx.grpprl[i] );
			}

			if ( this->size != 0 )
			{
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );
					memcpy( this->bytes, sepx.bytes, this->size );
				}
			}
		}

		virtual ~Sepx()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		bool operator == ( const Sepx& sepx )
		{
			if ( ( this->cb != sepx.cb ) || ( this->grpprl.size() != sepx.grpprl.size() ) || ( this->size != sepx.size ) )
			{
				return false;
			}
			else
			{
				unsigned int minPrlCount = min( this->grpprl.size(), sepx.grpprl.size() );

				for ( unsigned int i = 0; i < minPrlCount; i++ )
				{
					if ( this->grpprl[i] != sepx.grpprl[i] )
					{
						return false;
					}
				}

				if ( memcmp( this->bytes, sepx.bytes, this->size ) != 0 )
				{
					return false;
				}
			}

			return true;
		}

		bool operator != ( const Sepx& sepx )
		{
			return !( *this == sepx );
		}

		Sepx& operator = ( const Sepx& sepx )
		{
			if ( this != &sepx )
			{
				this->cb = sepx.cb;
				this->size = sepx.size;

				this->grpprl.clear();

				for ( unsigned int i = 0; i < sepx.grpprl.size(); i++ )
				{
					this->grpprl.push_back( sepx.grpprl[i] );
				}

				RELEASEARRAYOBJECTS (bytes);

				if ( this->size != 0 )
				{
					this->bytes = new unsigned char[this->size];

					if ( this->bytes != NULL )
					{
						memset( this->bytes, 0, this->size );
						memcpy( this->bytes, sepx.bytes, this->size );
					}
				}
			}

			return *this;
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(this->bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)(this->bytes);
		}

		virtual unsigned int Size() const
		{
			return this->size;
		}

	private:

		short cb; //Size in bytes of the grpprl
		vector<Prl> grpprl;
		unsigned char* bytes;
		unsigned int size;
	};
}
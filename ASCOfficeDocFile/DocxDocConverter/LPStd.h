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

#include "Stdf.h"
#include "Xstz.h"
#include "GrLPUpxSw.h"

namespace ASCDocFileFormat
{
	class STD: public IOperand
	{
		friend class LPStd;

	public:
		STD(): bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
		}

		explicit STD( const Stdf& _stdf, const Xstz& _xstzName, const GrLPUpxSw& _grLPUpxSw ): bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			this->sizeInBytes = ( _stdf.Size() + _xstzName.Size() + _grLPUpxSw.Size() );
			this->sizeInBytesWithoutPadding = ( _stdf.Size() + _xstzName.Size() + _grLPUpxSw.SizeWithoutPadding() );
			this->properties = _grLPUpxSw.GetProperties();

			this->bytes = new unsigned char[this->sizeInBytes];

			if (bytes)
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, (unsigned char*)_stdf, _stdf.Size() );
				memcpy( ( this->bytes + _stdf.Size() ), (unsigned char*)_xstzName, _xstzName.Size() );
				memcpy( ( this->bytes + _stdf.Size() + _xstzName.Size() ), (unsigned char*)_grLPUpxSw, _grLPUpxSw.Size() );
			}
		}

		STD( const STD& _sTD ) : properties(_sTD.properties), bytes(NULL), sizeInBytes(_sTD.sizeInBytes), sizeInBytesWithoutPadding(_sTD.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _sTD.bytes, this->sizeInBytes );
			}
		}

		virtual ~STD()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return this->properties;
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

		vector<Prl> properties;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;
	};

	class LPStd: public IOperand
	{
	private:
		vector<Prl> properties;

		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int sizeInBytesWithoutPadding;

	public:
		LPStd() : bytes(NULL), sizeInBytes(sizeof(short)), sizeInBytesWithoutPadding(sizeof(short))
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
			}
		}

		explicit LPStd( const STD& _std ) : bytes(NULL), sizeInBytes(0), sizeInBytesWithoutPadding(0)
		{
			STD std(_std);

			FormatUtils::SetBytes( ( std.bytes + 6 ), (unsigned short)std.Size()/*SizeWithoutPadding()*/ );

			this->sizeInBytes = ( sizeof(short) + std.Size() );
			this->sizeInBytesWithoutPadding = ( sizeof(short) + std.SizeWithoutPadding() );
			this->properties = _std.GetProperties();

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				FormatUtils::SetBytes( this->bytes, (short)std.Size()/*SizeWithoutPadding()*/ );

				memcpy( ( this->bytes + sizeof(short) ), (unsigned char*)std, std.Size() );
				// заполняем поле bchUpe StdfBase
				FormatUtils::SetBytes( this->bytes + sizeof(short) + 6, (short)std.Size() );
			}
		}

		LPStd( const LPStd& _lPStd ) : properties(_lPStd.properties), bytes(NULL), sizeInBytes(_lPStd.sizeInBytes), sizeInBytesWithoutPadding(_lPStd.sizeInBytesWithoutPadding)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _lPStd.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const LPStd& _lPStd )
		{
			return ( ( this->sizeInBytes == _lPStd.sizeInBytes ) &&
				( this->sizeInBytesWithoutPadding == _lPStd.sizeInBytesWithoutPadding ) &&
				( memcmp( this->bytes, _lPStd.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const LPStd& _lPStd )
		{
			return !( this->operator == ( _lPStd ) );
		}

		LPStd& operator = ( const LPStd& _lPStd )
		{
			if ( *this != _lPStd )
			{
				RELEASEARRAYOBJECTS(bytes);

				this->sizeInBytes = _lPStd.sizeInBytes;
				this->sizeInBytesWithoutPadding = _lPStd.sizeInBytesWithoutPadding;
				this->properties = _lPStd.properties;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _lPStd.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~LPStd()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		const vector<Prl> GetProperties() const
		{
			return this->properties;
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
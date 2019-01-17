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

#include "LVL.h"

namespace ASCDocFileFormat
{
	class LFOLVL: public IOperand
	{
	private:
		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:

		LFOLVL() : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = ( sizeof(int) + sizeof(unsigned int) );

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );
				}
			}
		}

		explicit LFOLVL( int _iStartAt, unsigned char _iLvl, bool _fStartAt, grfhic _grfhic, const LVL* _lvl = NULL ) : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = ( sizeof(_iStartAt) + sizeof(unsigned int) );

			if ( _lvl != NULL )
			{
				this->sizeInBytes += _lvl->Size();
			}

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					int iStartAt = 0;

					if ( _iStartAt > 0x7FFF )
					{
						iStartAt = 0x7FFF; 
					}
					else if ( _iStartAt < 0 )
					{
						iStartAt = 0;  
					}
					else
					{
						iStartAt = _iStartAt;  
					}

					FormatUtils::SetBytes( this->bytes, iStartAt );

					unsigned int flags = 0;
					unsigned int fStartAt = 0;
					unsigned int fFormatting = 0;
					unsigned int grfhicValue = (unsigned int)(unsigned char)_grfhic;
					grfhicValue <<= 6;

					( _iLvl <= 0x08 ) ? ( flags = _iLvl ) : ( flags = 0x08 );
					( _fStartAt ) ? ( fStartAt = 0x10 ) : ( fStartAt = 0 );
					( _lvl != NULL ) ? ( fFormatting = 0x20 ) : ( fFormatting = 0 );

					flags |= ( fStartAt | fFormatting | grfhicValue );

					FormatUtils::SetBytes( ( this->bytes + sizeof(iStartAt) ), flags );

					if ( _lvl != NULL )
					{
						memcpy( ( this->bytes + sizeof(iStartAt) + sizeof(flags) ), (unsigned char*)(*_lvl), _lvl->Size() );
					}
				}
			}
		}

		LFOLVL( const LFOLVL& _lFOLVL ) : bytes(NULL), sizeInBytes(_lFOLVL.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _lFOLVL.bytes, this->sizeInBytes );
			}
		}

		virtual ~LFOLVL()
		{
			RELEASEARRAYOBJECTS (bytes);
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
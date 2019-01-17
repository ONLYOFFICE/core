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

#include "PlfLst.h"
#include "LVL.h"

namespace ASCDocFileFormat
{
	class ListFormattingInformation: public IOperand
	{
	private:
		unsigned char* bytes;
		unsigned int sizeInBytes;
		unsigned int listFormattingInformationSize;
		unsigned int listLevelsSize;

	public:

		ListFormattingInformation() : bytes(NULL), sizeInBytes(0), listFormattingInformationSize(0), listLevelsSize(0)
		{
		}

		explicit ListFormattingInformation( const PlfLst& _plfLst, const vector<LVL>& _lvls ) : bytes(NULL), sizeInBytes(0), listFormattingInformationSize(0), listLevelsSize(0)
		{
			this->listFormattingInformationSize = _plfLst.Size();

			for ( vector<LVL>::const_iterator iter = _lvls.begin(); iter != _lvls.end(); iter++ )
			{
				this->listLevelsSize += iter->Size();
			}

			this->sizeInBytes = ( this->listFormattingInformationSize + this->listLevelsSize );

			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, (unsigned char*)_plfLst, _plfLst.Size() );

					unsigned int offset = _plfLst.Size();

					for ( vector<LVL>::const_iterator iter = _lvls.begin(); iter != _lvls.end(); iter++ )
					{
						memcpy( ( this->bytes + offset ), (unsigned char*)(*iter), iter->Size() );
						offset += iter->Size();
					}
				}
			}
		}

		ListFormattingInformation( const ListFormattingInformation& _lfi ) : bytes(NULL), sizeInBytes(_lfi.sizeInBytes), listFormattingInformationSize(_lfi.listFormattingInformationSize), listLevelsSize(_lfi.listLevelsSize)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );

				memcpy( this->bytes, _lfi.bytes, this->sizeInBytes );
			}
		}

		bool operator == ( const ListFormattingInformation& _lfi )
		{
			return ( ( this->sizeInBytes == _lfi.sizeInBytes ) &&
				( this->listFormattingInformationSize == _lfi.listFormattingInformationSize ) &&
				( this->listLevelsSize == _lfi.listLevelsSize ) &&
				( memcmp( this->bytes, _lfi.bytes, this->sizeInBytes ) == 0 ) );
		}

		bool operator != ( const ListFormattingInformation& _lfi )
		{
			return !( this->operator == ( _lfi ) );
		}

		ListFormattingInformation& operator = ( const ListFormattingInformation& _lfi )
		{
			if ( *this != _lfi )
			{
				RELEASEARRAYOBJECTS (bytes);

				this->sizeInBytes = _lfi.sizeInBytes;
				this->listFormattingInformationSize = _lfi.listFormattingInformationSize;
				this->listLevelsSize = _lfi.listLevelsSize;

				this->bytes = new unsigned char[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memcpy( this->bytes, _lfi.bytes, this->sizeInBytes );
				}  
			}

			return *this;
		}

		virtual ~ListFormattingInformation()
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

		unsigned int ListFormattingInformationSize() const
		{
			return this->listFormattingInformationSize;
		}

		unsigned int ListLevelsSize() const
		{
			return this->listLevelsSize;
		}
	};
}
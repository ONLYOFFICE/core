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

#include "LSTF.h"

namespace ASCDocFileFormat
{
	class PlfLst : public IOperand
	{
	public:
		PlfLst() : bytes(NULL), sizeInBytes(2)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );
		}

		explicit PlfLst( const vector<LSTF>& _rgLstf ) :  bytes(NULL), sizeInBytes(sizeof(short))
		{
			if ( !_rgLstf.empty() )
			{
				this->sizeInBytes += ( _rgLstf.size() * _rgLstf[0].Size() );
			}

			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );

			FormatUtils::SetBytes( this->bytes, (short)_rgLstf.size() );

			for ( unsigned int i = 0; i < _rgLstf.size(); i++ )
			{
				memcpy( ( this->bytes + sizeof(short) + ( i * _rgLstf[i].Size() ) ), (unsigned char*)_rgLstf[i], _rgLstf[i].Size() );
			}
		}

		PlfLst( const PlfLst& _plfLst ) : bytes(NULL), sizeInBytes(_plfLst.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			memset( this->bytes, 0, this->sizeInBytes );

			memcpy( this->bytes, _plfLst.bytes, this->sizeInBytes );
		}

		virtual ~PlfLst()
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
	private:

		unsigned char* bytes;
		unsigned int sizeInBytes;
	};
}
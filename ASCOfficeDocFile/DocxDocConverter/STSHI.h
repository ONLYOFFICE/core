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

#include "Stshif.h"
#include "StshiLsd.h"
//!!!A structure that has no effect and MUST be ignored.!!!
#include "STSHIB.h"

namespace ASCDocFileFormat
{
	class STSHI : public IOperand
	{
	public:
		STSHI () : bytes(NULL), sizeInBytes(0)
		{

		}

		STSHI (const Stshif& _stshif, short _ftcBi, const StshiLsd& _stshiLsd, const STSHIB& _stshiB /*!!!MUST be ignored!!!*/ ) : bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes = ( _stshif.Size() + sizeof(_ftcBi) + _stshiLsd.Size() + _stshiB.Size() );

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, (unsigned char*)_stshif, _stshif.Size() );
				FormatUtils::SetBytes( ( this->bytes + _stshif.Size() ), _ftcBi );
				memcpy( ( this->bytes + _stshif.Size() + sizeof(_ftcBi) ), (unsigned char*)_stshiLsd, _stshiLsd.Size() );
				memcpy( ( this->bytes + _stshif.Size() + sizeof(_ftcBi) + _stshiLsd.Size() ), (unsigned char*)_stshiB, _stshiB.Size() );
			}
		}

		STSHI (const STSHI& oSTSHI) : bytes(NULL), sizeInBytes(oSTSHI.sizeInBytes)
		{
			bytes = new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				memcpy ( bytes, oSTSHI.bytes, sizeInBytes );
			}
		}

		virtual ~STSHI()
		{
			RELEASEARRAYOBJECTS(bytes);
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

	private:

		unsigned char*			bytes;
		unsigned int	sizeInBytes;
	};
}
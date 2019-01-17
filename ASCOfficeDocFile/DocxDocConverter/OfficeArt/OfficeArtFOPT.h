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

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"
#include "OfficeArtRGFOPTE.h"

namespace OfficeArt
{
	class OfficeArtFOPT: public IOfficeArtRecord
	{
	public:
		OfficeArtFOPT() : rh( 0x3, 0, 0xF00B, 0 ), fopt(), bytes(NULL), size(0)
		{
			size = sizeof(rh);

			if (size)
			{
				bytes = new unsigned char[size];

				if (bytes)
				{
					memset( bytes, 0, size );
					memcpy( bytes, (unsigned char*)rh, sizeof(rh) );
				}
			}
		}

		explicit OfficeArtFOPT (const OfficeArtRGFOPTE& _fopt ) : rh( 0x3, _fopt.Count(), 0xF00B, _fopt.Size() ), fopt(_fopt), bytes(NULL), size(0)
		{
			this->size = ( sizeof(this->rh) + this->fopt.Size() );

			if ( this->size != 0 )
			{
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					memcpy( this->bytes, (unsigned char*)this->rh, sizeof(this->rh) );
					memcpy( ( this->bytes + sizeof(this->rh) ), (unsigned char*)this->fopt, this->fopt.Size() );
				}
			}
		}

		OfficeArtFOPT (const OfficeArtFOPT& _officeArtFOPT) : rh(_officeArtFOPT.rh), fopt(_officeArtFOPT.fopt), bytes(NULL), size(_officeArtFOPT.size)
		{
			if (size)
			{
				bytes = new unsigned char[size];

				if ( this->bytes != NULL )
				{
					memset( bytes, 0, size );
					memcpy( bytes, _officeArtFOPT.bytes, size );
				}
			}
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(bytes);
		}

		virtual operator unsigned char* () const
		{
			return bytes;
		}

		virtual unsigned int Size() const
		{
			return size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtFOPT ();
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFOPT( *this );
		}

		virtual ~OfficeArtFOPT()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	protected:

		OfficeArtRecordHeader		rh;
		OfficeArtRGFOPTE			fopt;

		unsigned char*						bytes;
		unsigned int				size;
	};
	class OfficeArtSecondaryFOPT: public IOfficeArtRecord
	{
	public:
		OfficeArtSecondaryFOPT() : rh( 0x3, 0, 0xF122, 0 ), fopt(), bytes(NULL), size(0)
		{
			size = sizeof(rh);

			if (size)
			{
				bytes = new unsigned char[size];

				if (bytes)
				{
					memset( bytes, 0, size );
					memcpy( bytes, (unsigned char*)rh, sizeof(rh) );
				}
			}
		}

		explicit OfficeArtSecondaryFOPT (const OfficeArtRGFOPTE& _fopt ) : rh( 0x3, _fopt.Count(), 0xF122, _fopt.Size() ), fopt(_fopt), bytes(NULL), size(0)
		{
			this->size = ( sizeof(this->rh) + this->fopt.Size() );

			if ( this->size != 0 )
			{
				this->bytes = new unsigned char[this->size];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->size );

					memcpy( this->bytes, (unsigned char*)this->rh, sizeof(this->rh) );
					memcpy( ( this->bytes + sizeof(this->rh) ), (unsigned char*)this->fopt, this->fopt.Size() );
				}
			}
		}

		OfficeArtSecondaryFOPT (const OfficeArtSecondaryFOPT& _officeArtFOPT) : rh(_officeArtFOPT.rh), fopt(_officeArtFOPT.fopt), bytes(NULL), size(_officeArtFOPT.size)
		{
			if (size)
			{
				bytes = new unsigned char[size];

				if ( this->bytes != NULL )
				{
					memset( bytes, 0, size );
					memcpy( bytes, _officeArtFOPT.bytes, size );
				}
			}
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(bytes);
		}

		virtual operator unsigned char* () const
		{
			return bytes;
		}

		virtual unsigned int Size() const
		{
			return size;
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtSecondaryFOPT ();
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSecondaryFOPT( *this );
		}

		virtual ~OfficeArtSecondaryFOPT()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	protected:

		OfficeArtRecordHeader		rh;
		OfficeArtRGFOPTE			fopt;

		unsigned char*						bytes;
		unsigned int				size;
	};
}
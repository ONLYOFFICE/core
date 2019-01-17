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

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtFDG : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 16;
	public:
		OfficeArtFDG(): rh(0x0, 0xFFE, 0xF008, 0x00000008), spidCur(0), csp(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtFDG(MSOSPID _spid, unsigned int _csp) : rh(0x0, 0xFFE, 0xF008, 0x00000008), spidCur(_spid), csp(_csp)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (unsigned char*)(rh), sizeof(rh) );
			offset += sizeof(rh);
			
			memcpy( ( bytes + offset ), &(csp), sizeof(csp) );
			offset += sizeof(csp);

			memcpy( ( bytes + offset ), &(spidCur), sizeof(spidCur) );
			offset += sizeof(spidCur);
		}

		OfficeArtFDG(const OfficeArtFDG& _fdg ): rh( _fdg.rh ), spidCur(_fdg.spidCur), csp(_fdg.csp)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, _fdg.bytes, SIZE_IN_BYTES);
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(&bytes);
		}

		virtual operator unsigned char* () const
		{
			return (unsigned char*)(&bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(bytes);
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtFDG;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFDG(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;
		MSOSPID					spidCur;
		unsigned int			csp;

		unsigned char					bytes[SIZE_IN_BYTES];
	};
}
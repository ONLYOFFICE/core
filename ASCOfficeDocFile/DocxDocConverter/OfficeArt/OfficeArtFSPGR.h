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
	class OfficeArtFSPGR : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 24;
	public:

		OfficeArtFSPGR() : rh (0x1, 0x000, 0xF009, 0x00000010), xLeft(0), yTop(0), xRight(0), yBottom(0)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
		}

		OfficeArtFSPGR (int Left, int Top, int Right, int Bottom) : rh (0x1, 0x000, 0xF009, 0x00000010), xLeft(Left), yTop(Top), xRight(Right), yBottom(Bottom) 
		{
			memset (bytes, 0, SIZE_IN_BYTES);

			unsigned int offset = 0;

			memcpy ((bytes + offset), (unsigned char*)(rh), sizeof(rh));
			offset += sizeof(rh);

			memcpy ((bytes + offset), &xLeft, sizeof(xLeft));
			offset += sizeof(xLeft);

			memcpy ((bytes + offset), &yTop, sizeof(yTop));
			offset += sizeof(yTop);

			memcpy ((bytes + offset), &xRight, sizeof(xRight));
			offset += sizeof(xRight);

			memcpy ((bytes + offset), &yBottom, sizeof(yBottom));
			offset += sizeof(yBottom);
		}

		OfficeArtFSPGR (const OfficeArtFSPGR& _office): rh(_office.rh), xLeft(_office.xLeft), yTop(_office.yTop), xRight(_office.xRight), yBottom(_office.yBottom)
		{
			memset (bytes, 0, SIZE_IN_BYTES);
			memcpy (bytes, _office.bytes, SIZE_IN_BYTES);
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(&this->bytes);
		}

		virtual operator unsigned char* () const
		{
			return (unsigned char*)(&this->bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtFSPGR;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFSPGR (*this);
		}

	protected:

		OfficeArtRecordHeader rh;

		int xLeft;
		int yTop;
		int xRight;
		int yBottom;

		unsigned char bytes[SIZE_IN_BYTES];
	};

	typedef OfficeArtFSPGR FSPGR;
}
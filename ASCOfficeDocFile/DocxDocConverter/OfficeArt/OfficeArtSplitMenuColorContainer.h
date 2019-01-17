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
	class OfficeArtSplitMenuColorContainer : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 24;
	public:
		
		OfficeArtSplitMenuColorContainer () : rh(0x0, 0x004, 0xF11E, 0x00000010), fillColor(0), lineColor(0), shadowColor(0), color3D(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtSplitMenuColorContainer (unsigned int fill, unsigned int line, unsigned int shadow, unsigned int n3D) : rh(0x0, 0x004, 0xF11E, 0x00000010), fillColor(fill), lineColor(line), shadowColor(shadow), color3D(n3D)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (unsigned char*)(rh), sizeof(rh) );
			offset += sizeof(rh);

			memcpy( ( bytes + offset ), &(fillColor), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
			memcpy( ( bytes + offset ), &(lineColor), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
			memcpy( ( bytes + offset ), &(shadowColor), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
			memcpy( ( bytes + offset ), &(color3D), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
		}

		OfficeArtSplitMenuColorContainer(const OfficeArtSplitMenuColorContainer& color): rh(color.rh), fillColor(color.fillColor), lineColor(color.lineColor), shadowColor(color.shadowColor), color3D(color.color3D)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, color.bytes, SIZE_IN_BYTES);
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
			return new OfficeArtSplitMenuColorContainer();
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSplitMenuColorContainer(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;
		
		unsigned int			fillColor;		// fill color
		unsigned int			lineColor;		// line color
		unsigned int			shadowColor;	// shadow color
		unsigned int			color3D;		// 3D color

		unsigned char					bytes[SIZE_IN_BYTES];
	};
}
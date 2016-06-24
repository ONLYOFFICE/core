/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Record.h"

namespace DocFileFormat
{
	class BitmapBlip: public Record
	{
	public:  
		static const unsigned short TYPE_CODE_0xF01D = 0xF01D;
		static const unsigned short TYPE_CODE_0xF01E = 0xF01E;
		static const unsigned short TYPE_CODE_0xF01F = 0xF01F;
		static const unsigned short TYPE_CODE_0xF020 = 0xF020;
		static const unsigned short TYPE_CODE_0xF021 = 0xF021;
		static const unsigned short TYPE_CODE_0xF02A = 0xF02A;
		static const unsigned short TYPE_CODE_0xF029 = 0xF029;

	public:
		BitmapBlip() : Record(), m_rgbUid(NULL), m_rgbUidPrimary(NULL), m_bTag(0), m_pvBits(NULL)
		{
		}

		BitmapBlip(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : 
			Record(_reader, size, typeCode, version, instance ), m_rgbUid(NULL), m_rgbUidPrimary(NULL), m_bTag(0), m_pvBits(NULL), pvBitsSize(0)
		{
			m_rgbUid = Reader->ReadBytes(16, true);

			if ((instance == 0x46B) || (instance == 0x6E3) || (instance == 0x6E1) || (instance == 0x7A9) || (instance == 0x6E5))
			{
				m_rgbUidPrimary = Reader->ReadBytes(16, true);
			}

			m_bTag = Reader->ReadByte();

			pvBitsSize = (size - 17);

			if (m_rgbUidPrimary)
			{
				pvBitsSize -= 16;  
			}

			m_pvBits = Reader->ReadBytes((int)(pvBitsSize), true);
		}

		virtual ~BitmapBlip()
		{
			RELEASEARRAYOBJECTS(m_rgbUid);
			RELEASEARRAYOBJECTS(m_rgbUidPrimary);
			RELEASEARRAYOBJECTS(m_pvBits);
		}

		virtual Record* NewObject(IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
		{
			return new BitmapBlip(_reader, bodySize, typeCode, version, instance);
		}

	public:

		/// The secondary, or data, UID - should always be set.
		unsigned char*			m_rgbUid;
		/// The primary UID - this defaults to 0, in which case the primary ID is that of the internal data.
		/// NOTE!: The primary UID is only saved to disk if (blip_instance ^ blip_signature == 1).
		/// Blip_instance is MSOFBH.finst and blip_signature is one of the values defined in MSOBI
		unsigned char*			m_rgbUidPrimary;
		unsigned char			m_bTag;
		/// Raster bits of the blip
		unsigned char*			m_pvBits;
		unsigned int	pvBitsSize;
	};
}
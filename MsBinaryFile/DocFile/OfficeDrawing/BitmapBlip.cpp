/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "BitmapBlip.h"

namespace DocFileFormat
{	
	BitmapBlip::BitmapBlip() : Record(), m_rgbUid(NULL), m_rgbUidPrimary(NULL), m_bTag(0), m_pvBits(NULL)
	{
	}
	BitmapBlip::BitmapBlip(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) :
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

	BitmapBlip::~BitmapBlip()
	{
		RELEASEARRAYOBJECTS(m_rgbUid);
		RELEASEARRAYOBJECTS(m_rgbUidPrimary);
		RELEASEARRAYOBJECTS(m_pvBits);
	}
	Record* BitmapBlip::NewObject(IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
	{
		return new BitmapBlip(_reader, bodySize, typeCode, version, instance);
	}
}

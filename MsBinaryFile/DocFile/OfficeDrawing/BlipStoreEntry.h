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
#pragma once

#include "RecordFactory.h"
#include "../Global.h"

namespace DocFileFormat
{
	class BlipStoreEntry: public Record		//	OfficeArtFBSE
	{
		friend class VMLShapeMapping;

	public:
		static const unsigned short TYPE_CODE_0xF007 = 0xF007;

		/// Required type on Win32 
		Global::BlipType btWin32;
		/// Required type on Mac 
		Global::BlipType btMacOS;
		Record* Blip;

	private:     
		/// Identifier of blip
		unsigned char* rgbUid;
		/// currently unused
		short tag;
		/// Blip size in stream
		unsigned int size;
		/// Reference count on the blip
		unsigned int cRef;
		unsigned int foDelay; // File offset in the delay stream 
		/// How this blip is used (MSOBLIPUSAGE)
		Global::BlipUsage usage;
		/// length of the blip name 
		unsigned char cbName;
		/// for the future 
		unsigned char unused2;
		/// for the future
		unsigned char unused3;
		/// The primary UID - this defaults to 0, in which case the primary ID is that of the internal data.
		/// NOTE!: The primary UID is only saved to disk if (blip_instance ^ blip_signature == 1).
		/// Blip_instance is MSOFBH.inst and blip_signature is one of the values defined in MSOBI
		unsigned char* m_rgbUid;
		/// optional based on the above check.
		unsigned char* m_rgbUidPrimary;
		unsigned char m_bTag;
		/// Cache of the metafile size 
		short m_cb;
		//RECT m_rcBounds; // Boundary of metafile drawing commands 
		//POINT m_ptSize; // Size of metafile in EMUs 
		/// Cache of saved size (size of m_pvBits) 
		short m_cbSave;
		/// Compression
		unsigned char m_fCompression;
		/// always msofilterNone
		unsigned char m_fFilter;

	public:
		BlipStoreEntry();
		BlipStoreEntry(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance );

		virtual ~BlipStoreEntry();
		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance );
	};
}

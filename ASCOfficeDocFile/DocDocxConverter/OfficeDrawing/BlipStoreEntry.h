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

		BlipStoreEntry() : Record(), btWin32(Global::msoblipERROR), btMacOS(Global::msoblipERROR), rgbUid(NULL), tag(0), size(0), cRef(0),
			foDelay(0), usage(Global::msoblipUsageDefault), cbName(0), unused2(0), unused3(0), m_rgbUid(NULL), m_rgbUidPrimary(NULL),
			m_bTag(0), m_cb(0), m_cbSave(0), m_fCompression(0), m_fFilter(0), Blip(NULL)
		{
		}

		BlipStoreEntry(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
		Record( _reader, size, typeCode, version, instance ), btWin32(Global::msoblipERROR), btMacOS(Global::msoblipERROR), rgbUid(NULL), tag(0), size(0), cRef(0),
			foDelay(0), usage(Global::msoblipUsageDefault), cbName(0), unused2(0), unused3(0), m_rgbUid(NULL), m_rgbUidPrimary(NULL),
			m_bTag(0), m_cb(0), m_cbSave(0), m_fCompression(0), m_fFilter(0), Blip(NULL)
		{
			btWin32		=	(Global::BlipType)Reader->ReadByte();
			btMacOS		=	(Global::BlipType)Reader->ReadByte();
			rgbUid		=	Reader->ReadBytes(16, true);
			tag			=	Reader->ReadInt16();
			size		=	Reader->ReadUInt32();
			cRef		=	Reader->ReadUInt32();
			foDelay		=	Reader->ReadUInt32();
			usage		=	(Global::BlipUsage) Reader->ReadByte();
			cbName		=	Reader->ReadByte();
			unused2		=	Reader->ReadByte();
			unused3		=	Reader->ReadByte();

			if (btMacOS != btWin32)
			{
				btWin32 = btMacOS = (Global::BlipType)this->Instance;
			}

			if (BodySize > 0x24)
			{
				Blip	=	RecordFactory::ReadRecord(Reader, 0);
			}
		}

		virtual ~BlipStoreEntry()
		{
			RELEASEARRAYOBJECTS(rgbUid);
			RELEASEARRAYOBJECTS(m_rgbUid);
			RELEASEARRAYOBJECTS(m_rgbUidPrimary);
			RELEASEOBJECT(Blip);
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new BlipStoreEntry( _reader, bodySize, typeCode, version, instance );
		}
	};
}

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

#include "BlipStoreEntry.h"

namespace DocFileFormat
{	
	BlipStoreEntry::BlipStoreEntry() : Record(), btWin32(Global::msoblipERROR), btMacOS(Global::msoblipERROR), rgbUid(NULL), tag(0), size(0), cRef(0),
		foDelay(0), usage(Global::msoblipUsageDefault), cbName(0), unused2(0), unused3(0), m_rgbUid(NULL), m_rgbUidPrimary(NULL),
		m_bTag(0), m_cb(0), m_cbSave(0), m_fCompression(0), m_fFilter(0), Blip(NULL)
	{
	}

	BlipStoreEntry::BlipStoreEntry(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
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
	BlipStoreEntry::~BlipStoreEntry()
	{
		RELEASEARRAYOBJECTS(rgbUid);
		RELEASEARRAYOBJECTS(m_rgbUid);
		RELEASEARRAYOBJECTS(m_rgbUidPrimary);
		RELEASEOBJECT(Blip);
	}
	Record* BlipStoreEntry::NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	{
		return new BlipStoreEntry( _reader, bodySize, typeCode, version, instance );
	}
}

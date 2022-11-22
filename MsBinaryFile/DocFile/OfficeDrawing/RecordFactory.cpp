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
#include "RecordFactory.h"

#include "Shape.h"
#include "ShapeOptions.h"
#include "ShapeContainer.h"
#include "ClientAnchor.h"
#include "ChildAnchor.h"
#include "BlipStoreEntry.h"
#include "BitmapBlip.h"
#include "MetafilePictBlip.h"
#include "DrawingGroupRecord.h"
#include "BlipStoreContainer.h"
#include "SplitMenuColorContainer.h"
#include "ClientData.h"
#include "DrawingContainer.h"
#include "DrawingRecord.h"
#include "DrawingGroup.h"
#include "GroupContainer.h"
#include "GroupShapeRecord.h"
#include "OfficeArtClientTextbox.h"
#include "UnknownRecord.h"
#include "OfficeArtFRITContainer.h"

namespace DocFileFormat
{
	Record* RecordFactory::ReadRecord (IBinaryReader* reader, unsigned int siblingIdx)
	{
		Record* result = NULL;

		try
		{
			unsigned short verAndInstance	=	reader->ReadUInt16();
			unsigned int version			=	verAndInstance & 0x000FU;			//	first 4 bit of field verAndInstance
			unsigned int instance			=	( verAndInstance & 0xFFF0U ) >> 4;	//	last 12 bit of field verAndInstance

			unsigned short typeCode			=	reader->ReadUInt16();
			unsigned int size				=	reader->ReadUInt32();

			bool isContainer				=	( version == 0xF );

			if (typeCode == 0x01f6) return NULL;	//орг диаграмм

			Record* pRecord					=	NewRecord (typeCode);
			if (pRecord)
			{
				result = pRecord->NewObject (reader, size, typeCode, version, instance);
				if (result)
					result->SiblingIdx		=	siblingIdx;
				RELEASEOBJECT(pRecord);
			}
		}
		catch ( ... )
		{
			return NULL;
		}

		return result;
	}

	Record* RecordFactory::NewRecord (unsigned short typeCode)
	{
		switch (typeCode)
		{
		case ShapeContainer::TYPE_CODE_0xF004:
			return new ShapeContainer();

		case Shape::TYPE_CODE_0xF00A:
			return new Shape();

		case ShapeOptions::TYPE_CODE_0xF00B:
		case ShapeOptions::TYPE_CODE_0xF121:
		case ShapeOptions::TYPE_CODE_0xF122:
			return new ShapeOptions();

		case ClientAnchor::TYPE_CODE_0xF010:
			return new ClientAnchor();

		case ChildAnchor::TYPE_CODE_0xF00F:
			return new ChildAnchor();

		case BlipStoreEntry::TYPE_CODE_0xF007:
			return new BlipStoreEntry();

		case BitmapBlip::TYPE_CODE_0xF01D:				//OfficeArtBlipJPEG
		case BitmapBlip::TYPE_CODE_0xF01E:				//OfficeArtBlipPNG
		case BitmapBlip::TYPE_CODE_0xF01F:				//OfficeArtBlipDIB
		case BitmapBlip::TYPE_CODE_0xF020:				//??
		case BitmapBlip::TYPE_CODE_0xF021:				//??
		case BitmapBlip::TYPE_CODE_0xF029:				//OfficeArtBlipTIFF
		case BitmapBlip::TYPE_CODE_0xF02A:				//OfficeArtBlipJPEG
			return new BitmapBlip();

		case MetafilePictBlip::TYPE_CODE_0xF01A:			//OfficeArtBlipEMF
		case MetafilePictBlip::TYPE_CODE_0xF01B:			//OfficeArtBlipWMF
		case MetafilePictBlip::TYPE_CODE_0xF01C			:	//OfficeArtBlipPICT
															return new MetafilePictBlip();

		case DrawingGroup::TYPE_CODE_0xF000				:	return new DrawingGroup();
		case DrawingGroupRecord::TYPE_CODE_0xF006		:	return new DrawingGroupRecord();
		case BlipStoreContainer::TYPE_CODE_0xF001		:	return new BlipStoreContainer();
		case SplitMenuColorContainer::TYPE_CODE_0xF11E	:	return new SplitMenuColorContainer();
		case ClientData::TYPE_CODE_0xF011				:	return new ClientData();
		case DrawingContainer::TYPE_CODE_0xF002			:	return new DrawingContainer();
		case DrawingRecord::TYPE_CODE_0xF008			:	return new DrawingRecord();		
		case GroupContainer::TYPE_CODE_0xF003			:	return new GroupContainer();
		case GroupShapeRecord::TYPE_CODE_0xF009			:	return new GroupShapeRecord ();
		case OfficeArtClientTextbox::TYPE_CODE_0xF00D	:	return new OfficeArtClientTextbox ();
		case OfficeArtFRITContainer::TYPE_CODE_0xF118	:	return new OfficeArtFRITContainer ();

		default:	
			return new UnknownRecord();		
		}
	}
}

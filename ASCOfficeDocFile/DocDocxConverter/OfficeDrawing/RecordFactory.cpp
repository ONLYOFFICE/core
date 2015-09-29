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

			Record* pRecord					=	NewRecord (typeCode);
			if (pRecord)
			{
				result = pRecord->NewObject (reader, size, typeCode, version, instance);
				result->SiblingIdx			=	siblingIdx;
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
			break;

		case Shape::TYPE_CODE_0xF00A:
			return new Shape();
			break;

		case ShapeOptions::TYPE_CODE_0xF00B:
		case ShapeOptions::TYPE_CODE_0xF121:
		case ShapeOptions::TYPE_CODE_0xF122:
			return new ShapeOptions();
			break;

		case ClientAnchor::TYPE_CODE_0xF010:
			return new ClientAnchor();
			break;

		case ChildAnchor::TYPE_CODE_0xF00F:
			return new ChildAnchor();
			break;

		case BlipStoreEntry::TYPE_CODE_0xF007:
			return new BlipStoreEntry();
			break;

		case BitmapBlip::TYPE_CODE_0xF01D:				//OfficeArtBlipJPEG
		case BitmapBlip::TYPE_CODE_0xF01E:				//OfficeArtBlipPNG
		case BitmapBlip::TYPE_CODE_0xF01F:				//OfficeArtBlipDIB
		case BitmapBlip::TYPE_CODE_0xF020:				//??
		case BitmapBlip::TYPE_CODE_0xF021:				//??
		case BitmapBlip::TYPE_CODE_0xF029:				//OfficeArtBlipTIFF
		case BitmapBlip::TYPE_CODE_0xF02A:				//OfficeArtBlipJPEG
			return new BitmapBlip();
			break;

		case MetafilePictBlip::TYPE_CODE_0xF01A:			//OfficeArtBlipEMF
		case MetafilePictBlip::TYPE_CODE_0xF01B:			//OfficeArtBlipWMF
		case MetafilePictBlip::TYPE_CODE_0xF01C			:	//OfficeArtBlipPICT
															return new MetafilePictBlip();				break;

		case DrawingGroup::TYPE_CODE_0xF000				:	return new DrawingGroup();					break;
		case DrawingGroupRecord::TYPE_CODE_0xF006		:	return new DrawingGroupRecord();			break;
		case BlipStoreContainer::TYPE_CODE_0xF001		:	return new BlipStoreContainer();			break;
		case SplitMenuColorContainer::TYPE_CODE_0xF11E	:	return new SplitMenuColorContainer();		break;
		case ClientData::TYPE_CODE_0xF011				:	return new ClientData();					break;
		case DrawingContainer::TYPE_CODE_0xF002			:	return new DrawingContainer();				break;
		case DrawingRecord::TYPE_CODE_0xF008			:	return new DrawingRecord();					break;
		case GroupContainer::TYPE_CODE_0xF003			:	return new GroupContainer();				break;
		case GroupShapeRecord::TYPE_CODE_0xF009			:	return new GroupShapeRecord ();				break;
		case OfficeArtClientTextbox::TYPE_CODE_0xF00D	:	return new OfficeArtClientTextbox ();		break;

		default:	return new UnknownRecord();		break;
		}
	}
}

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

#include "Shape.h"

namespace DocFileFormat
{
	Shape::Shape():
	Record(), spid(0), fGroup(false), fChild(false), fPatriarch(false), fDeleted(false), fOleShape(false),
		fHaveMaster(false), fFlipH(false), fFlipV(false), fConnector(false), fHaveAnchor(false), fBackground(false),
		fHaveSpt(false), shapeType(NULL)
	{
	}
	Shape::~Shape()
	{
		RELEASEOBJECT( shapeType );
	}
	Shape::Shape( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
	Record( _reader, size, typeCode, version, instance ), spid(0), fGroup(false), fChild(false), fPatriarch(false), fDeleted(false), fOleShape(false),
		fHaveMaster(false), fFlipH(false), fFlipV(false), fConnector(false), fHaveAnchor(false), fBackground(false),
		fHaveSpt(false), shapeType(NULL)
	{
		spid				=	Reader->ReadInt32();

		unsigned int flag	=	Reader->ReadUInt32();

		fGroup				=	FormatUtils::BitmaskToBool( flag, 0x1 );
		fChild				=	FormatUtils::BitmaskToBool( flag, 0x2 );
		fPatriarch			=	FormatUtils::BitmaskToBool( flag, 0x4 );
		fDeleted			=	FormatUtils::BitmaskToBool( flag, 0x8 );
		fOleShape			=	FormatUtils::BitmaskToBool( flag, 0x10 );
		fHaveMaster			=	FormatUtils::BitmaskToBool( flag, 0x20 );
		fFlipH				=	FormatUtils::BitmaskToBool( flag, 0x40 );
		fFlipV				=	FormatUtils::BitmaskToBool( flag, 0x80 );
		fConnector			=	FormatUtils::BitmaskToBool( flag, 0x100 );
		fHaveAnchor			=	FormatUtils::BitmaskToBool( flag, 0x200 );
		fBackground			=	FormatUtils::BitmaskToBool( flag, 0x400 );
		fHaveSpt			=	FormatUtils::BitmaskToBool( flag, 0x800 );

		if (Instance > 0)
			shapeType			=	ShapeTypeFactory::NewShapeType((MSOSPT)Instance);
		else if (!fHaveSpt)
		{
			shapeType			=	ShapeTypeFactory::NewShapeType(msosptNotPrimitive);
		}
	}

	Record* Shape::NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	{
		return new Shape( _reader, bodySize, typeCode, version, instance );
	}	
}

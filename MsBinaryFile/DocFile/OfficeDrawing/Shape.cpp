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

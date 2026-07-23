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

#include "Record.h"
#include "ShapeTypeFactory.h"

namespace DocFileFormat
{
	class Shape: public Record
	{
	public: 
		int		spid;
		bool	fGroup;
		bool	fChild;
		bool	fPatriarch;			// This is the topmost group shape. Exactly one of these per drawing.
		bool	fDeleted;			// The shape has been deleted 
		bool	fOleShape;			// The shape is an OLE object
		bool	fHaveMaster;
		bool	fFlipH;
		bool	fFlipV;
		bool	fConnector;
		bool	fHaveAnchor;
		bool	fBackground;
		bool	fHaveSpt;

		ShapeType* shapeType;

	public:
		static const unsigned short TYPE_CODE_0xF00A = 0xF00A;

		Shape();
		virtual ~Shape();
		Shape( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance );

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance );

		inline int GetShapeID() const
		{
			return spid;
		}

		inline ShapeType* GetShapeType() const
		{
			return shapeType;
		}

		template<class T> bool is() const
		{
			bool isResult = false;

			if ( shapeType != NULL )
			{
				isResult = ( typeid(*shapeType) == typeid(T) );
			}

			return isResult;
		}
	};
}

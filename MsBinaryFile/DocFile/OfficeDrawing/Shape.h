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

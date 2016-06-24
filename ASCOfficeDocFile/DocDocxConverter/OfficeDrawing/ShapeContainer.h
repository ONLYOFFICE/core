/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "RegularContainer.h"
#include "ShapeOptions.h"
#include "Shape.h"

namespace DocFileFormat
{
	class ShapeContainer: public RegularContainer
	{
	public:
		static const unsigned short TYPE_CODE_0xF004 = 0xF004;

		int Index;

		ShapeContainer():
		RegularContainer(), Index(0)
		{
		}

		ShapeContainer( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
		RegularContainer( _reader, size, typeCode, version, instance ), Index(0) 
		{ 
		}

		int getShapeType()
		{
			int ret = 0;

			for ( vector<Record*>::const_iterator iter = this->Children.begin(); iter != this->Children.end(); iter++ )
			{
				Shape* sh = dynamic_cast<Shape*>( *iter );
				if (sh)
				{
					if (sh->shapeType) return sh->shapeType->GetTypeCode();
					else return 0;
				}
			}
			return 0;
		}

		virtual ~ShapeContainer()
		{
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new ShapeContainer( _reader, bodySize, typeCode, version, instance );
		}

		/// Searches all OptionEntry in the ShapeContainer and puts them into a list.
		list<OptionEntry> ExtractOptions() const
		{
			list<OptionEntry> ret;

			//build the list of all option entries of this shape
			for ( vector<Record*>::const_iterator iter = this->Children.begin(); iter != this->Children.end(); iter++ )
			{
				ShapeOptions* opt = dynamic_cast<ShapeOptions*>( *iter );

				if ( opt != NULL )
				{
					for ( vector<OptionEntry>::iterator oeIter = opt->Options.begin(); oeIter != opt->Options.end(); oeIter++ )
					{
						ret.push_back( *oeIter );
					}
				}
			}

			return ret;
		}
	};
}
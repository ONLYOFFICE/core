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

#include "ShapeOptions.h"

namespace DocFileFormat
{
	ShapeOptions::ShapeOptions() : Record()
	{
	}
	ShapeOptions::~ShapeOptions()
	{
	}
	ShapeOptions::ShapeOptions (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance)
	{
		long pos = Reader->GetPosition();

		// parse the flags and the simple values
		for (unsigned int i = 0; i < instance; ++i)
		{
			ODRAW::OfficeArtFOPTEPtr fopte = ODRAW::OfficeArtFOPTE::load_and_create(Reader);
			if (!fopte)continue;


			Options.push_back(fopte);
		}
		// complex load
		for(size_t i = 0; i < Options.size();  ++i)
		{
			if(Options[i]->fComplex && Options[i]->op > 0)
			{
				Options[i]->ReadComplexData(Reader);
			}
			OptionsByID.insert(std::make_pair((ODRAW::ePropertyId)Options[i]->opid, Options[i]));
		}

		Reader->Seek(( pos + size ), 0/*STREAM_SEEK_SET*/);
	}
	Record* ShapeOptions::NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	{
		return new ShapeOptions( _reader, bodySize, typeCode, version, instance );
	}
}

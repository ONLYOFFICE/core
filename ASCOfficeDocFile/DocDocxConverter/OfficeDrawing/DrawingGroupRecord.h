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

#include "Record.h"

namespace DocFileFormat
{
	class DrawingGroupRecord: public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF006 = 0xF006;

		struct FileIdCluster
		{
			FileIdCluster( IBinaryReader* reader )
			{
				DrawingGroupId	=	reader->ReadUInt32();
				CSpIdCur		=	reader->ReadUInt32();
			}
			unsigned int DrawingGroupId;
			unsigned int CSpIdCur;
		};

		unsigned int MaxShapeId;			//	Maximum shape ID
		unsigned int IdClustersCount;		//	Number of FileIdClusters
		unsigned int ShapesSavedCount;		//	Total number of shapes saved
		unsigned int DrawingsSavedCount;	//	Total number of drawings saved

		list<FileIdCluster> Clusters;

		DrawingGroupRecord () : Record(), MaxShapeId(0), IdClustersCount(0), ShapesSavedCount(0), DrawingsSavedCount(0)
		{

		}

		DrawingGroupRecord (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record( _reader, size, typeCode, version, instance )
		{
			MaxShapeId			=	Reader->ReadUInt32();
			IdClustersCount		=	Reader->ReadUInt32() - 1; // Office saves the actual value + 1 -- flgr
			ShapesSavedCount	=	Reader->ReadUInt32();
			DrawingsSavedCount	=	Reader->ReadUInt32();

			for (unsigned int i = 0; i < IdClustersCount; ++i)
				Clusters.push_back(FileIdCluster(Reader));
		}

		virtual ~DrawingGroupRecord()
		{
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new DrawingGroupRecord( _reader, bodySize, typeCode, version, instance );
		}
	};
}
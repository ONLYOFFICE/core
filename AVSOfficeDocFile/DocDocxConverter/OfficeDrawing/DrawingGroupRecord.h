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
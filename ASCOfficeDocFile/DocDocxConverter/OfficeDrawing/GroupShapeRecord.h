#pragma once

#include "Record.h"

namespace DocFileFormat
{
	class GroupShapeRecord: public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF009 = 0xF009;

		GroupShapeRecord () : Record(), rcgBounds()
		{
		}

		GroupShapeRecord(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record(_reader, size, typeCode, version, instance), rcgBounds()
		{
			int left	=	Reader->ReadInt32();
			int top		=	Reader->ReadInt32();
			int right	=	Reader->ReadInt32();
			int bottom	=	Reader->ReadInt32();

			//left = max( 0, left );
			//top = max( 0, top );
			//right = max( 0, right );
			//bottom = max( 0, bottom );

			POINT oPoint;
			oPoint.x	=	left;
			oPoint.y	=	top;

			SIZE oSize;
			oSize.cx	=	( right - left );
			oSize.cy	=	( bottom - top );

			rcgBounds	=	ASCDocFormatUtils::Rectangle(oPoint,oSize);
		}

		virtual ~GroupShapeRecord()
		{

		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new GroupShapeRecord( _reader, bodySize, typeCode, version, instance );
		}

		ASCDocFormatUtils::Rectangle rcgBounds;
	};
}
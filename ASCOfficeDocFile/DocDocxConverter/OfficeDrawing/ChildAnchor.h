#pragma once

#include "Record.h"

namespace DocFileFormat
{
	class ChildAnchor: public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF00F = 0xF00F;

		ChildAnchor() : Record(), rcgBounds(), Left(0), Top(0), Right(0), Bottom(0)
		{

		}

		ChildAnchor (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance), rcgBounds(), Left(0), Top(0), Right(0), Bottom(0)
		{
			Left		=	Reader->ReadInt32();
			Top			=	Reader->ReadInt32();
			Right		=	Reader->ReadInt32();
			Bottom		=	Reader->ReadInt32();

			POINT _point;
			_point.x	=	Left;
			_point.y	=	Top;

			SIZE _size;
			_size.cx	=	Right - Left;
			_size.cy	=	Bottom - Top;

			rcgBounds	=	AVSDocFormatUtils::Rectangle (_point, _size);
		}

		virtual ~ChildAnchor()
		{
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new ChildAnchor( _reader, bodySize, typeCode, version, instance );
		}

	public:

		/// Rectangle that describes the bounds of the anchor
		AVSDocFormatUtils::Rectangle rcgBounds;
		int Left;
		int Top;
		int Right;
		int Bottom;
	};
}
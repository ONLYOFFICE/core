#pragma once

#include "Record.h"

namespace DocFileFormat
{
	class SplitMenuColorContainer: public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF11E = 0xF11E;

	public:

		SplitMenuColorContainer () : Record(), fillColor(0), lineColor(0), shadowColor(0), color3D(0)
		{

		}

		SplitMenuColorContainer (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance)
		{
			fillColor	=	Reader->ReadUInt32();
			lineColor	=	Reader->ReadUInt32();
			shadowColor =	Reader->ReadUInt32();
			color3D		=	Reader->ReadUInt32();
		}

		virtual ~SplitMenuColorContainer()
		{
		}

		virtual Record* NewObject (IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
		{
			return new SplitMenuColorContainer( _reader, bodySize, typeCode, version, instance );
		}

	public:

		unsigned int fillColor;		// fill color
		unsigned int lineColor;		// line color
		unsigned int shadowColor;	// shadow color
		unsigned int color3D;		// 3D color
	};
}
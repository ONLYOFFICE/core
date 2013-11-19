#pragma once

#include "Record.h"

namespace DocFileFormat
{
	class OfficeArtClientTextbox : public Record
	{
	public:
		static const unsigned short TYPE_CODE_0xF00D = 0xF00D;

		OfficeArtClientTextbox () : Record(), m_nIndex(0)
		{

		}

		OfficeArtClientTextbox (IBinaryReader* reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record(reader, size, typeCode, version, instance), m_nIndex(0)
		{
			unsigned int number	=	Reader->ReadUInt16();
			m_nIndex			=	Reader->ReadUInt16();
		}

		virtual ~OfficeArtClientTextbox()
		{

		}

		virtual Record* NewObject(IBinaryReader* reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance)
		{
			return new OfficeArtClientTextbox(reader, bodySize, typeCode, version, instance);
		}

		inline int GetIndex ()
		{
			return m_nIndex;
		}

	public:

		int	m_nIndex;
	};
}
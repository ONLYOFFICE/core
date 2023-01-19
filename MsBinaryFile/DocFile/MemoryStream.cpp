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

#include "MemoryStream.h"

MemoryStream::MemoryStream (unsigned char* data, unsigned long size, bool bMemCopy) : m_Data(NULL), m_Size(0), m_Position(0), bMemoryCopy(bMemCopy)
{
	if ( ( data != NULL ) && ( size != 0 ) )
	{
		m_Size = size;

		if (bMemoryCopy)
		{
			m_Data	=	new unsigned char[m_Size];
			if (m_Data)
			{
				memcpy (m_Data, data, m_Size);
			}
		}
		else
		{
			m_Data	=	data;
		}
	}
}
MemoryStream::~MemoryStream()
{
	if (bMemoryCopy)
		RELEASEARRAYOBJECTS (m_Data);
}
_UINT64 MemoryStream::ReadUInt64()
{
	_UINT64 rdU64 = 0;

	if (m_Data)
	{
		rdU64 = DocFileFormat::FormatUtils::BytesToUInt64(m_Data, m_Position, m_Size);
		m_Position += 8;
	}

	return rdU64;
}
unsigned short MemoryStream::ReadUInt16()
{
	unsigned short rdUShort = 0;

	if (m_Data)
	{
		rdUShort = DocFileFormat::FormatUtils::BytesToUInt16 (m_Data, m_Position, m_Size);
		m_Position += 2;
	}

	return rdUShort;
}
void MemoryStream::WriteUInt16(unsigned short val)
{
	if (m_Data && (m_Position + sizeof(unsigned short) <= m_Size))
	{
		((unsigned short *)(m_Data + m_Position))[0] = val;
		m_Position	+=	2;
	}
}
short MemoryStream::ReadInt16()
{
	short rdShort = 0;

	if (m_Data)
	{
		rdShort		=	DocFileFormat::FormatUtils::BytesToInt16 (m_Data, m_Position, m_Size);
		m_Position	+=	2;
	}

	return rdShort;
}

int MemoryStream::ReadInt32()
{
	int rdInt = 0;

	if (m_Data)
	{
		rdInt		=	DocFileFormat::FormatUtils::BytesToInt32 (m_Data, m_Position, m_Size);
		m_Position	+=	4;
	}

	return rdInt;
}
void MemoryStream::WriteInt32(_INT32 val)
{
	if (m_Data && (m_Position + sizeof(_INT32) <= m_Size))
	{
		((_INT32 *)(m_Data + m_Position))[0] = val;
		m_Position	+=	4;
	}
}
void MemoryStream::Align(_UINT32 val)
{
	_UINT32 padding = val - (m_Position % val);

	if (padding > 0 && padding < 4)
		m_Position += padding;
}

unsigned int MemoryStream::ReadUInt32()
{
	int rdUInt = 0;

	if (m_Data )
	{
		rdUInt		=	DocFileFormat::FormatUtils::BytesToUInt32 (m_Data, m_Position, m_Size);
		m_Position	+=	4;
	}

	return rdUInt;
}
void MemoryStream::WriteByte(unsigned char val)
{
	if (m_Data && (m_Position + 1 <= m_Size))
	{
		m_Data[m_Position] = val;
		m_Position	+= 1;
	}
}
void MemoryStream::WriteUInt32(_UINT32 val)
{
	if (m_Data && (m_Position + sizeof(_UINT32) <= m_Size))
	{
		((_UINT32 *)(m_Data + m_Position))[0] = val;
		m_Position	+=	sizeof(_UINT32);
	}
}
unsigned char MemoryStream::ReadByte()
{
	unsigned char rdByte = 0;

	if (m_Data)
	{
		rdByte		=	(m_Position < m_Size) ? m_Data[m_Position] : 0;
		m_Position	+=	sizeof(rdByte);
	}

	return rdByte;
}

unsigned char* MemoryStream::ReadBytes (unsigned int count, bool isResultNeeded)
{
	if (!m_Data) return NULL;

	unsigned int size = ( count <= (m_Size - m_Position) ) ? (count) : (m_Size - m_Position);

	unsigned char* pBytes = (isResultNeeded && count > 0) ? new unsigned char[count] : NULL;

	if (pBytes)
	{
		memcpy(pBytes, (m_Data + m_Position), size);
	}
	m_Position += size;

	return pBytes;
}
void MemoryStream::WriteBytes(unsigned char* pData, int size)
{
	if (m_Data && (m_Position + size <= m_Size))
	{
		memcpy(m_Data + m_Position, pData, size);
		m_Position += size;
	}
}

unsigned long MemoryStream::GetPosition() const
{
	return m_Position;
}
unsigned long MemoryStream::GetSize() const
{
	return m_Size;
}
int MemoryStream::Seek (int offset, int origin)
{
	if (origin == 2) offset = m_Position + offset;
	if (origin == 1) offset = m_Size - offset;

	if ( (m_Data != NULL) && (offset >= 0) && ((unsigned int)offset < m_Size) )
		return m_Position = offset;

	return 0;
}

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

#include "IBinaryReader.h"
#include "../Common/FormatUtils.h"

class MemoryStream: public IBinaryReader
{
public:

	MemoryStream (unsigned char* data, unsigned long size, bool bMemCopy = true) : m_Data(NULL), m_Size(0), m_Position(0), bMemoryCopy(bMemCopy)
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

	virtual ~MemoryStream()
	{
		if (bMemoryCopy)
			RELEASEARRAYOBJECTS (m_Data);
	}


	virtual unsigned short ReadUInt16()
	{
		unsigned short rdUShort = 0;

		if (m_Data)
		{
            rdUShort	=	DocFileFormat::FormatUtils::BytesToUInt16 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdUShort);
		}

		return rdUShort;
	}
	void WriteUInt16(unsigned short val)
	{
		if (m_Data)
		{
			((unsigned short *)(m_Data + m_Position))[0] = val;
			m_Position	+=	sizeof(unsigned short);
		}
	}
	virtual short ReadInt16()
	{
		short rdShort = 0;

		if (m_Data)
		{
            rdShort		=	DocFileFormat::FormatUtils::BytesToInt16 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdShort);
		}

		return rdShort;
	}

	virtual int ReadInt32()
	{
		int rdInt = 0;

		if (m_Data)
		{
            rdInt		=	DocFileFormat::FormatUtils::BytesToInt32 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdInt);
		}

		return rdInt;
	}
	void WriteInt32(_INT32 val)
	{
		if (m_Data)
		{
			((_INT32 *)(m_Data + m_Position))[0] = val;
			m_Position	+=	sizeof(_INT32);
		}
	}
	virtual unsigned int ReadUInt32()
	{
		int rdUInt = 0;

		if (m_Data )
		{
            rdUInt		=	DocFileFormat::FormatUtils::BytesToUInt32 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdUInt);
		}

		return rdUInt;
	}
	void WriteByte(unsigned char val)
	{
		if (m_Data)
		{
			m_Data[m_Position] = val;
			m_Position	+= 1;
		}
	}
	void WriteUInt32(_UINT32 val)
	{
		if (m_Data)
		{
			((_UINT32 *)(m_Data + m_Position))[0] = val;
			m_Position	+=	sizeof(_UINT32);
		}
	}
	virtual unsigned char ReadByte()
	{
		unsigned char rdByte = 0;

		if (m_Data)
		{
			rdByte		=	(m_Position < m_Size) ? m_Data[m_Position] : 0;
			m_Position	+=	sizeof(rdByte);
		}

		return rdByte;
	}

	virtual unsigned char* ReadBytes (unsigned int count, bool isResultNeeded)
	{
		unsigned char* pBytes = NULL;

		unsigned int size = 0;

		size = ( count <= (m_Size - m_Position) ) ? (count) : (m_Size - m_Position);

		if ( ( m_Data != NULL ) && ( size != 0 ) )
		{
			pBytes = new unsigned char[size];

			if (pBytes)
			{
                memcpy(pBytes,  (m_Data + m_Position), size);
				m_Position += sizeof(unsigned char)*size;

				//------------------------------------------------------------------------------------------------------

				if ( !isResultNeeded )
				{
					RELEASEARRAYOBJECTS (pBytes);
				}
			}
		}

		return pBytes;
	}
	void WriteBytes(unsigned char* pData, int size)
	{
		if (m_Data)
		{
			memcpy(m_Data + m_Position, pData, size);
			m_Position += size;
		}
	}

	virtual unsigned long GetPosition() const
	{
		return m_Position;
	}

	virtual unsigned long GetSize() const
	{
		return m_Size;
	}

    virtual int Seek (int offset, int origin = 0/*STREAM_SEEK_SET*/)
	{
		if ( (m_Data != NULL) && (offset >= 0) && ((unsigned int)offset < m_Size) )
			return m_Position = offset;

		return 0;
	}

private:

	unsigned char*	m_Data;
	unsigned long	m_Size;
	unsigned long	m_Position;
	bool			bMemoryCopy;
};

#pragma once

#include "IBinaryReader.h"

//#include <ATLFile.h>

class MemoryStream: public IBinaryReader
{
public:

	MemoryStream (byte* data, unsigned long size, bool bMemCopy = true) : m_Data(NULL), m_Size(0), m_Position(0), bMemoryCopy(bMemCopy)
	{
		if ( ( data != NULL ) && ( size != 0 ) )
		{
			m_Size = size;

			if (bMemoryCopy)
			{
				m_Data	=	new byte[m_Size];
				if (m_Data)
				{
					memcpy_s (m_Data, m_Size, data, m_Size);
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
			rdUShort	=	FormatUtils::BytesToUInt16 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdUShort);
		}

		return rdUShort;
	}

	virtual short ReadInt16()
	{
		short rdShort = 0;

		if (m_Data)
		{
			rdShort		=	FormatUtils::BytesToInt16 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdShort);
		}

		return rdShort;
	}

	virtual int ReadInt32()
	{
		int rdInt = 0;

		if (m_Data)
		{
			rdInt		=	FormatUtils::BytesToInt32 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdInt);
		}

		return rdInt;
	}

	virtual unsigned int ReadUInt32()
	{
		int rdUInt = 0;

		if (m_Data )
		{
			rdUInt		=	FormatUtils::BytesToUInt32 (m_Data, m_Position, m_Size);
			m_Position	+=	sizeof(rdUInt);
		}

		return rdUInt;
	}

	virtual byte ReadByte()
	{
		byte rdByte = 0;

		if (m_Data)
		{
			rdByte		=	m_Data[m_Position];
			m_Position	+=	sizeof(rdByte);
		}

		return rdByte;
	}

	virtual byte* ReadBytes (unsigned int count, bool isResultNeeded)
	{
		byte* pBytes = NULL;

		unsigned int size = 0;

		size = ( count <= (m_Size - m_Position) ) ? (count) : (m_Size - m_Position);

		if ( ( m_Data != NULL ) && ( size != 0 ) )
		{
			pBytes = new byte[size];

			if (pBytes)
			{
				memcpy_s (pBytes, size, (m_Data + m_Position), size);
				m_Position += sizeof(byte)*size;

				//------------------------------------------------------------------------------------------------------

				//ATL::CAtlFile oFile;
				//if (SUCCEEDED(oFile.Create(L"d:\\blop.dat",GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS)))
				//{
				//	DWORD nFileBuff	=	0L;
				//	oFile.Write (pBytes, count, &nFileBuff);
				//	oFile.Close();
				//}

				//------------------------------------------------------------------------------------------------------

				if ( !isResultNeeded )
				{
					RELEASEARRAYOBJECTS (pBytes);
				}
			}
		}

		return pBytes;
	}

	virtual unsigned long GetPosition() const
	{
		return m_Position;
	}

	virtual unsigned long GetSize() const
	{
		return m_Size;
	}

	virtual int Seek (int offset, STREAM_SEEK origin = STREAM_SEEK_SET)
	{
		if ( (m_Data != NULL) && (offset > 0) && ((unsigned int)offset < m_Size) )
			return m_Position = offset;

		return 0;
	}

private:

	byte*			m_Data;
	unsigned long	m_Size;
	unsigned long	m_Position;
	bool			bMemoryCopy;
};
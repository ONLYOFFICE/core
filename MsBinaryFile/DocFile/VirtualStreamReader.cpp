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

#include "VirtualStreamReader.h"

VirtualStreamReader::VirtualStreamReader (POLE::Stream* _stream, ULONG _position , int _nWordVersion) :
	nWordVersion(_nWordVersion), stream(_stream), position(_position)
{
	if ( stream == NULL )return;

	stream->seek(position);
}

VirtualStreamReader::~VirtualStreamReader()
{
}

unsigned short VirtualStreamReader::ReadUInt16()
{
	unsigned short rdUShort = 0;

	if (( stream != NULL ) && (position + 2 < stream->size()))
	{
		stream->seek( position );
		stream->read((unsigned char*)(&rdUShort), sizeof( rdUShort ));
	}
	position += sizeof( rdUShort );

	return rdUShort;
}

short VirtualStreamReader::ReadInt16()
{
	short rdShort = 0;

	if (( stream != NULL ) && (position + 2 < stream->size()))
	{
		stream->seek( position );
		stream->read((unsigned char*)(&rdShort), sizeof( rdShort ));

	}
	position += sizeof( rdShort );

	return rdShort;
}

int VirtualStreamReader::ReadInt32()
{
	_INT32 rdInt = 0;

	if (( stream != NULL ) && (position + 4 < stream->size()))
	{
		stream->seek( position );
		stream->read( (unsigned char*) &rdInt, sizeof( rdInt ) );

	}
	position += sizeof( rdInt );

	return rdInt;
}

unsigned int VirtualStreamReader::ReadUInt32()
{
	_UINT32 rdUInt = 0;

	if (( stream != NULL ) && (position + 4 < stream->size()))
	{
		stream->seek( position );
		stream->read((unsigned char*) &rdUInt, sizeof( rdUInt ) );
		
	}
	position += sizeof( rdUInt );

	return rdUInt;
}

unsigned char VirtualStreamReader::ReadByte()
{
	unsigned char rdByte = 0;

	if (( stream != NULL ) && (position + 1 < stream->size()))
	{
		stream->seek( position);
		stream->read( (unsigned char*)&rdByte, sizeof( rdByte ) );

	}
	position += sizeof( rdByte );

	return rdByte;
}

unsigned char* VirtualStreamReader::ReadBytes( unsigned int count, bool isResultNeeded )
{
	unsigned char *rdBytes = NULL;

	if ( count > 0  && isResultNeeded)
	{
		if (position + count > stream->size())
		{
			if (position > stream->size())	count = 0;
			else							count = (unsigned int)(stream->size() - position);
		}
		rdBytes = new unsigned char[count];
	}

	if ( stream != NULL  && rdBytes != NULL )
	{
		stream->seek( position );
		count = (unsigned int)stream->read( rdBytes, count );
	}

	position += count;

	return rdBytes;
}

unsigned long VirtualStreamReader::GetPosition() const
{
	return (unsigned long)this->position;
}

unsigned long VirtualStreamReader::GetSize() const
{
	unsigned long size = 0;

	if ( stream != NULL )
	{
		size = (unsigned long)stream->size();
	}

	return size;
}

int VirtualStreamReader::Seek( int offset, int origin )
{
	if ( stream != NULL )
	{
		if (origin > 1)
		{

		}
		stream->seek( offset );
		position = offset;
		return offset;
	}
	else
	{
		return 0;
	}
}

std::wstring VirtualStreamReader::ReadXst()
{
	if (!stream) return L"";
	std::wstring wstrResult;

	unsigned char* xstz = NULL;
	unsigned char* cch	= NULL;

	if (nWordVersion > 0)
	{
		int cchSize = 1;
		cch = ReadBytes( cchSize, true );

		int xstzSize = DocFileFormat::FormatUtils::BytesToUChar( cch, 0, cchSize ) * 1;
		xstz = ReadBytes(xstzSize, true);

		DocFileFormat::FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &wstrResult, xstz, xstzSize, ENCODING_WINDOWS_1250 );
	}
	else
	{
		int cchSize = 2;
		cch = ReadBytes( cchSize, true );

		int xstzSize = DocFileFormat::FormatUtils::BytesToInt16( cch, 0, cchSize ) * 2;
		xstz = ReadBytes(xstzSize, true);

		DocFileFormat::FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &wstrResult, xstz, xstzSize, ENCODING_UTF16 );
	}

	RELEASEARRAYOBJECTS(xstz);
	RELEASEARRAYOBJECTS(cch);

	return wstrResult;
}

/// Read a length prefixed Unicode string from the given stream.
/// The string must have the following structure:
/// unsigned char 1 - 4: Character count (cch)
/// unsigned char 5 - (cch*2)+4: Unicode characters terminated by \0
std::wstring VirtualStreamReader::ReadLengthPrefixedUnicodeString()
{
	std::wstring result;

	int cch = ReadInt32();

	if ( cch > 0 )
	{
		//dont read the terminating zero
		unsigned char* stringBytes = ReadBytes( ( cch * 2 ), true );

		DocFileFormat::FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &result, stringBytes, ( ( cch * 2 ) - 2 ), ENCODING_UTF16 );

		RELEASEARRAYOBJECTS( stringBytes );
	}

	return result;
}

/// Read a length prefixed ANSI string from the given stream.
/// The string must have the following structure:
/// unsigned char 1-4: Character count (cch)
/// unsigned char 5-cch+4:   ANSI characters terminated by \0
std::wstring VirtualStreamReader::ReadLengthPrefixedAnsiString(unsigned int max_size)
{
	std::wstring result;

	unsigned int cch = ReadUInt32();

	unsigned char* stringBytes = NULL;

	if (cch > max_size)
	{
		//error ... skip to 0
		unsigned int pos_orinal = GetPosition();
		unsigned int pos = 0;
		
		stringBytes = ReadBytes( max_size, true );

		if (stringBytes)
		{
			while(pos < max_size)
			{
				if (stringBytes[pos] == 0)
					break;
				pos++;
			}
		}
		Seek(pos_orinal + pos - 1, 0);
	}else
		if ( cch > 0 )
		{
			//dont read the terminating zero
			stringBytes = ReadBytes( cch, true );

			DocFileFormat::FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &result, stringBytes, ( cch - 1 ), ENCODING_WINDOWS_1250);

		}
	RELEASEARRAYOBJECTS( stringBytes );

	return result;
}

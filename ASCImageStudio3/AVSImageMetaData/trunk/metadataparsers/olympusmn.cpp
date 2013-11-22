/*!	\file olympusmn.cpp
*
*	\remarks	см.	doc\makernote\MakerNoteOlympusTags.html
*					doc\makernote\olympus_mn.html
*/

/*!
\remarks

	Header:
		7 Bytes
		"OLYMP\x00\x01" or "OLYMP\x00\x02"

	Unknown:
		1 Byte
		Unknown

	IFD Data:
		Variable
		Standard TIFF IFD Data using Olympus Tags
		Offsets are relative to the start of the TIFF header at the beginning of the EXIF segment
*/

#include "stdafx.h"
#include "specificmn.h"
#include "..\openexif\src\ExifTags.h"
#include "..\openexif\src\ExifIO.h"
#include "..\openexif\src\ExifIFD.h"


#include <string>
#include <sstream>
#include <iostream>


namespace AVS
{
	namespace Image
	{
		bool COlumpusMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
			m_internalXMLEntry = L"";
			char _header[8];
			unsigned int baseOffset = base_offset;
			unsigned short size_ = ExifData->seek(0, SEEK_END);
			if (size_- offset - base_offset < 8)
				return false;

			ExifData->seek(offset, SEEK_SET);
			ExifData->read(&(_header[0]), 8);
			if (0 != strncmp(_header, "OLYMP", 5))
				return false;

			bool bType2 = false;
			short makernoteOrder = 0;
			if (0 == strncmp(_header, "OLYMPUS", 7))
			{
				// это не описанная в документации ситуация:
				// за заголовком из 8 байт (OLYMPUS\0x??) идет идентификатор byte order (II или MM(?))
				// затем через 2 байта начинается IFD, при этом смещения расчитываются от начала makernote
				// todo: надо более подробно проверить
				bType2 = true;
				baseOffset = offset;
				ExifData->read(&makernoteOrder, 2);
			}
			
			int pos = 0;
			if (getParseAppSegHeaderState())
			{
				char _byte = 0;
				ExifData->seek(2, SEEK_SET);             
				ExifData->read(&_byte, 1);
				while(_byte != 0)
					ExifData->read(&_byte, 1);

				ExifData->read(&_byte, 1);
				while(0 == _byte)
					ExifData->read(&_byte, 1);

				pos = ExifData->seek(-1, SEEK_CUR);
			}
			Exif::ExifIFDMap _Map;
			Exif::ExifIFD exif_ifd(0, (!bType2)?(offset + 8):(offset + 12), baseOffset + pos, 0, &_Map, 0);

			if (bType2)
			{
				if (makernoteOrder != 0x4949)
				{
					ExifData->flags() |= EXIF_SWAB;
					ExifData->bigEndian(true);
				}else
				{
					ExifData->flags() &= (~EXIF_SWAB);
					ExifData->bigEndian(false);
				}
			}

			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_OLYMPUS, m_internalXMLEntry);
			return true;
		}


		std::wstring COlumpusMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}
	}
}
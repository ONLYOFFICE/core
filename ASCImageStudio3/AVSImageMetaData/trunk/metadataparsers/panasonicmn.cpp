/*!	\file panasonicmn.cpp
*
*	\remarks	см.	doc\makernote\MakerNotePanasonicTags.html
*					doc\makernote\panasonic_mn.html
*/

/*!
\remarks

	Panasonic Type 1 Makernote

	Header:
		12 Bytes
		"Panasonic\x00\x00\x00"

	IFD Data:
		Variable
		NON-Standard TIFF IFD Data using Panasonic Tags. There is no Next-IFD pointer after the IFD
		Offsets are relative to the start of the TIFF header at the beginning of the EXIF segment 

	
	Panasonic Type 2 Makernote (Blank)

	Header:
		4 Bytes
		"MKED"

	Junk:
		1 or 2 bytes
		Blank or Junk data
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
		bool CPanasonicMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
			m_internalXMLEntry = L"";

			char _header[12];
			unsigned short size_ = ExifData->seek(0, SEEK_END);
			if (size_- offset - base_offset < 12)
				return false;

			ExifData->seek(offset, SEEK_SET);
			ExifData->read(&(_header[0]), 12);
			if (0 != strncmp("Panasonic", _header, 9))
				return false;

			Exif::ExifIFDMap _Map;
			// кусок взят из ExifAppSegManager::getThisAppSeg
			
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
			Exif::ExifIFD exif_ifd(0, offset + 12, base_offset + pos, 0, &_Map,0);
			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_PANASONIC, m_internalXMLEntry);
			return true;
		}

		std::wstring CPanasonicMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}
	}
}
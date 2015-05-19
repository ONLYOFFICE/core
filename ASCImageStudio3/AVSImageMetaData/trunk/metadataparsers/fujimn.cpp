/*!	\file fujimn.cpp
*
*	\remarks	см.	doc\makernote\MakerNoteFujiFilmTags.html
*					doc\makernote\fujifilm_mn.html
*/

/*!
\remarks

	Header:
		8 Bytes
		"FUJIFILM"

	IFD Offset:
		4 Bytes
		Intel (Little Endian) byte aligned offset to IFD from start of Makernote (usually 12)

	IFD Data:
		Variable 	NON-Standard IFD Data using Fujifilm Tags.
		Exif offsets are relative to start of makernote instead of the start of the TIFF header
		Byte alignment is always Intel (Little Endian) 
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
		bool CFujiMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
			m_internalXMLEntry = L"";
			char _header[8];					
			unsigned short size_ = ExifData->seek(0, SEEK_END);
			if (size_- offset - base_offset < 8)
				return false;

			ExifData->seek(offset, SEEK_SET);
			ExifData->read(&(_header[0]), 8);
			if (0 != strncmp(_header, "FUJIFILM", 8))
				return false;
		
			unsigned long fujiIFDOffset = 0;
			ExifData->read(&fujiIFDOffset, 4);


			
			int pos = 0;
			Exif::ExifIFDMap _Map;
			Exif::ExifIFD exif_ifd(0, offset + fujiIFDOffset, /*base_offset*/offset + pos, 0, &_Map,0);
			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_FUJI, m_internalXMLEntry);
			return true;
									
		}
		
		std::wstring CFujiMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}
	}
}
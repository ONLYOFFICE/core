/*!	\file canonmn.cpp
*	\brief Canon Makernote parser
*
*	\remarks	см.	offilne:
*					doc\makernote\MakerNoteCanonTags.html
*					doc\makernote\canon_mn.html
*					web:
*					http://www.ozhiker.com/electronics/pjmt/jpeg_info/canon_mn.html
*					http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/Canon.html
*
*
*	\todo		ПРОТЕСТИРОВАТЬ!
*/

/*!
	Casio Type 1 Makernote

	IFD Data:
		Variable
		Standard TIFF IFD Data using Casio Type 1 Tags but always uses Motorola (Big-Endian) Byte Alignment

	
	Casio Type 2 Makernote

	Header:
		6 Bytes
		"QVC\x00\x00\x00"
	
	IFD Data:
	Variable
	Standard TIFF IFD Data using Casio Type 2 Tags but always uses Motorola (Big-Endian) Byte Alignment


	All EXIF offsets are relative to the start of the TIFF header at the beginning of the EXIF segment
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
		/*!	\brief	Разобрать makernote для Exif, предоставляемой камерами Canon
		*
		*	Передаем AppSegment в котором содержится makernote и записанный за ним Сам makernote
		*/
		bool CCasioMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
			m_internalXMLEntry = L"";
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

			Exif::ExifIFD exif_ifd(0, offset, base_offset + pos, 0, &_Map,0);
			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_CASIO, m_internalXMLEntry);
			return true;
		}

		std::wstring CCasioMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}
	}
}
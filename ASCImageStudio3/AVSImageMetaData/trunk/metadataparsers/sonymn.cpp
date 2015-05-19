/*!	\file sonymn.cpp
*
*	\remarks	см.	doc\makernote\MakerNoteSonyTags.html
*					doc\makernote\sony_mn.html
*/

/*!
\remarks

	Header:
		12 Bytes
		"SONY CAM \x00\x00\x00" or "SONY DSC \x00\x00\x00"

	IFD Data:
		Variable
		NON-Standard TIFF IFD Data using Sony Tags
		There is no Next-IFD pointer at end of the IFD 
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
		int CSonyMakerNote::CheckType(char * buffer_, size_t buffer_size,size_t & headersize)
		{
			headersize = 0;
			if ((buffer_size >= 12) && (buffer_[0] == 'S')&& (buffer_[1] == 'O')
				&& (buffer_[2] == 'N')&& (buffer_[3] == 'Y')&& (buffer_[4] == ' ')
				&& (buffer_[8] == ' ')&& (buffer_[9] == 0)&& (buffer_[10] == 0)&& (buffer_[11] == 0)
				)
			{
				headersize = 12;
				return 1;
			}
			return 0;
		}

		/*!	\brief	Разобрать makernote для Exif, предоставляемой камерами Canon
		*
		*	Передаем AppSegment в котором содержится makernote и записанный за ним Сам makernote
		*/
		bool CSonyMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
			m_internalXMLEntry = L"";
			char _header[12];					
			unsigned short size_ = ExifData->seek(0, SEEK_END);
			if (size_- offset - base_offset < 12)
				return false;
			ExifData->seek(offset, SEEK_SET);
			ExifData->read(&(_header[0]), 12);
			size_t _headersize;
			int SonyType = CheckType(&(_header[0]), 12, _headersize);
			if (SonyType == 0)
				return false;

			Exif::ExifIFDMap _Map;

			int pos = 0;
			// кусок взят из ExifAppSegManager::getThisAppSeg
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

			Exif::ExifIFD exif_ifd(0, offset + _headersize, base_offset + pos, 0, &_Map,0);
			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_SONY, m_internalXMLEntry);
			return true;
		}

		std::wstring CSonyMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}
	}
}
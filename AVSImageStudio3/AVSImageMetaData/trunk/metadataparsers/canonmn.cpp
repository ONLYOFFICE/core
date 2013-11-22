/*!	\file canonmn.cpp
*
*	\remarks	см.	doc\makernote\MakerNoteCanonTags.html
*					doc\makernote\canon_mn.html
*/

/*!
\remarks

	IFD Data:
		Variable
		Standard TIFF IFD Data using Canon Tags

	This makernote has no header - the IFD starts immediately
	All EXIF offsets are relative to the start of the TIFF header at the beginning of the EXIF segment

*/
#include "stdafx.h"
#include "specificmn.h"
#include "..\openexif\src\ExifTags.h"
#include "..\openexif\src\ExifIO.h"
#include "..\openexif\src\ExifIFD.h"
#include "..\XML\MetaDataTransform.h"


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
		bool CCanonMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
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
			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_CANON, m_internalXMLEntry);
			return true;
		}

		std::wstring CCanonMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}

	}
}
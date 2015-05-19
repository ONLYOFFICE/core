/*!	\file nikonmn.cpp
*	\brief Nikon Makernote parser
*
*	\remarks	см.	doc\makernote\MakerNoteNikonTags.html
*					doc\makernote\nikon_mn.html
*					http://www.ozhiker.com/electronics/pjmt/jpeg_info/nikon_mn.html
*					http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/Nikon.html
*
*	\todo протестировать для типа 1 и 2
*/

/*!
\remarks

	Nikon Type 1 Makernote

	Header:
		8 Bytes
		"Nikon\x00\x01\x00"

	IFD Data:
		Variable
		Standard TIFF IFD Data using Nikon Type 1 Tags
		Offsets are relative to the start of the TIFF header at the beginning of the EXIF segment 


	Nikon Type 2 Makernote

	IFD Data:
		Variable
		Standard TIFF IFD Data using Nikon Type 3 Tags
		Offsets are relative to the start of the TIFF header at the beginning of the EXIF segment

		This makernote has no header - the IFD starts immediately


	Nikon Type 3 Makernote

	Header:
		10 Bytes
		"Nikon\x00\x02\x10\x00\x00" or "Nikon\x00\x02\x00\x00\x00"

	TIFF Data: 
		Variable
		TIFF header, then standard IFD Data using Nikon Type 3 Tags.
		Offsets are from this second TIFF header, not the one at the start of the EXIF segment 

*/

#include "stdafx.h"
#include "specificmn.h"

#include "..\openexif\src\ExifTags.h"
#include "..\openexif\src\ExifIO.h"
#include "..\openexif\src\ExifIFD.h"
#include "..\openexif\src\ExifTiffAppSeg.h"

#include <string>
#include <sstream>
#include <iostream>

namespace AVS
{
	namespace Image
	{
		int CNikonMakerNote::CheckType(char * buffer_, size_t buffer_size,size_t & headersize)
		{
			headersize = 0;
			if ((buffer_size >= 5) && (buffer_[0] == 'N')&& (buffer_[1] == 'i')
				&& (buffer_[2] == 'k')&& (buffer_[3] == 'o')&& (buffer_[4] == 'n'))
			{
				if ((buffer_size >= 8)&&(buffer_[5] == 0)&&(buffer_[6] == 1)&&(buffer_[7] == 0))
				{
					headersize = 8;
					return 1;
				}
				if ((buffer_size >= 10)&&(buffer_[5] == 0)&&(buffer_[6] == 2)&&(1 /* todo */ || (buffer_[7] == 1) || (buffer_[7] == 0) )
					&&(buffer_[8] == 0)&&(buffer_[9] == 0))
				{
					headersize = 10;
					return 3;
				}
				return 0; // invalid
			}
			return 2;
		}

		bool CNikonMakerNote::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
			m_internalXMLEntry = L"";
			char _header[10];					
			unsigned short size_ = ExifData->seek(0,SEEK_END);
			if (size_ - offset - base_offset < 10)
				return false;
			ExifData->seek(offset, SEEK_SET);
			ExifData->read(&(_header[0]), 10);
			size_t _headersize;
			int NikonType = CheckType(&(_header[0]), 10, _headersize);
			
			if (0 == NikonType)
				return false;

			ExifData->seek(_headersize + offset, SEEK_SET);
			if (3 == NikonType) // TIFF // это протестировано (камера Nikon D700, например)
			{
				Exif::ExifTiffAppSeg AppSeg(/*MakerNoteData*/ExifData, 0, size_, 0, _headersize + offset);
				Exif::ExifPathsTags makernoteTags;
				AppSeg.getAllTags(makernoteTags);
				std::wstringstream outputXMLEntry;
				Exif::ExifPathsTagsIter crntPathsTags = makernoteTags.begin();
				Exif::ExifPathsTagsIter endPathsTags = makernoteTags.end();
				while( crntPathsTags != endPathsTags )
				{
					Exif::ExifTagsIter crnt = (*crntPathsTags).second.begin() ;
					Exif::ExifTagsIter end = (*crntPathsTags).second.end() ;
					while( crnt != end )
					{
						Exif::ExifTagEntry* tag = *(crnt);
						tag->PrintXMLEntryEx(outputXMLEntry, std::wstring(L"makernote"), MAKERNOTE_NIKON3);
						outputXMLEntry << std::endl;
						crnt++;
					}
					crntPathsTags++;
				}
				m_internalXMLEntry = outputXMLEntry.str();
				return true;
			}
			// этот кусок не протестирован! все фотографии nikon, которые попадались были с типом 3

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

			Exif::ExifIFD exif_ifd(0, offset + ((1 == NikonType)?8:0), base_offset + pos, 0, &_Map, 0);
			PrintAllTags(&exif_ifd, ExifData, MAKERNOTE_NIKON1, m_internalXMLEntry);
			return true;
		}

		std::wstring CNikonMakerNote::_DoGetXML()
		{
			return m_internalXMLEntry;
		}
	}
}
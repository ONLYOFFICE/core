#include "stdafx.h"
#include "exifmakernote.h"
#include "..\openexif\src\ExifImageFile.h"
#include "..\openexif\src\ExifIO.h"
#include "..\openexif\src\ExifTags.h"
#include "..\openexif\src\ExifIFD.h"

#include <string>
#include <sstream>
#include <iostream>

namespace AVS
{
	namespace Image
	{
		CMakerNoteFactory::Registry* CMakerNoteFactory::pRegistry_ = NULL;

		bool CMakerNoteIfd::_DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
		{
/*			if (!ParseIfdHeader(MakerNoteData, this->IfdHeaderOffset, flags))
				return false;*/


			//for (int i = 0; i < DirEntrys.size(); i++)
			//{
			//	std::cout << "Tag: " << DirEntrys[i].tag << " type: " << DirEntrys[i].type
			//		<< " count: " << DirEntrys[i].count << " offest: " << DirEntrys[i].offset << std::endl;
			//}

			return true;
		}



		/*bool CMakerNoteIfd::ParseIfdHeader(const std::vector<char> & MakerNoteData, unsigned int IfdOffset, unsigned int flags)
		{
			char * data = (char *)(&(MakerNoteData[0])) + IfdOffset;
			std::vector<char>::size_type data_size = MakerNoteData.size() - IfdOffset;

			unsigned short DirCount = *((unsigned short*)data);
			data += sizeof(unsigned short);
			data_size -= sizeof(unsigned short);
			Swab(&DirCount);

			if (DirCount > 0x1FFF || DirCount == 0)
				return false;

			DirEntrys.resize(DirCount);
			unsigned short i;
			for (i = 0; i < DirCount; i++)
			{
				if (data_size < sizeof(ExifDirEntrysArray::value_type))
					// ошибка, конец буфера
					break;

				if (0 != memcpy_s(&(DirEntrys[i]), sizeof(ExifDirEntrysArray::value_type), data, sizeof(ExifDirEntrysArray::value_type)))
					break;

				data += sizeof(ExifDirEntrysArray::value_type);
				data_size -= sizeof(ExifDirEntrysArray::value_type);

				Swab(&(DirEntrys[i].count));
				//Swab(&(DirEntrys[i].offset)); // TODO
				Swab(&(DirEntrys[i].tag));
				Swab(&(DirEntrys[i].type));
			}
		}
		*/


	}
}

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
#include "ImageFileFormatChecker.h"
#include "../common/File.h"

#include "../cximage/CxImage/ximacfg.h"


#define MIN_SIZE_BUFFER 4096
#define MAX_SIZE_BUFFER 102400

typedef struct ___tagBITMAPINFOHEADER {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} ___BITMAPINFOHEADER;

CImageFileFormatChecker::CImageFileFormatChecker()
{
	eFileType = _CXIMAGE_FORMAT_UNKNOWN;
}
CImageFileFormatChecker::CImageFileFormatChecker(std::wstring sFileName)
{
	eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	isImageFile(sFileName);
}
CImageFileFormatChecker::CImageFileFormatChecker(BYTE* pBuffer, DWORD dwBytes)
{
	eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	isImageFile(pBuffer, dwBytes);
}

//bmp ( http://ru.wikipedia.org/wiki/BMP )
bool CImageFileFormatChecker::isBmpFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;
	
	if ( (34 <= dwBytes) && (0x42 == pBuffer[0]) && (0x4D == pBuffer[1]) &&
		 (0x00 == pBuffer[6]) && (0x00 == pBuffer[7]) && (0x01 == pBuffer[26]) && (0x00 == pBuffer[27]) &&
		 ( (0x00 == pBuffer[28]) || (0x01 == pBuffer[28]) || (0x04 == pBuffer[28]) || (0x08 == pBuffer[28]) ||
		   (0x10 == pBuffer[28]) || (0x18 == pBuffer[28]) || (0x20 == pBuffer[28]) ) && (0x00 == pBuffer[29]) &&
		 ( (0x00 == pBuffer[30]) || (0x01 == pBuffer[30]) || (0x02 == pBuffer[30]) || (0x03 == pBuffer[30]) ||
		   (0x04 == pBuffer[30]) || (0x05 == pBuffer[30]) ) && (0x00 == pBuffer[31]) && (0x00 == pBuffer[32]) && (0x00 == pBuffer[33]) )
		return true;

	return false;
}
//png Hex: 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
bool CImageFileFormatChecker::isPngFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (16 <= dwBytes) && (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
		 && (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
		 && (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
		 && (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
		return true;

	return false;
}
//gif Hex: 47 49 46 38 //or Hex: 47 49 46 38 37 61 //or Hex: 47 49 46 38 39 61
bool CImageFileFormatChecker::isGifFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (4 <= dwBytes) && (0 == strncmp( (char *)pBuffer, "GIF8", 4)))
		return true;
	if ( (6 <= dwBytes) && ( (0 == strncmp( (char *)pBuffer, "GIF87a", 6)) || (0 == strncmp((char *)pBuffer, "GIF89a", 6)) ) )
		return true;

	return false;
}
//tiff //Hex big endian: 4D 4D 00 2A	//Hex little endian: 49 49 2A 00
bool CImageFileFormatChecker::isTiffFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ((dwBytes >=4) && ( (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2]) && (0x00 == pBuffer[3]) ) ||
			( (0x4D == pBuffer[0]) && (0x4D == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x2A == pBuffer[3]) ) ||
			( (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2]) && (0x00 == pBuffer[3]) ))
		return true;
	
	return false;
}
//wmf - Hex: D7 CD C6 9A 00 00 //or Hex: 01 00 09 00 00 03
bool CImageFileFormatChecker::isWmfFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;
	
	if ((dwBytes >=6) && ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) ||
			((0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x09 == pBuffer[2]) && (0x00 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x03 == pBuffer[5]) ))
		return true;
	
	return false;
}
//emf ( http://wvware.sourceforge.net/caolan/ora-wmf.html )
bool CImageFileFormatChecker::isEmfFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (44 <= dwBytes) && (0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x00 == pBuffer[3]) &&
		 (0x20 == pBuffer[40]) && (0x45 == pBuffer[41]) && (0x4D == pBuffer[42]) && (0x46 == pBuffer[43]) )
		return true;

	return false;
}

//pcx ( http://www.fileformat.info/format/pcx/corion.htm )
bool CImageFileFormatChecker::isPcxFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)
		return false;

	if ( (4 <= dwBytes) && (0x0A == pBuffer[0]) && (0x00 == pBuffer[1] || 0x01 == pBuffer[1] ||
													0x02 == pBuffer[1] || 0x03 == pBuffer[1] || 0x04 == pBuffer[1] || 0x05 == pBuffer[1] ) &&
		 ( 0x01 == pBuffer[3] || 0x02 == pBuffer[3] || 0x04 == pBuffer[3] || 0x08 == pBuffer[3] ))
		return true;

	return false;
}
//tga ( http://www.fileformat.info/format/tga/corion.htm )
bool CImageFileFormatChecker::isTgaFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (17 <= dwBytes) && ( (0x01 == pBuffer[1] && 0x01 == pBuffer[2]) || (0x00 == pBuffer[1] && 0x02 == pBuffer[2]) ||
							  (0x00 == pBuffer[1] && 0x03 == pBuffer[2]) || (0x01 == pBuffer[1] && 0x09 == pBuffer[2]) ||
							  (0x00 == pBuffer[1] && 0x0A == pBuffer[2]) || (0x00 == pBuffer[1] && 0x0B == pBuffer[2]) )
		 && ( 0x08 == pBuffer[16] || 0x10 == pBuffer[16] || 0x18 == pBuffer[16] || 0x20 == pBuffer[16] ))
		return true;

	return false;
}
//jpeg Hex: FF D8 FF
bool CImageFileFormatChecker::isJpgFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;
	
	if ( (3 <= dwBytes) && (0xFF == pBuffer[0]) && (0xD8 == pBuffer[1]) && (0xFF == pBuffer[2]) )
		return true;

	return false;
}
//webshot
bool CImageFileFormatChecker::isWbzFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	return false;
}
bool CImageFileFormatChecker::isWbcFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	return false;
}
//webshot(wb ver 1) HEX 57 57 42 42 31 31 31 31
//webshot (wb ver 2) HEX 00 00 02 00 02 10 c9 00 02 00 c8 06 4c 00 02 00
bool CImageFileFormatChecker::isWbFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (8 <= dwBytes) && (0x57 == pBuffer[0] && 0x57 == pBuffer[1]  && 0x42 == pBuffer[2]  && 0x42 == pBuffer[3]
							&& 0x31 == pBuffer[4] && 0x31 == pBuffer[5]  && 0x31 == pBuffer[6]  && 0x31 == pBuffer[7]))
		return true;

	if ( (16 <= dwBytes) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1]  && 0x02 == pBuffer[2]  && 0x00 == pBuffer[3]
							 && 0x02 == pBuffer[4] && 0x10 == pBuffer[5]  && 0xc9 == pBuffer[6]  && 0x00 == pBuffer[7]
							 && 0x02 == pBuffer[8] && 0x00 == pBuffer[9]  && 0xc8 == pBuffer[10]  && 0x06 == pBuffer[11]
							 && 0x4c == pBuffer[12] && 0x00 == pBuffer[13]  && 0x02 == pBuffer[14]  && 0x00 == pBuffer[15]))
		return true;

	return false;
}
//ico http://en.wikipedia.org/wiki/ICO_(file_format)
bool CImageFileFormatChecker::isIcoFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;
	
	if ( (16 <= dwBytes) && (0x00 == pBuffer[0] && 0x00 == pBuffer[1]))
	{
		long width = pBuffer [6];
		long height = pBuffer [7];

		if (width==0)width =256;
		if (height==0)height =256;

		DWORD offset_image = *(DWORD*)(pBuffer+18);

		if (offset_image < dwBytes-40)
		{
			___BITMAPINFOHEADER *image_header=0;//bmp or png
			image_header = (___BITMAPINFOHEADER *)(pBuffer + offset_image);

			if (width == image_header->biWidth
					/*height == image_header->biHeight*/) // в хеадере может быть черестрочная высота
				return true;
		}
	}

	return false;
}
//http://www.adobe.com/devnet-apps/photoshop/fileformatashtml/#50577409_19840
bool CImageFileFormatChecker::isPsdFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (12 <= dwBytes) && (0x38 == pBuffer[0] && 0x42 == pBuffer[1]  && 0x50 == pBuffer[2]  && 0x53 == pBuffer[3]
							 && 0x00 == pBuffer[6] && 0x00 == pBuffer[7]  && 0x18 == pBuffer[8]
							 && 0x00 == pBuffer[9] && 0x00 == pBuffer[10]  && 0x18 == pBuffer[11]))
		return true;

	return false;
}
// http://en.wikipedia.org/wiki/Raster_graphics
bool CImageFileFormatChecker::isRasFile(BYTE* pBuffer,DWORD dwBytes)//sun image
{
	if (eFileType)return false;
	
	if ( (16 <= dwBytes) && (0x59 == pBuffer[0] && 0xa6 == pBuffer[1]  && 0x6a == pBuffer[2]  && 0x95 == pBuffer[3]
							 && 0x00 == pBuffer[4] && 0x00 == pBuffer[5]  && 0x18 == pBuffer[15]))
		return true;

	return false;
}
//http://www.atari-portfolio.co.uk/library/text/pgxspec.txt
bool CImageFileFormatChecker::isPgxFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (8 <= dwBytes) && ('P' == pBuffer[0] && 'G' == pBuffer[1]  && 'X' == pBuffer[2]
							&& 0x00 == pBuffer[4] && 0x00 == pBuffer[5]  && 0x00 == pBuffer[6] && 0x00 == pBuffer[7]))
		return true;

	return false;
}
//flash
bool CImageFileFormatChecker::isSwfFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;
	
	if ((10 <= dwBytes) && ((0x46 == pBuffer[0] && 0x57 == pBuffer[1]  && 0x53 == pBuffer[2] && 0x00 == pBuffer[9])
							||	 (0x43 == pBuffer[0] && 0x57 == pBuffer[1]  && 0x53 == pBuffer[2] && 0x78 == pBuffer[8] && 0x9c == pBuffer[9])))
		return true;

	return false;
}
//http://en.wikipedia.org/wiki/Seattle_FilmWorks
bool CImageFileFormatChecker::isSfwFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (44 <= dwBytes) &&(0x53 == pBuffer[0] && 0x46 == pBuffer[1]  && 0x57 == pBuffer[2] && 0x39 == pBuffer[3]
							&& 0x34 == pBuffer[4] && 0x41 == pBuffer[5]  && 0x004 == pBuffer[6] && 0x00 == pBuffer[7]

							&& 0x10 == pBuffer[8] && 0x00 == pBuffer[9]  && 0x01 == pBuffer[10] && 0x00 == pBuffer[11]
							&& 0x64 == pBuffer[12] && 0x00 == pBuffer[13]  && 0x00 == pBuffer[14] && 0x00 == pBuffer[15]

							&& 0xcc == pBuffer[16] && 0x00 == pBuffer[17]  && 0x00 == pBuffer[18] && 0x00 == pBuffer[19]
							&& 0x3b == pBuffer[20] && 0x00 == pBuffer[21]  && 0x00 == pBuffer[22] && 0x00 == pBuffer[23]

							&& 0xda == pBuffer[24] && 0x07 == pBuffer[25]  && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
							&& 0x07 == pBuffer[28] && 0x01 == pBuffer[29]  && 0x00 == pBuffer[30] && 0x00 == pBuffer[31]

							&& 0x44 == pBuffer[32] && 0x00 == pBuffer[33]  && 0x00 == pBuffer[34] && 0x00 == pBuffer[35]
							&& 0xd0 == pBuffer[36] && 0x07 == pBuffer[37]  && 0x00 == pBuffer[38] && 0x00 == pBuffer[39]

							&& 0x4b == pBuffer[40] && 0x01 == pBuffer[41]  && 0x00 == pBuffer[42] && 0x00 == pBuffer[43]))
		return true;

	return false;
}
//open office - StarView Meta File
bool CImageFileFormatChecker::isSvmFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (6 <= dwBytes) &&(0x56 == pBuffer[0] && 0x43 == pBuffer[1]  && 0x4c == pBuffer[2] && 0x4d == pBuffer[3]
						   && 0x54 == pBuffer[4] && 0x46 == pBuffer[5]  /*&& 0x01 == pBuffer[6] && 0x00 == pBuffer[7]
												&& 0x31 == pBuffer[8]*/ && 0x00 == pBuffer[9]  && 0x00 == pBuffer[10] && 0x00 == pBuffer[11]) )
		//0x02, 0x00, 0x32,
		//0x01,0x00, 0x031
		return true;

	return false;
}
bool CImageFileFormatChecker::isSvgFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (6 <= dwBytes) &&(0x3C == pBuffer[0] && 0x3F == pBuffer[1]  && 0x78 == pBuffer[2] && 0x6D == pBuffer[3]
						   && 0x6C == pBuffer[4] && 0x20 == pBuffer[5]))
	{
		std::string sXml_part = std::string((char*)pBuffer, dwBytes);
		if (sXml_part.find(std::string("svg")) != std::string::npos)
		{
			return true;
		}
	}
	return false;
}

bool CImageFileFormatChecker::isJ2kFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (52 <= dwBytes) && (0xff == pBuffer[0] && 0x4f == pBuffer[1]  && 0xff == pBuffer[2] && 0x51 == pBuffer[3]
							 //&& 0x00 == pBuffer[4] && 0x2f == pBuffer[5]  && 0x00 == pBuffer[6] && 0x00 == pBuffer[7]
							 //..
							 //&& 0x00 == pBuffer[16] && 0x00 == pBuffer[17]  && 0x00 == pBuffer[18] && 0x00 == pBuffer[19]
							 //&& 0x00 == pBuffer[20] && 0x00 == pBuffer[21]  && 0x00 == pBuffer[22] && 0x00 == pBuffer[23]
							 //..
							 // && 0x00 == pBuffer[32] && 0x00 == pBuffer[33]  && 0x00 == pBuffer[34] && 0x00 == pBuffer[35]
							 // && 0x00 == pBuffer[36] && 0x00 == pBuffer[37]  && 0x00 == pBuffer[38] && 0x00 == pBuffer[39]

							 // && 0x00 == pBuffer[40] && 0x03 == pBuffer[41]  && 0x07 == pBuffer[42] && 0x01 == pBuffer[43]
							 // && 0x01 == pBuffer[44] && 0x07 == pBuffer[45]  && 0x01 == pBuffer[46] && 0x01 == pBuffer[47]

							 // && 0x07 == pBuffer[48] && 0x01 == pBuffer[49]  && 0x01 == pBuffer[50] && 0xff == pBuffer[51]
							 ))
		return true;

	return false;
}

bool CImageFileFormatChecker::isJp2File(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (32 <= dwBytes) &&(0x00 == pBuffer[0] && 0x00 == pBuffer[1]  && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
							&& 0x6a == pBuffer[4] && 0x50 == pBuffer[5]  && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]

							&& 0x0d == pBuffer[8] && 0x0a == pBuffer[9]  && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
							&& 0x00 == pBuffer[12] && 0x00 == pBuffer[13]  && 0x00 == pBuffer[14]/* &&  (0x14 == pBuffer[15] || 0x18 == pBuffer[15] )*/

							&& 0x66 == pBuffer[16] && 0x74 == pBuffer[17]  && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
							&& 0x6a == pBuffer[20] && 0x70 == pBuffer[21]  && 0x32 == pBuffer[22] && 0x20 == pBuffer[23]

							&& 0x00 == pBuffer[24] && 0x00 == pBuffer[25]  && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
							/*&& 0x6a == pBuffer[28] && 0x70 == pBuffer[29]  && 0x32 == pBuffer[30] && 0x20 == pBuffer[31]*/))
		return true;

	return false;
}
bool CImageFileFormatChecker::isMj2File(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	if ( (32 <= dwBytes) &&(0x00 == pBuffer[0] && 0x00 == pBuffer[1]  && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
							&& 0x6a == pBuffer[4] && 0x50 == pBuffer[5]  && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]

							&& 0x0d == pBuffer[8] && 0x0a == pBuffer[9]  && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
							&& 0x00 == pBuffer[12] && 0x00 == pBuffer[13]  && 0x00 == pBuffer[14] && 0x18 == pBuffer[15]

							&& 0x66 == pBuffer[16] && 0x74 == pBuffer[17]  && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
							&& 0x6d == pBuffer[20] && 0x6a == pBuffer[21]  && 0x70 == pBuffer[22] && 0x32 == pBuffer[23]

							&& 0x00 == pBuffer[24] && 0x00 == pBuffer[25]  && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
							&& 0x6d == pBuffer[28] && 0x6a == pBuffer[29]  && 0x70 == pBuffer[30] && 0x32 == pBuffer[31]))
		return true;

	return false;
}
bool CImageFileFormatChecker::isIpodFile(BYTE* pBuffer,DWORD dwBytes)
{
	if (eFileType)return false;

	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CImageFileFormatChecker::isImageFile(std::wstring& fileName)
{
	eFileType  = _CXIMAGE_FORMAT_UNKNOWN;
	///////////////////////////////////////////////////////////////////////////////
	NSFile::CFileBinary file;
	if (!file.OpenFile(fileName))
		return false;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!file.ReadFile(buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete []buffer;
		return false;
	}
	file.CloseFile();
	/////////////////////////////////////////////////////////////////////////////////
	if (isBmpFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_BMP;
	}
	if (isGifFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_GIF;
	}
	if (isPngFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PNG;
	}
	if (isTgaFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_TGA;
	}
	if (isPcxFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PCX;
	}
	if (isJpgFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JPG;
	}
	if (isEmfFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_EMF;
	}
	if (isWmfFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_WMF;
	}
	if (isTiffFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_TIF;
	}
	if (isIcoFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_ICO;
	}
	if (isWbFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_WB;
	}
	if (isPsdFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PSD;
	}
	if (isRasFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_RAS;
	}

	if (isIpodFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isJ2kFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JP2;
	}
	if (isJp2File(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JP2;
	}
	if (isMj2File(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JP2;
	}
	if (isSfwFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isSvmFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isSwfFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isWbcFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isWbzFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	///////////////////////////////////////////////////////////////////////
	if (isSvgFile(fileName))
	{
		eFileType = _CXIMAGE_FORMAT_SVG;
	}
	if (isRawFile(fileName))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	///////////////////////////////////////////////////////////////////////
	delete [] buffer;

	if (eFileType)return true;
	return false;
}
bool CImageFileFormatChecker::isImageFile(BYTE* buffer, DWORD sizeRead)
{
	eFileType  = _CXIMAGE_FORMAT_UNKNOWN;
	///////////////////////////////////////////////////////////////////////
	if (!buffer || sizeRead <= 0)
		return false;
	/////////////////////////////////////////////////////////////////////////////////
	if (isBmpFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_BMP;
	}
	if (isGifFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_GIF;
	}
	if (isPngFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PNG;
	}
	if (isTgaFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_TGA;
	}
	if (isPcxFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PCX;
	}
	if (isJpgFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JPG;
	}
	if (isEmfFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_EMF;
	}
	if (isWmfFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_WMF;
	}
	if (isTiffFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_TIF;
	}
	if (isIcoFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_ICO;
	}
	if (isWbFile(buffer,  sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_WB;
	}
	if (isPsdFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PSD;
	}
	if (isRasFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_RAS;
	}

	if (isIpodFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isJ2kFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JP2;
	}
	if (isJp2File(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JP2;
	}
	if (isMj2File(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_JP2;
	}
	if (isSfwFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isSvmFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isSwfFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isWbcFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	if (isWbzFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	///////////////////////////////////////////////////////////////////////
	if (isSvgFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_SVG;
	}
	if (isRawFile(buffer, sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	}
	///////////////////////////////////////////////////////////////////////
	if (eFileType) return true;
	return false;
}
bool CImageFileFormatChecker::isSvmFile(std::wstring & fileName)
{
	eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	////////////////////////////////////////////////////////////////////////////////
	NSFile::CFileBinary file;
	if (!file.OpenFile(fileName))
		return false;

	BYTE* buffer = new BYTE[12];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!file.ReadFile(buffer, 12, sizeRead))
	{
		delete []buffer;
		return false;
	}
	file.CloseFile();
	////////////////////////////////////////////////////////////////////////////////
	
	if (isSvmFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_SVM;
	}
	delete [] buffer;

	if (eFileType)return true;
	else return false;
}
bool CImageFileFormatChecker::isPngFile(std::wstring & fileName)
{
	eFileType = _CXIMAGE_FORMAT_UNKNOWN;
	////////////////////////////////////////////////////////////////////////////////
	NSFile::CFileBinary file;
	if (!file.OpenFile(fileName))
		return false;

	BYTE* buffer = new BYTE[MIN_SIZE_BUFFER];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!file.ReadFile(buffer, MIN_SIZE_BUFFER, sizeRead))
	{
		delete []buffer;
		return false;
	}
	file.CloseFile();
	////////////////////////////////////////////////////////////////////////////////
	
	if (isPngFile(buffer,sizeRead))
	{
		eFileType = _CXIMAGE_FORMAT_PNG;
	}
	delete [] buffer;

	if (eFileType)return true;
	else return false;

}

bool CImageFileFormatChecker::isRawFile(std::wstring& fileName)
{
	// TODO:
	return false;
}
bool CImageFileFormatChecker::isRawFile(BYTE* pBuffer, DWORD dwBytes)
{
	// TODO:
	return false;
}
bool CImageFileFormatChecker::isSvgFile(std::wstring& fileName)
{
	NSFile::CFileBinary file;
	if (!file.OpenFile(fileName))
		return false;

	DWORD nSize = (DWORD)file.GetFileSize();
	if (nSize > 100)
		nSize = 100;

	BYTE* buffer = new BYTE[nSize];
	if (!buffer)
		return false;

	DWORD sizeRead = 0;
	if (!file.ReadFile(buffer, nSize, sizeRead))
	{
		delete []buffer;
		return false;
	}
	file.CloseFile();

	if ('<' == buffer[0] &&
			's' == buffer[1] &&
			'v' == buffer[2] &&
			'g' == buffer[3])
	{
		delete [] buffer;
		return true;
	}

	if ('<' == buffer[0] &&
			'?' == buffer[1] &&
			'x' == buffer[2] &&
			'm' == buffer[3] &&
			'l' == buffer[4])
	{
		std::string test((char*)buffer, nSize);
		if (std::string::npos != test.find("<svg"))
		{
			delete [] buffer;
			return true;
		}
	}

	delete [] buffer;
	return false;
}

std::wstring CImageFileFormatChecker::DetectFormatByData(BYTE *Data, int DataSize)
{
	if (isBmpFile(Data,DataSize)) return L"bmp";
	else if (isJpgFile(Data,DataSize)) return L"jpeg";
	else if (isPngFile(Data,DataSize)) return L"png";
	else if (isGifFile(Data,DataSize)) return L"gif";
	else if (isIcoFile(Data,DataSize)) return L"ico";
	else if (isPcxFile(Data,DataSize)) return L"pcx";
	else if (isRasFile(Data,DataSize)) return L"ras";
	else if (isTiffFile(Data,DataSize))return L"tif";
	else if (isWmfFile(Data,DataSize)) return L"wmf";
    else if (isEmfFile(Data,DataSize)) return L"emf";
	else if (isSvmFile(Data,DataSize)) return L"svm";
	else if (isSvgFile(Data,DataSize)) return L"svg";
	
	return L"";
}

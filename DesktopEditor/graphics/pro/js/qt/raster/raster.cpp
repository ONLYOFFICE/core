/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "raster.h"
#include "../../../../../raster/BgraFrame.h"
#include "../../../../../raster/ImageFileFormatChecker.h"
#include "../../../../../raster/Metafile/MetaFile.h"

void* Raster_DecodeFile(unsigned char* buffer, int size, bool isRgba)
{
	CBgraFrame* pFrame = new CBgraFrame();
	pFrame->put_IsRGBA(isRgba);
	pFrame->Decode(buffer, size);
	return pFrame;
}
void* Raster_GetDecodedBuffer(void* frame)
{
	return ((CBgraFrame*)frame)->get_Data();
}
int Raster_GetWidth (void* frame)
{
	return ((CBgraFrame*)frame)->get_Width();
}
int Raster_GetHeight(void* frame)
{
	return ((CBgraFrame*)frame)->get_Height();
}
int Raster_GetStride(void* frame)
{
	return ((CBgraFrame*)frame)->get_Stride();
}
void Raster_Destroy(void* frame)
{
	delete ((CBgraFrame*)frame);
}

class CEncodedData
{
public:
	BYTE* Data;
	int Size;

	bool IsDeleteDeleter;

public:
	CEncodedData()
	{
		Data = 0;
		Size = 0;
		IsDeleteDeleter = false;
	}
	~CEncodedData()
	{
		if (Data)
		{
			if (!IsDeleteDeleter)
				CBgraFrame::FreeEncodedMemory(Data);
			else
				delete [] Data;
		}
	}
};

void* Raster_EncodeImageData(unsigned char* buffer, int w, int h, int stride, int format, bool isRgba)
{
	CBgraFrame oFrame;
	oFrame.put_Data(buffer);
	oFrame.put_Width(w);
	oFrame.put_Height(h);
	oFrame.put_Stride(stride);
	oFrame.put_IsRGBA(isRgba);
	CEncodedData* pEncodedData = new CEncodedData();
	oFrame.Encode(pEncodedData->Data, pEncodedData->Size, format);
	oFrame.put_Data(NULL);
	return pEncodedData;
}
void* Raster_Encode(unsigned char* buffer, int size, int format)
{
	CImageFileFormatChecker oChecker;
	bool bIsImageFile = oChecker.isImageFile(buffer, (DWORD)size);

	if (bIsImageFile)
	{
		switch (oChecker.eFileType)
		{
		case _CXIMAGE_FORMAT_WMF:
		case _CXIMAGE_FORMAT_EMF:
		{
			if (_CXIMAGE_FORMAT_SVG == format)
			{
		#ifndef GRAPHICS_DISABLE_METAFILE
				MetaFile::IMetaFile* pMetaFile = MetaFile::Create(NULL);
				pMetaFile->LoadFromBuffer(buffer, (DWORD)size);
				std::wstring wsSvg = pMetaFile->ConvertToSvg();
				std::string sSvg = U_TO_UTF8(wsSvg);
				pMetaFile->Release();

				CEncodedData* pEncodedData = new CEncodedData();
				pEncodedData->IsDeleteDeleter = true;
				pEncodedData->Data = new BYTE[sSvg.length()];
				pEncodedData->Size = (int)sSvg.length();

				memcpy(pEncodedData->Data, sSvg.c_str(), sSvg.length());
				return pEncodedData;
		#endif
			}
			break;
		}
		default:
			CBgraFrame oFrame;
			oFrame.Decode(buffer, size, format);

			BYTE* pBuffer = NULL;
			int nEncodedSize = 0;

			if (oFrame.Encode(pBuffer, nEncodedSize, format))
			{
				CEncodedData* pEncodedData = new CEncodedData();
				pEncodedData->Data = pBuffer;
				pEncodedData->Size = nEncodedSize;
				return pEncodedData;
			}
			break;
		}
	}
	return NULL;
}
int Raster_GetEncodedSize(void* encodedData)
{
	return ((CEncodedData*)encodedData)->Size;
}
void* Raster_GetEncodedBuffer(void* encodedData)
{
	return ((CEncodedData*)encodedData)->Data;
}
void Raster_DestroyEncodedData(void* encodedData)
{
	delete ((CEncodedData*)encodedData);
}

int Image_GetFormat(unsigned char* buffer, int size)
{
	CImageFileFormatChecker oChecker;
	if (oChecker.isImageFile(buffer, (DWORD)size))
		return oChecker.eFileType;
	return 0;
}

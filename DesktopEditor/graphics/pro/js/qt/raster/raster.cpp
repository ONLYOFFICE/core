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

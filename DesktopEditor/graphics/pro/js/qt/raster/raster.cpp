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

public:
	CEncodedData()
	{
		Data = 0;
		Size = 0;
	}
	~CEncodedData()
	{
		if (Data)
			CBgraFrame::FreeEncodedMemory(Data);
	}
};

void* Raster_EncodeFile(unsigned char* buffer, int w, int h, int stride, int format, bool isRgba)
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

void* SVG_DecodeMetafile(unsigned char* buffer, int size)
{
	return NULL;
}

int Image_GetFormat(unsigned char* buffer, int size)
{
	CImageFileFormatChecker oChecker;
	if (oChecker.isImageFile(buffer, (DWORD)size))
		return oChecker.eFileType;
	return 0;
}

int main()
{
	if (false)
	{
		std::wstring sFilePath = L"D:/1.jpg";
		CBgraFrame oFrame;
		if (oFrame.OpenFile(sFilePath))
		{
			oFrame.SaveFile(L"D:/1.png", 4);
			return 0;
		}
	}

	if (true)
	{
		std::wstring sFilePath = L"D:/1.emf";
		MetaFile::CMetaFile oFrame(NULL);
		if (oFrame.LoadFromFile(sFilePath.c_str()))
		{
			oFrame.ConvertToSvg(L"D:/1.svg");
			return 0;
		}
	}
	return 1;
}

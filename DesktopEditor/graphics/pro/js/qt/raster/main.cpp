#include "raster.h"
#include "../../../../../raster/BgraFrame.h"
#include "../../../../../raster/ImageFileFormatChecker.h"
#include "../../../../../raster/Metafile/MetaFile.h"

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

	if (false)
	{
		std::wstring sFilePath = L"D:/1.emf";
		MetaFile::CMetaFile oFrame(NULL);
		if (oFrame.LoadFromFile(sFilePath.c_str()))
		{
			std::wstring sContentSvg = oFrame.ConvertToSvg();
			NSFile::CFileBinary::SaveToFile(L"D:/1.svg", sContentSvg, true);

			return 0;
		}
	}

	if (true)
	{
		BYTE* pFileBuffer = NULL;
		DWORD nFileSize = 0;
		NSFile::CFileBinary::ReadAllBytes(L"D:/image1.wmf", &pFileBuffer, nFileSize);

		void* pEncodedData = Raster_Encode(pFileBuffer, (int)nFileSize, 24);
		int nEncodedSize = Raster_GetEncodedSize(pEncodedData);
		BYTE* pEncodedBuffer = (BYTE*)Raster_GetEncodedBuffer(pEncodedData);

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(L"D:/123.svg");
		oFile.WriteFile(pEncodedBuffer, nEncodedSize);
		oFile.CloseFile();

		Raster_DestroyEncodedData(pEncodedData);

		RELEASEARRAYOBJECTS(pFileBuffer);
	}

	return 1;
}

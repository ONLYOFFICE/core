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

// PdfReaderTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../PdfReader.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include <vector>
#include <string>
#include "windows.h"

std::vector<std::wstring> GetAllFilesInFolder(std::wstring wsFolder, std::wstring wsExt)
{
	std::vector<std::wstring> vwsNames;

	std::wstring wsSearchPath = wsFolder;
	wsSearchPath.append(L"*.");
	wsSearchPath.append(wsExt);

	WIN32_FIND_DATA oFindData;
	HANDLE hFind = ::FindFirstFile(wsSearchPath.c_str(), &oFindData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(oFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				vwsNames.push_back(oFindData.cFileName);
			}
		} while (::FindNextFile(hFind, &oFindData));
		::FindClose(hFind);
	}
	return vwsNames;
}
void ConvertFolder(PdfReader::CPdfReader& oReader, std::wstring wsFolderPath)
{
	oReader.Close();

	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"pdf");
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		std::wstring wsFilePathName = (wsFilePath.substr(0, wsFilePath.size() - 4));
		if (oReader.LoadFromFile(wsFilePath.c_str(), NULL, NULL, NULL))
		{
			int nPagesCount = oReader.GetPagesCount();
			for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
			{
				std::wstring wsDstFilePath = wsFilePathName + L"_" + std::to_wstring(nPageIndex) + L".png";
				oReader.ConvertToRaster(nPageIndex, wsDstFilePath.c_str(), 4);
			}
			oReader.Close();
		}
		else
		{
			printf("%d of %d %S error %d\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str(), oReader.GetError());
		}		
	}
}

void main()
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	PdfReader::CPdfReader oPdfReader(&oFonts);
	oPdfReader.SetTempFolder(L"D://Test Files//Temp//");
	oPdfReader.SetCMapFolder(L"D://Subversion//AVS//Redist//AVSOfficeStudio//CMaps//");
	ConvertFolder(oPdfReader, L"D://Test Files//");
}


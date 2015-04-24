//#define _CRTDBG_LEAK_CHECK_DF

#include "stdafx.h"

#include "../../fontengine/ApplicationFonts.h"
#include "MetaFile.h"

#include <Windows.h>
#include <vector>

using namespace MetaFile;

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
void ConvertFile(CMetaFile &oMetaFile, std::wstring wsFilePath)
{
	oMetaFile.Close();

	std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"jpg");
	if (oMetaFile.LoadFromFile(wsFilePath.c_str()))
	{
		oMetaFile.ConvertToRaster(wsDstFilePath.c_str(), 4, 1000);
	}
}
void ConvertFolder(CMetaFile &oMetaFile, std::wstring wsFolderPath)
{
	oMetaFile.Close();

	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"emf");
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		if (oMetaFile.LoadFromFile(wsFilePath.c_str()))
		{
			std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"jpg");
			oMetaFile.ConvertToRaster(wsDstFilePath.c_str(), 4, 1000);
			oMetaFile.Close();
		}

		printf("%d of %d %S\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
	}
}

void Test()
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	CMetaFile oMetaFile(&oFonts);
	//ConvertFile(oMetaFile, L"D://Test Files//fulltest.wmf");
	ConvertFolder(oMetaFile, L"D://Test Files//Bugs//");
}

void main()
{
	Test();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtDumpMemoryLeaks();
}
// XpsFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../XpsFile.h"

#include <vector>
#include <string>
#include "windows.h"

#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include <iostream>
#include <ctime>

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
void ConvertFolderToRaster(const std::wstring& wsFolderPath)
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	CXpsFile oReader(&oFonts);
	oReader.SetTempFolder(L"D:/Test Files/Temp/");

	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"xps");
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		std::wstring wsFilePathName = (wsFilePath.substr(0, wsFilePath.size() - 4));
		if (oReader.LoadFromFile(wsFilePath.c_str()))
		{
			int nPagesCount = oReader.GetPagesCount();

			for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
			{
				std::wstring wsDstFilePath = wsFilePathName + L"_" + std::to_wstring(nPageIndex) + L".png";
				oReader.ConvertToRaster(nPageIndex, wsDstFilePath.c_str(), 4);
				printf("%d of %d %S page %d / %d\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str(), nPageIndex, nPagesCount);
			}
			oReader.Close();
		}
		else
		{
			printf("%d of %d %S error\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
		}
	}
}
void ConvertFolderToPdf(const std::wstring& wsFolderPath)
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	CXpsFile oReader(&oFonts);
	oReader.SetTempFolder(L"D:/Test Files/Temp/");

	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"xps");
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		std::wstring wsFilePathName = (wsFilePath.substr(0, wsFilePath.size() - 4));
		if (oReader.LoadFromFile(wsFilePath.c_str()))
		{
			printf("%d of %d %S\n", nIndex + 1, vFiles.size(), vFiles.at(nIndex).c_str());
			std::wstring wsDstFilePath = wsFilePathName + L".pdf";
			oReader.ConvertToPdf(wsDstFilePath);
			oReader.Close();
		}
		else
		{
			printf("%d of %d %S error\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
		}
	}
}

void main()
{
	clock_t oBeginTime = clock();

	//ConvertFolderToRaster(L"D:/Test Files//Xps//");
	ConvertFolderToPdf(L"D:/Test Files//Xps//");

	clock_t oEndTime = clock();
	double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
	printf("%fseconds\n", dElapsedSecs);

	char q;
	std::cin >> q;
}

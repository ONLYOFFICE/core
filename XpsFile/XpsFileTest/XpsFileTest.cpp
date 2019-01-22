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
// XpsFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../XpsFile.h"

#include <vector>
#include <string>
#include "windows.h"

#include "../../DesktopEditor/common/StringExt.h"
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

	ConvertFolderToRaster(L"D:/Test Files//Xps//");
	ConvertFolderToPdf(L"D:/Test Files//Xps//");

	clock_t oEndTime = clock();
	double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
	printf("%fseconds\n", dElapsedSecs);

	//char q;
	//std::cin >> q;
}

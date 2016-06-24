/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
// DjVuFileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../DjVu.h"

#include <vector>
#include <string>
#include "windows.h"

#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include <ctime>
#include <iostream>

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
void ConvertFolderToRaster(CDjVuFile& oReader, std::wstring wsFolderPath)
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	oReader.Close();

	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"djvu");
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
				oReader.ConvertToRaster(&oFonts, nPageIndex, wsDstFilePath.c_str(), 4);
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
void ConvertFolderToPdf(CDjVuFile& oReader, std::wstring wsFolderPath)
{
	CApplicationFonts oFonts;
	oFonts.Initialize();
	oReader.Close();
	
	clock_t oBeginTime = clock();
	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"djvu");
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		std::wstring wsFilePathName = (wsFilePath.substr(0, wsFilePath.size() - 5));
		if (oReader.LoadFromFile(wsFilePath.c_str()))
		{
			std::wstring wsDstFilePath = wsFilePathName + L".pdf";
			oReader.ConvertToPdf(&oFonts, wsDstFilePath);
			printf("%d of %d converted\n", nIndex, vFiles.size());
		}
		else
		{
			printf("%d of %d %S error\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
		}
	}
	clock_t oEndTime = clock();
	double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
	printf("%f\n", dElapsedSecs);
}

void main()
{
	CDjVuFile oFile;
	//ConvertFolderToRaster(oFile, L"D:/Test Files//");
	ConvertFolderToPdf(oFile, L"D:/Test Files//djvu//");

	char q;
	std::cin >> q;
}

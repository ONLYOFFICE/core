/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
//#define _CRTDBG_LEAK_CHECK_DF

#include <vector>

#include "../../fontengine/ApplicationFonts.h"
#include "MetaFile.h"

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

	std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"png");
	if (oMetaFile.LoadFromFile(wsFilePath.c_str()))
	{
		oMetaFile.ConvertToRaster(wsDstFilePath.c_str(), 4, 1000);
	}
}
void ConvertFolder(CMetaFile &oMetaFile, std::wstring wsFolderPath, const int nType)
{
	oMetaFile.Close();

	std::wstring sExt;

	switch(nType)
	{
	case c_lMetaEmf: sExt = L"emf"; break;
	case c_lMetaWmf: sExt = L"wmf"; break;
	case c_lMetaSvm: sExt = L"svm"; break;
	}
	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath,  sExt);
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		if (oMetaFile.LoadFromFile(wsFilePath.c_str()))
		{
			std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"png");

			double w, h, x, y;
			oMetaFile.GetBounds(&x, &y, &w, &h);
			////oMetaFile.ConvertToRaster(wsDstFilePath.c_str(), 4, 500);
			oMetaFile.ConvertToRaster(wsDstFilePath.c_str(), 4, w);
			oMetaFile.Close();
		}

		printf("%d of %d %S\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
	}
}

void main()
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	CMetaFile oMetaFile(&oFonts);
	//ConvertFolder(oMetaFile, L"D://test//_svm//1//", c_lMetaSvm);
	//ConvertFolder(oMetaFile, L"D://Test Files//Wmf//Test//", c_lMetaWmf);
	ConvertFolder(oMetaFile, L"D://test//_emf//", c_lMetaEmf);

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtDumpMemoryLeaks();
}
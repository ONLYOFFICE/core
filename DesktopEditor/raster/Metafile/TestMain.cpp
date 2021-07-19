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
//#define _CRTDBG_LEAK_CHECK_DF

#include <vector>
#include <tchar.h>

#include "../../common/Directory.h"
#include "../../fontengine/ApplicationFonts.h"
#include "../../../HtmlRenderer/include/ASCSVGWriter.h"
#include "MetaFile.h"

#if defined(_WIN64)
#if defined(_DEBUG)
#pragma comment(lib, "../../../build/lib/win_64/DEBUG/kernel.lib")
#pragma comment(lib, "../../../build/lib/win_64/DEBUG/UnicodeConverter.lib")
#else
#pragma comment(lib, "../../../build/lib/win_64/kernel.lib")
#pragma comment(lib, "../../../build/lib/win_64/UnicodeConverter.lib")
#endif
#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)
#if defined(_DEBUG)
#pragma comment(lib, "../../../build/lib/win_32/DEBUG/kernel.lib")
#pragma comment(lib, "../../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
#else
#pragma comment(lib, "../../../build/lib/win_32/kernel.lib")
#pragma comment(lib, "../../../build/lib/win_32/UnicodeConverter.lib")
#endif
#pragma comment(lib, "../../../build/bin/icu/win_32/icuuc.lib")
#endif

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
void ConvertFileToRaster(MetaFile::IMetaFile *pMetaFile, std::wstring wsFilePath)
{
	pMetaFile->Close();

	std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"png");
	if (pMetaFile->LoadFromFile(wsFilePath.c_str()))
	{
		pMetaFile->ConvertToRaster(wsDstFilePath.c_str(), 4, 1000);
		
		pMetaFile->Close();
	}
}
//void ConvertFileToSVG(MetaFile::IMetaFile* pMetaFile, NSHtmlRenderer::CASCSVGWriter &oWriterSVG, std::wstring wsFilePath)
//{
//	pMetaFile->Close();
//
//	std::wstring wsDstFilePath = (wsFilePath.substr(0, wsFilePath.size() - 3)).append(L"svg");
//	
//	if (pMetaFile->LoadFromFile(wsFilePath.c_str()))
//	{
//		double x = 0, y = 0, w = 0, h = 0;
//		pMetaFile->GetBounds(&x, &y, &w, &h);
//
//		double _max = (w >= h) ? w : h;
//		double dKoef = 100000.0 / _max;
//
//		int WW = (int)(dKoef * w + 0.5);
//		int HH = (int)(dKoef * h + 0.5);
//
//		oWriterSVG.Reset();
//		oWriterSVG.put_Width(WW);
//		oWriterSVG.put_Height(HH);
//		
//		pMetaFile->DrawOnRenderer(&oWriterSVG, 0, 0, WW, HH);		
//		
//		oWriterSVG.SaveFile(wsDstFilePath);
//
//		pMetaFile->Close();
//	}
//}
void ConvertFolder(MetaFile::IMetaFile* pMetaFile, std::wstring wsFolderPath, const int nType)
{
	pMetaFile->Close();

	std::wstring sExt = L"*";

	//NSHtmlRenderer::CASCSVGWriter oWriterSVG;		
	//oWriterSVG.SetFontManager(pMetaFile->get_FontManager());

	switch(nType)
	{
		case MetaFile::c_lMetaEmf: sExt = L"emf"; break;
		case MetaFile::c_lMetaWmf: sExt = L"wmf"; break;
		case MetaFile::c_lMetaSvm: sExt = L"svm"; break;
	}
	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath,  sExt);
	
	for (size_t nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		
		//ConvertFileToSVG(pMetaFile, oWriterSVG, wsFilePath);
		ConvertFileToRaster(pMetaFile, wsFilePath);

		printf("%d of %d %S\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	NSFonts::IApplicationFonts *pFonts = NSFonts::NSApplication::Create();
	pFonts->Initialize();

	std::wstring	sMetafilesFolder	= L"D:\\tests\\_wmf\\";
	int				nType				= MetaFile::c_lMetaWmf;
	if (argc > 1)
		sMetafilesFolder = argv[1];

	if (argc > 2)
	{
		try
		{
			nType = _wtoi(argv[2]);
		}
		catch(...)
		{
		}
	}

	MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);

	if (NSFile::CFileBinary::Exists(sMetafilesFolder))
	{	
		//NSHtmlRenderer::CASCSVGWriter oWriterSVG;		
		//oWriterSVG.SetFontManager(pMetafile->get_FontManager());
		
		//ConvertFileToSVG(pMetafile, oWriterSVG, sMetafilesFolder);
		ConvertFileToRaster(pMetafile, sMetafilesFolder);
	}
	else if (NSDirectory::Exists(sMetafilesFolder))
	{
		ConvertFolder(pMetafile, sMetafilesFolder, nType);
	}

	return 0;
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtDumpMemoryLeaks();
}
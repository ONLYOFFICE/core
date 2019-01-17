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
// PdfReaderTest.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <string>
#include <iostream>
#include <tchar.h>

#include "../PdfReader.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/common/Directory.h"

#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

	#if defined(_DEBUG)
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/HtmlRenderer.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/CryptoPPLib.lib")
	#else
		#pragma comment(lib, "../../build/lib/win_32/HtmlRenderer.lib")
		#pragma comment(lib, "../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/kernel.lib")
		#pragma comment(lib, "../../build/lib/win_32/UnicodeConverter.lib")
		#pragma comment(lib, "../../build/lib/win_32/CryptoPPLib.lib")
	#endif
	#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif

HRESULT convert_single(PdfReader::CPdfReader& oReader, const std::wstring & srcFileName, const std::wstring & sPassword)
{
	if (oReader.LoadFromFile(srcFileName.c_str(), L"", sPassword, sPassword))
	{
		int nPagesCount = oReader.GetPagesCount();

		for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
		{			
			int nRasterWCur = 1000;
			int nRasterHCur = 1000;

			double dPageDpiX, dPageDpiY;
			double dWidth, dHeight;
			oReader.GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

			double dKoef1 = nRasterWCur / dWidth;
			double dKoef2 = nRasterHCur / dHeight;
			if (dKoef1 > dKoef2)
				dKoef1 = dKoef2;

			nRasterWCur = (int)(dWidth * dKoef1 + 0.5);
			nRasterHCur = (int)(dHeight * dKoef1 + 0.5);

			std::wstring wsFilePathName = NSFile::GetDirectoryName(srcFileName);
			std::wstring wsDstFilePath = srcFileName + L"_" + std::to_wstring(nPageIndex + 1) + L".png";

			oReader.ConvertToRaster(nPageIndex, wsDstFilePath.c_str(), 4, nRasterWCur, nRasterHCur);
		}
		oReader.Close();
		
		return S_OK;
	}
	
	return S_FALSE;
}
HRESULT convert_directory(PdfReader::CPdfReader& oReader, std::wstring pathName, const std::wstring & sPassword)
{
	HRESULT hr = S_OK;

	std::vector<std::wstring> arFiles = NSDirectory::GetFiles(pathName, false);

	for (size_t i = 0; i < arFiles.size(); i++)
	{
		hr = convert_single(oReader, arFiles[i], sPassword);

		printf("%d of %d %S \tresult = %d\n", i + 1, arFiles.size(), arFiles[i].c_str(), hr);

	}
	return S_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2) return 1;

	NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
	pApplicationFonts->Initialize();

	PdfReader::CPdfReader oPdfReader(pApplicationFonts);

	HRESULT hr = S_FALSE;

	std::wstring sPassword = argc > 2 ? argv[2] : L"";

	if (NSFile::CFileBinary::Exists(argv[1]))
	{	
		hr = convert_single(oPdfReader, argv[1], sPassword);
	}
	else if (NSDirectory::Exists(argv[1]))
	{
		hr = convert_directory(oPdfReader, argv[1], sPassword);
	}	
	return hr;
}


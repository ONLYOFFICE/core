/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include <iostream>
#include <vector>

#include "../Fb2File.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

void getDirectories(const std::wstring& sDirectory, std::vector<std::wstring>& arrDirectory)
{
	arrDirectory.push_back(sDirectory);
	for (const std::wstring& sT : NSDirectory::GetDirectories(sDirectory))
		getDirectories(sT, arrDirectory);
}

int main()
{
	bool bBatchMode = false;
	bool bFromHtml  = false;
	if (bBatchMode)
	{
		// Files directory
		std::wstring sDirectory = NSFile::GetProcessDirectory() + L"/../../../examples/fb2";
		// Subdirectories
		std::vector<std::wstring> arrDirectory;
		getDirectories(sDirectory, arrDirectory);

		// Conversion parameters
		CFb2Params oParams;
		oParams.bNeedDocx = false;
		oParams.bNeedContents = true;
		COfficeUtils oZip;
		// Set temporary directory
		std::wstring sTmp = NSFile::GetProcessDirectory() + L"/tmp";
		NSDirectory::DeleteDirectory(sTmp);
		NSDirectory::CreateDirectory(sTmp);

		int nErrorCol = 0;
		std::vector<std::wstring> arrError;

		for (std::wstring sD : arrDirectory)
		{
			if (bFromHtml)
			{
			}
			else
			{
				std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sD);

				// Directory where docx will be created
				size_t nPos = sD.find(L"/fb2");
				sD.insert(nPos + 4, L"-res");
				NSDirectory::DeleteDirectory(sD);
				NSDirectory::CreateDirectory(sD);

				for(const std::wstring& sFile : arrFiles)
				{
					CFb2File oFile;
					std::wstring sFileName = NSFile::GetFileName(sFile);
					std::wcout << sFileName << std::endl;
					if(!oFile.IsFb2File(sFile))
					{
						nErrorCol++;
						arrError.push_back(sFileName);
						std::cout << "This isn't a fb2 file" << std::endl;
						continue;
					}

					if(oFile.Open(sFile, sTmp, &oParams) == S_OK)
					{
						std::cout << "Success" << std::endl;
						oZip.CompressFileOrDirectory(sTmp, sD + L"/" + sFileName + L".docx");
						NSDirectory::DeleteDirectory(sTmp + L"/word/media");
					}
					else
					{
						nErrorCol++;
						arrError.push_back(sFileName);
						std::cout << "Failure" << std::endl;
					}
				}
			}
		}

		std::cout << "ERRORS - "<< nErrorCol << std::endl;
		for(const std::wstring& sError : arrError)
			std::wcout << sError << std::endl;
	}
	else
	{
		CFb2File oFile;

		// File to open
		std::wstring sFile = NSFile::GetProcessDirectory() + L"/res.fb2";

		// Directory where docx will be created
		std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
		NSDirectory::DeleteDirectory(sOutputDirectory);
		NSDirectory::CreateDirectory(sOutputDirectory);

		if (bFromHtml)
		{
			sFile = NSFile::GetProcessDirectory() + L"/test.html";
			oFile.FromHtml(sFile, sOutputDirectory + L"/res.fb2", L"Test Title");
			return 0;
		}

		if (!oFile.IsFb2File(sFile))
		{
			std::cout << "This isn't a fb2 file" << std::endl;
			return 1;
		}

		CFb2Params oParams;
		oParams.bNeedDocx = true;
		oParams.bNeedContents = false;

		std::cout << (oFile.Open(sFile, sOutputDirectory, &oParams) == S_OK ? "Success" : "Failure") << std::endl;
	}
	std::cout << "THE END" << std::endl;
	return 0;
}
